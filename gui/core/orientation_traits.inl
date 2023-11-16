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
 * @brief     orientation traits
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<>
    template<typename T, coordinate_system C>
    inline typename basic_point<T, C>::type
    orientation_traits<orientation_t::horizontal>::get_1 (const basic_point<T, C>& pt) {
      return pt.x();
    }

    template<>
    template<typename S, coordinate_system C>
    inline typename basic_size<S, C>::type
    orientation_traits<orientation_t::horizontal>::get_1 (const basic_size<S, C>& sz) {
      return sz.width();
    }

    template<>
    template<typename T, coordinate_system C>
    inline typename basic_point<T, C>::type
    orientation_traits<orientation_t::horizontal>::get_2 (const basic_point<T, C>& pt) {
      return pt.y();
    }

    template<>
    template<typename S, coordinate_system C>
    inline typename basic_size<S, C>::type
    orientation_traits<orientation_t::horizontal>::get_2 (const basic_size<S, C>& sz) {
      return sz.height();
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set (basic_point<T, C>& pt,
                                                        typename basic_point<T, C>::type d1,
                                                        typename basic_point<T, C>::type d2) {
      pt.x(d1);
      pt.y(d2);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (basic_point<T, C>& pt,
                                                          typename basic_point<T, C>::type d1) {
      pt.x(d1);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (basic_point<T, C>& pt,
                                                          typename basic_point<T, C>::type d2) {
      pt.y(d2);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::move_1 (basic_point<T, C>& pt,
                                                           typename basic_point<T, C>::type d1) {
      pt.move_x(d1);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::move_2 (basic_point<T, C>& pt,
                                                           typename basic_point<T, C>::type d2) {
      pt.move_y(d2);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set (basic_size<S, C>& sz,
                                                        typename basic_size<S, C>::type d1,
                                                        typename basic_size<S, C>::type d2) {
      sz.width(d1);
      sz.height(d2);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (basic_size<S, C>& sz,
                                                          typename basic_size<S, C>::type d1) {
      sz.width(d1);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (basic_size<S, C>& sz,
                                                          typename basic_size<S, C>::type d2) {
      sz.height(d2);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::inc_1 (basic_size<S, C>& sz,
                                                          typename basic_size<S, C>::type d1) {
      sz.inc_x(d1);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::inc_2 (basic_size<S, C>& sz,
                                                          typename basic_size<S, C>::type d2) {
      sz.inc_y(d2);
    }

    template<>
    template<typename T, typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_1 (basic_rectangle<T, S, C>& r,
                                                          typename basic_point<T, C>::type v,
                                                          typename basic_size<S, C>::type s) {
      r.x(v);
      r.width(s);
    }

    template<>
    template<typename T, typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::horizontal>::set_2 (basic_rectangle<T, S, C>& r,
                                                          typename basic_point<T, C>::type v,
                                                          typename basic_size<S, C>::type s) {
      r.y(v);
      r.height(s);
    }

    // --------------------------------------------------------------------------
    template<>
    template<typename T, coordinate_system C>
    inline typename basic_point<T, C>::type
    orientation_traits<orientation_t::vertical>::get_1 (const basic_point<T, C>& pt) {
      return pt.y();
    }

    template<>
    template<typename S, coordinate_system C>
    inline typename basic_size<S, C>::type
    orientation_traits<orientation_t::vertical>::get_1 (const basic_size<S, C>& sz) {
      return sz.height();
    }

    template<>
    template<typename T, coordinate_system C>
    inline typename basic_point<T, C>::type
    orientation_traits<orientation_t::vertical>::get_2 (const basic_point<T, C>& pt) {
      return pt.x();
    }

    template<>
    template<typename S, coordinate_system C>
    inline typename basic_size<S, C>::type
    orientation_traits<orientation_t::vertical>::get_2 (const basic_size<S, C>& sz) {
      return sz.width();
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set (basic_point<T, C>& pt,
                                                      typename basic_point<T, C>::type d1,
                                                      typename basic_point<T, C>::type d2) {
      pt.y(d1);
      pt.x(d2);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (basic_point<T, C>& pt,
                                                        typename basic_point<T, C>::type d1) {
      pt.y(d1);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (basic_point<T, C>& pt,
                                                        typename basic_point<T, C>::type d2) {
      pt.x(d2);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::move_1 (basic_point<T, C>& pt,
                                                        typename basic_point<T, C>::type d1) {
      pt.move_y(d1);
    }

    template<>
    template<typename T, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::move_2 (basic_point<T, C>& pt,
                                                        typename basic_point<T, C>::type d2) {
      pt.move_x(d2);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set (basic_size<S, C>& sz,
                                                      typename basic_size<S, C>::type d1,
                                                      typename basic_size<S, C>::type d2) {
      sz.height(d1);
      sz.width(d2);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (basic_size<S, C>& sz,
                                                        typename basic_size<S, C>::type d1) {
      sz.height(d1);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (basic_size<S, C>& sz,
                                                        typename basic_size<S, C>::type d2) {
      sz.width(d2);
   }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::inc_1 (basic_size<S, C>& sz,
                                                        typename basic_size<S, C>::type d1) {
      sz.inc_y(d1);
    }

    template<>
    template<typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::inc_2 (basic_size<S, C>& sz,
                                                        typename basic_size<S, C>::type d2) {
      sz.inc_x(d2);
   }

    template<>
    template<typename T, typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_1 (basic_rectangle<T, S, C>& r,
                                                        typename basic_point<T, C>::type v,
                                                        typename basic_size<S, C>::type s) {
      r.y(v);
      r.height(s);
    }

    template<>
    template<typename T, typename S, coordinate_system C>
    inline void
    orientation_traits<orientation_t::vertical>::set_2 (basic_rectangle<T, S, C>& r,
                                                        typename basic_point<T, C>::type v,
                                                        typename basic_size<S, C>::type s) {
      r.x(v);
      r.width(s);
    }

  } // namespace core

} // namespace gui
