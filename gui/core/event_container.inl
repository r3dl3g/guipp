/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     event registrar
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace core {

    template<typename T>
    inline void event_container::register_event_handler (T* t,
                                                         bool (T::*method)(const core::event &, gui::os::event_result &)) {
      register_event_handler(util::bind_method(t, method));
    }

  } // core

} // gui
