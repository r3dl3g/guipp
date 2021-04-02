
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

template<typename T>
void draw_icon (draw::graphics& g,
                const core::rectangle& r,
                const std::string& str) {
  static draw::pen icon_pen(color::black, 5, draw::pen::Style::solid, draw::pen::Cap::round);
  const auto radius = std::min(r.height(), r.width()) / 3;
  g.frame(T(r.center(), radius), icon_pen);
  g.text(text_box(str, r, text_origin_t::bottom_hcenter), font::system_small(), color::black);
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  main_window main;

  main.on_paint(draw::paint([&](graphics& graph) {
    clog::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    auto area = main.client_geometry().shrink({5, 5});
    clog::trace() << "Draw graphs in area:" << area;

    core::grid<6, 6> g(area);
    draw_icon<up_icon>(graph, g(0, 0), "up");
    draw_icon<down_icon>(graph, g(1, 0), "down");
    draw_icon<left_icon>(graph, g(2, 0), "left");
    draw_icon<right_icon>(graph, g(3, 0), "right");
    draw_icon<add_icon>(graph, g(4, 0), "add");
    draw_icon<remove_icon>(graph, g(5, 0), "remove");

    draw_icon<clock_icon>(graph, g(0, 1), "clock");
    draw_icon<stopwatch_icon>(graph, g(1, 1), "stopwatch");
    draw_icon<timer_icon>(graph, g(2, 1), "timer");
    draw_icon<info_icon>(graph, g(3, 1), "info");
    draw_icon<important_icon>(graph, g(4,1), "important");
    draw_icon<clear_icon>(graph, g(5, 1), "clear");

    draw_icon<play_icon>(graph, g(0, 2), "play");
    draw_icon<pause_icon>(graph, g(1, 2), "pause");
    draw_icon<stop_icon>(graph, g(2, 2), "stop");
    draw_icon<reset_icon>(graph, g(3, 2), "reset");
    draw_icon<back_icon>(graph, g(4, 2), "back");
    draw_icon<menu_icon>(graph, g(5, 2), "menu");

    draw_icon<okay_icon>(graph, g(0, 3), "okay");
    draw_icon<cancel_icon>(graph, g(1, 3), "cancel");
    draw_icon<close_icon>(graph, g(2, 3), "close");
    draw_icon<find_icon>(graph, g(3, 3), "find");
    draw_icon<zoom_in_icon>(graph, g(4, 3), "zoom_in");
    draw_icon<zoom_out_icon>(graph, g(5, 3), "zoom_out");

    draw_icon<undo_icon>(graph, g(0, 4), "undo");
    draw_icon<redo_icon>(graph, g(1, 4), "redo");
    draw_icon<forward_icon>(graph, g(2, 4), "forward");
    draw_icon<backward_icon>(graph, g(3, 4), "backward");
    draw_icon<sync_icon>(graph, g(4, 4), "sync");
    draw_icon<restart_icon>(graph, g(5, 4), "restart");

    draw_icon<off_icon>(graph, g(0, 5), "off");
    draw_icon<check_off_icon>(graph, g(1, 5), "check_off");
    draw_icon<check_on_icon>(graph, g(2, 5), "check_on");
    draw_icon<radio_off_icon>(graph, g(3, 5), "radio_off");
    draw_icon<radio_on_icon>(graph, g(4, 5), "radio_on");

    clog::trace() << "on_paint finished";
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
