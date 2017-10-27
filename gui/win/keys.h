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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>

#ifdef WIN32
# include <gui/win/win32keys.h>
#endif // WIN32

#ifdef X11
# include <gui/win/x11keys.h>
#endif // X11

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    typedef bit_mask<os::key_state, state::left_button> left_button_bit_mask;
    typedef bit_mask<os::key_state, state::middle_button> middle_button_bit_mask;
    typedef bit_mask<os::key_state, state::right_button> right_button_bit_mask;

    typedef bit_mask<os::key_state, state::control> control_key_bit_mask;
    typedef bit_mask<os::key_state, state::shift> shift_key_bit_mask;
    typedef bit_mask<os::key_state, state::alt> alt_key_bit_mask;
    typedef bit_mask<os::key_state, state::system> system_key_bit_mask;

    typedef bit_mask<os::key_state, state::num_lock> num_lock_bit_mask;
    typedef bit_mask<os::key_state, state::scroll_lock> scroll_lock_bit_mask;
    typedef bit_mask<os::key_state, state::caps_lock> caps_lock_bit_mask;

    // --------------------------------------------------------------------------
    struct hot_key {
      typedef void (fn)();
      typedef std::function<fn> call;

      hot_key (os::key_symbol key = 0,
               os::key_state modifiers = 0);

      std::string get_key_string () const;
      bool match (os::key_state, os::key_symbol) const;

      bool empty () const;

      bool operator== (const hot_key&) const;
      bool operator< (const hot_key&) const;

      os::key_symbol get_key () const;
      os::key_state get_modifiers () const;

    private:
      os::key_symbol key;
      os::key_state modifiers;

      mutable std::string key_string;
    };

    // --------------------------------------------------------------------------
    // inlines
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