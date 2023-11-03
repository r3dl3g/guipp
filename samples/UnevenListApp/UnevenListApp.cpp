

#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/uneven_list.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<border::layouter</*50, 50, 50, 50*/>> main;

  typedef ctrl::horizontal_scrollable_uneven_list list1_t;
  typedef ctrl::vertical_scrollable_uneven_list list2_t;
  typedef ctrl::vertical_split_view<list1_t, list2_t> left_split_view_t;

  typedef ctrl::horizontal_scrollable_multi_uneven_list list3_t;
  typedef ctrl::vertical_scrollable_multi_uneven_list list4_t;
  typedef ctrl::vertical_split_view<list3_t, list4_t> right_split_view_t;

  typedef ctrl::horizontal_split_view<left_split_view_t, right_split_view_t> main_split_view_t;

  std::vector<std::string> data = { "Eins", "Zwei", "Drei", "View", "Fünf", "Sechs",
                                    "Sieben", "Acht", "Neun", "Zehn", "Elf", "Zwölf" };

  main_split_view_t main_split_view;
  list1_t& list1 = main_split_view.first.first;
  list2_t& list2 = main_split_view.first.second;
  list3_t& list3 = main_split_view.second.first;
  list4_t& list4 = main_split_view.second.second;

  list1->set_data(ctrl::const_list_data<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  list1->set_size_function([](int i, const core::size&){
    return i * 5 + 20;
  });
  list2->set_data(ctrl::indirect_list_data<std::string>(data));
  list2->set_size_function([&](int i, const core::size&){
    return data[i].size() * 8;
  });
  list3->set_data(ctrl::calc_list_data<std::string>(
    [](std::size_t idx) {
      return util::string::convert::from(idx);
    },[]() {
      return 100;
    }
  ));
  list3->set_size_function([&](int i, const core::size&){
    return i * 5 + 20;
  });
  list4->set_data(ctrl::indirect_list_data<std::string>(data));
  list4->set_size_function([&](int i, const core::size&){
    return data[i].size() * 8;
  });

  main.get_layout().set_center(lay(main_split_view));
  main.on_create([&] () {
    main_split_view.create(main, main.client_geometry());
 });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("UnevenListApp");
  main.set_visible();

  return run_main_loop();
}
