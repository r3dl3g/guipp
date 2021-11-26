
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/split_layout.h>
#include <gui/draw/icons.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/core/grid.h>
#ifdef WIN32
#include <math.h>
#endif // GUIPP_WIN


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::ctrl;
using namespace gui::draw;
using namespace gui::core;

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  constexpr int COLUMNS = 12;
  constexpr int ROWS = 5;

  layout_main_window<footer_layout> main;
  horizontal_scroll_bar scrollbar;
  client_control icons_view;
  group_window<horizontal_adaption<2, 10>> statusbar;
  label current_size;
  text_button adjust_button(const_text("Adjust"));
  check_box<> autoadjust_button(const_text("Auto adjust"));

  scrollbar.set_min_max_step_value(1, 100, 1, 10);

  main.get_layout().set_header_and_body(lay(statusbar), lay(icons_view));

  statusbar.get_layout().add({&current_size, &scrollbar, &adjust_button, &autoadjust_button});

  statusbar.on_create([&] () {
    current_size.create(statusbar);
    scrollbar.create(statusbar);
    adjust_button.create(statusbar);
    autoadjust_button.create(statusbar);
  });
  main.on_create([&] () {
    icons_view.create(main);
    statusbar.create(main);
  });
  current_size.set_text([&] () {
    return ostreamfmt("Current size: " << scrollbar.get_value());
  });
  auto adjust = [&] () {
    const auto area = icons_view.client_geometry();
    scrollbar.set_value(std::min(area.width()/(COLUMNS*2)-10, area.height()/(ROWS*2)-20));
  };

  adjust_button.on_clicked(adjust);
  autoadjust_button.on_clicked([&] () {
    if (autoadjust_button.is_checked()) {
      icons_view.invalidate();
    }
  });

  scrollbar.on_scroll([&] (point::type) {
    icons_view.invalidate();
    current_size.invalidate();
  });
  main.on_key_up<keys::up>([&] () {
    scrollbar.set_value(scrollbar.get_value() + 1);
  });
  main.on_key_up<keys::down>([&] () {
    scrollbar.set_value(scrollbar.get_value() - 1);
  });

  icons_view.on_paint(draw::paint([&](graphics& graph) {
    logging::trace() << "Received on_paint, clear white";
    graph.clear(color::white);

    const auto area = icons_view.client_geometry();
    if (autoadjust_button.is_checked()) {
      adjust();
    }
    const auto radius =  scrollbar.get_value();
    logging::trace() << "Draw graphs in area:" << area;

    core::grid<COLUMNS, ROWS> g(area);
    g.for_each<icon_type>([&] (icon_type i, const core::rectangle& r) {
      frame::lines(graph, r), color::buttonColor();
      if (i < icon_type::background) {
        const auto center = r.center().dy(-radius/3);
        draw::pen icon_pen(color::black, radius/8, draw::pen::Style::solid, draw::pen::Cap::round, draw::pen::Join::round);
        graph.frame(icon_t<icon_type::background>(center, radius), icon_pen);
        graph.frame(icon(i, center, radius), icon_pen);
        graph.text(text_box(icon_name(i), r.dy(-radius/3), text_origin_t::bottom_hcenter), font::system().with_size(radius/2), color::black);
      }
    });

    logging::trace() << "on_paint finished";
  }));

  main.create({50, 50, 480, 360});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
