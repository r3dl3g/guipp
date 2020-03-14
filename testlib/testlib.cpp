// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "testlib.h"
#include <gui/core/guidefs.h>


namespace testing {

  void init_gui (const testing::start_params& params) {
#ifdef X11
    gui::core::global::init(XOpenDisplay(0));
#endif // X11
#ifdef WIN32
    gui::core::global::init(params.hInstance);
#endif // WIN32
  }

}
