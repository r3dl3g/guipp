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

    std::string getLastErrorText() {
#ifdef WIN32
      LPTSTR lpMsgBuf;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, nullptr);
      return lpMsgBuf;
#elif X11
      return std::string();
#endif // X11
    }

    window_class::window_class()
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

    window_class::window_class(const window_class& rhs)
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

    window_class::window_class(const std::string& cls_name,
                               core::windows_style class_style,
                               core::windows_style style,
                               core::windows_style ex_style,
                               core::icon_id icon,
                               core::cursor_id cursor,
                               core::brush_id background,
                               core::color_type foreground,
                               core::event_callback callback)
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

    void window_class::operator=(const window_class& rhs) {
      if (this == &rhs) {
        return;
      }
      unregister_class();

      class_name = rhs.class_name;
      class_style = rhs.class_style;
      style = rhs.style;
      ex_style = rhs.ex_style;
      icon = rhs.icon;
      cursor = rhs.cursor;
      background = rhs.background;
      foreground = rhs.foreground;
      callback = rhs.callback;
    }

    void window_class::prepare(window* win) const {
#ifdef WIN32
      if (callback) {
        SetWindowLongPtr(win->get_id(), GWLP_WNDPROC, (LONG_PTR)detail::WindowEventProc);
      }
#endif // WIN32
    }

    const std::string& window_class::get_class_name() const {
      register_class();
      return class_name;
    }

    const core::windows_style window_class::get_class_style() const {
      register_class();
      return class_style;
    }

    const core::windows_style window_class::get_style() const {
      register_class();
      return style;
    }

    const core::windows_style window_class::get_ex_style() const {
      register_class();
      return ex_style;
    }

    const core::icon_id window_class::get_icon() const {
      register_class();
      return icon;
    }

    const core::cursor_id window_class::get_cursor() const {
      register_class();
      return cursor;
    }

    const core::brush_id window_class::get_background() const {
      register_class();
      return background;
    }

    const core::color_type window_class::get_foreground() const {
      register_class();
      return foreground;
    }

    const core::event_callback window_class::get_callback() const {
      register_class();
      return callback;
    }

    void window_class::register_class() const {
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
        cursor,
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

    window_class window_class::custom_class(const std::string& cls_name,
                                            core::windows_style class_style,
                                            core::windows_style style,
                                            core::windows_style ex_style,
                                            core::icon_id icon,
                                            core::cursor_id cursor,
                                            core::brush_id background,
                                            core::color_type foreground) {
      return window_class(cls_name, class_style, style, ex_style, icon, cursor, background, foreground);
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

    window_class window_class::sub_class(const std::string& cls,
                                         const std::string& base_cls,
                                         core::windows_style style,
                                         core::windows_style ex_style,
                                         core::color_type foreground) {
#ifdef WIN32
      WNDCLASS wc;
      GetClassInfo(core::global::get_instance(), base_cls.c_str(), &wc);
      return window_class(base_cls, wc.style, style, ex_style, wc.hIcon, wc.hCursor, wc.hbrBackground, foreground, wc.lpfnWndProc);
#else // !WIN32
      // TBD! Mask here are only temporary
      return window_class(cls, ButtonPressMask | ButtonReleaseMask | ExposureMask | PointerMotionMask |
                               StructureNotifyMask | SubstructureRedirectMask | FocusChangeMask |
                               EnterWindowMask | LeaveWindowMask, style, ex_style, 0, 0, 0, foreground);
#endif // !WIN32
    }

    void window_class::unregister_class() {
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
