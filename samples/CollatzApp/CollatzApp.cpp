
#include <gui/win/overlapped_window.h>
#include <gui/draw/diagram.h>
#include <gui/core/grid.h>
#include <util/time_util.h>
#ifdef WIN32
#include <math.h>
#endif // GUIPP_WIN
#include <map>


using namespace gui;
using namespace gui::draw;
using namespace gui::draw::diagram;

typedef std::map<uint64_t, uint64_t> map_t;
typedef std::vector<point2d<uint64_t, uint64_t>> points_t;

// --------------------------------------------------------------------------
struct separate_thousands : std::numpunct<char> {
  char_type do_thousands_sep() const override { return '.'; }  // separate with commas
  string_type do_grouping() const override { return "\3"; } // groups of 3 digit
};

namespace hidden {
  static std::locale default_locale;

  std::locale& save_default_locale (std::ios_base& __base) {
    default_locale = __base.getloc();
    return default_locale;
  }
}

inline std::ios_base& separat_k (std::ios_base& __base) {
  static auto default_locale = hidden::save_default_locale(__base);
  static std::locale __private_locale(__base.getloc(), new separate_thousands());
  __base.imbue(__private_locale);
  return __base;
}

inline std::ios_base& unseparat_k (std::ios_base& __base) {
  __base.imbue(hidden::default_locale);
  return __base;
}
// --------------------------------------------------------------------------

std::vector<uint64_t> collatz_cache;
//map_t collatz_cache; // needs 13 GB memory for 0 .. 1e8
//// --------------------------------------------------------------------------
//template <uint64_t a>
//struct is_even {
//  static constexpr bool value = !(a % 2);
//};
//// --------------------------------------------------------------------------
//template <uint64_t num, bool even = is_even<num>::value>
//struct collatz_t {
//  static constexpr uint64_t value = 0;
//};
//// --------------------------------------------------------------------------
//template <uint64_t num>
//struct collatz_t<num, true> {
//  static constexpr uint64_t value = collatz_t<num / 2>::value + 1;
//};
//// --------------------------------------------------------------------------
//template <uint64_t num>
//struct collatz_t<num, false> {
//  static constexpr uint64_t value = collatz_t<num * 3 + 1>::value + 1;
//};
// --------------------------------------------------------------------------
template <uint64_t num>
struct collatz_t {
  static constexpr uint64_t value = collatz_t<(num % 2) ? num * 3 + 1 : num / 2>::value + 1;
};
// --------------------------------------------------------------------------
template <>
struct collatz_t<1> {
  static constexpr uint64_t value = 0;
};
// --------------------------------------------------------------------------
template<uint64_t num>
void calc_t (map_t& map) {
  map[collatz_t<num>::value]++;
  calc_t<num - 1>(map);
}
// --------------------------------------------------------------------------
template<>
void calc_t<1> (map_t& map) {
}
// --------------------------------------------------------------------------
int collatz (uint64_t num) {
  if (num < 2) {
    return 0;
  }
  if (num < collatz_cache.size()) {
    uint64_t& cached = collatz_cache[num];
    if (cached < 1) {
      cached = collatz((num % 2) ? num * 3 + 1 : num / 2) + 1;
    }
    return cached;
  }
  return collatz((num % 2) ? num * 3 + 1 : num / 2) + 1;
}
// --------------------------------------------------------------------------
points_t fill_points (const map_t& map) {
  points_t points;
  points.reserve(map.size());
  for(const auto c : map) {
    points.push_back({c.first, c.second});
  }
  return std::move(points);
}
// --------------------------------------------------------------------------
util::time::duration calc_collatz (map_t& map, uint64_t& i, const uint64_t max, util::time::duration prev_duration) {
  logging::info() << "Start collatz calculation for " << separat_k << i << " ... " << max;
  util::time::chronometer timer;
  for(; i < max; ++i) {
    map[collatz(i)]++;
  }
  auto duration = timer.stop() + prev_duration;
  logging::info() << "Finish collatz calculation for " << separat_k << i << " ... " << max << ": " << unseparat_k << duration;
  return duration;
}
// --------------------------------------------------------------------------
void draw_graph (graphics& graph, const core::rectangle& area, const points_t& data, const std::string& title) {
  graph.erase(area, color::very_very_light_gray);

  const auto r = find_min_max<uint64_t, double>(data);

  const auto lx = next_bigger_dezimal(std::max(r.first.begin(), r.first.end())/10);
  const auto ly = next_bigger_dezimal(std::max(r.second.begin(), r.second.end())/10);

  chart<uint64_t, double> d(area, r.first, {0, r.second.end()});
  d.draw_xscale(graph, lx, lx / 5);
  d.draw_yscale(graph, ly, ly / 5);
  d.draw_point_graph(graph, data, color::red, 1.0F);
  d.draw_axis(graph);
  d.draw_title(graph, title);
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  const uint64_t max = 1e9L;
  collatz_cache.resize(max);

  map_t map;
  uint64_t i = 1;
  auto duration1k4 = calc_collatz(map, i, max / 1e5L, std::chrono::seconds::zero());
  auto points1k4 = fill_points(map);

  auto duration1k5 = calc_collatz(map, i, max / 1e4L, duration1k4);
  auto points1k5 = fill_points(map);

  auto duration1k6 = calc_collatz(map, i, max / 1e3L, duration1k5);
  auto points1k6 = fill_points(map);

  auto duration1k7 = calc_collatz(map, i, max / 1e2L, duration1k6);
  auto points1k7 = fill_points(map);

  auto duration1k8 = calc_collatz(map, i, max / 1e1L, duration1k7);
  auto points1k8 = fill_points(map);

  auto duration1k9 = calc_collatz(map, i, max / 1e0L, duration1k8);
  auto points1k9 = fill_points(map);

//  map.clear();
//  points_t points2k4, points2k5;//, points2k6, points2k7;
//  auto duration2k4 = timer.process([&](){calc_t<10000>(map);});
//  fill_points(map, points2k4);
//  map.clear();
//  auto duration2k5 = timer.process([&](){calc_t<100000>(map);});
//  fill_points(map, points2k5);
//  map.clear();
//  auto duration2k6 = timer.process([&](){calc_t<1000000>(map);});
//  fill_points(map, points2k6);
//  map.clear();
//  auto duration2k7 = timer.process([&](){calc_t<10000000>(map);});
//  fill_points(map, points2k7);

//  logging::info() << "Finish template based collatz calculation. Durations: 1e4: " << duration2k4
//                  << ", 1e5: " << duration2k5
//  << ", 1e6: " << duration2k6
//  << ", 1e7: " << duration2k7
//      ;

  main.on_paint(draw::paint([&](graphics& graph) {
    core::grid<2, 3> g(main.client_geometry());
    draw_graph(graph, g(0, 0), points1k4, ostreamfmt("Von 0 bis " << separat_k << uint64_t(max / 1e5L)));
    draw_graph(graph, g(0, 1), points1k5, ostreamfmt("Von 0 bis " << separat_k << uint64_t(max / 1e4L)));
    draw_graph(graph, g(0, 2), points1k6, ostreamfmt("Von 0 bis " << separat_k << uint64_t(max / 1e3L)));
    draw_graph(graph, g(1, 0), points1k7, ostreamfmt("Von 0 bis " << separat_k << uint64_t(max / 1e2L)));
    draw_graph(graph, g(1, 1), points1k8, ostreamfmt("Von 0 bis " << separat_k << uint64_t(max / 1e1L)));
    draw_graph(graph, g(1, 2), points1k9, ostreamfmt("Von 0 bis " << separat_k << max));

//    draw_graph(graph, g(2, 0), points2k4, "T von 0 bis 10.000");
//    draw_graph(graph, g(2, 1), points2k5, "T von 0 bis 100.000");
//    draw_graph(graph, g(2, 2), points2k6, "T 1.000.000");
//    draw_graph(graph, g(2, 3), points2k7, "T 10.000.000");
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Collatz");
  main.set_visible();

  return run_main_loop();
}
