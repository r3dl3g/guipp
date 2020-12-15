// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "testlib.h"
#include <gui/core/guidefs.h>
#include <logging/logger.h>

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
    char* argv[0];// = {params.args[0].c_str()};
    int argc = 0;
    static QApplication qapplication(argc, argv);
    gui::core::global::init(&qapplication);
#endif // GUIPP_QT
    testing::set_error_log([] (const std::string& s) {
      clog::error().raw() << s;
    });
    testing::set_info_log([] (const std::string& s) {
      clog::info().raw() << s;
    });
  }

}
