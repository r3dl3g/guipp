
#define WITH_CTRL

#include <gui/win/container.h>
#ifdef WITH_CTRL
#include <gui/win/layout_container.h>
#include <gui/win/adaption_layout.h>
#include <gui/ctrl/label.h>
#endif // WITH_CTRL

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

#ifdef WITH_CTRL
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<vertical_adaption<>> main;
  label_center left_label;
  label_center right_label;

  main.on_create([&] (window* parent, const rectangle& rect) {
    left_label.create(main);
    left_label.set_text([&] () { return ostreamfmt(left_label.get_id() << " (" << left_label.place() << ")"); });
    right_label.create(main);
    right_label.set_text([&] () { return ostreamfmt(right_label.get_id() << " (" << right_label.place() << ")"); });
  });
  main.on_lost_focus([&] (window* next) {
    main.set_title(ostreamfmt("Lost focus to: " << (next ? next->get_id() : 0)));
  });
  main.on_set_focus([&] (window* prev) {
    main.set_title(ostreamfmt("Got focus from: " << (prev ? prev->get_id() : 0)));
  });
//  main.on_paint(gui::draw::paint([&] (const gui::draw::graphics& g) {
//    gui::ctrl::paint::text_item(g, main.client_area(), gui::color::very_light_gray, "Hello world", false, gui::text_origin::center);
//  }));
#else
  main_window  main;
#endif // WITH_CTRL

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello world");
  main.set_visible();

  return run_main_loop();
}
