
#include "window.h"
#include "window_event_handler.h"
#include "dbg_win_message.h"
#include "gui_types.h"
#include "font.h"

struct Bool {
  inline Bool(bool b)
    : b(b) {
  }

  bool b;
};

std::ostream& operator<<(std::ostream& out, const Bool& b) {
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

  bool operator()(const win::window_event& e, core::event_result& result) {
    if ((result == 0xdeadbeef) && !win::is_none_client_event(e.msg) && !win::is_frequent_event(e.msg)) {
      LogDebug << "Message: " << win::EventId(e.msg) << " (" << std::hex << e.param_1 << ", " << e.param_2 << ")";
    }
    return false;
  }
};

class init_result_handler {
public:
  init_result_handler() {
  }

  bool operator()(const win::window_event& e, core::event_result& result) {
    result = 0xdeadbeef;
    return false;
  }
};

win::window_class mainCls("mainwindow",
                          CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                          WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                          WS_EX_NOPARENTNOTIFY,
                          NULL,
                          LoadCursor(NULL, IDC_ARROW),
                          (HBRUSH)(COLOR_APPWORKSPACE + 1));

win::window_class chldCls("childwindow",
                          CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                          WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE,
                          WS_EX_NOPARENTNOTIFY | WS_EX_WINDOWEDGE,
                          NULL,
                          LoadCursor(NULL, IDC_ARROW),
                          (HBRUSH)(COLOR_WINDOW + 1));

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  ibr::odebugstream dbgStrm;
  ibr::log::core::instance().addSink(&dbgStrm, ibr::log::level::debug, ibr::log::core::instance().getConsoleFormatter());

  win::window_class::init(hInstance);

  win::windowT<mainCls> main;
  win::windowT<chldCls> window1;
  win::windowT<chldCls> window2;

  LogDebug << "window size:" << sizeof(main);
  LogDebug << "window_class size:" << sizeof(mainCls);
  LogDebug << "event_handler_fnct size:" << sizeof(win::window::event_handler_fnct);
  LogDebug << "event_handler_ptr size:" << sizeof(win::window::event_handler_ptr);
  LogDebug << "event_handler_list size:" << sizeof(std::vector<win::window::event_handler_ptr>);

  main.register_event_handler(init_result_handler());

  main.register_event_handler(win::pos_changing_event([](unsigned int& flags, core::rectangle & r) {
    LogDebug << "Main changing: " << flags << ", " << r;
  }));
  main.register_event_handler(win::pos_changed_event([](unsigned int flags, core::rectangle  const& r) {
    LogDebug << "Main changed: " << flags << ", " << r;
  }));

  main.register_event_handler(win::get_minmax_event([](const core::size& sz,
    const core::point& pos,
    core::size& mi, core::size& ma) {
    mi = { 300, 200 };
    ma = { 800, 600 };
    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
  }));

  main.register_event_handler(win::create_event([](win::window*,
    const core::rectangle& r) {
    LogDebug << "Create Main: " << r;
  }));
  main.register_event_handler(win::destroy_event([&]() {
    LogDebug << "Destroyed!";
    main.quit();
  }));
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
  main.register_event_handler(win::set_focus_event([](win::window* win) { LogDebug << "Set Focus"; }));
  main.register_event_handler(win::lost_focus_event([](win::window* win) { LogDebug << "Lost Focus"; }));
  main.register_event_handler(win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
  main.register_event_handler(win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
  main.register_event_handler(win::move_event([](core::point const& p) { LogDebug << "Main move: " << p; }));
  main.register_event_handler(win::moving_event([](core::rectangle  const& r) { LogDebug << "Main moving: " << r; }));
  main.register_event_handler(win::sizing_event([](unsigned int flags, core::rectangle  const& r) {
    LogDebug << "Main sizing: " << flags << ", " << r;
  }));
  main.register_event_handler(win::size_event([](unsigned int flags, core::size const& s) {
    LogDebug << "Main size: " << flags << ", " << s;
  }));
  main.register_event_handler(win::activate_app_event([](bool on) {
    LogDebug << (on ? "A" : "Dea") << "ctivate App";
  }));

  main.register_event_handler(win::show_event([](bool show, unsigned int flags) {
    LogDebug << "Main " << (show ? "show" : "hide") << " reason: " << flags;
  }));

  main.register_event_handler(win::wheel_x_event([&](unsigned int keys, int delta, const core::point& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p << " key " << keys;
    if (window1.absolute_place().is_inside(p)) {
      window1.move(window1.position() - core::size(delta, 0));
    }
  }));
  main.register_event_handler(win::wheel_y_event([&](unsigned int keys, int delta, const core::point& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p << " key " << keys;
    if (window1.absolute_place().is_inside(p)) {
      window1.move(window1.position() + core::size(0, delta));
    }
  }));
  main.register_event_handler(win::mouse_move_event([](unsigned int keys, const core::point& p) {
    LogDebug << "Mouse move : " << keys << " at " << p;
  }));
  main.register_event_handler(win::mouse_hover_event([](unsigned int keys, const core::point& p) {
    LogDebug << "Mouse hover : " << keys << " at " << p;
  }));
  main.register_event_handler(win::left_btn_dblclk_event([&](unsigned int keys, const core::point& p) {
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
  main.register_event_handler(win::right_btn_dblclk_event([&](unsigned int keys, const core::point& p) {
    window1.move({ 50, 50 });
  }));


  main.register_event_handler(log_all_events());

  win::window::event_handler_ptr painter1 = window2.register_event_handler(win::paint_event([](draw::graphics& graph) {
    using namespace draw;

    pen blue(color::blue);
    pen red(color::red);

    graph.drawPixel(core::point(3, 3), color::gray);
    graph.drawPixel(core::point(6, 6), color::gray);

    core::size sz(30, 50);
    core::size offs1(0, 60);
    core::size offs2(0, 120);

    core::point pos1(10, 10);
    graph.frame(rectangle(core::rectangle(pos1, sz)), blue);
    graph.fill(rectangle(core::rectangle(pos1 + offs1, sz)), color::darkGreen);
    graph.draw(rectangle(core::rectangle(pos1 + offs2, sz)), color::yellow, red);

    core::point pos2(50, 10);
    graph.frame(ellipse(core::rectangle(pos2, sz)), blue);
    graph.fill(ellipse(core::rectangle(pos2 + offs1, sz)), color::darkGreen);
    graph.draw(ellipse(core::rectangle(pos2 + offs2, sz)), color::yellow, red);

    core::point pos3(90, 10);
    core::size rd(10, 10);
    graph.frame(round_rectangle(core::rectangle(pos3, sz), rd), blue);
    graph.fill(round_rectangle(core::rectangle(pos3 + offs1, sz), rd), color::darkGreen);
    graph.draw(round_rectangle(core::rectangle(pos3 + offs2, sz), rd), color::yellow, red);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

  }));


  win::window::event_handler_ptr painter2 = window2.register_event_handler(win::paint_event([](draw::graphics& graph) {
    using namespace draw;

    graph.draw(text("Hello World!", core::point(10, 190)), font::system, color::green);
    graph.draw(text("Hello World!", core::point(10, 205)), font::system_bold, color::green);
    graph.draw(text("Hello World!", core::point(10, 220)), font::sans_serif, color::green);
    graph.draw(text("Hello World!", core::point(10, 235)), font::serif, color::green);
    graph.draw(text("Hello World!", core::point(10, 250)), font::monospace, color::green);
    graph.draw(text("Hello World!", core::point(10, 265)), font("Modern", font::system.size()), color::blue);
    graph.draw(text("Hello World!", core::point(10, 280)), font("Modern", font::system.size(), font::regular, 0, true), color::blue);
    graph.draw(text_box("Hello World!", core::rectangle(10, 295, 180, 20), center), font::serif, color::red);
  }));
  window2.unregister_event_handler(painter2);

  bool p1 = true;

  window2.register_event_handler(win::left_btn_dblclk_event([&](unsigned int keys, const core::point& p) {
    if (p1) {
      p1 = false;
      window2.unregister_event_handler(painter1);
      painter2 = window2.register_event_handler(*painter2.get());
    } else {
      p1 = true;
      window2.unregister_event_handler(painter2);
      painter1 = window2.register_event_handler(*painter1.get());
    }
    window2.redraw_later();
  }));

  main.create(core::rectangle(50, 50, 640, 480));
  window1.create(main, core::rectangle(50, 50, 200, 380));
  window2.create(main, core::rectangle(300, 50, 200, 380));
  main.setText("Window Test");
  main.show();

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  ibr::log::core::instance().finish();

  return (int)msg.wParam;
}

