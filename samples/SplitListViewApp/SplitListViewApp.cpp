
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

  typedef ctrl::edit_list list1_t;
  typedef ctrl::vertical_scrollable_list list2_t;
  typedef ctrl::vertical_split_view<list1_t, list2_t> left_split_view_t;

  typedef ctrl::horizontal_scrollable_list list3_t;
  typedef ctrl::column_list_t<layout::simple_column_list_layout, core::selector::single,
                              std::string, int, double> list4_t;
  typedef ctrl::vertical_split_view<list3_t, list4_t> right_split_view_t;

  typedef ctrl::horizontal_split_view<left_split_view_t, right_split_view_t> main_split_view_t;

  std::vector<std::string> data = { "Eins", "Zwei", "Drei", "View", "Fünf", "Sechs",
                                    "Sieben", "Acht", "Neun", "Zehn", "Elf", "Zwölf" };

  main_split_view_t main_split_view;
  list1_t& list1 = main_split_view.get<0>().get<0>();
  list2_t& list2 = main_split_view.get<0>().get<1>();
  list3_t& list3 = main_split_view.get<1>().get<0>();
  list4_t& list4 = main_split_view.get<1>().get<1>();

  list1.set_data(ctrl::indirect_list_data<std::string>(data));
  list1.on_selection_changed([&](ctrl::event_source) {
    main_split_view.invalidate();
  });

  list1.set_data_source_and_target([&](int idx) {
    return data[idx];
  }, [&](int idx, const std::string& s) {
    data[idx] = s;
  });

  list2->set_data(ctrl::indirect_list_data<std::string>(data));
  list3->set_data(ctrl::const_list_data<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});

  auto columns = {
    layout::simple_column_info{ 100, text_origin_t::vcenter_left, 20 },
    layout::simple_column_info{ 100, text_origin_t::center, 20 },
    layout::simple_column_info{ 100, text_origin_t::vcenter_right, 20 }
  };
  list4.get_column_layout().set_columns(columns);

  typedef ctrl::const_column_list_data<std::string, int, double> column_data_type;
  typedef ctrl::const_column_list_data<std::string, int, double>::row_type row_type;
  list4.set_data(column_data_type({
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
                 }));
  list4.header.set_labels({"Text", "Int", "Double"});

  main.get_layout().set_center(lay(main_split_view));
  main.on_create([&] () {
    main_split_view.create(main, main.client_geometry());
 });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Split view");
  main.set_visible();

  return run_main_loop();
}
