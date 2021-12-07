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
 * @brief     key definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace core {

    inline bool hot_key::match (gui::os::key_state m, gui::os::key_symbol k) const {
      return (key && (key == k) && ((m & modifiers) == modifiers));
    }

    inline bool hot_key::empty () const {
      return key == 0;
    }

    inline bool hot_key::operator== (const hot_key& rhs) const {
      return (key == rhs.key) && (modifiers == rhs.modifiers);
    }

    inline gui::os::key_symbol hot_key::get_key () const {
      return key;
    }

    inline gui::os::key_state hot_key::get_modifiers () const {
      return modifiers;
    }

  } // namespace core

} // namespace gui
