/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/widget.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    widget& widget_state::get_win () const {
      return const_cast<widget&>(win);
    }

    bool widget_state::get_flag (byte bit) const {
      return win.get_flag(bit);
    }

    bool widget_state::set_flag (byte bit, bool a) const {
      if (win.get_flag(bit) != a) {
        get_win().set_flag(bit, a);
        return true;
      }
      return false;
    }

    bool widget_state::is_visible () const {
      return get_flag(flags::is_visible);
    }

    bool widget_state::has_focus () const {
      return win.has_focus();
    }

    bool widget_state::is_focus_accepting () const {
      return get_flag(flags::focus_accepting);
    }

    bool widget_state::is_redraw_disabled () const {
      return get_flag(flags::redraw_disabled);
    }

    bool widget_state::is_enabled () const {
      return !get_flag(flags::window_disabled);
    }

    bool widget_state::set_visible (bool a) {
      return set_flag(flags::is_visible, a);
    }

    bool widget_state::set_accept_focus (bool a) {
      return set_flag(flags::focus_accepting, a);
    }

    bool widget_state::disable_redraw (bool on) {
      return set_flag(flags::redraw_disabled, on);
    }

    bool widget_state::set_needs_redraw (bool on) const {
      return set_flag(flags::needs_redraw, on);
    }

    bool widget_state::needs_redraw () const {
      return get_flag(flags::needs_redraw);
    }

    bool widget_state::set_enable (bool on) {
      return set_flag(flags::window_disabled, !on);
    }

  } // namespace win

} // namespace gui
