
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

template<icon_t T>
void draw_icon (draw::graphics& g,
                const core::rectangle& r,
                const std::string& str) {
  const auto radius = std::min(r.height(), r.width()) / 3;
  draw::pen icon_pen(color::black, radius/8, draw::pen::Style::solid, draw::pen::Cap::round, draw::pen::Join::round);
  g.frame(icon<icon_t::background>(r.center().dy(-radius/5), radius), icon_pen);
  g.frame(icon<T>(r.center().dy(-radius/5), radius), icon_pen);
  g.text(text_box(str, r, text_origin_t::bottom_hcenter), font::system().with_size(radius/2), color::black);
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

    core::grid<6, 9> g(area);
    draw_icon<icon_t::up>(graph, g(0, 0), "up");
    draw_icon<icon_t::down>(graph, g(1, 0), "down");
    draw_icon<icon_t::left>(graph, g(2, 0), "left");
    draw_icon<icon_t::right>(graph, g(3, 0), "right");
    draw_icon<icon_t::add>(graph, g(4, 0), "add");
    draw_icon<icon_t::remove>(graph, g(5, 0), "remove");

    draw_icon<icon_t::clock>(graph, g(0, 1), "clock");
    draw_icon<icon_t::stopwatch>(graph, g(1, 1), "stopwatch");
    draw_icon<icon_t::timer>(graph, g(2, 1), "timer");
    draw_icon<icon_t::info>(graph, g(3, 1), "info");
    draw_icon<icon_t::important>(graph, g(4,1), "important");
    draw_icon<icon_t::clear>(graph, g(5, 1), "clear");

    draw_icon<icon_t::play>(graph, g(0, 2), "play");
    draw_icon<icon_t::pause>(graph, g(1, 2), "pause");
    draw_icon<icon_t::stop>(graph, g(2, 2), "stop");
    draw_icon<icon_t::reset>(graph, g(3, 2), "reset");
    draw_icon<icon_t::back>(graph, g(4, 2), "back");
    draw_icon<icon_t::menu>(graph, g(5, 2), "menu");

    draw_icon<icon_t::okay>(graph, g(0, 3), "okay");
    draw_icon<icon_t::cancel>(graph, g(1, 3), "cancel");
    draw_icon<icon_t::cross>(graph, g(2, 3), "cross");
    draw_icon<icon_t::find>(graph, g(3, 3), "find");
    draw_icon<icon_t::zoom_in>(graph, g(4, 3), "zoom_in");
    draw_icon<icon_t::zoom_out>(graph, g(5, 3), "zoom_out");

    draw_icon<icon_t::undo>(graph, g(0, 4), "undo");
    draw_icon<icon_t::redo>(graph, g(1, 4), "redo");
    draw_icon<icon_t::forward>(graph, g(2, 4), "forward");
    draw_icon<icon_t::backward>(graph, g(3, 4), "backward");
    draw_icon<icon_t::sync>(graph, g(4, 4), "sync");
    draw_icon<icon_t::restart>(graph, g(5, 4), "restart");

    draw_icon<icon_t::check_off>(graph, g(0, 5), "check_off");
    draw_icon<icon_t::check_on>(graph, g(1, 5), "check_on");
    draw_icon<icon_t::radio_off>(graph, g(2, 5), "radio_off");
    draw_icon<icon_t::radio_on>(graph, g(3, 5), "radio_on");
    draw_icon<icon_t::open>(graph, g(4, 5), "open");
    draw_icon<icon_t::close>(graph, g(5, 5), "close");

    draw_icon<icon_t::off>(graph, g(0, 6), "off");
    draw_icon<icon_t::settings>(graph, g(1, 6), "settings");
    draw_icon<icon_t::person>(graph, g(2, 6), "person");
    draw_icon<icon_t::trash>(graph, g(3, 6), "trash");
    draw_icon<icon_t::file>(graph, g(4, 6), "file");
    draw_icon<icon_t::folder>(graph, g(5, 6), "folder");

    draw_icon<icon_t::folder_open>(graph, g(0, 7), "folder_open");
    draw_icon<icon_t::list>(graph, g(1, 7), "list");
    draw_icon<icon_t::grid>(graph, g(2, 7), "grid");
    draw_icon<icon_t::columns>(graph, g(3, 7), "columns");
    draw_icon<icon_t::lock>(graph, g(4, 7), "lock");
    draw_icon<icon_t::unlock>(graph, g(5, 7), "unlock");

    draw_icon<icon_t::up_arrow>(graph, g(0, 8), "up_arrow");
    draw_icon<icon_t::down_arrow>(graph, g(1, 8), "down_arrow");
    draw_icon<icon_t::left_arrow>(graph, g(2, 8), "left_arrow");
    draw_icon<icon_t::right_arrow>(graph, g(3, 8), "right_arrow");

    clog::trace() << "on_paint finished";
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
