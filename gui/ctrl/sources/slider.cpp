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
      slider_class<orientation::vertical>::slider_class ()
        : no_erase_window_class("VSLIDER++",
#ifdef WIN32
                       (os::color)(COLOR_BTNFACE + 1),
                       (ULONG_PTR)IDC_SIZEWE
#endif // WIN32
#ifdef X11
                       color::buttonColor(),
                       XC_sb_h_double_arrow
#endif // X11
        )
      {}

      template<>
      slider_class<orientation::horizontal>::slider_class ()
        : no_erase_window_class("HSLIDER++",
#ifdef WIN32
                       (os::color)(COLOR_BTNFACE + 1),
                       (ULONG_PTR)IDC_SIZENS
#endif // WIN32
#ifdef X11
                       color::buttonColor(),
                       XC_sb_v_double_arrow
#endif // X11
        )
      {}

      template <>
      slider_t<orientation::vertical>::slider_t () {
        register_event_handler(__PRETTY_FUNCTION__, win::mouse_move_abs_event([&] (os::key_state keys,
                                                              const core::point& p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            core::point_type new_x = std::min<core::point_type>(max, std::max<core::point::type>(min, start_window_point.x() + p.x() - start_mouse_point.x()));
            core::point_type dx = new_x - start_window_point.x();
            if (dx != 0) {
              start_mouse_point = p;
              start_window_point.x(new_x);
              move(start_window_point);
              send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(dx));
            }
          }
        }));
      }

      template <>
      slider_t<orientation::horizontal>::slider_t () {
        register_event_handler(__PRETTY_FUNCTION__, win::mouse_move_abs_event([&] (os::key_state keys,
                                                              const core::point& p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            core::point_type new_y = std::min<core::point_type>(max, std::max<core::point::type>(min, start_window_point.y() + p.y() - start_mouse_point.y()));
            core::point_type dy = new_y - start_window_point.y();
            if (dy != 0) {
              start_mouse_point = p;
              start_window_point.y(new_y);
              move(start_window_point);
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
        detail::init_control_messages();
#endif // X11
        register_event_handler(__PRETTY_FUNCTION__, left_btn_down_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          capture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = client_to_screen(pt);
          start_window_point = position();
        }));
        register_event_handler(__PRETTY_FUNCTION__, left_btn_up_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          uncapture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = core::point::undefined;
          start_window_point = core::point::undefined;
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
