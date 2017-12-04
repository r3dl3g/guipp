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
 * @brief     C++ API: window class definition
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window.h>
#include <gui/win/window_event_proc.h>


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

    class_info::class_info ()
      : background(0)
      , class_style(0)
      , style(0)
      , ex_style(0)
    {}

    class_info::class_info (const class_info& rhs)
      : class_name(rhs.class_name)
      , background(rhs.background)
      , cursor(rhs.cursor)
      , class_style(rhs.class_style)
      , style(rhs.style)
      , ex_style(rhs.ex_style)
    {}

    class_info::class_info (const std::string& cls_name,
                            os::color background,
                            win::cursor_type cursor,
                            os::style style,
                            os::style ex_style,
                            os::style class_style)
      : class_name(cls_name)
      , background(background)
      , cursor(win::cursor::get(cursor))
      , class_style(class_style)
      , style(style)
      , ex_style(ex_style)
    {}

    const std::string& class_info::get_class_name () const {
      return class_name;
    }

    os::color class_info::get_background () const {
      return background;
    }

    os::cursor class_info::get_cursor () const {
      return cursor;
    }

    os::style class_info::get_class_style () const {
      return class_style;
    }

    os::style class_info::get_style () const {
      return style;
    }

    os::style class_info::get_ex_style () const {
      return ex_style;
    }

    class_info::is_valid () const {
      return !class_name.empty();
    }

  } // win

} // gui
