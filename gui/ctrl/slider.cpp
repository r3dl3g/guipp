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
# include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/slider.h>


//#define NO_CAPTURE

namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation::vertical>::basic_slider () {
        register_event_handler(REGISTER_FUNCTION, mouse_move_abs_event([&](os::key_state keys,
                                                                       const core::point & p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            set_value(start_window_point.x() + p.x() - start_mouse_point.x());
          }
        }));
        register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case keys::left:
               set_value(position().x() - (control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case keys::right:
               set_value(position().x() + (control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case keys::home:
               set_value(min);
               break;
             case keys::end:
               set_value(max);
               break;
           }
         }));
      }

      template<>
      void basic_slider<orientation::vertical>::set_value (core::point::type v) {
        const auto new_x = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_x != pt.x()) {
          move(core::point(new_x, pt.y()), true);
          send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(new_x - pt.x()));
        }

      }

      template<>
      core::point::type basic_slider<orientation::vertical>::get_value () const {
        return position().x();
      }

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation::horizontal>::basic_slider () {
        register_event_handler(REGISTER_FUNCTION, mouse_move_abs_event([&](os::key_state keys,
                                                                           const core::point & p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && left_button_bit_mask::is_set(keys)) {
            set_value(start_window_point.y() + p.y() - start_mouse_point.y());
          }
          return;
        }));
        register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case keys::up:
               set_value(position().y() - (control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case keys::down:
               set_value(position().y() + (control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case keys::home:
               set_value(min);
               break;
             case keys::end:
               set_value(max);
               break;
           }
        }));
      }

      template<>
      void basic_slider<orientation::horizontal>::set_value (core::point::type v) {
        const auto new_y = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_y != pt.y()) {
          move(core::point(pt.x(), new_y), true);
          send_client_message(this, detail::SLIDER_MESSAGE, static_cast<long>(new_y - pt.y()));
        }

      }

      template<>
      core::point::type basic_slider<orientation::horizontal>::get_value () const {
        return position().y();
      }

      // --------------------------------------------------------------------------
      slider_base::slider_base ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
#ifdef X11
        static int initialized = detail::init_control_messages();
#endif // X11

        set_accept_focus(true);

        register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&] (os::key_state, const core::point & pt) {
#ifndef NO_CAPTURE
          capture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = client_to_screen(pt);
          start_window_point = position();
          take_focus();
        }));

        register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&] (os::key_state, const core::point & pt) {
#ifndef NO_CAPTURE
          uncapture_pointer();
#endif // NO_CAPTURE
          start_mouse_point = core::point::undefined;
          start_window_point = core::point::undefined;
        }));
        register_event_handler(REGISTER_FUNCTION, set_focus_event([&] (window*) {
          redraw();
        }));
        register_event_handler(REGISTER_FUNCTION, lost_focus_event([&] (window*) {
          redraw();
        }));
      }

      void slider_base::set_min (type i) {
        if (min != i) {
          min = i;
          redraw();
        }
      }

      void slider_base::set_max (type i) {
        if (max != i) {
          max = i;
          redraw();
        }
      }

      void slider_base::set_min_max (type mi, type ma) {
        if ((min != mi) || (max != ma)) {
          min = mi;
          max = ma;
          redraw();
        }
      }

    } // detail

  } // win

} // gui
