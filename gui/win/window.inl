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

    template<typename H>
    inline void window::on (typename H::function&& f) {
      register_event_handler(H(std::move(f)), H::mask);
    }

    template<typename H>
    inline void window::on (const typename H::function& f) {
      register_event_handler(H(f), H::mask);
    }

    template<typename H>
    void window::unregister_event_handler (const typename H::function& f) {
      events.unregister_event_handler(H(f));
    }

    template<>
    inline void window::on_wheel<orientation::horizontal> (std::function<wheel_fn>&& f) {
      on_wheel_x(std::move(f));
    }

    template<>
    inline void window::on_wheel<orientation::vertical> (std::function<wheel_fn>&& f) {
      on_wheel_y(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void window::on_key_down (std::function<notification_fn>&& f) {
      on<key_down_event<symbol, state>>(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    inline void window::on_key_up (std::function<notification_fn>&& f) {
      on<key_up_event<symbol, state>>(std::move(f));
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
