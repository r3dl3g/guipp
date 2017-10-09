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
      static cursor c(cursor_type::arrow);
      return c;
    }

    const cursor& cursor::size_h () {
      static cursor c(cursor_type::size_h);
      return c;
    }

    const cursor& cursor::size_v () {
      static cursor c(cursor_type::size_v);
      return c;
    }

    const cursor& cursor::size_ne_sw () {
      static cursor c(cursor_type::size_ne_sw);
      return c;
    }

    const cursor& cursor::size_nw_se () {
      static cursor c(cursor_type::size_nw_se);
      return c;
    }

    const cursor& cursor::move () {
      static cursor c(cursor_type::move);
      return c;
    }

    const cursor& cursor::ibeam () {
      static cursor c(cursor_type::ibeam);
      return c;
    }

    const cursor& cursor::cross () {
      static cursor c(cursor_type::cross);
      return c;
    }

    const cursor& cursor::wait () {
      static cursor c(cursor_type::wait);
      return c;
    }

    const cursor& cursor::no () {
      static cursor c(cursor_type::no);
      return c;
    }

    cursor::operator os::cursor () const {
      if ((type != cursor_type::none) && !id) {
#ifdef WIN32
        id = LoadCursor(nullptr, MAKEINTRESOURCE(type));
#endif // Win32
#ifdef X11
        id = XCreateFontCursor(core::global::get_instance(), type);
#endif // X11
      }
      return id;
    }

  } // win

} // gui
