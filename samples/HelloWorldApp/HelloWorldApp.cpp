

#include <gui/win/overlapped_window.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/font.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::draw;

  main_window  main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.on_paint(paint([&](graphics& graph){
    graph.text(text_box("Hello world!",
                        main.client_geometry(),
                        text_origin_t::center),
               font::system(), color::black);
  }));

  main.set_title("Hello world");
  main.set_visible();

  return run_main_loop();
}
