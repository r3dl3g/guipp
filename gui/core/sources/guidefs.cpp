/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: basic window types
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <stdexcept>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace core {

    namespace global {

      bool is_global_initialized = false;
      instance_id global_instance = 0;

#ifdef X11
      screen_id global_screen = 0;
#endif // X11

      void init(core::instance_id instance) {
        global_instance = instance;
#ifdef X11
        global_screen = DefaultScreen(global_instance);
#endif // X11
        is_global_initialized = true;
      }

      core::instance_id get_instance() {
        if (!is_global_initialized) {
          throw std::runtime_error("gui::core::global::init must be called before first use!");
          return 0;
        }
        return global_instance;
      }

#ifdef X11
      screen_id get_screen() {
        return global_screen;
      }

      void set_screen(screen_id screen) {
        global_screen = screen;
      }

#endif // X11

    }
  } // core

} // gui