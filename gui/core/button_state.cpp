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
 * @brief     button state
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/button_state.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    button_state::is::is (const state_type& state)
      : super::is(state)
    {}

    bool button_state::is::hilited () const {
      return flags.button_hilited;
    }

    bool button_state::is::pushed () const {
      return flags.button_pushed;
    }

    bool button_state::is::checked () const {
      return flags.button_checked;
    }

    // --------------------------------------------------------------------------
    button_state::set::set (state_type& state)
      : super::set(state)
    {}

    bool button_state::set::hilited (bool on) {
      return (flags.button_hilited == on ? false : flags.button_hilited = on, true);
    }

    bool button_state::set::pushed (bool on) {
      return (flags.button_pushed == on ? false : flags.button_pushed = on, true);
    }

    bool button_state::set::checked (bool on) {
      return (flags.button_checked == on ? false : flags.button_checked = on, true);
    }

    // --------------------------------------------------------------------------
    button_state::temp::temp ()
      : set(state)
      , is(state)
    {}

    button_state::temp::temp (bool enabled_, bool hilited_, bool pushed_, bool checked_)
      : set(state)
      , is(state) {
      set::enable(enabled_);
      set::hilited(hilited_);
      set::pushed(pushed_);
      set::checked(checked_);
    }

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const button_state::is& s) {
      out << static_cast<const window_state::is&>(s);
      if (s.pushed()) out << " pushed,";
      if (s.hilited()) out << " hilited,";
      if (s.checked()) out << " checked,";

      return out;
    }

  } // core

} // gui
