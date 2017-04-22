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
#ifdef X11
#include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include "slider.h"

//#define NO_CAPTURE

namespace gui {

  namespace win {

    namespace detail {

      template<>
      slider_class<false>::slider_class ()
        : no_erase_window_class("VSLIDER++",
#ifdef WIN32
                       (os::color)(COLOR_BTNFACE + 1),
                       IDC_SIZEWE
#endif // WIN32
#ifdef X11
                       color::buttonColor(),
                       XC_sb_h_double_arrow
#endif // X11
        )
      {}

      template<>
      slider_class<true>::slider_class ()
        : no_erase_window_class("HSLIDER++",
#ifdef WIN32
                       (os::color)(COLOR_BTNFACE + 1),
                       IDC_SIZENS
#endif // WIN32
#ifdef X11
                       color::buttonColor(),
                       XC_sb_v_double_arrow
#endif // X11
        )
      {}

      template <>
      slider_t<false>::slider_t () {
        register_event_handler(win::mouse_move_event([&] (os::key_state keys,
                                                          const core::point& p) {
          if ((last_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            core::point pt = position();
            core::point::type new_x = std::min<core::point::type>(max, std::max<core::point::type>(min, pt.x() + p.x() - last_mouse_point.x()));
            core::point::type dx = new_x - pt.x();
            if (dx != 0) {
              pt.x(new_x);
              move(pt);
              send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(dx));
            }
          }
        }));
      }

      template <>
      slider_t<true>::slider_t () {
        register_event_handler(win::mouse_move_event([&] (os::key_state keys,
                                                          const core::point& p) {
          if ((last_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            core::point pt = position();
            core::point::type new_y = std::min<core::point::type>(max, std::max<core::point::type>(min, pt.y() + p.y() - last_mouse_point.y()));
            core::point::type dy = new_y - pt.y();
            if (dy != 0) {
              pt.y(new_y);
              move(pt);
              send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(dy));
            }
           }
          return;
        }));
      }

      slider::slider ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
#ifdef X11
        if (!detail::SLIDER_MESSAGE) {
          detail::SLIDER_MESSAGE = XInternAtom(core::global::get_instance(), "SLIDER_MESSAGE", False);
        }
#endif // X11
        register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          capture_pointer();
#endif // NO_CAPTURE
          last_mouse_point = pt;
        }));
        register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          uncapture_pointer();
#endif // NO_CAPTURE
          last_mouse_point = core::point::undefined;
        }));
      }

      void slider::set_min (type i) {
        min = i;
        redraw_later();
      }

      void slider::set_max (type i) {
        max = i;
        redraw_later();
      }

      void slider::set_min_max (type mi, type ma) {
        min = mi;
        max = ma;
        redraw_later();
      }

    } // detail

  } // win

} // gui
