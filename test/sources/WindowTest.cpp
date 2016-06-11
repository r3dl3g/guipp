
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

class log_all_events : public gui::event_handler {
public:
  log_all_events()
  {}

  virtual bool handle_event(const gui::window_event& e, gui::event_result& result) {
    if ((result == 0xdeadbeef) && !gui::is_none_client_event(e.msg) && !gui::is_frequent_event(e.msg)) {
      LogDebug << "Message: " << gui::EventId(e.msg) << " (" << std::hex << e.param_1 << ", " << e.param_2 << ")";
    }
    return false;
  }
};

class init_result_handler : public gui::event_handler {
public:
  init_result_handler() {}

  virtual bool handle_event(const gui::window_event& e, gui::event_result& result) {
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

  gui::window_class mainCls("mainwindow",
                         CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                         WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                         WS_EX_NOPARENTNOTIFY,
                         hInstance,
                         NULL,
                         LoadCursor(NULL, IDC_ARROW),
                         (HBRUSH)(COLOR_APPWORKSPACE + 1));

  gui::window_class chldCls("childwindow",
                            CS_DBLCLKS,// | CS_VREDRAW | CS_HREDRAW,
                            WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SIZEBOX | WS_VISIBLE,
                            WS_EX_NOPARENTNOTIFY | WS_EX_CLIENTEDGE,
                            hInstance,
                            NULL,
                            LoadCursor(NULL, IDC_ARROW),
                            (HBRUSH)(COLOR_WINDOW + 1));

  gui::window main;
  gui::window window1;
  gui::window window2;
  
  main.register_event_handler(new init_result_handler());


  main.register_event_handler(new gui::pos_changing_event(
    [](unsigned int& flags, gui::rectangle & r) {
    LogDebug << "Main changing: " << flags << ", " << r;
  }));
  main.register_event_handler(new gui::pos_changed_event(
    [](unsigned int flags, gui::rectangle const& r) {
    LogDebug << "Main changed: " << flags << ", " << r;
  }));

  main.register_event_handler(new gui::get_minmax_event(
    [](const gui::size& sz, const gui::position& pos, gui::size& mi, gui::size& ma) {
    mi = { 300, 200 };
    ma = { 800, 600 };
    LogDebug << "Min/Max: " << mi << " < " << ma << " < " << sz;
  }));

  main.register_event_handler(new gui::create_event([](gui::window*, gui::rectangle const& r) { 
    LogDebug << "Create Main: " << r;
  }));
  main.register_event_handler(new gui::destroy_event([&]() { 
    LogDebug << "Destroyed!";
    main.quit();
  }));
  //main.register_event_handler(new gui::close_event([&]()->gui::event_result { 
  //  LogDebug << "Close!";
  //  main.destroy();
  //  return 0;
  //}));
  main.register_event_handler(new gui::enable_event([](bool on) { LogDebug << (on ? "Enableed" : "Disabled"); }));
  main.register_event_handler(new gui::activate_event([](bool on, gui::window* win) {
    LogDebug << "Main " << (on ? "activate" : "deactivate");
  }));
  main.register_event_handler(new gui::set_focus_event([](gui::window* win) { LogDebug << "Set Focus"; }));
  main.register_event_handler(new gui::lost_focus_event([](gui::window* win) { LogDebug << "Lost Focus"; }));
  main.register_event_handler(new gui::begin_size_or_move_event([]() { LogDebug << "Start Move/Size"; }));
  main.register_event_handler(new gui::end_size_or_move_event([]() { LogDebug << "Finish Move/Size"; }));
  main.register_event_handler(new gui::move_event([](gui::position const& p) { LogDebug << "Main move: " << p; }));
  main.register_event_handler(new gui::moving_event([](gui::rectangle const& r) { LogDebug << "Main moving: " << r; }));
  main.register_event_handler(new gui::sizing_event([](unsigned int flags, gui::rectangle const& r) {
    LogDebug << "Main sizing: " << flags << ", " << r;
  }));
  main.register_event_handler(new gui::size_event([](unsigned int flags, gui::size const& s) {
    LogDebug << "Main size: " << flags << ", " << s;
  }));
  main.register_event_handler(new gui::activate_app_event([](bool on) {
    LogDebug << (on ? "A" : "Dea") << "ctivate App";
  }));

  main.register_event_handler(new gui::show_event([](bool show, unsigned int flags) {
    LogDebug << "Main " << (show ? "show" : "hide") << " reason: " << flags;
  }));
  


  main.register_event_handler(new log_all_events());

  main.create(mainCls, gui::rectangle(50, 50, 640, 480));
  window1.create(chldCls, main, gui::rectangle(50, 50, 200, 380));
  window2.create(chldCls, main, gui::rectangle(300, 50, 200, 380));
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

