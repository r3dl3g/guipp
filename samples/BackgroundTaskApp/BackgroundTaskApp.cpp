

#include <gui/win/overlapped_window.h>
#include <gui/win/background_repeater.h>
#include <gui/draw/datamap.h>
#include <gui/draw/graphics.h>
#include "gui/draw/font.h"
#include "gui/draw/drawers.h"
#include <util/fps_counter.h>
// #include <gui/ctrl/button.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::draw;
  using namespace gui::core;

  main_window main;
  // gui::ctrl::text_button ok_button(const_text("Click me!"));

  draw::rgbamap img(core::size(800, 600));

  int frame = 0;
  auto draw_colors = [&] () {
    ++frame;
    auto data = img.get_data();
    for (int y = 0; y < data.height(); ++y) {
      auto row = data.row(y);
      for (int x = 0; x < data.width(); ++x) {
          byte r = (x * 255) / data.width();
          byte g = (y * 255) / data.height();
          byte b = std::abs(256 - frame % 512);
          row[x] = {r, g, b, 0};
      }
    }
    main.invalidate();
  };
  draw_colors();

  background_repeater task(main, std::chrono::milliseconds(40), draw_colors);
  fps_counter fps;

  main.on_destroy(&quit_main_loop);
  main.set_title("BackgroundTaskApp");

  main.create({50, 50, 800, 600});
  main.on_paint(draw::paint([&](graphics&g){
    fps();
    g.copy_from(img, point::zero);
    g.text(draw::text(fps.str("FPS:"), point(5, 5)), font::system_bold(), color::white);
  }));
  // ok_button.create(main, rectangle(100, 550, 600, 50));
  main.set_visible();
  task.start();

  // ok_button.on_clicked([] () {
  //   quit_main_loop();
  // });

  return run_main_loop();
}
