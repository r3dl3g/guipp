

#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/tile_view.h>
#include <gui/draw/diagram.h>
#include <gui/core/grid.h>
#include <gui/layout/grid_layout.h>
#include <logging/core.h>
#include <util/csv_reader.h>
#include <util/string_util.h>
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

namespace gui { namespace draw { namespace diagram {
  inline bool operator< (const point& lhs, const point& rhs) {
    return lhs.x < rhs.x;
  }
}}}

namespace std {
  std::ostream& operator<< (std::ostream& out, const point& pt) {
    out << "[";
    util::time::format_date(out, pt.x);
    out << ":" << pt.y << "]";
    return out;
  }
}

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
std::vector<point> mean (const std::vector<point>& v, int count) {
  std::vector<point> result;
  const int sz = v.size();
  result.resize(sz);
  for (int i = 0; i < sz; ++i) {
    const int min = std::max(0, i - count + 1);
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

  const int sz = divident.size();
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
void add (std::vector<point>& v, const std::vector<point>& s) {
  if (v.size() < s.size()) {
    v = s;
  } else {
    for (std::size_t i = 0; i< s.size(); ++i) {
      v[i].y += s[i].y;
    }
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
// --------------------------------------------------------------------------
struct country_data : public gui::ctrl::list_data_t<std::string> {

  struct country {
    std::vector<point> positives;
    std::vector<point> deaths;
    std::size_t population;
    std::string region;
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
  options_count
};
// --------------------------------------------------------------------------
const std::string heads[15] = {
  "date",
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
  "lethal/14"
};
// --------------------------------------------------------------------------
typedef ctrl::column_list_t<layout::weight_column_list_layout, std::time_t, double, double, double, double, double, double, double, double, double, double, double, double, double, double> data_column_list_t;
typedef layout_main_window<gui::layout::border::layouter<25, 0, 150, 20, layout::border::type_t::left_right_maximize>> main_type;
// --------------------------------------------------------------------------
struct covid19main : public main_type {
  typedef main_type super;

  covid19main ();

  void load_data (const sys_fs::path& p);
  void initOption (int idx);
  void initSwitch (int idx);

  void draw_at (std::size_t idx,
                const draw::graphics&,
                const core::rectangle& place,
                const draw::brush& background,
                item_state state) const;

  country_data data;

  country_data::country option_data[options_count];

  vertical_list countries;
  text_button load_button;
  text_button tab_button;
  ctrl::vertical_tile_view charts;
  data_column_list_t table;
};
// --------------------------------------------------------------------------
struct covid19data : public list_data {

  covid19data (covid19main* main)
    : main(main)
  {}

  std::size_t size () const override {
    return main->option_data[absolute_increase].positives.empty() ? 0 : 7;
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
void time_drawer (const std::time_t& t,
                  const draw::graphics& graph,
                  const core::rectangle& place,
                  const draw::brush& background,
                  item_state state,
                  text_origin_t align) {
  ctrl::paint::text_item(graph, place, background, util::time::format_date(t), state, align);
  if (item_state::selected != state) {
    draw::frame::sunken_relief(graph, place);
  }
}
// --------------------------------------------------------------------------
void double_drawer (const double& t,
                  const draw::graphics& graph,
                  const core::rectangle& place,
                  const draw::brush& background,
                  item_state state,
                  text_origin_t align) {
  ctrl::paint::text_item(graph, place, background, ostreamfmt(std::fixed << std::setprecision(3) << t), state, align);
  if (item_state::selected != state) {
    draw::frame::sunken_relief(graph, place);
  }
}
// --------------------------------------------------------------------------
void int_drawer (const double& t,
                 const draw::graphics& graph,
                 const core::rectangle& place,
                 const draw::brush& background,
                 item_state state,
                 text_origin_t align) {
  ctrl::paint::text_item(graph, place, background, ostreamfmt(std::fixed << std::setprecision(0) << t), state, align);
  if (item_state::selected != state) {
    draw::frame::sunken_relief(graph, place);
  }
}
// --------------------------------------------------------------------------
void percent_drawer (const double& t,
                     const draw::graphics& graph,
                     const core::rectangle& place,
                     const draw::brush& background,
                     item_state state,
                     text_origin_t align) {
  ctrl::paint::text_item(graph, place, background, ostreamfmt(std::fixed << std::setprecision(2) << (t * 100.0) << '%'), state, align);
  if (item_state::selected != state) {
    draw::frame::sunken_relief(graph, place);
  }
}
// --------------------------------------------------------------------------
typedef ctrl::column_list_data_t<std::time_t, double, double, double, double, double, double, double, double, double, double, double, double, double, double> data_column_list_base;
struct data_column_list_data : public data_column_list_base {
  typedef data_column_list_base super;

  data_column_list_data (covid19main* main)
    : super(time_drawer,
            int_drawer, int_drawer,
            int_drawer, int_drawer,
            int_drawer, int_drawer,
            double_drawer, double_drawer,
            double_drawer, double_drawer,
            double_drawer, double_drawer,
            percent_drawer, percent_drawer)
    , main(main)
  {}

  std::size_t size () const override {
    return main->option_data[absolute_increase].positives.size();
  }

  row_type at (std::size_t i) const override {
    const auto& d = main->option_data;
    return std::make_tuple(d[absolute_increase].positives[i].x,
                           d[absolute_increase].positives[i].y, d[absolute_increase].deaths[i].y,
                           d[increase_median_7].positives[i].y, d[increase_median_7].deaths[i].y,
                           d[absolute_cumulated].positives[i].y, d[absolute_cumulated].deaths[i].y,
                           d[relative_increase].positives[i].y, d[relative_increase].deaths[i].y,
                           d[relative_median_7_increase].positives[i].y, d[relative_median_7_increase].deaths[i].y,
                           d[r_value].positives[i].y, d[r_value].deaths[i].y,
                           d[lethality].positives[i].y, d[lethality].deaths[i].y);
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
    auto pmima = diagram::find_min_max<std::time_t, double>(c.positives);
    auto dmima = diagram::find_min_max<std::time_t, double>(c.deaths);
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
os::color colors[] = {color::light_red, color::light_green,
                      color::dark_red, color::dark_green,
                      color::light_blue, color::dark_blue};
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
  const auto ymiax = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::linear>::calc(ymiax.begin(), ymiax.end());
  diagram::chart<std::time_t, double> d(area, xmima, l);
//  check_points(d.get_scale_x(), d.get_scale_y(), c.positives);
//  check_points(d.get_scale_x(), d.get_scale_y(), c.deaths);
  d.fill_area(graph);
  d.draw_xscale(graph, 60.0*60*24*61, 60.0*60*24*7, fmtx);
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
  const auto ymiax = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::log>::calc(ymiax.end() / 100000.0, ymiax.end());
  diagram::chart<std::time_t, double, diagram::scaling::linear, diagram::scaling::log> d(area, xmima, l);
  d.fill_area(graph);
  d.draw_xscale(graph, 60.0*60*24*61, 60.0*60*24*7, fmtx);
  d.draw_yscale(graph, 1, 1, fmty);
  for (auto& c : cs) {
    d.draw_line_graph(graph, c.positives, color::light_red);
    d.draw_line_graph(graph, c.deaths, color::light_green);
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
  const auto ymiax = mima.second;

  const auto l = diagram::limits<double, diagram::scaling::symlog>::calc(ymiax.end() / 100000.0, ymiax.end());
  diagram::chart<std::time_t, double, diagram::scaling::linear, diagram::scaling::symlog> d(area, xmima, l);
  d.fill_area(graph);
  d.draw_xscale(graph, 60.0*60*24*61, 60.0*60*24*7, fmtx);
  d.draw_yscale(graph, 1, 1, fmty);
  for (auto& c : cs) {
    d.draw_line_graph(graph, c.positives, color::light_red);
    d.draw_line_graph(graph, c.deaths, color::light_green);
  }
  d.draw_axis(graph);
  d.draw_title(graph, title);
  d.draw_legend(graph, build_legend_labels(legends));
}
// --------------------------------------------------------------------------
covid19main::covid19main () {

  on_create([&] (window*, core::rectangle) {
    countries.create(*this);
    load_button.create(*this, "Load CSV");
    charts.create(*this);
    tab_button.create(*this, "<");
    table.create(*this);
    table.set_visible(false);

    get_layout().set_left(layout::lay(countries));
    get_layout().set_top(layout::lay(load_button));
    get_layout().set_center(layout::lay(charts));
    get_layout().set_right(layout::lay(tab_button));

    set_children_visible();
  });

  load_button.on_clicked([&] () {
    file_open_dialog::show(*this, "Open CSV", "Ok", "Cancel", [&] (const sys_fs::path& p) {
      load_data(p);
      countries.set_data(data);
      countries.set_count();
    });
  });

  tab_button.on_clicked([&] () {
    if (charts.is_visible()) {
      table.set_visible(true);
      charts.set_visible(false);
      get_layout().set_center(layout::lay(table));
      layout();
      tab_button.set_text(">");
    } else {
      charts.set_visible(true);
      table.set_visible(false);
      get_layout().set_center(layout::lay(charts));
      layout();
      tab_button.set_text("<");
    }
  });

  charts.set_item_size({ 575, 400 });
  charts.set_background(color::white);
//  charts.set_border({ 10, 10 });
//  charts.set_spacing({ 5, 5 });

  charts.set_data(covid19data{this});

  auto weight_columns = {
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F },
    layout::weight_column_info{ 30, text_origin_t::vcenter_left, 20, 1.0F/14.0F }
  };

  table.header.set_cell_drawer([] (std::size_t i, const draw::graphics& g,
                               const core::rectangle& r, const draw::brush& background) {
    using namespace draw;
    g.fill(rectangle(r), background);
    frame::raised_deep_relief(g, r);
    g.text(text_box(heads[i], r, text_origin_t::center), font::system(), color::windowTextColor());
  });

  table.get_column_layout().set_columns(weight_columns);
  table.set_data(data_column_list_data(this));
  table.list.set_count();


  countries.on_selection_changed([&] (event_source) {
    int sel = countries.get_selection();
    const auto& n = data.countries[sel];
    option_data[absolute_increase] = data.data[n];

    option_data[absolute_cumulated].positives = accumulated(option_data[absolute_increase].positives);
    option_data[absolute_cumulated].deaths = accumulated(option_data[absolute_increase].deaths);

    option_data[increase_median_7].positives = mean(option_data[absolute_increase].positives, 7);
    option_data[increase_median_7].deaths = mean(option_data[absolute_increase].deaths, 7);

    option_data[relative_increase].positives = increase(option_data[absolute_increase].positives);
    option_data[relative_increase].deaths = increase(option_data[absolute_increase].deaths);

    option_data[relative_median_7_increase].positives = increase(option_data[increase_median_7].positives);
    option_data[relative_median_7_increase].deaths = increase(option_data[increase_median_7].deaths);

    option_data[r_value].deaths = mean(option_data[absolute_increase].positives, 4);
    option_data[r_value].positives = ::ratio(option_data[r_value].deaths, option_data[r_value].deaths, 4);
    option_data[r_value].deaths = mean(option_data[r_value].positives, 7);

    option_data[lethality].positives = ::ratio(option_data[absolute_cumulated].deaths, option_data[absolute_cumulated].positives, 0);
    option_data[lethality].deaths = ::ratio(option_data[increase_median_7].deaths, option_data[increase_median_7].positives, 14);

    charts.set_count();
    table.list.set_count();
  });

}
// --------------------------------------------------------------------------
void covid19main::draw_at (std::size_t idx,
                           const draw::graphics& graph,
                           const core::rectangle& area,
                           const draw::brush& background,
                           item_state state) const {
//  auto inner = draw::frame::sunken_relief(graph, area);
  graph.fill(draw::rectangle(area), background);
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
      drawChart<diagram::scaling::symlog>(graph, area, "Logarithmic relative Inncrease",
        {option_data[relative_increase]},
        {"positive", "dead"});
      break;
    case 4:
      drawChart<diagram::scaling::symlog>(graph, area, "Logarithmic relative Inncrease",
        {option_data[relative_median_7_increase]},
        {"positive", "dead"});
      break;
    case 5:
      drawChart<diagram::scaling::linear>(graph, area, "R-Value",
        {option_data[r_value]},
        {"R-Value", "R-Value/7-day"});
      break;
    case 6:
      drawChart<diagram::scaling::log>(graph, area, "Lethality",
        {option_data[lethality]},
        {"Deads/positives", "Deads/positives/14-day"});
      break;
  }

}
// --------------------------------------------------------------------------
void covid19main::load_data (const sys_fs::path& p) {
  using namespace util;

  data.data.clear();
  data.countries.clear();

  std::ifstream in(p);
  csv::reader(',', true).read_csv_data(in, [&] (const csv::reader::string_list& l) {
    // 0:ateRep, 1:day, 2:month, 3:year, 4:cases, 5:deaths, 6:countriesAndTerritories
    // 0:dateRep, 1:day, 2:month, 3:year, 4:cases, 5:deaths, 6:countriesAndTerritories, 7:geoId, 8:countryterritoryCode, 9:popData2019, 10:continentExp, 11:Cumulative_number_for_14_days_of_COVID-19_cases_per_100000
    const auto x = time::tm2time_t(time::mktm(util::string::convert::to<int>(l[3]), util::string::convert::to<int>(l[2]), util::string::convert::to<int>(l[1])));
    const double p = util::string::convert::to<int>(l[4]);
    const double d = util::string::convert::to<int>(l[5]);
    auto i = data.data.find(l[6]);
    if (i == data.data.end()) {
      auto p = data.data.insert(std::make_pair(l[6], country_data::country()));
      i = p.first;
      i->second.region = l[10];
      i->second.population = util::string::convert::to<std::size_t>(l[9]);
    }
    auto& c = *i;
    c.second.positives.push_back({x, p});
    c.second.deaths.push_back({x, d});
  });

  country_data::country world;
  country_data::map_type regions;
  for (auto& i : data.data) {
    std::sort(i.second.positives.begin(), i.second.positives.end());
    std::sort(i.second.deaths.begin(), i.second.deaths.end());
    add(world.positives, i.second.positives);
    add(world.deaths, i.second.deaths);
    auto& region = regions[" " + i.second.region];
    add(region.positives, i.second.positives);
    add(region.deaths, i.second.deaths);
  }

  data.data.insert(std::make_move_iterator(begin(regions)),
                   std::make_move_iterator(end(regions)));
  data.data.insert(std::make_pair(" World", std::move(world)));

  std::transform(std::begin(data.data), std::end(data.data), std::back_inserter(data.countries),
                 [](country_data::map_type::value_type const& pair) { return pair.first; });
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  covid19main main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Covid-19");
  main.set_visible();

  return run_main_loop();
}