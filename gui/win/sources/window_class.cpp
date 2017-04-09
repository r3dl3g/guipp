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
#elif X11
      return std::string();
#endif // X11
    }

    window_class::window_class ()
      : class_style(0)
      , style(0)
      , ex_style(0)
      , icon(0)
      , cursor(0)
      , background(0)
      , foreground(0)
      , callback(0)
      , is_initialized(false)
    {}

    window_class::window_class (const window_class& rhs)
      : class_name(rhs.class_name)
      , class_style(rhs.class_style)
      , style(rhs.style)
      , ex_style(rhs.ex_style)
      , icon(rhs.icon)
      , cursor(rhs.cursor)
      , background(rhs.background)
      , foreground(rhs.foreground)
      , callback(rhs.callback)
      , is_initialized(false)
    {}

    window_class::window_class (const std::string& cls_name,
                                os::style class_style,
                                os::style style,
                                os::style ex_style,
                                os::icon icon,
                                os::cursor_type cursor,
                                const draw::brush& background,
                                draw::color foreground,
                                os::event_callback callback)
      : class_name(cls_name)
      , class_style(class_style)
      , style(style)
      , ex_style(ex_style)
      , icon(icon)
      , cursor(cursor)
      , background(background)
      , foreground(foreground)
      , callback(callback)
      , is_initialized(false)
    {}

    void window_class::prepare (window* win) const {
#ifdef WIN32
      if (callback) {
        SetWindowLongPtr(win->get_id(), GWLP_WNDPROC, (LONG_PTR)detail::WindowEventProc);
      }
#endif // WIN32
    }

    const std::string& window_class::get_class_name () const {
      register_class();
      return class_name;
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

    const os::icon window_class::get_icon () const {
      register_class();
      return icon;
    }

    const os::cursor_type window_class::get_cursor () const {
      register_class();
      return cursor;
    }

    const draw::brush& window_class::get_background () const {
      register_class();
      return background;
    }

    const draw::color window_class::get_foreground () const {
      register_class();
      return foreground;
    }

    const os::event_callback window_class::get_callback () const {
      register_class();
      return callback;
    }

    bool window_class::is_valid () const {
      return !class_name.empty();
    }

    void window_class::register_class () const {
      if (is_initialized || callback) {
        return;
      }
#ifdef WIN32
      WNDCLASS wc = {
        /* Register the window class. */
        class_style,
        detail::WindowEventProc,
        0,
        sizeof(window_class*),
        core::global::get_instance(),
        icon,
        cursor ? LoadCursor(nullptr, cursor) : nullptr,
        background,
        nullptr,
        class_name.c_str()
      };

      ATOM result = RegisterClass(&wc);
      if (!result) {
        throw std::runtime_error(getLastErrorText());
      }
#endif // WIN32
      is_initialized = true;
    }

    window_class window_class::custom_class (const std::string& cls_name,
                                             os::style class_style,
                                             os::style style,
                                             os::style ex_style,
                                             os::icon icon,
                                             os::cursor_type cursor,
                                             const draw::brush& background,
                                             draw::color foreground) {
      return window_class(cls_name, class_style, style, ex_style, icon, cursor, background, foreground);
    }

    window_class window_class::custom_class (const std::string& cls_name,
                                             const draw::brush& background,
                                             os::style class_style) {
      return custom_class(cls_name, class_style, // X11: Border width
                          IF_WIN32(0) IF_X11(ButtonPressMask |
                                             ButtonReleaseMask |
                                             ExposureMask |
                                             PointerMotionMask |
                                             StructureNotifyMask |
                                             SubstructureRedirectMask |
                                             FocusChangeMask |
                                             EnterWindowMask |
                                             LeaveWindowMask),
                          0,
                          0,
                          0,
                          background,
                          draw::color::black());
    }

    window_class window_class::sub_class(window_class& cls, const std::string& base_cls) {
#ifdef WIN32
      WNDCLASS wc;
      GetClassInfo(core::global::get_instance(), base_cls.c_str(), &wc);
      cls.callback = wc.lpfnWndProc;
      cls.class_name = base_cls;
#endif // WIN32
      return cls;
    }

    window_class window_class::sub_class (const std::string& cls,
                                          const std::string& base_cls,
                                          os::style style,
                                          os::style ex_style,
                                          draw::color foreground) {
#ifdef WIN32
      WNDCLASS wc;
      GetClassInfo(core::global::get_instance(), base_cls.c_str(), &wc);
      return window_class(base_cls, wc.style, style, ex_style, wc.hIcon, wc.hCursor, wc.hbrBackground, foreground, wc.lpfnWndProc);
#else // !WIN32
      // TBD! Mask here are only temporary
      return custom_class(cls);
#endif // !WIN32
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
