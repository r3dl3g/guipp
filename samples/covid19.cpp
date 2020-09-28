

#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/tile_view.h>
#include <gui/ctrl/table.h>
#include <gui/draw/diagram.h>
#include <gui/draw/bitmap.h>
#include <gui/core/grid.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/adaption_layout.h>
#include <logging/core.h>
#include <logging/file_logger.h>
#include <util/csv_reader.h>
#include <util/time_util.h>
#include <util/vector_util.h>
#include <testing/testing.h>

#include <numeric>
#include <chrono>
#include <fstream>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::ctrl;
using namespace gui::win;
using namespace gui::draw;
using namespace util;

// --------------------------------------------------------------------------
typedef diagram::point2d<std::time_t, double> point;
typedef core::range<std::time_t> time_range;

// --------------------------------------------------------------------------
struct timer {

  timer () {
    restart();
  }

  void restart () {
    begin = std::chrono::system_clock::now();
  }

  std::chrono::system_clock::duration duration () const {
    return std::chrono::system_clock::now() - begin;
  }

private:
  std::chrono::system_clock::time_point begin;
};

// --------------------------------------------------------------------------
namespace gui { namespace draw { namespace diagram {
  inline bool operator< (const point& lhs, const point& rhs) {
    return lhs.x < rhs.x;
  }
}}}

struct separate_thousands : std::numpunct<char> {
  char_type do_thousands_sep() const override { return '.'; }  // separate with commas
  string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};

// --------------------------------------------------------------------------
namespace std {

  inline std::ostream& operator<< (std::ostream& out, const point& pt) {
    out << "[";
    util::time::format_date(out, pt.x);
    out << ":" << pt.y << "]";
    return out;
  }

  inline std::ostream& operator<< (std::ostream& out, const time_range& r) {
    out << "[";
    util::time::format_date(out, r.begin());
    out << ":" ;
    util::time::format_date(out, r.end());
    out << "]";
    return out;
  }

  inline std::ostream& operator<< (std::ostream& out, const timer& t) {
    out << t.duration();
    return out;
  }

  inline ios_base& separat_k (ios_base& __base) {
    static std::locale __private_locale(__base.getloc(), new separate_thousands());
    __base.imbue(__private_locale);
    return __base;
  }
}

namespace calc {

  // --------------------------------------------------------------------------
  std::vector<point> accumulated (std::vector<point> v) {
    double prev = 0;
    for (auto& i : v) {
      prev += i.y;
      i.y = prev;
    }
    return v;
  }

  // --------------------------------------------------------------------------
  std::vector<point> rolling_mean (const std::vector<point>& v, int count) {
    std::vector<point> result;
    const auto sz = v.size();
    result.resize(sz);
    for (int i = 0; i < sz; ++i) {
      const int min = std::max(0, i - count) + 1;
      const int max = i + 1;
      const int diff = std::max(1, max - min);
      double sum = 0;
      for (int j = min; j < max; ++j) {
        sum += v[j].y;
      }
      result[i] = { v[i].x, sum / diff };
    }
    return result;
  }

  // --------------------------------------------------------------------------
  std::vector<point> ratio (const std::vector<point>& divident,
                            const std::vector<point>& divisor,
                            int offset) {
    if ((divisor.size() + offset) < divident.size()) {
      throw std::out_of_range("divisor list is to small!");
    }

    const auto sz = divident.size();
    std::vector<point> result;
    result.resize(sz - offset);
    for (int i = offset; i < sz; ++i) {
      const auto& divid = divident[i];
      const auto& divis = divisor[i - offset];
      const double div = divid.y / divis.y;
      result[i - offset] = { divid.x, std::isfinite(div) ? div : 1.0 };
    }
    return result;
  }

  // --------------------------------------------------------------------------
  std::vector<point> increase (std::vector<point> v) {
    double prev = 1;
    for (auto& i : v) {
      const double next = i.y;
      const double div = next / prev;
      i.y = std::isfinite(div) ? div - 1.0 : 0.0;
      prev = next;
    }
    return v;
  }

  // --------------------------------------------------------------------------
  std::vector<point> divide (std::vector<point> v, double divisor) {
    std::vector<point> result;
    const auto sz = v.size();
    result.resize(sz);
    for (int i = 0; i < sz; ++i) {
      result[i] = { v[i].x, (v[i].y / divisor) };
    }
    return result;
  }

  // --------------------------------------------------------------------------
  void add (std::vector<point>& v, const std::vector<point>& s) {
    if (v.empty()) {
      v = s;
    } else if (v.size() == s.size()) {
      for (std::size_t i = 0; i< s.size(); ++i) {
        v[i].y += s[i].y;
#ifdef DEBUG
        if (v[i].x != s[i].x) {
          clog::debug() << "X-Value differs (" << v[i].x << " != " << s[i].x << ") at " << i;
        }
#endif
      }
    }
  }

  // --------------------------------------------------------------------------
  void fill_up (std::vector<point>& v, const std::size_t count, const time_range& r) {
    if ((v.size() == count) && (v.front().x == r.begin()) && (v.back().x == r.end())) {
      return;
    } else {
      std::vector<point> result;
      result.resize(count);
      auto i = v.cbegin();
      auto o = result.begin();
      const auto ie = v.cend();
      const auto oe = result.end();
      std::time_t dt = r.begin();
      while (o < oe) {
        if (i < ie) {
          while ((i->x > dt) && (o < oe)) {
           *o++ = { dt, 0.0 };
            dt += 60*60*24;
          }
          if (o < oe) {
            dt = i->x + 60*60*24;
            *o++ = *i++;
          }
        } else {
          *o++ = { dt, 0.0 };
          dt += 60*60*24;
        }
      }
      v = result;
    }
  }

  // --------------------------------------------------------------------------
  typedef double (calculator) (double);
  std::vector<point> calc (std::vector<point> v, calculator c) {
    for (auto& i : v) {
      i.y = c(i.y);
    }
    return v;
  }

} // namespace calc
// --------------------------------------------------------------------------
struct country_data : public gui::ctrl::list_data_t<std::string> {

  struct country {
    country ()
      : population(0)
    {}

    std::vector<point> positives;
    std::vector<point> deaths;
    std::size_t population;
    std::string region;
    std::string name;
  };

  typedef std::map<std::string, country> map_type;
  typedef std::vector<std::string> list_type;

  std::string at (std::size_t i) const override {
    return countries[i];
  }

  std::size_t size () const override {
    return countries.size();
  }

  list_type countries;
  map_type data;
  time_range x_range;
};
// --------------------------------------------------------------------------
enum option {
  undedined = -1,
  absolute_increase = 0,
  increase_median_7 = 1,
  absolute_cumulated = 2,
  relative_increase = 3,
  relative_median_7_increase = 4,
  r_value = 5,
  lethality = 6,
  per_100k = 7,
  deads_per_100k = 8,
  options_count
};
// --------------------------------------------------------------------------
enum class chart_t {
  country_chart,
  cases_chart,
  r_value_chart,
  lethality_chart,
  per_100k_chart
};
// --------------------------------------------------------------------------
const std::array<std::string, options_count*2> heads = {
  "pos.inc.",
  "dead inc.",
  "pos./7",
  "dead/7.",
  "pos.cum.",
  "dead cum.",
  "pos.rel.",
  "dead rel.",
  "pos.r./7",
  "dead r./7",
  "r-val",
  "r-val/7",
  "lethal",
  "lethal/14",
  "per 100k"
  "per 100k/7"
};
// --------------------------------------------------------------------------
typedef layout_main_window<gui::layout::border::layouter<40, 0, 150, 0>> main_type;
// --------------------------------------------------------------------------
struct covid19main : public main_type {
  typedef main_type super;

  typedef std::map<std::size_t, pixmap> pixmap_map_t;

  covid19main ();

  void load_data (const sys_fs::path& p);

  void draw_uncached (std::size_t idx,
                      const draw::graphics&,
                      const core::rectangle& place,
                      const draw::brush& background,
                      item_state state) const;

  void draw_at (std::size_t idx,
                const draw::graphics&,
                const core::rectangle& place,
                const draw::brush& background,
                item_state state);

  std::size_t chart_count () const;

  void refresh ();
  void clear_cache ();

  void showChart (chart_t t);
  void showTable ();

  country_data data;
  country_data::country option_data[options_count];
  chart_t chart_type;

  vertical_list countries;
  layout::grid_adaption<7, 1> button_layout;
  text_button load_button;
  text_button table_button;
  text_button chart_button;
  text_button cases_button;
  text_button r_value_button;
  text_button lethality_button;
  text_button per100k_button;
  ctrl::vertical_tile_view charts;
  ctrl::table_view table;

  pixmap_map_t pixmap_cache;
};
// --------------------------------------------------------------------------
struct covid19data : public list_data {

  covid19data (covid19main* main)
    : main(main)
  {}

  std::size_t size () const override {
    return main->chart_count();
  }

  void draw_at (std::size_t idx,
                const draw::graphics& graph,
                const core::rectangle& place,
                const draw::brush& background,
                item_state state) const override {
    main->draw_at(idx, graph, place, background, state);
  }

  covid19main* main;
};
// --------------------------------------------------------------------------
template<typename X, typename Y, diagram::scaling SX, diagram::scaling SY>
void check_points (const diagram::scaler<X, SX>& sx,
                   const diagram::scaler<Y, SY>& sy,
                   const std::vector<point>& points) {
  for (auto& pt : points) {
    if (pt.x < sx.get_source().begin()) {
      clog::warn() << "Point " << pt << " x is lower than min (" << sx.get_source().begin() << ")";
    }
    if (pt.x > sx.get_source().end()) {
      clog::warn() << "Point " << pt << " x is greater than max (" << sx.get_source().end() << ")";
    }
    if (std::isinf(pt.y)) {
      clog::warn() << "Point " << pt << " y is inf";
    }
    if (std::isnan(pt.y)) {
      clog::warn() << "Point " << pt << " y is nan";
    }
    if (pt.y < sy.get_source().begin()) {
      clog::warn() << "Point " << pt << " y is lower than min (" << sy.get_source().begin() << ")";
    }
    if (pt.y > sy.get_source().end()) {
      clog::warn() << "Point " << pt << " y is greater than max (" << sy.get_source().end() << ")";
    }
  }
}
// --------------------------------------------------------------------------
static auto fmtx = [] (std::time_t i) {
  return util::time::format_date(i);
};
// --------------------------------------------------------------------------
static auto fmty = [] (double i) {
  return ostreamfmt(i);
};
// --------------------------------------------------------------------------
diagram::range_pair<std::time_t, double> get_mima (std::initializer_list<country_data::country> cs) {
  diagram::range_pair<std::time_t, double> r;
  for (auto& c : cs) {
    auto pmima = diagram::find_min_max_ignore_0<std::time_t, double>(c.positives);
    auto dmima = diagram::find_min_max_ignore_0<std::time_t, double>(c.deaths);
    if (r.first.empty() && r.second.empty()) {
      r = diagram::get_min_max(pmima, dmima);
    } else {
      r = diagram::get_min_max(diagram::get_min_max(pmima, r), diagram::get_min_max(dmima, r));
    }
  }
  return r;
  }
// --------------------------------------------------------------------------
template<diagram::scaling S>
void drawChart (const graphics& graph,
                const core::rectangle& area,
                const std::string& title,
                std::initializer_list<country_data::country> cs,
                std::initializer_list<std::string> legends);
// --------------------------------------------------------------------------
std::array<os::color, 6> colors = {
  color::light_red, color::light_green,
  color::light_blue, color::dark_blue,
  color::dark_red, color::dark_green,
};
// --------------------------------------------------------------------------
std::vector<diagram::legend_label> build_legend_labels ( std::initializer_list<std::string> labels) {
  std::vector<diagram::legend_label> legends;
  legends.reserve(labels.size());
  int i = 0;
  for (const auto& l : labels) {
    legends.emplace_back(colors[(i++) % 6], l);
  }
  return legends;
}
// --------------------------------------------------------------------------
template<>
void drawChart<diagram::scaling::linear> (const graphics& graph,
                                          const core::rectangle& area,
                                          const std::string& title,
                                          std::initializer_list<country_data::country> cs,
                                          std::initializer_list<std::string> legends) {
  auto mima = get_mima(cs);
  const auto xmima = mima.first;
  const auto ymima = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::linear>::calc(ymima.begin(), ymima.end());
  diagram::chart<std::time_t, double> d(area, xmima, l);
//  check_points(d.get_scale_x(), d.get_scale_y(), c.positives);
//  check_points(d.get_scale_x(), d.get_scale_y(), c.deaths);
  d.fill_area(graph);
  d.draw_xscale(graph, 60*60*24*61, 60*60*24*7, fmtx);
  const auto steps = diagram::next_smaller_pow10(std::max(std::abs(l.begin()), std::abs(l.end()))/3);
  d.draw_yscale(graph, steps, steps/5, fmty);
  int i = 0;
  for (auto& c : cs) {
    d.draw_line_graph(graph, c.positives, colors[(i++) % 6]);
    d.draw_line_graph(graph, c.deaths, colors[(i++) % 6]);
  }
  d.draw_axis(graph);
  d.draw_title(graph, title);
  d.draw_legend(graph, build_legend_labels(legends));
}
// --------------------------------------------------------------------------
template<>
void drawChart<diagram::scaling::log> (const graphics& graph,
                                       const core::rectangle& area,
                                       const std::string& title,
                                       std::initializer_list<country_data::country> cs,
                                       std::initializer_list<std::string> legends) {
  auto mima = get_mima(cs);
  const auto xmima = mima.first;
  const auto ymima = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::log>::calc(std::max(ymima.begin(), ymima.end()/1e6), ymima.end());
  diagram::chart<std::time_t, double, diagram::scaling::linear, diagram::scaling::log> d(area, xmima, l);
  d.fill_area(graph);
  d.draw_xscale(graph, 60*60*24*61, 60*60*24*7, fmtx);
  d.draw_yscale(graph, 1, 1, fmty);
  int i = 0;
  for (auto& c : cs) {
    d.draw_line_graph(graph, c.positives, colors[(i++) % 6]);
    d.draw_line_graph(graph, c.deaths, colors[(i++) % 6]);
  }
  d.draw_axis(graph);
  d.draw_title(graph, title);
  d.draw_legend(graph, build_legend_labels(legends));
}
// --------------------------------------------------------------------------
template<>
void drawChart<diagram::scaling::symlog> (const graphics& graph,
                                          const core::rectangle& area,
                                          const std::string& title,
                                          std::initializer_list<country_data::country> cs,
                                          std::initializer_list<std::string> legends) {
  auto mima = get_mima(cs);
  const auto xmima = mima.first;
  const auto ymima = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::symlog>::calc(std::max(ymima.begin(), ymima.end()/1e6), ymima.end());
  diagram::chart<std::time_t, double, diagram::scaling::linear, diagram::scaling::symlog> d(area, xmima, l);
  d.fill_area(graph);
  d.draw_xscale(graph, 60*60*24*61, 60*60*24*7, fmtx);
  d.draw_yscale(graph, 1, 1, fmty);
  int i = 0;
  for (auto& c : cs) {
    d.draw_line_graph(graph, c.positives, colors[(i++) % 6]);
    d.draw_line_graph(graph, c.deaths, colors[(i++) % 6]);
  }
  d.draw_axis(graph);
  d.draw_title(graph, title);
  d.draw_legend(graph, build_legend_labels(legends));
}
// --------------------------------------------------------------------------
std::string format_column (int i, double v) {
  if (i < 6) {
    return ostreamfmt(std::fixed << std::setprecision(0) << v);
  }
  if ((i > 9) && (i < 12)) {
    return ostreamfmt(std::fixed << std::setprecision(3) << v);
  }
  return ostreamfmt(std::fixed << std::setprecision(2) << (v * 100.0) << '%');
}
// --------------------------------------------------------------------------
covid19main::covid19main ()
  : chart_type(chart_t::country_chart)
  , countries(40, color::very_light_gray, false)
  , button_layout({layout::lay(load_button),
                  layout::lay(table_button),
                  layout::lay(chart_button),
                  layout::lay(cases_button),
                  layout::lay(r_value_button),
                  layout::lay(lethality_button),
                  layout::lay(per100k_button)})
  , table(64, 20)
{

  on_create([&] (window*, core::rectangle) {
//    countries.set_item_size(40);
    countries.create(*this);
    load_button.create(*this, "Load CSV");
    table_button.create(*this, "Show table");
    chart_button.create(*this, "Show chart");
    cases_button.create(*this, "Show cases");
    r_value_button.create(*this, "R-Values");
    lethality_button.create(*this, "Show lethality");
    per100k_button.create(*this, "Per 100k");

    charts.create(*this);
    table.create(*this);
    table.set_visible(false);

    get_layout().set_left(layout::lay(countries));
    get_layout().set_top(layout::lay(button_layout));
    get_layout().set_center(layout::lay(charts));

    set_children_visible();
  });

  load_button.on_clicked([&] () {
    file_open_dialog::show(*this, "Open CSV", "Ok", "Cancel", [&] (const sys_fs::path& p) {
      load_data(p);
      clear_cache();
      countries.set_data(data);
      countries.set_count();
    });
  });

  table_button.on_clicked([&] () {
    showTable();
  });

  chart_button.on_clicked([&] () {
    showChart(chart_t::country_chart);
  });

  cases_button.on_clicked([&] () {
    showChart(chart_t::cases_chart);
  });

  r_value_button.on_clicked([&] () {
    showChart(chart_t::r_value_chart);
  });

  lethality_button.on_clicked([&] () {
    showChart(chart_t::lethality_chart);
  });

  per100k_button.on_clicked([&] () {
    showChart(chart_t::per_100k_chart);
  });

  charts.on_size([&] (const core::size& sz) {
    const auto w = sz.width() - ctrl::scroll_bar::get_scroll_bar_width();
    const auto c = std::max(1.0, std::floor(w / 580.0));
    const auto i = std::max(320.0, std::floor(w / c));
    charts.set_item_size({ static_cast<core::size::type>(i), static_cast<core::size::type>(std::floor(i * 3.0 / 4.0)) });
    clear_cache();
  });
  charts.set_background(color::white);
//  charts.set_border({ 10, 10 });
//  charts.set_spacing({ 5, 5 });

  charts.set_data(covid19data{this});

  table.set_scroll_maximum_calcer([&](const core::size& size,
                                           const core::point&,
                                           const core::point&) {
    return core::point(table.geometrie.widths.position_of(14) +
                       table.geometrie.widths.get_offset() - size.width(),
                       table.geometrie.heights.position_of(option_data[absolute_increase].positives.size()) +
                       table.geometrie.heights.get_offset() + - size.height());
  });

  table.data.set_drawer(ctrl::table::default_data_drawer([&](const ctrl::table::position& cell) -> std::string {
    int id0 = cell.x() / 2;
    if (id0 < options_count) {
      const auto& d = option_data[id0];
      if (cell.x() % 2) {
        if (cell.y() < d.deaths.size()) {
          return format_column(cell.x(), d.deaths[cell.y()].y);
        }

      } else if (cell.y() < d.positives.size()) {
        return format_column(cell.x(), d.positives[cell.y()].y);
      }
    }
    return std::string();
  }));
  table.columns.set_drawer(ctrl::table::default_header_drawer([](const ctrl::table::position& cell) -> std::string {
    if (cell.x() < heads.size()) {
      return heads[cell.x()];
    }
    return std::string();
  }));
  table.rows.set_drawer(ctrl::table::default_header_drawer([&](const ctrl::table::position& cell) -> std::string {
    if (cell.y() < option_data[absolute_increase].positives.size()) {
      return util::time::format_date(option_data[absolute_increase].positives[cell.y()].x);
    }
    return std::string();
  }));

  countries.on_selection_changed([&] (event_source) {
    int sel = countries.get_selection();
    if (!countries.has_selection()) {
      return;
    }
    clear_cache();

    const auto& n = data.countries[sel];
    const auto& c = data.data.at(n);
    option_data[absolute_increase] = c;

//    timer stopwatch;
    option_data[absolute_cumulated].positives = calc::accumulated(c.positives);
    option_data[absolute_cumulated].deaths = calc::accumulated(c.deaths);
//    clog::info() << "Duration for accumulation: " << stopwatch;

    option_data[increase_median_7].positives = calc::rolling_mean(c.positives, 7);
    option_data[increase_median_7].deaths = calc::rolling_mean(c.deaths, 7);

    option_data[relative_increase].positives = calc::increase(option_data[absolute_cumulated].positives);
    option_data[relative_increase].deaths = calc::increase(option_data[absolute_cumulated].deaths);

    option_data[relative_median_7_increase].positives = calc::rolling_mean(option_data[relative_increase].positives, 7);
    option_data[relative_median_7_increase].deaths = calc::rolling_mean(option_data[relative_increase].deaths, 7);

    option_data[r_value].deaths = calc::rolling_mean(c.positives, 4);
    option_data[r_value].positives = calc::ratio(option_data[r_value].deaths, option_data[r_value].deaths, 4);
    option_data[r_value].deaths = calc::rolling_mean(option_data[r_value].positives, 7);

    option_data[lethality].positives = calc::ratio(option_data[absolute_cumulated].deaths, option_data[absolute_cumulated].positives, 0);
    option_data[lethality].deaths = calc::ratio(option_data[increase_median_7].deaths, option_data[increase_median_7].positives, 14);

    option_data[per_100k].positives = calc::divide(option_data[absolute_cumulated].positives, static_cast<double>(c.population) / 100000.0);
    option_data[per_100k].deaths = calc::divide(option_data[increase_median_7].positives, static_cast<double>(c.population) / 700000.0);

    option_data[deads_per_100k].positives = calc::divide(option_data[absolute_cumulated].deaths, static_cast<double>(c.population) / 100000.0);
    option_data[deads_per_100k].deaths = calc::divide(option_data[increase_median_7].deaths, static_cast<double>(c.population) / 700000.0);

    charts.set_count();
    table.data.invalidate();
    table.rows.invalidate();
  });

}
// --------------------------------------------------------------------------
void covid19main::refresh () {
  countries.set_data(data);
  countries.set_count();
}
// --------------------------------------------------------------------------
void covid19main::clear_cache () {
  clog::debug() << "Clear pixmap cache";
  pixmap_cache.clear();
}
// --------------------------------------------------------------------------
void covid19main::showChart (chart_t t) {
  charts.set_visible(true);
  table.set_visible(false);
  get_layout().set_center(layout::lay(charts));
  layout();
  if (chart_type != t) {
    clear_cache();
  }
  chart_type = t;
  charts.set_count();
}
// --------------------------------------------------------------------------
void covid19main::showTable () {
  charts.set_visible(false);
  table.set_visible(true);
  get_layout().set_center(layout::lay(table));
  layout();
  chart_type = chart_t::country_chart;
  charts.set_count();
}
// --------------------------------------------------------------------------
std::size_t covid19main::chart_count () const {
  switch (chart_type) {
    case chart_t::country_chart:
      return option_data[absolute_increase].positives.empty() ? 0 : options_count - 1;
    case chart_t::cases_chart:
    case chart_t::r_value_chart:
    case chart_t::lethality_chart:
    case chart_t::per_100k_chart:
      return data.countries.size();
    default:
      return 0;
  }
}
// --------------------------------------------------------------------------
void covid19main::draw_at (std::size_t idx,
                           const draw::graphics& graph,
                           const core::rectangle& area,
                           const draw::brush& background,
                           item_state state) {
  const auto i = pixmap_cache.find(idx);
  if (i != pixmap_cache.end()) {
    const auto& px = i->second;
    if (px.scaled_size() == area.size()) {
      clog::debug() << "Draw " << idx << " from cache";
      graph.copy_from(i->second, area.top_left());
      return;
    } else {
      clear_cache();
    }
  }

  draw::pixmap px(area.size());
  draw::graphics g(px);
  draw_uncached(idx, g, core::rectangle(area.size()), background, state);
  graph.copy_from(px, area.top_left());
  clog::debug() << "Insert " << idx << " into cache";
  pixmap_cache[idx] = std::move(px);
}
// --------------------------------------------------------------------------
void covid19main::draw_uncached (std::size_t idx,
                                 const draw::graphics& graph,
                                 const core::rectangle& area,
                                 const draw::brush& background,
                                 item_state state) const {
  //  auto inner = draw::frame::sunken_relief(graph, area);
  graph.fill(draw::rectangle(area), background);
  switch (chart_type) {
    case chart_t::country_chart:
      switch (idx) {
        case 0:
          drawChart<diagram::scaling::linear>(graph, area, "Increase positives/deads",
          {option_data[absolute_increase], option_data[increase_median_7]},
          {"positive", "dead", "positive/7-day", "dead/7-day"});
          break;
        case 1:
          drawChart<diagram::scaling::log>(graph, area, "Logarithmic Increase pos./deads",
          {option_data[absolute_increase], option_data[increase_median_7]},
          {"positive", "dead", "positive/7-day", "dead/7-day"});
          break;
        case 2:
          drawChart<diagram::scaling::log>(graph, area, "Cumulated positives/deads",
          {option_data[absolute_cumulated]},
          {"positive", "dead"});
          break;
        case 3:
          drawChart<diagram::scaling::log>(graph, area, "Logarithmic relative Inncrease",
          {option_data[relative_increase], option_data[relative_median_7_increase]},
          {"positive", "dead", "positive/7-day", "dead/7-day"});
          break;
        case 4:
          drawChart<diagram::scaling::log>(graph, area, "R-Value",
          {option_data[r_value]},
          {"R-Value", "R-Value/7-day"});
          break;
        case 5:
          drawChart<diagram::scaling::log>(graph, area, "Lethality",
          {option_data[lethality]},
          {"Deads/positives", "Deads/positives/14-day"});
          break;
        case 6:
          drawChart<diagram::scaling::log>(graph, area, "Per 100.000",
          {option_data[per_100k], option_data[deads_per_100k]},
          {"per 100k cumulated", "per 100k last 7 days", "deads per 100k cumulated", "deads per 100k last 7 days"});
          break;
      }
      break;
    case chart_t::cases_chart: {
      const auto& n = data.countries[idx];
      const auto& c = data.data.at(n);

      country_data::country ctry;
      ctry.positives = calc::rolling_mean(c.positives, 7);
      ctry.deaths = calc::rolling_mean(c.deaths, 7);

      drawChart<diagram::scaling::log>(graph, area, n, {ctry}, {"positive", "dead"});
      break;
    }
    case chart_t::r_value_chart: {
      const auto& n = data.countries[idx];
      const auto& c = data.data.at(n);

      auto med_pos = calc::rolling_mean(c.positives, 4);

      country_data::country ctry;
      ctry.positives = calc::ratio(med_pos, med_pos, 4);
      ctry.deaths = calc::rolling_mean(ctry.positives, 7);

      drawChart<diagram::scaling::log>(graph, area, n, {ctry}, {"R-Value", "R-Value/7-day"});
      break;
    }
    case chart_t::lethality_chart: {
      const auto& n = data.countries[idx];
      const auto& c = data.data.at(n);

      auto acc_pos = calc::accumulated(c.positives);
      auto acc_dea = calc::accumulated(c.deaths);
      auto med_pos = calc::rolling_mean(c.positives, 7);
      auto med_dea = calc::rolling_mean(c.deaths, 7);

      country_data::country ctry;
      ctry.positives = calc::ratio(acc_dea, acc_pos, 0);
      ctry.deaths = calc::ratio(med_dea, med_pos, 14);

      drawChart<diagram::scaling::log>(graph, area, n,
      {ctry}, {"Deads/positives", "Deads/positives/14-day"});
      break;
    }
    case chart_t::per_100k_chart: {
      const auto& n = data.countries[idx];
      const auto& c = data.data.at(n);

      auto acc_pos = calc::accumulated(c.positives);
      auto med_pos = calc::rolling_mean(c.positives, 7);
      auto acc_dea = calc::accumulated(c.deaths);
      auto med_dea = calc::rolling_mean(c.deaths, 7);

      country_data::country pos, deads;
      pos.positives = calc::divide(acc_pos, c.population / 100000.0);
      pos.deaths = calc::divide(med_pos, c.population / 700000.0);
      deads.positives = calc::divide(acc_dea, c.population / 100000.0);
      deads.deaths = calc::divide(med_dea, c.population / 700000.0);

      drawChart<diagram::scaling::log>(graph, area, ostreamfmt(n << " [" << std::separat_k << c.population << "]"),
      {pos, deads}, {"per 100k cumulated", "per 100k last 7 days", "deads per 100k cumulated", "deads per 100k last 7 days"});
      break;
    }
  }
}
// --------------------------------------------------------------------------
typedef csv::tuple_reader<std::string, int, int, int, int, int, std::string, std::string, std::string, std::size_t, std::string, std::string> covid19reader;
// --------------------------------------------------------------------------
void covid19main::load_data (const sys_fs::path& p) {
  using namespace util;

  data.data.clear();
  data.countries.clear();

  clog::info() << "Load CSV data from '" << p << "'";
  timer stopwatch;
  std::ifstream in(p);

  if (!in.is_open()) {
    clog::warn() << "Could not open file '" << p << "'";
  }

  covid19reader::read_csv(in, ',', true, [&] (const covid19reader::tuple& t) {
//    clog::info() << "Read tuple:" << t;

    const auto x = time::tm2time_t(time::mktm(std::get<3>(t), std::get<2>(t), std::get<1>(t)));

    static time_range null_range(0);
    if (data.x_range == null_range) {
      data.x_range = { x, x };
    } else {
      data.x_range = core::min_max( data.x_range, { x, x });
    }

    auto &n = std::get<6>(t);
    auto i = data.data.find(n);
    if (i == data.data.end()) {
      auto p = data.data.insert(std::make_pair(n, country_data::country()));
      i = p.first;
      i->second.name = n;
      i->second.region = std::get<10>(t);
      i->second.population = std::get<9>(t);
    }
    auto& country = *i;
    country.second.positives.push_back({x, static_cast<double>(std::get<4>(t))});
    country.second.deaths.push_back({x, static_cast<double>(std::get<5>(t))});
  });

//  csv::reader(',', true).read_csv_data(in, [&] (const csv::reader::string_list& l) {
//    if (l.size() < 11) {
//      return;
//    }
//    // 0:dateRep, 1:day, 2:month, 3:year, 4:cases, 5:deaths, 6:countriesAndTerritories, 7:geoId, 8:countryterritoryCode, 9:popData2019, 10:continentExp, 11:Cumulative_number_for_14_days_of_COVID-19_cases_per_100000
//    const auto x = time::tm2time_t(time::mktm(util::string::convert::to<int>(l[3]), util::string::convert::to<int>(l[2]), util::string::convert::to<int>(l[1])));
//    const double p = util::string::convert::to<int>(l[4]);
//    const double d = util::string::convert::to<int>(l[5]);
//    auto i = data.data.find(l[6]);
//    if (i == data.data.end()) {
//      auto p = data.data.insert(std::make_pair(l[6], country_data::country()));
//      i = p.first;
//      i->second.region = l[10];
//      i->second.population = util::string::convert::to<std::size_t>(l[9]);
//    }
//    auto& c = *i;
//    c.second.positives.push_back({x, p});
//    c.second.deaths.push_back({x, d});
//  });

  clog::info() << "Duration for parsing: " << stopwatch;

  clog::info() << "Inspected range: " << data.x_range;
  std::size_t count = data.x_range.size() / (60*60*24) + 1;
  country_data::country world;
  country_data::map_type regions;
  for (auto& i : data.data) {
    country_data::country& cntry = i.second;

    std::sort(cntry.positives.begin(), cntry.positives.end());
    std::sort(cntry.deaths.begin(), cntry.deaths.end());

#ifdef DEBUG
    if (cntry.name == "San_Marino") {
      clog::debug() << "Fill up " << cntry.name;
    }
#endif

    calc::fill_up(cntry.positives, count, data.x_range);
    calc::fill_up(cntry.deaths, count, data.x_range);

//    clog::debug() << "Add " << cntry.name << " to world";
    calc::add(world.positives, cntry.positives);
    calc::add(world.deaths, cntry.deaths);

    world.population += cntry.population;

    if (cntry.region != "Other") {
      auto& region = regions["- " + cntry.region];

//    clog::debug() << "Add " << cntry.name << " to region - " << cntry.region;
      calc::add(region.positives, cntry.positives);
      calc::add(region.deaths, cntry.deaths);

      region.population += cntry.population;
    }
  }

  data.data.insert(std::make_move_iterator(begin(regions)),
                   std::make_move_iterator(end(regions)));
  data.data.insert(std::make_pair("- World", std::move(world)));

  std::transform(std::begin(data.data), std::end(data.data), std::back_inserter(data.countries),
                 [](country_data::map_type::value_type const& pair) { return pair.first; });
}
// --------------------------------------------------------------------------
void test_fill_up () {
  time_range r(time::tm2time_t(time::mktm(2020, 1, 1)), time::tm2time_t(time::mktm(2020, 1, 31)));
  std::vector<point> test0;
  calc::fill_up(test0, 31, r);

  EXPECT_EQUAL(test0.size(), 31);
  EXPECT_EQUAL(test0.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test0[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test0[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test0.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test1 = {{ time::tm2time_t(time::mktm(2020, 1, 1)), 1.0 }};
  calc::fill_up(test1, 31, r);

  EXPECT_EQUAL(test1.size(), 31);
  EXPECT_EQUAL(test1.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test1[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test1[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test1.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test2 = {{ time::tm2time_t(time::mktm(2020, 1, 31)), 1.0 }};
  calc::fill_up(test2, 31, r);

  EXPECT_EQUAL(test2.size(), 31);
  EXPECT_EQUAL(test2.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test2[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test2[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test2.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test3 = {{ time::tm2time_t(time::mktm(2020, 1, 15)), 1.0 }};
  calc::fill_up(test3, 31, r);

  EXPECT_EQUAL(test3.size(), 31);
  EXPECT_EQUAL(test3.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test3[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test3[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test3.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test4 = {{ time::tm2time_t(time::mktm(2020, 1, 1)), 1.0 },
                              { time::tm2time_t(time::mktm(2020, 1, 15)), 1.0 }};
  calc::fill_up(test4, 31, r);

  EXPECT_EQUAL(test4.size(), 31);
  EXPECT_EQUAL(test4.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test4[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test4[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test4.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test5 = {{ time::tm2time_t(time::mktm(2020, 1, 15)), 1.0 },
                              { time::tm2time_t(time::mktm(2020, 1, 31)), 1.0 }};
  calc::fill_up(test5, 31, r);

  EXPECT_EQUAL(test5.size(), 31);
  EXPECT_EQUAL(test5.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test5[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test5[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test5.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  std::vector<point> test6 = {{ time::tm2time_t(time::mktm(2020, 1, 1)), 1.0 },
                              { time::tm2time_t(time::mktm(2020, 1, 15)), 1.0 },
                              { time::tm2time_t(time::mktm(2020, 1, 31)), 1.0 }};
  calc::fill_up(test6, 31, r);

  EXPECT_EQUAL(test6.size(), 31);
  EXPECT_EQUAL(test6.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test6[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test6[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test6.back().x, (time::tm2time_t(time::mktm(2020, 1, 31))));

  time_range r2(time::tm2time_t(time::mktm(2020, 1, 1)), time::tm2time_t(time::mktm(2020, 31, 31)));
  std::vector<point> test7 = {{ time::tm2time_t(time::mktm(2020, 3, 1)), 1.0 }};
  calc::fill_up(test7, 31+29+31, r2);

  EXPECT_EQUAL(test7.size(), 31+29+31);
  EXPECT_EQUAL(test7.front().x, (time::tm2time_t(time::mktm(2020, 1, 1))));
  EXPECT_EQUAL(test7[9].x, (time::tm2time_t(time::mktm(2020, 1, 10))));
  EXPECT_EQUAL(test7[19].x, (time::tm2time_t(time::mktm(2020, 1, 20))));
  EXPECT_EQUAL(test7.back().x, (time::tm2time_t(time::mktm(2020, 3, 31))));
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  logging::file_logger l("/tmp/covid19.log", logging::level::debug, logging::core::get_standard_formatter());

  clog::info() << "Current working dir:" << sys_fs::current_path();
//  clog::debug() << util::time::format_date(1583190000) << " != " << util::time::format_date(1583276400);
//  return 0;

//  test_fill_up();

  covid19main main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Covid-19");
  main.set_visible();

  if (args.size() > 1) {
    win::run_on_main([&] () {
      main.load_data(args[1]);
      main.refresh();
    });
  }

  return run_main_loop();
}
