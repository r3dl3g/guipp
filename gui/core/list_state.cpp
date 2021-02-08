/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: list
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/list_state.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    list_state::is::is (const state_type& state)
      : super::is(state)
    {}

    bool list_state::is::moved () const {
      return flags.mouse_moved;
    }

    bool list_state::is::scroll_bar_enabled () const {
      return flags.scroll_bar_enabled;
    }

    bool list_state::is::grab_focus () const {
      return flags.grab_focus;
    }

    // --------------------------------------------------------------------------
    list_state::set::set (state_type& state)
      : super::set(state)
    {}

    bool list_state::set::moved (bool on) {
      return (flags.mouse_moved == on ? false : flags.mouse_moved = on, true);
    }

    bool list_state::set::scroll_bar_enabled (bool on) {
      return (flags.scroll_bar_enabled == on ? false : flags.scroll_bar_enabled = on, true);
    }

    bool list_state::set::grab_focus (bool on) {
      return (flags.grab_focus == on ? false : flags.grab_focus = on, true);
    }

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const list_state::is& s) {
      out << static_cast<const window_state::is&>(s);
      if (s.moved()) out << " moved,";
      if (s.scroll_bar_enabled()) out << " scroll_bar_enabled,";
      if (s.grab_focus()) out << " grab_focus,";

      return out;
    }

  } // namespace core

} // namespace gui
