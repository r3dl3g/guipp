// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "testlib.h"
#include <gui/core/guidefs.h>
#include <logging/dbgstream.h>
#include <logging/logger.h>
#include <logging/core.h>

#ifdef GUIPP_QT
#include <QtWidgets/qapplication.h>
#endif // GUIPP_QT


namespace testing {

  void init_gui (const testing::start_params& params) {
#ifdef GUIPP_X11
    gui::core::global::init(XOpenDisplay(0));
#endif // GUIPP_X11
#ifdef GUIPP_WIN
    gui::core::global::init(params.hInstance);
#endif // GUIPP_WIN
#ifdef GUIPP_QT
    char* argv[1] = { "" };// = {params.args[0].c_str()};
    int argc = 0;
    static QApplication qapplication(argc, argv);
    gui::core::global::init(&qapplication);
#endif // GUIPP_QT
  }

}

struct test_initializer {
  logging::odebugstream* dbgStrm;

  test_initializer ()
    : dbgStrm(nullptr)
  {
#ifdef GUIPP_WIN
    testing::set_error_log([] (const std::string& s) {
      clog::error().raw() << s;
    });
    testing::set_info_log([] (const std::string& s) {
      clog::info().raw() << s;
    });
    testing::set_test_init([&] () {
      dbgStrm = new logging::odebugstream(logging::level::debug, logging::core::get_console_formatter());
    });
    testing::set_test_fini([&] () {
      delete dbgStrm;
      dbgStrm = nullptr;
      logging::core::instance().finish();
    });
#endif // GUIPP_WIN
  }

};

namespace {
  static test_initializer init;
}
