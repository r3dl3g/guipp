

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::border::layouter<3, 3, 3, 3>> main;
  virtual_view<file_list<path_tree::sorted_path_info>> client;

  main.on_create([&] () {
    client.create(main, main.client_geometry());
    client.view.set_path("/"/*sys_fs::current_path().parent_path()*/);
    main.get_layout().set_center(layout::lay(client));

    client.view.on_selection_commit([&] () {
      auto path = client.view.get_selected_path();
      if (sys_fs::is_directory(path)) {
        client.view.set_path(path);
      }
    });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("filelist");
  main.set_visible();

  return run_main_loop();
}
