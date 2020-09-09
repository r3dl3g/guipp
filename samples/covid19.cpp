

#include <gui/ctrl/std_dialogs.h>
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
struct point {
  std::time_t x;
  double y;

  bool operator< (const point& rhs) const {
    return x < rhs.x;
  }
};

// --------------------------------------------------------------------------
namespace gui { namespace draw { namespace diagram {
  template<typename T>
  struct get<T, point> {
    inline static T x (const point& p) {
      return p.x;
    }

    inline static T y (const point& p) {
      return p.y;
    }
  };
}}}

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
  result.resize(sz);
  for (int i = offset; i < sz; ++i) {
    const auto& divid = divident[i];
    const auto& divis = divisor[i - offset];
    const double div = divid.y / divis.y;
    result[i] = { divid.x, std::isfinite(div) ? div : 1.0 };
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
template<typename T>
T get_prev_pow10 (T t) {
  return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(10.0, std::floor(std::log10(std::abs(t)))), t));
}
// --------------------------------------------------------------------------
template<typename T>
T get_next_pow10 (T t) {
  return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(10.0, std::ceil(std::log10(std::abs(t)))), t));
}
// --------------------------------------------------------------------------
template<typename T>
T get_next_main (T t) {
  const T u = get_prev_pow10<T>(t);
  const T v = std::ceil(t / u);
  return u * v;
}
// --------------------------------------------------------------------------
template<typename T>
T get_prev_main (T t) {
  const T u = get_prev_pow10<T>(t);
  const T v = std::floor(t / u);
  return u * v;
}
// --------------------------------------------------------------------------
template<typename T, diagram::scaling S>
struct limits {
  static core::range<T> calc (T min, T max);
};
// --------------------------------------------------------------------------
template<typename T>
struct limits<T, diagram::scaling::log> {
  static core::range<T> calc (T min, T max) {
    auto lmin = get_prev_pow10(min);
    auto lmax = get_next_pow10(max);
    return { lmin, lmax };
  }
};
// --------------------------------------------------------------------------
template<typename T>
struct limits<T, diagram::scaling::linear> {
  static core::range<T> calc (T min, T max) {
    T lmin = 0;
    T lmax = 0;
    if ((min == T(0)) && (max == T(0))) {
    } else if (min == T(0)) {
      lmax = get_next_main(max);
    } else if (max == T(0)) {
      lmin = get_next_main(min);
    } else {
      const auto ma = std::floor(std::log10(std::abs(max)));
      const auto mi = std::floor(std::log10(std::abs(min)));
      const auto m = std::max(mi, ma);
      const auto la = std::copysign(std::pow(10.0, (ma < m - 1 ? m - 1  : ma)), max);
      const auto li = std::copysign(std::pow(10.0, mi < m - 1 ? m - 1 : mi), min);
      lmax = static_cast<T>(la * std::ceil(max / la));
      lmin = static_cast<T>(li * (min < 0 ? std::ceil(min / li) : std::floor(min / li)));
    }

    return { lmin, lmax };
  }
};

void test_limits () {
  typedef limits<double, diagram::scaling::linear> lim;
  typedef core::range<double> ran;
  EXPECT_EQUAL((lim::calc(0, 1))     , ran(0, 1)      );
  EXPECT_EQUAL((lim::calc(0, 10))    , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0, 100))   , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(0, 0.1))   , ran(0, 0.1)    );
  EXPECT_EQUAL((lim::calc(0, 0.01))  , ran(0, 0.01)   );

  EXPECT_EQUAL((lim::calc(0, 1.1))   , ran(0, 2)      );
  EXPECT_EQUAL((lim::calc(0, 4.5))   , ran(0, 5)      );
  EXPECT_EQUAL((lim::calc(0, 0.45))  , ran(0, 0.5)    );
  EXPECT_EQUAL((lim::calc(0, 45))    , ran(0, 50)     );
  EXPECT_EQUAL((lim::calc(0, 8.99))  , ran(0, 9)      );
  EXPECT_EQUAL((lim::calc(0, 9.99))  , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0, 10.99)) , ran(0, 20)     );

  EXPECT_EQUAL((lim::calc(0.1, 0.2)) , ran(0.1, 0.2)  );
  EXPECT_EQUAL((lim::calc(0.1, 1))   , ran(0.1, 1)    );
  EXPECT_EQUAL((lim::calc(0.1, 10))  , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0.1, 100)) , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(0.1, 1000)), ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(1.1, 2.1)) , ran(1, 3)      );
  EXPECT_EQUAL((lim::calc(1.1, 10))  , ran(1, 10)     );
  EXPECT_EQUAL((lim::calc(1.1, 100)) , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(1.1, 1000)), ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(11, 100)) , ran(10, 100)    );
  EXPECT_EQUAL((lim::calc(11, 1000)), ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(111, 1000)), ran(100, 1000)   );

  EXPECT_EQUAL((lim::calc(-1.1, 2.1)) , ran(-2, 3)    );
  EXPECT_EQUAL((lim::calc(-1.1, 10))  , ran(-2, 10)   );
  EXPECT_EQUAL((lim::calc(-1.1, 100)) , ran(-10, 100) );
  EXPECT_EQUAL((lim::calc(-1.1, 1000)), ran(-100, 1000));

  EXPECT_EQUAL((lim::calc(-2.1, 1.1)) , ran(-3, 2)      );
  EXPECT_EQUAL((lim::calc(-10,  1.1)) , ran(-10, 2)     );
  EXPECT_EQUAL((lim::calc(-100, 1.1)) , ran(-100, 10)   );
  EXPECT_EQUAL((lim::calc(-1000, 1.1)), ran(-1000, 100) );

  EXPECT_EQUAL((lim::calc(-1.1, 0))   , ran(-2, 0)    );
  EXPECT_EQUAL((lim::calc(-4.5, 0))   , ran(-5, 0)    );
  EXPECT_EQUAL((lim::calc(-0.45, 0))  , ran(-0.5, 0)  );
  EXPECT_EQUAL((lim::calc(-45, 0))    , ran(-50, 0)   );
  EXPECT_EQUAL((lim::calc(-8.99, 0))  , ran(-9, 0)    );
  EXPECT_EQUAL((lim::calc(-9.99, 0))  , ran(-10, 0)   );
  EXPECT_EQUAL((lim::calc(-10.99, 0)) , ran(-20, 0)   );

}

// --------------------------------------------------------------------------
struct country_data : public gui::ctrl::list_data_t<std::string> {

  struct country {
    std::vector<point> positives;
    std::vector<point> deaths;
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
std::string option_names[options_count] = {
  "Absolute increase",
  "7-Day median",
  "Cumulated",
  "Relativ increase",
  "Median 7 Relativ increase",
  "R-Value",
  "Lethality"
};
// --------------------------------------------------------------------------
enum switches {
  logarithmic = 0,
  switch_count = 1
};
// --------------------------------------------------------------------------
std::string switch_names[switch_count] = {
  "Logarithmic",
};
// --------------------------------------------------------------------------
struct covid19main : public layout_main_window<gui::layout::border::layouter<25, 25, 150, 0>> {
  typedef layout_main_window<gui::layout::border::layouter<25, 25, 150, 0>> super;

  covid19main ();

  void load_data (const sys_fs::path& p);
  void initOption (int idx);
  void initSwitch (int idx);

  country_data data;

  option current_option;
  country_data::country option_data[options_count];

  vertical_list countries;
  text_button load_button;
  client_control<> chart;
  group_window<layout::grid_adaption<options_count + switch_count, 1, 2, 4>> button_group;
  radio_button<true> options[options_count];
  check_box<> switches[switch_count];

};
// --------------------------------------------------------------------------
covid19main::covid19main ()
  : current_option(undedined)
{
  button_group.on_create([&] (window*, core::rectangle) {
    for (int i = 0; i < options_count; ++i) {
      options[i].create(button_group, ctrl::const_text(option_names[i]));
    }
    for (int i = 0; i < switch_count; ++i) {
      switches[i].create(button_group, ctrl::const_text(switch_names[i]));
    }
  });

  for (int i = 0; i < switch_count; ++i) {
    initSwitch(i);
  }
  for (int i = 0; i < options_count; ++i) {
    initOption(i);
  }

  on_create([&] (window*, core::rectangle) {
    countries.create(*this);
    load_button.create(*this, "Load CSV");
    chart.create(*this);
    button_group.create(*this);

    get_layout().set_left(layout::lay(countries));
    get_layout().set_top(layout::lay(load_button));
    get_layout().set_bottom(layout::lay(button_group));
    get_layout().set_center(layout::lay(chart));
    set_children_visible();
  });

  load_button.on_clicked([&] () {
    file_open_dialog::show(*this, "Open CSV", "Ok", "Cancel", [&] (const sys_fs::path& p) {
      load_data(p);
      countries.set_data(data);
      countries.set_count();
    });
  });

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

    option_data[lethality].positives = ::ratio(option_data[increase_median_7].deaths, option_data[increase_median_7].positives, 0);
    option_data[lethality].deaths = ::ratio(option_data[increase_median_7].deaths, option_data[increase_median_7].positives, 14);

    if (undedined == current_option) {
      current_option = absolute_increase;
      options[current_option].set_checked(true);
    }

    chart.invalidate();
  });

  chart.on_paint(draw::paint([&](const draw::graphics& graph) {
    graph.clear(color::white);
    if (current_option != undedined) {
      const auto area = chart.client_area();
      const auto xmin = time::tm2time_t(time::mktm(2020, 1, 1));
      const auto xmax = time::tm2time_t(time::mktm(2020, 10, 1));

      core::grid<3, 2> g(area);

      const auto ymax = util::max_element(option_data[current_option].positives, [] (const point& l, const point& r) {
        return l.y < r.y;
      }).y;

      static auto fmtx = [] (std::time_t i) {
        return util::time::format_date(i);
      };
      static auto fmty = [] (double i) {
        return ostreamfmt(i);
      };

      if (switches[logarithmic].is_checked()) {
        const auto l = limits<double, diagram::scaling::log>::calc(ymax / 10000.0, ymax);
        diagram::chart<std::time_t, double, diagram::scaling::linear, diagram::scaling::log> d(area, xmin, xmax, l.begin(), l.end());
        d.fill_area(graph);
        d.draw_xscale(graph, 60.0*60*24*30, 60.0*60*24*5, fmtx);
        d.draw_yscale(graph, 1, 1, fmty);
        d.draw_line_graph(graph, option_data[current_option].positives, color::light_red);
        d.draw_line_graph(graph, option_data[current_option].deaths, color::light_green);
      } else {
        const auto ymin = util::min_element(option_data[current_option].positives, [] (const point& l, const point& r) {
          return l.y < r.y;
        }).y;
        const auto l = limits<double, diagram::scaling::linear>::calc(ymin, ymax);
        diagram::chart<std::time_t, double> d(area, xmin, xmax, l.begin(), l.end());
        d.fill_area(graph);
        d.draw_xscale(graph, 60.0*60*24*30, 60.0*60*24*5, fmtx);
        const auto steps = get_prev_pow10(std::max(std::abs(l.begin()), std::abs(l.end()))/3);
        d.draw_yscale(graph, steps, steps/5, fmty);
        d.draw_line_graph(graph, option_data[current_option].positives, color::light_red);
        d.draw_line_graph(graph, option_data[current_option].deaths, color::light_green);

      }

    }
  }));
}
// --------------------------------------------------------------------------
void covid19main::load_data (const sys_fs::path& p) {
  using namespace util;

  data.data.clear();
  data.countries.clear();

  std::ifstream in(p);
  csv::reader(',', true).read_csv_data(in, [&] (const csv::reader::string_list& l) {
    // 0:ateRep, 1:day, 2:month, 3:year, 4:cases, 5:deaths, 6:countriesAndTerritories
    const auto x = time::tm2time_t(time::mktm(util::string::convert::to<int>(l[3]), util::string::convert::to<int>(l[2]), util::string::convert::to<int>(l[1])));
    const double p = util::string::convert::to<int>(l[4]);
    const double d = util::string::convert::to<int>(l[5]);
    auto& c = data.data[l[6]];
    c.positives.push_back({x, p});
    c.deaths.push_back({x, d});
  });

  auto& world = data.data["World"];
  for (auto& i : data.data) {
    std::sort(i.second.positives.begin(), i.second.positives.end());
    std::sort(i.second.deaths.begin(), i.second.deaths.end());
    add(world.positives, i.second.positives);
    add(world.deaths, i.second.deaths);
  }

  std::transform(std::begin(data.data), std::end(data.data), std::back_inserter(data.countries),
                 [](country_data::map_type::value_type const& pair) { return pair.first; });

}
// --------------------------------------------------------------------------
void covid19main::initOption (int idx) {
  options[idx].on_clicked([&, idx] () {
    current_option = static_cast<option>(idx);
    for (int i = 0; i < options_count; ++i) {
      if (i != idx) {
        options[i].set_checked(false);
      }
    }
    chart.invalidate();
  });
  button_group.get_layout().add(layout::lay(options[idx]));
}
// --------------------------------------------------------------------------
void covid19main::initSwitch (int idx) {
  switches[idx].on_clicked([&] () {
    chart.invalidate();
  });
  button_group.get_layout().add(layout::lay(switches[idx]));
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {

  test_limits();

  covid19main main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Covid-19");
  main.set_visible();

  return run_main_loop();
}
