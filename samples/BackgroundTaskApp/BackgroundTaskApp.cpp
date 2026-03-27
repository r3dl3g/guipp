

#include <util/ostreamfmt.h>
#include <util/fps_counter.h>
#include "gui/draw/datamap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/font.h"
#include "gui/draw/drawers.h"
#include "gui/draw/shared_datamap.h"
#include "gui/layout/split_layout.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/background_repeater.h"
#include "gui/ctrl/toggle_group.h"

#ifdef NDEBUG
# define IF_DEBUG_ELSE(a, b) b
#else
# define IF_DEBUG_ELSE(a, b) a
#endif

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
  buttons.add_button("Color");
  buttons.add_button("Gray");
  buttons.set_selection_index(0);

  typedef layout_main_window<footer_layout> mainview_t;
  mainview_t main;
  main.get_layout().set_header(lay(buttons));

  fps_counter fps;

#ifdef GUIPP_USE_XSHM
  int ms_delay = IF_DEBUG_ELSE(50, 20);
  int frame_step = 1;
  draw::shared_datamap img(core::size(800, 600));
#else
  int ms_delay = 500;
  int frame_step = 10;
  draw::bgramap img(core::size(800, 600));
#endif
  draw::datamap<pixel_format_t::GRAY> gray(img.native_size());

  int frame = 0;
  bool changed = false;
 
  auto draw_colors = [&] () {
    frame += frame_step;
    int step = std::abs(255 - frame % 511);
    auto data = img.get_data();
    for (int y = 0; y < data.height(); ++y) {
      auto row = data.row(y);
      for (int x = 0; x < data.width(); ++x) {
        auto& pixel = row[x];
        pixel.red = (x * 255) / data.width();
        pixel.green = (y * 255) / data.height();
        pixel.blue = step;
        pixel.alpha = 255;
      }
    }
    fps();
    changed = true;
    main.invalidate();
  };

  auto draw_gray = [&] () {
    frame += frame_step;
    auto data = gray.get_data();
    for (int y = 0; y < data.height(); ++y) {
      auto row = data.row(y);
      for (int x = 0; x < data.width(); ++x) {
        row[x] = (byte)std::abs(255 - (x + y + frame) % 511);
      }
    }
    img.get_data().copy_from(data);
    fps();
    changed = true;
    main.invalidate();
  };

  background_repeater task(std::chrono::milliseconds(ms_delay), [&] () {
    if (buttons.get_selection_index()) {
      draw_gray();
    } else {
      draw_colors();
    }
  });

  main.on_destroy(&quit_main_loop);
  main.set_title("BackgroundTaskApp");
  main.on_create([&] () {
    buttons.create(main);
  });

  main.create({50, 50, 800, 625});
  main.on_paint(draw::paint([&](graphics& g){
    if (changed) {
      g.copy_from(img, point::zero);
      g.text(draw::text(fps.str("FPS:"), point(5, 5)), font::system_bold(), color::white);
      g.text(draw::text(str_fmt() << "Frame:" << frame, point(5, 595), text_origin_t::bottom_left), font::system_bold(), color::black);
    }
  }));
  main.set_visible();
  task.start();

  auto ret = run_main_loop();

  return ret;
}
