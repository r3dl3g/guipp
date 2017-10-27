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
// Library includes
//
#include <gui/core/guidefs.h>


namespace gui {

  namespace win {

    enum class cursor_type {
      none,
      arrow,
      size_h,
      size_v,
      size_ne_sw,
      size_nw_se,
      move,
      ibeam,
      cross,
      wait,
      no
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

      static const cursor& get (win::cursor_type t);

      cursor ();

      operator os::cursor () const;

      operator bool () const;

    private:
      cursor (os::cursor_type t);

      os::cursor_type type;
      mutable os::cursor id;
    };

    // --------------------------------------------------------------------------
    // inlines
    inline cursor::cursor ()
      : type(0)
      , id(0)
    {}

    inline cursor::operator bool () const {
      return type != 0;
    }

    inline cursor::cursor (os::cursor_type t)
      : type(t)
      , id(0)
    {}

  } // namespace win

} // namespace gui
