/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     helper to receive events and dispatch notifications
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/core/keys.h>
#include <gui/core/event_container.h>
#include <gui/win/window_event_handler.h>


// --------------------------------------------------------------------------
//
// Common includes
//


namespace gui {

  namespace win {

    class GUIPP_WIN_EXPORT receiver {
    public:
      typedef core::event_container::event_handler_function event_handler_function;

      receiver ();
      virtual ~receiver ();

      typedef void(notification_fn)();
      typedef void(mouse_fn)(os::key_state, core::native_point);
      typedef void(wheel_fn)(core::native_point::type, core::native_point);
      typedef void(move_fn)(core::point);
      typedef void(size_fn)(core::size);
      typedef void(place_fn)(core::rectangle);
      typedef void(key_down_fn)(os::key_state, os::key_symbol, std::string);
      typedef void(key_up_fn)(os::key_state, os::key_symbol);

      template<orientation_t V>
      void on_wheel (std::function<wheel_fn>&& f);

      template<os::key_symbol symbol, os::key_state state = core::state::none>
      void on_key_down (std::function<notification_fn>&& f);

      template<os::key_symbol symbol, os::key_state state = core::state::none>
      void on_key_up (std::function<notification_fn>&& f);

      void on_create (std::function<notification_fn>&& f);

      void on_close (std::function<notification_fn>&& f);

      void on_destroy (std::function<notification_fn>&& f);

      void on_any_key_down (std::function<key_down_fn>&& f);

      void on_any_key_up (std::function<key_up_fn>&& f);

      void on_mouse_move (std::function<mouse_fn>&& f);

      void on_left_btn_down (std::function<mouse_fn>&& f);

      void on_left_btn_up (std::function<mouse_fn>&& f);

      void on_right_btn_down (std::function<mouse_fn>&& f);

      void on_right_btn_up (std::function<mouse_fn>&& f);

      void on_middle_btn_down (std::function<mouse_fn>&& f);

      void on_middle_btn_up (std::function<mouse_fn>&& f);

      void on_btn_down (std::function<mouse_fn>&& f);

      void on_btn_up (std::function<mouse_fn>&& f);

      void on_left_btn_dblclk (std::function<mouse_fn>&& f);

      void on_right_btn_dblclk (std::function<mouse_fn>&& f);

      void on_middle_btn_dblclk (std::function<mouse_fn>&& f);

      void on_wheel_x (std::function<wheel_fn>&& f);

      void on_wheel_y (std::function<wheel_fn>&& f);

      void on_show (std::function<notification_fn>&& f);

      void on_hide (std::function<notification_fn>&& f);

      void on_set_focus (std::function<notification_fn>&& f);

      void on_lost_focus (std::function<notification_fn>&& f);

      void on_mouse_enter (std::function<notification_fn>&& f);

      void on_mouse_leave (std::function<notification_fn>&& f);

      void on_move (std::function<move_fn>&& f);

      void on_size (std::function<size_fn>&& f);

      void on_layout (std::function<place_fn>&& f);

      void on_paint (std::function<void(core::context*, core::native_rect*)>&& f);

      template<typename H>
      void on (typename H::function&& f);

      template<typename H>
      void on (const typename H::function& f);

      virtual void register_event_handler (event_handler_function&& f, os::event_id mask);

      template<typename H>
      void unregister_event_handler (const typename H::function& f);

      os::event_id get_event_mask () const;
      virtual void add_event_mask (os::event_id mask);

    protected:
      core::event_container events;
      os::event_id event_mask;
    };

  } // namespace win

} // namespace gui

#include <gui/win/receiver.inl>
