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
#ifdef X11
# include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include <gui++-win-export.h>


namespace gui {

  namespace win {

    enum class cursor_type : IF_QT_ELSE(int, os::cursor_type) {
      none        = IF_QT_ELSE(Qt::CursorShape::CustomCursor, 0),
      arrow       = IF_WIN32_ELSE(IDC_ARROW, IF_QT_ELSE(Qt::CursorShape::ArrowCursor, XC_arrow)),
      size_h      = IF_WIN32_ELSE(IDC_SIZEWE, IF_QT_ELSE(Qt::CursorShape::SizeHorCursor, XC_sb_h_double_arrow)),
      size_v      = IF_WIN32_ELSE(IDC_SIZENS, IF_QT_ELSE(Qt::CursorShape::SizeVerCursor, XC_sb_v_double_arrow)),
      size_ne_sw  = IF_WIN32_ELSE(IDC_SIZENESW, IF_QT_ELSE(Qt::CursorShape::SizeBDiagCursor, XC_bottom_left_corner)),
      size_nw_se  = IF_WIN32_ELSE(IDC_SIZENWSE, IF_QT_ELSE(Qt::CursorShape::SizeFDiagCursor, XC_bottom_right_corner)),
      move        = IF_WIN32_ELSE(IDC_SIZEALL, IF_QT_ELSE(Qt::CursorShape::DragMoveCursor, XC_fleur)),
      ibeam       = IF_WIN32_ELSE(IDC_IBEAM, IF_QT_ELSE(Qt::CursorShape::IBeamCursor, XC_xterm)),
      cross       = IF_WIN32_ELSE(IDC_CROSS, IF_QT_ELSE(Qt::CursorShape::CrossCursor, XC_crosshair)),
      wait        = IF_WIN32_ELSE(IDC_WAIT, IF_QT_ELSE(Qt::CursorShape::WaitCursor, XC_watch)),
      no          = IF_WIN32_ELSE(IDC_NO, IF_QT_ELSE(Qt::CursorShape::ForbiddenCursor, XC_pirate))
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

      operator const os::cursor& () const;

      operator bool () const;

      cursor ();

    private:
      cursor (cursor_type t);

      cursor_type type;
      mutable os::cursor id;
    };

  } // namespace win

} // namespace gui

#include <gui/win/cursor.inl>
