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

      template <>
      basic_slider<orientation::vertical>::basic_slider () {
        register_event_handler(REGISTER_FUNCTION, mouse_move_abs_event([&](os::key_state keys,
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
        register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state state, os::key_symbol key, const std::string&) {
          core::point_type dx = 0;
          switch (key) {
           case keys::left:
              dx = -1 ;
           break;
           case keys::right:
              dx = 1;
           break;
           case keys::home:
              dx = -100000;
           break;
           case keys::end:
              dx = 100000;
           break;
          }
          if (dx) {
            if (control_key_bit_mask::is_set(state)) {
              dx *= 10;
            }
            core::point pos = position();
            core::point_type new_x = std::min<core::point_type>(max, std::max<core::point::type>(min, pos.x() + dx));
            dx = new_x - pos.x();
            if (dx) {
              pos.x(new_x);
              move(pos);
              send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(dx));
            }
          }
        }));
      }

      template <>
      basic_slider<orientation::horizontal>::basic_slider () {
        register_event_handler(REGISTER_FUNCTION, mouse_move_abs_event([&](os::key_state keys,
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
        register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state state, os::key_symbol key, const std::string&) {
          core::point_type dy = 0;
          switch (key) {
           case keys::up:
              dy = -1 ;
           break;
           case keys::down:
              dy = 1;
           break;
           case keys::home:
              dy = -100000;
           break;
           case keys::end:
              dy = 100000;
           break;
          }
          if (dy) {
            if (control_key_bit_mask::is_set(state)) {
              dy *= 10;
            }
            core::point pos = position();
            core::point_type new_y = std::min<core::point_type>(max, std::max<core::point::type>(min, pos.y() + dy));
            dy = new_y - pos.y();
            if (dy) {
              pos.y(new_y);
              move(pos);
              send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(dy));
            }
          }
        }));
      }

      slider_base::slider_base ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
#ifdef X11
        static int initialized = detail::init_control_messages();
#endif // X11

        set_accept_focus(true);

        register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          capture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = client_to_screen(pt);
          start_window_point = position();
          take_focus();
        }));

        register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point& pt) {
#ifndef NO_CAPTURE
          uncapture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = core::point::undefined;
          start_window_point = core::point::undefined;
        }));
        register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window*){
          redraw_later();
        }));
        register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*){
          redraw_later();
        }));
      }

      void slider_base::set_min (type i) {
        min = i;
        redraw_later();
      }

      void slider_base::set_max (type i) {
        max = i;
        redraw_later();
      }

      void slider_base::set_min_max (type mi, type ma) {
        min = mi;
        max = ma;
        redraw_later();
      }

    } // detail

  } // win

} // gui
