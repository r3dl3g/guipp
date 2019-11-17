/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: event
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#ifdef WIN32
#include <gui/core/win32_event.h>
#endif // Win32


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    using event = IF_WIN32_ELSE(win32::event, IF_X11_ELSE(XEvent, CGEventRef));

    typedef bool (event_handler_callback)(const event&, gui::os::event_result&);

#ifdef X11
    // --------------------------------------------------------------------------
    extern Atom WM_LAYOUT_WINDOW;

    namespace x11 {

      extern Atom WM_CREATE_WINDOW;
      extern Atom WM_DELETE_WINDOW;
      extern Atom WM_PROTOCOLS;
      extern Atom WM_TAKE_FOCUS;

      GUIPP_CORE_EXPORT int init_messages ();

      GUIPP_CORE_EXPORT void init_atom (Atom& message, const char* name);

    } // namespace x11
#endif // X11


  } // core

} // gui

namespace std {
  // --------------------------------------------------------------------------
  GUIPP_CORE_EXPORT std::ostream& operator<< (std::ostream& out, const gui::core::event& t);
}
