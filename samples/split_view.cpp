
#include <gui/win/container.h>
#include <logging/core.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  using label_t = basic_label<text_origin_t::center,
                              draw::frame::raised_deep_relief,
                              color::black,
                              color::very_light_gray>;

  layout_main_window<vertical_adaption<>> main;
  typedef horizontal_split_view<label_t, label_t> labels;
  vertical_split_view<label_t, labels> client;

  main.get_layout().add(lay(client));
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
  main.set_title("Split view");
  main.set_visible();

  return run_main_loop();
}
