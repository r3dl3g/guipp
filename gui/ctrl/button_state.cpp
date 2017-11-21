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
#include <gui/ctrl/button.h>

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    button_state::button_state (const button_base& win)
      : super(win)
    {}

    bool button_state::is_hilited () const {
      return get_flag(flags::button_hilited);
    }

    bool button_state::is_pushed () const {
      return get_flag(flags::button_pushed);
    }

    bool button_state::is_checked () const {
      return get_flag(flags::button_checked);
    }

    bool button_state::set_hilited (bool h) {
      return set_flag(flags::button_hilited, h);
    }

    bool button_state::set_pushed (bool h) {
      return set_flag(flags::button_pushed, h);
    }

    bool button_state::set_checked (bool f) {
      return set_flag(flags::button_checked, f);
    }

  } // win

} // gui
