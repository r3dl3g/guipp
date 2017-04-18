
#include "label.h"
#include "button.h"
#include "list.h"
#include "scroll_view.h"
#include "slider.h"
#include "split_view.h"
#include "edit.h"
#include "column_list.h"
#include "dbg_win_message.h"


std::ostream& operator<<(std::ostream& out, const bool& b) {
  out << (b ? "true" : "false");
  return out;
}

#include <logger.h>

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
      LogDebug << "Message: " << win::EventId(e) IF_WIN32(<< " (" << std::hex << e.param_1 << ", " << e.param_2 << ")");
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


class my_main_window : public win::layout_main_window<layout::attach> {
public:
  my_main_window (win::paint_event p1, win::paint_event p2);

  void onCreated (win::window*, const core::rectangle&);
  void created_children ();
  void query_state ();

  static win::paint_event create_paint1();
  static win::paint_event create_paint2();

private:
  win::scroll_view view;

  win::client_window window1;
  win::client_window window2;

  win::text_button calc_button;
  win::text_button inc_button;
  win::text_button dec_button;

  win::text_button ok_button;
  win::text_button del_button;
  win::text_button clear_button;

  win::group_window<layout::horizontal_adaption<5, 10>> btn_group;
  win::group_window<layout::vertical_adaption<5, 5>> chck_group;

  win::group_window<layout::horizontal_adaption<5, 5>> group_group;

  win::group_window<layout::horizontal_lineup<30, 5, 5>> h_lineup_group;
  win::label h_lineup_labels[4];

  win::group_window<layout::vertical_lineup<30, 5, 5>> v_lineup_group;
  win::label v_lineup_labels[4];

  win::group_window<layout::grid_lineup<30, 30, 5, 5>> grid_lineup_group;
  win::label grid_lineup_labels[4];

  win::group_window<layout::grid_adaption<2, 2, 5, 5>> grid_adaption_group;
  win::label grid_adaption_labels[4];

  win::radio_button radio_button, radio_button2;
  win::check_box check_box;
  win::label label;
  win::label_center labelC;
  win::label_right labelR;

  win::text_button min_button;
  win::text_button max_button;
  win::text_button norm_button;
  win::text_button info_button;

  win::simple_list_data<std::string> data;

  win::list list1;
  win::list& list2;
  win::list& list3;

  typedef win::split_view_t<false, win::list, win::list> list_split_view;
  typedef win::simple_column_list<layout::simple_column_list_layout> simple_list;
  typedef win::split_view_t<false, win::hlist, simple_list> column_list_split_view;

  win::split_view_t<true, list_split_view, column_list_split_view> vsplit_view;

  win::text_button up_button;
  win::text_button down_button;

  win::vscroll_bar vscroll;
  win::hscroll_bar hscroll;

  win::check_box scroll_check_box;

  win::hslider hslider;
  win::vslider vslider;
  win::paint_event paint1;
  win::paint_event paint2;

  win::edit edit1;

  win::group_window<layout::horizontal_adaption<0, 2>> edit_btn_group;
  win::text_button cur_plus;
  win::text_button cur_minus;
  win::text_button sel_first_plus;
  win::text_button sel_first_minus;
  win::text_button sel_last_plus;
  win::text_button sel_last_minus;

  win::custom_push_button custom_button;

  typedef win::column_list_t<layout::weight_column_list_layout, int, std::string, float, int, bool> my_column_list_t;
  my_column_list_t column_list;
  //my_column_list_t::standard_data column_list_data;
  my_column_list_t::row_drawer column_list_drawer;

  bool at_paint1;
  bool at_drag;
  core::point last_pos;
};

int gui_main(const std::vector<std::string>& args) {

  win::paint_event paint1(my_main_window::create_paint1());
  win::paint_event paint2(my_main_window::create_paint2());

  my_main_window main(paint1, paint2);

  LogDebug << "window size:" << sizeof(main)  << ", window_class size:" << sizeof(win::window_class);
  LogDebug << "long size:" << sizeof(long)<< ", pointer size:" << sizeof(void*);

//#ifdef WIN32
//  main.register_event_handler(win::get_minmax_event([](const core::size& sz,
//    const core::point& pos,
//    core::size& mi, core::size& ma) {
//    mi = { 300, 200 };
//    ma = { 880, 660 };
//    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
//  }));
//#endif

  const core::rectangle r = core::rectangle(50, 50, 1000, 600);
  LogDebug << "Create Main: " << r;
  main.create(r);
  main.set_title("Window Test");

  main.set_visible();
  main.redraw_later();

  return win::run_main_loop();
}

my_main_window::my_main_window (win::paint_event p1, win::paint_event p2)
  : paint1(p1)
  , paint2(p2)
  , at_paint1(true)
  , at_drag(false)
  , list2(vsplit_view.first.first)
  , list3(vsplit_view.first.second)
{
  register_event_handler(init_result_handler(), 0);

  register_event_handler(win::destroy_event([&] () {
    LogDebug << "Destroyed!";
    quit();
  }));


//#ifdef WIN32
//  register_event_handler(win::close_event([&]() {
//    LogDebug << "Close!";
//    destroy();
//  }));
//  register_event_handler(win::enable_event([](bool on) {
//    LogDebug << (on ? "Enableed" : "Disabled");
//  }));
//  register_event_handler(win::activate_event([](bool on, win::window* win) {
//    LogDebug << "Main " << (on ? "activate" : "deactivate");
//  }));
//  register_event_handler(win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
//  register_event_handler(win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
//  register_event_handler(win::activate_app_event([](bool on) {
//    LogDebug << (on ? "A" : "Dea") << "ctivate App";
//  }));

//#endif

  register_event_handler(win::moving_event([] (const core::point& r) {
    LogDebug << "Main moving: " << r;
  }));
  register_event_handler(win::sizing_event([] (const core::size& r) {
    LogDebug << "Main sizing: " << r;
  }));
  register_event_handler(win::placing_event([] (const core::rectangle& r) {
    LogDebug << "Main placing: " << r;
  }));

  register_event_handler(win::move_event([] (const core::point& p) {
    LogDebug << "Main move: " << p;
  }));
  register_event_handler(win::size_event([] (const core::size& s) {
    LogDebug << "Main size: " << s;
  }));
  register_event_handler(win::place_event([] (const core::rectangle& r) {
    LogDebug << "Main place: " << r;
  }));

//#ifdef WIN32
//  ok_button.register_event_handler(win::activate_event([](bool on, win::window* win) {
//    LogDebug << "Button " << (on ? "" : "de") << "activate";
//  }));
//#endif
  ok_button.register_event_handler(win::set_focus_event([] (win::window* win) {
    LogDebug << "Button Set Focus";
  }));
  ok_button.register_event_handler(win::lost_focus_event([&] (win::window* win) {
    LogDebug << "Button Lost Focus";
  }));

  register_event_handler(win::left_btn_down_event([&] (const core::point& p) {
    LogDebug << "Left Button Down at " << p;
  }));
  register_event_handler(win::left_btn_up_event([&] (const core::point& p) {
    LogDebug << "Left Button Up at " << p;
  }));
  register_event_handler(win::right_btn_down_event([&] (const core::point& p) {
    LogDebug << "Right Button Down at " << p;
  }));
  register_event_handler(win::right_btn_up_event([&] (const core::point& p) {
    LogDebug << "Right Button Up at " << p;
  }));
  window1.register_event_handler(win::wheel_x_event([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(delta, 0));
    }
  }));
  window1.register_event_handler(win::wheel_y_event([&] (core::point::type delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  }));

  window1.register_event_handler(win::left_btn_down_event([&](const core::point& p) {
    at_drag = true;
    last_pos = p;
    window1.capture_pointer();
    LogDebug << "Window1 Mouse down at " << p;
  }));
  window1.register_event_handler(win::left_btn_up_event([&](const core::point& p) {
    window1.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window Mouse up at " << p;
  }));

  window2.register_event_handler(win::left_btn_down_event([&](const core::point& p) {
    at_drag = true;
    last_pos = p;
    window2.capture_pointer();
    LogDebug << "Window2 Mouse down at " << p;
  }));
  window2.register_event_handler(win::left_btn_up_event([&](const core::point& p) {
    window2.uncapture_pointer();
    at_drag = false;
    LogDebug << "Window Mouse up at " << p;
  }));

  window1.register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                            const core::point& p) {
    //LogDebug << "Window Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      core::point delta = p - last_pos;
      //last_pos = p;
      window1.move(window1.position() + delta);
    }
  }));
  window2.register_event_handler(win::mouse_move_event([&] (unsigned int keys,
                                                            const core::point& p) {
    //LogDebug << "Window Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      core::point delta = p - last_pos;
      //last_pos = p;
      window2.move(window2.position() + delta);
    }
  }));

  window1.register_event_handler(win::left_btn_dblclk_event([&] (const core::point& p) {
    LogDebug << "Window1 Double Click up at " << p;
    window2.set_visible(!window2.is_visible());
  }));


  register_event_handler(win::left_btn_dblclk_event([&] (const core::point& p) {
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
  register_event_handler(win::right_btn_dblclk_event([&] (const core::point& p) {
    window1.move({50, 50});
  }));

  window2.register_event_handler(paint1);

  at_paint1 = true;

  window2.register_event_handler(win::left_btn_dblclk_event([&] (const core::point& p) {
    LogDebug << "Window2 Double Click up at " << p;
    if (at_paint1) {
      at_paint1 = false;
      window2.unregister_event_handler(paint1);
      window2.register_event_handler(paint2);
    } else {
      at_paint1 = true;
      window2.unregister_event_handler(paint2);
      window2.register_event_handler(paint1);
    }
    window2.redraw_later();
  }));
  window2.register_event_handler(win::show_event([] () {
    LogDebug << "Window2 show:";
  }));
  window2.register_event_handler(win::hide_event([] () {
    LogDebug << "Window2 hide:";
  }));

//#ifdef WIN32
//  ok_button.register_event_handler(win::button_state_event([](bool state) {
//    LogDebug << "Button " << (state ? "hilited" : "unhilited");
//  }));
//#endif // WIN32
  ok_button.register_event_handler(win::button_pushed_event([&] () {
    LogDebug << "Button pushed";
    label.set_text("Pushed!");
    label.redraw_now();
  }));
  ok_button.register_event_handler(win::button_released_event([&] () {
    LogDebug << "Button released";
    label.set_text("Released!");
    label.redraw_now();
  }));
  radio_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Radio clicked";
    labelR.set_text("Radio clicked!");
    bool check = radio_button.is_checked();
//    radio_button.set_checked(!check);
    radio_button2.set_checked(!check);
  }));
  radio_button2.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Radio2 clicked";
    labelR.set_text("Radio2 clicked!");
    bool check = radio_button2.is_checked();
//    radio_button2.set_checked(!check);
    radio_button.set_checked(!check);
  }));
  check_box.register_event_handler(win::button_state_event([&] (bool on) {
    LogDebug << "Check clicked";
    label.set_text("Check clicked!");
    radio_button.enable(on);
    radio_button2.enable(on);
    ok_button.enable(on);
  }));

  min_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Min clicked";
    minimize();
    query_state();
  }));
  max_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Max clicked";
    maximize();
    query_state();
  }));
  norm_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Norm clicked";
    restore();
    query_state();
  }));
  info_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Info clicked";
    query_state();
  }));

  auto list_drawer = [] (int idx,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& background,
                         bool selected) {
    using namespace draw;

    std::ostringstream strm;
    strm << "Item " << idx;

    win::paint::text_item(strm.str(), g, place, background, selected);
  };

  list1.set_drawer(list_drawer, core::size(0, 25));
  list1.register_event_handler(win::selection_changed_event([&] () {
    std::ostringstream strm;
    strm << "List1 item " << list1.get_selection();
    labelC.set_text(strm.str());
  }));

  up_button.register_event_handler(win::button_clicked_event([&] () {
    list1.set_selection(list1.get_selection() - 1);
    list2.set_selection(list2.get_selection() - 1);
    list3.set_selection(list3.get_selection() - 1);
  }));
  down_button.register_event_handler(win::button_clicked_event([&] () {
    list1.set_selection(list1.get_selection() + 1);
    list2.set_selection(list2.get_selection() + 1);
    list3.set_selection(list3.get_selection() + 1);
  }));

  data.insert(data.end(), { "Eins", "Zwei", "Drei", "View", "Fünf", "Fuß" });
  list2.set_drawer([&] (int idx,
                        const draw::graphics& g,
                        const core::rectangle& place,
                        const draw::brush& background,
                        bool selected) {
    data(idx, g, place, background, selected);
  }, core::size(0, 16));

  list2.register_event_handler(win::selection_changed_event([&] () {
    std::ostringstream strm;
    strm << "List2 item " << list2.get_selection() << ": " << data[list2.get_selection()];
    labelC.set_text(strm.str());
  }));

  column_list.list.register_event_handler(win::selection_changed_event([&] () {
    std::ostringstream strm;
    strm << "column_list item " << column_list.list.get_selection();
    labelC.set_text(strm.str());
  }));

  ok_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Ok Button clicked";
    label.set_text("OK Clicked!");
    data.insert(data.end(), { "Sechs", "Sieben", "Acht", "Neun", "Zehn" });
    data.update_list(list2);
  }));

  del_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Del Button clicked";
    label.set_text("Del Clicked!");
    data.erase(data.begin());
    data.update_list(list2);
  }));

  clear_button.register_event_handler(win::button_clicked_event([&] () {
    LogDebug << "Clear Button clicked";
    label.set_text("Clear Clicked!");
    data.clear();
    data.update_list(list2);
  }));

  scroll_check_box.register_event_handler(win::button_state_event([&] (bool on) {
    list1.enable_scroll_bar(on);
    list2.enable_scroll_bar(on);
  }));

  vscroll.register_event_handler(win::scroll_event([&](core::point::type pos) {
    list1.set_scroll_pos(pos);
    list2.set_scroll_pos(pos);
    list3.set_scroll_pos(pos);
  }));

  calc_button.register_event_handler(win::button_clicked_event([&] () {
    view.layout();
  }));

  inc_button.register_event_handler(win::button_clicked_event([&] () {
    view.resize(view.size() + core::size{5, 5});
  }));
  dec_button.register_event_handler(win::button_clicked_event([&] () {
    view.resize(view.size() - core::size{5, 5});
  }));

  vslider.register_event_handler(win::move_event([&](const core::point&) {
    layout();
  }));
  hslider.register_event_handler(win::move_event([&](const core::point&) {
    layout();
  }));

  hscroll.register_event_handler(win::scroll_event([&](core::point::type pos) {
    vsplit_view.set_split_pos((double)pos / 100.0);
  }));
  vsplit_view.slider.register_event_handler(win::move_event([&](const core::point&){
    hscroll.set_value(static_cast<win::scroll_bar::type>(vsplit_view.get_split_pos() * hscroll.get_max()));
  }));

  cur_plus.register_event_handler(win::button_clicked_event([&] () {
    edit1.set_cursor_pos(edit1.get_cursor_pos() + 1);
  }));
  cur_minus.register_event_handler(win::button_clicked_event([&] () {
    if (edit1.get_cursor_pos() > 0) {
      edit1.set_cursor_pos(edit1.get_cursor_pos() - 1);
    }
  }));
  sel_first_plus.register_event_handler(win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    (r.first)++;
    edit1.set_selection(r);
  }));
  sel_first_minus.register_event_handler(win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    if (r.first > 0) {
      (r.first)--;
      edit1.set_selection(r);
    }
  }));
  sel_last_plus.register_event_handler(win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    (r.last)++;
    edit1.set_selection(r);
  }));
  sel_last_minus.register_event_handler(win::button_clicked_event([&] () {
    auto r = edit1.get_selection();
    if (r.last > 0) {
      (r.last)--;
      edit1.set_selection(r);
    }
  }));

  custom_button.register_event_handler(win::mouse_enter_event([&]() {
    custom_button.set_hilited(true);
  }));
  custom_button.register_event_handler(win::mouse_leave_event([&]() {
    custom_button.set_hilited(false);
  }));

  /*
    window2.register_event_handler(win::mouse_enter_event([]() {
    LogDebug << "Window2 mouse enter";
  }));
  window2.register_event_handler(win::mouse_leave_event([]() {
    LogDebug << "Window2 mouse leave";
  }));
  window1.register_event_handler(win::move_event([](const core::point& p) {
    LogDebug << "Window1 move: " << p;
  }));
  window1.register_event_handler(win::size_event([](const core::size& s) {
    LogDebug << "Window1 size: " << s;
  }));
  */

  register_event_handler(win::create_event(core::bind_method(this, &my_main_window::onCreated)));
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
    labels[i].create(m, core::rectangle::def, ostreamfmt("No. " << (i + 1)));
    labels[i].set_visible();
  }
}

template<int T, typename C>
void create_group (win::container& m, C& c, win::label labels[T]) {
  c.create(m);
  create_buttons<T>(c, labels);
  c.set_visible();
}

void my_main_window::created_children () {

  my_main_window& main = *this;

  view.create(main, core::rectangle(0, 0, 300, 330));
  view.set_visible();

  window1.create(view, core::rectangle(10, 10, 100, 280));
  window1.set_visible();

  window2.create(view, core::rectangle(120, 10, 200, 280));
  window2.set_visible();

  calc_button.create(main, core::rectangle(330, 20, 60, 25), "Calc");
  calc_button.set_visible();

  inc_button.create(main, core::rectangle(400, 20, 25, 25), "+");
  inc_button.set_visible();
  dec_button.create(main, core::rectangle(435, 20, 25, 25), "-");
  dec_button.set_visible();

  list1.create(main, core::rectangle(330, 50, 70, 250));
  list1.set_count(20);
  list1.set_visible();

  float floats[] = { 1.1F, 2.2F, 3.3F, 4.4F, 5.5F };

  auto columns = {
    layout::simple_column_info{ 30, draw::vcenter_right, 20 },
    layout::simple_column_info{ 30, draw::center, 20 },
    layout::simple_column_info{ 30, draw::vcenter_left, 20 }
  };

  win::simple_column_list_data<int, draw::frame::lines> col_data = {
    { 1, 2, 3 },
    { 3, 4, 5 },
    { 5, 6, 7 },
    { 7, 8, 9 },
    { 9, 10, 11 }
  };

  vsplit_view.create(main, core::rectangle(410, 50, 160, 250));
  vsplit_view.first.second.set_data(win::simple_list_data<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), 16);
  vsplit_view.second.first.set_data(win::simple_list_data<float>(floats), 25);
  vsplit_view.second.second.get_column_layout().set_columns(columns);
  vsplit_view.second.second.set_data(col_data);
  vsplit_view.set_visible();

  data.update_list(list2);

  //column_list_data = my_column_list_t::standard_data{ std::make_tuple(1, "eins", 1.1F),
  //                                                    std::make_tuple(2, "zwei", 2.2F),
  //                                                    std::make_tuple(3, "drei", 3.3F) };

  auto weight_columns = {
    layout::weight_column_info{ 30, draw::vcenter_left, 20, 0.0F },
    layout::weight_column_info{ 30, draw::vcenter_right, 20, 1.0F },
    layout::weight_column_info{ 30, draw::center, 20, 1.0F },
    layout::weight_column_info{ 30, draw::center, 20, 1.0F },
    layout::weight_column_info{ 30, draw::center, 20, 1.0F }
  };

  column_list_drawer = {
    [](const int& v, const draw::graphics& g, const core::rectangle& r, const draw::brush&b, bool s, draw::text_origin align) {
      win::paint::text_item(ostreamfmt(v), g, r, draw::color::buttonColor(), false, draw::center);
      draw::frame::raised_relief(g, r);
    },

    win::cell_drawer<std::string, draw::frame::lines>,
    win::cell_drawer<float, draw::frame::lines>,
    win::cell_drawer<int, draw::frame::lines>,

    [](const bool& v, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, bool s, draw::text_origin align) {
      std::string text = v ? IF_NOT_VC12(u8"\u25C9" : u8"\u25CB") IF_VC12("X" : "-");
      win::paint::text_item(text, g, r, b, s, align);
      draw::frame::lines(g, r);
    }
  };

//  column_list.set_data(column_list_data, column_list_data.size());
  column_list.create(main, core::rectangle(580, 50, 140, 250));
  column_list.header.set_cell_drawer([](int i, const draw::graphics& g, const core::rectangle& r, const draw::brush&) {
    using namespace draw;
    frame::raised_relief(g, r);
    g.text(text_box(ostreamfmt((char)('C' + i) << (i + 1)), r, center), font::system(), color::windowTextColor());
  });
  column_list.set_drawer(column_list_drawer, 25);
  column_list.get_column_layout().set_columns(weight_columns);
  column_list.set_data([](int i){
    return std::make_tuple(i, ostreamfmt(i << '-' << i), (1.1F * (float)i), i * i, i % 2 == 1);
  }, 20);
  column_list.set_visible();
  column_list.get_column_layout().get_slider(0)->disable();
  column_list.layout();

  hscroll.create(main, core::rectangle(550, 305, 130, win::scroll_bar::get_scroll_bar_width()));
  hscroll.set_visible();

  vscroll.create(main, core::rectangle(700, 50, win::scroll_bar::get_scroll_bar_width(), 250));
  vscroll.set_max((int)list1.get_count() * list1.get_item_size().height() - list1.size().height());
  vscroll.set_step(list1.get_item_size().height());
  vscroll.set_visible();

  up_button.create(main, core::rectangle(330, 305, 47, 25), "Up");
  up_button.set_visible();

  down_button.create(main, core::rectangle(383, 305, 47, 25), "Down");
  down_button.set_visible();

  scroll_check_box.create(main, core::rectangle(440, 305, 100, 20), "Enable");
  scroll_check_box.set_checked(true);
  scroll_check_box.set_visible();

  label.create(main, core::rectangle(50, 350, 120, 20), "Text");
  label.set_visible();
  label.redraw_later();

  labelC.create(main, core::rectangle(50, 371, 120, 20));
  labelC.set_visible();
  labelC.redraw_later();

  labelR.create(main, core::rectangle(50, 392, 120, 20));
  labelR.set_visible();
  labelR.redraw_later();

  hslider.create(main, core::rectangle(5, 335, 500, 5));
  hslider.set_visible();
  hslider.set_min_max(330, 345);

  vslider.create(main, core::rectangle(750, 5, 5, 500));
  vslider.set_visible();
  vslider.set_min_max(570, 1200);

  chck_group.create(main, core::rectangle(180, 350, 100, 80));
  chck_group.set_visible();

  radio_button.create(chck_group, core::rectangle::def, "Radio");
  radio_button.set_visible();
  radio_button.redraw_later();

  radio_button2.create(chck_group, core::rectangle(0, 20, 100, 20), "Radio2");
  radio_button2.set_visible();
  radio_button2.redraw_later();

  check_box.create(chck_group, core::rectangle(0, 40, 100, 20), "Check");
  check_box.set_visible();
  check_box.redraw_later();

  chck_group.layout();

  edit1.create(main, core::rectangle(290, 350, 100, 25), "Text zwei drei vier fuenf sechs sieben acht");
  edit1.set_visible();

  edit_btn_group.create(main, core::rectangle(290, 380, 100, 16));
  edit_btn_group.set_visible();

  cur_minus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "c-");
  cur_minus.set_visible();
  cur_plus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "c+");
  cur_plus.set_visible();
  sel_first_minus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "f-");
  sel_first_minus.set_visible();
  sel_first_plus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "f+");
  sel_first_plus.set_visible();
  sel_last_minus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "l-");
  sel_last_minus.set_visible();
  sel_last_plus.create(edit_btn_group, core::rectangle(0, 0, 16, 16), "l+");
  sel_last_plus.set_visible();

  edit_btn_group.layout();

  custom_button.set_drawer([](const draw::graphics& g, const win::button& btn) {
    core::rectangle r = btn.client_area();

    if (btn.is_checked()) {
      g.fill(draw::rectangle(r), draw::color::darkGray());
    } else
    if (btn.is_hilited()) {
      g.fill(draw::rectangle(r), draw::color::lightGray());
    } else
    {
      g.fill(draw::rectangle(r), draw::color::workSpaceColor());
    }
    g.text(draw::text_box("Custom", r, draw::center), draw::font::serif(), draw::color::white());
  });

  custom_button.create(main, core::rectangle(290, 410, 100, 25));
  custom_button.set_visible();

  btn_group.create(main, core::rectangle(10, 440, 780, 35));
  btn_group.set_visible();

  ok_button.create(btn_group, core::rectangle(380, 350, 100, 25), "Ok");
  ok_button.set_visible();
  ok_button.redraw_later();

  del_button.create(btn_group, core::rectangle(490, 350, 100, 25), "Del");
  del_button.set_visible();
  del_button.redraw_later();

  clear_button.create(btn_group, core::rectangle(600, 350, 100, 25), "Clear");
  clear_button.set_visible();
  clear_button.redraw_later();

  min_button.create(btn_group, core::rectangle(180, 400, 60, 25), "Min");
  min_button.set_visible();

  max_button.create(btn_group, core::rectangle(250, 400, 60, 25), "Max");
  max_button.set_visible();

  norm_button.create(btn_group, core::rectangle(320, 400, 60, 25), "Norm");
  norm_button.set_visible();

  info_button.create(btn_group, core::rectangle(390, 400, 90, 25), "Info");
  info_button.set_visible();

  btn_group.layout();

  group_group.create(main, core::rectangle(400, 345, 300, 115));
  create_group<4>(group_group, h_lineup_group, h_lineup_labels);
  create_group<4>(group_group, v_lineup_group, v_lineup_labels);
  create_group<4>(group_group, grid_lineup_group, grid_lineup_labels);
  create_group<4>(group_group, grid_adaption_group, grid_adaption_labels);
  group_group.set_visible();
  group_group.layout();

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
  get_layout().attach_fix<What::right, Where::x, -4>(&vscroll, &vslider);

  get_layout().attach_fix<What::right, Where::x, -4>(&group_group, &vslider);
  get_layout().attach_fix<What::top, Where::y2, 4>(&group_group, &hslider);

  layout();
}

win::paint_event my_main_window::create_paint1 () {
  return win::paint_event([](const draw::graphics& graph) {
    //LogDebug << "win::paint 1";

    using namespace draw;

    graph.frame(polygon(calc_star(10, 10, 40, 40)), color::blue());
    graph.fill(polygon(calc_star(60, 10, 40, 40)), color::darkGreen());
    graph.draw(polygon(calc_star(110, 10, 40, 40)), color::yellow(), color::red());

    graph.frame(polygon({ { 5, 5 }, { 155, 5 }, {75, 8} }), color::red());

    core::point pt(10, 60);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 1!", pt), font::system(), color::black());

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 2!", pt), font::system_bold(), color::black());

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 3!", pt), font::sans_serif().with_italic(true), color::black());

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 4!", pt), font::serif(), color::black());

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 5!", pt), font::monospace(), color::black());

    pt.move_y(15);
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("Hello World 6!", pt), font::sans_serif().with_size(18), color::blue());

    core::rectangle r(10, 155, 50, 18);
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("TL", r, top_left), font::system(), color::red());
    r= {70, 155, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("TC", r, top_hcenter), font::system(), color::red());
    r= {130, 155, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("TR", r, top_right), font::system(), color::red());

    r= {10, 175, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("CL", r, vcenter_left), font::system(), color::red());
    r= {70, 175, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("CC", r, center), font::system(), color::red());
    r= {130, 175, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("CR", r, vcenter_right), font::system(), color::red());

    r= {10, 195, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("BL", r, bottom_left), font::system(), color::red());
    r= {70, 195, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("BC", r, bottom_hcenter), font::system(), color::red());
    r= {130, 195, 50, 18};
    graph.frame(rectangle(r), color::blue());
    graph.text(text_box("BR", r, bottom_right), font::system(), color::red());

    pt = {10, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("TL", pt, top_left), font::system(), color::red());
    pt = {70, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("TC", pt, top_hcenter), font::system(), color::red());
    pt = {130, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("TR", pt, top_right), font::system(), color::red());

    pt = {10, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("CL", pt, vcenter_left), font::system(), color::red());
    pt = {70, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("CC", pt, center), font::system(), color::red());
    pt = {130, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("CR", pt, vcenter_right), font::system(), color::red());

    pt = {10, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("BL", pt, bottom_left), font::system(), color::red());
    pt = {70, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("BC", pt, bottom_hcenter), font::system(), color::red());
    pt = {130, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue());
    graph.text(text("BR", pt, bottom_right), font::system(), color::red());
  });
}

win::paint_event my_main_window::create_paint2 () {
  return win::paint_event([](const draw::graphics& graph) {
    //LogDebug << "win::paint 2";
    using namespace draw;

    pen blue(color::blue());
    color red = color::red();
    color darkGreen = color::darkGreen();
    color yellow = color::cyan();

    graph.draw_pixel(core::point(3, 3), color::gray());
    graph.draw_pixel(core::point(6, 6), color::gray());

    core::size sz(30, 50);
    core::size offs1(0, 60);
    core::size offs2(0, 120);

    core::point pos1(10, 10);
    graph.frame(rectangle(pos1, sz), blue);
    graph.fill(rectangle(pos1 + offs1, sz), darkGreen);
    graph.draw(rectangle(pos1 + offs2, sz), red, yellow);

    core::point pos2(50, 10);
    graph.frame(ellipse(pos2, sz), blue);
    graph.fill(ellipse(pos2 + offs1, sz), darkGreen);
    graph.draw(ellipse(pos2 + offs2, sz), red, yellow);

    core::point pos3(90, 10);
    core::size rd(10, 10);
    graph.frame(round_rectangle(core::rectangle(pos3, sz), rd), blue);
    graph.fill(round_rectangle(core::rectangle(pos3 + offs1, sz), rd), darkGreen);
    graph.draw(round_rectangle(core::rectangle(pos3 + offs2, sz), rd), red, yellow);

    core::point pos4(150, 30);
    graph.frame(arc(pos4, 20, 0, 350), blue);
    graph.fill(arc(pos4 + offs1, 20, 0, 350), darkGreen);
    graph.draw(arc(pos4 + offs2, 20, 0, 350), red, yellow);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

  });
}
