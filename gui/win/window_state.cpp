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
#include <gui/win/window.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    bool window_state::get_flag (byte bit) const {
      return win.get_flag(bit);
    }

    bool window_state::is_visible () const {
//      return win.is_visible();
      return get_flag(flags::is_visible);
    }

    bool window_state::has_focus () const {
      return win.has_focus();
    }

    bool window_state::is_focus_accepting () const {
      return get_flag(flags::focus_accepting);
    }

    bool window_state::is_redraw_disabled () const {
      return get_flag(flags::redraw_disabled);
    }

    bool window_state::is_enabled () const {
#ifdef WIN32
      return win.is_valid() && IsWindowEnabled(win.get_id());
#endif // WIN32
#ifdef X11
      return !get_flag(flags::window_disabled);
#endif // X11
    }

    bool window_state::set_visible (bool a) {
      return set_flag(flags::is_visible, a);
    }

    window& window_state::get_win () const {
      return const_cast<window&>(win);
    }

    bool window_state::set_flag (byte bit, bool a) const {
      if (win.get_flag(bit) != a) {
        get_win().set_flag(bit, a);
        return true;
      }
      return false;
    }

    bool window_state::set_accept_focus (bool a) {
      return set_flag(flags::focus_accepting, a);
    }

    bool window_state::disable_redraw (bool on) {
      return set_flag(flags::redraw_disabled, on);
    }

    bool window_state::set_in_event_handle (bool on) {
      return set_flag(flags::in_event_handle, on);
    }

    bool window_state::is_in_event_handle () const {
      return get_flag(flags::in_event_handle);
    }

    bool window_state::set_needs_redraw (bool on) const {
      return set_flag(flags::needs_redraw, on);
    }

    bool window_state::needs_redraw () const {
      return get_flag(flags::needs_redraw);
    }

#ifdef WIN32
    bool window_state::set_enable (bool on) {
      if (win.is_valid() && (is_enabled() != on)) {
        EnableWindow(win.get_id(), on);
        get_win().redraw();
        return true;
      }
      return false;
    }

#endif // WIN32
#ifdef X11

    namespace x11 {
      bool check_return (int r);
    }

    bool window_state::set_enable (bool on) {
      if (set_flag(flags::window_disabled, !on)) {
        if (win.get_window_class().get_cursor()) {
          unsigned long mask = CWCursor;
          XSetWindowAttributes wa = {0};
          wa.cursor = on ? win.get_window_class().get_cursor()
                      : (os::cursor)win::cursor::arrow();
          x11::check_return(XChangeWindowAttributes(core::global::get_instance(), win.get_id(), mask, &wa));
        }
        get_win().redraw();
        return true;
      }
      return false;
    }

#endif // X11

  } // namespace win

} // namespace gui
