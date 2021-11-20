
#include <gui/win/overlapped_window.h>
#include <gui/draw/icons.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/core/grid.h>
#ifdef WIN32
#include <math.h>
#endif // GUIPP_WIN


using namespace gui;
using namespace gui::draw;

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_paint(draw::paint([&](graphics& graph) {
    logging::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    auto area = main.client_geometry().shrink({5, 5});
    logging::trace() << "Draw graphs in area:" << area;

    core::grid<6, 10> g(area);
    g.for_each<icon_type>([&] (icon_type i, const core::rectangle& r) {
      if (i < icon_type::background) {
        const auto radius = std::min(r.height(), r.width()) / 3;
        draw::pen icon_pen(color::black, radius/8, draw::pen::Style::solid, draw::pen::Cap::round, draw::pen::Join::round);
        graph.frame(icon_t<icon_type::background>(r.center().dy(-radius/5), radius), icon_pen);
        graph.frame(icon(i, r.center().dy(-radius/5), radius), icon_pen);
        graph.text(text_box(icon_name(i), r, text_origin_t::bottom_hcenter), font::system().with_size(radius/2), color::black);
      }
    });

    logging::trace() << "on_paint finished";
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
