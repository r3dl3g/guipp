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
    window_state::is::is (const state_type& state)
      :state(state)
    {}

    bool window_state::is::test (ulong f) const {
      return state.test(f);
    }

    bool window_state::is::visible () const {
      return test(flags::is_visible);
    }

    bool window_state::is::focused () const {
      return test(flags::is_focused);
    }

    bool window_state::is::focus_accepting () const {
      return test(flags::focus_accepting);
    }

    bool window_state::is::redraw_disabled () const {
      return test(flags::redraw_disabled);
    }

    bool window_state::is::enabled () const {
      return !test(flags::window_disabled);
//#ifdef GUIPP_WIN
//      return win.is_valid() && IsWindowEnabled(win.get_id());
//#elif GUIPP_X11
//#elif GUIPP_QT
//      return win.is_valid() && win.get_id()->isEnabled();
//#else
//# error Undefined system: window_state::is_enabled ()
//#endif // GUIPP_X11
    }

    bool window_state::is::overlapped () const {
      return test(flags::is_overlapped);
    }

    // --------------------------------------------------------------------------
    window_state::set::set (state_type& state)
      :state(state)
    {}

    bool window_state::set::set_flag (ulong bit, bool ona) {
      if (state.test(bit) != ona) {
        state.set(bit, ona);
        return true;
      }
      return false;
    }

    bool window_state::set::visible (bool on) {
      return set_flag(flags::is_visible, on);
    }

    bool window_state::set::focused (bool on) {
      return set_flag(flags::is_focused, on);
    }

    bool window_state::set::accept_focus (bool on) {
      return set_flag(flags::focus_accepting, on);
    }

    bool window_state::set::disable_redraw (bool on) {
      return set_flag(flags::redraw_disabled, on);
    }

    bool window_state::set::overlapped (bool on) {
      return set_flag(flags::is_overlapped, on);
    }

    bool window_state::set::enable (bool on) {
      return set_flag(flags::window_disabled, !on);
    }

  } // namespace win

} // namespace gui
