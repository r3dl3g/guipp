

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/table.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<gui::layout::border::layouter<24>> main;

  table_edit table_view;
  table::data::matrix<std::string> table_data("Test");

  table_view.set_scroll_maximum({2000, 2000});
  table_view.set_data_source_and_target(
    [&](const ctrl::table::position& cell) -> std::string {
      return table_data.get_cell(cell);
    },
    [&](const ctrl::table::position& cell, const std::string& s) {
      table_data.set_cell(cell, s);
    }
  );
  table_view.data.set_drawer(ctrl::table::default_data_drawer([&](const ctrl::table::position& cell) -> std::string {
    return table_data.get_cell(cell);
  }));
  table_view.columns.set_drawer(ctrl::table::default_header_drawer([](const ctrl::table::position& cell) -> std::string {
    return ctrl::table::build_std_column_name(cell.x());
  }));
  table_view.rows.set_drawer(ctrl::table::default_header_drawer([](const ctrl::table::position& cell) -> std::string {
    return ostreamfmt((1 + cell.y()));
  }));
  table_view.edge.set_text("0:0");
  table_view.enable_size(true, true);

  main.on_create([&] () {
    table_view.create(main);

    main.get_layout().set_center(lay(table_view));
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Table View");
  main.set_visible();

  return run_main_loop();
}
