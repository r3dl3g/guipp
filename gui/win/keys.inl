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
 * @brief     C++ API: key definitions
 *
 * @file
 */

#pragma once


namespace gui {

  namespace win {

    inline bool hot_key::match (os::key_state m, os::key_symbol k) const {
      return (key && (key == k) && ((m & modifiers) == modifiers));
    }

    inline bool hot_key::empty () const {
      return key == 0;
    }

    inline bool hot_key::operator== (const hot_key& rhs) const {
      return (key == rhs.key) && (modifiers == rhs.modifiers);
    }

    inline os::key_symbol hot_key::get_key () const {
      return key;
    }

    inline os::key_state hot_key::get_modifiers () const {
      return modifiers;
    }

  } // namespace win

} // namespace gui
