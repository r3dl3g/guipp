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
#include "guidefs.h"

#ifdef WIN32
# include "win32keys.h"
#endif // WIN32

#ifdef X11
# include "x11keys.h"
#endif // X11

namespace gui {

  namespace win {

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


    struct hot_key {
      typedef void(fn)();
      typedef std::function<fn> call;

      hot_key (os::key_symbol key = 0,
               os::key_state modifiers = 0);

      std::string get_key_string () const;
      bool match (os::key_state, os::key_symbol) const;

      inline bool empty () const {
        return key == 0;
      }

      bool operator== (const hot_key&) const;
      bool operator< (const hot_key&) const;

      os::key_symbol get_key () const {
        return key;
      }

      os::key_state get_modifiers () const {
        return modifiers;
      }

    private:
      os::key_symbol key;
      os::key_state modifiers;

      mutable std::string key_string;
    };

  } // win

} // gui
