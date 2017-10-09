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
* @brief     C++ API: cursor definition
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
#include "guidefs.h"


namespace gui {

  namespace win {

    enum class cursor_type : os::cursor_type {
      none = 0,
      arrow = IF_WIN32_ELSE((os::cursor_type)IDC_ARROW, XC_arrow),
      size_h = IF_WIN32_ELSE((os::cursor_type)IDC_SIZEWE, XC_sb_h_double_arrow),
      size_v = IF_WIN32_ELSE((os::cursor_type)IDC_SIZENS, XC_sb_v_double_arrow),
      size_ne_sw = IF_WIN32_ELSE((os::cursor_type)IDC_SIZENESW, XC_bottom_left_corner),
      size_nw_se = IF_WIN32_ELSE((os::cursor_type)IDC_SIZENWSE, XC_bottom_right_corner),
      move = IF_WIN32_ELSE((os::cursor_type)IDC_SIZEALL, XC_fleur),
      ibeam = IF_WIN32_ELSE((os::cursor_type)IDC_IBEAM, XC_xterm),
      cross = IF_WIN32_ELSE((os::cursor_type)IDC_CROSS, XC_crosshair),
      wait = IF_WIN32_ELSE((os::cursor_type)IDC_WAIT, XC_watch),
      no = IF_WIN32_ELSE((os::cursor_type)IDC_NO, XC_pirate)
    };

    class cursor {
    public:
      static const cursor& none ();
      static const cursor& arrow ();
      static const cursor& size_h ();
      static const cursor& size_v ();
      static const cursor& size_ne_sw ();
      static const cursor& size_nw_se ();
      static const cursor& move ();
      static const cursor& ibeam ();
      static const cursor& cross ();
      static const cursor& wait ();
      static const cursor& no ();

      constexpr cursor ()
        : type(cursor_type::none)
        , id(0)
      {}

      constexpr cursor (win::cursor_type t)
        : type(t)
        , id(0)
      {}

      operator os::cursor () const;

      inline operator bool () const {
        return type != cursor_type::none;
      }

    private:
      cursor_type type;
      mutable os::cursor id;
    };

  } // win

} // gui
