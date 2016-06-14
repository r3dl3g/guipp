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
#include <windowsx.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"


namespace gui {

  namespace core {

    rectangle rectangle::default = rectangle(core::point(-1, -1), core::size(-1, -1));

    size::size(const size_type& sz)
      : width(sz.cx)
      , height(sz.cy) {
    }

    size::size(const point_type& pt)
      : width(pt.x)
      , height(pt.y) {
    }

    size::size(const event_param_1& p)
      : width(GET_X_LPARAM(p))
      , height(GET_Y_LPARAM(p)) {
    }

    size::size(const event_param_2& p)
      : width(GET_X_LPARAM(p))
      , height(GET_Y_LPARAM(p)) {
    }

    size::operator size_type() const {
      return{ width, height };
    }

    size::operator point_type() const {
      return{ width, height };
    }

    point::point(const point_type& pt)
      : x(pt.x)
      , y(pt.y) {
    }

    point::point(const event_param_2& p)
      : x(GET_X_LPARAM(p))
      , y(GET_Y_LPARAM(p)) {
    }

    point::operator point_type() const {
      return{ x, y };
    }

    rectangle::rectangle(const rectangle_type& r)
      : topleft(r.left, r.top)
      , bottomright(r.right, r.bottom) {
    }

    rectangle::rectangle(const event_param_2& p)
      : rectangle(*reinterpret_cast<LPRECT>(p)) {
    }

    rectangle::operator rectangle_type() const {
      return{ topleft.x, topleft.y, bottomright.x, bottomright.y };
    }

    void rectangle::setSize(const core::size& sz) {
      bottomright = { topleft.x + sz.width, topleft.y + sz.height };
    }

    std::ostream& operator<<(std::ostream& out, const size& sz) {
      out << sz.width << ", " << sz.height;
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const point& p) {
      out << p.x << ", " << p.y;
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const rectangle& r) {
      out << r.position() << ", " << r.size();
      return out;
    }

  } // core

} // gui
