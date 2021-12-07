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
#include <gui/core/gui++-core-export.h>


#ifdef GUIPP_WIN
# include <gui/core/keys_win32.h>
#elif GUIPP_X11
# include <gui/core/keys_x11.h>
#elif GUIPP_QT
# include <gui/core/keys_qt.h>
#else
# error Unknown target system
#endif

namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    typedef core::bit_mask<gui::os::key_state, state::left_button> left_button_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::middle_button> middle_button_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::right_button> right_button_bit_mask;

    typedef core::bit_mask<gui::os::key_state, state::control> control_key_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::shift> shift_key_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::alt> alt_key_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::system> system_key_bit_mask;

    typedef core::bit_mask<gui::os::key_state, state::num_lock> num_lock_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::scroll_lock> scroll_lock_bit_mask;
    typedef core::bit_mask<gui::os::key_state, state::caps_lock> caps_lock_bit_mask;

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT hot_key {
      typedef void (fn)();
      typedef std::function<fn> call;

      explicit hot_key (gui::os::key_symbol key = 0,
                        gui::os::key_state modifiers = 0);

      std::string get_key_string () const;
      bool match (gui::os::key_state, gui::os::key_symbol) const;

      bool empty () const;

      bool operator== (const hot_key&) const;
      bool operator< (const hot_key&) const;

      gui::os::key_symbol get_key () const;
      gui::os::key_state get_modifiers () const;

    private:
      gui::os::key_symbol key;
      gui::os::key_state modifiers;

      mutable std::string key_string;
    };

  } // namespace core

} // namespace gui

#include <gui/core/keys.inl>
