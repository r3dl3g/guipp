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
 * @brief     C++ API: basic window
 *
 * @file
 */

#pragma once


namespace gui {

  namespace core {

    template<typename T>
    inline void event_container::register_event_handler (char const name[],
                                                         T* t,
                                                         bool (T::*method)(const core::event &, os::event_result &)) {
      register_event_handler(name, bind_method(t, method));
    }

    inline event_container::event_handler_info::event_handler_info (char const* n, event_handler_function fn)
      : cb(fn)
      , name(n)
    {}

    inline bool event_container::event_handler_info::operator() (const event& ev, os::event_result& res) const {
      return cb(ev, res);
    }

    inline const std::type_info& event_container::event_handler_info::target_type () const {
      return cb.target_type();
    }

  } // core

} // gui
