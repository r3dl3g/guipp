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
* @brief     C++ API: window class definition
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window_class.h"
#include "window.h"
#include "window_event_proc.h"


namespace gui {

  namespace win {

    class window;

    std::string getLastErrorText () {
#ifdef WIN32
      LPTSTR lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, nullptr);
      return lpMsgBuf;
#endif // Win32
#ifdef X11
      return std::string();
#endif // X11
    }

    window_class::window_class ()
      : class_style(0)
      , background(0)
      , cursor(0)
      , cursor_type(0)
      , style(0)
      , ex_style(0)
      , callback(0)
      , is_initialized(false)
    {}

    window_class::window_class (const window_class& rhs)
      : class_name(rhs.class_name)
      , background(rhs.background)
      , cursor(rhs.cursor)
      , cursor_type(rhs.cursor_type)
      , class_style(rhs.class_style)
      , style(rhs.style)
      , ex_style(rhs.ex_style)
      , callback(rhs.callback)
      , is_initialized(rhs.is_initialized)
    {}

    window_class::window_class (const std::string& cls_name,
                                os::color background,
                                os::cursor_type cursor_t,
                                os::style class_style,
                                os::style style,
                                os::style ex_style,
                                os::event_callback callback)
      : class_name(cls_name)
      , background(background)
      , cursor(0)
      , cursor_type(cursor_t)
      , class_style(class_style)
      , style(style)
      , ex_style(ex_style)
      , callback(callback)
      , is_initialized(false)
    {}

    void window_class::prepare (window* win) const {
#ifdef WIN32
      if (callback) {
        SetWindowLongPtr(win->get_id(), GWLP_WNDPROC, (LONG_PTR)detail::WindowEventProc);
      }
#endif // WIN32
#ifdef X11
      if (get_cursor()) {
        XSetWindowAttributes wa;
        wa.cursor = get_cursor();
        XChangeWindowAttributes(core::global::get_instance(), win->get_id(), CWCursor, &wa);
      }
#endif // X11
    }

    const std::string& window_class::get_class_name () const {
      register_class();
      return class_name;
    }

    const os::color window_class::get_background () const {
      register_class();
      return background;
    }

    const os::cursor window_class::get_cursor () const {
      register_class();
      return cursor;
    }

    const os::style window_class::get_class_style () const {
      register_class();
      return class_style;
    }

    const os::style window_class::get_style () const {
      register_class();
      return style;
    }

    const os::style window_class::get_ex_style () const {
      register_class();
      return ex_style;
    }

    const os::event_callback window_class::get_callback () const {
      register_class();
      return callback;
    }

    bool window_class::is_valid () const {
      return !class_name.empty();
    }

    os::brush window_class::get_background_brush () const {
#ifdef WIN32
      return ((background > 0) && (background < 20)) ? reinterpret_cast<os::brush>(static_cast<LPARAM>(background))
                                                     : CreateSolidBrush(background);
#endif // WIN32
#ifdef X11
      return background;
#endif // X11
    }

    void window_class::register_class () const {
      if (is_initialized || callback) {
        return;
      }
#ifdef WIN32
      if (cursor_type && !cursor) {
        cursor = LoadCursor(nullptr, cursor_type);
      }

      WNDCLASS wc = {
        /* Register the window class. */
        CS_DBLCLKS,
        detail::WindowEventProc,
        0,
        sizeof(window_class*),
        core::global::get_instance(),
        nullptr,
        cursor,
        get_background_brush(),
        nullptr,
        class_name.c_str()
      };

      ATOM result = RegisterClass(&wc);
      if (!result) {
        std::string msg = getLastErrorText();
        LogError << msg;
        //throw std::runtime_error(msg);
      }
#endif // WIN32
#ifdef X11
      if (cursor_type && !cursor) {
        cursor = XCreateFontCursor(core::global::get_instance(), cursor_type);
      }
#endif
      is_initialized = true;
    }

    void window_class::unregister_class () {
      if (is_initialized && !callback) {
        is_initialized = false;
#ifdef WIN32
        BOOL result = UnregisterClass(class_name.c_str(), core::global::get_instance());
        if (!result) {
          throw std::runtime_error(getLastErrorText());
        }
#endif // WIN32
      }
    }

  } // win

} // gui
