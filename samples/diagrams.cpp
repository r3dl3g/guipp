
#include <gui/win/container.h>
#include <gui/draw/diagram.h>
#include <gui/core/grid.h>
#include <logging/core.h>
#ifdef WIN32
#define _USE_MATH_DEFINES
#include <math.h>
#endif // GUIPP_WIN

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::draw::diagram;

template<typename T>
struct sinus_data_t {

  sinus_data_t (T delta = 0)
    : delta(delta)
  {}

  std::size_t size () const {
    return 100;
  }

  point2d<T, T> operator[] (std::size_t i) const {
    const T x = static_cast<T>(i / 50.0 * M_PI);
    const T y = static_cast<T>(std::sin(x*2 + delta * M_PI));
    return {x, y};
  }

private:
  T delta;
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

typedef sinus_data_t<double> sinus_data;
typedef linear_data_t<double, double> linear_data;

constexpr os::color wall_back = color::rgb_gray<0xF8>::value;

font& font_serif () {
#ifdef GUIPP_QT
  static font f = [] () {
    os::font fn;
    fn.setStyleHint(QFont::StyleHint::Serif);
    fn.setPointSize(font::system().size() * 4 / 5);
    return fn;
  }();
#else
  static font f = font::serif();
#endif
  return f;
}

// --------------------------------------------------------------------------
void draw_graph_1 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 1 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  auto fmt = [] (double i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  scaler<double> xscale({0, 6.5}, {p0.x(), area.x2() - 20});
  scaler<double> yscale({-1.2, 1.2}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2,
                                                      yscale.get_target().size(), yscale.get_target().size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 0.2, 0.05,
                                                    xscale.get_target().size(), xscale.get_target().size(),
                                                    color::very_light_gray, color::very_very_light_gray,
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

  graph.frame(xy_axis<double, double>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_2 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 2 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double> xscale({0, 6.5}, {p0.x(), area.x2() - 20});
  scaler<double, scaling::log> yscale({0.01, 10000}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double, scaling::linear, scaling::log>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2, yscale.get_target().size(), yscale.get_target().size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target().size(), xscale.get_target().size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::linear, scaling::log>(xscale, yscale, linear_data()), color::blue);
  graph.fill(points_graph<double, double, sinus_data, scaling::linear, scaling::log>(xscale, yscale, sinus_data(), diagram::diamond(2)), color::green);

  graph.frame(xy_axis<double, double, scaling::linear, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_3 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 3 in area:" << area;

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale({0.01, 10.0}, {p0.x(), area.x2() - 20});
  scaler<double> yscale({0, 10000}, {p0.y(), area.y() + 20});

  graph.frame(draw::rectangle(area), color::black);
  graph.draw(wall<double, double, scaling::log>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target().size(), yscale.get_target().size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 1000, 100, xscale.get_target().size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::linear>(xscale, yscale, linear_data()), color::green);

  graph.frame(xy_axis<double, double, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_4 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 4 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale({0.01, 10}, {p0.x(), area.x2() - 20});
  scaler<double, scaling::log> yscale({0.01, 10000}, {p0.y(), area.y() + 20});

  graph.draw(wall<double, double, scaling::log, scaling::log>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target().size(), yscale.get_target().size()),
             font_serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target().size(), xscale.get_target().size()),
             font_serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::log>(xscale, yscale, linear_data()), color::light_red);

  graph.frame(xy_axis<double, double, scaling::log, scaling::log>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_5 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 5 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<float> xscale({0, 6.5}, {p0.x(), area.x2() - 20});
  scaler<float> yscale({-1.2F, 1.2F}, {p0.y(), area.y() + 20});

  graph.draw(wall<float, float>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<float, orientation_t::horizontal>(p0, xscale, 1.0F, 0.2F,
                                                     yscale.get_target().size(),
                                                     yscale.get_target().size()),
             font_serif(), color::black);

  auto fmt = [] (float i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  graph.text(scale<float, orientation_t::vertical>(p0, yscale, 0.2F, 0.05F,
                                                   xscale.get_target().size(),
                                                   xscale.get_target().size(),
                                                   color::very_light_gray, color::very_very_light_gray, fmt),
             font_serif(), color::black);

  std::vector<point2d<float, float>> data1;
  std::array<point2d<float, float>, 100> data2;
  {
    sinus_data_t<float> src1(+0.5);
    sinus_data_t<float> src2(-0.5);

    data1.reserve(src1.size());
    for (std::size_t i = 0; i < src1.size(); ++i) {
      data1.push_back(src1[i]);
      data2[i] = src2[i];
    }
  }

  graph.fill(cascade<float, float, const std::array<point2d<float, float>, 100>>(xscale, yscale, data2), color::light_red);
  graph.fill(bar_graph<float, float, const std::vector<point2d<float, float>>>(xscale, yscale, data1, 2), color::blue);
  graph.fill(points_graph<float, float, sinus_data>(xscale, yscale, sinus_data(-1), diagram::circle(2)), color::light_green);

  graph.frame(xy_axis<float, float>(xscale, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_6 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 6 in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 80, area.y2() - 25);

  scaler<int> xscale({0, 100}, {static_cast<int>(p0.x()), static_cast<int>(area.x2() - 20)});
  scaler<double, scaling::log> yscale({0.01, 10000.0}, {p0.y(), area.y() + 20});

  graph.draw(wall<int, double, scaling::linear, scaling::log>(xscale, yscale), wall_back, wall_back);

  graph.text(scale<int, orientation_t::horizontal>(p0, xscale, 20, 5,
                                                   static_cast<int>(yscale.get_target().size()), static_cast<int>(yscale.get_target().size())),
             font_serif(), color::black);
  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target().size(), xscale.get_target().size()),
             font_serif(), color::black);

  typedef linear_data_t<int, double> data_type;
  graph.fill(line_graph<int, double, data_type, scaling::linear, scaling::log>(
             xscale, yscale, data_type(100), yscale.get_source().begin()), color::very_light_red);

  graph.frame(axis<double, orientation_t::vertical, scaling::log>(p0, yscale), color::black);
}
// --------------------------------------------------------------------------
void draw_graph_7 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 7 in area:" << area;

  chart<double, double> d(area, {0, 6.5}, {-1.2, 1.2});
  d.draw_background(graph, 1, 0.2, 0.2, 0.05);
  d.draw_line_graph(graph, sinus_data(0.5), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data(-0.5), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data(-1), color::green, 2);
}
// --------------------------------------------------------------------------
void draw_graph_8 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 8 in area:" << area;

  chart<double, double> d(area, {0, 6.5}, {-1.2, 1.2});
  d.draw_xscale(graph, 1, 0.2);
  d.draw_yscale(graph, 0.2, 0.05);
  d.draw_area_graph(graph, sinus_data(0.5), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data(-0.5), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data(-1), color::green, 2);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_9 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 9 in area:" << area;

  chart<int, double, scaling::linear, scaling::log> d(area, {0, 100}, {0.01, 10000.0});
  d.draw_xscale(graph, 20, 5);
  d.draw_yscale(graph, 1, 1);
  d.draw_area_graph(graph, linear_data_t<int, double>(100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_10 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 10 in area:" << area;

  chart<int, double, scaling::linear, scaling::symlog> d(area, {-100, 100}, {0.01, 10000.0});
  clog::trace() << "Draw xscale in graph 10";
  d.draw_xscale(graph, 50, 10);
  auto fmt = [] (float i) {
    return ostreamfmt(i);
  };
  clog::trace() << "Draw yscale in graph 10";
  d.draw_yscale(graph, 1, 1, fmt);
  clog::trace() << "Draw line_graph in graph 10";
  d.draw_line_graph(graph, linear_data_t<int, double>(100, -100), color::very_light_red);
  clog::trace() << "Draw axis in graph 10";
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_11 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 11 in area:" << area;

  chart<double, double, scaling::symlog, scaling::symlog> d(area, {0.01, 100}, {0.01, 1000.0});
  auto fmt = [] (float i) {
    return ostreamfmt(i);
  };
  d.draw_xscale(graph, 1, 1, fmt);
  d.draw_yscale(graph, 1, 1, fmt);
  d.draw_line_graph(graph, linear_data_t<double, double>(100, -100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
void draw_graph_12 (const graphics& graph, const core::rectangle& area) {
  clog::trace() << "Draw graph 12 in area:" << area;

  chart<double, double, scaling::symlog, scaling::linear> d(area, {0.01, 100}, {-1000.0, 1000.0});
  auto fmt = [] (float i) {
    return ostreamfmt(i);
  };
  d.draw_xscale(graph, 1, 1, fmt);
  d.draw_yscale(graph, 100, 10, fmt);
  d.draw_line_graph(graph, linear_data_t<double, double>(100, -100), color::very_light_red);
  d.draw_axis(graph);
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_size([&] (const core::size& sz) {
    clog::trace() << "Resized to " << sz << " -> initiate redraw";
    main.invalidate();
  });
  main.on_paint(draw::paint([&](const graphics& graph) {
    clog::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    auto area = main.client_area();
    graph.fill(draw::rectangle(area), color::very_very_light_gray);
    clog::trace() << "Draw graphs in area:" << area;

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

    clog::trace() << "on_paint finished";
  }));
  main.on_left_btn_down([&] (os::key_state, const core::point& pt) {
    clog::trace() << "Left button down at " << pt << " -> initiate redraw";
    main.invalidate();
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Diagrams");
  main.set_visible();

  return run_main_loop();
}
