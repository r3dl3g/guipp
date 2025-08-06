
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

  menu.add_entries({
    main_menu_entry("File", 'F', menu, file_sub_menu),
  });

  auto open = [&]() {
    dir_open_dialog::show(main, "Open Project", "Open", "Cancel", [&] (container&, const sys_fs::path& dirname) {
      if (sys_fs::exists(dirname)) {
        std::ifstream file((dirname / "project.txt").string());
        client.view.set_text(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
      }
    }, [] (const sys_fs::directory_entry& e) -> bool {
      return !e.is_directory() || !util::string::ends_with(e.path().string(), ".prj");
    }, [] (const sys_fs::directory_entry& e) -> bool {
      return util::string::ends_with(e.path().string(), ".prj");
    });
  };
  auto open_file = [&]() {
    file_open_dialog::show(main, "Open File", "Open", "Cancel", [&] (container&, const sys_fs::path& filename) {
      if (sys_fs::exists(filename)) {
        std::ifstream file(filename.string());
        client.view.set_text(std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
      }
    }, [] (const sys_fs::directory_entry& e) -> bool {
      return !util::string::ends_with(e.path().string(), ".txt");
    });
  };
  auto save = [&] () {
    file_save_dialog::show(main, "Save Project as", "new_project.prj", "Project name:", "Save", "Cancel",
                           [&] (container&, const sys_fs::path& path) {
      sys_fs::path dirname = path;
      if (!util::string::ends_with(dirname.string(), ".prj")) {
        dirname.replace_extension("prj");
      }

      sys_fs::create_directories(dirname);
      std::ofstream((dirname / "project.txt").string()) << client.view.get_text();
    });
  };
  auto about = [&]() {
    message_dialog::show(main, "About ProjectDirApp", "gui++ sample ProjectDirApp version 1.0.0", "Ok");
  };

  file_sub_menu.add_entries({
    menu_entry("Open project", 'o', open, hot_key(keys::o, state::control)),
    menu_entry("Open file", 'f', open_file, hot_key(keys::f, state::control)),
    menu_entry("Save", 's', save, hot_key(keys::s, state::control)),
    menu_entry("About", 'a', about, hot_key(), true),
    menu_entry("Exit", 'x', quit_main_loop, hot_key(keys::f4, state::alt), true)
  });

  main.get_layout().set_header_and_body(lay(menu.view()), lay(client));

  main.on_create([&] () {
    menu.create(main);
    client.create(main);
    file_sub_menu.register_hot_keys(&main);
  });
  main.on_close(quit_main_loop);

  client.view.set_font(draw::font::system_bold().with_size(14));
  main.create({50, 50, 800, 600});
  main.on_destroy(quit_main_loop);
  main.set_title("gui++ ProjectDirApp");
  main.set_visible();

  return run_main_loop();
}
