

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/file_tree.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::border::layouter<10, 10, 10, 10>> main;
  file_list<path_tree::sorted_path_info> client;

  main.on_create([&] (window* parent, const rectangle& rect) {
    client.create(main, rect);
    client.set_path(sys_fs::current_path().parent_path());
    main.get_layout().set_center(layout::lay(client));
    main.set_children_visible();

    client.on_selection_commit([&] () {
      auto path = client.get_selected_path();
      if (sys_fs::is_directory(path)) {
        client.set_path(path);
      }
    });
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("filelist");
  main.set_visible();

  return run_main_loop();
}
