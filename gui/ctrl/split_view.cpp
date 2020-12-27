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
 * @brief     C++ API: basic controls
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/split_view.h>


namespace gui {

  namespace layout {

    template<>
    core::size::type split_view_traits<orientation_t::vertical>::get_slider_width () {
      return 8;
    }

    template<>
    core::size::type split_view_traits<orientation_t::horizontal>::get_slider_width () {
      return 8;
    }

    template<>
    double split_view_traits<orientation_t::vertical>::get_split_pos (const core::point& pt, const core::size& sz) {
      return double(pt.x() + get_slider_width() / 2) / (double)sz.width();
    }

    template<>
    double split_view_traits<orientation_t::horizontal>::get_split_pos (const core::point& pt, const core::size& sz) {
      return double(pt.y() + get_slider_width() / 2) / (double)sz.height();
    }

    template<>
    core::rectangle split_view_traits<orientation_t::vertical>::get_first_place (const core::rectangle& sz,
                                                                                 double pos) {
      const auto w = core::point::type(sz.width() * pos - get_slider_width() / 2);
      return sz.with_width(w);
    }

    template<>
    core::rectangle split_view_traits<orientation_t::vertical>::get_second_place (const core::rectangle& sz,
                                                                                  double pos) {
      const core::point::type x = sz.x() + core::point::type(sz.width() * pos + get_slider_width() / 2);
      return sz.with_horizontal(x, sz.x2() - x);
    }

    template<>
    core::rectangle split_view_traits<orientation_t::vertical>::get_slider_place (const core::rectangle& sz,
                                                                                  double pos) {
      const core::point::type x = sz.x() + core::point::type(sz.width() * pos - get_slider_width() / 2);
      return sz.with_horizontal(x, get_slider_width());
    }

    template<>
    core::rectangle split_view_traits<orientation_t::horizontal>::get_first_place (const core::rectangle& sz,
                                                                                   double pos) {
      const auto h = core::point::type(sz.height() * pos - get_slider_width() / 2);
      return sz.with_height(h);
    }

    template<>
    core::rectangle split_view_traits<orientation_t::horizontal>::get_second_place (const core::rectangle& sz,
                                                                                    double pos) {
      const core::point::type y = sz.y() + core::point::type(sz.height() * pos + get_slider_width() / 2);
      return sz.with_vertical(y, sz.y2() - y);
    }

    template<>
    core::rectangle split_view_traits<orientation_t::horizontal>::get_slider_place (const core::rectangle& sz,
                                                                                    double pos) {
      const core::point::type y = sz.y() + core::point::type(sz.height() * pos - get_slider_width() / 2);
      return sz.with_vertical(y, get_slider_width());
    }

  }

} // gui
