
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  using label_t = basic_label<text_origin_t::center,
                              draw::frame::raised_deep_relief>;

  layout_main_window<border::layouter</*50, 50, 50, 50*/>> main;
  typedef horizontal_split_view<label_t, label_t> labels;
  vertical_split_view<label_t, labels> client;

  main.get_layout().set_center(lay(client));
  main.on_create([&] () {
    client.create(main, main.client_geometry());
    client.set_split_pos(0.5);
    client.first.set_text([&] () { return ostreamfmt(client.first << " Left (" << client.first.geometry() << ")"); });
    client.second.set_split_pos(0.5);
    client.second.first.set_text([&] () { return ostreamfmt(client.second.first << " Top (" << client.second.first.geometry() << ")"); });
    client.second.second.set_text([&] () { return ostreamfmt(client.second.second << " Bottom (" << client.second.second.geometry() << ")"); });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Split view 2");
  main.set_visible();

  return run_main_loop();
}
