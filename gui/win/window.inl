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
    inline void window::register_event_handler (char const name[], const H& h) {
      register_event_handler(name, h, h.mask);
    }

    template<typename H>
    inline void window::register_event_handler (char const name[], H&& h) {
      register_event_handler(name, std::move(h), h.mask);
    }

    template<typename T>
    inline void window::register_event_handler (char const name[], T* t,
                                                bool (T::*method)(const core::event &,
                                                                  os::event_result & result),
                                                os::event_id mask) {
      register_event_handler(name, core::bind_method(t, method), mask);
    }

    // --------------------------------------------------------------------------
    template<os::color B>
    inline client_window<B>::client_window ()
    {}

    template<os::color B>
    inline client_window<B>::client_window (const client_window& rhs)
      : super(rhs)
    {}

    template<os::color B>
    inline client_window<B>::client_window (client_window&& rhs)
      : super(rhs)
    {}

    template<os::color B>
    inline void client_window<B>::create (const container& parent,
                                          const core::rectangle& r) {
      window::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
