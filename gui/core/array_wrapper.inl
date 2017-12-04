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


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    namespace detail {

      inline boundary_check::boundary_check (size_t maximum)
#ifndef NDEBUG
        : maximum(maximum)
#endif // NDEBUG
      {
#ifdef NDEBUG
        (void)maximum;
#endif // NDEBUG
      }

      inline void boundary_check::operator () (size_t i) const {
#ifndef NDEBUG
        if (i >= maximum) {
          throw std::out_of_range("array_wrapper try to access element beyond size");
        }
#else
        (void)i;
#endif // NDEBUG
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T>
    inline array_wrapper<T>::array_wrapper (type* data, size_t size)
      : data_(data)
      , check_boundary(size)
    {}

    template<typename T>
    inline array_wrapper<T>::array_wrapper (std::vector<type>& data)
      : data_(data.data())
      , check_boundary(data.size())
    {}

    template<typename T>
    inline auto array_wrapper<T>::operator[] (size_t i) -> type& {
      check_boundary(i);
      return data_[i];
    }

    template<typename T>
    inline auto array_wrapper<T>::sub (size_t offset, size_t n) -> array_wrapper {
      check_boundary(offset + n - 1);
      return array_wrapper(data_ + offset, n);
    }

    template<typename T>
    inline auto array_wrapper<T>::data (size_t offset, size_t n) -> type* {
      check_boundary(offset + n - 1);
      return data_ + offset;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline array_wrapper<T const>::array_wrapper (const type* data, size_t size)
      : data_(data)
      , check_boundary(size)
    {}

    template<typename T>
    inline array_wrapper<T const>::array_wrapper (const std::vector<type>& data)
      : data_(data.data())
      , check_boundary(data.size())
    {}

    template<typename T>
    inline auto array_wrapper<T const>::operator[] (size_t i) const -> const type& {
      check_boundary(i);
      return data_[i];
    }

    template<typename T>
    inline auto array_wrapper<T const>::sub (size_t offset, size_t sz) const -> array_wrapper {
      check_boundary(offset + sz - 1);
      return array_wrapper(data_ + offset, sz);
    }

    template<typename T>
    inline auto array_wrapper<T const>::data (size_t offset, size_t n) const -> const type* {
      check_boundary(offset + n - 1);
      return data_ + offset;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline bit_array_wrapper<T>::bit_array_wrapper (type* data, size_t size)
      : data_(data)
      , check_boundary(size)
    {}

    template<typename T>
    inline bit_array_wrapper<T>::bit_array_wrapper (std::vector<type>& data)
      : data_(data.data())
      , check_boundary(data.size())
    {}

    template<typename T>
    inline bit_wrapper<T> bit_array_wrapper<T>::operator[] (size_t i) {
      check_boundary(i);
      return bit_wrapper<T>(data_[i / 8], i % 8);
    }

    template<typename T>
    inline auto bit_array_wrapper<T>::sub (size_t offset, size_t n) -> bit_array_wrapper {
      check_boundary(offset + n - 1);
      return bit_array_wrapper(data_ + offset / 8, n);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline bit_array_wrapper<T const>::bit_array_wrapper (const type* data, size_t size)
      : data_(data)
      , check_boundary(size)
    {}

    template<typename T>
    inline bit_array_wrapper<T const>::bit_array_wrapper (const std::vector<type>& data)
      : data_(data.data())
      , check_boundary(data.size())
    {}

    template<typename T>
    inline bit_wrapper<T const> bit_array_wrapper<T const>::operator[] (size_t i) const {
      check_boundary(i);
      return bit_wrapper<T const>(data_[i / 8], i % 8);
    }

    template<typename T>
    inline auto bit_array_wrapper<T const>::sub (size_t offset, size_t n) -> bit_array_wrapper {
      check_boundary(offset + n - 1);
      return bit_array_wrapper(data_ + offset / 8, n);
    }

  } // core

} // gui
