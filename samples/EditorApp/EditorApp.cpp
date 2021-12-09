
#include <fstream>

#include <gui/win/clipboard.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/editbox.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/layout/split_layout.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  typedef layout_main_window<header_layout> mainview_t;
  typedef virtual_view<editbox> clientview_t;

  mainview_t main;
  clientview_t client;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu help_sub_menu;

  sys_fs::path file_name;
  bool content_changed = false;

  menu.add_entries({
    main_menu_entry("File", 'F', menu, file_sub_menu),
    main_menu_entry("Edit", 'E', menu, edit_sub_menu),
    main_menu_entry("Help", 'H', menu, help_sub_menu)
  });

  client.view.on_content_changed([&] () {
    content_changed = true;
  });

  auto open = [&]() {
    file_open_dialog::show(main, "Open file", "Open", "Cancel", [&] (container&, const sys_fs::path& name) {
      if (sys_fs::exists(name)) {
        file_name = name;
        std::ifstream file(file_name.string());
        client.view.set_text(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
        content_changed = false;
      }
    });
  };
  auto save_as = [&] () {
    file_save_dialog::show(main, "Save file as", "new_file.txt", "Filename:", "Save", "Cancel",
                           [&] (container&, const sys_fs::path& name) {
      file_name = name;
      std::ofstream(file_name.string()) << client.view.get_text();
      content_changed = false;
    });
  };
  auto save = [&] () {
    if (file_name.empty()) {
      save_as();
    } else {
      std::ofstream(file_name.string()) << client.view.get_text();
      content_changed = false;
    }
  };
  auto quit = [&] () {
    if (content_changed) {
      yes_no_dialog::ask(main, "Content has", "Content has changed\nDo you realy want to quit without save?", "Yes", "No",
                         [&] (win::overlapped_window&, bool yes) {
        if (yes) {
          gui::win::quit_main_loop();
        }
      });
    } else {
      gui::win::quit_main_loop();
    }
  };
  auto cut = [&] () {
    clipboard::get().set_text(main, client.view.cut_selected_text());
  };
  auto copy = [&] () {
    clipboard::get().set_text(main, client.view.get_selected_text());
  };
  auto paste = [&] () {
    clipboard::get().get_text(main, [&] (const std::string& t) {
      client.view.replace_selection(t);
    });
  };
  auto about = [&]() {
    message_dialog::show(main, "About gui++ editor", "gui++ sample editor version 1.0.0", "Ok");
  };

  file_sub_menu.add_entries({
    menu_entry("Open", 'o', open, hot_key(keys::o, state::control)),
    menu_entry("Save", 's', save, hot_key(keys::s, state::control)),
    menu_entry("Save as", 'a', save_as, hot_key(keys::s, state::control | state::shift)),
    menu_entry("Exit", 'x', quit, hot_key(keys::f4, state::alt), true)
  });
  file_sub_menu.register_hot_keys(&main);

  edit_sub_menu.add_entries({
    menu_entry("Cut", 't', cut, hot_key('X', state::control), false),
    menu_entry("Copy", 'C', copy, hot_key('C', state::control), false),
    menu_entry("Paste", 'P', paste, hot_key('V', state::control), false),
  });
  edit_sub_menu.register_hot_keys(&main);

  help_sub_menu.add_entry(
    menu_entry("About", 'A', about)
  );

  main.get_layout().set_header_and_body(lay(menu.view()), lay(client));

  main.on_create([&] () {
    menu.create(main);
    client.create(main);
  });
  main.on_close(quit);

  main.create({50, 50, 800, 600});
  main.on_destroy(quit_main_loop);
  main.set_title("gui++ editor");
  main.set_visible();

  return run_main_loop();
}
