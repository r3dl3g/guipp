
#include <gui/win/overlapped_window.h>
#include <gui/draw/diagram.h>
#include <gui/core/grid.h>
#ifdef WIN32
#include <math.h>
#endif // GUIPP_WIN

#include <util/time_util.h>


using namespace gui;
using namespace gui::draw;
using namespace gui::draw::diagram;

namespace gui {
  namespace draw {
    namespace diagram {

      template<>
      inline core::point::type convert<core::point::type, util::time::duration> (const util::time::duration& d) {
        return static_cast<core::point::type>(d.count());
      }

      template<>
      inline core::point::type convert<core::point::type, util::time::time_point> (const util::time::time_point& d) {
        return static_cast<core::point::type>(d.time_since_epoch().count());
      }

      template<>
      struct default_sub_type<util::time::time_point> {
        typedef util::time::duration type;
      };


    }
  }
}

template<typename X, typename Y, std::size_t S = 100>
struct sinus_data_t {

  sinus_data_t (X dx = 0)
    : delta(dx)
  {}

  std::size_t size () const {
    return S;
  }

  point2d<X, Y> operator[] (std::size_t i) const {
    const X x{i / 50.0 * M_PI};
    const Y y{std::sin(x*2 + delta * M_PI)};
    return {x, y};
  }

private:
  X delta;
};

template<typename Y, std::size_t S>
struct sinus_data_t<util::time::duration, Y, S> {

  sinus_data_t ()
  {}

  std::size_t size () const {
    return S;
  }

  point2d<util::time::duration, Y> operator[] (std::size_t i) const {
    const util::time::duration x = util::time::mkduration(0, 0, i);
    const Y y{std::sin(i * M_PI / 30.0)};
    return {x, y};
  }
};

template<typename Y, std::size_t S>
struct sinus_data_t<util::time::time_point, Y, S> {

  sinus_data_t (util::time::time_point dy = 0)
    : delta(dy)
  {}

  std::size_t size () const {
    return S;
  }

  point2d<util::time::time_point, Y> operator[] (std::size_t i) const {
    const util::time::time_point x = delta + util::time::mkduration(0, 0, i);
    const Y y{std::sin(i * M_PI / 30.0)};
    return {x, y};
  }

private:
  util::time::time_point delta;
};

template<typename X, typename Y>
struct linear_data_t {

  linear_data_t (X max = M_PI, X min = 0)
    : max(max)
    , min(min)
  {}

  std::size_t size () const {
    return 1000;
  }

  point2d<X, Y> operator[] (std::size_t i) const {
    const X ix = static_cast<X>(i) + min * 5;
    const X x = static_cast<X>(ix / 500.0 * (max - min));
    const Y y = static_cast<Y>(std::copysign(ix * ix / 100.0 + 0.01, ix));
    return {x, y};
  }

private:
  X max;
  X min;
};

typedef sinus_data_t<double, double> sinus_data;
typedef linear_data_t<double, double> linear_data;

constexpr os::color wall_back = color::rgb_gray<0xF8>::value;

// --------------------------------------------------------------------------
auto duration_fmt = [] (util::time::duration i) {
  return util::time::format_duration_only_h(i, ":", false, true);
};
auto time_point_fmt = [] (util::time::time_point i) {
  return util::time::format_datetime(i);
};

font& font_serif () {
#ifdef GUIPP_QT
  static font f = [] () {
    os::font fn;
    fn.setStyleHint(QFont::StyleHint::Serif);
    fn.setPointSize(font::system().size() * 4 / 5);
    return font(fn);
  }();
#else
  static font f = font::serif();
#endif
  return f;
}

// --------------------------------------------------------------------------
void draw_graph_1 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 1 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scale_formatter<double> fmt = [] (double i) -> std::string {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  scaler<double> xscale({0.0, 6.5}, {p0.x(), area.x2() - 20});
  scaler<double> yscale({-1.2, 1.2}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double>(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 1"), font_serif(), color::black);

  graph.text(mk_scale<orientation_t::horizontal>(p0, xscale, 1.0, 0.2,
                                                 yscale.get_target_size(),
                                                 yscale.get_target_size()),
             font_serif(), color::black);

  graph.text(mk_scale<orientation_t::vertical>(p0, yscale, 0.2, 0.05,
                                               xscale.get_target_size(),
                                               xscale.get_target_size(),
                                               color::very_light_gray,
                                               color::very_very_light_gray,
                                               fmt),
             font_serif(), color::black);

  std::vector<point2d<double, double>> data1;
  std::array<point2d<double, double>, 100> data2;
  {
    sinus_data src1(+0.5);
    sinus_data src2(-0.5);

    data1.reserve(src1.size());
    for (std::size_t i = 0; i < src1.size(); ++i) {
      data1.push_back(src1[i]);
      data2[i] = src2[i];
    }
  }

  graph.fill(line_graph<double, double, const std::array<point2d<double, double>, 100>>(xscale, yscale, data2), color::very_light_red);
  graph.fill(bar_graph<double, double, const std::vector<point2d<double, double>>>(xscale, yscale, data1, 1), color::blue);
  graph.fill(points_graph<double, double, sinus_data>(xscale, yscale, sinus_data(-1), diagram::cross(2)), color::green);

  graph.frame(mk_xy_axis(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_2 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 2 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double> xscale({0, 6.5}, {p0.x(), area.x2() - 20});
  scaler<double, scaling::log> yscale({0.01, 10000}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double, scaling::linear, scaling::log>(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 2"), font_serif(), color::black);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2, yscale.get_target_size(), yscale.get_target_size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_size(), xscale.get_target_size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::linear, scaling::log>(xscale, yscale, linear_data()), color::blue);
  graph.fill(points_graph<double, double, sinus_data, scaling::linear, scaling::log>(xscale, yscale, sinus_data(), diagram::diamond(2)), color::green);

  graph.frame(xy_axis<double, double, scaling::linear, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_3 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 3 in area:" << area;

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale({0.01, 10.0}, {p0.x(), area.x2() - 20});
  scaler<double> yscale({0, 10000}, {p0.y(), area.y() + 20});

  graph.frame(draw::rectangle(area), color::black);
  graph.draw(wall<double, double, scaling::log>(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 3"), font_serif(), color::black);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target_size(), yscale.get_target_size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 1000, 100, xscale.get_target_size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::linear>(xscale, yscale, linear_data()), color::green);

  graph.frame(xy_axis<double, double, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_4 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 4 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale({0.01, 10}, {p0.x(), area.x2() - 20});
  scaler<double, scaling::log> yscale({0.01, 10000}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double, scaling::log, scaling::log>(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 4"), font_serif(), color::black);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target_size(), yscale.get_target_size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_size(), xscale.get_target_size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::log>(xscale, yscale, linear_data()), color::light_red);

  graph.frame(xy_axis<double, double, scaling::log, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_5 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 5 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  typedef util::time::duration xtype;
  // const xtype now = std::chrono::system_clock::now();
  const auto min_time = util::time::mkduration();
  const auto max_time = util::time::mkduration(0, 1, 40);
  const auto main_scale = util::time::mkduration(0, 0, 15);
  const auto sub_scale = util::time::mkduration(0, 0, 5);

  scaler<xtype> xscale({{}, max_time}, {p0.x(), area.x2() - 20});
  scaler<float> yscale({0.0F, 5.0F}, {p0.y(), area.y() + 20});

  graph.draw(mk_wall(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 5"), font_serif(), color::black);

  graph.text(scale<xtype, orientation_t::horizontal>(p0, xscale, main_scale, sub_scale,
                                                     xscale.get_target_size(),
                                                     xscale.get_target_size(),
                                                     color::very_light_gray,
                                                     color::very_very_light_gray,
                                                     duration_fmt),
             font_serif(), color::black);

  auto yfmt = [] (float i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };
  graph.text(scale<float, orientation_t::vertical>(p0, yscale, 1.0F, 0.25F,
                                                   yscale.get_target_size(),
                                                   yscale.get_target_size(),
                                                   color::very_light_gray, color::very_very_light_gray, yfmt),
             font_serif(), color::black);

  std::vector<point2d<xtype, float>> data1;
  std::array<point2d<xtype, float>, 100> data2;
  data1.reserve(100);
  for (std::size_t i = 0; i < 20; ++i) {
    data1.push_back(point2d<xtype, float>{util::time::mkduration(0, 0, i*5), (float)i / 5.0F});
    data2[i] = point2d<xtype, float>{util::time::mkduration(0, 0, i*5), (float)i / 6.0F};
  }

  // graph.fill(mk_bar_graph(xscale, yscale, data2, 1.0F), color::blue);
  graph.fill(mk_line_graph(xscale, yscale, data1), color::light_red);
  graph.fill(mk_points_graph(xscale, yscale, data2, diagram::circle(2)), color::light_green);

  graph.frame(mk_xy_axis(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_6 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 6 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 80, area.y2() - 25);

  scaler<int> xscale({0, 100}, {p0.x(), (area.x2() - 20)});
  scaler<double, scaling::log> yscale({0.01, 10000.0}, {p0.y(), area.y() + 20});

  graph.draw(wall<int, double, scaling::linear, scaling::log>(xscale, yscale), wall_back, wall_back);
  graph.text(mk_headline(xscale, yscale, "graph 6"), font_serif(), color::black);

  graph.text(scale<int, orientation_t::horizontal>(p0, xscale, 20, 5,
                                                   static_cast<int>(yscale.get_target_size()), static_cast<int>(yscale.get_target_size())),
             font_serif(), color::black);
  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_size(), xscale.get_target_size()),
             font_serif(), color::black);

  typedef linear_data_t<int, double> data_type;
  graph.fill(line_graph<int, double, data_type, scaling::linear, scaling::log>(
             xscale, yscale, data_type(100), yscale.get_source().begin()), color::very_light_red);

  graph.frame(mk_axis<orientation_t::vertical>(p0, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_7 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 7 in area:" << area;

  chart<double, double> d(area, {0, 6.5}, {-1.2, 1.2});
  d.draw_background(graph, 1, 0.2, 0.2, 0.05);
  d.draw_title(graph, "graph 7");
  d.draw_line_graph(graph, sinus_data(0.5), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data(-0.5), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data(-1), color::green, 2);
}
// --------------------------------------------------------------------------
void draw_graph_8 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 8 in area:" << area;

  const util::time::time_point min_time = util::time::now();
  const util::time::time_point max_time = min_time + std::chrono::minutes(2);
  const auto main_scale = util::time::mkduration(0, 1, 0);
  const auto sub_scale = util::time::mkduration(0, 0, 15);

  typedef util::time::time_point xtype;

  auto d = mk_chart(area, core::mk_range(min_time, max_time), core::mk_range(-1.2, 1.2));
  d.draw_xscale(graph, main_scale, sub_scale, time_point_fmt);
  d.draw_yscale(graph, 0.2, 0.05);
  d.draw_title(graph, "graph 8");
  d.draw_area_graph(graph, sinus_data_t<xtype, double, 110>(min_time), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data_t<xtype, double>(min_time), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data_t<xtype, double, 120>(min_time), color::green, 2);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_9 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 9 in area:" << area;

  auto d = mk_chart<scaling::linear, scaling::log>(area,
          core::mk_range(0, 100), core::mk_range(0.01, 10000.0));
  d.draw_xscale(graph, 20, 5);
  d.draw_yscale(graph, 1, 1);
  d.draw_title(graph, "graph 9");
  d.draw_area_graph(graph, linear_data_t<int, double>(100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_10 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 10 in area:" << area;

  chart<int, double, scaling::linear, scaling::symlog> d(area, {-100, 100}, {0.01, 10000.0});
  logging::trace() << "Draw xscale in graph 10";
  d.draw_xscale(graph, 50, 10);
  auto fmt = [] (double i) {
    return ostreamfmt(i);
  };
  logging::trace() << "Draw yscale in graph 10";
  d.draw_yscale(graph, 1, 1, fmt);
  logging::trace() << "Draw line_graph in graph 10";
  d.draw_title(graph, "graph 10");
  d.draw_line_graph(graph, linear_data_t<int, double>(100, -100), color::very_light_red);
  logging::trace() << "Draw axis in graph 10";
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_11 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 11 in area:" << area;

  chart<double, double, scaling::symlog, scaling::symlog> d(area, {0.01, 100}, {0.01, 1000.0});
  auto fmt = [] (double i) {
    return ostreamfmt(i);
  };
  d.draw_xscale(graph, 1, 1, fmt);
  d.draw_yscale(graph, 1, 1, fmt);
  d.draw_title(graph, "graph 11");
  d.draw_line_graph(graph, linear_data_t<double, double>(100, -100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_12 (graphics& graph, const core::rectangle& area) {
  logging::trace() << "Draw graph 12 in area:" << area;

  chart<double, double, scaling::symlog, scaling::linear> d(area, {0.01, 100}, {-1000.0, 1000.0});
  auto fmt = [] (double i) {
    return ostreamfmt(i);
  };
  d.draw_xscale(graph, 1, 1, fmt);
  d.draw_yscale(graph, 100, 10, fmt);
  d.draw_title(graph, "graph 12");
  d.draw_line_graph(graph, linear_data_t<double, double>(100, -100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_size([&] (const core::size& sz) {
    logging::trace() << "Resized to " << sz << " -> initiate redraw";
    main.invalidate();
  });
  main.on_paint(draw::paint([&](graphics& graph) {
    logging::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    auto area = main.client_geometry();
    graph.fill(draw::rectangle(area), color::very_very_light_gray);
    logging::trace() << "Draw graphs in area:" << area;

    core::grid<4, 3> g(area);
    draw_graph_1(graph, g(0, 0));
    draw_graph_2(graph, g(1, 0));
    draw_graph_3(graph, g(2, 0));
    draw_graph_4(graph, g(0, 1));
    draw_graph_5(graph, g(1, 1));
    draw_graph_6(graph, g(2, 1));
    draw_graph_7(graph, g(0, 2));
    draw_graph_8(graph, g(1, 2));
    draw_graph_9(graph, g(2, 2));
    draw_graph_10(graph, g(3, 0));
    draw_graph_11(graph, g(3, 1));
    draw_graph_12(graph, g(3, 2));

    logging::trace() << "on_paint finished";
  }));
  main.on_left_btn_down([&] (os::key_state, const core::native_point& pt) {
    logging::trace() << "Left button down at " << pt << " -> initiate redraw";
    main.invalidate();
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Diagrams");
  main.set_visible();

  return run_main_loop();
}
