
#include "control.h"
#include "dbg_win_message.h"


struct AsBool {
  inline AsBool(bool b)
    : b(b) {
  }

  bool b;
};

std::ostream& operator<<(std::ostream& out, const AsBool& b) {
  out << (b.b ? "true" : "false");
  return out;
}

#include <logger.h>
#include <dbgstream.h>

#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)

using namespace gui;

class log_all_events {
public:
  log_all_events() {
  }

  bool operator()(const core::event& e, core::event_result& result) {
    if ((result == 0xdeadbeef) &&
        !win::is_none_client_event(e) &&
        !win::is_frequent_event(e) ) {
      LogDebug << "Message: " << win::EventId(e)
#ifdef WIN32
               << " (" << std::hex << e.param_1 << ", " << e.param_2 << ")"
#endif
               ;
    }
    return false;
  }
};

class init_result_handler {
public:
  init_result_handler() {
  }

  bool operator()(const core::event& e, core::event_result& result) {
    result = 0xdeadbeef;
    return false;
  }
};

std::vector<core::point> calc_star(int x, int y, int w, int h) {
  int x1 = x + w / 4;
  int x2 = x + w / 2;
  int x3 = x + w * 3 / 4;
  int x4 = x + w;
  int y1 = y + h / 4;
  int y2 = y + h / 2;
  int y3 = y + h * 3 / 4;
  int y4 = y + h;
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

  win::push_button calc_button;
  win::push_button inc_button;
  win::push_button dec_button;

  win::push_button ok_button;
  win::push_button del_button;
  win::push_button clear_button;

  win::group_window<layout::horizontal_lineup<5, 10>> btn_group;
  win::group_window<layout::vertical_lineup<5, 5>> chck_group;

  win::radio_button radio_button, radio_button2;
  win::check_box check_box;
  win::label label;
  win::label_center labelC;
  win::label_right labelR;

  win::push_button min_button;
  win::push_button max_button;
  win::push_button norm_button;
  win::push_button info_button;

  win::list::data<std::string> data;

  win::list list1;
  win::list& list2;
  win::list& list3;

  typedef win::split_viewT<false, win::list, win::list> list_split_view;
  typedef win::split_viewT<false, win::list, win::column_list> column_list_split_view;
  win::split_viewT<true, list_split_view, column_list_split_view> vsplit_view;

  win::push_button up_button;
  win::push_button down_button;

  win::vscroll_bar vscroll;
  win::hscroll_bar hscroll;

  win::check_box scroll_check_box;

  win::hslider hslider;
  win::vslider vslider;

  win::paint_event paint1;
  win::paint_event paint2;

  win::edit edit1;

  win::group_window<layout::horizontal_lineup<0, 3>> edit_btn_group;
  win::push_button cur_plus;
  win::push_button cur_minus;
  win::push_button sel_first_plus;
  win::push_button sel_first_minus;
  win::push_button sel_last_plus;
  win::push_button sel_last_minus;

  win::column_list column_list;

  bool at_paint1;
  bool at_drag;
  core::point last_pos;
};

#ifdef WIN32
int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  ibr::odebugstream dbgStrm;
  ibr::log::core::instance().addSink(&dbgStrm, ibr::log::level::debug, ibr::log::core::instance().getConsoleFormatter());

  gui::core::global::init(hInstance);
#elif X11
int main(int argc, char* argv[]) {
  ibr::log::core::instance().addSink(&std::cerr,
                                     ibr::log::level::debug,
                                     ibr::log::core::instance().getConsoleFormatter());

  gui::core::global::init(XOpenDisplay(0));
#endif

  win::paint_event paint1(my_main_window::create_paint1());
  win::paint_event paint2(my_main_window::create_paint2());

  my_main_window main(paint1, paint2);

  LogDebug << "window size:" << sizeof(main)  << ", window_class size:" << sizeof(win::window_class);
  LogDebug << "long size:" << sizeof(long)<< ", pointer size:" << sizeof(void*);

#ifdef WIN32
  main.register_event_handler(win::get_minmax_event([](const core::size& sz,
    const core::point& pos,
    core::size& mi, core::size& ma) {
    mi = { 300, 200 };
    ma = { 880, 660 };
    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
  }));
#endif

  const core::rectangle& r = core::rectangle(50, 50, 800, 480);
  LogDebug << "Create Main: " << r;
  main.create(r);
  main.set_title("Window Test");

  main.set_visible();
  main.redraw_later();

  int ret = 0;
  try {
    ret = win::run_main_loop();
  } catch (std::exception e) {
    LogFatal << e;
  }

  gui::core::global::fini();

#ifdef X11
  ibr::log::core::instance().removeSink(&std::cerr);
#endif

  ibr::log::core::instance().finish();

  return ret;
}

my_main_window::my_main_window (win::paint_event p1, win::paint_event p2)
  : paint1(p1)
  , paint2(p2)
  , at_paint1(true)
  , at_drag(false)
  , list2(vsplit_view.first.first)
  , list3(vsplit_view.first.second)
{
  register_event_handler(init_result_handler());

  register_event_handler(win::destroy_event([&] () {
    LogDebug << "Destroyed!";
    quit();
  }));


#ifdef WIN32
  register_event_handler(win::close_event([&]() {
    LogDebug << "Close!";
    destroy();
  }));
  register_event_handler(win::enable_event([](bool on) {
    LogDebug << (on ? "Enableed" : "Disabled");
  }));
  register_event_handler(win::activate_event([](bool on, win::window* win) {
    LogDebug << "Main " << (on ? "activate" : "deactivate");
  }));
  register_event_handler(win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
  register_event_handler(win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
  register_event_handler(win::activate_app_event([](bool on) {
    LogDebug << (on ? "A" : "Dea") << "ctivate App";
  }));

#endif

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

#ifdef WIN32
  ok_button.register_event_handler(win::activate_event([](bool on, win::window* win) {
    LogDebug << "Button " << (on ? "" : "de") << "activate";
  }));
#endif
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
  window1.register_event_handler(win::wheel_x_event([&] (int delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(delta, 0));
    }
  }));
  window1.register_event_handler(win::wheel_y_event([&] (int delta,
                                                         const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  }));

  //window1.register_event_handler(win::move_event([](const core::point& p) {
  //  LogDebug << "Window1 move: " << p;
  //}));
  //window1.register_event_handler(win::size_event([](const core::size& s) {
  //  LogDebug << "Window1 size: " << s;
  //}));

  auto down_handler = [&] (const core::point& p) {
    at_drag = true;
    last_pos = p;
    LogDebug << "Window Mouse down at " << p;
  };
  auto up_handler = [&] (const core::point& p) {
    at_drag = false;
    LogDebug << "Window Mouse up at " << p;
  };
  window1.register_event_handler(win::left_btn_down_event(down_handler));
  window1.register_event_handler(win::left_btn_up_event(up_handler));
  window2.register_event_handler(win::left_btn_down_event(down_handler));
  window2.register_event_handler(win::left_btn_up_event(up_handler));

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

  //window2.register_event_handler(win::mouse_enter_event([]() {
  //  LogDebug << "Window2 mouse enter";
  //}));
  //window2.register_event_handler(win::mouse_leave_event([]() {
  //  LogDebug << "Window2 mouse leave";
  //}));

#ifdef WIN32
  ok_button.register_event_handler(win::button_state_event([](bool state) {
    LogDebug << "Button " << (state ? "hilited" : "unhilited");
  }));
#endif // WIN32
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

  auto list_drawer = [] (draw::graphics& g,
                         int idx,
                         const core::rectangle& place,
                         bool selected) {
    using namespace draw;

    std::ostringstream strm;
    strm << "Item " << idx;

    g.fill(rectangle(place), selected ? color::highLightColor : color::white);
    g.text(text_box(strm.str(), place, vcenter_left), font::system(),
           selected ? color::highLightTextColor : color::windowTextColor);
  };

  list1.set_drawer(list_drawer, 25);
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
  list2.set_drawer([&] (draw::graphics& g,
                        int idx,
                        const core::rectangle& place,
                        bool selected) {
    data(g, idx, place, selected);
  }, 16);

  list2.register_event_handler(win::selection_changed_event([&] () {
    std::ostringstream strm;
    strm << "List2 item " << list2.get_selection() << ": " << data[list2.get_selection()];
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
    list1.enable_vscroll_bar(on);
    list2.enable_vscroll_bar(on);
  }));

  vscroll.register_event_handler(win::scroll_event([&] (int pos) {
    list1.set_scroll_pos(pos);
    list2.set_scroll_pos(pos);
    list3.set_scroll_pos(pos);
  }));

  calc_button.register_event_handler(win::button_clicked_event([&] () {
    view.calc_area();
  }));

  inc_button.register_event_handler(win::button_clicked_event([&] () {
    view.resize(view.size() + core::size{5, 5});
  }));
  dec_button.register_event_handler(win::button_clicked_event([&] () {
    view.resize(view.size() - core::size{5, 5});
  }));

  vslider.register_event_handler(win::move_event([&](const core::point&) {
    do_layout();
  }));

  hscroll.register_event_handler(win::scroll_event([&](int pos){
    vsplit_view.set_split_pos((double)pos / 100.0);
  }));
  vsplit_view.slider.register_event_handler(win::move_event([&](const core::point&){
    hscroll.set_value(int(vsplit_view.get_split_pos() * hscroll.get_max()));
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

  register_event_handler(win::create_event(gui::core::easy_bind(this, &my_main_window::onCreated)));
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

//  list2.create(main, core::rectangle(410, 50, 60, 250));
//  data.update_list(list2);
//  list2.set_visible();
//
//  list3.create(main, core::rectangle(480, 50, 60, 250));
//  list3.set_data(win::list::data<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), 16);
//  list3.set_visible();

  float floats[] = { 1.1F, 2.2F, 3.3F, 4.4F, 5.5F };

  win::column_list::columns_t columns;
  columns.push_back(win::column_list::column(30, 0.7F));
  columns.push_back(win::column_list::column(30, 0.3F));
  columns.push_back(win::column_list::column(40));

  win::column_list::data<int> col_data;
  col_data.push_back({ 1, 2, 3 });
  col_data.push_back({ 3, 4, 5 });
  col_data.push_back({ 5, 6, 7 });
  col_data.push_back({ 7, 8, 9 });
  col_data.push_back({ 9, 10, 11 });

  vsplit_view.create(main, core::rectangle(410, 50, 160, 250));
  vsplit_view.first.second.set_data(win::list::data<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}), 16);
  vsplit_view.second.first.set_data(win::list::data<float>(floats), 16);
  vsplit_view.second.second.set_columns(columns);
  vsplit_view.second.second.set_data(col_data);
  vsplit_view.set_visible();

  data.update_list(list2);

  column_list.create(main, core::rectangle(580, 50, 100, 250), columns, col_data);
  column_list.set_visible();

  hscroll.create(main, core::rectangle(550, 305, 130, 16));
  hscroll.set_visible();

  vscroll.create(main, core::rectangle(700, 50, 16, 250));
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

  hslider.create(main, core::rectangle(5, 420, 700, 5));
  hslider.set_visible();
  hslider.set_max(500);

  vslider.create(main, core::rectangle(750, 5, 5, 500));
  vslider.set_visible();
  vslider.set_max(750);

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

  chck_group.do_layout();

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

  edit_btn_group.do_layout();

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

  btn_group.do_layout();

  using namespace layout;
  get_layout().abs(&btn_group, this, What::left, Where::width, -600);
  get_layout().abs(&btn_group, this, What::right, Where::width, -10);
  get_layout().abs(&btn_group, this, What::top, Where::height, -45);
  get_layout().abs(&btn_group, this, What::bottom, Where::height, -10);

  get_layout().abs(&hslider, this, What::right, Where::width, -5);
  get_layout().abs(&vslider, this, What::bottom, Where::height, -5);

  get_layout().abs(&vscroll, &vslider, What::left, Where::x, -20);
  get_layout().abs(&vscroll, &vslider, What::right, Where::x, -4);

  do_layout();
}

win::paint_event my_main_window::create_paint1 () {
  return win::paint_event([](draw::graphics& graph) {
    //LogDebug << "win::paint 1";

    using namespace draw;

    graph.frame(polygon(calc_star(10, 10, 40, 40)), color::blue);
    graph.fill(polygon(calc_star(60, 10, 40, 40)), color::darkGreen);
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

    core::rectangle r(10, 155, 50, 18);
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("TL", r, top_left), font::system(), color::red);
    r= {70, 155, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("TC", r, top_hcenter), font::system(), color::red);
    r= {130, 155, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("TR", r, top_right), font::system(), color::red);

    r= {10, 175, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("CL", r, vcenter_left), font::system(), color::red);
    r= {70, 175, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("CC", r, center), font::system(), color::red);
    r= {130, 175, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("CR", r, vcenter_right), font::system(), color::red);

    r= {10, 195, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("BL", r, bottom_left), font::system(), color::red);
    r= {70, 195, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("BC", r, bottom_hcenter), font::system(), color::red);
    r= {130, 195, 50, 18};
    graph.frame(rectangle(r), color::blue);
    graph.text(text_box("BR", r, bottom_right), font::system(), color::red);

    pt = {10, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("TL", pt, top_left), font::system(), color::red);
    pt = {70, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("TC", pt, top_hcenter), font::system(), color::red);
    pt = {130, 215};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("TR", pt, top_right), font::system(), color::red);

    pt = {10, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("CL", pt, vcenter_left), font::system(), color::red);
    pt = {70, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("CC", pt, center), font::system(), color::red);
    pt = {130, 235};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("CR", pt, vcenter_right), font::system(), color::red);

    pt = {10, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("BL", pt, bottom_left), font::system(), color::red);
    pt = {70, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("BC", pt, bottom_hcenter), font::system(), color::red);
    pt = {130, 255};
    graph.fill(arc(pt, 2, 0, 360), color::blue);
    graph.text(text("BR", pt, bottom_right), font::system(), color::red);
  });
}

win::paint_event my_main_window::create_paint2 () {
  return win::paint_event([](draw::graphics& graph) {
    //LogDebug << "win::paint 2";
    using namespace draw;

    pen blue(color::blue);
    color red = color::red;
    color darkGreen = color::darkGreen;
    color yellow = color::cyan;

    graph.draw_pixel(core::point(3, 3), color::gray);
    graph.draw_pixel(core::point(6, 6), color::gray);

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
