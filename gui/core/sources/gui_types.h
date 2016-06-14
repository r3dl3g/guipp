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
      inline size(int width_ = 0, int height_ = 0)
        : width(width_)
        , height(height_) {
      }

      explicit size(const size_type& sz);
      explicit size(const point_type& pt);
      explicit size(const event_param_1& p);
      explicit size(const event_param_2& p);

      operator size_type() const;
      operator point_type() const;

      int width;
      int height;
    };

    std::ostream& operator<<(std::ostream& out, const size&);

    struct point {
      inline point(int x_ = 0, int y_ = 0)
        : x(x_)
        , y(y_) {
      }

      explicit point(const point_type& pt);
      explicit point(const event_param_2& p);

      operator point_type() const;

      inline point operator+ (const size& s) const {
        return{ x + s.width, y + s.height };
      }

      inline point operator- (const size& s) const {
        return{ x - s.width, y - s.height };
      }

      inline size operator- (const point& rhs) const {
        return{ x - rhs.x, y - rhs.y };
      }

      inline bool operator< (const point& rhs) const {
        return (x < rhs.x) && (y < rhs.y);
      }

      inline bool operator<= (const point& rhs) const {
        return (x <= rhs.x) && (y <= rhs.y);
      }

      inline bool operator>(const point& rhs) const {
        return (x > rhs.x) && (y > rhs.y);
      }

      inline bool operator>= (const point& rhs) const {
        return (x >= rhs.x) && (y >= rhs.y);
      }

      int x;
      int y;
    };

    std::ostream& operator<<(std::ostream& out, const point&);

    struct rectangle {
      inline rectangle(const point& pos = core::point(),
                       const size& sz = core::size())
        : topleft(pos)
        , bottomright(pos.x + sz.width, pos.y + sz.height) {
      }

      inline explicit rectangle(const size& sz)
        : bottomright(sz) {
      }

      inline rectangle(const point& topleft,
                       const point& bottomright)
        : topleft(topleft)
        , bottomright(bottomright.x - topleft.x, bottomright.y - topleft.y) {
      }

      inline rectangle(int x, int y, int width, int height)
        : topleft(x, y)
        , bottomright(x + width, y + height) {
      }

      explicit rectangle(const rectangle_type& r);
      explicit rectangle(const event_param_2& p);

      operator rectangle_type() const;

      inline bool is_inside(const point& p) const {
        return (p >= topleft) && (p < bottomright);
      }

      inline point position() const {
        return topleft;
      }

      inline size size() const {
        return{ bottomright.x - topleft.x, bottomright.y - topleft.y };
      }

      void setSize(const core::size& sz);

      core::point topleft;
      core::point bottomright;

      static rectangle default;
    };

    std::ostream& operator<<(std::ostream& out, const rectangle&);

  } // core

} // gui
