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

  namespace win {

    inline void main_window::create (const core::rectangle& r) {
      create(clazz::get(), r);
    }

    inline void popup_window::create (const container& parent, const core::rectangle& r) {
      create(clazz::get(), parent, r);
    }

    inline void dialog_window::create (const container& parent, const core::rectangle& r) {
      create(clazz::get(), parent, r);
    }

  } // namespace win

} // namespace gui
