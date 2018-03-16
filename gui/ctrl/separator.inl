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
      inline draw::line line_traits<orientation::vertical>::first (const core::rectangle& r) {
        return draw::line(r.top_left(), r.bottom_left());
      }

      template<>
      inline draw::line line_traits<orientation::vertical>::second (const core::rectangle& r) {
        return draw::line(r.top_right(), r.bottom_right());
      }

      template<>
      inline core::rectangle line_traits<orientation::vertical>::area (const core::rectangle& r) {
        return r - core::size(1, 0);
      }

      // --------------------------------------------------------------------------
      template<>
      inline draw::line line_traits<orientation::horizontal>::first (const core::rectangle& r) {
        return draw::line(r.top_left(), r.top_right());
      }

      template<>
      inline draw::line line_traits<orientation::horizontal>::second (const core::rectangle& r) {
        return draw::line(r.bottom_right(), r.bottom_left());
      }

      template<>
      inline core::rectangle line_traits<orientation::horizontal>::area (const core::rectangle& r) {
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

    template<orientation O, bool S, os::color B>
    inline basic_separator<O, S, B>::basic_separator () {
      on_paint(basepp::bind_method(this, &basic_separator::paint));
    }

    template<orientation O, bool S, os::color B>
    inline void basic_separator<O, S, B>::paint (const draw::graphics& graph) {
      core::rectangle r = lt::area(client_area());
      graph.frame(lt::first(r), ct::first(B));
      graph.frame(lt::second(r), ct::second(B));
    }

  } // ctrl

} // gui
