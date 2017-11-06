
// --------------------------------------------------------------------------
//
// Common includes
//
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/string_util.h>
#include <gui/core/logger.h>

#include <gui/win/dbg_win_message.h>
#include <gui/win/attach_layout.h>
#include <gui/win/adaption_layout.h>
#include <gui/win/lineup_layout.h>
#include <gui/win/grid_layout.h>

#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/list.h>
#include <gui/ctrl/scroll_view.h>
#include <gui/ctrl/slider.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/column_list.h>
#include <gui/ctrl/separator.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/table.h>
#include <gui/ctrl/editbox.h>
#include <gui/ctrl/tile_view.h>


std::ostream& operator<<(std::ostream& out, const bool& b) {
  out << (b ? "true" : "false");
  return out;
}

#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)

using namespace gui;

class log_all_events {
public:
  log_all_events() {
  }

  bool operator()(const core::event& e, os::event_result& result) {
    if ((result == 0x0) &&
        !win::is_none_client_event(e) &&
        !win::is_frequent_event(e) ) {
      LogDebug << "Message: " << win::EventId(e) << IF_WIN32_ELSE(" (" << std::hex << e.wParam << ", " << e.lParam << ")", "");
    }
    return false;
  }
};

class init_result_handler {
public:
  init_result_handler() {
  }

  bool operator()(const core::event& e, os::event_result& result) {
    return false;
  }
};

std::vector<core::point> calc_star(core::point::type x, core::point::type y, core::point::type w, core::point::type h) {
  core::point::type x1 = x + w / 4;
  core::point::type x2 = x + w / 2;
  core::point::type x3 = x + w * 3 / 4;
  core::point::type x4 = x + w;
  core::point::type y1 = y + h / 4;
  core::point::type y2 = y + h / 2;
  core::point::type y3 = y + h * 3 / 4;
  core::point::type y4 = y + h;
  return{
    core::point(x, y), core::point(x2, y1), core::point(x4, y),
    core::point(x3, y2), core::point(x4, y4), core::point(x2, y3),
    core::point(x, y4), core::point(x1, y2), core::point(x, y)
  };
}

template<draw::frame::drawer F = draw::frame::sunken_relief>
void tile_drawer (std::size_t idx,
                  const draw::graphics& g,
                  const core::rectangle& place,
                  const draw::brush& background,
                  bool selected,
                  bool hilited) {
  using namespace draw;

  win::paint::text_cell<std::size_t, F>(static_cast<int>(idx), g, place,
                                        text_origin::center, color::black,
                                        background.color(), selected, hilited);
}

namespace gui {

  namespace win {

    inline std::string color_to_string (const os::color& c) {
      return ostreamfmt("#" << std::setfill('0') << std::setw(6) << std::hex << c);
    }

    template<>
    void default_drop_down_drawer<os::color> (const os::color& c,
                                              const draw::graphics& graph,
                                              const core::rectangle& place,
                                              const draw::brush&,
                                              bool selected,
                                              bool hilited) {
      graph.fill(draw::rectangle(place), c);
      graph.text(draw::text_box(color_to_string(c), place, text_origin::center), draw::font::system(), color::invert(c));

      if (hilited) {
        graph.frame(draw::rectangle(place), color::highLightColor());
      } else if (selected) {
        graph.frame(draw::rectangle(place), color::black);
      }
    }

  } // namespace win

} // namespace gui

// --------------------------------------------------------------------------
class my_main_window : public win::layout_main_window<layout::attach> {
public:
  typedef win::layout_main_window<layout::attach> super;
  my_main_window (win::paint_event p1, win::paint_event p2);

  void onCreated (win::window*, const core::rectangle&);
  void created_children ();
  void query_state ();
  void set_size_null ();

  static win::paint_event create_paint1();
  static win::paint_event create_paint2();

  typedef super::clazz<my_main_window, color::very_very_light_gray> myclazz;

  void create (const core::rectangle& r = core::rectangle::def) {
    super::create(myclazz::get(), r);
  }

private:
  win::scroll_view scroll_view;

  win::client_window<> window1;
  win::client_window<> window2;

  win::horizontal_separator hseparator;
  win::vertical_separator vseparator;

  win::text_button calc_button;
  win::text_button inc_button;
  win::text_button dec_button;

  win::text_button ok_button;
  win::text_button del_button;
  win::text_button clear_button;

  win::vertical_separator btn_sep1;
  win::vertical_separator btn_sep2;

  win::group_window<layout::horizontal_adaption<5, 10, 2, 50, 90, origin::end>, color::very_light_gray> btn_group;
  win::group_window<layout::vertical_adaption<5, 5>> chck_group;

  win::group_window<layout::horizontal_adaption<5, 5>, color::dark_gray> group_group;

  win::group_window<layout::horizontal_lineup<30, 5, 5>> h_lineup_group;
  win::label h_lineup_labels[4];

  win::group_window<layout::vertical_lineup<30, 5, 5>> v_lineup_group;
  win::label v_lineup_labels[4];

  win::group_window<layout::grid_lineup<30, 30, 5, 5>> grid_lineup_group;
  win::label grid_lineup_labels[4];

  win::group_window<layout::grid_adaption<2, 2, 5, 5>> grid_adaption_group;
  win::label grid_adaption_labels[4];

  win::group_window<layout::horizontal_lineup<30, 5, 5, 2, origin::center>> hc_lineup_group;
  win::label hc_lineup_labels[4];

  win::group_window<layout::vertical_lineup<30, 5, 5, 2, origin::center>> vc_lineup_group;
  win::label vc_lineup_labels[4];

  win::radio_button<> radio_button, radio_button2;
  win::check_box<> check_box;
  win::label label;
  win::basic_label<text_origin::center, draw::frame::no_frame, color::blue, color::light_gray> labelC;
  win::label_right labelR;

  win::text_button min_button;
  win::text_button max_button;
  win::text_button norm_button;
  win::text_button info_button;
  win::text_button null_button;
  win::text_button full_button;

  win::simple_list_data<std::string> data;

  typedef win::vertical_list List1;
  typedef win::edit_list List2;
  typedef win::vertical_list List3;

  List1 list1;
  List2& list2;
  List3& list3;

  typedef win::vertical_split_view<List2, List3> list_split_view;
  typedef win::simple_column_list<layout::simple_column_list_layout> simple_list;
  typedef win::vertical_split_view<win::horizontal_list, simple_list> column_list_split_view;

  win::horizontal_split_view<list_split_view, column_list_split_view> main_split_view;

  win::text_button up_button;
  win::text_button down_button;

  win::vertical_scroll_bar vscroll;
  win::horizontal_scroll_bar hscroll;

  win::check_box<> scroll_check_box;

  win::horizontal_slider hslider;
  win::vertical_slider vslider;
  const win::paint_event paint1;
  const win::paint_event paint2;

  win::edit edit1;

  win::group_window<layout::horizontal_adaption<0, 2>> edit_btn_group;
  win::text_button cur_plus;
  win::text_button cur_minus;
  win::text_button sel_first_plus;
  win::text_button sel_first_minus;
  win::text_button sel_last_plus;
  win::text_button sel_last_minus;

  win::custom_push_button custom_button;
  win::drop_down_list<std::string> drop_down;
  win::drop_down_list<os::color> color_drop_down;

  typedef win::column_list_t<layout::weight_column_list_layout, int, std::string, float, int, bool> my_column_list_t;
  my_column_list_t column_list;
  //my_column_list_t::standard_data column_list_data;
  my_column_list_t::row_drawer column_list_drawer;

  win::table_edit table_view;
  win::table::data::matrix<std::string> table_data;

  typedef win::virtual_view<win::editbox> editbox_view;
  editbox_view editor;

  typedef win::basic_textbox<text_origin::top_right, draw::frame::sunken_relief, color::dark_blue, color::very_very_light_gray> textbox_type;
  typedef win::virtual_view<textbox_type> textbox_view;

  textbox_view textbox;

  win::horizontal_tile_view htileview;
  win::vertical_tile_view vtileview;

  bool at_paint1;
  bool at_drag;
  core::point last_pos;

  bool calc_pressed;
};

int gui_main(const std::vector<std::string>& args) {

  win::paint_event paint1(my_main_window::create_paint1());
  win::paint_event paint2(my_main_window::create_paint2());

  my_main_window main(paint1, paint2);

  LogDebug << "window size:" << sizeof(main)  << ", window_class_info size:" << sizeof(win::class_info);
  LogDebug << "long size:" << sizeof(long)<< ", pointer size:" << sizeof(void*);
  size_t str_size = sizeof(std::string);
  size_t evc_size = sizeof(core::event_container);
  size_t win_size = sizeof(win::window);
  size_t cln_size = sizeof(win::client_window<>);
  size_t btn_size = sizeof(win::button_base);
  size_t push_size = sizeof(win::button_base);
  size_t tgl_size = sizeof(win::basic_button<win::push_button_traits>);
  size_t tbtn_size = sizeof(win::text_button);
  size_t pnt_size = sizeof(win::paint_event);

  LogInfo << "Sizes: "
          <<   "std::string:" << str_size
          << ", event_container:" << evc_size
          << ", window:" << win_size
          << ", client_window:" << cln_size
          << ", button:" << btn_size
          << ", push_button:" << push_size
          << ", toggle_button:" << tgl_size
          << ", text_button:" << tbtn_size
          << ", paint_event:" << pnt_size;

//#ifdef WIN32
//  main.register_event_handler(REGISTER_FUNCTION, win::get_minmax_event([](const core::size& sz,
//    const core::point& pos,
//    core::size& mi, core::size& ma) {
//    mi = { 300, 200 };
//    ma = { 880, 660 };
//    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
//  }));
//#endif

  const core::rectangle r = core::rectangle(50, 50, 1000, 800);
  LogDebug << "Create Main: " << r;
  main.create(r);
  main.set_title("Window Test");

  main.set_visible();

  return win::run_main_loop();
}

my_main_window::my_main_window (win::paint_event p1, win::paint_event p2)
  : list2(main_split_view.first.first)
  , list3(main_split_view.first.second)
  , paint1(p1)
  , paint2(p2)
  , column_list(my_column_list_t(20, color::very_very_light_gray))
  , table_view(50, 20)
  , table_data(std::string())
  , at_paint1(true)
  , at_drag(false)
  , calc_pressed(false)
{
  main_split_view.second.second.list.set_item_size_and_background(16, color::very_light_gray);
  list1.set_item_size(25);
  list3.set_item_size_and_background(20, color::light_gray);

  register_event_handler(REGISTER_FUNCTION, init_result_handler(), 0);

  register_event_handler(REGISTER_FUNCTION, win::destroy_event([&] () {
    LogDebug << "Destroyed!";
    win::quit_main_loop();
  }));

  register_event_handler(REGISTER_FUNCTION, win::show_event([&] () {
    btn_group.layout();
    group_group.layout();
  }));

//#ifdef WIN32
//  register_event_handler(REGISTER_FUNCTION, win::close_event([&]() {
//    LogDebug << "Close!";
//    destroy();
//  }));
//  register_event_handler(REGISTER_FUNCTION, win::enable_event([](bool on) {
//    LogDebug << (on ? "Enableed" : "Disabled");
//  }));
//  register_event_handler(REGISTER_FUNCTION, win::activate_event([](bool on, win::window* win) {
//    LogDebug << "Main " << (on ? "activate" : "deactivate");
//  }));
//  register_event_handler(REGISTER_FUNCTION, win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
//  register_event_handler(REGISTER_FUNCTION, win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
//  register_event_handler(REGISTER_FUNCTION, win::activate_app_event([](bool on) {
//    LogDebug << (on ? "A" : "Dea") << "ctivate App";
//  }));

//#endif

  //register_event_handler(REGISTER_FUNCTION, win::moving_event([] (const core::point& r) {
  //  LogDebug << "Main moving: " << r;
  //}));
  //register_event_handler(REGISTER_FUNCTION, win::sizing_event([] (const core::size& r) {
  //  LogDebug << "Main sizing: " << r;
  //}));
  //register_event_handler(REGISTER_FUNCTION, win::placing_event([] (const core::rectangle& r) {
  //  LogDebug << "Main placing: " << r;
  //}));

  //register_event_handler(REGISTER_FUNCTION, win::move_event([] (const core::point& p) {
  //  LogDebug << "Main move: " << p;
  //}));
  //register_event_handler(REGISTER_FUNCTION, win::size_event([] (const core::size& s) {
  //  LogDebug << "Main size: " << s;
  //}));
  //register_event_handler(REGISTER_FUNCTION, win::place_event([] (const core::rectangle& r) {
  //  LogDebug << "Main place: " << r;
  //}));

//#ifdef WIN32
//  ok_button.register_event_handler(REGISTER_FUNCTION, win::activate_event([](bool on, win::window* win) {
//    LogDebug << "Button " << (on ? "" : "de") << "activate";
//  }));
//#endif
  ok_button.register_event_handler(REGISTER_FUNCTION, win::set_focus_event([] (win::window* win) {
    LogDebug << "Button Set Focus";
  }));
  ok_button.register_event_handler(REGISTER_FUNCTION, win::lost_focus_event([&] (win::window* win) {
    LogDebug << "Button Lost Focus";
  }));

  register_event_handler(REGISTER_FUNCTION, win::left_btn_down_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Left Button Down at " << p;
  }));
  register_event_handler(REGISTER_FUNCTION, win::left_btn_up_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Left Button Up at " << p;
  }));
  register_event_handler(REGISTER_FUNCTION, win::right_btn_down_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Right Button Down at " << p;
  }));
  register_event_handler(REGISTER_FUNCTION, win::right_btn_up_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Right Button Up at " << p;
  }));
  window1.register_event_handler(REGISTER_FUNCTION, win::wheel_x_event([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(delta, 0));
    }
  }));
  window1.register_event_handler(REGISTER_FUNCTION, win::wheel_y_event([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  }));

  window1.register_event_handler(REGISTER_FUNCTION, win::left_btn_down_event([&](os::key_state, const core::point& p) {
    at_drag = true;
    last_pos = p;
    window1.capture_pointer();
    LogDebug << "Window1 Mouse down at " << p;
  }));
  window1.register_event_handler(REGISTER_FUNCTION, win::left_btn_up_event([&](os::key_state, const core::point& p) {
    window1.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window Mouse up at " << p;
  }));

  window2.register_event_handler(REGISTER_FUNCTION, win::left_btn_down_event([&](os::key_state, const core::point& p) {
    at_drag = true;
    last_pos = p;
    window2.capture_pointer();
    LogDebug << "Window2 Mouse down at " << p;
  }));
  window2.register_event_handler(REGISTER_FUNCTION, win::left_btn_up_event([&](os::key_state, const core::point& p) {
    window2.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window Mouse up at " << p;
  }));

  window1.register_event_handler(REGISTER_FUNCTION, win::mouse_move_event([&] (os::key_state keys,
                                                                               const core::point& p) {
    //LogDebug << "Window Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      auto delta = p - last_pos;
      //last_pos = p;
      window1.move(window1.position() + delta);
    }
  }));
  window2.register_event_handler(REGISTER_FUNCTION, win::mouse_move_event([&] (os::key_state keys,
                                                                               const core::point& p) {
    //LogDebug << "Window Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      auto delta = p - last_pos;
      //last_pos = p;
      window2.move(window2.position() + delta);
    }
  }));

  window1.register_event_handler(REGISTER_FUNCTION, win::left_btn_dblclk_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Window1 Double Click up at " << p;
    window2.set_visible(!window2.is_visible());
  }));

  register_event_handler(REGISTER_FUNCTION, win::left_btn_dblclk_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Double Click up at " << p;

    core::point pos = window1.position();
    core::size sz = window1.size();
    LogDebug << "Pos: " << pos << " Size " << sz;

    core::rectangle pl = window1.place();
    LogDebug << "Place: " << pl;

    core::rectangle apl = window1.absolute_place();
    LogDebug << "Abs Place: " << apl;

    core::point apos = window1.absolute_position();
    LogDebug << "Abs Pos: " << apos;

    core::rectangle car = window1.client_area();
    LogDebug << "Client: " << car;
  }));
  register_event_handler(REGISTER_FUNCTION, win::right_btn_dblclk_event([&] (os::key_state, const core::point& p) {
    window1.move({50, 50});
  }));

  window2.register_event_handler(REGISTER_FUNCTION, paint1);

  at_paint1 = true;

  window2.register_event_handler(REGISTER_FUNCTION, win::left_btn_dblclk_event([&] (os::key_state, const core::point& p) {
    LogDebug << "Window2 Double Click up at " << p;
    if (at_paint1) {
      at_paint1 = false;
      window2.unregister_event_handler(paint1);
      window2.register_event_handler(REGISTER_FUNCTION, paint2);
    } else {
      at_paint1 = true;
      window2.unregister_event_handler(paint2);
      window2.register_event_handler(REGISTER_FUNCTION, paint1);
    }
    window2.redraw_later();
  }));
  window2.register_event_handler(REGISTER_FUNCTION, win::show_event([] () {
    LogDebug << "Window2 show:";
  }));
  window2.register_event_handler(REGISTER_FUNCTION, win::hide_event([] () {
    LogDebug << "Window2 hide:";
  }));

//#ifdef WIN32
//  ok_button.register_event_handler(REGISTER_FUNCTION, win::button_state_event([](bool state) {
//    LogDebug << "Button " << (state ? "hilited" : "unhilited");
//  }));
//#endif // WIN32
  ok_button.register_event_handler(REGISTER_FUNCTION, win::button_pushed_event([&] () {
    LogDebug << "Button pushed";
    label.set_text("Pushed!");
    label.redraw_now();
  }));
  ok_button.register_event_handler(REGISTER_FUNCTION, win::button_released_event([&] () {
    LogDebug << "Button released";
    label.set_text("Released!");
    label.redraw_now();
  }));
  radio_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Radio clicked";
    labelR.set_text("Radio clicked!");
    bool check = radio_button.is_checked();
//    radio_button.set_checked(!check);
    radio_button2.set_checked(!check);
  }));
  radio_button2.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Radio2 clicked";
    labelR.set_text("Radio2 clicked!");
    bool check = radio_button2.is_checked();
//    radio_button2.set_checked(!check);
    radio_button.set_checked(!check);
  }));
  check_box.register_event_handler(REGISTER_FUNCTION, win::button_state_event([&] (bool on) {
    LogDebug << "Check clicked";
    label.set_text("Check clicked!");
    radio_button.enable(on);
    radio_button2.enable(on);
    ok_button.enable(on);
    vscroll.enable(on);
    hscroll.enable(on);
    table_view.set_enable_edit(on);
  }));

  min_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Min clicked";
    minimize();
    query_state();
  }));
  max_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Max clicked";
    maximize();
    query_state();
  }));
  norm_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Norm clicked";
    restore();
    query_state();
  }));
  info_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Info clicked";
    query_state();
    textbox.view.set_text(editor.view.get_selected_text());
    textbox.layout();
  }));
  null_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "0 clicked";
    vslider.set_value(vslider.get_min());
  }));
  full_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Full clicked";
    vslider.set_value(size().width() - 15);
  }));

  auto list_drawer = [] (std::size_t idx,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& background,
                         bool selected,
                         bool hilited) {
    using namespace draw;

    std::ostringstream strm;
    strm << "Item " << idx;

    win::paint::text_item(g, place, background, strm.str(), selected);
  };

  list1.set_drawer(list_drawer);
  list1.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](win::event_source) {
    labelC.set_text(ostreamfmt("List1 item " << list1.get_selection()));
  }));
  list1.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event([&] () {
    labelC.set_text(ostreamfmt("List1 commited " << list1.get_selection()));
  }));

  up_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    list1.set_selection(list1.get_selection() - 1, win::event_source::mouse);
    list2.set_selection(list2.get_selection() - 1, win::event_source::mouse);
    list3.set_selection(list3.get_selection() - 1, win::event_source::mouse);
  }));
  down_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    list1.set_selection(list1.get_selection() + 1, win::event_source::mouse);
    list2.set_selection(list2.get_selection() + 1, win::event_source::mouse);
    list3.set_selection(list3.get_selection() + 1, win::event_source::mouse);
  }));

  data.insert(data.end(), { "Eins", "Zwei", "Drei", "View", "Fünf", "Fuß" });
  list2.set_drawer([&] (std::size_t idx,
                        const draw::graphics& g,
                        const core::rectangle& place,
                        const draw::brush& background,
                        bool selected,
                        bool hilited) {
    data(idx, g, place, background, selected, hilited);
  });
  list2.set_data_source_and_target([&](int idx) {
    return data[idx];
  }, [&](int idx, const std::string& s) {
    data[idx] = s;
  });

  list2.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](win::event_source) {
    std::ostringstream strm;
    strm << "List2 item " << list2.get_selection() << ": ";
    if (list2.get_selection() > -1) {
      strm  << data[list2.get_selection()];
    }
    labelC.set_text(strm.str());
  }));
  list2.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event([&] () {
    labelC.set_text(ostreamfmt("List2 commited " << list2.get_selection()));
  }));

  column_list.list.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](win::event_source) {
    labelC.set_text(ostreamfmt("column_list item " << column_list.list.get_selection()));
  }));
  column_list.list.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event([&] () {
    labelC.set_text(ostreamfmt("column_list commited " << column_list.list.get_selection()));
  }));

  ok_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Ok Button clicked";
    label.set_text("OK Clicked!");
    data.insert(data.end(), { "Sechs", "Sieben", "Acht", "Neun", "Zehn" });
    data.update_list(list2);
  }));

  del_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Del Button clicked";
    label.set_text("Del Clicked!");
    if (!data.empty()) {
      data.erase(data.begin());
      data.update_list(list2);
    }
  }));

  clear_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    LogDebug << "Clear Button clicked";
    label.set_text("Clear Clicked!");
    data.clear();
    data.update_list(list2);
  }));

  scroll_check_box.register_event_handler(REGISTER_FUNCTION, win::button_state_event([&] (bool on) {
    list1.enable_scroll_bar(on);
    list2.enable_scroll_bar(on);
  }));

  vscroll.register_event_handler(REGISTER_FUNCTION, win::scroll_event([&](core::point::type pos) {
    list1.set_scroll_pos(pos);
    list2.set_scroll_pos(pos);
    list3.set_scroll_pos(pos);
    editor.vscroll.set_value(pos);
    textbox.vscroll.set_value(pos);
  }));

  calc_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    calc_pressed = true;
    scroll_view.layout();
  }));

  inc_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    scroll_view.resize(scroll_view.size() + core::size{5, 5});
  }));
  dec_button.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    scroll_view.resize(scroll_view.size() - core::size{5, 5});
  }));

  vslider.register_event_handler(REGISTER_FUNCTION, win::move_event([&](const core::point&) {
    layout();
  }));
  hslider.register_event_handler(REGISTER_FUNCTION, win::move_event([&](const core::point&) {
    layout();
  }));

  hscroll.register_event_handler(REGISTER_FUNCTION, win::scroll_event([&](core::point::type pos) {
    main_split_view.set_split_pos((double)pos / 100.0);
    editor.hscroll.set_value(pos);
    textbox.hscroll.set_value(pos);
  }));
  main_split_view.slider.register_event_handler(REGISTER_FUNCTION, win::move_event([&](const core::point&){
    hscroll.set_value(static_cast<win::scroll_bar::type>(main_split_view.get_split_pos() * hscroll.get_max()));
  }));

  cur_plus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    edit1.set_cursor_pos(edit1.get_cursor_pos() + 1);
  }));
  cur_minus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    if (edit1.get_cursor_pos() > 0) {
      edit1.set_cursor_pos(edit1.get_cursor_pos() - 1);
    }
  }));
  sel_first_plus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    (r.first)++;
    edit1.set_selection(r, win::event_source::mouse);
  }));
  sel_first_minus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    if (r.first > 0) {
      (r.first)--;
      edit1.set_selection(r, win::event_source::mouse);
    }
  }));
  sel_last_plus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    (r.last)++;
    edit1.set_selection(r, win::event_source::mouse);
  }));
  sel_last_minus.register_event_handler(REGISTER_FUNCTION, win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    if (r.last > 0) {
      (r.last)--;
      edit1.set_selection(r, win::event_source::mouse);
    }
  }));

  custom_button.register_event_handler(REGISTER_FUNCTION, win::mouse_enter_event([&]() {
    custom_button.set_hilited(true);
  }));
  custom_button.register_event_handler(REGISTER_FUNCTION, win::mouse_leave_event([&]() {
    custom_button.set_hilited(false);
  }));

  /*
    window2.register_event_handler(REGISTER_FUNCTION, win::mouse_enter_event([]() {
    LogDebug << "Window2 mouse enter";
  }));
  window2.register_event_handler(REGISTER_FUNCTION, win::mouse_leave_event([]() {
    LogDebug << "Window2 mouse leave";
  }));
  window1.register_event_handler(REGISTER_FUNCTION, win::move_event([](const core::point& p) {
    LogDebug << "Window1 move: " << p;
  }));
  window1.register_event_handler(REGISTER_FUNCTION, win::size_event([](const core::size& s) {
    LogDebug << "Window1 size: " << s;
  }));
  */

//  table_view.columns.set_selection_filter([] (const win::table::position& cell,
//                                              const win::table::metric& geo) {
//    return (geo.selection.column == cell.column);
//  });

  table_view.set_data_source_and_target([&](const win::table::position& cell) -> std::string {
    return table_data.get_cell(cell);
  }, [&](const win::table::position& cell, const std::string& s) {
    table_data.set_cell(cell, s);
  });
  table_view.set_scroll_maximum_calcer([&](const core::size& size,
                                           const core::point& current_pos,
                                           const core::point&) {
    return core::point(table_view.geometrie.widths.position_of(26) +
                       table_view.geometrie.widths.get_offset() - size.width(),
                       current_pos.y() + size.height() * 2);
  });

  htileview.set_item_size({ 50, 30 });
  htileview.set_background(color::very_light_gray);
  htileview.set_border({ 10, 20 });
  htileview.set_spacing({ 2, 4 });
  
  vtileview.set_item_size({ 40, 60 });
  vtileview.set_background(color::very_light_gray);
  vtileview.set_border({ 20, 10 });
  vtileview.set_spacing({ 5, 5 });

  htileview.set_drawer(tile_drawer<draw::frame::sunken_relief>);
  vtileview.set_drawer(tile_drawer<draw::frame::raised_relief>);

  register_event_handler(REGISTER_FUNCTION, win::create_event(this, &my_main_window::onCreated));
}

void my_main_window::query_state () {
  if (is_minimized()) {
    labelC.set_text("Minimized");
  } else if (is_maximized()) {
    labelC.set_text("Maximized");
  } else {
    labelC.set_text("Normal");
  }
}

void my_main_window::onCreated (win::window* w, const core::rectangle& r) {
  LogDebug << "Main created: this:" << std::hex << this << ", w:" << w << ", rect:" << std::dec << r;
  created_children();
}

template<int T>
void create_buttons (win::container& m, win::label labels[T]) {
  for (int i = 0; i < T; ++i) {
    labels[i].create(m, win::const_text(ostreamfmt("No. " << (i + 1))));
  }
}

template<int T, typename C>
void create_group (win::container& m, C& c, win::label labels[T]) {
  c.create(m);
  create_buttons<T>(c, labels);
}

void my_main_window::created_children () {

  my_main_window& main = *this;

  win::detail::get_window(get_id());

  scroll_view.create(main, core::rectangle(0, 0, 300, 330));

  window1.create(scroll_view, core::rectangle(10, 10, 100, 280));
  window1.set_accept_focus(false);

  window2.create(scroll_view, core::rectangle(120, 10, 200, 280));
  window2.set_accept_focus(false);

  hseparator.create(main, core::rectangle(330, 10, 300, 2));
  vseparator.create(main, core::rectangle(310, 20, 2, 300));

  calc_button.create(main, [&]() {
    return calc_pressed ? "Recalc" : "Calc";
  }, core::rectangle(330, 20, 60, 25));

  inc_button.create(main, "+", core::rectangle(400, 20, 25, 25));
  dec_button.create(main, "-", core::rectangle(435, 20, 25, 25));

  list1.create(main, core::rectangle(330, 50, 70, 250));
  list1.set_count(20);

  float floats[] = { 1.1F, 2.2F, 3.3F, 4.4F, 5.5F };

  auto columns = {
    layout::simple_column_info{ 30, text_origin::vcenter_right, 20 },
    layout::simple_column_info{ 30, text_origin::center, 20 },
    layout::simple_column_info{ 30, text_origin::vcenter_left, 20 }
  };

  win::simple_column_list_data<int, draw::frame::lines> col_data = {
    { 1, 2, 3 },
    { 3, 4, 5 },
    { 5, 6, 7 },
    { 7, 8, 9 },
    { 9, 10, 11 }
  };

  main_split_view.create(main, core::rectangle(410, 50, 160, 250));
  main_split_view.first.second.set_data(win::simple_list_data<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
  main_split_view.second.first.set_data(win::simple_list_data<float>(floats));
  main_split_view.second.second.get_column_layout().set_columns(columns);
  main_split_view.second.second.set_data(col_data);

  data.update_list(list2);

  //column_list_data = my_column_list_t::standard_data{ std::make_tuple(1, "eins", 1.1F),
  //                                                    std::make_tuple(2, "zwei", 2.2F),
  //                                                    std::make_tuple(3, "drei", 3.3F) };

  auto weight_columns = {
    layout::weight_column_info{ 30, text_origin::vcenter_left, 20, 0.0F },
    layout::weight_column_info{ 30, text_origin::vcenter_right, 20, 1.0F },
    layout::weight_column_info{ 30, text_origin::center, 20, 1.0F },
    layout::weight_column_info{ 30, text_origin::center, 20, 1.0F },
    layout::weight_column_info{ 30, text_origin::center, 20, 1.0F }
  };

  column_list_drawer = {
    [] (const int& v, const draw::graphics& g, const core::rectangle& r,
        const draw::brush&b, bool s, bool h, text_origin align) {
      win::paint::text_item(g, r, color::buttonColor(), ostreamfmt(v), false, text_origin::center);
      draw::frame::raised_relief(g, r);
    },

    win::cell_drawer<std::string, draw::frame::sunken_relief>,
    win::cell_drawer<float, draw::frame::sunken_relief>,
    win::cell_drawer<int, draw::frame::sunken_relief>,

    [] (const bool& v, const draw::graphics& g, const core::rectangle& r,
        const draw::brush& b, bool s, bool h, text_origin align) {
      std::string text = v ? u8"♣" : u8"♥";
      win::paint::text_item(g, r, b, text, s, align);
      draw::frame::sunken_relief(g, r);
    }
  };

//  column_list.set_data(column_list_data, column_list_data.size());
  column_list.create(main, core::rectangle(580, 50, 140, 250));
  column_list.header.set_cell_drawer([] (std::size_t i, const draw::graphics& g,
                                         const core::rectangle& r, const draw::brush& background) {
    using namespace draw;
    g.fill(rectangle(r), background);
    frame::raised_deep_relief(g, r);
    g.text(text_box(ostreamfmt((char)('C' + i) << (i + 1)), r, text_origin::center), font::system(), color::windowTextColor());
  });
  column_list.set_drawer(column_list_drawer);
  column_list.get_column_layout().set_columns(weight_columns);
  column_list.set_data([](std::size_t i){
    return std::make_tuple(static_cast<int>(i), ostreamfmt(i << '-' << i), (1.1F * (float)i), static_cast<int>(i * i), i % 2 == 1);
  }, 20);
//  column_list.get_column_layout().get_slider(0)->disable();

  table_view.create(main, core::rectangle(740, 50, 150, 250));

  table_view.data.set_drawer(win::table::default_data_drawer([&](const win::table::position& cell) -> std::string {
    return table_data.get_cell(cell);//ostreamfmt(win::table::build_std_column_name(column) << ':' << (row + 1));
  }));
  table_view.data.foregrounds.set_column(1, color::blue);
  table_view.data.foregrounds.set_column(2, color::red);
  table_view.data.foregrounds.set_row(3, color::dark_green);
  table_view.data.foregrounds.set_cell(win::table::position(2, 3), color::black);
  table_view.data.backgrounds.set_cell(win::table::position(2, 3), color::yellow);
  table_view.data.foregrounds.set_cell(win::table::position(3, 4), color::blue);

  table_view.columns.set_drawer(win::table::default_header_drawer([](const win::table::position& cell) -> std::string {
    return win::table::build_std_column_name(cell.column);
  }));
  table_view.geometrie.widths.set_size(2, 40);

  table_view.rows.set_drawer(win::table::default_header_drawer([](const win::table::position& cell) -> std::string {
    return ostreamfmt((1 + cell.row));
  }));

  table_view.edge.set_text("0:0");
  table_view.enable_size(true, true);

  editor.create(main, core::rectangle(740, 320, 150, 250));
  editor.view.set_text("1. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,\n"
                       "2. sed diam nonumy eirmod tempor invidunt ut labore et dolore\n"
                       "3. magna aliquyam erat, sed diam voluptua.\n"
                       "4. At vero eos et accusam et justo duo dolores et ea rebum.\n"
                       "5. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
                       "6. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,\n"
                       "7. sed diam nonumy eirmod tempor invidunt ut labore et dolore\n"
                       "8. magna aliquyam erat, sed diam voluptua.\n"
                       "9. At vero eos et accusam et justo duo dolores et ea rebum.\n"
                       "10. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
                       "11. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,\n"
                       "12. sed diam nonumy eirmod tempor invidunt ut labore et dolore\n"
                       "13. magna aliquyam erat, sed diam voluptua.\n"
                       "14. At vero eos et accusam et justo duo dolores et ea rebum.\n"
                       "15. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n"
                       "16. Lorem ipsum dolor sit amet, consetetur sadipscing elitr,\n"
                       "17. sed diam nonumy eirmod tempor invidunt ut labore et dolore\n"
                       "18. magna aliquyam erat, sed diam voluptua.\n"
                       "19. At vero eos et accusam et justo duo dolores et ea rebum.\n"
                       "20. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n");
  editor.view.set_cursor_pos({3, 5});
  editor.view.set_selection({{3, 2}, {2, 4}});

  textbox.create(main, core::rectangle(740, 580, 150, 250));
  textbox.view.enable_select_by_mouse();

  htileview.create(main, core::rectangle(10, 580, 200, 250));
  htileview.set_count(20);

  vtileview.create(main, core::rectangle(220, 580, 400, 250));
  vtileview.set_count(30);

  hscroll.create(main, core::rectangle(550, 305, 130, static_cast<core::size_type>(win::scroll_bar::get_scroll_bar_width())));

  vscroll.create(main, core::rectangle(700, 50, static_cast<core::size_type>(win::scroll_bar::get_scroll_bar_width()), 250));
  vscroll.set_max((int)list1.get_count() * list1.get_item_size() - list1.size().height());
  vscroll.set_step(static_cast<win::scroll_bar::type>(list1.get_item_size()));

  up_button.create(main, "Up", core::rectangle(330, 305, 47, 25));

  down_button.create(main, "Down", core::rectangle(383, 305, 47, 25));

  scroll_check_box.create(main, "Enable", core::rectangle(440, 305, 100, 20));
  scroll_check_box.set_checked(true);

  label.create(main, "Text", core::rectangle(50, 350, 120, 20));
  labelC.create(main, core::rectangle(50, 371, 120, 20));
  labelR.create(main, core::rectangle(50, 392, 120, 20));

  hslider.create(main, core::rectangle(5, 470, 500, 5));
  hslider.set_min_max(420, 600);

  vslider.create(main, core::rectangle(750, 5, 5, 500));
  vslider.set_min_max(570, 1800);

  chck_group.create(main, core::rectangle(180, 350, 100, 80));

  radio_button.create(chck_group, "Radio");
  radio_button2.create(chck_group, "Radio2", core::rectangle(0, 20, 100, 20));
  check_box.create(chck_group, "Check", core::rectangle(0, 40, 100, 20));

  edit1.create(main, "Text zwei drei vier fuenf sechs sieben acht", core::rectangle(290, 350, 100, 25));

  edit_btn_group.create(main, core::rectangle(290, 380, 100, 16));

  cur_minus.create(edit_btn_group, "c-");
  cur_plus.create(edit_btn_group, "c+");
  sel_first_minus.create(edit_btn_group, "f-");
  sel_first_plus.create(edit_btn_group, "f+");
  sel_last_minus.create(edit_btn_group, "l-");
  sel_last_plus.create(edit_btn_group, "l+");

  custom_button.set_drawer([] (const draw::graphics& g,
                               const core::rectangle& r,
                               const win::button_state& s,
                               bool focused,
                               bool enabled) {
    win::paint::flat_button(g, r, "Custom", s, focused, enabled);
  });

  custom_button.create(main, core::rectangle(290, 410, 100, 25));

  drop_down.set_data([](std::size_t i) {
    return ostreamfmt("Item " << i);
  }, 10);
  drop_down.create(main, core::rectangle(180, 445, 100, 20));
  drop_down.set_visible_items(8);

  color_drop_down.set_data([&](std::size_t i) {
    static const os::color colors[] = {
      color::red,
      color::blue,
      color::green,
      color::cyan,
      color::yellow,
      color::magenta
    };

    return colors[i % 6];
  }, 20);

  color_drop_down.create(main, core::rectangle(290, 445, 100, 20));

  btn_group.create(main);
  ok_button.create(btn_group, "Ok");
  del_button.create(btn_group, "Del");
  clear_button.create(btn_group, "Clear");

  btn_sep1.create(btn_group);
  btn_group.get_layout().add_separator(&btn_sep1);

  min_button.create(btn_group, "Min");
  max_button.create(btn_group, "Max");
  norm_button.create(btn_group, "Norm");

  btn_sep2.create(btn_group);
  btn_group.get_layout().add_separator(&btn_sep2);

  info_button.create(btn_group, "Info");
  null_button.create(btn_group, "0");
  full_button.create(btn_group, "Full");

  group_group.create(main, core::rectangle(400, 345, 10, 10));
  create_group<4>(group_group, h_lineup_group, h_lineup_labels);
  create_group<4>(group_group, v_lineup_group, v_lineup_labels);
  create_group<4>(group_group, grid_lineup_group, grid_lineup_labels);
  create_group<4>(group_group, grid_adaption_group, grid_adaption_labels);
  create_group<4>(group_group, hc_lineup_group, hc_lineup_labels);
  create_group<4>(group_group, vc_lineup_group, vc_lineup_labels);

  using namespace layout;
  get_layout().attach_relative<What::left, make_relative(0.1), 20>(&btn_group, this);
  get_layout().attach_relative<What::right, make_relative(0.9), -20>(&btn_group, this);
  get_layout().attach_fix<What::top, Where::height, -45>(&btn_group, this);
  get_layout().attach_fix<What::bottom, Where::height>(&btn_group, this);

  get_layout().attach_relative<What::left, make_relative(0.1), 10>(&hslider, this);
  get_layout().attach_relative<What::right, make_relative(0.9)>(&hslider, this);

  get_layout().attach_fix<What::bottom, Where::y, -5>(&vslider, &btn_group);

  get_layout().attach_fix<What::right, Where::x, -25>(&column_list, &vslider);
  get_layout().attach_fix<What::left, Where::x, -20>(&vscroll, &vslider);
  get_layout().attach_fix<What::right, Where::x, -3>(&vscroll, &vslider);

  get_layout().attach_fix<What::right, Where::x, -4>(&group_group, &vslider);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(&group_group, &hslider);

  get_layout().attach_fix<What::left, Where::x2, 2>(&table_view, &vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(&table_view, this);

  get_layout().attach_fix<What::left, Where::x2, 2>(&editor, &vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(&editor, this);
  get_layout().attach_fix<What::top, Where::y, 320>(&editor, this);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(&editor, &hslider);

  get_layout().attach_fix<What::left, Where::x2, 2>(&textbox, &vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(&textbox, this);
  get_layout().attach_fix<What::top, Where::y2, 4>(&textbox, &hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(&textbox, this);

  get_layout().attach_fix<What::bottom, Where::y2, -8>(&drop_down, &hslider);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(&color_drop_down, &hslider);

  get_layout().attach_fix<What::top, Where::y2, 4>(&htileview, &hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(&htileview, this);

  get_layout().attach_fix<What::top, Where::y2, 4>(&vtileview, &hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(&vtileview, this);
}

win::paint_event my_main_window::create_paint1 () {
  return win::paint_event(draw::buffered_paint([](const draw::graphics& graph) {
    //LogDebug << "win::paint 1";

    using namespace draw;

    graph.fill(rectangle(graph.area()), color::light_gray);

    graph.frame(polygon(calc_star(10, 10, 40, 40)), color::blue);
    graph.fill(polygon(calc_star(60, 10, 40, 40)), color::dark_green);
    graph.draw(polygon(calc_star(110, 10, 40, 40)), color::yellow, color::red);

    graph.frame(polygon({ { 5, 5 }, { 155, 5 }, {75, 8} }), color::red);

    core::point pt(10, 60);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 1!", pt), font::system(), color::black);

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 2!", pt), font::system_bold(), color::black);

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 3!", pt), font::sans_serif().with_italic(true), color::black);

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 4!", pt), font::serif(), color::black);

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 5!", pt), font::monospace(), color::black);

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 6!", pt), font::sans_serif().with_size(18), color::blue);

    auto draw_text_box = [](const draw::graphics& g,
                            const std::string& t,
                            core::rectangle r,
                            text_origin o) {
      g.frame(rectangle(r), color::blue);
      core::rectangle rb = r;
      g.text(bounding_box(t, rb, o), font::system(), color::red);
      g.frame(rectangle(rb), color::dark_green);
      g.text(text_box(t, r, o), font::system(), color::red);
    };

    auto draw_text = [](const draw::graphics& g,
      const std::string& t,
      const core::point& p,
      text_origin o) {
      g.fill(arc(p, 2, 0, 360), color::blue);
      g.text(text(t, p, o), font::system(), color::red);
    };

    std::string texte[] = {
      "TLg", "TCg", "TRg",
      "CLg", "CCg", "CRg",
      "BRg", "BCg", "BRg"
    };
    text_origin origins[] = {
      text_origin::top_left, text_origin::top_hcenter, text_origin::top_right,
      text_origin::vcenter_left, text_origin::center, text_origin::vcenter_right,
      text_origin::bottom_left, text_origin::bottom_hcenter, text_origin::bottom_right
    };

    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 3; ++x) {
        int i = y * 3 + x;
        draw_text_box(graph, texte[i], { core::point_type(10 + x * 55), core::point_type(155 + y * 25), 50, 22 }, origins[i]);
        draw_text(graph, texte[i], { core::point_type(10 + x * 80), core::point_type(235 + y * 20) }, origins[i]);
      }
    }

  }));
}

win::paint_event my_main_window::create_paint2 () {
  return win::paint_event([](const draw::graphics& graph) {
    //LogDebug << "win::paint 2";
    using namespace draw;

    graph.draw_pixel(core::point(3, 3), color::gray);
    graph.draw_pixel(core::point(6, 6), color::gray);

    core::size sz(30, 50);
    core::size offs1(0, 60);
    core::size offs2(0, 120);

    core::point pos1(10, 10);
    graph.frame(rectangle(pos1, sz), color::blue);
    graph.fill(rectangle(pos1 + offs1, sz), color::dark_green);
    graph.draw(rectangle(pos1 + offs2, sz), color::red, color::cyan);

    core::point pos2(50, 10);
    graph.frame(ellipse(pos2, sz), color::blue);
    graph.fill(ellipse(pos2 + offs1, sz), color::dark_green);
    graph.draw(ellipse(pos2 + offs2, sz), color::red, color::cyan);

    core::point pos3(90, 10);
    core::size rd(10, 10);
    graph.frame(round_rectangle(core::rectangle(pos3, sz), rd), color::blue);
    graph.fill(round_rectangle(core::rectangle(pos3 + offs1, sz), rd), color::dark_green);
    graph.draw(round_rectangle(core::rectangle(pos3 + offs2, sz), rd), color::red, color::cyan);

    core::point pos4(150, 30);
    graph.frame(arc(pos4, 20, 0, 350), color::blue);
    graph.fill(arc(pos4 + offs1, 20, 0, 350), color::dark_green);
    graph.draw(arc(pos4 + offs2, 20, 0, 350), color::red, color::cyan);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

  });
}
