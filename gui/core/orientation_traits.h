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
#include <gui/core/rectangle.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct orientation_traits {
      template<typename T, coordinate_system C>
      static typename basic_point<T, C>::type get_1 (const basic_point<T, C>&);
      template<typename T, coordinate_system C>
      static typename basic_point<T, C>::type get_2 (const basic_point<T, C>&);

      template<typename S, coordinate_system C>
      static typename basic_size<S, C>::type get_1 (const basic_size<S, C>&);
      template<typename S, coordinate_system C>
      static typename basic_size<S, C>::type get_2 (const basic_size<S, C>&);

      template<typename T, coordinate_system C>
      static void set (basic_point<T, C>&, typename basic_point<T, C>::type dim, typename basic_point<T, C>::type other);
      template<typename S, coordinate_system C>
      static void set (basic_size<S, C>&, typename basic_size<S, C>::type dim, typename basic_size<S, C>::type other);

      template<typename T, coordinate_system C>
      static void set_1 (basic_point<T, C>&, typename basic_point<T, C>::type dim);
      template<typename T, coordinate_system C>
      static void set_2 (basic_point<T, C>&, typename basic_point<T, C>::type other);

      template<typename S, coordinate_system C>
      static void set_1 (basic_size<S, C>&, typename basic_size<S, C>::type dim);
      template<typename S, coordinate_system C>
      static void set_2 (basic_size<S, C>&, typename basic_size<S, C>::type other);

      template<typename T, typename S, coordinate_system C>
      static void set_1 (basic_rectangle<T, S, C>&, typename basic_point<T, C>::type, typename basic_size<S, C>::type);
      template<typename T, typename S, coordinate_system C>
      static void set_2 (basic_rectangle<T, S, C>&, typename basic_point<T, C>::type, typename basic_size<S, C>::type);
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/orientation_traits.inl>
