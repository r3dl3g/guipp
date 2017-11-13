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

      inline void check_boundary (size_t i, size_t maximum) {
#ifndef NDEBUG
        if (i >= maximum) {
          throw std::out_of_range("array_wrapper try to access element beyond size");
        }
#endif // NDEBUG
      }

    }

    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper {
      typedef T type;

      inline array_wrapper (type* data, size_t size)
        : data_(data)
#ifndef NDEBUG
        , size_(size)
#endif // NDEBUG
      {}

      inline array_wrapper (std::vector<type>& data)
        : data_(data.data())
#ifndef NDEBUG
        , size_(data.size())
#endif // NDEBUG
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        detail::check_boundary(i, size_);
#endif // NDEBUG
      }

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
#ifndef NDEBUG
      const size_t size_;
#endif // NDEBUG
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper<T const> {
      typedef T type;

      inline array_wrapper (const type* data, size_t size)
        : data_(data)
#ifndef NDEBUG
        , size_(size)
#endif // NDEBUG
      {}

      inline array_wrapper (const std::vector<type>& data)
        : data_(data.data())
#ifndef NDEBUG
        , size_(data.size())
#endif // NDEBUG
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        detail::check_boundary(i, size_);
#endif // NDEBUG
      }

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
#ifndef NDEBUG
      const size_t size_;
#endif // NDEBUG
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct bool_array_wrapper {
      typedef byte type;

      inline bool_array_wrapper (type* data, size_t size)
        : data_(data)
#ifndef NDEBUG
        , size_(size)
#endif // NDEBUG
      {}

      inline bool_array_wrapper (std::vector<type>& data)
        : data_(data.data())
#ifndef NDEBUG
        , size_(data.size())
#endif // NDEBUG
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        detail::check_boundary(i, size_);
#endif // NDEBUG
      }

      inline bool_wrapper<T> operator[] (size_t i) {
        check_boundary(i);
        return bool_wrapper<T>(data_[i / 8], i % 8);
      }

      inline bool_array_wrapper sub (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return bool_array_wrapper(data_ + offset / 8, n);
      }

    private:
      type* data_;
#ifndef NDEBUG
      const size_t size_;
#endif // NDEBUG
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct bool_array_wrapper<T const> {
      typedef byte type;

      inline bool_array_wrapper (const type* data, size_t size)
        : data_(data)
#ifndef NDEBUG
        , size_(size)
#endif // NDEBUG
      {}

      inline bool_array_wrapper (const std::vector<type>& data)
        : data_(data.data())
#ifndef NDEBUG
        , size_(data.size())
#endif // NDEBUG
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        detail::check_boundary(i, size_);
#endif // NDEBUG
      }

      inline bool_wrapper<T const> operator[] (size_t i) const {
        check_boundary(i);
        return bool_wrapper<T const>(data_[i / 8], i % 8);
      }

      inline bool_array_wrapper sub (size_t offset, size_t n) {
        check_boundary(offset + n - 1);
        return bool_array_wrapper(data_ + offset / 8, n);
      }

    private:
      const type* data_;
#ifndef NDEBUG
      const size_t size_;
#endif // NDEBUG
    };

    // --------------------------------------------------------------------------
    template<>
    struct array_wrapper<bool> : public bool_array_wrapper<bool> {};

    template<>
    struct array_wrapper<const bool> : public bool_array_wrapper<const bool> {};

  } // core

} // gui
