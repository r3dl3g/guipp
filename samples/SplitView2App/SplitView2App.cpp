
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  using button_t = basic_text_button<push_button_traits,
                                     look::button_frame_t<look::look_and_feel_t::w95>,
                                     look::button_text>;

  using label_t = basic_label<text_origin_t::center, draw::frame::raised_deep_relief>;
  using client_top = vertical_split_view<label_t, label_t>;
  using client_mid = vertical_split_view<label_t, placeholder, label_t>;
  using client_bot = vertical_split_view<label_t, button_t, button_t, label_t>;
  using client_single = vertical_split_view<label_t>;
  using client_t = horizontal_split_view<client_top, client_mid, client_bot, client_single>;

  using center_deep = basic_label<text_origin_t::center, draw::frame::sunken_deep_relief>;
  using center_raised = basic_label<text_origin_t::center, draw::frame::raised_deep_relief>;

  center_deep deep_center_label([&] () { return ostreamfmt("Deep (" << deep_center_label.geometry() << ")"); });
  center_raised raised_center_label([&] () { return ostreamfmt("Raised (" << raised_center_label.geometry() << ")"); });

  client_t client;
  layout_main_window<border::center_layout<>, client_t&> main(client);

  auto& top = client.get<0>();
  top.get<0>().set_text([&] () { return ostreamfmt("Left (" << top.get<0>().geometry() << ")"); });
  top.get<1>().set_text([&] () { return ostreamfmt("Right (" << top.get<1>().geometry() << ")"); });

  auto& mid = client.get<1>();
  mid.get<0>().set_text([&] () { return ostreamfmt("Left (" << mid.get<0>().geometry() << ")"); });
  mid.get<1>().set(&deep_center_label);
  mid.get<2>().set_text([&] () { return ostreamfmt("Right (" << mid.get<2>().geometry() << ")"); });

  auto& bot = client.get<2>();
  bot.get<0>().set_text([&] () { return ostreamfmt("Left (" << bot.get<0>().geometry() << ")"); });
  bot.get<1>().set_text("Raise");
  bot.get<1>().on_clicked([&] () {
    deep_center_label.set_visible(false);
    mid.get<1>().set(&raised_center_label);
    raised_center_label.set_visible();
    mid.layout();
  });
  bot.get<2>().set_text("Deep");
  bot.get<2>().on_clicked([&] () {
    raised_center_label.set_visible(false);
    mid.get<1>().set(&deep_center_label);
    deep_center_label.set_visible();
    mid.layout();
  });
  bot.get<3>().set_text([&] () { return ostreamfmt("Right (" << bot.get<3>().geometry() << ")"); });

  auto& single = client.get<3>();
  single.get<0>().set_text([&] () { return ostreamfmt("Single (" << single.get<0>().geometry() << ")"); });

  main.set_background(color::blue);
  main.on_create([&] () {
    client.create(main);
    // not necessary
    // deep_center_label.create(mid);
    raised_center_label.create(mid);
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Split view 2");
  main.set_visible();

  return run_main_loop();
}
