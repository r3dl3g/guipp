
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::border_layout<>, float, float, float, float> main(20,0,0,0);
  dir_file_view<> client;
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
    main_menu_entry("Window", 'W', [&]() {
    }, menu_state::disabled),
    main_menu_entry("Help", 'H', [&]() {
      help_sub_menu.popup(menu);
    })
  });

  file_sub_menu.data.add_entry(
    menu_entry("Exit", 'x', &quit_main_loop, hot_key(keys::f4, state::alt), true)
  );

  edit_sub_menu.data.add_entries({
    menu_entry("Cut", 't', [](){}, hot_key('X', state::control), false),
    menu_entry("Copy", 'C', [](){}, hot_key('C', state::control), false),
    menu_entry("Paste", 'P', [](){}, hot_key('V', state::control), false),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(main, "About Filemanager", "Filemanager Version 0.0.1", "Ok");
    })
  );

  main.on_create([&] (window* parent, const rectangle& rect) {
    menu.create(main);
    menu.set_visible();

    client.create(main, rect);
    client.set_split_pos(0.5);
    main.get_layout().set_top(&menu);
    main.get_layout().set_center(&client);
    main.set_children_visible();

    sys_fs::path current = sys_fs::current_path();
    client.first.root = current.root_path();
    for (auto next = current; next.has_root_path(); next = next.parent_path()) {
      client.first.add_open_node(next);
    }
    client.first.update_node_list();
    client.first.select_node(current);
    client.second.set_path(current);
    client.init([] (const sys_fs::path& path) {
      if (sys_fs::is_regular_file(path)) {
        LogDebug << "exec return:" << std::system(path.c_str());
      }
    });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("filemanager");
  main.set_visible();

  return run_main_loop();
}
