
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;

  using label_t = basic_label<text_origin_t::center, draw::frame::raised_deep_relief>;
  using client_t = vertical_split_view<label_t, label_t>;

  client_t client;
  layout_main_window<border::center_layout<>, client_t&> main(client);

  main.on_create([&] () {
    client.create(main);
    client.set_split_pos(0.5);
    client.get<0>().set_text([&] () { return ostreamfmt("Left (" << client.get<0>().geometry() << ")"); });
    client.get<1>().set_text([&] () { return ostreamfmt("Right (" << client.get<1>().geometry() << ")"); });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Split view");
  main.set_visible();

  return run_main_loop();
}
