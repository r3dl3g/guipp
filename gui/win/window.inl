/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */

#pragma once


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    inline os::window window::get_id () const {
      return id;
    }

    inline window::operator os::drawable() const {
      return get_id();
    }

    inline bool window::get_flag (byte bit) const {
      return flags.test(bit);
    }

    inline void window::set_flag (byte bit, bool a) {
      flags.set(bit, a);
    }

    inline const window_state window::get_state () const {
      return window_state(*this);
    }

    inline window_state window::get_state () {
      return window_state(*this);
    }

    inline void window::set_accept_focus (bool a) {
      get_state().set_accept_focus(a);
    }

    inline bool window::is_focus_accepting () const {
      return get_state().is_focus_accepting();
    }

    inline void window::disable_redraw (bool on) {
      get_state().disable_redraw(on);
    }

    inline bool window::is_redraw_disabled () const {
      return get_state().is_redraw_disabled();
    }

    inline bool window::is_enabled () const {
      return get_state().is_enabled();
    }

    inline void window::enable (bool on) {
      get_state().set_enable(on);
    }

    inline void window::disable () {
      enable(false);
    }

    inline void window::on_create (create_event::function&& f) {
      register_event_handler(create_event(std::move(f)), create_event::mask);
    }

    inline void window::on_close (close_event::function&& f) {
      register_event_handler(close_event(std::move(f)), close_event::mask);
    }

    inline void window::on_destroy (destroy_event::function&& f) {
      register_event_handler(destroy_event(std::move(f)), destroy_event::mask);
    }

    inline void window::on_any_key_down (any_key_down_event::function&& f) {
      register_event_handler(any_key_down_event(std::move(f)), any_key_down_event::mask);
    }

    inline void window::on_any_key_up (any_key_up_event::function&& f) {
      register_event_handler(any_key_up_event(std::move(f)), any_key_up_event::mask);
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void window::on_key_down (std::function<notification_fn>&& f) {
      register_event_handler(key_down_event<symbol, state>(std::move(f)), key_down_event<symbol, state>::mask);
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void window::on_key_up (std::function<notification_fn>&& f) {
      register_event_handler(key_up_event<symbol, state>(std::move(f)), key_up_event<symbol, state>::mask);
    }

    inline void window::on_mouse_move (mouse_move_event::function&& f) {
      register_event_handler(mouse_move_event(std::move(f)), mouse_move_event::mask);
    }

    inline void window::on_mouse_move_abs (mouse_move_abs_event::function&& f) {
      register_event_handler(mouse_move_abs_event(std::move(f)), mouse_move_abs_event::mask);
    }

    inline void window::on_left_btn_down (left_btn_down_event::function&& f) {
      register_event_handler(left_btn_down_event(std::move(f)), left_btn_down_event::mask);
    }

    inline void window::on_left_btn_up (left_btn_up_event::function&& f) {
      register_event_handler(left_btn_up_event(std::move(f)), left_btn_up_event::mask);
    }

    inline void window::on_right_btn_down (right_btn_down_event::function&& f) {
      register_event_handler(right_btn_down_event(std::move(f)), right_btn_down_event::mask);
    }

    inline void window::on_right_btn_up (right_btn_up_event::function&& f) {
      register_event_handler(right_btn_up_event(std::move(f)), right_btn_up_event::mask);
    }

    inline void window::on_middle_btn_down (middle_btn_down_event::function&& f) {
      register_event_handler(middle_btn_down_event(std::move(f)), middle_btn_down_event::mask);
    }

    inline void window::on_middle_btn_up (middle_btn_up_event::function&& f) {
      register_event_handler(middle_btn_up_event(std::move(f)), middle_btn_up_event::mask);
    }

    inline void window::on_btn_down (btn_down_event::function&& f) {
      register_event_handler(btn_down_event(std::move(f)), btn_down_event::mask);
    }

    inline void window::on_btn_up (btn_up_event::function&& f) {
      register_event_handler(btn_up_event(std::move(f)), btn_up_event::mask);
    }

    inline void window::on_left_btn_dblclk (left_btn_dblclk_event::function&& f) {
      register_event_handler(left_btn_dblclk_event(std::move(f)), left_btn_dblclk_event::mask);
    }

    inline void window::on_right_btn_dblclk (right_btn_dblclk_event::function&& f) {
      register_event_handler(right_btn_dblclk_event(std::move(f)), right_btn_dblclk_event::mask);
    }

    inline void window::on_middle_btn_dblclk (middle_btn_dblclk_event::function&& f) {
      register_event_handler(middle_btn_dblclk_event(std::move(f)), middle_btn_dblclk_event::mask);
    }

    inline void window::on_wheel_x (wheel_x_event::function&& f) {
      register_event_handler(wheel_x_event(std::move(f)), wheel_x_event::mask);
    }

    inline void window::on_wheel_y (wheel_y_event::function&& f) {
      register_event_handler(wheel_y_event(std::move(f)), wheel_y_event::mask);
    }

    inline void window::on_show (show_event::function&& f) {
      register_event_handler(show_event(std::move(f)), show_event::mask);
    }

    inline void window::on_hide (hide_event::function&& f) {
      register_event_handler(hide_event(std::move(f)), hide_event::mask);
    }

    inline void window::on_set_focus (set_focus_event::function&& f) {
      register_event_handler(set_focus_event(std::move(f)), set_focus_event::mask);
    }

    inline void window::on_lost_focus (lost_focus_event::function&& f) {
      register_event_handler(lost_focus_event(std::move(f)), lost_focus_event::mask);
    }

    inline void window::on_mouse_enter (mouse_enter_event::function&& f) {
      register_event_handler(mouse_enter_event(std::move(f)), mouse_enter_event::mask);
    }

    inline void window::on_mouse_leave (mouse_leave_event::function&& f) {
      register_event_handler(mouse_leave_event(std::move(f)), mouse_leave_event::mask);
    }

    inline void window::on_move (move_event::function&& f) {
      register_event_handler(move_event(std::move(f)), move_event::mask);
    }

    inline void window::on_size (size_event::function&& f) {
      register_event_handler(size_event(std::move(f)), size_event::mask);
    }

    inline void window::on_place (place_event::function&& f) {
      register_event_handler(place_event(std::move(f)), place_event::mask);
    }

    inline void window::on_moving (moving_event::function&& f) {
      register_event_handler(moving_event(std::move(f)), moving_event::mask);
    }

    inline void window::on_sizing (sizing_event::function&& f) {
      register_event_handler(sizing_event(std::move(f)), sizing_event::mask);
    }

    inline void window::on_placing (placing_event::function&& f) {
      register_event_handler(placing_event(std::move(f)), placing_event::mask);
    }

    inline void window::on_layout (layout_event::function&& f) {
      register_event_handler(layout_event(std::move(f)), layout_event::mask);
    }

    inline void window::on_os_paint (os_paint_event::function&& f) {
      register_event_handler(os_paint_event(std::move(f)), os_paint_event::mask);
    }

    template<typename H>
    inline void window::register_event_handler (H&& h) {
      register_event_handler(std::move(h), h.mask);
    }

    // --------------------------------------------------------------------------
    template<os::color B>
    inline void client_window<B>::create (const container& parent,
                                          const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
