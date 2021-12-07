/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     window class definition
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <typeinfo>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/color.h>
#include <gui/win/cursor.h>


namespace gui {

  namespace win {

    GUIPP_WIN_EXPORT std::string getLastErrorText ();

    template<core::os::platform_t P = core::os::system_platform>
    struct window_class_defaults {};

    template<>
    struct window_class_defaults<core::os::platform_t::win32> {
      static constexpr win::cursor_type cursor = win::cursor_type::arrow;
      static constexpr os::style style = IF_WIN32_ELSE(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, static_cast<os::style>(0));
      static constexpr os::style style_no_focus = IF_WIN32_ELSE(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, static_cast<os::style>(0));
      static constexpr os::style ex_style = IF_WIN32_ELSE(WS_EX_NOPARENTNOTIFY, static_cast<os::style>(0));
      static constexpr os::style class_style = IF_WIN32_ELSE(CS_DBLCLKS, static_cast<os::style>(0));
    };

    template<>
    struct window_class_defaults<core::os::platform_t::x11> {
      static constexpr win::cursor_type cursor = win::cursor_type::none;
      static constexpr os::style style = static_cast<os::style>(WS_TABSTOP);
      static constexpr os::style style_no_focus = static_cast<os::style>(0);
      static constexpr os::style ex_style = static_cast<os::style>(0);
      static constexpr os::style class_style = IF_X11_ELSE(InputOutput, static_cast<os::style>(0));
    };

    template<>
    struct window_class_defaults<core::os::platform_t::qt> {
      static constexpr win::cursor_type cursor = win::cursor_type::arrow;
      static constexpr os::style style = static_cast<os::style>(IF_QT_ELSE((Qt::WindowFlags::Int)(Qt::Window), 0));
      static constexpr os::style style_no_focus = static_cast<os::style>(IF_QT_ELSE((Qt::WindowFlags::Int)(Qt::Window)|Qt::WindowDoesNotAcceptFocus, 0));
      static constexpr os::style ex_style = static_cast<os::style>(0);
      static constexpr os::style class_style = static_cast<os::style>(0);
    };

    class GUIPP_WIN_EXPORT class_info {
    public:
      class_info ();
      class_info (const class_info&);

      class_info (const char* cls_name,
                  os::color background,
                  win::cursor_type cursor,
                  os::style style,
                  os::style ex_style,
                  os::style class_style);

      const char* get_class_name () const;
      os::color get_background () const;
      const win::cursor& get_cursor () const;
      os::style get_class_style () const;
      os::style get_style () const;
      os::style get_ex_style () const;

      bool is_valid () const;

    protected:
      const char* class_name;
      os::color background;
      win::cursor cursor;
      os::style class_style;
      os::style style;
      os::style ex_style;
    };

    template<typename T,
             os::color B = color::white,
             win::cursor_type C = window_class_defaults<>::cursor,
             os::style S = window_class_defaults<>::style,
             os::style ES = window_class_defaults<>::ex_style,
             os::style CS = window_class_defaults<>::class_style>
    struct window_class {
      static const char* name () {
        return typeid (T).name();
      }

      static constexpr os::color background = B;
      static constexpr win::cursor_type cursor = C;
      static constexpr os::style style = S;
      static constexpr os::style ex_style = ES;
      static constexpr os::style class_style = CS;

      static class_info get () {
        return class_info {name(), background, cursor, style, ex_style, class_style};
      }

    };

  } // namespace win

} // namespace gui
