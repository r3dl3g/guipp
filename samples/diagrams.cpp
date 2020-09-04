

#include <gui/win/container.h>
#include <logging/core.h>
#include <gui/draw/diagram.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

struct sinus_data : public gui::draw::graph_data<double> {

  sinus_data (double delta = 0)
    : delta(delta)
  {}

  std::size_t size () const override {
    return 100;
  }

  gui::draw::point2d<double> at (std::size_t i) const override {
    const double x = static_cast<double>(i) / 50 * M_PI;
    const double y = std::sin(x*2 + delta * M_PI);
    return {x, y};
  }

  virtual ~sinus_data ()
  {}

private:
  double delta;
};


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::draw;

  main_window main;

  main.on_paint(draw::paint([&](const graphics& graph) {
    core::rectangle area = main.client_area();
    graph.clear(color::white);

    core::point p0(50, area.y2() - 50);
    core::point p1(50, area.height() / 2);

    auto fmt = [] (double i) {
      return ostreamfmt(std::fixed << std::setprecision(1) << i);
    };

    scaler<double> xscale(0, 6.5, 50, area.x2() - 50);
    scaler<double> yscale(-1.2, 1.2, area.y2() - 50, 50);

    graph.draw(wall<double>(p0, xscale, yscale, 1, 0.2, 0.2, 0.05),
               color::very_very_light_gray, color::very_light_gray);

    graph.text(scale<double, orientation_t::horizontal>(p0, xscale, 1, -3, 0.2, 1),
               font::serif(), color::black);
    graph.text(scale<double, orientation_t::horizontal>(p1, xscale, 1, -3, 0.2, 1),
               font::serif(), color::black);

    graph.text(scale<double, orientation_t::vertical>(p0, yscale, 0.2, 3, 0.05, 1, fmt),
               font::serif(), color::black);

    graph.fill(bar_graph<double>(p0, xscale, yscale, sinus_data(+0.5)), color::blue);
    graph.fill(points_graph<double>(p0, xscale, yscale, sinus_data(-1)), color::green);
    graph.frame(line_graph<double>(p0, xscale, yscale, sinus_data(-0.5)), color::red);

  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Diagrams");
  main.set_visible();

  return run_main_loop();
}
