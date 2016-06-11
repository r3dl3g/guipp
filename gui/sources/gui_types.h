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

  struct size {
    inline size(int width_ = 0, int height_ = 0)
      : width(width_)
      , height(height_)
    {}

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
      , y(y_)
    {}

    explicit position(const point_type& pt);
    explicit position(const event_param_2& p);

    operator point_type() const;

    int x;
    int y;
  };

  std::ostream& operator<<(std::ostream& out, const position&);

  struct rectangle {
    inline rectangle(const gui::position& pos = gui::position(),
                     const gui::size& sz = gui::size())
      : position(pos)
      , size(sz)
    {}

    inline rectangle(const gui::position& topleft,
                     const gui::position& bottomright)
      : position(topleft)
      , size(bottomright.x - topleft.x, bottomright.y - topleft.y)
    {}

    inline rectangle(int x, int y, int width, int height)
      : position(x, y)
      , size(width, height)
    {}

    explicit rectangle(const rectangle_type& r);
    explicit rectangle(const event_param_2& p);

    operator rectangle_type() const;

    gui::position position;
    gui::size size;

    static rectangle default;
  };

  std::ostream& operator<<(std::ostream& out, const rectangle&);

}


