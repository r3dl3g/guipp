

#include <gui/win/container.h>
#include <logging/core.h>
#include <gui/draw/diagram.h>
#include <gui/core/grid.h>

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

  linear_data_t (X max = M_PI)
    : max(max)
  {}

  std::size_t size () const {
    return 1000;
  }

  point2d<X, Y> operator[] (std::size_t i) const {
    const X x = static_cast<X>(i / 500.0 * max);
    const Y y = static_cast<Y>(i * i / 100.0 + 0.01);
    return {x, y};
  }

private:
  X max;
};

typedef sinus_data_t<double> sinus_data;
typedef linear_data_t<double, double> linear_data;

constexpr os::color wall_back = color::rgb_gray<0xF8>::value;

void draw_graph_1 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  auto fmt = [] (double i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  scaler<double> xscale(0, 6.5, p0.x(), area.x2() - 20);
  scaler<double> yscale(-1.2, 1.2, p0.y(), area.y() + 20);

  graph.draw(wall<double, double>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2,
                                                      yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 0.2, 0.05,
                                                    xscale.get_target_range(), xscale.get_target_range(),
                                                    color::very_light_gray, color::very_very_light_gray,
                                                    fmt),
             font::serif(), color::black);

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


  graph.fill(line_graph<double, double, const std::array<point2d<double, double>, 100>>(p0, xscale, yscale, data2), color::very_light_red);
  graph.fill(bar_graph<double, double, const std::vector<point2d<double, double>>>(p0, xscale, yscale, data1, 1), color::blue);
  graph.fill(points_graph<double, double, sinus_data>(p0, xscale, yscale, sinus_data(-1), diagram::cross(2)), color::green);

  graph.frame(axis<double, double>(p0, xscale, yscale), color::black);
}

void draw_graph_2 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double> xscale(0, 6.5, p0.x(), area.x2() - 20);
  scaler<double, scaling::log> yscale(0.01, 10000, p0.y(), area.y() + 20);

  graph.draw(wall<double, double>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_range(), xscale.get_target_range()),
             font::serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::linear, scaling::log>(p0, xscale, yscale, linear_data()), color::blue);
  graph.fill(points_graph<double, double, sinus_data, scaling::linear, scaling::log>(p0, xscale, yscale, sinus_data(), diagram::diamond(2)), color::green);

  graph.frame(axis<double, double, scaling::linear, scaling::log>(p0, xscale, yscale), color::black);
}

void draw_graph_3 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale(0.01, 10.0, p0.x(), area.x2() - 20);
  scaler<double> yscale(0, 10000, p0.y(), area.y() + 20);

  graph.draw(wall<double, double>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 1000, 100, xscale.get_target_range()),
             font::serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::linear>(p0, xscale, yscale, linear_data()), color::green);

  graph.frame(axis<double, double, scaling::log>(p0, xscale, yscale), color::black);
}

void draw_graph_4 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<double, scaling::log> xscale(0.01, 10, p0.x(), area.x2() - 20);
  scaler<double, scaling::log> yscale(0.01, 10000, p0.y(), area.y() + 20);

  graph.draw(wall<double, double>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<double, orientation_t::horizontal, scaling::log>(p0, xscale, 1, 1, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);

  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_range(), xscale.get_target_range()),
             font::serif(), color::black);

  graph.frame(line_graph<double, double, linear_data, scaling::log, scaling::log>(p0, xscale, yscale, linear_data()), color::light_red);

  graph.frame(axis<double, double, scaling::log, scaling::log>(p0, xscale, yscale), color::black);
}

void draw_graph_5 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<float> xscale(0, 6.5, p0.x(), area.x2() - 20);
  scaler<float> yscale(-1.2, 1.2, p0.y(), area.y() + 20);

  graph.draw(wall<float, float>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<float, orientation_t::horizontal>(p0, xscale, 1, 0.2,
                                                     yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);

  auto fmt = [] (float i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  graph.text(scale<float, orientation_t::vertical>(p0, yscale, 0.2, 0.05,
                                                   xscale.get_target_range(), xscale.get_target_range(),
                                                   color::very_light_gray, color::very_very_light_gray, fmt),
             font::serif(), color::black);

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

  graph.fill(cascade<float, float, const std::array<point2d<float, float>, 100>>(p0, xscale, yscale, data2), color::light_red);
  graph.fill(bar_graph<float, float, const std::vector<point2d<float, float>>>(p0, xscale, yscale, data1, 2), color::blue);
  graph.fill(points_graph<float, float, sinus_data>(p0, xscale, yscale, sinus_data(-1), diagram::circle(2)), color::light_green);

  graph.frame(axis<float, float>(p0, xscale, yscale), color::black);
}

void draw_graph_6 (const graphics& graph, const core::rectangle& area) {
  graph.frame(draw::rectangle(area), color::black);

  core::point p0(area.x() + 50, area.y2() - 25);

  scaler<int> xscale(0, 100, p0.x(), area.x2() - 20);
  scaler<double, scaling::log> yscale(0.01, 10000.0, p0.y(), area.y() + 20);

  graph.draw(wall<int, double>(p0, xscale, yscale), wall_back, wall_back);

  graph.text(scale<int, orientation_t::horizontal>(p0, xscale, 20, 5, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::black);
  graph.text(scale<double, orientation_t::vertical, scaling::log>(p0, yscale, 1, 1, xscale.get_target_range(), xscale.get_target_range()),
             font::serif(), color::black);

  typedef linear_data_t<int, double> data_type;
  graph.fill(line_graph<int, double, data_type, scaling::linear, scaling::log>(
             p0, xscale, yscale, data_type(100), yscale.get_min()), color::very_light_red);

  graph.frame(axis<int, double, scaling::linear, scaling::log>(p0, xscale, yscale), color::black);
}

// --------------------------------------------------------------------------
void draw_graph_7 (const graphics& graph, const core::rectangle& area) {
  chart<double, double> d(area, 0, 6.5, -1.2, 1.2);
  d.draw_background(graph, 1, 0.2, 0.2, 0.05);
  d.draw_line_graph(graph, sinus_data(0.5), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data(-0.5), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data(-1), color::green, 2);
}

// --------------------------------------------------------------------------
void draw_graph_8 (const graphics& graph, const core::rectangle& area) {
  chart<double, double> d(area, 0, 6.5, -1.2, 1.2);
  d.draw_xscale(graph, 1, 0.2);
  d.draw_yscale(graph, 0.2, 0.05);
  d.draw_area_graph(graph, sinus_data(0.5), color::very_light_red);
  d.draw_bar_graph(graph, sinus_data(-0.5), color::blue, 2);
  d.draw_cross_graph(graph, sinus_data(-1), color::green, 2);
  d.draw_axis(graph);
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_paint(draw::paint([&](const graphics& graph) {
    graph.clear(color::white);

    core::grid<3, 3> g(main.client_area());
    draw_graph_1(graph, g(0, 0));
    draw_graph_2(graph, g(1, 0));
    draw_graph_3(graph, g(2, 0));
    draw_graph_4(graph, g(0, 1));
    draw_graph_5(graph, g(1, 1));
    draw_graph_6(graph, g(2, 1));
    draw_graph_7(graph, g(0, 2));
    draw_graph_8(graph, g(1, 2));
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Diagrams");
  main.set_visible();

  return run_main_loop();
}
