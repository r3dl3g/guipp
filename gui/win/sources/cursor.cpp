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

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef X11
#include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include "cursor.h"


namespace gui {

  namespace win {

    const cursor& cursor::none () {
      static cursor c;
      return c;
    }

    const cursor& cursor::arrow () {
      static cursor c(IF_WIN32_ELSE(IDC_ARROW, XC_arrow));
      return c;
    }

    const cursor& cursor::size_h () {
      static cursor c(IF_WIN32_ELSE(IDC_SIZEWE, XC_sb_h_double_arrow));
      return c;
    }

    const cursor& cursor::size_v () {
      static cursor c(IF_WIN32_ELSE(IDC_SIZENS, XC_sb_v_double_arrow));
      return c;
    }

    const cursor& cursor::size_ne_sw () {
      static cursor c(IF_WIN32_ELSE(IDC_SIZENESW, XC_bottom_left_corner));
      return c;
    }

    const cursor& cursor::size_nw_se () {
      static cursor c(IF_WIN32_ELSE(IDC_SIZENWSE, XC_bottom_right_corner));
      return c;
    }

    const cursor& cursor::move () {
      static cursor c(IF_WIN32_ELSE(IDC_SIZEALL, XC_fleur));
      return c;
    }

    const cursor& cursor::ibeam () {
      static cursor c(IF_WIN32_ELSE(IDC_IBEAM, XC_xterm));
      return c;
    }

    const cursor& cursor::cross () {
      static cursor c(IF_WIN32_ELSE(IDC_CROSS, XC_crosshair));
      return c;
    }

    const cursor& cursor::wait () {
      static cursor c(IF_WIN32_ELSE(IDC_WAIT, XC_watch));
      return c;
    }

    const cursor& cursor::no () {
      static cursor c(IF_WIN32_ELSE(IDC_NO, XC_pirate));
      return c;
    }

    cursor::operator os::cursor () const {
      if (type && !id) {
#ifdef WIN32
        id = LoadCursor(nullptr, (LPCSTR)type);
#endif // Win32
#ifdef X11
        id = XCreateFontCursor(core::global::get_instance(), type);
#endif // X11
      }
      return id;
    }

  } // win

} // gui
