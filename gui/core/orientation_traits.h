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
#include <gui/core/rectangle.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct orientation_traits {
      static core::point::type get_1 (const core::point&);
      static core::point::type get_2 (const core::point&);

      static core::size::type get_1 (const core::size&);
      static core::size::type get_2 (const core::size&);

      static void set (core::point&, core::point::type dim, core::point::type other);
      static void set (core::size&, core::size::type dim, core::size::type other);

      static void set_1 (core::point&, core::point::type dim);
      static void set_2 (core::point&, core::point::type other);

      static void set_1 (core::size&, core::size::type dim);
      static void set_2 (core::size&, core::size::type other);

      static void set_1 (core::rectangle&, core::point::type, core::size::type);
      static void set_2 (core::rectangle&, core::point::type, core::size::type);
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/orientation_traits.inl>
