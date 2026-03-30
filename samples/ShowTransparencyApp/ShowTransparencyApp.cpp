

#include "gui/draw/datamap.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/font.h"
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

  draw::bgramap bimg(core::native_size(256, 256));
  draw::bgramap fimg(core::native_size(256, 256));

  auto bdat = bimg.get_data();
  auto fdat = fimg.get_data();
  for (int y = 0; y < bdat.height(); ++y) {
    auto row = bdat.row(y);
    auto frow = fdat.row(y);
    for (int x = 0; x < bdat.width(); ++x) {
      auto& bpix = row[x];
      bpix.red = x;
      bpix.green = y;
      bpix.blue = std::abs(256 - x + y);
      bpix.alpha = color::transparency<>::off;

      auto& fpix = frow[x];
      fpix = color::invert(bpix);
      // fpix.red = y;
      // fpix.green = 256 - x;
      // fpix.blue = std::abs(256 - y + x);
      // fpix.alpha = color::transparency<>::off;
    }
  }

  logging::trace() << "Create draw::pixmap";
  draw::pixmap bpix(bimg);
  draw::pixmap fpix(fimg);

  buttons.add_buttons({"0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%"});

  auto calc = [&] () {
    int a = (buttons.get_selection_index() * 255) / 10;
    auto fdat = fimg.get_data();
    for (int y = 0; y < fdat.height(); ++y) {
      auto frow = fdat.row(y);
      for (int x = 0; x < fdat.width(); ++x) {
        auto& fpix = frow[x];
        fpix.alpha = a;
      }
    }

    fpix = fimg;
  };

  main.on_destroy(&quit_main_loop);
  main.set_title("ShowTransparencyApp");

  main.on_create([&] () {
    buttons.create(main);
  });
  main.create({50, 50, 800, float(600 + main.get_layout().size)});

  const int ty = 25;
  const int iy = 50;

  const int x0 = 50;
  const int x1 = x0 + 256 + 5;
  const int x2 = x1 + 256 + 5;

  point p0(x0, iy);
  point p1(x1, iy);
  point p2(x2, iy);

  const core::rectangle r0(x0, ty, 256 + 5, 25);
  const core::rectangle r1(x1, ty, 256 + 5, 25);
  const core::rectangle r2(x2, ty, 256 + 5, 25);
  const core::rectangle rx( 0, iy, x2 + 256 + x0, iy + 256);

  const font big_font = font::system().with_size(160);

  main.on_paint(draw::paint([&](graphics& graph) {
    graph.clear(color::white);

    int a = buttons.get_selection_index() * 10;

    graph.text(text_box("Background", rx, text_origin_t::center), big_font, color::gray);

    graph.text(text_box("Background", r0, text_origin_t::center), font::system(), color::black);
    std::string info = ostreamfmt("Inverse + " << a << "% Transparency");
    graph.text(text_box(info, r1, text_origin_t::center), font::system(), color::black);
    graph.text(text_box("Both together", r2, text_origin_t::center), font::system(), color::black);

    graph.copy_from(bpix, p0);
    graph.copy_from(fpix, p1);
    graph.copy_from(bpix, p2);
    graph.copy_from(fpix, p2);
  }));
  buttons.on_selection_changed([&] (event_source) {
    calc();
    main.invalidate();
  });
  main.set_visible();
  buttons.set_selection_index(5);

  return run_main_loop();
}
