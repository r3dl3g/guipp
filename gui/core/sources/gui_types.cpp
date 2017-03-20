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
    const rectangle rectangle::def = rectangle(core::point(-1, -1), core::point(-1, -1));
#endif // WIN32
#ifdef X11
    const rectangle rectangle::def = rectangle(0, 0, 1, 1);
#endif // X11

    const size size::zero;
    const point point::zero;
    const rectangle rectangle::zero;

    size::size(const size_type& sz)
      : sz(sz)
     {}

    size::size(const point_type& pt) {
      sz = { (type)pt.x, (type)pt.y };
    }

#ifdef WIN32
    size::size(const event_param_1& p) {
      sz = { (type)GET_X_LPARAM(p), (type)GET_Y_LPARAM(p) };
    }

    size::size(const event_param_2& p) {
      sz = { (type)GET_X_LPARAM(p), (type)GET_Y_LPARAM(p) };
    }
#endif // WIN32

    size::operator size_type() const {
      return sz;
    }

    size::operator point_type() const {
#ifdef WIN32
      return { width(), height() };
#elif X11
      return { (short)width(), (short)height() };
#endif // X11
    }

    point::point(const point_type& rhs)
      : pt(rhs)
    {}

#ifdef WIN32
    point::point(const event_param_2& p) {
      pt = { (type)GET_X_LPARAM(p) , (type)GET_Y_LPARAM(p) };
    }
#endif // WIN32

    point::operator point_type() const {
      return pt;
    }

    rectangle::rectangle(const rectangle_type& r)
#ifdef WIN32
      : tl(r.left, r.top)
      , br(r.right, r.bottom) {
#elif X11
      : tl(r.x, r.y)
      , br(r.x + r.width, r.y + r.height) {
#endif // X11
    }

#ifdef WIN32
    rectangle::rectangle(const event_param_2& p)
      : rectangle(*reinterpret_cast<LPRECT>(p)) {
    }
#endif // WIN32

    rectangle::operator rectangle_type() const {
#ifdef WIN32
      return{ tl.x(), tl.y(), br.x(), br.y() };
#elif X11
      return{ x(), y(), width(), height() };
#endif // X11
    }

    void rectangle::move (const core::point& pt) {
      tl += pt;
      br += pt;
    }

    void rectangle::move (const core::size& sz) {
      tl += sz;
      br += sz;
    }

    void rectangle::move_x (core::point::type dx) {
      tl.move_x(dx);
      br.move_x(dx);
    }

    void rectangle::move_y (core::point::type dy) {
      tl.move_y(dy);
      br.move_y(dy);
    }

    void rectangle::set_size (const core::size& sz) {
      br = core::point(tl.x() + sz.width(), tl.y() + sz.height() );
    }

    void rectangle::x (core::point::type x) {
      tl.x(x);
    }

    void rectangle::y (core::point::type y) {
      tl.y(y);
    }

    void rectangle::x2 (core::point::type x) {
      br.x(x);
    }

    void rectangle::y2 (core::point::type y) {
      br.y(y);
    }

    void rectangle::height (core::size::type height) {
      br.y(tl.y() + height);
    }

    void rectangle::width (core::size::type width) {
      br.x(tl.x() + width);
    }

    rectangle& rectangle::operator|= (const rectangle& rhs) {
      core::point::type x0 = std::min(x(), rhs.x());
      core::point::type y0 = std::min(y(), rhs.y());
      core::point::type x1 = std::max(x2(), rhs.x2());
      core::point::type y1 = std::max(y2(), rhs.y2());
      tl = {x0, y0};
      br = {x1, y1};
      return *this;
    }

    rectangle& rectangle::operator&= (const rectangle& rhs) {
      core::point::type x0 = std::max(x(), rhs.x());
      core::point::type y0 = std::max(y(), rhs.y());
      core::point::type x1 = std::min(x2(), rhs.x2());
      core::point::type y1 = std::min(y2(), rhs.y2());
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
