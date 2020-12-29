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

    // --------------------------------------------------------------------------
    template<os::color B>
    inline void client_window<B>::create (container& parent,
                                          const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    // --------------------------------------------------------------------------

  } // namespace win

} // namespace gui
