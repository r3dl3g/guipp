/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

    inline void window::set_id (os::window w) {
      id = w;
    }

    inline core::window_state::is window::get_state () const {
      return core::window_state::is(flags);
    }

    inline core::window_state::set window::set_state () {
      return core::window_state::set(flags);
    }

    inline void window::set_disable_redraw (bool on) {
      set_state().disable_redraw(on);
    }

    inline bool window::is_redraw_disabled () const {
      return get_state().redraw_disabled();
    }

    inline bool window::is_enabled () const {
      return get_state().enabled();
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
    inline void window::on_wheel<orientation_t::horizontal> (std::function<wheel_fn>&& f) {
      on_wheel_x(std::move(f));
    }

    template<>
    inline void window::on_wheel<orientation_t::vertical> (std::function<wheel_fn>&& f) {
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
    inline void client_window<B>::create (container& parent,
                                          const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
