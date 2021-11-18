/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

#include <ostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/window_state.h>


namespace gui {

  namespace core {

    state_type::state_type ()
      : flags(0)
    {}

    // --------------------------------------------------------------------------
    window_state::is::is (const state_type& state)
      :flags(state)
    {}

    bool window_state::is::created () const {
      return flags.created;
    }

    bool window_state::is::visible () const {
      return flags.is_visible;
    }

    bool window_state::is::focused () const {
      return flags.is_focused;
    }

    bool window_state::is::focus_accepting () const {
      return flags.focus_accepting;
    }

    bool window_state::is::redraw_disabled () const {
      return flags.redraw_disabled;
    }

    bool window_state::is::enabled () const {
      return !flags.window_disabled;
    }

    bool window_state::is::overlapped () const {
      return flags.is_overlapped;
    }

    // --------------------------------------------------------------------------
    window_state::set::set (state_type& state)
      : is(state)
      , flags(state)
    {}

    bool window_state::set::created (bool on) {
      return (flags.created == on ? false : flags.created = on, true);
    }

    bool window_state::set::visible (bool on) {
      return (flags.is_visible == on ? false : flags.is_visible = on, true);
    }

    bool window_state::set::focused (bool on) {
      return (flags.is_focused == on ? false : flags.is_focused = on, true);
    }

    bool window_state::set::accept_focus (bool on) {
      return (flags.focus_accepting == on ? false : flags.focus_accepting = on, true);
    }

    bool window_state::set::disable_redraw (bool on) {
      return (flags.redraw_disabled == on ? false : flags.redraw_disabled = on, true);
    }

    bool window_state::set::overlapped (bool on) {
      return (flags.is_overlapped == on ? false : flags.is_overlapped = on, true);
    }

    bool window_state::set::enable (bool on) {
      return (flags.window_disabled == !on ? false : flags.window_disabled = !on, true);
    }

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const window_state::is& s) {
      if (s.created()) out << " created,";
      if (s.enabled()) out << " enabled,";
      if (s.visible()) out << " visible,";
      if (s.focused()) out << " focused,";
      if (s.focus_accepting()) out << " focus_accepting,";
      if (s.redraw_disabled()) out << " redraw_disabled,";
      if (s.overlapped()) out << " overlapped,";

      return out;
    }

  } // namespace core

} // namespace gui
