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
#include <gui/core/guidefs.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    typedef float size_type;

    struct point;

    template<typename T>
    typename std::enable_if<std::is_unsigned<T>::value, T>::type os_dimension_cast (size_type v) {
      return static_cast<T>(std::max<size_type>(v, 0));
    }

    template<typename T>
    typename std::enable_if<std::is_signed<T>::value, T>::type os_dimension_cast (size_type v) {
      return static_cast<T>(v);
    }

    // --------------------------------------------------------------------------
    struct size {
      typedef size_type type;

      static const size zero;
      static const size one;
      static const size two;

      size ();
      size (type width, type height);

      explicit size (type i);
      explicit size (const os::size& sz);
      explicit size (const os::point& pt);
      explicit size (const os::rectangle& r);

      operator os::size () const;
      operator os::point() const;

      os::size os () const;

      bool empty () const;
      void clear (type v = 0);

      bool operator== (const size& rhs) const;
      bool operator!= (const size& rhs) const;

      bool operator< (const size& rhs) const;
      bool operator<= (const size& rhs) const;
      bool operator> (const size& rhs) const;
      bool operator>= (const size& rhs) const;

      size operator+ (const size& rhs) const;
      size operator- (const size& rhs) const;
      size operator* (size_type f) const;
      size operator/ (size_type f) const;

      size& operator+= (const size& s);
      size& operator-= (const size& s);
      size& operator*= (size_type f);
      size& operator/= (size_type f);

      size operator- () const;

      type width () const;
      type height () const;

      os::size_type os_width () const;
      os::size_type os_height () const;

      void width (type w);
      void height (type h);

    private:
      struct data {
        data (type w = 0, type h = 0);

        type w;
        type h;
      } data;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const size&);

    // --------------------------------------------------------------------------
    typedef float point_type;

    // --------------------------------------------------------------------------
    struct point {
      typedef point_type type;

      static const point zero;
      static const point undefined;

      inline point ();
      inline point (type x, type y);

      explicit inline point (type i);
      explicit point (const os::point& pt);
      explicit point (const os::rectangle& r);

      operator os::point () const;
      os::point os () const;

      type x () const;
      type y () const;

      os::point_type os_x () const;
      os::point_type os_y () const;

      void x (type v);
      void y (type v);

      bool operator== (const point& rhs) const;
      bool operator!= (const point& rhs) const;
      bool operator< (const point& rhs) const;
      bool operator> (const point& rhs) const;
      bool operator<= (const point& rhs) const;
      bool operator>= (const point& rhs) const;

      point& operator+= (const size& s);
      point& operator-= (const size& s);
      point& operator+= (const point& pt);
      point& operator-= (const point& pt);
      point& operator*= (point_type f);
      point& operator/= (point_type f);

      point operator+ (const size& s) const;
      point operator- (const size& s) const;

      size operator- (const point& pt) const;
      point operator+ (const point& pt) const;
      point operator* (point_type f) const;
      point operator/ (point_type f) const;
      point operator- () const;

      void move (const point& pt);
      void move_x (type dx);
      void move_y (type dy);

    private:
      struct data {
        data (type x = 0, type y = 0);

        type x;
        type y;
      } data;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const point&);

    // --------------------------------------------------------------------------
    struct rectangle {
      static const rectangle zero;
      static const rectangle def;

      rectangle (const point& pos = point::zero,
                 const core::size& sz = core::size::zero);


      rectangle (const point& topleft,
                 const point& bottomright);

      rectangle (point::type x,
                 point::type y,
                 size::type width,
                 size::type height);

      explicit rectangle (const core::size& sz);
      explicit rectangle (const os::rectangle& r);

      operator os::rectangle () const;

      os::rectangle os () const;

      const point& position () const;
      const core::size& size () const;

      const point& top_left () const;
      point top_right () const;
      point bottom_right () const;
      point bottom_left () const;

      point::type x () const;
      point::type y () const;
      size::type width () const;
      size::type height () const;
      point::type x2 () const;
      point::type y2 () const;

      void x (point::type x);
      void y (point::type y);
      void x2 (point::type x);
      void y2 (point::type y);
      void height (size::type height);
      void width (size::type width);

      bool empty () const;
      bool is_inside (const point& p) const;

      void top_left (const point& pt);
      void bottom_right (const point& pt);

      point::type center_x () const;
      point::type center_y () const;
      point center () const;

      rectangle with_pos (const core::point& pt) const;
      rectangle with_size (const core::size& sz) const;
      rectangle with_x (const core::size::type x) const;
      rectangle with_y (const core::size::type y) const;
      rectangle with_width (const core::size::type w) const;
      rectangle with_height (const core::size::type h) const;

      rectangle grown (const core::size& s) const;
      rectangle shrinked (const core::size& s) const;

      rectangle& grow (const core::size& s);
      rectangle& shrink (const core::size& s);

      bool operator== (const rectangle& rhs) const;
      bool operator!= (const rectangle& rhs) const;

      rectangle& operator+= (const core::size& s);
      rectangle& operator-= (const core::size& s);
      rectangle& operator+= (const point& pt);
      rectangle& operator-= (const point& pt);
      rectangle& operator+= (const rectangle& r);
      rectangle& operator-= (const rectangle& r);

      rectangle operator+ (const core::size& s) const;
      rectangle operator- (const core::size& s) const;
      rectangle operator+ (const point& pt) const;
      rectangle operator- (const point& pt) const;
      rectangle operator+ (const rectangle& r) const;
      rectangle operator- (const rectangle& r) const;

      void move (const point& pt);
      void move (const core::size& sz);

      void move_x (point::type dx);
      void move_y (point::type dy);

      void move_to_x (point::type x);
      void move_to_y (point::type y);

      void set_size (const core::size& sz);
      void set_position (const core::point& pt);
      void set_center (const core::point& pt);

      inline os::point_type os_x () const;
      inline os::point_type os_y () const;
      inline os::point_type os_x2 () const;
      inline os::point_type os_y2 () const;
      inline os::size_type os_width () const;
      inline os::size_type os_height () const;

      // union
      rectangle& operator&= (const rectangle& rhs);

      // intersection
      rectangle& operator|= (const rectangle& rhs);

    private:
      point pos;
      core::size sz;
    };

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const rectangle&);

    // --------------------------------------------------------------------------
    template<typename T>
    struct position {
      typedef T type;
      static constexpr type invalid = std::numeric_limits<type>::min();
      static constexpr type max = std::numeric_limits<type>::max();

      static const position zero;
      static const position end;

      position ();

      template<typename C, typename R>
      position (C c, R r);

      bool is_cell (std::size_t c, std::size_t r) const;
      bool is_column (std::size_t c) const;
      bool is_row (std::size_t r) const;

      bool operator== (const position& rhs) const;
      bool operator!= (const position& rhs) const;
      bool operator> (const position& rhs) const;
      bool operator< (const position& rhs) const;
      bool operator>= (const position& rhs) const;
      bool operator<= (const position& rhs) const;

      bool is_empty () const;
      bool is_valid () const;
      void clear ();

      position operator+ (const position& rhs) const;
      position operator- (const position& rhs) const;

      type column;
      type row;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out, const position<T>& p);

    // --------------------------------------------------------------------------
    template<typename T>
    struct range {
      typedef T type;

      type first;
      type last;

      range (type v = type());
      range (type f, type l);

      bool is_inside (type i) const;
      void sort ();
      bool empty () const;
      void clear ();
    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    inline size::size ()
    {}

    inline size::size (type i)
      : data(i, i)
    {}

    inline size::size (type width, type height)
      : data(width, height)
    {}

    inline auto size::width() const->type {
      return data.w;
    }

    inline auto size::height() const->type {
      return data.h;
    }

    inline os::size_type size::os_width () const {
      return os_dimension_cast<os::size_type>(data.w);
    }

    inline os::size_type size::os_height () const {
      return os_dimension_cast<os::size_type>(data.h);
    }

    inline void size::width (type w) {
      data.w = w;
    }

    inline void size::height (type h) {
      data.h = h;
    }

    // --------------------------------------------------------------------------
    inline point::point ()
    {}

    inline point::point (type i)
      : data(i, i)
    {}

    inline point::point (type x, type y)
      : data(x, y)
    {}

    inline auto point::x() const->type {
      return data.x;
    }

    inline void point::x (type v) {
      data.x = v;
    }

    inline auto point::y() const->type {
      return data.y;
    }

    inline void point::y (type v) {
      data.y = v;
    }

    inline os::point_type point::os_x () const {
      return os_dimension_cast<os::point_type>(data.x);
    }

    inline os::point_type point::os_y () const {
      return os_dimension_cast<os::point_type>(data.y);
    }

    // --------------------------------------------------------------------------
    inline rectangle::rectangle (const point& pos,
                                 const core::size& sz)
      : pos(pos)
      , sz(sz)
    {}

    inline rectangle::rectangle (const core::size& sz)
      : sz(sz)
    {}

    inline rectangle::rectangle (const point& topleft,
                                 const point& bottomright)
      : pos(topleft)
      , sz(bottomright - topleft)
    {}

    inline rectangle::rectangle (point::type x,
                                 point::type y,
                                 size::type width,
                                 size::type height)
      : pos(x, y)
      , sz(width, height)
    {}

    inline const point& rectangle::position () const {
      return pos;
    }

    inline const core::size& rectangle::size () const {
      return sz;
    }

    inline const point& rectangle::top_left () const {
      return pos;
    }

    inline point rectangle::top_right () const {
      return {x2(), y()};
    }

    inline point rectangle::bottom_right () const {
      return {x2(), y2()};
    }

    inline point rectangle::bottom_left () const {
      return {x(), y2()};
    }

    inline point::type rectangle::x () const {
      return pos.x();
    }

    inline point::type rectangle::y () const {
      return pos.y();
    }

    inline size::type rectangle::width () const {
      return sz.width();
    }

    inline size::type rectangle::height () const {
      return sz.height();
    }

    inline point::type rectangle::x2 () const {
      return x() + width();
    }

    inline point::type rectangle::y2 () const {
      return y() + height();
    }

    inline void rectangle::set_position (const core::point& pt) {
      pos = pt;
    }

    inline void rectangle::set_size (const core::size& s) {
      sz = s;
    }

    inline void rectangle::x (point::type x) {
      pos.x(x);
    }

    inline void rectangle::y (point::type y) {
      pos.y(y);
    }

    inline void rectangle::x2 (point::type new_x) {
      sz.width(new_x - x());
    }

    inline void rectangle::y2 (point::type new_y) {
      sz.height(new_y - y());
    }

    inline void rectangle::width (core::size::type width) {
      sz.width(width);
    }

    inline void rectangle::height (core::size::type height) {
      sz.height(height);
    }

    inline void rectangle::top_left (const point& pt) {
      pos = pt;
    }

    inline void rectangle::bottom_right (const point& pt) {
      sz.width(pt.x() - pos.x());
      sz.height(pt.y() - pos.y());
    }

    inline os::point_type rectangle::os_x () const {
      return pos.os_x();
    }

    inline os::point_type rectangle::os_y () const {
      return pos.os_y();
    }

    inline os::point_type rectangle::os_x2 () const {
      return os_dimension_cast<os::point_type>(x2());
    }

    inline os::point_type rectangle::os_y2 () const {
      return os_dimension_cast<os::point_type>(y2());
    }

    inline os::size_type rectangle::os_width () const {
      return size().os_width();
    }

    inline os::size_type rectangle::os_height () const {
      return size().os_height();
    }

    // --------------------------------------------------------------------------
    template<typename T>
    const position<T> position<T>::zero = {0, 0};

    template<typename T>
    const position<T> position<T>::end = {max, max};

    template<typename T>
    inline position<T>::position ()
      : column(invalid)
      , row(invalid)
    {}

    template<typename T>
    template<typename C, typename R>
    inline position<T>::position (C c, R r)
      : column(static_cast<type>(c))
      , row(static_cast<type>(r))
    {}

    template<typename T>
    inline bool position<T>::is_cell (std::size_t c, std::size_t r) const {
      return (column == static_cast<type>(c)) && (row == static_cast<type>(r));
    }

    template<typename T>
    inline bool position<T>::is_column (std::size_t c) const {
      return (column == static_cast<type>(c)) && (row < 0);
    }

    template<typename T>
    inline bool position<T>::is_row (std::size_t r) const {
      return (row == static_cast<type>(r)) && (column < 0);
    }

    template<typename T>
    inline bool position<T>::operator== (const position& rhs) const {
      return (column == rhs.column) && (row == rhs.row);
    }

    template<typename T>
    inline bool position<T>::operator!= (const position& rhs) const {
      return !operator== (rhs);
    }

    template<typename T>
    inline bool position<T>::operator> (const position& rhs) const {
      return (row > rhs.row) || ((row == rhs.row) && (column > rhs.column));
    }

    template<typename T>
    inline bool position<T>::operator>= (const position& rhs) const {
      return (row > rhs.row) || ((row == rhs.row) && (column >= rhs.column));
    }

    template<typename T>
    inline bool position<T>::operator< (const position& rhs) const {
      return (row < rhs.row) || ((row == rhs.row) && (column < rhs.column));
    }

    template<typename T>
    inline bool position<T>::operator<= (const position& rhs) const {
      return (row < rhs.row) || ((row == rhs.row) && (column <= rhs.column));
    }

    template<typename T>
    inline bool position<T>::is_empty () const {
      return (column != invalid) && (row != invalid);
    }

    template<typename T>
    inline bool position<T>::is_valid () const {
      return (column != invalid) && (row != invalid);
    }

    template<typename T>
    inline void position<T>::clear () {
      column = invalid;
      row = invalid;
    }

    template<typename T>
    inline auto position<T>::operator+ (const position& rhs) const->position {
      return position(column + rhs.column, row + rhs.row);
    }

    template<typename T>
    inline auto position<T>::operator- (const position& rhs) const->position {
      return position(column - rhs.column, row - rhs.row);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline std::ostream& operator<< (std::ostream& out,
                                     const position<T>& p) {
      out << p.column << ", " << p.row;
      return out;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    range<T>::range (type v)
      : first(v)
      , last(v)
    {}

    template<typename T>
    range<T>::range (type f, type l)
      : first(f)
      , last(l)
    {}

    template<typename T>
    bool range<T>::is_inside (type i) const {
      return (i > first) && (i < last);
    }

    template<typename T>
    void range<T>::sort () {
      if (first > last) {
        std::swap(first, last);
      }
    }

    template<typename T>
    bool range<T>::empty () const {
      return first >= last;
    }

    template<typename T>
    void range<T>::clear () {
      first = last = type();
    }

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out,
                              const range<T>& r) {
      out << r.first << ":" << r.last;
      return out;
    }

    // --------------------------------------------------------------------------
  }  // namespace core

}  // namespace gui
