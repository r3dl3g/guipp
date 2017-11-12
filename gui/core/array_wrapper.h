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


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper {
      inline array_wrapper (T* data, size_t size)
        : data_(data)
        , size_(size)
      {}

      inline array_wrapper (std::vector<T>& data)
        : data_(data.data())
        , size_(data.size())
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        if (i >= size_) {
          throw std::out_of_range("array_wrapper try to access element beyond size");
        }
#endif // NDEBUG
      }

      inline T& operator[] (size_t i) {
        check_boundary(i);
        return data_[i];
      }

      inline array_wrapper sub (size_t offset, size_t sz) {
        check_boundary(offset + sz - 1);
        return array_wrapper(data_ + offset, sz);
      }

      inline size_t size () const {
        return size_;
      }

      inline T* data () {
        return data_;
      }

      inline T* operator+ (size_t n) {
        check_boundary(n);
        return data_ + n;
      }

    private:
      T* data_;
      const size_t size_;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct array_wrapper<T const> {
      inline array_wrapper (const T* data, size_t size)
        : data_(data)
        , size_(size)
      {}

      inline array_wrapper (const std::vector<T>& data)
        : data_(data.data())
        , size_(data.size())
      {}

      inline void check_boundary (size_t i) const {
#ifndef NDEBUG
        if (i >= size_) {
          throw std::out_of_range("array_wrapper try to access element beyond size");
        }
#endif // NDEBUG
      }

      inline const T& operator[] (size_t i) const {
        check_boundary(i);
        return data_[i];
      }

      inline array_wrapper sub (size_t offset, size_t sz) const {
        check_boundary(offset + sz - 1);
        return array_wrapper(data_ + offset, sz);
      }

      inline size_t size () const {
        return size_;
      }

      inline const T* data () const {
        return data_;
      }

      inline const T* operator+ (size_t n) const {
        check_boundary(n);
        return data_ + n;
      }

    private:
      const T* data_;
      const size_t size_;
    };

  } // core

} // gui
