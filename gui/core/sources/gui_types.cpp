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

    rectangle rectangle::default_rectangle = rectangle(core::point(-1, -1), core::point(-1, -1));

    size::size(const size_type& sz)
      : width(sz.cx)
      , height(sz.cy)
     {}

    size::size(const point_type& pt)
      : width(pt.x)
      , height(pt.y) {
    }

#ifdef WIN32
    size::size(const event_param_1& p)
      : width(GET_X_LPARAM(p))
      , height(GET_Y_LPARAM(p)) {
    }

    size::size(const event_param_2& p)
      : width(GET_X_LPARAM(p))
      , height(GET_Y_LPARAM(p)) {
    }
#endif // WIN32

    size::operator size_type() const {
      return { width, height };
    }

    size::operator point_type() const {
#ifdef WIN32
      return { width, height };
#elif X11
      return { (short)width, (short)height };
#endif // X11
    }

    point::point(const point_type& pt)
      : x(pt.x)
      , y(pt.y) {
    }

#ifdef WIN32
    point::point(const event_param_2& p)
      : x(GET_X_LPARAM(p))
      , y(GET_Y_LPARAM(p)) {
    }
#endif // WIN32

    point::operator point_type() const {
#ifdef WIN32
      return{ x, y };
#elif X11
      return { (short)x, (short)y };
#endif // X11
    }

    rectangle::rectangle(const rectangle_type& r)
#ifdef WIN32
      : topleft(r.left, r.top)
      , bottomright(r.right, r.bottom) {
#elif X11
      : topleft(r.x, r.y)
      , bottomright(r.x + r.width, r.y + r.height) {
#endif // X11
    }

#ifdef WIN32
    rectangle::rectangle(const event_param_2& p)
      : rectangle(*reinterpret_cast<LPRECT>(p)) {
    }
#endif // WIN32

    rectangle::operator rectangle_type() const {
#ifdef WIN32
      return{ topleft.x, topleft.y, bottomright.x, bottomright.y };
#elif X11
      return{ (short)topleft.x, (short)topleft.y,
              (unsigned short)(bottomright.x - topleft.x), (unsigned short)(bottomright.y - topleft.y) };
#endif // X11
    }

    void rectangle::setSize(const core::size& sz) {
      bottomright = core::point( topleft.x + sz.width, topleft.y + sz.height );
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
