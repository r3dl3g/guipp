

#include <gui/win/container.h>
#include <logging/core.h>
#include <gui/draw/diagram.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::draw::diagram;

struct sinus_data {

  sinus_data (double delta = 0)
    : delta(delta)
  {}

  std::size_t size () const {
    return 100;
  }

  point2d<double> operator[] (std::size_t i) const {
    const double x = static_cast<double>(i) / 50 * M_PI;
    const double y = std::sin(x*2 + delta * M_PI);
    return {x, y};
  }

  virtual ~sinus_data ()
  {}

private:
  double delta;
};

struct linear_data {
  std::size_t size () const {
    return 1000;
  }

  point2d<double> operator[] (std::size_t i) const {
    const double x = static_cast<double>(i) / 500 * M_PI;
    const double y = (i*i/100.0 + 0.01);
    return {x, y};
  }

  virtual ~linear_data ()
  {}

};

void draw_graph_1 (const graphics& graph, const core::rectangle& area) {
  core::point p0(area.x() + 50, area.y2() - 50);
  core::point p1(area.x() + 50, (area.y() + area.y2()) / 2);

  auto fmt = [] (double i) {
    return ostreamfmt(std::fixed << std::setprecision(1) << i);
  };

  scaler<double> xscale(0, 6.5, area.x() + 50, area.x2() - 50);
  scaler<double> yscale(-1.2, 1.2, area.y2() - 50, area.y() + 50);

  graph.draw(wall<double>(p0, xscale, yscale),
             color::very_very_light_gray, color::very_light_gray);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::light_gray);
//  graph.text(scale<double, orientation_t::horizontal>(p1, xscale, 1, 0.2),
//             font::serif(), color::light_gray);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 0.2, 0.05, xscale.get_target_range(), xscale.get_target_range(), fmt),
             font::serif(), color::light_gray);

  std::vector<point2d<double>> data1;
  std::array<point2d<double>, 100> data2;
  {
    sinus_data src1(+0.5);
    sinus_data src2(-0.5);

    data1.reserve(src1.size());
    for (std::size_t i = 0; i < src1.size(); ++i) {
      data1.push_back(src1[i]);
      data2[i] = src2[i];
    }
  }


  graph.fill(bar_graph<double, const std::vector<point2d<double>>>(p0, xscale, yscale, data1), color::blue);
  graph.frame(line_graph<double, const std::array<point2d<double>, 100>>(p0, xscale, yscale, data2), color::red);
  graph.fill(points_graph<double, sinus_data>(p0, xscale, yscale, sinus_data(-1)), color::green);

}

void draw_graph_2 (const graphics& graph, const core::rectangle& area) {
  core::point p0(area.x() + 50, area.y2() - 50);

  scaler<double> xscale(0, 6.5, area.x() + 50, area.x2() - 50);
  scaler<double, scaling_type::log> yscale(0.01, 10000, area.y2() - 50, area.y() + 50);

  graph.draw(wall<double>(p0, xscale, yscale),
             color::very_very_light_gray, color::very_light_gray);

  graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, 0.2, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.text(scale<double, orientation_t::vertical, scaling_type::log>(p0, yscale, 1, 1, xscale.get_target_range(), xscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.frame(line_graph<double, linear_data, scaling_type::linear, scaling_type::log>(p0, xscale, yscale, linear_data()), color::blue);
}

void draw_graph_3 (const graphics& graph, const core::rectangle& area) {
  core::point p0(area.x() + 50, area.y2() - 50);

  scaler<double, scaling_type::log> xscale(0.01, 10.0, area.x() + 50, area.x2() - 50);
  scaler<double> yscale(0, 10000, area.y2() - 50, area.y() + 50);

  graph.draw(wall<double>(p0, xscale, yscale),
             color::very_very_light_gray, color::very_light_gray);

  graph.text(scale<double, orientation_t::horizontal, scaling_type::log>(p0, xscale, 1, 1, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.text(scale<double, orientation_t::vertical>(p0, yscale, 1000, 100, xscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.frame(line_graph<double, linear_data, scaling_type::log, scaling_type::linear>(p0, xscale, yscale, linear_data()), color::green);
}

void draw_graph_4 (const graphics& graph, const core::rectangle& area) {
  core::point p0(area.x() + 50, area.y2() - 50);

  scaler<double, scaling_type::log> xscale(0.01, 10, area.x() + 50, area.x2() - 50);
  scaler<double, scaling_type::log> yscale(0.01, 10000, area.y2() - 50, area.y() + 50);

  graph.draw(wall<double>(p0, xscale, yscale),
             color::very_very_light_gray, color::very_light_gray);

  graph.text(scale<double, orientation_t::horizontal, scaling_type::log>(p0, xscale, 1, 1, yscale.get_target_range(), yscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.text(scale<double, orientation_t::vertical, scaling_type::log>(p0, yscale, 1, 1, xscale.get_target_range(), xscale.get_target_range()),
             font::serif(), color::light_gray);

  graph.frame(line_graph<double, linear_data, scaling_type::log, scaling_type::log>(p0, xscale, yscale, linear_data()), color::red);
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_paint(draw::paint([&](const graphics& graph) {
    core::rectangle area = main.client_area();
    graph.clear(color::white);

    draw_graph_1(graph, area.with_size(area.size() / 2));
    draw_graph_2(graph, area.with_x(area.x2() / 2).with_size(area.size() / 2));
    draw_graph_3(graph, area.with_y(area.y2() / 2).with_size(area.size() / 2));
    draw_graph_4(graph, area.with_pos(area.x2y2() / 2).with_size(area.size() / 2));

  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Diagrams");
  main.set_visible();

  return run_main_loop();
}
