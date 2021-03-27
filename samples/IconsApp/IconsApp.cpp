
#include <gui/win/overlapped_window.h>
#include <gui/draw/icons.h>
#include <gui/draw/pen.h>
#include <gui/core/grid.h>
#ifdef WIN32
#include <math.h>
#endif // GUIPP_WIN


using namespace gui;
using namespace gui::draw;

template<typename T>
void draw_icon (draw::graphics& g,
                  const core::rectangle& r) {
  static draw::pen icon_pen(color::black, 5, draw::pen::Style::solid, draw::pen::Cap::round);
  const auto radius = std::min(r.height(), r.width()) / 3;
  g.frame(T(r.center(), radius), icon_pen);
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_paint(draw::paint([&](graphics& graph) {
    clog::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    auto area = main.client_geometry();
    clog::trace() << "Draw graphs in area:" << area;

    core::grid<6, 5> g(area);
    draw_icon<up_icon>(graph, g(0, 0));
    draw_icon<down_icon>(graph, g(1, 0));
    draw_icon<left_icon>(graph, g(2, 0));
    draw_icon<right_icon>(graph, g(3, 0));
    draw_icon<add_icon>(graph, g(4, 0));
    draw_icon<remove_icon>(graph, g(5, 0));

    draw_icon<clock_icon>(graph, g(0, 1));
    draw_icon<stopwatch_icon>(graph, g(1, 1));
    draw_icon<timer_icon>(graph, g(2, 1));
    draw_icon<info_icon>(graph, g(3, 1));
    draw_icon<important_icon>(graph, g(4,1));
    draw_icon<clear_icon>(graph, g(5, 1));

    draw_icon<play_icon>(graph, g(0, 2));
    draw_icon<pause_icon>(graph, g(1, 2));
    draw_icon<stop_icon>(graph, g(2, 2));
    draw_icon<reset_icon>(graph, g(3, 2));
    draw_icon<back_icon>(graph, g(4, 2));
    draw_icon<menu_icon>(graph, g(5, 2));

    draw_icon<okay_icon>(graph, g(0, 3));
    draw_icon<cancel_icon>(graph, g(1, 3));
    draw_icon<close_icon>(graph, g(2, 3));
    draw_icon<find_icon>(graph, g(3, 3));
    draw_icon<zoom_in_icon>(graph, g(4, 3));
    draw_icon<zoom_out_icon>(graph, g(5, 3));

    draw_icon<undo_icon>(graph, g(0, 4));
    draw_icon<redo_icon>(graph, g(1, 4));
    draw_icon<sync_icon>(graph, g(2, 4));
    draw_icon<restart_icon>(graph, g(3, 4));
    draw_icon<off_icon>(graph, g(4, 4));

    clog::trace() << "on_paint finished";
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
