
#include <gui/win/overlapped_window.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/draw/icons.h>

#include <gui/core/grid.h>
#include <util/ostreamfmt.h>


using namespace gui;
using namespace gui::draw;

// --------------------------------------------------------------------------
void draw_text (text_origin_t t, graphics& graph, const core::rectangle& area, const std::string text) {
  logging::trace() << "Draw graph " << text << " in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  const auto pt = area.center();
  core::rectangle bounds{pt, core::size::zero};
  draw::bounding_box(text, bounds, t)(graph, font::system(), color::black);
  graph.fill(draw::rectangle(bounds), color::light_gray);

  graph.frame(draw::icon_t<icon_type::cross>(pt, 3), color::red);

  graph.text(draw::text(text, pt, t), font::system(), color::black);
}
// --------------------------------------------------------------------------
void draw_text_box (text_origin_t t, graphics& graph, const core::rectangle& area, const std::string text) {
  logging::trace() << "Draw graph " << text << " in area:" << area;

  graph.frame(draw::rectangle(area), color::black);

  core::rectangle rect = area.shrinked({10, 10});
  core::rectangle bounds = rect;
  draw::bounding_box(text, bounds, t)(graph, font::system(), color::black);
  graph.fill(draw::rectangle(bounds), color::light_gray);

  graph.frame(draw::rectangle(rect), color::red);
  graph.frame(draw::icon_t<icon_type::cross>(rect.center(), 3), color::red);

  graph.text(draw::text_box(text, rect, t), font::system(), color::black);
}
// --------------------------------------------------------------------------
void draw_standard_text (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Single Text:" << t);
  draw_text(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_singleline_text (text_origin_t t_, graphics& graph, const core::rectangle& area) {
  text_origin_t t = t_ | line_handling_t::singleline;
  const std::string text = ostreamfmt("Double Text:\n" << t);
  draw_text(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_double_text (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Double Text:\n" << t);
  draw_text(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_multiline_text (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Tripple\nText:\n" << t);
  draw_text(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_standard_text_box (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Single Box:" << t);
  draw_text_box(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_singleline_text_box (text_origin_t t_, graphics& graph, const core::rectangle& area) {
  text_origin_t t = t_ | line_handling_t::singleline;
  const std::string text = ostreamfmt("Double Box:\n" << t);
  draw_text_box(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_double_text_box (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Double Box:\n" << t);
  draw_text_box(t, graph, area, text);
}
// --------------------------------------------------------------------------
void draw_multiline_text_box (text_origin_t t, graphics& graph, const core::rectangle& area) {
  const std::string text = ostreamfmt("Tripple\nBox:\n" << t);
  draw_text_box(t, graph, area, text);
}
// --------------------------------------------------------------------------
typedef void (text_function)(text_origin_t, graphics&, const core::rectangle&);

template<text_function F>
void draw_it (graphics& graph, const core::rectangle& area, int n, int m = 0) {
  core::grid<6, 12> g(area);
  F(text_origin_t::bottom_right, graph, g(m + 0, n));
  F(text_origin_t::bottom_hcenter, graph, g(m + 1, n));
  F(text_origin_t::bottom_left, graph, g(m + 2, n));
  F(text_origin_t::vcenter_right, graph, g(m + 0, n + 1));
  F(text_origin_t::center, graph, g(m + 1, n + 1));
  F(text_origin_t::vcenter_left, graph, g(m + 2, n + 1));
  F(text_origin_t::top_right, graph, g(m + 0, n + 2));
  F(text_origin_t::top_hcenter, graph, g(m + 1, n + 2));
  F(text_origin_t::top_left, graph, g(m + 2, n + 2));
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

    draw_it<draw_standard_text>(graph, area, 0, 0);
    draw_it<draw_singleline_text>(graph, area, 0, 3);
    draw_it<draw_double_text>(graph, area, 3, 0);
    draw_it<draw_multiline_text>(graph, area, 3, 3);
    draw_it<draw_standard_text_box>(graph, area, 6, 0);
    draw_it<draw_singleline_text_box>(graph, area, 6, 3);
    draw_it<draw_double_text_box>(graph, area, 9, 0);
    draw_it<draw_multiline_text_box>(graph, area, 9, 3);

    logging::trace() << "on_paint finished";
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Text");
  main.set_visible();

  return run_main_loop();
}
