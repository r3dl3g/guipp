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
 * @brief     mouse cursor definition
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_X11
# include <X11/cursorfont.h>
#endif // GUIPP_X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include <gui/win/gui++-win-export.h>


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

    class GUIPP_WIN_EXPORT cursor {
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

      operator os::cursor () const;

      operator bool () const;
      bool operator== (const cursor&) const;
      bool operator!= (const cursor&) const;

      cursor ();

    private:
      static os::cursor_type convert (cursor_type);

      explicit cursor (cursor_type t);

      cursor_type type;
      mutable os::cursor id;
    };

  } // namespace win

} // namespace gui

#include <gui/win/cursor.inl>
