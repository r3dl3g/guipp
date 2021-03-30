/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include "size.h"
#include "context.h"


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C = core::coordinate_system::independent>
    struct basic_point {
      typedef T type;
      typedef basic_point self;
      typedef basic_size<T, C> size_t;

      static const self zero;
      static const self end;
      static const self undefined;

      inline basic_point ();
      inline basic_point (type x, type y);

      explicit inline basic_point (type i);
      explicit basic_point (const gui::os::point& rhs, const context&);
      explicit basic_point (const gui::os::rectangle& r, const context&);

      gui::os::point os (const context&) const;

      type x () const;
      type y () const;

      gui::os::point_type os_x (const context&) const;
      gui::os::point_type os_y (const context&) const;

      void x (type v);
      void y (type v);

      bool is_valid () const;
      bool isnan () const;
      void clear ();

      bool operator== (const self& rhs) const;
      bool operator!= (const self& rhs) const;
      bool operator< (const self& rhs) const;
      bool operator> (const self& rhs) const;
      bool operator<= (const self& rhs) const;
      bool operator>= (const self& rhs) const;

      self& operator= (const self& rhs);
      self& operator+= (const size_t& s);
      self& operator-= (const size_t& s);
      self& operator+= (const self& pt);
      self& operator-= (const self& pt);
      self& operator*= (double f);
      self& operator/= (double f);

      self operator+ (const size_t& s) const;
      self operator- (const size_t& s) const;

      self operator- (const self& pt) const;
      self operator+ (const self& pt) const;
      self operator* (double f) const;
      self operator/ (double f) const;
      self operator- () const;

      size_t diff (const self& pt) const;

      void move (const self& pt);
      void move_x (type dx);
      void move_y (type dy);

      self dx (type x) const;
      self dy (type y) const;
      self dxy (type x, type y) const;

    private:
      type x_;
      type y_;
    };

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_point<T, C>&);

    // --------------------------------------------------------------------------
    typedef basic_point<float, coordinate_system::local> point;
    typedef basic_point<int32_t, coordinate_system::surface> native_point;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui

#include <gui/core/point.inl>
