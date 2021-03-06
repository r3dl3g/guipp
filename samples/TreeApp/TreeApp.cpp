

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/file_tree.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::border::layouter<25, 25, 25, 25>> main;
  virtual_view<sorted_dir_tree> client;

  main.on_create([&] () {
    client.create(main, main.client_geometry());

    sys_fs::path current = sys_fs::current_path();
    client->set_roots(fs::get_all_root_file_infos());
    client->add_open_node(current.root_path());
    client->update_node_list();

    main.get_layout().set_center(layout::lay(client));
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("dirtree");
  main.set_visible();

  return run_main_loop();
}
