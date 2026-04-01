
#include <fstream>

#include <gui/win/clipboard.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/layout/split_layout.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  typedef layout_main_window<header_layout> mainview_t;

  mainview_t main;
  client_control client;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu context_menu;

  auto about = [&]() {
    message_dialog::show(main, "About gui++ Menu", "gui++ menu sampleversion 1.0.0", "Ok");
  };
  auto quit = [&] () {
    // yes_no_dialog::ask(main, "Close App", "nDo you realy want to quit?", "Yes", "No",
    //                     [&] (win::container&, bool yes) {
    //   if (yes) {
        gui::win::quit_main_loop();
    //   }
    // });
  };

  menu.add_entries({
    main_menu_entry("File", 'F', menu, file_sub_menu),
    main_menu_entry("Edit", 'E', menu, edit_sub_menu)
  });

  file_sub_menu.add_entries({
    menu_entry("Open", 'o', about),
    menu_entry("Save", 's', about),
    menu_entry("Save as", 'a', about),
    menu_entry("Exit", 'x', quit, hot_key(keys::f4, state::alt), true)
  });

  edit_sub_menu.add_entries({
    menu_entry("Cut", 't', about),
    menu_entry("Copy", 'C', about, hot_key(), true),
    menu_entry("Paste", 'P', about),
    sub_menu_entry("More", 'M', [&] () {
      context_menu.popup(edit_sub_menu);
    }),
  });

  context_menu.add_entries({
    menu_entry("About", 'A', about),
    menu_entry("Disable", 'D', about),
    menu_entry("Enable", 'E', about)
  });

  client.on_right_btn_down ([&] (gui::os::key_state, const core::native_point& pt) {
    context_menu.popup_at(client.surface_to_client(pt), client);
  });

  main.get_layout().set_header_and_body(lay(menu.view()), lay(client));

  main.on_create([&] () {
    menu.create(main);
    client.create(main);
  });
  main.on_close(quit);

  main.create({50, 50, 800, 600});
  main.on_destroy(quit_main_loop);
  main.set_title("gui++ menu");
  main.set_visible();

  return run_main_loop();
}
