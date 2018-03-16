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
 * @brief     C++ API: list
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      list_state::list_state (const list_base& win)
        : super(win)
      {}

      bool list_state::is_moved () const {
        return get_flag(flags::mouse_moved);
      }

      bool list_state::is_scroll_bar_enabled () const {
        return get_flag(flags::scroll_bar_enabled);
      }

      bool list_state::is_grab_focus () const {
        return get_flag(flags::grab_focus);
      }

      bool list_state::set_moved (bool h) {
        return set_flag(flags::mouse_moved, h);
      }

      bool list_state::set_scroll_bar_enabled (bool h) {
        return set_flag(flags::scroll_bar_enabled, h);
      }

      bool list_state::set_grab_focus (bool f) {
        return set_flag(flags::grab_focus, f);
      }

    } // namespace detail

  } // namespace ctrl

} // namespace gui
