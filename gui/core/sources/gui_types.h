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

      inline point& operator+= (const size& s) {
        x += (type)s.width;
        y += (type)s.height;
        return *this;
      }

      inline point& operator-= (const size& s) {
        x -= (type)s.width;
        y -= (type)s.height;
        return *this;
      }

      inline size operator- (const point& pt) const {
        return { (size::type)(x - pt.x), (size::type)(y - pt.y) };
      }

      inline point operator+ (const point& pt) const {
        return { x + pt.x, y + pt.y };
      }

      inline point& operator+= (const point& pt) {
        x += pt.x;
        y += pt.x;
        return *this;
      }

      inline point& operator-= (const point& pt) {
        x -= pt.x;
        y -= pt.x;
        return *this;
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
        : top_left(pos)
        , bottom_right(pos.x + sz.width, pos.y + sz.height) {
      }

      inline explicit rectangle(const size& sz)
        : bottom_right(sz.width, sz.height) {
      }

      inline rectangle(const point& topleft,
                       const point& bottomright)
        : top_left(topleft)
        , bottom_right(bottomright) {
      }

      inline rectangle(point::type x, point::type y, size::type width, size::type height)
        : top_left(x, y)
        , bottom_right(x + width, y + height) {
      }

      explicit rectangle(const rectangle_type& r);
#ifdef WIN32
      explicit rectangle(const event_param_2& p);
#endif // WIN32
#ifdef X11
      template<typename T>
      explicit rectangle(const T& rhs)
        : top_left(rhs.x, rhs.y)
        , bottom_right(rhs.x + rhs.width, rhs.y + rhs.height)
      {}
#endif // X11

      operator rectangle_type() const;

      inline bool is_inside(const point& p) const {
        return (p >= top_left) && (p < bottom_right);
      }

      inline point position() const {
        return top_left;
      }

      inline rectangle grown (const size& sz) const {
        return { top_left - sz, bottom_right + sz };
      }

      inline rectangle& grow (const size& sz) {
        top_left -= sz;
        bottom_right += sz;
        return *this;
      }

      inline point center() const {
        return { (top_left.x + bottom_right.x) / 2, (top_left.y + bottom_right.y) / 2 };
      }

      inline core::size size() const {
        return{ (size::type)(bottom_right.x - top_left.x), (size::type)(bottom_right.y - top_left.y) };
      }

      inline rectangle operator- (const core::size& sz) const {
        return { top_left, bottom_right - sz };
      }

      inline rectangle& operator-= (const core::size& sz) {
        bottom_right -= sz;
        return *this;
      }

      inline rectangle operator+ (const core::size& sz) const {
        return { top_left, bottom_right + sz };
      }

      inline rectangle& operator+= (const core::size& sz) {
        bottom_right += sz;
        return *this;
      }

      inline rectangle operator- (const point& pt) const {
        return { top_left + pt, bottom_right };
      }

      inline rectangle& operator-= (const core::point& pt) {
        top_left -= pt;
        return *this;
      }

      inline rectangle operator+ (const core::point& pt) const {
        return { top_left + pt, bottom_right };
      }

      inline rectangle& operator+= (const core::point& pt) {
        top_left += pt;
        return *this;
      }

      inline bool operator== (const rectangle& rhs) const {
        return (top_left == rhs.top_left) && (bottom_right == rhs.bottom_right);
      }

      inline bool operator!= (const rectangle& rhs) const {
        return !operator==(rhs);
      }

      void move (const core::size& sz);

      void set_size (const core::size& sz);

      void set_height (core::size::type height);

      void set_width (core::size::type width);

      core::point top_left;
      core::point bottom_right;

      static rectangle default_rectangle;
    };

    std::ostream& operator<<(std::ostream& out, const rectangle&);

  } // core

} // gui
