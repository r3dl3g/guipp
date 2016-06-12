
#include "window.h"
#include "window_event_handler.h"
#include "dbg_win_message.h"

struct Bool {
  inline Bool(bool b)
    : b(b)
  {}

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

class log_all_events : public gui::win::event_handler {
public:
  log_all_events()
  {}

  virtual bool handle_event(const gui::win::window_event& e, gui::core::event_result& result) {
    if ((result == 0xdeadbeef) && !gui::win::is_none_client_event(e.msg) && !gui::win::is_frequent_event(e.msg)) {
      LogDebug << "Message: " << gui::win::EventId(e.msg) << " (" << std::hex << e.param_1 << ", " << e.param_2 << ")";
    }
    return false;
  }
};

class init_result_handler : public gui::win::event_handler {
public:
  init_result_handler() {}

  virtual bool handle_event(const gui::win::window_event& e, gui::core::event_result& result) {
    result = 0xdeadbeef;
    return false;
  }
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  ibr::odebugstream dbgStrm;
  ibr::log::core::instance().addSink(&dbgStrm, ibr::log::level::debug, ibr::log::core::instance().getConsoleFormatter());

  gui::win::window_class mainCls("mainwindow",
                                 CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                                 WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                                 WS_EX_NOPARENTNOTIFY,
                                 hInstance,
                                 NULL,
                                 LoadCursor(NULL, IDC_ARROW),
                                 (HBRUSH)(COLOR_APPWORKSPACE + 1));

  gui::win::window_class chldCls("childwindow",
                                 CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                                 WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE,
                                 WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
                                 hInstance,
                                 NULL,
                                 LoadCursor(NULL, IDC_ARROW),
                                 (HBRUSH)(COLOR_WINDOW + 1));

  gui::win::window main;
  gui::win::window window1;
  gui::win::window window2;
  
  main.register_event_handler(new init_result_handler());


  main.register_event_handler(new gui::win::pos_changing_event(
    [](unsigned int& flags, gui::core::rectangle & r) {
    LogDebug << "Main changing: " << flags << ", " << r;
  }));
  main.register_event_handler(new gui::win::pos_changed_event(
    [](unsigned int flags, gui::core::rectangle  const& r) {
    LogDebug << "Main changed: " << flags << ", " << r;
  }));

  main.register_event_handler(new gui::win::get_minmax_event(
    [](const gui::core::size& sz, const gui::core::position& pos, gui::core::size& mi, gui::core::size& ma) {
    mi = { 300, 200 };
    ma = { 800, 600 };
    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
  }));

  main.register_event_handler(new gui::win::create_event([](gui::win::window*, gui::core::rectangle  const& r) {
    LogDebug << "Create Main: " << r;
  }));
  main.register_event_handler(new gui::win::destroy_event([&]() {
    LogDebug << "Destroyed!";
    main.quit();
  }));
  //main.register_event_handler(new gui::win::close_event([&]()->gui::event_result { 
  //  LogDebug << "Close!";
  //  main.destroy();
  //  return 0;
  //}));
  main.register_event_handler(new gui::win::enable_event([](bool on) { LogDebug << (on ? "Enableed" : "Disabled"); }));
  main.register_event_handler(new gui::win::activate_event([](bool on, gui::win::window* win) {
    LogDebug << "Main " << (on ? "activate" : "deactivate");
  }));
  main.register_event_handler(new gui::win::set_focus_event([](gui::win::window* win) { LogDebug << "Set Focus"; }));
  main.register_event_handler(new gui::win::lost_focus_event([](gui::win::window* win) { LogDebug << "Lost Focus"; }));
  main.register_event_handler(new gui::win::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
  main.register_event_handler(new gui::win::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
  main.register_event_handler(new gui::win::move_event([](gui::core::position const& p) { LogDebug << "Main move: " << p; }));
  main.register_event_handler(new gui::win::moving_event([](gui::core::rectangle  const& r) { LogDebug << "Main moving: " << r; }));
  main.register_event_handler(new gui::win::sizing_event([](unsigned int flags, gui::core::rectangle  const& r) {
    LogDebug << "Main sizing: " << flags << ", " << r;
  }));
  main.register_event_handler(new gui::win::size_event([](unsigned int flags, gui::core::size const& s) {
    LogDebug << "Main size: " << flags << ", " << s;
  }));
  main.register_event_handler(new gui::win::activate_app_event([](bool on) {
    LogDebug << (on ? "A" : "Dea") << "ctivate App";
  }));

  main.register_event_handler(new gui::win::show_event([](bool show, unsigned int flags) {
    LogDebug << "Main " << (show ? "show" : "hide") << " reason: " << flags;
  }));
  
  main.register_event_handler(new gui::win::wheel_x_event([&](unsigned int keys, int delta, const gui::core::position& p) {
    LogDebug << "Wheel-X: " << delta << " at " << p << " key " << keys;
    if (window1.absolute_place().is_inside(p)) {
      window1.move(window1.position() - gui::core::size(delta, 0));
    }
  }));
  main.register_event_handler(new gui::win::wheel_y_event([&](unsigned int keys, int delta, const gui::core::position& p) {
    LogDebug << "Wheel-Y: " << delta << " at " << p << " key " << keys;
    if (window1.absolute_place().is_inside(p)) {
      window1.move(window1.position() + gui::core::size(0, delta));
    }
  }));
  main.register_event_handler(new gui::win::mouse_move_event([](unsigned int keys, const gui::core::position& p) {
    LogDebug << "Mouse move : " << keys << " at " << p;
  }));
  main.register_event_handler(new gui::win::mouse_hover_event([](unsigned int keys, const gui::core::position& p) {
    LogDebug << "Mouse hover : " << keys << " at " << p;
  }));
  main.register_event_handler(new gui::win::left_btn_dblclk_event([&](unsigned int keys, const gui::core::position& p) {
    gui::core::position pos = window1.position();
    gui::core::size sz = window1.size();
    LogDebug << "Pos: " << pos << " Size " << sz;

    gui::core::rectangle  pl = window1.place();
    LogDebug << "Place: " << pl;

    gui::core::rectangle  apl = window1.absolute_place();
    LogDebug << "Abs Place: " << apl;

    gui::core::position apos = window1.absolute_position();
    LogDebug << "Abs Pos: " << apos;

    gui::core::rectangle  car = window1.client_area();
    LogDebug << "Client: " << car;
  }));
  main.register_event_handler(new gui::win::right_btn_dblclk_event([&](unsigned int keys, const gui::core::position& p) {
    window1.move({50, 50});
  }));


  main.register_event_handler(new log_all_events());

  main.create(mainCls, gui::core::rectangle (50, 50, 640, 480));
  window1.create(chldCls, main, gui::core::rectangle (50, 50, 200, 380));
  window2.create(chldCls, main, gui::core::rectangle (300, 50, 200, 380));
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

