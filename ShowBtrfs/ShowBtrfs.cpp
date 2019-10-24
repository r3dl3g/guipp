

#include <gui/win/container.h>
#include <gui/win/layout_container.h>
#include <gui/win/adaption_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<vertical_adaption<>> main;
  typedef horizontal_split_view<label_center, label_center> labels;
  vertical_split_view<label_center, labels> client;

  main.on_create([&] (window* parent, const rectangle& rect) {
    client.create(main, rect);
    client.set_split_pos(0.5);
    client.second.set_split_pos(0.5);
    client.first.set_text([&] () { return ostreamfmt(client.first.get_id() << " (" << client.first.place() << ")"); });
    client.second.first.set_text([&] () { return ostreamfmt(client.second.first.get_id() << " (" << client.second.first.place() << ")"); });
    client.second.second.set_text([&] () { return ostreamfmt(client.second.second.get_id() << " (" << client.second.second.place() << ")"); });
  });
  main.on_lost_focus([&] (window* next) {
    main.set_title(ostreamfmt("Lost focus to: " << (next ? next->get_id() : 0)));
  });
  main.on_set_focus([&] (window* prev) {
    main.set_title(ostreamfmt("Got focus from: " << (prev ? prev->get_id() : 0)));
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello world");
  main.set_visible();

  return run_main_loop();
}
