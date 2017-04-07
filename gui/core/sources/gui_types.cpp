/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
#include <windowsx.h>
#endif

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

    const size size::zero;
    const size size::one(1, 1);

    const point point::zero;
    const point point::one(1, 1);
    const point point::undefined(std::numeric_limits<point::type>::min(), std::numeric_limits<point::type>::min());

    const rectangle rectangle::zero;

    size::size(const os::size& s)
      : m_w(static_cast<type>(s.cx))
      , m_h(static_cast<type>(s.cy))
    {}

    size::size(const os::point& pt)
      : m_w(static_cast<type>(pt.x))
      , m_h(static_cast<type>(pt.y))
    {}

    size::size(const os::rectangle& r) 
      : m_w(static_cast<type>(r.right - r.left))
      , m_h(static_cast<type>(r.bottom - r.top))
    {}

#ifdef WIN32
    size::size(const event_param_1& p) {
      m_w = static_cast<type>(GET_X_LPARAM(p));
      m_h = static_cast<type>(GET_Y_LPARAM(p));
    }

    size::size(const event_param_2& p) {
      m_w = static_cast<type>(GET_X_LPARAM(p));
      m_h = static_cast<type>(GET_Y_LPARAM(p));
    }
#endif // WIN32

    size::operator os::size() const {
      return os::size{ static_cast<os::size_type>(m_w), static_cast<os::size_type>(m_h) };
    }

    size::operator os::point() const {
      return{ static_cast<os::point_type>(width()), static_cast<os::point_type>(height()) };
    }

    point::point(const os::point& rhs)
      : m_x(static_cast<type>(rhs.x))
      , m_y(static_cast<type>(rhs.y))
    {}

    point::point(const os::rectangle& rhs)
      : m_x(static_cast<type>(rhs.left))
      , m_y(static_cast<type>(rhs.top))
    {}

#ifdef WIN32
    point::point(const event_param_2& p) {
      m_x = static_cast<type>(GET_X_LPARAM(p));
      m_y = static_cast<type>(GET_Y_LPARAM(p));
    }
#endif // WIN32

    point::operator os::point() const {
      return{ static_cast<os::point_type>(m_x), static_cast<os::point_type>(m_y) };
    }

    rectangle::rectangle(const os::rectangle& r)
#ifdef WIN32
      : tl(static_cast<point::type>(r.left), static_cast<point::type>(r.top))
      , br(static_cast<point::type>(r.right), static_cast<point::type>(r.bottom))
#elif X11
      : tl(static_cast<point::type>(r.x), static_cast<point::type>(r.y))
      , br(static_cast<point::type>(r.x + r.width), static_cast<point::type>(r.y + r.height))
#endif // X11
    {}

#ifdef WIN32
    rectangle::rectangle(const event_param_2& p)
      : rectangle(*reinterpret_cast<LPRECT>(p)) {
    }
#endif // WIN32

    rectangle::operator os::rectangle() const {
      return{ static_cast<os::point_type>(x()), static_cast<os::point_type>(y()),
#ifdef WIN32
              static_cast<os::point_type>(x2()), static_cast<os::point_type>(y2())
#elif X11
              static_cast<os::size_type>(width()), static_cast<os::size_type>(height())
#endif // X11
      };
    }

    void rectangle::move (const point& pt) {
      tl += pt;
      br += pt;
    }

    void rectangle::move (const core::size& sz) {
      tl += sz;
      br += sz;
    }

    void rectangle::move_x (point::type dx) {
      tl.move_x(dx);
      br.move_x(dx);
    }

    void rectangle::move_y (point::type dy) {
      tl.move_y(dy);
      br.move_y(dy);
    }

    void rectangle::set_size (const core::size& sz) {
      br = point(tl.x() + sz.width(), tl.y() + sz.height() );
    }

    void rectangle::x (point::type x) {
      tl.x(x);
    }

    void rectangle::y (point::type y) {
      tl.y(y);
    }

    void rectangle::x2 (point::type x) {
      br.x(x);
    }

    void rectangle::y2 (point::type y) {
      br.y(y);
    }

    void rectangle::height (core::size::type height) {
      br.y(tl.y() + height);
    }

    void rectangle::width (core::size::type width) {
      br.x(tl.x() + width);
    }

    rectangle& rectangle::operator|= (const rectangle& rhs) {
      point::type x0 = std::min(x(), rhs.x());
      point::type y0 = std::min(y(), rhs.y());
      point::type x1 = std::max(x2(), rhs.x2());
      point::type y1 = std::max(y2(), rhs.y2());
      tl = {x0, y0};
      br = {x1, y1};
      return *this;
    }

    rectangle& rectangle::operator&= (const rectangle& rhs) {
      point::type x0 = std::max(x(), rhs.x());
      point::type y0 = std::max(y(), rhs.y());
      point::type x1 = std::min(x2(), rhs.x2());
      point::type y1 = std::min(y2(), rhs.y2());
      tl = {x0, y0};
      br = {x1, y1};
      return *this;
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
