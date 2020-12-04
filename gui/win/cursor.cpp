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
        case cursor_type::none:       return IF_QT_ELSE(Qt::CursorShape::CustomCursor, 0);
        case cursor_type::arrow:      return IF_WIN32_ELSE(IDC_ARROW, IF_QT_ELSE(Qt::CursorShape::ArrowCursor, XC_arrow));
        case cursor_type::size_h:     return IF_WIN32_ELSE(IDC_SIZEWE, IF_QT_ELSE(Qt::CursorShape::SizeHorCursor, XC_sb_h_double_arrow));
        case cursor_type::size_v:     return IF_WIN32_ELSE(IDC_SIZENS, IF_QT_ELSE(Qt::CursorShape::SizeVerCursor, XC_sb_v_double_arrow));
        case cursor_type::size_ne_sw: return IF_WIN32_ELSE(IDC_SIZENESW, IF_QT_ELSE(Qt::CursorShape::SizeBDiagCursor, XC_bottom_left_corner));
        case cursor_type::size_nw_se: return IF_WIN32_ELSE(IDC_SIZENWSE, IF_QT_ELSE(Qt::CursorShape::SizeFDiagCursor, XC_bottom_right_corner));
        case cursor_type::move:       return IF_WIN32_ELSE(IDC_SIZEALL, IF_QT_ELSE(Qt::CursorShape::DragMoveCursor, XC_fleur));
        case cursor_type::ibeam:      return IF_WIN32_ELSE(IDC_IBEAM, IF_QT_ELSE(Qt::CursorShape::IBeamCursor, XC_xterm));
        case cursor_type::cross:      return IF_WIN32_ELSE(IDC_CROSS, IF_QT_ELSE(Qt::CursorShape::CrossCursor, XC_crosshair));
        case cursor_type::wait:       return IF_WIN32_ELSE(IDC_WAIT, IF_QT_ELSE(Qt::CursorShape::WaitCursor, XC_watch));
        case cursor_type::no:         return IF_WIN32_ELSE(IDC_NO, IF_QT_ELSE(Qt::CursorShape::ForbiddenCursor, XC_pirate));
      }
    }

    cursor::cursor ()
      : type(cursor_type::none)
#ifndef QT_WIDGETS_LIB
      , id(0)
#endif
    {}

    cursor::cursor (cursor_type t)
      : type(t)
      , id(IF_QT_ELSE(convert(type), 0))
    {}

    cursor::operator const os::cursor& () const {
#ifdef WIN32
      if ((type != cursor_type::none) && !id) {
        id = LoadCursor(nullptr, convert(type));
      }
#endif // Win32
#ifdef X11
      if ((type != cursor_type::none) && !id) {
        id = XCreateFontCursor(core::global::get_instance(), convert(type));
      }
#endif // X11
#ifdef QT_WIDGETS_LIB
      const auto t = convert(type);
      if ((type != cursor_type::none) && (id.shape() != t)) {
        id = QCursor(t);
      }
#endif
      return id;
    }

  } // win

} // gui
