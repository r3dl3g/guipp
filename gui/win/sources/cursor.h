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

      inline cursor ()
        : type(0)
        , id(0)
      {}

      operator os::cursor () const;

      inline operator bool () const {
        return type != 0;
      }

    private:
      inline cursor (os::cursor_type t)
        : type(t)
        , id(0)
      {}

      os::cursor_type type;
      mutable os::cursor id;
    };

  } // win

} // gui
