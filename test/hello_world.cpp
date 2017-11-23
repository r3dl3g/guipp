
#include <gui/win/container.h>
#include <gui/ctrl/control.h>

#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  gui::win::main_window main;

  main.register_event_handler(REGISTER_FUNCTION, gui::win::paint_event([&] (const gui::draw::graphics& g) {
    gui::win::paint::text_item(g, main.client_area(), gui::color::very_light_gray, "Hello world", false, gui::text_origin::center);
  }));
  main.create({50, 50, 800, 600});
  main.set_title("Hello world");
  main.set_visible();

  return gui::win::run_main_loop();
}
