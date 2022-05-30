

#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/std_dialogs.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::horizontal_adaption<>> main;
  virtual_view<file_list<path_tree::sorted_path_info,
               default_file_item_drawer,
               core::selector::multi>> multi_client;
  virtual_view<file_list<path_tree::sorted_path_info,
               default_file_item_drawer,
               core::selector::single>> single_client;
  virtual_view<file_list<path_tree::sorted_path_info,
               default_file_item_drawer,
               core::selector::none>> none_client;

  multi_client->on_selection_commit([&] () {
    auto path = multi_client->get_selected_path();
    if (sys_fs::is_directory(path)) {
      multi_client->set_path(path);
      multi_client.layout();
    }
  });
  multi_client->on_key_down<core::keys::up, core::state::alt>([&] () {
    auto path = multi_client->get_current_path();
    multi_client->set_path(path.parent_path());
    multi_client.layout();
  });
  single_client->on_key_down<core::keys::enter>([&] () {
    auto path = single_client->get_selected_path();
    if (sys_fs::is_directory(path)) {
      single_client->set_path(path);
      single_client.layout();
    }
  });
  single_client->on_key_down<core::keys::up, core::state::alt>([&] () {
    auto path = single_client->get_current_path();
    single_client->set_path(path.parent_path());
    single_client.layout();
  });
  single_client->on_left_btn_dblclk([&] (gui::os::key_state ks, const core::native_point& pt) {
    const int idx = single_client->get_index_at_point(single_client->surface_to_client(pt));
    auto path = single_client->get_selected_path();
    message_dialog::show(main, "Info", str_fmt() << "Double click at index: " << idx <<
                                                    "\nfor path; '" << path << "'", "Ok");

  });

  none_client->on_selection_commit([&] () {
    auto path = none_client->get_selected_path();
    if (sys_fs::is_directory(path)) {
      none_client->set_path(path);
      none_client.layout();
    }
  });

  main.add({multi_client, single_client, none_client});
  main.on_create([&] () {
    multi_client->set_path("/");
    single_client->set_path("/");
    none_client->set_path("/");
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("filelist");
  main.set_visible();

  return run_main_loop();
}
