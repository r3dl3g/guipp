

#include "gui/draw/datamap.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/layout/split_layout.h"
#include "gui/win/overlapped_window.h"
#include "gui/ctrl/toggle_group.h"


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::core;
  using namespace gui::draw;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;

  using button_type = tab_button<alignment_t::bottom, true>;
  ctrl::htoggle_group<button_type> buttons;

  typedef layout_main_window<footer_layout> mainview_t;
  mainview_t main;
  main.get_layout().set_header(lay(buttons));

  logging::trace() << "Create draw::rgbmap";
  draw::sysmap img(core::native_size(512, 256));

  auto data = img.get_data();
  for (int y = 0; y < data.height(); ++y) {
    auto row = data.row(y);
    for (int x = 0; x < data.width(); ++x) {
      auto& pixel = row[x];
      pixel.red = x;
      pixel.green = y;
      pixel.blue = std::abs(256 - x + y);
      pixel.alpha = 255;
    }
  }

  logging::trace() << "Create draw::pixmap";
  draw::pixmap pixmap;
#ifndef GUIPP_SDL
  pixmap = img;
#endif

  buttons.add_button("1:1");
  auto filters = draw::graphics::get_filter_list(pixmap);
  for (auto f : filters) {
    buttons.add_button(f);
  }

  main.on_destroy(&quit_main_loop);
  main.set_title("DrawStrechedApp");

  main.on_create([&] () {
    buttons.create(main);
  });

  main.on_paint(draw::paint([&](graphics& g){
    // logging::trace() << "Call main.on_paint";
    if (!pixmap.is_valid()) {
      pixmap = img;
    }
    int i = buttons.get_selection_index();
    if (i == 0) {
      g.copy_from(pixmap, point::zero);
    } else {
      g.draw_streched(pixmap, {main.get_layout().body_size(main.client_geometry())}, core::point::zero, filters[i - 1]);
    }
  }));
  buttons.on_selection_changed([&] (event_source) {
    main.invalidate();
  });

  main.create({50, 50, 800, float(600 + main.get_layout().size)});
  main.set_visible();
  buttons.set_selection_index(0);

  auto ret = run_main_loop();

  return ret;
}
