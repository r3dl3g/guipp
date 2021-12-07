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


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    template<typename H>
    void receiver::on (typename H::function&& f) {
      register_event_handler(H(std::move(f)), H::mask);
    }

    template<typename H>
    void receiver::on (const typename H::function& f) {
      register_event_handler(H(f), H::mask);
    }

    template<typename H>
    void receiver::unregister_event_handler (const typename H::function& f) {
      events.unregister_event_handler(H(f));
    }

    template<>
    inline void receiver::on_wheel<orientation_t::horizontal> (std::function<wheel_fn>&& f) {
      on_wheel_x(std::move(f));
    }

    template<>
    inline void receiver::on_wheel<orientation_t::vertical> (std::function<wheel_fn>&& f) {
      on_wheel_y(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    void receiver::on_key_down (std::function<notification_fn>&& f) {
      on<key_down_event<symbol, state>>(std::move(f));
    }

    template<os::key_symbol symbol, os::key_state state>
    void receiver::on_key_up (std::function<notification_fn>&& f) {
      on<key_up_event<symbol, state>>(std::move(f));
    }
    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
