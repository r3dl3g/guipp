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
        , height(height_) {}

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

    struct position {
      inline position(int x_ = 0, int y_ = 0)
        : x(x_)
        , y(y_) {}

      explicit position(const point_type& pt);
      explicit position(const event_param_2& p);

      operator point_type() const;

      inline position operator+ (const size& s) const {
        return{ x + s.width, y + s.height };
      }

      inline position operator- (const size& s) const {
        return{ x - s.width, y - s.height };
      }

      inline size operator- (const position& rhs) const {
        return{ x - rhs.x, y - rhs.y };
      }

      inline bool operator< (const position& rhs) const {
        return (x < rhs.x) && (y < rhs.y);
      }

      inline bool operator<= (const position& rhs) const {
        return (x <= rhs.x) && (y <= rhs.y);
      }

      inline bool operator>(const position& rhs) const {
        return (x > rhs.x) && (y > rhs.y);
      }

      inline bool operator>= (const position& rhs) const {
        return (x >= rhs.x) && (y >= rhs.y);
      }

      int x;
      int y;
    };

    std::ostream& operator<<(std::ostream& out, const position&);

    struct rectangle {
      inline rectangle(const position& pos = core::position(),
                       const size& sz = core::size())
        : topleft(pos)
        , bottomright(pos.x + sz.width, pos.y + sz.height) {}

      inline explicit rectangle(const size& sz)
        : bottomright(sz) {}

      inline rectangle(const position& topleft,
                       const position& bottomright)
        : topleft(topleft)
        , bottomright(bottomright.x - topleft.x, bottomright.y - topleft.y) {}

      inline rectangle(int x, int y, int width, int height)
        : topleft(x, y)
        , bottomright(x + width, y + height) {}

      explicit rectangle(const rectangle_type& r);
      explicit rectangle(const event_param_2& p);

      operator rectangle_type() const;

      inline bool is_inside(const position& p) const {
        return (p >= topleft) && (p < bottomright);
      }

      inline position position() const {
        return topleft;
      }

      inline size size() const {
        return{ bottomright.x - topleft.x, bottomright.y - topleft.y };
      }

      void setSize(const core::size& sz);

      core::position topleft;
      core::position bottomright;

      static rectangle default;
    };

    std::ostream& operator<<(std::ostream& out, const rectangle&);

  } // core

} // gui
