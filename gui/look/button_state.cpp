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
 * @brief     C++ API: button state
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/look/button_state.h>

namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    button_state::is::is (const win::state_type& state)
      : super::is(state)
    {}

    bool button_state::is::hilited () const {
      return test(flags::button_hilited);
    }

    bool button_state::is::pushed () const {
      return test(flags::button_pushed);
    }

    bool button_state::is::checked () const {
      return test(flags::button_checked);
    }

    // --------------------------------------------------------------------------
    button_state::set::set (win::state_type& state)
      : super::set(state)
    {}

    bool button_state::set::hilited (bool h) {
      return set_flag(flags::button_hilited, h);
    }

    bool button_state::set::pushed (bool h) {
      return set_flag(flags::button_pushed, h);
    }

    bool button_state::set::checked (bool f) {
      return set_flag(flags::button_checked, f);
    }

  } // ctrl

} // gui
