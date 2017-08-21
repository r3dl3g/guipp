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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "split_view.h"


namespace gui {

  namespace layout {

    template<>
    core::size::type split_view<orientation::vertical>::get_slider_width () {
      return 8;
    }

    template<>
    core::size::type split_view<orientation::horizontal>::get_slider_width () {
      return 8;
    }

    template<>
    double split_view<orientation::vertical>::get_split_pos (const core::size& sz) const {
      return double(data.slider->position().x() + get_slider_width() / 2) / (double)sz.width();
    }

    template<>
    double split_view<orientation::horizontal>::get_split_pos (const core::size& sz) const {
      return double(data.slider->position().y() + get_slider_width() / 2) / (double)sz.height();
    }

    template<>
    core::rectangle split_view<orientation::vertical>::get_first_place (const core::size& sz,
                                                                        double pos) {
      const core::point::type x = core::point::type(sz.width() * pos - get_slider_width() / 2);
      return core::rectangle(core::point::zero, core::point(x, sz.height()));
    }

    template<>
    core::rectangle split_view<orientation::vertical>::get_second_place (const core::size& sz,
                                                                         double pos) {
      const core::point::type x2 = core::point::type(sz.width() * pos + get_slider_width() / 2);
      return core::rectangle(core::point(x2, 0), core::point(sz.width(), sz.height()));
    }

    template<>
    core::rectangle split_view<orientation::vertical>::get_slider_place (const core::size& sz,
                                                                         double pos) {
      const core::point::type x = core::point::type(sz.width() * pos - get_slider_width() / 2);
      return core::rectangle(core::point(x, 0), core::size(get_slider_width(), sz.height()));
    }

    template<>
    core::rectangle split_view<orientation::horizontal>::get_first_place (const core::size& sz,
                                                                          double pos) {
      const core::point::type y = core::point::type(sz.height() * pos - get_slider_width() / 2);
      return core::rectangle(core::point::zero, core::point(sz.width(), y));
    }

    template<>
    core::rectangle split_view<orientation::horizontal>::get_second_place (const core::size& sz,
                                                                           double pos) {
      const core::point::type y2 = core::point::type(sz.height() * pos + get_slider_width() / 2);
      return core::rectangle(core::point(0, y2), core::point(sz.width(), sz.height()));
    }

    template<>
    core::rectangle split_view<orientation::horizontal>::get_slider_place (const core::size& sz,
                                                                           double pos) {
      const core::point::type y = core::point::type(sz.height() * pos - get_slider_width() / 2);
      return core::rectangle(core::point(0, y), core::size(sz.width(), get_slider_width()));
    }

  }

  namespace win {

    namespace detail {

      template<>
      window_class split_view<orientation::vertical>::clazz("vsplit_view",
#ifdef WIN32
                                            (os::color)(COLOR_BTNFACE + 1)
#endif // WIN32
#ifdef X11
                                            color::buttonColor()
#endif // X11
      );

      template<>
      window_class split_view<orientation::horizontal>::clazz("hsplit_view",
#ifdef WIN32
                                           (os::color)(COLOR_BTNFACE + 1)
#endif // WIN32
#ifdef X11
                                           color::buttonColor()
#endif // X11
      );

      template<>
      void split_view<orientation::vertical>::init () {
        slider.register_event_handler(REGISTER_FUNCTION, slider_event([&] (int) {
          get_layout().layout(size());
        }));
      }

      template<>
      void split_view<orientation::horizontal>::init () {
        slider.register_event_handler(REGISTER_FUNCTION, slider_event([&] (int) {
          get_layout().layout(size());
        }));
      }

    }

  } // win

} // gui
