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
 * @brief     begin - end range
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T>
    struct range {
      typedef T type;

      range (type v = type());
      range (const type& f, const type& l);
      range (type&& f, type&& l);

      bool operator== (const range&) const;
      bool operator!= (const range&) const;

      bool is_inside (const type& i) const;
      void sort ();
      bool empty () const;
      void clear ();

      type begin ();
      type end ();

      const type begin () const;
      const type end () const;

      const type size () const;

    private:
      type first;
      type last;

    };

    template<typename T>
    range<T> min_max (const range<T>& lhs, const range<T>& rhs);

    template<typename T>
    range<T> intersection (const range<T>& lhs, const range<T>& rhs);

    // --------------------------------------------------------------------------
    template<typename T>
    std::ostream& operator<< (std::ostream& out, const range<T>& r);

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/range.inl>
