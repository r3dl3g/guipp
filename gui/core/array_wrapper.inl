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
        if (!(i <= maximum)) {
          throw std::out_of_range("array_wrapper try to access element beyond size");
        }
#else
        (void)i;
#endif // NDEBUG
      }

      template<typename I, typename T>
      struct index_type {
        static I make (T* data, int i) {
          return data[i];
        }

        static int offset (int i) {
          return i;
        }
      };

      template<typename I>
      struct index_type<I, pixel::mono> {
        static I make (byte* data, int i) {
          return I(data[i / 8], i % 8);
        }

        static int offset (int i) {
          return i / 8;
        }
      };

      template<typename I>
      struct index_type<I, const pixel::mono> {
        static I make (const byte* const data, int i) {
          return I(data[i / 8], i % 8);
        }

        static int offset (int i) {
          return i / 8;
        }
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T>
    inline array_wrapper<T>::array_wrapper (raw_type* data, size_t size)
      : data_(data)
      , check_boundary(size)
    {}

    template<typename T>
    inline array_wrapper<T>::array_wrapper (std::vector<T>& data)
      : data_(data.data())
      , check_boundary(data.size())
    {}

    template<typename T>
    inline auto array_wrapper<T>::operator[] (size_t i) -> index_type {
      check_boundary(i);
      return detail::index_type<index_type, T>::make(data_, i);
    }

    template<typename T>
    inline auto array_wrapper<T>::sub (size_t offs, size_t n) -> array_wrapper {
      const auto offset = detail::index_type<index_type, T>::offset(offs);
      check_boundary(offset + n);
      return array_wrapper(data_ + offset, n);
    }

    template<typename T>
    inline auto array_wrapper<T>::data (size_t offs, size_t n) -> full_raw_type* {
      const auto offset = detail::index_type<index_type, T>::offset(offs);
      check_boundary(offset + n);
      return data_ + offset;
    }

    template<typename T>
    inline auto array_wrapper<T>::operator[] (size_t i) const -> const const_index_type {
      check_boundary(i);
      return detail::index_type<const_index_type, T>::make(data_, i);
    }

    template<typename T>
    inline auto array_wrapper<T>::sub (size_t offs, size_t sz) const -> const array_wrapper {
      const auto offset = detail::index_type<index_type, T>::offset(offs);
      check_boundary(offset + sz);
      return array_wrapper(data_ + offset, sz);
    }

    template<typename T>
    inline auto array_wrapper<T>::data (size_t offs, size_t n) const -> const raw_type* {
      const auto offset = detail::index_type<index_type, T>::offset(offs);
      check_boundary(offset + n);
      return data_ + offset;
    }

    template<typename T>
    array_wrapper<T>& array_wrapper<T>::copy_from (const array_wrapper<type>& rhs, size_t n) {
      check_boundary(n);
      memcpy(data_, rhs.data(0, n), n);
      return *this;
    }

    template<typename T>
    array_wrapper<T>& array_wrapper<T>::copy_from (const array_wrapper<const type>& rhs, size_t n) {
      check_boundary(n);
      memcpy(data_, rhs.data(0, n), n);
      return *this;
    }

  } // namespace core

} // namespace gui
