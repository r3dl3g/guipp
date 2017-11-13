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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/point.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T>
    struct basic_rectangle {
      typedef T type;
      typedef basic_rectangle self;
      typedef basic_point<T> point_t;
      typedef basic_size<T> size_t;

      static const self zero;
      static const self def;

      basic_rectangle (const point_t& pos = point_t::zero,
                       const size_t& sz = size_t::zero);


      basic_rectangle (const point_t& topleft,
                       const point_t& bottomright);

      basic_rectangle (type x, type y, type width, type height);

      explicit basic_rectangle (const size_t& sz);
      explicit basic_rectangle (const os::rectangle& r);

      operator os::rectangle () const;

      os::rectangle os () const;

      const point_t& position () const;
      const size_t& size () const;

      const point_t& top_left () const;
      point_t top_right () const;
      point_t bottom_right () const;
      point_t bottom_left () const;

      type x () const;
      type y () const;
      type width () const;
      type height () const;
      type x2 () const;
      type y2 () const;

      void x (type x);
      void y (type y);
      void x2 (type x);
      void y2 (type y);
      void height (type height);
      void width (type width);

      bool empty () const;
      bool is_inside (const point_t& p) const;

      void top_left (const point_t& pt);
      void bottom_right (const point_t& pt);

      type center_x () const;
      type center_y () const;
      point_t center () const;

      self with_pos (const point_t& pt) const;
      self with_size (const size_t& sz) const;
      self with_x (const type x) const;
      self with_y (const type y) const;
      self with_width (const type w) const;
      self with_height (const type h) const;

      self grown (const size_t& s) const;
      self shrinked (const size_t& s) const;

      self& grow (const size_t& s);
      self& shrink (const size_t& s);

      bool operator== (const self& rhs) const;
      bool operator!= (const self& rhs) const;

      self& operator= (const self& s);
      self& operator+= (const size_t& s);
      self& operator-= (const size_t& s);
      self& operator+= (const point_t& pt);
      self& operator-= (const point_t& pt);
      self& operator+= (const self& r);
      self& operator-= (const self& r);

      self operator+ (const size_t& s) const;
      self operator- (const size_t& s) const;
      self operator+ (const point_t& pt) const;
      self operator- (const point_t& pt) const;
      self operator+ (const self& r) const;
      self operator- (const self& r) const;

      void move (const point_t& pt);
      void move (const size_t& sz);

      void move_x (type dx);
      void move_y (type dy);

      void move_to_x (type x);
      void move_to_y (type y);

      void set_size (const size_t& sz);
      void set_position (const point_t& pt);
      void set_center (const point_t& pt);

      inline os::point_type os_x () const;
      inline os::point_type os_y () const;
      inline os::point_type os_x2 () const;
      inline os::point_type os_y2 () const;
      inline os::size_type os_width () const;
      inline os::size_type os_height () const;

      // union
      self& operator&= (const self& rhs);

      // intersection
      self& operator|= (const self& rhs);

    private:
      point_t pos;
      size_t sz;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out, const basic_rectangle<T>&);

    // --------------------------------------------------------------------------
    typedef basic_rectangle<float> rectangle;
    typedef basic_rectangle<uint32_t> uint32_rect;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui

#include <gui/core/rectangle.inl>
