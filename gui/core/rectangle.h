/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     2D point and size structure
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include "point.h"
#include <util/math_util.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T, typename S, coordinate_system C = core::coordinate_system::independent>
    struct basic_rectangle {
      typedef T point_type;
      typedef S size_type;
      typedef basic_rectangle self;
      typedef basic_point<T, C> point_t;
      typedef basic_size<S, C> size_t;

      static const self zero;
      static const self def;

      basic_rectangle (const point_t& pos = point_t::zero,
                       const size_t& sz = size_t::zero);


      basic_rectangle (const point_t& topleft,
                       const point_t& bottomright);

      basic_rectangle (point_type x, point_type y, size_type width, size_type height);

      explicit basic_rectangle (const size_t& sz);
      explicit basic_rectangle (const gui::os::rectangle& r, const context&);
      explicit basic_rectangle (const gui::os::point& p, const gui::os::size& s, const context&);
      explicit basic_rectangle (const gui::os::size& s);

//      operator gui::os::rectangle () const;

      gui::os::rectangle os (const context&) const;

      const point_t& position () const; /// top left pixel position
      const size_t& size () const;      /// count of x and y pixels

      const point_t& top_left () const; /// top left pixel position
      point_t top_right () const;       /// top inner-right pixel position
      point_t bottom_right () const;    /// inner-bottom inner-right pixel position
      point_t bottom_left () const;     /// inner-bottom left pixel position

      point_t x2y1 () const;            /// top outer-right pixel position
      point_t x2y2 () const;            /// outer-bottom outer-right pixel position
      point_t x1y2 () const;            /// outer-bottom left pixel position

      point_type x () const;            /// left
      point_type y () const;            /// top
      size_type width () const;         /// count of pixels in width
      size_type height () const;        /// count of pixels in heigh
      point_type x2 () const;           /// x + width
      point_type y2 () const;           /// y + height
      point_type right () const;        /// position of most right pixel
      point_type bottom () const;       /// position of most bottom pixel

      self& x (point_type x);
      self& y (point_type y);
      self& x2 (point_type x);
      self& y2 (point_type y);
      self& height (size_type height);
      self& width (size_type width);
      self& right (point_type x);
      self& bottom (point_type y);

      bool empty () const;
      bool is_valid () const;
      bool is_inside (const point_t& p) const;
      point_t limit_inside (const point_t& p) const;

      self& top_left (const point_t& pt);
      self& bottom_right (const point_t& pt);
      self& x2y2 (const point_t& pt);

      point_type center_x () const;
      point_type center_y () const;
      point_t center () const;
      size_type max_radius () const;

      self with_pos (const point_t& pt) const;
      self with_size (const size_t& sz) const;
      self with_x (const point_type x) const;
      self with_y (const point_type y) const;
      self with_width (const size_type w) const;
      self with_height (const size_type h) const;

      self with_horizontal (const point_type x, const size_type w) const;
      self with_vertical (const point_type y, const size_type h) const;

      self right_width (const size_type w) const;
      self bottom_height (const size_type h) const;

      self grown (const size_t& s) const;
      self grown (const size_type top, const size_type bottom, const size_type left, const size_type right) const;

      self shrinked (const size_t& s) const;
      self shrinked (const size_type top, const size_type bottom, const size_type left, const size_type right) const;

      self& grow (const size_t& s);
      self& grow (const size_type top, const size_type bottom, const size_type left, const size_type right);

      self& shrink (const size_t& s);
      self& shrink (const size_type top, const size_type bottom, const size_type left, const size_type right);

      self& square ();
      self squared () const;

      bool operator== (const self& rhs) const;
      bool operator!= (const self& rhs) const;

      self& operator= (const self& s);
      self& operator+= (const size_t& s);
      self& operator-= (const size_t& s);
      self& operator+= (const point_t& pt);
      self& operator-= (const point_t& pt);
      self& operator+= (const self& r);
      self& operator-= (const self& r);
      self& operator*= (double f);
      self& operator/= (double f);

      self operator+ (const size_t& s) const;
      self operator- (const size_t& s) const;
      self operator+ (const point_t& pt) const;
      self operator- (const point_t& pt) const;
      self operator+ (const self& r) const;
      self operator- (const self& r) const;
      self operator* (double f) const;
      self operator/ (double f) const;

      self& move (const point_t& pt);
      self& move (const size_t& sz);

      self& move_x (point_type dx);
      self& move_y (point_type dy);

      self& move_to_x (point_type x);
      self& move_to_y (point_type y);

      self dx (point_type x) const;
      self dy (point_type y) const;
      self dxy (point_type x, point_type y) const;

      self& set_size (const size_t& sz);
      self& set_position (const point_t& pt);
      self& set_center (const point_t& pt);

      self& set_horizontal (const point_type x, const size_type w);
      self& set_vertical (const point_type y, const size_type h);

      gui::os::point_type os_x (const context&) const;
      gui::os::point_type os_y (const context&) const;
      gui::os::point_type os_x2 (const context&) const;
      gui::os::point_type os_y2 (const context&) const;
      gui::os::size_type os_width () const;
      gui::os::size_type os_height () const;
      gui::os::point_type os_right (const context&) const;
      gui::os::point_type os_bottom (const context&) const;

      // intersection
      self& operator&= (const self& rhs);
      self operator& (const self& rhs) const;
      self intersection (const self& rhs) const;
      bool overlap (const self& rhs) const;

      // union
      self& operator|= (const self& rhs);
      self operator| (const self& rhs) const;
      self Union (const self& rhs) const;   /// lowercase union is a reserved word

    private:
      point_t pos;
      size_t sz;
    };

    // --------------------------------------------------------------------------
    template<typename T, typename S, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_rectangle<T, S, C>&);

    // --------------------------------------------------------------------------
    typedef basic_rectangle<float, float, coordinate_system::local> rectangle;
    typedef basic_rectangle<int32_t, uint32_t, coordinate_system::surface> native_rect;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui

#include <gui/core/rectangle.inl>
