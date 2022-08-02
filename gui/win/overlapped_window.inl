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
 * @brief     window that can overlap windows of other applications
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace win {

    inline os::window overlapped_window::get_os_window () const {
      return id;
    }

    inline void overlapped_window::set_os_window (os::window w) {
      id = w;
    }

    inline void main_window::create (const core::rectangle& r, bool adjust_size) {
      create(clazz::get(), r, adjust_size);
    }

    inline void popup_window::create (overlapped_window& parent, const core::rectangle& r, bool adjust_size) {
      create(clazz::get(), parent, r, adjust_size);
    }

    inline void dialog_window::create (overlapped_window& parent, const core::rectangle& r, bool adjust_size) {
      create(clazz::get(), parent, r, adjust_size);
    }

  } // namespace win

} // namespace gui
