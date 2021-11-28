
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/tile_view.h>
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

struct icon_drawer : public list_data {

  std::size_t size () const override { return static_cast<std::size_t>(icon_type::MAX); }

  void draw_at (std::size_t idx, graphics& graph, const core::rectangle& r, const brush& b, item_state state) const override {
    look::button_frame<look::look_and_feel_t::w95>(graph, r, true, state.is_selected(), state.is_hilited(), false);
    if (idx < size()) {
      draw::clip clp(graph, r);
      const auto radius =  r.width() / 2 - 10;
      const auto center = r.center().dy(-radius/3);
      const auto i = (icon_type)idx;
      const auto col = state.is_selected() ? color::red : color::black;
      draw::pen icon_pen(col, radius/8, draw::pen::Style::solid, draw::pen::Cap::round, draw::pen::Join::round);
      graph.frame(icon_t<icon_type::background>(center, radius), icon_pen);
      graph.frame(icon(i, center, radius), icon_pen);
      graph.text(text_box(icon_name(i), r.dy(-radius/3), text_origin_t::bottom_hcenter), font::system().with_size(radius/2), color::black);
    }
  }
};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  constexpr int COLUMNS = 11;
  constexpr int ROWS = 5;

  layout_main_window<footer_layout> main;
  horizontal_scroll_bar scrollbar;
  vertical_scrollable_tile_view icons_view;
  group_window<horizontal_adaption<2, 10>> statusbar;
  label current_size;
  text_button adjust_button(const_text("Adjust"));
  check_box<> autoadjust_button(const_text("Auto adjust"));

  scrollbar.set_min_max_step_page(20, 400, 1, 10);
  scrollbar.set_value(50, false);

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
    const auto sz = round(std::min(area.width()/COLUMNS, area.height()/ROWS));
    scrollbar.set_value(sz);
  };

  adjust_button.on_clicked(adjust);
  autoadjust_button.on_clicked([&] () {
    if (autoadjust_button.is_checked()) {
      icons_view.layout();
    }
  });

  scrollbar.on_scroll([&] (point::type s) {
    const point::type v = round(s);
    icons_view->set_item_size({ v, v * 3 / 2});
    icons_view.layout();
    icons_view->invalidate();
    current_size.invalidate();
  });
  main.on_key_up<keys::up>([&] () {
    scrollbar.set_value(scrollbar.get_value() + 1);
  });
  main.on_key_up<keys::down>([&] () {
    scrollbar.set_value(scrollbar.get_value() - 1);
  });

  icons_view->set_item_size({ 50, 75 });
  icons_view->set_background(color::very_very_light_gray);
  icons_view->set_spacing({ 5, 5 });
  icons_view->set_data(icon_drawer());
  main.on_size([&] (core::size) {
    if (autoadjust_button.is_checked()) {
      adjust();
    }
  });

  main.create({50, 50, 480, 360});
  main.on_destroy(&quit_main_loop);
  main.set_title("Icons");
  main.set_visible();

  return run_main_loop();
}
