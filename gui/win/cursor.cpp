/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
// Library includes
//
#include <gui/win/cursor.h>


namespace gui {

  namespace win {

    const cursor& cursor::none () {
      static cursor c(cursor_type::none);
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

    const cursor& cursor::get (win::cursor_type t) {
      switch (t) {
        default:
        case cursor_type::none:       return cursor::none();
        case cursor_type::arrow:      return cursor::arrow();
        case cursor_type::size_h:     return cursor::size_h();
        case cursor_type::size_v:     return cursor::size_v();
        case cursor_type::size_ne_sw: return cursor::size_ne_sw();
        case cursor_type::size_nw_se: return cursor::size_nw_se();
        case cursor_type::move:       return cursor::move();
        case cursor_type::ibeam:      return cursor::ibeam();
        case cursor_type::cross:      return cursor::cross();
        case cursor_type::wait:       return cursor::wait();
        case cursor_type::no:         return cursor::no();
      }
    }

    os::cursor_type cursor::convert (cursor_type t) {
      switch (t) {
        default:
        case cursor_type::none:       return IF_WIN32_X11_QT_ELSE(0,            0,                      Qt::CursorShape::CustomCursor,    0);
        case cursor_type::arrow:      return IF_WIN32_X11_QT_ELSE(IDC_ARROW,    XC_arrow,               Qt::CursorShape::ArrowCursor,     1);
        case cursor_type::size_h:     return IF_WIN32_X11_QT_ELSE(IDC_SIZEWE,   XC_sb_h_double_arrow,   Qt::CursorShape::SizeHorCursor,   2);
        case cursor_type::size_v:     return IF_WIN32_X11_QT_ELSE(IDC_SIZENS,   XC_sb_v_double_arrow,   Qt::CursorShape::SizeVerCursor,   3);
        case cursor_type::size_ne_sw: return IF_WIN32_X11_QT_ELSE(IDC_SIZENESW, XC_bottom_left_corner,  Qt::CursorShape::SizeBDiagCursor, 4);
        case cursor_type::size_nw_se: return IF_WIN32_X11_QT_ELSE(IDC_SIZENWSE, XC_bottom_right_corner, Qt::CursorShape::SizeFDiagCursor, 5);
        case cursor_type::move:       return IF_WIN32_X11_QT_ELSE(IDC_SIZEALL,  XC_fleur,               Qt::CursorShape::DragMoveCursor,  6);
        case cursor_type::ibeam:      return IF_WIN32_X11_QT_ELSE(IDC_IBEAM,    XC_xterm,               Qt::CursorShape::IBeamCursor,     7);
        case cursor_type::cross:      return IF_WIN32_X11_QT_ELSE(IDC_CROSS,    XC_crosshair,           Qt::CursorShape::CrossCursor,     8);
        case cursor_type::wait:       return IF_WIN32_X11_QT_ELSE(IDC_WAIT,     XC_watch,               Qt::CursorShape::WaitCursor,      9);
        case cursor_type::no:         return IF_WIN32_X11_QT_ELSE(IDC_NO,       XC_pirate,              Qt::CursorShape::ForbiddenCursor, 10);
      }
    }

    cursor::cursor ()
      : type(cursor_type::none)
#ifndef GUIPP_QT
      , id(0)
#endif
    {}

    cursor::cursor (cursor_type t)
      : type(t)
      , id(IF_QT_ELSE(convert(type), 0))
    {}

    cursor::operator os::cursor () const {
#ifdef GUIPP_WIN
      if ((type != cursor_type::none) && !id) {
        id = LoadCursor(nullptr, convert(type));
      }
#elif GUIPP_X11
      if ((type != cursor_type::none) && !id) {
        id = XCreateFontCursor(core::global::get_instance(), convert(type));
      }
#elif GUIPP_QT
      const auto t = convert(type);
      if ((type != cursor_type::none) && (id.shape() != t)) {
        id = QCursor(t);
      }
#endif
      return id;
    }

  } // win

} // gui
