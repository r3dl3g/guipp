
// --------------------------------------------------------------------------
//
// Common includes
//
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <logging/logger.h>

#include <gui/win/dbg_win_message.h>
#include <gui/layout/attach_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>

#include <gui/ctrl/progress_bar.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/list.h>
#include <gui/ctrl/virtual_view.h>
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


using namespace gui;

class log_all_events {
public:
  log_all_events() {
  }

  bool operator()(const core::event& e, gui::os::event_result& result) {
    if (!win::is_none_client_event(e) &&
        !win::is_frequent_event(e) ) {
      LogDebug << "Message: " << e << IF_WIN32_ELSE(" (" << std::hex << e.wParam << ", " << e.lParam << ")", "");
    }
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
void htile_drawer (std::size_t idx,
                  const draw::graphics& g,
                  const core::rectangle& place,
                  const draw::brush& background,
                  ctrl::item_state state) {
  using namespace draw;

  ctrl::paint::text_cell<std::size_t, F>(idx, g, place, text_origin::center, color::black, background.color(), state);
}

template<draw::frame::drawer F = draw::frame::sunken_relief>
void vtile_drawer (std::size_t idx,
                  const draw::graphics& g,
                  const core::rectangle& place,
                  const draw::brush& background,
                  ctrl::item_state state) {
  using namespace draw;

  std::string s = util::string::utf16_to_utf8(std::wstring(1, std::wstring::value_type(idx + 32)));
  ctrl::paint::text_cell<std::string, F>(ostreamfmt(' ' << std::hex << std::setw(4) << std::setfill('0') << (idx + 32) << ": '" << s << '\''),
                                        g, place, text_origin::vcenter_left, color::black, background.color(),
                                        state);
}

namespace gui {

  namespace win {

    inline std::string color_to_string (const os::color& c) {
      return ostreamfmt("#" << std::setfill('0') << std::setw(6) << std::hex << c);
    }

  } // namespace win

  namespace ctrl {
    template<>
    void default_drop_down_drawer<os::color> (const os::color& c,
                                              const draw::graphics& graph,
                                              const core::rectangle& place,
                                              const draw::brush&,
                                              item_state state) {
      graph.fill(draw::rectangle(place), c);
      graph.text(draw::text_box(win::color_to_string(c), place, text_origin::center), draw::font::system(), color::invert(c));

      switch (state) {
        case item_state::selected:
          graph.frame(draw::rectangle(place), color::black);
          break;
        case item_state::hilited:
          graph.frame(draw::rectangle(place), color::highLightColor());
          break;
      }
    }

  } // namespace win

} // namespace gui

// --------------------------------------------------------------------------
class my_main_window : public win::layout_main_window<layout::win_attach> {
public:
  typedef win::layout_main_window<layout::win_attach> super;
  typedef win::group_window<layout::horizontal_adaption<5, 5>, color::dark_gray> group_group_t;
  typedef win::cls::main_window_class<my_main_window, color::very_very_light_gray> myclazz;

  my_main_window ();

  void onCreated (win::window*, const core::rectangle&);
  void created_children ();
  void query_state ();
  void set_size_null ();

  ctrl::paint_function create_paint1 ();
  ctrl::paint_function create_paint2 ();

  void create (const core::rectangle& r = core::rectangle::def) {
    super::create(myclazz::get(), r);
  }

private:
  ctrl::scroll_view scroll_view;

  ctrl::client_control<> window1;
  ctrl::client_control<> window2;

  ctrl::horizontal_separator hseparator;
  ctrl::vertical_separator vseparator;

  ctrl::text_button calc_button;
  ctrl::text_button inc_button;
  ctrl::text_button dec_button;
  ctrl::text_button invert_button;

  ctrl::text_button ok_button;
  ctrl::text_button del_button;
  ctrl::text_button clear_button;

  ctrl::vertical_separator btn_sep1;
  ctrl::vertical_separator btn_sep2;

  win::group_window<layout::horizontal_adaption<5, 10, 2, 50, 90, origin::end>, color::very_light_gray> btn_group;
  win::group_window<layout::vertical_adaption<5, 5>> chck_group;

  group_group_t group_group;

  win::group_window<layout::horizontal_lineup<30, 5, 5>> h_lineup_group;
  ctrl::label h_lineup_labels[4];

  win::group_window<layout::vertical_lineup<30, 5, 5>> v_lineup_group;
  ctrl::label v_lineup_labels[4];

  win::group_window<layout::grid_lineup<30, 30, 5, 5>> grid_lineup_group;
  ctrl::label grid_lineup_labels[4];

  win::group_window<layout::grid_adaption<2, 2, 5, 5>> grid_adaption_group;
  ctrl::label grid_adaption_labels[4];

  win::group_window<layout::horizontal_lineup<30, 5, 5, 2, origin::center>> hc_lineup_group;
  ctrl::label hc_lineup_labels[4];

  win::group_window<layout::vertical_lineup<30, 5, 5, 2, origin::center>> vc_lineup_group;
  ctrl::label vc_lineup_labels[4];

  ctrl::radio_button<> radio_button, radio_button2;
  ctrl::check_box<> check_box;
  ctrl::switch_button<> switch_button;
  ctrl::animated_switch_button<> switch_button2;

  ctrl::horizontal_scroll_bar start_angle;
  ctrl::horizontal_scroll_bar end_angle;

  ctrl::label label;
  ctrl::basic_label<text_origin::center, draw::frame::no_frame, color::blue, color::light_gray> labelC;
  ctrl::label_right labelR;

  ctrl::text_button min_button;
  ctrl::text_button max_button;
  ctrl::text_button norm_button;
  ctrl::text_button info_button;
  ctrl::text_button null_button;
  ctrl::text_button full_button;

  ctrl::simple_list_data<std::string> data;

  typedef ctrl::vertical_list List1;
  typedef ctrl::edit_list List2;
  typedef ctrl::vertical_list List3;

  List1 list1;
  List2& list2;
  List3& list3;

  typedef ctrl::vertical_split_view<List2, List3> list_split_view;
  typedef ctrl::simple_column_list<layout::simple_column_list_layout> simple_list;
  typedef ctrl::vertical_split_view<ctrl::horizontal_list, simple_list> column_list_split_view;

  ctrl::horizontal_split_view<list_split_view, column_list_split_view> main_split_view;

  ctrl::text_button up_button;
  ctrl::text_button down_button;

  ctrl::vertical_scroll_bar vscroll;
  ctrl::horizontal_scroll_bar hscroll;
  ctrl::progress_bar progress;

  ctrl::check_box<> scroll_check_box;

  ctrl::horizontal_slider hslider;
  ctrl::vertical_slider vslider;

  const ctrl::paint_function paint1;
  const ctrl::paint_function paint2;

  ctrl::edit edit1;

  win::group_window<layout::horizontal_adaption<0, 2>> edit_btn_group;
  ctrl::text_button cur_plus;
  ctrl::text_button cur_minus;
  ctrl::text_button sel_first_plus;
  ctrl::text_button sel_first_minus;
  ctrl::text_button sel_last_plus;
  ctrl::text_button sel_last_minus;

  ctrl::custom_push_button custom_button;
  ctrl::drop_down_list<std::string> drop_down;
  ctrl::drop_down_list<os::color> color_drop_down;

  typedef ctrl::column_list_t<layout::weight_column_list_layout, int, std::string, float, int, bool> my_column_list_t;
  my_column_list_t column_list;
  my_column_list_t::row_drawer column_list_drawer;

  ctrl::table_edit table_view;
  ctrl::table::data::matrix<std::string> table_data;

  typedef ctrl::virtual_view<ctrl::editbox> editbox_view;
  editbox_view editor;

  typedef ctrl::basic_textbox<text_origin::vcenter_left, draw::frame::sunken_relief, color::dark_blue, color::very_very_light_gray> textbox_type;
  typedef ctrl::virtual_view<textbox_type> textbox_view;

  textbox_view textbox;

  ctrl::horizontal_tile_view htileview;
  ctrl::vertical_tile_view vtileview;

  bool at_paint1;
  bool at_drag;
  core::point last_pos;

  bool calc_pressed;
  bool draw_invert;
};

int gui_main(const std::vector<std::string>& /*args*/) {

  my_main_window main;

  LogDebug << "window size:" << sizeof(main)  << ", window_class_info size:" << sizeof(win::class_info);
  LogDebug << "long size:" << sizeof(long)<< ", pointer size:" << sizeof(void*);
  size_t str_size = sizeof(std::string);
  size_t evc_size = sizeof(core::event_container);
  size_t win_size = sizeof(win::window);
  size_t cln_size = sizeof(ctrl::client_control<>);
  size_t btn_size = sizeof(ctrl::button_base);
  size_t push_size = sizeof(ctrl::button_base);
  size_t tgl_size = sizeof(ctrl::basic_button<ctrl::push_button_traits>);
  size_t tbtn_size = sizeof(ctrl::text_button);
  size_t pnt_size = sizeof(ctrl::paint_function);

  LogInfo << "Sizes: "
          <<   "std::string:" << str_size
          << ", event_container:" << evc_size
          << ", window:" << win_size
          << ", client_control:" << cln_size
          << ", button:" << btn_size
          << ", push_button:" << push_size
          << ", toggle_button:" << tgl_size
          << ", text_button:" << tbtn_size
          << ", paint_event:" << pnt_size;

//#ifdef WIN32
//  main.on_get_minmax([](const core::size& sz,
//    const core::point& pos,
//    core::size& mi, core::size& ma) {
//    mi = { 300, 200 };
//    ma = { 880, 660 };
//    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
//  }));
//#endif

  const core::rectangle r = core::rectangle(50, 50, 1500, 800);
  LogDebug << "Create Main: " << r;
  main.create(r);
  main.set_title("Window Test");

  main.set_visible();

  return win::run_main_loop();
}

my_main_window::my_main_window ()
  : at_paint1(true)
  , at_drag(false)
  , calc_pressed(false)
  , draw_invert(false)
  , list2(main_split_view.first.first)
  , list3(main_split_view.first.second)
  , paint1(create_paint1())
  , paint2(create_paint2())
  , column_list(my_column_list_t(20, color::very_very_light_gray))
  , table_view(50, 20)
  , table_data(std::string())
{
  main_split_view.second.second.list.set_item_size_and_background(16, color::very_light_gray);
  list1.set_item_size(25);
  list3.set_item_size_and_background(20, color::light_gray);

  on_destroy([] (const window* w) {
    LogDebug << "Destroyed!";
    win::quit_main_loop(w);
  });

  on_show([&] () {
    layout();
    btn_group.layout();
    group_group.layout();
  });

//#ifdef WIN32
//  on_close([&]() {
//    LogDebug << "Close!";
//    destroy();
//  });
//  on_enable([](bool on) {
//    LogDebug << (on ? "Enableed" : "Disabled");
//  });
//  on_activate([](bool on, win::window* win) {
//    LogDebug << "Main " << (on ? "activate" : "deactivate");
//  });
//  on_begin_size_or_move([]() { LogDebug << "Start Move/Size"; });
//  on_end_size_or_move([]() { LogDebug << "Finish Move/Size"; });
//  on_activate_app([](bool on) {
//    LogDebug << (on ? "A" : "Dea") << "ctivate App";
//  });

//#endif

  on_moving([] (const core::point& r) {
    LogDebug << "Main moving: " << r;
  });
  on_sizing([] (const core::size& r) {
    LogDebug << "Main sizing: " << r;
  });
  on_placing([] (const core::rectangle& r) {
    LogDebug << "Main placing: " << r;
  });

  on_move([] (const core::point& p) {
    LogDebug << "Main move: " << p;
  });
  on_size([] (const core::size& s) {
    LogDebug << "Main size: " << s;
  });
  on_place([] (const core::rectangle& r) {
    LogDebug << "Main place: " << r;
  });

//#ifdef WIN32
//  ok_button.on_activate([](bool on, win::window* win) {
//    LogDebug << "Button " << (on ? "" : "de") << "activate";
//  });
//#endif
//  ok_button.on_set_focus([] (win::window*) {
//    LogDebug << "Button Set Focus";
//  });
//  ok_button.on_lost_focus([&] (win::window*) {
//    LogDebug << "Button Lost Focus";
//  });

  on_left_btn_down([&] (os::key_state, const core::point& p) {
    LogDebug << "Left Button Down at " << p;
  });
  on_left_btn_up([&] (os::key_state, const core::point& p) {
    LogDebug << "Left Button Up at " << p;
  });
  on_right_btn_down([&] (os::key_state, const core::point& p) {
    LogDebug << "Right Button Down at " << p;
  });
  on_right_btn_up([&] (os::key_state, const core::point& p) {
    LogDebug << "Right Button Up at " << p;
  });
  window1.on_wheel<orientation::horizontal>([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(delta, 0));
    }
  });
  window1.on_wheel<orientation::vertical>([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  });

  window1.on_left_btn_down([&](os::key_state, const core::point& p) {
    at_drag = true;
    last_pos = p;
    window1.capture_pointer();
    LogDebug << "Window1 Mouse down at " << p;
  });
  window1.on_left_btn_up([&](os::key_state, const core::point& p) {
    window1.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window1 Mouse up at " << p;
  });

  window2.on_left_btn_down([&](os::key_state, const core::point& p) {
    at_drag = true;
    last_pos = p;
    window2.capture_pointer();
    LogDebug << "Window2 Mouse down at " << p;
  });
  window2.on_left_btn_up([&](os::key_state, const core::point& p) {
    window2.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window2 Mouse up at " << p;
  });

  window1.on_mouse_move([&] (os::key_state, const core::point& p) {
    //LogDebug << "Window1 Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      auto delta = p - last_pos;
      //last_pos = p;
      window1.move(window1.position() + delta);
    }
  });
  window2.on_mouse_move([&] (os::key_state, const core::point& p) {
    //LogDebug << "Window2 Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      auto delta = p - last_pos;
      //last_pos = p;
      window2.move(window2.position() + delta);
    }
  });

  window1.on_left_btn_dblclk([&] (os::key_state, const core::point& p) {
    LogDebug << "Window1 Double Click up at " << p;
    window2.set_visible(!window2.is_visible());
  });

  on_left_btn_dblclk([&] (os::key_state, const core::point& p) {
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
  });
  on_right_btn_dblclk([&] (os::key_state, const core::point&) {
    window1.move({50, 50});
  });

  window2.on_paint(draw::paint(paint1));

  at_paint1 = true;

  window2.on_left_btn_dblclk([&] (os::key_state, const core::point& p) {
    LogDebug << "Window2 Double Click up at " << p;
    if (at_paint1) {
      at_paint1 = false;
      window2.unregister_event_handler<win::paint_event>(draw::paint(paint1));
      window2.on_paint(draw::paint(paint2));
    } else {
      at_paint1 = true;
      window2.unregister_event_handler<win::paint_event>(draw::paint(paint2));
      window2.on_paint(draw::paint(paint1));
    }
    window2.invalidate();
  });
  window2.on_show([] () {
    LogDebug << "Window2 show:";
  });
  window2.on_hide([] () {
    LogDebug << "Window2 hide:";
  });

//#ifdef WIN32
//  ok_button.on_state_changed([](bool state) {
//    LogDebug << "Button " << (state ? "hilited" : "unhilited");
//  });
//#endif // WIN32

  ok_button.on_pushed([&] () {
    LogDebug << "Button pushed";
    label.set_text("Pushed!");
  });
  ok_button.on_released([&] () {
    LogDebug << "Button released";
    label.set_text("Released!");
  });
  radio_button.on_clicked([&] () {
    LogDebug << "Radio clicked";
    labelR.set_text("Radio clicked!");
    bool check = radio_button.is_checked();
//    radio_button.set_checked(!check);
    radio_button2.set_checked(!check);
  });
  radio_button2.on_clicked([&] () {
    LogDebug << "Radio2 clicked";
    labelR.set_text("Radio2 clicked!");
    bool check = radio_button2.is_checked();
//    radio_button2.set_checked(!check);
    radio_button.set_checked(!check);
  });
  check_box.on_state_changed([&] (bool on) {
    LogDebug << "Check clicked";
    label.set_text("Check clicked!");
    radio_button.enable(on);
    radio_button2.enable(on);
    ok_button.enable(on);
    vscroll.enable(on);
    hscroll.enable(on);
    table_view.set_enable_edit(on);
    up_button.enable(on);
    switch_button.enable(on);
  });

  min_button.on_clicked([&] () {
    LogDebug << "Min clicked";
    minimize();
    query_state();
  });
  max_button.on_clicked([&] () {
    LogDebug << "Max clicked";
    maximize();
    query_state();
  });
  norm_button.on_clicked([&] () {
    LogDebug << "Norm clicked";
    restore();
    query_state();
  });
  info_button.on_clicked([&] () {
    LogDebug << "Info clicked";
    query_state();
    textbox.view.set_text(editor.view.get_selected_text());
    textbox.layout();
  });
  null_button.on_clicked([&] () {
    LogDebug << "0 clicked";
    vslider.set_value(vslider.get_min());
  });
  full_button.on_clicked([&] () {
    LogDebug << "Full clicked";
    vslider.set_value(size().width() - 15);
  });

  auto list_drawer = [] (std::size_t idx,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& background,
                         ctrl::item_state state) {
    using namespace draw;

    std::ostringstream strm;
    strm << "Item " << idx;

    ctrl::paint::text_item(g, place, background, strm.str(), state);
  };

  list1.set_drawer(list_drawer);
  list1.on_selection_changed([&](ctrl::event_source) {
    labelC.set_text(ostreamfmt("List1 item " << list1.get_selection()));
  });
  list1.on_selection_commit([&] () {
    labelC.set_text(ostreamfmt("List1 commited " << list1.get_selection()));
  });

  up_button.on_clicked([&] () {
    list1.set_selection(list1.get_selection() - 1, ctrl::event_source::mouse);
    list2.set_selection(list2.get_selection() - 1, ctrl::event_source::mouse);
    list3.set_selection(list3.get_selection() - 1, ctrl::event_source::mouse);
  });
  down_button.on_clicked([&] () {
    list1.set_selection(list1.get_selection() + 1, ctrl::event_source::mouse);
    list2.set_selection(list2.get_selection() + 1, ctrl::event_source::mouse);
    list3.set_selection(list3.get_selection() + 1, ctrl::event_source::mouse);
  });

  data.insert(data.end(), { "Eins", "Zwei", "Drei", "View", "Fünf", "Fuß" });
  list2.set_drawer([&] (std::size_t idx,
                        const draw::graphics& g,
                        const core::rectangle& place,
                        const draw::brush& background,
                        ctrl::item_state state) {
    data(idx, g, place, background, state);
  });
  list2.set_data_source_and_target([&](int idx) {
    return data[idx];
  }, [&](int idx, const std::string& s) {
    data[idx] = s;
  });

  list2.on_selection_changed([&](ctrl::event_source) {
    std::ostringstream strm;
    strm << "List2 item " << list2.get_selection() << ": ";
    if (list2.get_selection() > -1) {
      strm  << data[list2.get_selection()];
    }
    labelC.set_text(strm.str());
  });
  list2.on_selection_commit([&] () {
    labelC.set_text(ostreamfmt("List2 commited " << list2.get_selection()));
  });

  column_list.list.on_selection_changed([&](ctrl::event_source) {
    labelC.set_text(ostreamfmt("column_list item " << column_list.list.get_selection()));
  });
  column_list.list.on_selection_commit([&] () {
    labelC.set_text(ostreamfmt("column_list commited " << column_list.list.get_selection()));
  });

  ok_button.on_clicked([&] () {
    LogDebug << "Ok Button clicked";
    label.set_text("OK Clicked!");
    data.insert(data.end(), { "Sechs", "Sieben", "Acht", "Neun", "Zehn" });
    data.update_list(list2);
  });

  del_button.on_clicked([&] () {
    LogDebug << "Del Button clicked";
    label.set_text("Del Clicked!");
    if (!data.empty()) {
      data.erase(data.begin());
      data.update_list(list2);
    }
  });

  clear_button.on_clicked([&] () {
    LogDebug << "Clear Button clicked";
    label.set_text("Clear Clicked!");
    data.clear();
    data.update_list(list2);
  });

  scroll_check_box.on_state_changed([&] (bool on) {
    list1.enable_scroll_bar(on);
    list2.enable_scroll_bar(on);
  });

  vscroll.on_scroll([&](core::point::type pos) {
    list1.set_scroll_pos(pos);
    list2.set_scroll_pos(pos);
    list3.set_scroll_pos(pos);
    editor.vscroll.set_value(pos);
    textbox.vscroll.set_value(pos);
  });

  calc_button.on_clicked([&] () {
    calc_pressed = true;
    scroll_view.layout();
  });

  inc_button.on_clicked([&] () {
    scroll_view.resize(scroll_view.size() + core::size{5, 5});
  });
  dec_button.on_clicked([&] () {
    scroll_view.resize(scroll_view.size() - core::size{5, 5});
  });
  invert_button.on_clicked([&] () {
    draw_invert = !draw_invert;
    window2.invalidate();
  });

  vslider.on_move([&](const core::point&) {
    layout();
  });
  hslider.on_move([&](const core::point&) {
    layout();
  });

  hscroll.on_scroll([&](core::point::type pos) {
    main_split_view.set_split_pos((double)pos / 100.0);
    editor.hscroll.set_value(pos);
    textbox.hscroll.set_value(pos);
    progress.set_value(pos);
  });
//  main_split_view.slider.on_move([&](const core::point&){
//    hscroll.set_value(static_cast<ctrl::scroll_bar::type>(main_split_view.get_split_pos() * hscroll.get_max()), false);
//  });

  cur_plus.on_clicked([&] () {
    edit1.set_cursor_pos(edit1.get_cursor_pos() + 1);
  });
  cur_minus.on_clicked([&] () {
    if (edit1.get_cursor_pos() > 0) {
      edit1.set_cursor_pos(edit1.get_cursor_pos() - 1);
    }
  });
  sel_first_plus.on_clicked([&] () {
    auto r = edit1.get_selection();
    edit1.set_selection({r.begin() + 1, r.end()}, ctrl::event_source::mouse);
  });
  sel_first_minus.on_clicked([&] () {
    auto r = edit1.get_selection();
    if (r.begin() > 0) {
      edit1.set_selection({r.begin() - 1, r.end()}, ctrl::event_source::mouse);
    }
  });
  sel_last_plus.on_clicked([&] () {
    auto r = edit1.get_selection();
    edit1.set_selection({r.begin(), r.end() + 1}, ctrl::event_source::mouse);
  });
  sel_last_minus.on_clicked([&] () {
    auto r = edit1.get_selection();
    if (r.end() > 0) {
      edit1.set_selection({r.begin(), r.end() - 1}, ctrl::event_source::mouse);
    }
  });

  custom_button.on_mouse_enter([&]() {
    custom_button.set_hilited(true);
  });
  custom_button.on_mouse_leave([&]() {
    custom_button.set_hilited(false);
  });

  start_angle.on_scroll([&](core::point::type) {
    window2.invalidate();
  });
  end_angle.on_scroll([&](core::point::type) {
    window2.invalidate();
  });

  window2.on_mouse_enter([]() {
    LogDebug << "Window2 mouse enter";
  });
  window2.on_mouse_leave([]() {
    LogDebug << "Window2 mouse leave";
  });
  window1.on_move([](const core::point& p) {
    LogDebug << "Window1 move: " << p;
  });
  window1.on_size([](const core::size& s) {
    LogDebug << "Window1 size: " << s;
  });

//  table_view.columns.set_selection_filter([] (const ctrl::table::position& cell,
//                                              const ctrl::table::metric& geo) {
//    return (geo.selection.column == cell.column);
//  });

  table_view.set_data_source_and_target([&](const ctrl::table::position& cell) -> std::string {
    return table_data.get_cell(cell);
  }, [&](const ctrl::table::position& cell, const std::string& s) {
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
  htileview.set_spacing({ 5, 5 });
  
  vtileview.set_item_size({ 70, 25 });
  vtileview.set_background(color::very_light_gray);
  vtileview.set_border({ 10, 10 });
  vtileview.set_spacing({ 5, 5 });

  htileview.set_drawer(htile_drawer<draw::frame::sunken_relief>);
  vtileview.set_drawer(vtile_drawer<draw::frame::raised_relief>);

  on_create(util::bind_method(this, &my_main_window::onCreated));
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
  LogDebug << "Main created: this:" << std::hex << get_id() << ", w:" << w << ", rect:" << std::dec << r;
  created_children();
  LogDebug << "Children created: this:" << std::hex << get_id() << ", count:" << get_children().size();
}

template<int T, typename C>
void create_buttons (C& m, ctrl::label labels[T]) {
  for (int i = 0; i < T; ++i) {
    labels[i].create(m, ctrl::const_text(ostreamfmt("No. " << (i + 1))));
    m.get_layout().add(layout::lay(labels[i]));
  }
}

template<int T, typename C>
void create_group (my_main_window::group_group_t& m, C& c, ctrl::label labels[T]) {
  c.create(m);
  m.get_layout().add(layout::lay(c));
  create_buttons<T>(c, labels);
}

void my_main_window::created_children () {

  my_main_window& main = *this;

  win::detail::get_window(get_id());

  scroll_view.create(main, core::rectangle(0, 0, 300, 330));

  window1.create(scroll_view, core::rectangle(10, 10, 100, 280));
  window2.create(scroll_view, core::rectangle(120, 10, 200, 280));

  hseparator.create(main, core::rectangle(330, 10, 300, 2));
  vseparator.create(main, core::rectangle(310, 20, 2, 300));

  calc_button.create(main, [&]() {
    return calc_pressed ? "Recalc" : "Calc";
  }, core::rectangle(330, 20, 60, 25));

  inc_button.create(main, "+", core::rectangle(400, 20, 25, 25));
  dec_button.create(main, "-", core::rectangle(435, 20, 25, 25));
  invert_button.create(main, "invert", core::rectangle(470, 20, 100, 25));

  list1.create(main, core::rectangle(330, 50, 70, 250));
  list1.set_count(20);

  float floats[] = { 1.1F, 2.2F, 3.3F, 4.4F, 5.5F };

  auto columns = {
    layout::simple_column_info{ 30, text_origin::vcenter_right, 20 },
    layout::simple_column_info{ 30, text_origin::center, 20 },
    layout::simple_column_info{ 30, text_origin::vcenter_left, 20 }
  };

  ctrl::simple_column_list_data<int, draw::frame::lines> col_data = {
    { 1, 2, 3 },
    { 3, 4, 5 },
    { 5, 6, 7 },
    { 7, 8, 9 },
    { 9, 10, 11 }
  };

  main_split_view.create(main, core::rectangle(410, 50, 160, 250));
  main_split_view.first.second.set_data(ctrl::simple_list_data<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}));
  main_split_view.second.first.set_data<float>(ctrl::simple_list_data<float>(floats));
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
        const draw::brush&, ctrl::item_state state, text_origin) {
      ctrl::paint::text_item(g, r, color::buttonColor(), ostreamfmt(v), state, text_origin::center);
      draw::frame::raised_relief(g, r);
    },

    ctrl::cell_drawer<std::string, draw::frame::sunken_relief>,
    ctrl::cell_drawer<float, draw::frame::sunken_relief>,
    ctrl::cell_drawer<int, draw::frame::sunken_relief>,

    [] (const bool& v, const draw::graphics& g, const core::rectangle& r,
        const draw::brush& b, ctrl::item_state state, text_origin align) {
      std::string text = v ? u8"♣" : u8"♥";
      ctrl::paint::text_item(g, r, b, text, state, align);
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

  table_view.data.set_drawer(ctrl::table::default_data_drawer([&](const ctrl::table::position& cell) -> std::string {
    return table_data.get_cell(cell);//ostreamfmt(ctrl::table::build_std_column_name(column) << ':' << (row + 1));
  }));
  table_view.data.foregrounds.set_column(1, color::blue);
  table_view.data.foregrounds.set_column(2, color::red);
  table_view.data.foregrounds.set_row(3, color::dark_green);
  table_view.data.foregrounds.set_cell(ctrl::table::position(2, 3), color::black);
  table_view.data.backgrounds.set_cell(ctrl::table::position(2, 3), color::yellow);
  table_view.data.foregrounds.set_cell(ctrl::table::position(3, 4), color::blue);

  table_view.columns.set_drawer(ctrl::table::default_header_drawer([](const ctrl::table::position& cell) -> std::string {
    return ctrl::table::build_std_column_name(cell.x());
  }));
  table_view.geometrie.widths.set_size(2, 40);
  table_view.geometrie.spawns.set({4, 5}, {1, 2});
  table_data.set_cell({4, 5}, "spawn!");

  table_view.rows.set_drawer(ctrl::table::default_header_drawer([](const ctrl::table::position& cell) -> std::string {
    return ostreamfmt((1 + cell.y()));
  }));

  table_view.edge.set_text("0:0");
  table_view.enable_size(true, true);

  editor.create(main, core::rectangle(740, 320, 150, 250));
  LogDebug << "Create editor: " << editor.get_id();
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
  vtileview.set_count(0xffff - 32);

  hscroll.create(main, core::rectangle(550, 305, 130, static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width())));
  progress.create(main, core::rectangle(550, 325, 130, static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width())));

  vscroll.create(main, core::rectangle(700, 50, static_cast<core::size::type>(ctrl::scroll_bar::get_scroll_bar_width()), 250));
  vscroll.set_max((int)list1.get_count() * list1.get_item_dimension() - list1.size().height());
  vscroll.set_step(static_cast<ctrl::scroll_bar::type>(list1.get_item_dimension()));

  up_button.create(main, "Up", core::rectangle(330, 305, 47, 25));

  down_button.create(main, "Down", core::rectangle(383, 305, 47, 25));

  scroll_check_box.create(main, "Enable", core::rectangle(440, 305, 100, 20));
  scroll_check_box.set_checked(true);

  start_angle.set_min_max_step_value(0, 360, 1, 0);
  start_angle.create(main, core::rectangle(5, 340, 160, 12));
  end_angle.set_min_max_step_value(0, 360, 1, 260);
  end_angle.create(main, core::rectangle(5, 352, 160, 12));

  label.create(main, "Text", core::rectangle(50, 370, 120, 20));
  labelC.create(main, core::rectangle(50, 391, 120, 20));
  labelR.create(main, core::rectangle(50, 412, 120, 20));

  hslider.create(main, core::rectangle(5, 470, 500, 5));
  hslider.set_min_max(420, 600);

  vslider.create(main, core::rectangle(750, 5, 5, 500));
  vslider.set_min_max(570, 1800);

  chck_group.create(main, core::rectangle(180, 350, 100, 80));

  radio_button.create(chck_group, "Radio");
  radio_button2.create(chck_group, "Radio2", core::rectangle(0, 20, 100, 20));
  switch_button.create(chck_group, "Switch", core::rectangle(0, 40, 100, 20));
  check_box.create(chck_group, "Check", core::rectangle(0, 60, 100, 20));
  chck_group.get_layout().add({&radio_button, &radio_button2, &switch_button, &check_box});

  edit1.create(main, "Text zwei drei vier fuenf sechs sieben acht", core::rectangle(290, 350, 100, 25));

  edit_btn_group.create(main, core::rectangle(290, 380, 100, 16));

  cur_minus.create(edit_btn_group, "c-");
  cur_plus.create(edit_btn_group, "c+");
  sel_first_minus.create(edit_btn_group, "f-");
  sel_first_plus.create(edit_btn_group, "f+");
  sel_last_minus.create(edit_btn_group, "l-");
  sel_last_plus.create(edit_btn_group, "l+");
  edit_btn_group.get_layout().add({&cur_minus, &cur_plus, &sel_first_minus, &sel_first_plus, &sel_last_minus, &sel_last_plus});

  custom_button.set_drawer([] (const draw::graphics& g,
                               const core::rectangle& r,
                               const ctrl::button_state& s) {
    ctrl::paint::flat_button(g, r, "Custom", s);
  });

  custom_button.create(main, core::rectangle(290, 410, 100, 25));

  switch_button2.create(main, "Switcher", core::rectangle(20, 445, 150, 20));

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
  btn_group.get_layout().add({layout::lay(ok_button), layout::lay(del_button), layout::lay(clear_button)});

  btn_sep1.create(btn_group);
  btn_group.get_layout().add(layout::lay(btn_sep1), true);

  min_button.create(btn_group, "Min");
  max_button.create(btn_group, "Max");
  norm_button.create(btn_group, "Norm");
  btn_group.get_layout().add({layout::lay(min_button), layout::lay(max_button), layout::lay(norm_button)});

  btn_sep2.create(btn_group);
  btn_group.get_layout().add(layout::lay(btn_sep2), true);

  info_button.create(btn_group, "Info");
  null_button.create(btn_group, "0");
  full_button.create(btn_group, "Full");
  btn_group.get_layout().add({layout::lay(info_button), layout::lay(null_button), layout::lay(full_button)});

  group_group.create(main, core::rectangle(400, 345, 10, 10));
  create_group<4>(group_group, h_lineup_group, h_lineup_labels);
  create_group<4>(group_group, v_lineup_group, v_lineup_labels);
  create_group<4>(group_group, grid_lineup_group, grid_lineup_labels);
  create_group<4>(group_group, grid_adaption_group, grid_adaption_labels);
  create_group<4>(group_group, hc_lineup_group, hc_lineup_labels);
  create_group<4>(group_group, vc_lineup_group, vc_lineup_labels);

  using namespace layout;
  get_layout().attach_relative<What::left, make_relative(0.1), 20>(btn_group, *this);
  get_layout().attach_relative<What::right, make_relative(0.9), -20>(btn_group, *this);
  get_layout().attach_fix<What::top, Where::height, -45>(btn_group, *this);
  get_layout().attach_fix<What::bottom, Where::height>(btn_group, *this);

  get_layout().attach_relative<What::left, make_relative(0.1), 10>(hslider, *this);
  get_layout().attach_relative<What::right, make_relative(0.9)>(hslider, *this);

  get_layout().attach_fix<What::bottom, Where::y, -5>(vslider, btn_group);

  get_layout().attach_fix<What::right, Where::x, -25>(column_list, vslider);
  get_layout().attach_fix<What::left, Where::x, -20>(vscroll, vslider);
  get_layout().attach_fix<What::right, Where::x, -3>(vscroll, vslider);

  get_layout().attach_fix<What::right, Where::x, -4>(group_group, vslider);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(group_group, hslider);

  get_layout().attach_fix<What::left, Where::x2, 2>(table_view, vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(table_view, *this);

  get_layout().attach_fix<What::left, Where::x2, 2>(editor, vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(editor, *this);
  get_layout().attach_fix<What::top, Where::y, 320>(editor, *this);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(editor, hslider);

  get_layout().attach_fix<What::left, Where::x2, 2>(textbox, vslider);
  get_layout().attach_fix<What::right, Where::width, -10>(textbox, *this);
  get_layout().attach_fix<What::top, Where::y2, 4>(textbox, hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(textbox, *this);

  get_layout().attach_fix<What::bottom, Where::y2, -8>(switch_button2, hslider);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(drop_down, hslider);
  get_layout().attach_fix<What::bottom, Where::y2, -8>(color_drop_down, hslider);

  get_layout().attach_fix<What::top, Where::y2, 4>(htileview, hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(htileview, *this);

  get_layout().attach_fix<What::top, Where::y2, 4>(vtileview, hslider);
  get_layout().attach_fix<What::bottom, Where::height, -50>(vtileview, *this);
}

ctrl::paint_function my_main_window::create_paint1 () {
  return [&](const draw::graphics& graph) {
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
        draw_text_box(graph, texte[i], { core::point::type(10 + x * 55), core::point::type(155 + y * 25), 50, 22 }, origins[i]);
        draw_text(graph, texte[i], { core::point::type(10 + x * 80), core::point::type(235 + y * 20) }, origins[i]);
      }
    }

    if (draw_invert) {
      graph.invert({10, 10, 100, 100});
    }

  };
}

ctrl::paint_function my_main_window::create_paint2 () {
  return [&](const draw::graphics& graph) {
    //LogDebug << "win::paint 2";
    using namespace draw;

    graph.fill(rectangle(window2.client_area()), color::light_gray);

    graph.draw_pixel(core::native_point(3, 3), color::gray);
    graph.draw_pixel(core::native_point(6, 6), color::gray);

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

    auto start = start_angle.get_value();
    auto end = end_angle.get_value();

    core::point pos4(150, 30);
    graph.frame(arc(pos4, 20, start, end), color::blue);
    graph.fill(arc(pos4 + offs1, 20, start, end), color::dark_green);
    graph.draw(arc(pos4 + offs2, 20, start, end), color::red, color::cyan);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

    if (draw_invert) {
      graph.invert({10, 10, 100, 100});
    }

  };
}
