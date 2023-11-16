
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/layout/split_layout.h>
#include <util/string_util.h>
#include <util/fs_util.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<header_layout> main;
  dir_file_view<> client;
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu help_sub_menu;

  menu.add_entries({
    main_menu_entry("File", 'F', menu, file_sub_menu),
    main_menu_entry("Edit", 'E', menu, edit_sub_menu),
    main_menu_entry("Window", 'W', [&]() {}, menu_state::disabled),
    main_menu_entry("Help", 'H', menu, help_sub_menu)
  });

  file_sub_menu.add_entry(
    menu_entry("Exit", 'x', &gui::win::quit_main_loop, hot_key(keys::f4, state::alt))
  );

  edit_sub_menu.add_entries({
    menu_entry("Cut", 't', [](){}, hot_key('X', state::control)),
    menu_entry("Copy", 'C', [](){}, hot_key('C', state::control)),
    menu_entry("Paste", 'P', [](){}, hot_key('V', state::control)),
  });

  help_sub_menu.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(main, "About Filemanager", "Filemanager Version 1.0.0", "Ok");
    })
  );

  main.on_create([&] () {
    menu.create(main);
    menu.view().set_visible();

    client.create(main, main.client_geometry());
    client.set_split_pos(0.5);
    main.get_layout().set_header_and_body(lay(menu.view()), lay(client));

    sys_fs::path current = sys_fs::current_path();
    auto& first = client.get<0>().view;
    first->set_roots(gui::fs::get_all_root_file_infos());
    for (auto next = current; next.has_root_path() && (next != next.root_path()); next = next.parent_path()) {
      first->add_open_node(next);
    }
    first->add_open_node(current.root_path());
    first->update_node_list();
    first->select_node(current);
    client.get<1>().set_path(current);
    client.init([] (container&, const sys_fs::path& path) {
      if (sys_fs::is_regular_file(path)) {
        const auto ret = util::fs::execute_or_open(path);
        logging::debug() << "exec return:" << ret;
      }
    });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("filemanager");
  main.set_visible();

  return run_main_loop();
}
