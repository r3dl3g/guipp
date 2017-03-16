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

    struct point;

    struct size {
      typedef IF_WIN32(LONG) IF_X11(unsigned short) type;
      static const size zero;

      inline size (type width = 0,
                   type height = 0) {
        sz = {width, height};
      }

      explicit size (const size_type& sz);

      explicit size (const point_type& pt);

#ifdef WIN32
      explicit size(const event_param_1& p);
      explicit size(const event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit size (const T& rhs)
        : sz({type(rhs.width), type(rhs.height)}) {}

#endif // X11

      operator size_type () const;

      operator point_type () const;

      inline bool operator== (const size& rhs) const {
        return (width() == rhs.width()) && (height() == rhs.height());
      }

      inline bool operator!= (const size& rhs) const {
        return !operator==(rhs);
      }

      inline size operator+ (const size& rhs) const {
        return {type(width() + rhs.width()), type(height() + rhs.height())};
      }

      inline size operator- (const size& rhs) const {
        return {type(width() - rhs.width()), type(height() - rhs.height())};
      }

      inline type width () const {
        return sz.cx;
      }

      inline type height () const {
        return sz.cy;
      }

      inline void width (type w) {
        sz.cx = w;
      }

      inline void height (type h) {
        sz.cy = h;
      }

    private:
      size_type sz;
    };

    std::ostream& operator<< (std::ostream& out,
                              const size&);

    struct point {
      typedef IF_WIN32(LONG) IF_X11(short) type;
      static const point zero;

      inline point (type x = 0,
                    type y = 0) {
        pt = {x, y};
      }

      explicit point (const point_type& pt);

#ifdef WIN32
      explicit point(const event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit point (const T& rhs) {
        pt = {type(rhs.x), type(rhs.y)};
      }

#endif // X11

      operator point_type () const;

      inline point operator+ (const size& s) const {
        return {type(x() + s.width()), type(y() + s.height())};
      }

      inline point operator- (const size& s) const {
        return {type(x() - s.width()), type(y() - s.height())};
      }

      inline point& operator+= (const size& s) {
        operator=(*this + s);
        return *this;
      }

      inline point& operator-= (const size& s) {
        operator=(*this - s);
        return *this;
      }

      inline point operator- (const point& pt) const {
        return { type(x() - pt.x()), type(y() - pt.y()) };
      }

      inline point operator+ (const point& pt) const {
        return { type(x() + pt.x()), type(y() + pt.y()) };
      }

      inline point& operator+= (const point& pt) {
        x(x() + pt.x());
        y(y() + pt.y());
        return *this;
      }

      inline point& operator-= (const point& pt) {
        x(x() - pt.x());
        y(y() - pt.y());
        return *this;
      }

      inline void move (const point& pt) {
        x(x() + pt.x());
        y(y() + pt.y());
      }

      inline void move_x (type dx) {
        x(x() + dx);
      }

      inline void move_y (type dy) {
        y(y() + dy);
      }

      inline bool operator== (const point& rhs) const {
        return (x() == rhs.x()) && (y() == rhs.y());
      }

      inline bool operator!= (const point& rhs) const {
        return !operator==(rhs);
      }

      inline bool operator< (const point& rhs) const {
        return (x() < rhs.x()) && (y() < rhs.y());
      }

      inline bool operator<= (const point& rhs) const {
        return (x() <= rhs.x()) && (y() <= rhs.y());
      }

      inline bool operator> (const point& rhs) const {
        return (x() > rhs.x()) && (y() > rhs.y());
      }

      inline bool operator>= (const point& rhs) const {
        return (x() >= rhs.x()) && (y() >= rhs.y());
      }

      inline type x () const {
        return pt.x;
      }

      inline void x (type x_) {
        pt.x = x_;
      }

      inline type y () const {
        return pt.y;
      }

      inline void y (type y_) {
        pt.y = y_;
      }

    private:
      point_type pt;
    };

    std::ostream& operator<< (std::ostream& out,
                              const point&);

    struct rectangle {
      static const rectangle zero;
      static const rectangle default_rectangle;

      inline rectangle (const point& pos = point::zero,
                        const size& sz = size::zero)
        : tl(pos), br(pos.x() + sz.width(), pos.y() + sz.height()) {
      }

      inline explicit rectangle (const size& sz)
        : br(sz.width(), sz.height()) {
      }

      inline rectangle (const point& topleft,
                        const point& bottomright)
        : tl(topleft), br(bottomright) {
      }

      inline rectangle (point::type x,
                        point::type y,
                        size::type width,
                        size::type height)
        : tl(x, y), br(x + width, y + height) {
      }

      explicit rectangle (const rectangle_type& r);

#ifdef WIN32
      explicit rectangle(const event_param_2& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit rectangle (const T& rhs)
        : tl(rhs.x, rhs.y), br(rhs.x + rhs.width, rhs.y + rhs.height) {}

#endif // X11

      operator rectangle_type () const;

      inline bool is_inside (const point& p) const {
        return (p >= tl) && (p < br);
      }

      inline point position () const {
        return tl;
      }

      inline rectangle grown (const core::size& sz) const {
        return {tl - sz, br + sz};
      }

      inline rectangle shrinked (const core::size& sz) const {
        return {tl + sz, br - sz};
      }

      inline rectangle& grow (const core::size& sz) {
        tl -= sz;
        br += sz;
        return *this;
      }

      inline rectangle& shrink (const core::size& sz) {
        tl += sz;
        br -= sz;
        return *this;
      }

      inline point center () const {
        return {point::type((tl.x() + br.x()) / 2), point::type((tl.y() + br.y()) / 2)};
      }

      inline core::size size () const {
        return {(core::size::type)(br.x() - tl.x()), (core::size::type)(br.y() - tl.y())};
      }

      inline rectangle operator- (const core::size& sz) const {
        return {tl, br - sz};
      }

      inline rectangle& operator-= (const core::size& sz) {
        br -= sz;
        return *this;
      }

      inline rectangle operator+ (const core::size& sz) const {
        return {tl, br + sz};
      }

      inline rectangle& operator+= (const core::size& sz) {
        br += sz;
        return *this;
      }

      inline rectangle operator- (const point& pt) const {
        return {tl + pt, br};
      }

      inline rectangle& operator-= (const point& pt) {
        tl -= pt;
        return *this;
      }

      inline rectangle operator+ (const point& pt) const {
        return {tl + pt, br};
      }

      inline rectangle& operator+= (const point& pt) {
        tl += pt;
        return *this;
      }

      inline bool operator== (const rectangle& rhs) const {
        return (tl == rhs.tl) && (br == rhs.br);
      }

      inline bool operator!= (const rectangle& rhs) const {
        return !operator==(rhs);
      }

      void move (const point& pt);

      void move (const core::size& sz);

      void move_x (point::type dx);

      void move_y (point::type dy);

      void set_size (const core::size& sz);

      const point& top_left () const {
        return tl;
      }

      void top_left (const point& pt) {
        tl = pt;
      }

      const point& bottom_right () const {
        return br;
      }

      void bottom_right (const point& pt) {
        br = pt;
      }

      inline point::type x () const {
        return tl.x();
      }

      inline point::type y () const {
        return tl.y();
      }

      inline point::type x2 () const {
        return br.x();
      }

      inline point::type y2 () const {
        return br.y();
      }

      inline size::type width () const {
        return size::type(br.x() - tl.x());
      }

      inline size::type height () const {
        return size::type(br.y() - tl.y());
      }

      void x (point::type x);

      void y (point::type y);

      void height (size::type height);

      void width (size::type width);

      // union
      rectangle& operator&= (const rectangle& rhs);
      
      // intersection
      rectangle& operator|= (const rectangle& rhs);

    private:
      point tl;
      point br;
    };

    std::ostream& operator<< (std::ostream& out,
                              const rectangle&);

  } // core

} // gui
