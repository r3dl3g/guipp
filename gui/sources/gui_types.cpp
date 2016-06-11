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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"


namespace gui {

  rectangle rectangle::default = rectangle(gui::position(-1, -1), gui::size(-1, -1));

  size::size(const size_type& sz)
    : width(sz.cx)
    , height(sz.cy)
  {}

  size::size(const point_type& pt)
    : width(pt.x)
    , height(pt.y)
  {}

  size::size(const event_param_1& p)
    : width(static_cast<INT16>(LOWORD(p)))
    , height(static_cast<INT16>(HIWORD(p)))
  {}

  size::size(const event_param_2& p)
    : width(static_cast<INT16>(LOWORD(p)))
    , height(static_cast<INT16>(HIWORD(p))) {}

  size::operator size_type() const {
    return { width, height };
  }

  size::operator point_type() const {
    return { width, height };
  }

  position::position(const point_type& pt)
    : x(pt.x)
    , y(pt.y)
  {}

  position::position(const event_param_2& p)
    : x(static_cast<INT16>(LOWORD(p)))
    , y(static_cast<INT16>(HIWORD(p)))
  {}

  position::operator point_type() const {
    return { x, y };
  }

  rectangle::rectangle(const rectangle_type& r)
    : position(r.left, r.top)
    , size(r.right - r.left, r.bottom - r.top)
  {}

  rectangle::rectangle(const event_param_2& p)
    : rectangle(*reinterpret_cast<LPRECT>(p))
  {}

  rectangle::operator rectangle_type() const {
    return { position.x, position.y, position.x + size.width, position.y + size.height };
  }

  std::ostream& operator<<(std::ostream& out, const size& sz) {
    out << sz.width << ", " << sz.height;
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const position& p) {
    out << p.x << ", " << p.y;
    return out;
  }

  std::ostream& operator<<(std::ostream& out, const rectangle& r) {
    out << r.position << ", " << r.size;
    return out;
  }

}
