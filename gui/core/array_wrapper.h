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
 * @brief     array wrapper
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <stdexcept>
#include <vector>
#include <memory.h>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "pixel.h"


namespace gui {

  namespace core {

    namespace detail {

      struct boundary_check {
        explicit boundary_check (size_t maximum);
        void operator () (size_t i) const;

      private:
#ifndef NDEBUG
        const size_t maximum;
#endif // NDEBUG
      };

      template<typename T>
      struct index_return_type { using type = T&; };

      template<>
      struct index_return_type<bool> { using type = bit_wrapper<bool>; };

      template<>
      struct index_return_type<const bool> { using type = bit_wrapper<const bool>; };

      template<>
      struct index_return_type<pixel::mono> { using type = bit_wrapper<pixel::mono>; };

      template<>
      struct index_return_type<const pixel::mono> { using type = bit_wrapper<const pixel::mono>; };

      template<typename T>
      struct array2raw_type { using type = T; };

      template<>
      struct array2raw_type<bool> { using type = byte; };

      template<>
      struct array2raw_type<const bool> { using type = const byte; };

      template<>
      struct array2raw_type<pixel::mono> { using type = byte; };

      template<>
      struct array2raw_type<const pixel::mono> { using type = const byte; };

    } // namespace detail  


    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper {
      using type = typename std::remove_const<T>::type;
      using raw_type = typename detail::array2raw_type<T>::type;
      using full_type = T;
      using full_raw_type = typename detail::array2raw_type<T>::type;
      using index_type = typename detail::index_return_type<T>::type;
      using const_index_type = typename detail::index_return_type<const type>::type;

      array_wrapper (raw_type* data, size_t size);
      explicit array_wrapper (std::vector<T>& data);

      index_type operator[] (size_t i);
      array_wrapper sub (size_t offset, size_t n);
      full_raw_type* data (size_t offset, size_t n);

      const const_index_type operator[] (size_t i) const;
      const array_wrapper sub (size_t offset, size_t sz) const;
      const raw_type* data (size_t offset, size_t n) const;

      array_wrapper& copy_from (const array_wrapper<type>& rhs, size_t n);
      array_wrapper& copy_from (const array_wrapper<const type>& rhs, size_t n);

    private:
      raw_type* data_;
      detail::boundary_check check_boundary;
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include "array_wrapper.inl"
