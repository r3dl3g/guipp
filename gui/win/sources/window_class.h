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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "color.h"


namespace gui {

  namespace win {

    template<os::platform P = os::system_platform>
    struct window_class_defaults {};

    template<>
    struct window_class_defaults<os::platform::win32> {
      static constexpr os::cursor_type cursor = IF_WIN32_ELSE((ULONG_PTR)IDC_ARROW, 0);
      static constexpr os::style style = IF_WIN32_ELSE(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, 0);
      static constexpr os::style ex_style = IF_WIN32_ELSE(WS_EX_NOPARENTNOTIFY, 0);
      static constexpr os::style class_style = IF_WIN32_ELSE(CS_DBLCLKS, 0);
    };

    template<>
    struct window_class_defaults<os::platform::x11> {
      static constexpr os::cursor_type cursor = 0;
      static constexpr os::style style = 0;
      static constexpr os::style ex_style = 0;
      static constexpr os::style class_style = 0;
    };

    class window;

    class window_class {
    public:
      window_class ();
      window_class (const window_class&);

      window_class (const std::string& cls_name,
                    os::color background =    color::white,
                    os::cursor_type cursor =  window_class_defaults<>::cursor,
                    os::style style =         window_class_defaults<>::style,
                    os::style ex_style =      window_class_defaults<>::ex_style,
                    os::style class_style =   window_class_defaults<>::class_style);

      virtual void prepare (window*, os::window) const;

      const std::string& get_class_name () const;
      const os::color get_background () const;
      const os::cursor get_cursor () const;
      const os::style get_class_style () const;
      const os::style get_style () const;
      const os::style get_ex_style () const;

      bool is_valid () const;

      inline ~window_class () {
        unregister_class();
      }

    protected:
      virtual os::brush get_background_brush () const;

      void register_class () const;
      void unregister_class ();

      std::string class_name;
      mutable os::color background;
      mutable os::cursor cursor;
      os::cursor_type cursor_type;
      mutable os::style class_style;
      os::style style;
      os::style ex_style;
      mutable bool is_initialized;
    };

  } // win

} // gui
