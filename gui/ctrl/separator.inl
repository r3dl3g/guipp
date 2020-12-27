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
 * @brief     C++ API: separator view
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      template<>
      inline draw::line line_traits<orientation_t::vertical>::first (const core::rectangle& r) {
        return {r.top_left(), r.bottom_left()};
      }

      template<>
      inline draw::line line_traits<orientation_t::vertical>::second (const core::rectangle& r) {
        return {r.top_right(), r.bottom_right()};
      }

      template<>
      inline core::rectangle line_traits<orientation_t::vertical>::area (const core::rectangle& r) {
        return r - core::size(1, 0);
      }

      // --------------------------------------------------------------------------
      template<>
      inline draw::line line_traits<orientation_t::horizontal>::first (const core::rectangle& r) {
        return {r.top_left(), r.top_right()};
      }

      template<>
      inline draw::line line_traits<orientation_t::horizontal>::second (const core::rectangle& r) {
        return {r.bottom_right(), r.bottom_left()};
      }

      template<>
      inline core::rectangle line_traits<orientation_t::horizontal>::area (const core::rectangle& r) {
        return r - core::size(0, 1);
      }

      // --------------------------------------------------------------------------
      template<>
      inline os::color color_traits<false>::first (os::color c) {
        return color::lighter(c);
      }

      template<>
      inline os::color color_traits<false>::second (os::color c) {
        return color::darker(c);
      }

      // --------------------------------------------------------------------------
      template<>
      inline os::color color_traits<true>::first (os::color c) {
        return color::darker(c);
      }

      template<>
      inline os::color color_traits<true>::second (os::color c) {
        return color::lighter(c);
      }

    } // namespace detail

    template<orientation_t O, bool S, os::color B>
    inline basic_separator<O, S, B>::basic_separator () {
      on_paint(draw::paint(this, &basic_separator::paint));
    }

    template<orientation_t O, bool S, os::color B>
    inline void basic_separator<O, S, B>::paint (const draw::graphics& graph) {
      core::rectangle r = /*lt::area(*/client_area()/*)*/;
      graph.frame(lt::first(r), ct::first(B));
      graph.frame(lt::second(r), ct::second(B));
    }

  } // ctrl

} // gui
