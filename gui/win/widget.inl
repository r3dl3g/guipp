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
    inline widget::operator os::drawable() const {
      return get_id();
    }

    inline bool widget::get_flag (byte bit) const {
      return flags.test(bit);
    }

    inline void widget::set_flag (byte bit, bool a) {
      flags.set(bit, a);
    }

    inline const widget_state widget::get_state () const {
      return widget_state(*this);
    }

    inline widget_state widget::get_state () {
      return widget_state(*this);
    }

    inline void widget::set_accept_focus (bool a) {
      get_state().set_accept_focus(a);
    }

    inline bool widget::is_focus_accepting () const {
      return get_state().is_focus_accepting();
    }

    inline void widget::disable_redraw (bool on) {
      get_state().disable_redraw(on);
    }

    inline bool widget::is_redraw_disabled () const {
      return get_state().is_redraw_disabled();
    }

    inline bool widget::is_enabled () const {
      return get_state().is_enabled();
    }

    inline void widget::enable (bool on) {
      get_state().set_enable(on);
    }

    inline void widget::disable () {
      enable(false);
    }

    template<typename H>
    inline void widget::on (typename H::function&& f) {
      register_event_handler(H(std::move(f)), H::mask);
    }

    template<typename H>
    inline void widget::on (const typename H::function& f) {
      register_event_handler(H(f), H::mask);
    }

    template<typename H>
    void widget::unregister_event_handler (const typename H::function& f) {
      events.unregister_event_handler(H(f));
    }

    template<>
    inline void widget::on_wheel<orientation_t::horizontal> (std::function<wheel_fn>&& f) {
      on_wheel_x(std::move(f));
    }

    template<>
    inline void widget::on_wheel<orientation_t::vertical> (std::function<wheel_fn>&& f) {
      on_wheel_y(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void widget::on_key_down (std::function<notification_fn>&& f) {
      on<key_down_event<symbol, state>>(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void widget::on_key_up (std::function<notification_fn>&& f) {
      on<key_up_event<symbol, state>>(std::move(f));
    }

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
