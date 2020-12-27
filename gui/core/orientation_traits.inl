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

namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<>
    inline point::type
    orientation_traits<orientation_t::horizontal>::get_1 (const point& pt) {
      return pt.x();
    }

    template<>
    inline size::type
    orientation_traits<orientation_t::horizontal>::get_1 (const size& sz) {
      return sz.width();
    }

    template<>
    inline point::type
    orientation_traits<orientation_t::horizontal>::get_2 (const point& pt) {
      return pt.y();
    }

    template<>
    inline size::type
    orientation_traits<orientation_t::horizontal>::get_2 (const size& sz) {
      return sz.height();
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set (point& pt,
                                                        point::type d1,
                                                        point::type d2) {
      pt.x(d1);
      pt.y(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (point& pt,
                                                          point::type d1) {
      pt.x(d1);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (point& pt,
                                                          point::type d2) {
      pt.y(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set (size& sz,
                                                        size::type d1,
                                                        size::type d2) {
      sz.width(d1);
      sz.height(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (size& sz,
                                                          size::type d1) {
      sz.width(d1);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (size& sz,
                                                          size::type d2) {
      sz.height(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (rectangle& r,
                                                          point::type v,
                                                          size::type s) {
      r.x(v);
      r.width(s);
    }

    template<>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (rectangle& r,
                                                          point::type v,
                                                          size::type s) {
      r.y(v);
      r.height(s);
    }

    // --------------------------------------------------------------------------
    template<>
    inline point::type
    orientation_traits<orientation_t::vertical>::get_1 (const point& pt) {
      return pt.y();
    }

    template<>
    inline size::type
    orientation_traits<orientation_t::vertical>::get_1 (const size& sz) {
      return sz.height();
    }

    template<>
    inline point::type
    orientation_traits<orientation_t::vertical>::get_2 (const point& pt) {
      return pt.x();
    }

    template<>
    inline size::type
    orientation_traits<orientation_t::vertical>::get_2 (const size& sz) {
      return sz.width();
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set (point& pt,
                                                      point::type d1,
                                                      point::type d2) {
      pt.y(d1);
      pt.x(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (point& pt,
                                                        point::type d1) {
      pt.y(d1);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (point& pt,
                                                        point::type d2) {
      pt.x(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set (size& sz,
                                                      size::type d1,
                                                      size::type d2) {
      sz.height(d1);
      sz.width(d2);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (size& sz,
                                                        size::type d1) {
      sz.height(d1);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (size& sz,
                                                        size::type d2) {
      sz.width(d2);
   }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (rectangle& r,
                                                        point::type v,
                                                        size::type s) {
      r.y(v);
      r.height(s);
    }

    template<>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (rectangle& r,
                                                        point::type v,
                                                        size::type s) {
      r.x(v);
      r.width(s);
    }

  } // namespace core

} // namespace gui
