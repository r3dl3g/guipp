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
    core::size::type split_view<orientation_t::vertical>::get_slider_width () {
      return 8;
    }

    template<>
    core::size::type split_view<orientation_t::horizontal>::get_slider_width () {
      return 8;
    }

    template<>
    double split_view<orientation_t::vertical>::get_split_pos (const core::rectangle& r) const {
      return double(data.slider->position().x() + get_slider_width() / 2) / (double)r.width();
    }

    template<>
    double split_view<orientation_t::horizontal>::get_split_pos (const core::rectangle& r) const {
      return double(data.slider->position().y() + get_slider_width() / 2) / (double)r.height();
    }

    template<>
    core::rectangle split_view<orientation_t::vertical>::get_first_place (const core::rectangle& r,
                                                                        double pos) {
      const core::point::type w = core::point::type(r.width() * pos - get_slider_width() / 2);
      return r.with_width(w);
    }

    template<>
    core::rectangle split_view<orientation_t::vertical>::get_second_place (const core::rectangle& r,
                                                                         double pos) {
      const core::point::type x = r.x() + core::point::type(r.width() * pos + get_slider_width() / 2);
      return r.with_horizontal(x, r.x2() - x);
    }

    template<>
    core::rectangle split_view<orientation_t::vertical>::get_slider_place (const core::rectangle& r,
                                                                         double pos) {
      const core::point::type x = r.x() + core::point::type(r.width() * pos - get_slider_width() / 2);
      return r.with_horizontal(x, get_slider_width());
    }

    template<>
    core::rectangle split_view<orientation_t::horizontal>::get_first_place (const core::rectangle& r,
                                                                          double pos) {
      const core::point::type h = core::point::type(r.height() * pos - get_slider_width() / 2);
      return r.with_height(h);
    }

    template<>
    core::rectangle split_view<orientation_t::horizontal>::get_second_place (const core::rectangle& r,
                                                                           double pos) {
      const core::point::type y = r.y() + core::point::type(r.height() * pos + get_slider_width() / 2);
      return r.with_vertical(y, r.y2() - y);
    }

    template<>
    core::rectangle split_view<orientation_t::horizontal>::get_slider_place (const core::rectangle& r,
                                                                           double pos) {
      const core::point::type y = r.y() + core::point::type(r.height() * pos - get_slider_width() / 2);
      return r.with_vertical(y, get_slider_width());
    }

  }

} // gui
