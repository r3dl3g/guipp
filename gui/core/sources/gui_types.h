/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <limits>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    typedef float size_type;

    struct point;

    inline os::size_type convert_size_dimension (size_type v) {
#ifdef WIN32
      return static_cast<os::size_type>(v);
#endif // WIN32
#ifdef X11
      return static_cast<os::size_type>(std::max(v, size_type(0)));
#endif // X11
    }

    // --------------------------------------------------------------------------
    struct size {
      typedef size_type type;

      static const size zero;
      static const size one;

      inline size () 
      {}

      explicit inline size (type i) 
        : data(i, i)
      {}

      inline size (type width, type height)
        : data(width, height)
      {}

      explicit size (const os::size& sz);

      explicit size (const os::point& pt);

      explicit size (const os::rectangle& r);

#ifdef WIN32
      explicit size(const os::win32::wParam& p);
      explicit size(const os::win32::lParam& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit size (const T& rhs)
        : data(static_cast<type>(rhs.width),
               static_cast<type>(rhs.height))
      {}

#endif // X11

      operator os::size () const;

      operator os::point () const;

      os::size os () const;

      inline bool empty () const {
        return (data.w <= 0) || (data.h <= 0);
      }

      void clear (type v = 0);

      inline bool operator== (const size& rhs) const {
        return (data.w == rhs.data.w) && (data.h == rhs.data.h);
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

      inline size operator* (size_type f) const {
        return {type(width() * f), type(height() * f)};
      }

      inline size operator/ (size_type f) const {
        return {type(width() / f), type(height() / f)};
      }

      inline size& operator+= (const size& s) {
        data.w += s.data.w;
        data.h += s.data.h;
        return *this;
      }

      inline size& operator-= (const size& s) {
        data.w -= s.data.w;
        data.h -= s.data.h;
        return *this;
      }

      inline size& operator*= (size_type f) {
        data.w *= f;
        data.h *= f;
        return *this;
      }

      inline size& operator/= (size_type f) {
        data.w /= f;
        data.h /= f;
        return *this;
      }

      inline size operator- () const {
        return {-width(), -height()};
      }

      inline type width () const {
        return data.w;
      }

      inline type height () const {
        return data.h;
      }

      inline os::size_type os_width () const {
        return convert_size_dimension(data.w);
      }

      inline os::size_type os_height () const {
        return convert_size_dimension(data.h);
      }

      inline void width (type w) {
        data.w = w;
      }

      inline void height (type h) {
        data.h = h;
      }

    private:
      struct data {
        data (type w = 0, type h = 0)
          : w(w)
          , h(h)
        {}

        type w;
        type h;
      } data;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out,
                              const size&);

    // --------------------------------------------------------------------------
    typedef float point_type;

    // --------------------------------------------------------------------------
    struct point {
      typedef point_type type;
      static const point zero;
      static const point one;
      static const point undefined;

      inline point ()
      {}

      explicit inline point (type i)
        : data(i, i)
      {}

      inline point (type x, type y)
        : data(x, y)
      {}

      explicit point (const os::point& pt);

      explicit point (const os::rectangle& r);

#ifdef WIN32
      explicit point(const os::win32::lParam& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit point (const T& rhs)
        : data(static_cast<type>(rhs.x),
               static_cast<type>(rhs.y))
      {}

#endif // X11

      operator os::point () const;
      os::point os () const;

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

      inline point operator* (point_type f) const {
        return { type(x() * f), type(y() * f) };
      }

      inline point operator/ (point_type f) const {
        return { type(x() / f), type(y() / f) };
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

      inline point& operator*= (point_type f) {
        x(x() * f);
        y(y() * f);
        return *this;
      }

      inline point& operator/= (point_type f) {
        x(x() / f);
        y(y() / f);
        return *this;
      }

      inline point operator- () const {
        return {-x(), -y()};
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
        return data.x;
      }

      inline void x (type v) {
        data.x = v;
      }

      inline type y () const {
        return data.y;
      }

      inline void y (type v) {
        data.y = v;
      }

      inline os::point_type os_x () const {
        return static_cast<os::point_type>(data.x);
      }

      inline os::point_type os_y () const {
        return static_cast<os::point_type>(data.y);
      }

    private:
      struct data {
        data (type x = 0, type y = 0)
          : x(x)
          , y(y)
        {}

        type x;
        type y;
      } data;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out,
                              const point&);

    // --------------------------------------------------------------------------
    struct rectangle {
      static const rectangle zero;
      static const rectangle def;

      inline rectangle (const point& pos = point::zero,
                        const size& sz = size::zero)
        : tl(pos)
        , br(pos.x() + sz.width(), pos.y() + sz.height())
      {}

      inline explicit rectangle (const size& sz)
        : br(sz.width(), sz.height())
      {}

      inline rectangle (const point& topleft,
                        const point& bottomright)
        : tl(topleft)
        , br(bottomright)
      {}

      inline rectangle (point::type x,
                        point::type y,
                        size::type width,
                        size::type height)
        : tl(x, y)
        , br(x + width, y + height)
      {}

      explicit rectangle (const os::rectangle& r);

#ifdef WIN32
      explicit rectangle(const os::win32::lParam& p);
#endif // WIN32
#ifdef X11

      template<typename T>
      explicit rectangle (const T& rhs)
        : rectangle(core::point(rhs), core::size(rhs))
      {}

#endif // X11

      operator os::rectangle () const;

      os::rectangle os () const;

      inline bool empty () const {
        return (tl.x() >= br.x()) || (tl.y() >= br.y());
      }

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

      inline point::type center_x () const {
        return (tl.x() + br.x()) / point::type(2);
      }

      inline point::type center_y () const {
        return (tl.y() + br.y()) / point::type(2);
      }

      inline core::size size () const {
        return {(core::size::type)(br.x() - tl.x()), (core::size::type)(br.y() - tl.y())};
      }

      inline rectangle with_width (const core::size::type w) const {
        return {tl, core::point(tl.x() + w, br.y())};
      }

      inline rectangle with_height (const core::size::type h) const {
        return {tl, core::point(br.x(), tl.y() + h)};
      }

      inline rectangle with_size (const core::size& sz) const {
        return {tl, tl + sz};
      }

      inline rectangle with_x (const core::size::type x) const {
        return {core::point(x, tl.y()), br};
      }

      inline rectangle with_y (const core::size::type y) const {
        return {core::point(tl.x(), y), br};
      }

      inline rectangle with_pos (const core::point& pt) const {
        return {pt, pt + size()};
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

      inline rectangle operator- (const rectangle& r) const {
        return {tl - r.tl, size() - r.size()};
      }

      inline rectangle& operator-= (const rectangle& r) {
        operator= (*this - r);
        return *this;
      }

      inline rectangle operator+ (const rectangle& r) const {
        return {tl + r.tl, size() + r.size()};
      }

      inline rectangle& operator+= (const rectangle& r) {
        operator= (*this + r);
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

      void move_to_x (point::type x);

      void move_to_y (point::type y);

      void set_size (const core::size& sz);

      const point& top_left () const {
        return tl;
      }

      point top_right () const {
        return { br.x(), tl.y() };
      }

      const point& bottom_right () const {
        return br;
      }

      point bottom_left () const {
        return { tl.x(), br.y() };
      }

      void top_left (const point& pt) {
        tl = pt;
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

      void x2 (point::type x);

      void y2 (point::type y);

      void height (size::type height);

      void width (size::type width);

      inline os::point_type os_x () const {
        return static_cast<os::point_type>(x());
      }

      inline os::point_type os_y () const {
        return static_cast<os::point_type>(y());
      }

      inline os::point_type os_x2 () const {
        return static_cast<os::point_type>(x2());
      }

      inline os::point_type os_y2 () const {
        return static_cast<os::point_type>(y2());
      }

      inline os::size_type os_width () const {
        return convert_size_dimension(width());
      }

      inline os::size_type os_height () const {
        return convert_size_dimension(height());
      }

      // union
      rectangle& operator&= (const rectangle& rhs);

      // intersection
      rectangle& operator|= (const rectangle& rhs);

    private:
      point tl;
      point br;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out,
                              const rectangle&);

    // --------------------------------------------------------------------------
    template<typename T>
    struct position {
      typedef T type;
      static constexpr type invalid = std::numeric_limits<type>::min();
      static constexpr type max = std::numeric_limits<type>::max();

      static const position zero;
      static const position end;

      inline position ()
        : column(invalid)
        , row(invalid)
      {}

      template<typename C, typename R>
      inline position (C c, R r)
        : column(static_cast<type>(c))
        , row(static_cast<type>(r))
      {}

      inline bool is_cell (std::size_t c, std::size_t r) const {
        return (column == static_cast<type>(c)) && (row == static_cast<type>(r));
      }

      inline bool is_column (std::size_t c) const {
        return (column == static_cast<type>(c)) && (row < 0);
      }

      inline bool is_row (std::size_t r) const {
        return (row == static_cast<type>(r)) && (column < 0);
      }

      inline bool operator== (const position& rhs) const {
        return (column == rhs.column) && (row == rhs.row);
      }

      inline bool operator!= (const position& rhs) const {
        return !operator==(rhs);
      }

      inline bool operator> (const position& rhs) const {
        return (row > rhs.row) || ((row == rhs.row) && (column > rhs.column));
      }

      inline bool operator>= (const position& rhs) const {
        return (row > rhs.row) || ((row == rhs.row) && (column >= rhs.column));
      }

      inline bool operator< (const position& rhs) const {
        return (row < rhs.row) || ((row == rhs.row) && (column < rhs.column));
      }

      inline bool operator<= (const position& rhs) const {
        return (row < rhs.row) || ((row == rhs.row) && (column <= rhs.column));
      }

      inline bool is_empty () const {
        return (column != invalid) && (row != invalid);
      }

      inline bool is_valid () const {
        return (column != invalid) && (row != invalid);
      }

      inline void clear () {
        column = invalid;
        row = invalid;
      }

      inline position operator+ (const position& rhs) const {
        return position(column + rhs.column, row + rhs.row);
      }

      inline position operator- (const position& rhs) const {
        return position(column - rhs.column, row - rhs.row);
      }

      type column;
      type row;
    };

    template<typename T>
    const position<T> position<T>::zero = {0, 0};

    template<typename T>
    const position<T> position<T>::end = {max, max};

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out,
                              const position<T>& p) {
      out << p.column << ", " << p.row;
      return out;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    struct range {
      typedef T type;

      type first;
      type last;

      range (type v = type())
        : first(v)
        , last(v)
      {}

      range (type f, type l)
        : first(f)
        , last(l)
      {}

      bool is_inside (type i) const {
        return (i > first) && (i < last);
      }

      void sort () {
        if (first > last) {
          std::swap(first, last);
        }
      }

      bool empty () const {
        return first >= last;
      }

      void clear () {
        first = last = type();
      }
    };

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out,
                              const range<T>& r) {
      out << r.first << ":" << r.last;
      return out;
    }

    // --------------------------------------------------------------------------
  } // core

} // gui
