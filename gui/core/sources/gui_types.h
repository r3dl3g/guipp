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
* @brief     C++ API: basic structures
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace core {

    struct size {
      typedef int type;

      inline size(type width = 0, type height = 0)
        : width(width)
        , height(height)
      {}

      explicit size(const size_type& sz);
      explicit size(const point_type& pt);

#ifdef WIN32
      explicit size(const event_param_1& p);
      explicit size(const event_param_2& p);
#endif // WIN32
#ifdef X11
      template<typename T>
      explicit size(const T& rhs)
        : width(rhs.width)
        , height(rhs.height)
      {}
#endif // X11

      operator size_type() const;
      operator point_type() const;

      inline bool operator==(const size& rhs) const {
        return (width == rhs.width) && (height == rhs.height);
      }

      inline bool operator!=(const size& rhs) const {
        return !operator==(rhs);
      }

      inline size operator+ (const size& rhs) const {
        return { width + rhs.width, height + rhs.height };
      }

      inline size operator- (const size& rhs) const {
        return { width - rhs.width, height - rhs.height };
      }

      type width;
      type height;
    };

    std::ostream& operator<<(std::ostream& out, const size&);

    struct point {
      typedef int type;

      inline point(type x = 0, type y = 0)
        : x(x)
        , y(y) {
      }

      explicit point(const point_type& pt);

#ifdef WIN32
      explicit point(const event_param_2& p);
#endif // WIN32
#ifdef X11
      template<typename T>
      explicit point(const T& rhs)
        : x(rhs.x)
        , y(rhs.y)
      {}
#endif // X11

      operator point_type() const;

      inline point operator+ (const size& s) const {
        return { x + (type)s.width, y + (type)s.height };
      }

      inline point operator- (const size& s) const {
        return { x - (type)s.width, y - (type)s.height };
      }

      inline size operator- (const point& rhs) const {
        return { (size::type)(x - rhs.x), (size::type)(y - rhs.y) };
      }

      inline bool operator== (const point& rhs) const {
        return (x == rhs.x) && (y == rhs.y);
      }

      inline bool operator!= (const point& rhs) const {
        return !operator==(rhs);
      }

      inline bool operator< (const point& rhs) const {
        return (x < rhs.x) && (y < rhs.y);
      }

      inline bool operator<= (const point& rhs) const {
        return (x <= rhs.x) && (y <= rhs.y);
      }

      inline bool operator> (const point& rhs) const {
        return (x > rhs.x) && (y > rhs.y);
      }

      inline bool operator>= (const point& rhs) const {
        return (x >= rhs.x) && (y >= rhs.y);
      }

      type x;
      type y;
    };

    std::ostream& operator<<(std::ostream& out, const point&);

    struct rectangle {
      inline rectangle(const point& pos = core::point(),
                       const size& sz = core::size())
        : topleft(pos)
        , bottomright(pos.x + sz.width, pos.y + sz.height) {
      }

      inline explicit rectangle(const size& sz)
        : bottomright(sz.width, sz.height) {
      }

      inline rectangle(const point& topleft,
                       const point& bottomright)
        : topleft(topleft)
        , bottomright(bottomright) {
      }

      inline rectangle(point::type x, point::type y, size::type width, size::type height)
        : topleft(x, y)
        , bottomright(x + width, y + height) {
      }

      explicit rectangle(const rectangle_type& r);
#ifdef WIN32
      explicit rectangle(const event_param_2& p);
#endif // WIN32
#ifdef X11
      template<typename T>
      explicit rectangle(const T& rhs)
        : topleft(rhs.x, rhs.y)
        , bottomright(rhs.x + rhs.width, rhs.y + rhs.height)
      {}
#endif // X11

      operator rectangle_type() const;

      inline bool is_inside(const point& p) const {
        return (p >= topleft) && (p < bottomright);
      }

      inline point position() const {
        return topleft;
      }

      inline point center() const {
        return { (topleft.x + bottomright.x) / 2, (topleft.y + bottomright.y) / 2 };
      }

      inline core::size size() const {
        return{ (size::type)(bottomright.x - topleft.x), (size::type)(bottomright.y - topleft.y) };
      }

      inline bool operator== (const rectangle& rhs) const {
        return (topleft == rhs.topleft) && (bottomright == rhs.bottomright);
      }

      inline bool operator!= (const rectangle& rhs) const {
        return !operator==(rhs);
      }

      void setSize(const core::size& sz);

      core::point topleft;
      core::point bottomright;

      static rectangle default_rectangle;
    };

    std::ostream& operator<<(std::ostream& out, const rectangle&);

  } // core

} // gui
