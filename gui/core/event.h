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
#ifdef GUIPP_WIN
#include <gui/core/win32_event.h>
#endif // Win32


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
#ifdef GUIPP_WIN
    using event = win32::event;

    const gui::os::event_id WM_LAYOUT_WINDOW = WM_USER + 0x100;
#endif //GUIPP_WIN

#ifdef GUIPP_X11
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
#endif // GUIPP_X11

#ifdef COCOA
    using event = CGEventRef;
#endif // COCOA

#ifdef GUIPP_QT

    struct event {
      gui::os::window id;
      QEvent* qevent;

      QEvent::Type type () const {
        return qevent->type();
      }

      template<typename T>
      const T& cast () const {
        return *static_cast<const T*>(qevent);
      }
    };

    const gui::os::event_id WM_LAYOUT_WINDOW = QEvent::User + 0x100;

    namespace qt {

      const gui::os::event_id WM_CREATE_WINDOW = QEvent::User + 0x101;
      const gui::os::event_id WM_DESTROY_WINDOW = QEvent::User + 0x102;

    } // namespace qt

#endif // GUIPP_QT

    typedef bool (event_handler_callback)(const event&, gui::os::event_result&);

  } // core

} // gui

namespace std {
  // --------------------------------------------------------------------------
  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, const gui::core::event& t);
}
