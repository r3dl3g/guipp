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
 * @brief     2D size structure
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iosfwd>
#include <limits>
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include "coordinate_system.h"


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C = core::coordinate_system::independent>
    struct basic_size {
      typedef T type;
      typedef basic_size self;

      static const self zero;
      static const self one;
      static const self two;
      static const self end;
      static const self undefined;

      basic_size ();
      basic_size (type width, type height);

      explicit basic_size (type i);
      explicit basic_size (const gui::os::size& sz);
      explicit basic_size (const gui::os::point& pt);
      explicit basic_size (const gui::os::rectangle& r);

      gui::os::size os () const;
      gui::os::point os_point () const;

      type width () const;
      type height () const;

      gui::os::size_type os_width () const;
      gui::os::size_type os_height () const;

      void width (type w);
      void height (type h);

      bool empty () const;
      bool is_valid() const;
      void clear (type v = 0);

      bool operator== (const self& rhs) const;
      bool operator!= (const self& rhs) const;

      bool operator< (const self& rhs) const;
      bool operator<= (const self& rhs) const;
      bool operator> (const self& rhs) const;
      bool operator>= (const self& rhs) const;

      self operator+ (const self& rhs) const;
      self operator- (const self& rhs) const;
      self operator* (double f) const;
      self operator/ (double f) const;

      self& operator= (const self& rhs);
      self& operator+= (const self& s);
      self& operator-= (const self& s);
      self& operator*= (double f);
      self& operator/= (double f);

      self operator- () const;

      self& inc (const self& pt);
      self& inc_x (type dx);
      self& inc_y (type dy);

      self min (const self& rhs) const;
      self max (const self& rhs) const;

    private:
      type w;
      type h;
    };

    // --------------------------------------------------------------------------
    template<typename T, coordinate_system C>
    std::ostream& operator<< (std::ostream& out, const basic_size<T, C>&);

    // --------------------------------------------------------------------------
    typedef basic_size<float, coordinate_system::local> size;
    typedef basic_size<uint32_t, coordinate_system::surface> native_size;

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui

#include <gui/core/size.inl>
