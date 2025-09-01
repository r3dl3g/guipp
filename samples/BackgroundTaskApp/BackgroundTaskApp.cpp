

#include <gui/win/overlapped_window.h>
#include <gui/win/background_repeater.h>
#include <gui/draw/datamap.h>
#include <gui/draw/graphics.h>
#include "gui/draw/font.h"
#include "gui/draw/drawers.h"
#include "gui/draw/shared_datamap.h"
#include <util/fps_counter.h>
// #include <gui/ctrl/button.h>

#ifdef NDEBUG
# define IF_DEBUG_ELSE(a, b) b
#else
# define IF_DEBUG_ELSE(a, b) a
#endif

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::draw;
  using namespace gui::core;

  main_window main;
  // gui::ctrl::text_button ok_button(const_text("Click me!"));

#ifdef GUIPP_USE_XSHM
  int ms_delay = IF_DEBUG_ELSE(50, 20);
  int frame_step = 1;
  draw::shared_datamap img(core::size(800, 600));
#else
  int ms_delay = 500;
  int frame_step = 10;
  draw::rgbmap img(core::size(800, 600));
#endif

  int frame = 0;
  auto draw_colors = [&] () {
    frame += frame_step;
    int step = std::abs(256 - frame % 512);
    auto data = img.get_data();
    for (int y = 0; y < data.height(); ++y) {
      auto row = data.row(y);
      for (int x = 0; x < data.width(); ++x) {
          byte r = (x * 255) / data.width();
          byte g = (y * 255) / data.height();
          byte b = step;
          row[x] = {r, g, b};
      }
    }
    main.invalidate();
  };
  draw_colors();

  background_repeater task(main, std::chrono::milliseconds(ms_delay), draw_colors);
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
