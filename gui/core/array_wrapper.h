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
#include <gui/core/guidefs.h>


namespace gui {

  namespace core {

    namespace detail {

      struct boundary_check {
        inline boundary_check (size_t maximum)
#ifndef NDEBUG
          : maximum(maximum)
  #endif // NDEBUG
        {}

        inline void operator () (size_t i) const {
#ifndef NDEBUG
          if (i >= maximum) {
            throw std::out_of_range("array_wrapper try to access element beyond size");
          }
#endif // NDEBUG
        }

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

      inline array_wrapper (type* data, size_t size)
        : data_(data)
        , check_boundary(size)
      {}

      inline array_wrapper (std::vector<type>& data)
        : data_(data.data())
        , check_boundary(data.size())
      {}

      inline type& operator[] (size_t i) {
        check_boundary(i);
        return data_[i];
      }

      inline array_wrapper sub (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return array_wrapper(data_ + offset, n);
      }

      inline type* data (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return data_ + offset;
      }

    private:
      type* data_;
      detail::boundary_check check_boundary;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper<T const> {
      typedef T type;

      inline array_wrapper (const type* data, size_t size)
        : data_(data)
        , check_boundary(size)
      {}

      inline array_wrapper (const std::vector<type>& data)
        : data_(data.data())
        , check_boundary(data.size())
      {}

      inline const type& operator[] (size_t i) const {
        check_boundary(i);
        return data_[i];
      }

      inline array_wrapper sub (size_t offset, size_t sz) const {
        check_boundary(offset + sz - 1);
        return array_wrapper(data_ + offset, sz);
      }

      inline const type* data (size_t offset, size_t n) const {
        check_boundary(offset + n - 1);
        return data_ + offset;
      }

    private:
      const type* data_;
      detail::boundary_check check_boundary;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct bit_array_wrapper {
      typedef byte type;

      inline bit_array_wrapper (type* data, size_t size)
        : data_(data)
        , check_boundary(size)
      {}

      inline bit_array_wrapper (std::vector<type>& data)
        : data_(data.data())
        , check_boundary(data.size())
      {}

      inline bit_wrapper<T> operator[] (size_t i) {
        check_boundary(i);
        return bit_wrapper<T>(data_[i / 8], i % 8);
      }

      inline bit_array_wrapper sub (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return bit_array_wrapper(data_ + offset / 8, n);
      }

    private:
      type* data_;
      detail::boundary_check check_boundary;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct bit_array_wrapper<T const> {
      typedef byte type;

      inline bit_array_wrapper (const type* data, size_t size)
        : data_(data)
        , check_boundary(size)
      {}

      inline bit_array_wrapper (const std::vector<type>& data)
        : data_(data.data())
        , check_boundary(data.size())
      {}

      inline bit_wrapper<T const> operator[] (size_t i) const {
        check_boundary(i);
        return bit_wrapper<T const>(data_[i / 8], i % 8);
      }

      inline bit_array_wrapper sub (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return bit_array_wrapper(data_ + offset / 8, n);
      }

    private:
      const type* data_;
      detail::boundary_check check_boundary;
    };

    // --------------------------------------------------------------------------
    template<>
    struct array_wrapper<bool> : public bit_array_wrapper<bool> {};

    template<>
    struct array_wrapper<const bool> : public bit_array_wrapper<const bool> {};

  } // core

} // gui
