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
    core::size::type split_view<false>::get_slider_width () {
      return 8;
    }

    template<>
    core::size::type split_view<true>::get_slider_width () {
      return 8;
    }

    template<>
    double split_view<false>::get_split_pos (const core::size& sz) const {
      return double(slider->position().x() + get_slider_width() / 2) / (double)sz.width();
    }

    template<>
    double split_view<true>::get_split_pos (const core::size& sz) const {
      return double(slider->position().y() + get_slider_width() / 2) / (double)sz.height();
    }

    template<>
    core::rectangle split_view<false>::get_first_place (const core::size& sz,
                                                        double pos) {
      const core::point::type x = core::point::type(sz.width() * pos - get_slider_width() / 2);
      return core::rectangle(core::point::zero, core::point(x, sz.height()));
    }

    template<>
    core::rectangle split_view<false>::get_second_place (const core::size& sz,
                                                         double pos) {
      const core::point::type x2 = core::point::type(sz.width() * pos + get_slider_width() / 2);
      return core::rectangle(core::point(x2, 0), core::point(sz.width(), sz.height()));
    }

    template<>
    core::rectangle split_view<false>::get_slider_place (const core::size& sz, double pos) {
      const core::point::type x = core::point::type(sz.width() * pos - get_slider_width() / 2);
      return core::rectangle(core::point(x, 0), core::size(get_slider_width(), sz.height()));
    }

    template<>
    core::rectangle split_view<true>::get_first_place (const core::size& sz,
                                                       double pos) {
      const core::point::type y = core::point::type(sz.height() * pos - get_slider_width() / 2);
      return core::rectangle(core::point::zero, core::point(sz.width(), y));
    }

    template<>
    core::rectangle split_view<true>::get_second_place (const core::size& sz,
                                                        double pos) {
      const core::point::type y2 = core::point::type(sz.height() * pos + get_slider_width() / 2);
      return core::rectangle(core::point(0, y2), core::point(sz.width(), sz.height()));
    }

    template<>
    core::rectangle split_view<true>::get_slider_place (const core::size& sz, double pos) {
      const core::point::type y = core::point::type(sz.height() * pos - get_slider_width() / 2);
      return core::rectangle(core::point(0, y), core::size(sz.width(), get_slider_width()));
    }

  }

  namespace win {

    namespace detail {

      template<>
      window_class split_view<false>::clazz(
#ifdef WIN32
        win::window_class::custom_class("vsplit_view",
                                        CS_VREDRAW | CS_HREDRAW,
                                        WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                        WS_EX_NOPARENTNOTIFY,
                                        nullptr,
                                        IDC_ARROW,
                                        (os::color)(COLOR_BTNFACE + 1))
#endif // WIN32
#ifdef X11
        win::window_class::custom_class("vsplit_view", os::get_sys_color(os::COLOR_BTNFACE))
#endif // X11
      );

      template<>
      window_class split_view<true>::clazz(
#ifdef WIN32
        win::window_class::custom_class("hsplit_view",
                                                CS_VREDRAW | CS_HREDRAW,
                                                WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
                                                WS_EX_NOPARENTNOTIFY,
                                                nullptr,
                                                IDC_ARROW,
                                                (os::color)(COLOR_BTNFACE + 1))
#endif // WIN32
#ifdef X11
        win::window_class::custom_class("hsplit_view", os::get_sys_color(os::COLOR_BTNFACE))
#endif // X11
      );

      template<>
      split_view<false>::split_view () {
        get_layout().set_slider(&slider);
        slider.register_event_handler(win::slider_event([&] (int) {
          get_layout().layout(size());
        }));
      }

      template<>
      split_view<true>::split_view () {
        get_layout().set_slider(&slider);
        slider.register_event_handler(win::slider_event([&] (int) {
          get_layout().layout(size());
        }));
      }

    }

  } // win

} // gui