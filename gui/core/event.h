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
#ifdef WIN32
    using event = win32::event;

    const gui::os::event_id WM_LAYOUT_WINDOW = WM_USER + 0x100;
#endif //WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    using event = XEvent;

    extern Atom WM_LAYOUT_WINDOW;

    namespace x11 {

      extern Atom WM_CREATE_WINDOW;
      extern Atom WM_DELETE_WINDOW;
      extern Atom WM_PROTOCOLS;
      extern Atom WM_TAKE_FOCUS;
      extern Atom GUI_LIB_WIN_PTR;

      GUIPP_CORE_EXPORT int init_messages ();

      GUIPP_CORE_EXPORT void init_atom (Atom& message, const char* name);

    } // namespace x11
#endif // X11

#ifdef COCOA
    using event = CGEventRef;
#endif // COCOA

#ifdef QT_WIDGETS_LIB
    using event = QEvent;

    const gui::os::event_id WM_LAYOUT_WINDOW = QEvent::User + 0x100;

#endif // QT_WIDGETS_LIB

    typedef bool (event_handler_callback)(const event&, gui::os::event_result&);

  } // core

} // gui

namespace std {
  // --------------------------------------------------------------------------
  GUIPP_CORE_EXPORT std::ostream& operator<< (std::ostream& out, const gui::core::event& t);
}
