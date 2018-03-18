
#define WITH_CTRL

#include <gui/win/container.h>
#ifdef WITH_CTRL
#include <gui/ctrl/control.h>
#endif // WITH_CTRL

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  gui::win::main_window main;

#ifdef WITH_CTRL
  main.on_paint(gui::draw::paint([&] (const gui::draw::graphics& g) {
    gui::ctrl::paint::text_item(g, main.client_area(), gui::color::very_light_gray, "Hello world", false, gui::text_origin::center);
  }));
#endif // WITH_CTRL
  main.create({50, 50, 800, 600});
  main.on_destroy(&gui::win::quit_main_loop);
  main.set_title("Hello world");
  main.set_visible();

  return gui::win::run_main_loop();
}
