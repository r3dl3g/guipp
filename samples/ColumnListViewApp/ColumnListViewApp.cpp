
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/column_list.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<border::layouter</*50, 50, 50, 50*/>> main;

  typedef ctrl::column_list_t<layout::simple_column_list_layout, core::selector::single,
                              std::string, int, double> list_t;
  typedef ctrl::const_column_list_data<std::string, int, double>::row_type row_type;

  list_t list;

  auto columns = {
    layout::simple_column_info{ 100, text_origin_t::vcenter_left, 20 },
    layout::simple_column_info{ 100, text_origin_t::center, 20 },
    layout::simple_column_info{ 100, text_origin_t::vcenter_right, 20 }
  };
  list.get_column_layout().set_columns(columns);

  list.set_data(ctrl::const_column_list_data<std::string, int, double> {
                   row_type{ "Eins", 1, 1.1 },
                   row_type{ "ZWei", 2, 2.2 },
                   row_type{ "Drei", 3, 3.3 },
                   row_type{ "Vier", 4, 4.4 },
                   row_type{ "Fünf", 5, 5.5 },
                   row_type{ "Sechs", 6, 6.6 },
                   row_type{ "Sieben", 7, 7.7 },
                   row_type{ "Acht", 8, 8.8 },
                   row_type{ "Neun", 9, 9.9 },
                   row_type{ "Zehn", 10, 10.10 },
                   row_type{ "Elf", 11, 11.11 },
                   row_type{ "Zwölf", 12, 12.12 },
                 });
  list.header.set_labels({"Text", "Int", "Double"});

  main.get_layout().set_center(lay(list));
  main.on_create([&] () {
    list.create(main, main.client_geometry());
 });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("column list view");
  main.set_visible();

  return run_main_loop();
}
