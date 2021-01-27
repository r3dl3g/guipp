
#include <fstream>

#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/editbox.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/layout/layout.h>
#include <util/string_util.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  typedef layout_main_window<border::layouter<20>> mainview_t;
  typedef virtual_view<editbox> clientview_t;

  mainview_t main;
  clientview_t client;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu help_sub_menu;

  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      file_sub_menu.popup(menu);
    }),
    main_menu_entry("Edit", 'E', [&]() {
      edit_sub_menu.popup(menu);
    }),
    main_menu_entry("Help", 'H', [&]() {
      help_sub_menu.popup(menu);
    })
  });

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'o', [&]() {
      file_open_dialog::show(main, "Open File", "Open", "Cancel", [&] (container&, const sys_fs::path& name) {
        if (sys_fs::exists(name)) {
          std::ifstream file(name);
          client.view.set_text(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
        }
      });
    }, hot_key(keys::o, state::control), true),
    menu_entry("Exit", 'x', &gui::win::quit_main_loop, hot_key(keys::f4, state::alt), true)
  });

  edit_sub_menu.data.add_entries({
    menu_entry("Cut", 't', [](){}, hot_key('X', state::control), false),
    menu_entry("Copy", 'C', [](){}, hot_key('C', state::control), false),
    menu_entry("Paste", 'P', [](){}, hot_key('V', state::control), false),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(main, "About gui++ editor", "gui++ editor version 0.0.1", "Ok");
    })
  );

  main.on_create([&] () {
    menu.create(main);
    menu.set_visible();

    client.create(main, main.client_area());
    main.get_layout().set_top(lay(menu));
    main.get_layout().set_center(lay(client));
    main.set_children_visible();
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("gui++ editor");
  main.set_visible();

  return run_main_loop();
}
