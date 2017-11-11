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

    inline os::window window::get_id () const {
      return id;
    }

    inline window::operator os::drawable() const {
      return id;
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
