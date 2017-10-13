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
* @brief     C++ API: basic window types
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>
#include <algorithm>

#ifdef WIN32
# include <windowsx.h>
#endif
#ifdef X11
//# ifdef XLIB
#  include <X11/Xlib.h>
//# endif
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"


namespace gui {

  namespace core {

#ifdef WIN32
    const rectangle rectangle::def = rectangle(point(-1, -1), point(-1, -1));
#endif // WIN32
#ifdef X11
    const rectangle rectangle::def = rectangle(0, 0, 1, 1);
#endif // X11

    // --------------------------------------------------------------------------
    const size size::zero;
    const size size::one(1, 1);

    const point point::zero;
    const point point::undefined(std::numeric_limits<point::type>::min(), std::numeric_limits<point::type>::min());

    const rectangle rectangle::zero;

    size::size (const os::size& s)
      : data(static_cast<type>(s.cx),
             static_cast<type>(s.cy))
    {}

    size::size (const os::point& pt)
      : data(static_cast<type>(pt.x),
             static_cast<type>(pt.y))
    {}

#ifdef WIN32
    size::size(const os::rectangle& r)
      : data(static_cast<type>(r.right - r.left),
             static_cast<type>(r.bottom - r.top))
    {}

    size::size(const os::win32::wParam& p) {
      data.w = static_cast<type>(GET_X_LPARAM(p));
      data.h = static_cast<type>(GET_Y_LPARAM(p));
    }

    size::size(const os::win32::lParam& p) {
      data.w = static_cast<type>(GET_X_LPARAM(p));
      data.h = static_cast<type>(GET_Y_LPARAM(p));
    }
#endif // WIN32
#ifdef X11
    size::size (const os::rectangle& r)
      : data(static_cast<type>(r.width),
             static_cast<type>(r.height))
    {}
#endif // X11

    void size::clear (type v) {
      data.w = data.h = v;
    }

    size::operator os::size() const {
      return os();
    }

    os::size size::os () const {
      return { os_width(), os_height() };
    }


    size::operator os::point() const {
      return{ static_cast<os::point_type>(width()), static_cast<os::point_type>(height()) };
    }

    // --------------------------------------------------------------------------
    point::point (const os::point& rhs)
      : data(static_cast<type>(rhs.x),
             static_cast<type>(rhs.y))
    {}

#ifdef WIN32
    point::point (const os::rectangle& rhs)
      : data(static_cast<type>(rhs.left),
             static_cast<type>(rhs.top))
    {}

    point::point (const os::win32::lParam& p) {
      data.x = static_cast<type>(GET_X_LPARAM(p));
      data.y = static_cast<type>(GET_Y_LPARAM(p));
    }
#endif // WIN32
#ifdef X11
    point::point (const os::rectangle& r)
      : data(static_cast<type>(r.x),
             static_cast<type>(r.y))
    {}
#endif // X11

    point::operator os::point () const {
      return { os_x(), os_y() };
    }

    os::point point::os () const {
      return { os_x(), os_y() };
    }

    // --------------------------------------------------------------------------
    rectangle::rectangle (const os::rectangle& r)
#ifdef WIN32
      : pos(static_cast<point::type>(r.left), static_cast<point::type>(r.top))
      , sz(static_cast<size::type>(r.right - r.left), static_cast<size::type>(r.bottom - r.top))
#endif // Win32
#ifdef X11
      : pos(static_cast<point::type>(r.x), static_cast<point::type>(r.y))
      , sz(static_cast<size::type>(r.width), static_cast<size::type>(r.height))
#endif // X11
    {}

#ifdef WIN32
    rectangle::rectangle (const os::win32::lParam& p)
      : rectangle(*reinterpret_cast<LPRECT>(p)) {
    }
#endif // WIN32

    rectangle::operator os::rectangle () const {
      return os();
    }

    os::rectangle rectangle::os () const {
      return{ static_cast<os::point_type>(x()), static_cast<os::point_type>(y()),
#ifdef WIN32
              static_cast<os::point_type>(x2()), static_cast<os::point_type>(y2())
#endif // Win32
#ifdef X11
              static_cast<os::size_type>(width()), static_cast<os::size_type>(height())
#endif // X11
      };
    }

    void rectangle::move (const point& pt) {
      pos += pt;
    }

    void rectangle::move (const core::size& sz) {
      pos += sz;
    }

    void rectangle::move_x (point::type dx) {
      pos.move_x(dx);
    }

    void rectangle::move_y (point::type dy) {
      pos.move_y(dy);
    }

    void rectangle::move_to_x (point::type new_x) {
      x(new_x);
    }

    void rectangle::move_to_y (point::type new_y) {
      y(new_y);
    }

    void rectangle::set_size (const core::size& s) {
      sz = s;
    }

    void rectangle::x (point::type x) {
      pos.x(x);
    }

    void rectangle::y (point::type y) {
      pos.y(y);
    }

    void rectangle::x2 (point::type new_x) {
      sz.width(new_x - x());
    }

    void rectangle::y2 (point::type new_y) {
      sz.height(new_y - y());
    }

    void rectangle::width (core::size::type width) {
      sz.width(width);
    }

    void rectangle::height (core::size::type height) {
      sz.height(height);
    }

    rectangle& rectangle::operator|= (const rectangle& rhs) {
      point::type x0 = std::min(x(), rhs.x());
      point::type y0 = std::min(y(), rhs.y());
      point::type x1 = std::max(x2(), rhs.x2());
      point::type y1 = std::max(y2(), rhs.y2());
      return operator=({point{x0, y0}, point{x1, y1}});
    }

    rectangle& rectangle::operator&= (const rectangle& rhs) {
      point::type x0 = std::max(x(), rhs.x());
      point::type y0 = std::max(y(), rhs.y());
      point::type x1 = std::min(x2(), rhs.x2());
      point::type y1 = std::min(y2(), rhs.y2());
      return operator=({point{x0, y0}, point{x1, y1}});
    }

    std::ostream& operator<<(std::ostream& out, const size& sz) {
      out << sz.width() << ", " << sz.height();
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const point& p) {
      out << p.x() << ", " << p.y();
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const rectangle& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

  } // core

} // gui
