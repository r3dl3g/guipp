
#include "control.h"
#include "window_event_handler.h"
#include "window_event_proc.h"
#include "dbg_win_message.h"
#include "gui_types.h"
#include "font.h"

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

win::window_class mainCls;
win::window_class chldCls;

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
  ibr::log::core::instance().addSink(&std::cerr, ibr::log::level::debug, ibr::log::core::instance().getConsoleFormatter());

  gui::core::global::init(XOpenDisplay(0));
#endif

#ifdef WIN32
 mainCls = win::window_class::custom_class("mainwindow",
    CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
    WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME | WS_VISIBLE,
    WS_EX_NOPARENTNOTIFY,
    nullptr,
    LoadCursor(nullptr, IDC_ARROW),
    (HBRUSH)(COLOR_APPWORKSPACE + 1));

 chldCls = win::window_class::custom_class("childwindow",
    CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW,
    WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE,
    WS_EX_NOPARENTNOTIFY | WS_EX_WINDOWEDGE,
    nullptr,
    LoadCursor(nullptr, IDC_ARROW),
    (HBRUSH)(COLOR_WINDOW + 1));

#elif X11
  mainCls = win::window_class::custom_class("mainwindow");
  chldCls = win::window_class::custom_class("childwindow");
#endif


  win::windowT<mainCls> main;
  win::windowT<chldCls> window1;
  win::windowT<chldCls> window2;

  LogDebug << "window size:" << sizeof(main);
  LogDebug << "window_class size:" << sizeof(mainCls);

  main.register_event_handler(init_result_handler());

  win::push_button button;
  win::radio_button radio_button, radio_button2;
  win::check_box check_box;
  win::label label;

#ifdef WIN32
  main.register_event_handler(win::get_minmax_event([](const core::size& sz,
    const core::point& pos,
    core::size& mi, core::size& ma) {
    mi = { 300, 200 };
    ma = { 800, 600 };
    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
  }));
#endif

  main.register_event_handler(win::create_event([](win::window*,
    const core::rectangle& r) {
    LogDebug << "Create Main: " << r;
  }));
  main.register_event_handler(win::destroy_event([&]() {
    LogDebug << "Destroyed!";
    main.quit();
  }));

  win::paint_event paint1([](draw::graphics& graph) {
    LogDebug << "win::paint 1";

    using namespace draw;

    graph.frame(polygone(calc_star(10, 10, 40, 40)), color::blue);
    graph.fill(polygone(calc_star(60, 10, 40, 40)), color::darkGreen);
    graph.draw(polygone(calc_star(110, 10, 40, 40)), color::yellow, color::red);

    graph.text(text("Hello World 1!", core::point(10, 60)), font::system(), color::black);
    graph.text(text("Hello World 2!", core::point(10, 75)), font::system_bold(), color::black);
    graph.text(text("Hello World 3!", core::point(10, 90)), font::sans_serif(), color::black);
    graph.text(text("Hello World 4!", core::point(10, 105)), font::serif(), color::black);
    graph.text(text("Hello World 5!", core::point(10, 120)), font::monospace(), color::black);
    graph.text(text("Hello World 6!", core::point(10, 135)), font::sans_serif().with_size(18), color::blue);

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

    core::point pt(10, 215);
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
    graph.text(text_box("CL", pt, vcenter_left), font::system(), color::red);
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

  win::paint_event paint2([](draw::graphics& graph) {
    LogDebug << "win::paint 2";
    using namespace draw;

    pen blue(color::blue);
    pen red(color::red);

    graph.drawPixel(core::point(3, 3), color::gray);
    graph.drawPixel(core::point(6, 6), color::gray);

    core::size sz(30, 50);
    core::size offs1(0, 60);
    core::size offs2(0, 120);

    core::point pos1(10, 10);
    graph.frame(rectangle(pos1, sz), blue);
    graph.fill(rectangle(pos1 + offs1, sz), color::darkGreen);
    graph.draw(rectangle(pos1 + offs2, sz), color::yellow, red);

    core::point pos2(50, 10);
    graph.frame(ellipse(pos2, sz), blue);
    graph.fill(ellipse(pos2 + offs1, sz), color::darkGreen);
    graph.draw(ellipse(pos2 + offs2, sz), color::yellow, red);

    core::point pos3(90, 10);
    core::size rd(10, 10);
    graph.frame(round_rectangle(core::rectangle(pos3, sz), rd), blue);
    graph.fill(round_rectangle(core::rectangle(pos3 + offs1, sz), rd), color::darkGreen);
    graph.draw(round_rectangle(core::rectangle(pos3 + offs2, sz), rd), color::yellow, red);

    core::point pos4(150, 30);
    graph.frame(arc(pos4, 20, 0, 360), blue);
    graph.fill(arc(pos4 + offs1, 20, 0, 360), color::darkGreen);
    graph.draw(arc(pos4 + offs2, 20, 0, 360), color::yellow, red);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

  });

  bool at_drag = false;
  core::point last_pos;

#ifdef WIN32
  main.register_event_handler(win::close_event([&]() {
    LogDebug << "Close!";
    main.destroy();
  }));
  main.register_event_handler(win::enable_event([](bool on) {
    LogDebug << (on ? "Enableed" : "Disabled");
  }));
  main.register_event_handler(win::activate_event([](bool on, win::window* win) {
    LogDebug << "Main " << (on ? "activate" : "deactivate");
  }));
  main.register_event_handler(win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
  main.register_event_handler(win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
  main.register_event_handler(win::activate_app_event([](bool on) {
    LogDebug << (on ? "A" : "Dea") << "ctivate App";
  }));

#endif

  main.register_event_handler(win::moving_event([](const core::point& r) {
    LogDebug << "Main moving: " << r;
  }));
  main.register_event_handler(win::sizing_event([](const core::size& r) {
    LogDebug << "Main sizing: " << r;
  }));
  main.register_event_handler(win::placing_event([](const core::rectangle& r) {
    LogDebug << "Main placing: " << r;
  }));

  main.register_event_handler(win::move_event([](const core::point& p) {
    LogDebug << "Main move: " << p;
  }));
  main.register_event_handler(win::size_event([](const core::size& s) {
    LogDebug << "Main size: " << s;
  }));
  main.register_event_handler(win::place_event([](const core::rectangle& r) {
    LogDebug << "Main place: " << r;
  }));

#ifdef WIN32
  button.register_event_handler(win::activate_event([](bool on, win::window* win) {
    LogDebug << "Button " << (on ? "" : "de") << "activate";
  }));
#endif
  button.register_event_handler(win::set_focus_event([](win::window* win) {
    LogDebug << "Button Set Focus";
  }));
  button.register_event_handler(win::lost_focus_event([](win::window* win) {
    LogDebug << "Button Lost Focus";
  }));

  main.register_event_handler(win::left_btn_down_event([&](const core::point& p){
    LogDebug << "Left Button Down at " << p;
  }));
  main.register_event_handler(win::left_btn_up_event([&](const core::point& p){
    LogDebug << "Left Button Up at " << p;
  }));
  main.register_event_handler(win::right_btn_down_event([&](const core::point& p){
    LogDebug << "Right Button Down at " << p;
  }));
  main.register_event_handler(win::right_btn_up_event([&](const core::point& p){
    LogDebug << "Right Button Up at " << p;
  }));
  window1.register_event_handler(win::wheel_x_event([&](int delta, const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(delta, 0));
    }
  }));
  window1.register_event_handler(win::wheel_y_event([&](int delta, const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p;
    if (window1.place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  }));

  window1.register_event_handler(win::move_event([](const core::point& p) {
    LogDebug << "Window1 move: " << p;
  }));
  window1.register_event_handler(win::size_event([](const core::size& s) {
    LogDebug << "Window1 size: " << s;
  }));
  window1.register_event_handler(win::mouse_move_event([&](unsigned int keys, const core::point& p) {
    //LogDebug << "Window1 Mouse " << (at_drag ? "drag" : "move") << " : " << keys << " at " << p;
    if (at_drag) {
      core::size delta = p - last_pos;
      //last_pos = p;
      window1.move(window1.position() + delta);
    }
  }));
  window1.register_event_handler(win::left_btn_down_event([&](const core::point& p) {
    at_drag = true;
    last_pos = p;
    LogDebug << "Window1 Mouse down at " << p;
  }));
  window1.register_event_handler(win::left_btn_up_event([&](const core::point& p) {
    at_drag = false;
    LogDebug << "Window1 Mouse up at " << p;
  }));
  window1.register_event_handler(win::left_btn_dblclk_event([&](const core::point& p) {
    LogDebug << "Window1 Double Click up at " << p;
    if (window2.is_visible()) {
      window2.hide();
    } else {
      window2.show();
    }
  }));

  main.register_event_handler(win::left_btn_dblclk_event([&](const core::point& p) {
    LogDebug << "Double Click up at " << p;

    core::point pos = window1.position();
    core::size sz = window1.size();
    LogDebug << "Pos: " << pos << " Size " << sz;

    core::rectangle  pl = window1.place();
    LogDebug << "Place: " << pl;

    core::rectangle  apl = window1.absolute_place();
    LogDebug << "Abs Place: " << apl;

    core::point apos = window1.absolute_position();
    LogDebug << "Abs Pos: " << apos;

    core::rectangle  car = window1.client_area();
    LogDebug << "Client: " << car;
  }));
  main.register_event_handler(win::right_btn_dblclk_event([&](const core::point& p) {
    window1.move({ 50, 50 });
  }));

  window2.register_event_handler(paint1);

  bool p1 = true;

  window2.register_event_handler(win::left_btn_dblclk_event([&](const core::point& p) {
    LogDebug << "Window2 Double Click up at " << p;
    if (p1) {
      p1 = false;
      window2.unregister_event_handler(paint1);
      window2.register_event_handler(paint2);
    } else {
      p1 = true;
      window2.unregister_event_handler(paint2);
      window2.register_event_handler(paint1);
    }
    window2.redraw_later();
  }));
  window2.register_event_handler(win::show_event([]() {
    LogDebug << "Window2 show:";
  }));
  window2.register_event_handler(win::hide_event([]() {
    LogDebug << "Window2 hide:";
  }));
  window2.register_event_handler(win::mouse_enter_event([]() {
    LogDebug << "Window2 mouse enter";
  }));
  window2.register_event_handler(win::mouse_leave_event([]() {
    LogDebug << "Window2 mouse leave";
  }));

  main.register_event_handler(log_all_events());

#ifdef WIN32
  button.register_event_handler(win::button_state_event([](bool state) {
    LogDebug << "Button " << (state ? "hilited" : "unhilited");
  }));
#endif // WIN32
  button.register_event_handler(win::button_pushed_event([&]() {
    LogDebug << "Button pushed";
    label.set_text("Pushed!");
    label.redraw_now();
  }));
  button.register_event_handler(win::button_released_event([&]() {
    LogDebug << "Button released";
    label.set_text("Released!");
    label.redraw_now();
  }));
  button.register_event_handler(win::button_clicked_event([&]() {
    LogDebug << "Button clicked";
    label.set_text("OK Clicked!");
  }));
  radio_button.register_event_handler(win::button_clicked_event([&]() {
    LogDebug << "Radio clicked";
    label.set_text("Radio clicked!");
    bool check = radio_button.is_checked();
//    radio_button.set_checked(!check);
    radio_button2.set_checked(!check);
  }));
  radio_button2.register_event_handler(win::button_clicked_event([&]() {
    LogDebug << "Radio2 clicked";
    label.set_text("Radio2 clicked!");
    bool check = radio_button2.is_checked();
//    radio_button2.set_checked(!check);
    radio_button.set_checked(!check);
  }));
  check_box.register_event_handler(win::button_state_event([&](bool on) {
    LogDebug << "Check clicked";
    label.set_text("Check clicked!");
    radio_button.enable(on);
    radio_button2.enable(on);
    button.enable(on);
  }));

  main.register_event_handler(win::create_event([&](win::window* w, const core::rectangle& rect) {
    LogDebug << "Main created";
  }));
  main.create(core::rectangle(50, 50, 640, 480));
    //main.set_text("Window Test");
    window1.create(main, core::rectangle(50, 50, 200, 280));
    window2.create(main, core::rectangle(300, 50, 200, 280));
    window1.show();
    window2.show();

    label.create(main, core::rectangle(50, 350, 100, 25), "Text");
    label.show();
    label.redraw_later();

    radio_button.create(main, core::rectangle(160, 350, 100, 25), "Radio");
    radio_button.show();
    radio_button.redraw_later();

    radio_button2.create(main, core::rectangle(160, 370, 100, 25), "Radio2");
    radio_button2.show();
    radio_button2.redraw_later();

    check_box.create(main, core::rectangle(270, 350, 100, 25), "Check");
    check_box.show();
    check_box.redraw_later();

    button.create(main, core::rectangle(400, 350, 100, 25), "Ok");
    button.show();
    button.redraw_later();

    main.show();
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

