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
 * @brief     C++ API: array wrapper
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <stdexcept>
#include <vector>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "bits.h"


namespace basepp {

  namespace detail {

    struct boundary_check {
      boundary_check (size_t maximum);
      void operator () (size_t i) const;

    private:
#ifndef NDEBUG
      const size_t maximum;
#endif // NDEBUG
    };

  } // namespace detail

  // --------------------------------------------------------------------------
  template<typename T>
  struct array_wrapper {
    typedef T type;

    array_wrapper (type* data, size_t size);
    array_wrapper (std::vector<type>& data);

    type& operator[] (size_t i);
    array_wrapper sub (size_t offset, size_t n);
    type* data (size_t offset, size_t n);

  private:
    type* data_;
    detail::boundary_check check_boundary;
  };

  // --------------------------------------------------------------------------
  template<typename T>
  struct array_wrapper<T const> {
    typedef T type;

    array_wrapper (const type* data, size_t size);
    array_wrapper (const std::vector<type>& data);

    const type& operator[] (size_t i) const;
    array_wrapper sub (size_t offset, size_t sz) const;
    const type* data (size_t offset, size_t n) const;

  private:
    const type* data_;
    detail::boundary_check check_boundary;
  };

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_array_wrapper {
    typedef byte type;

    bit_array_wrapper (type* data, size_t size);
    bit_array_wrapper (std::vector<type>& data);

    bit_wrapper<T> operator[] (size_t i);
    bit_array_wrapper sub (size_t offset, size_t n);

  private:
    type* data_;
    detail::boundary_check check_boundary;
  };

  // --------------------------------------------------------------------------
  template<typename T>
  struct bit_array_wrapper<T const> {
    typedef byte type;

    bit_array_wrapper (const type* data, size_t size);
    bit_array_wrapper (const std::vector<type>& data);

    bit_wrapper<T const> operator[] (size_t i) const;
    bit_array_wrapper sub (size_t offset, size_t n);

  private:
    const type* data_;
    detail::boundary_check check_boundary;
  };

  // --------------------------------------------------------------------------
  template<>
  struct array_wrapper<bool> : public bit_array_wrapper<bool> {};

  template<>
  struct array_wrapper<const bool> : public bit_array_wrapper<const bool> {};

} // namespace basepp

#include "array_wrapper.inl"
