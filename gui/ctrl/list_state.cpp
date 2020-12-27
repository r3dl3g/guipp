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
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      list_state::is::is (const win::state_type& state)
        : super::is(state)
      {}

      bool list_state::is::moved () const {
        return test(flags::mouse_moved);
      }

      bool list_state::is::scroll_bar_enabled () const {
        return test(flags::scroll_bar_enabled);
      }

      bool list_state::is::grab_focus () const {
        return test(flags::grab_focus);
      }

      // --------------------------------------------------------------------------
      list_state::set::set (win::state_type& state)
        : super::set(state)
      {}

      bool list_state::set::moved (bool h) {
        return set_flag(flags::mouse_moved, h);
      }

      bool list_state::set::scroll_bar_enabled (bool h) {
        return set_flag(flags::scroll_bar_enabled, h);
      }

      bool list_state::set::grab_focus (bool f) {
        return set_flag(flags::grab_focus, f);
      }

    } // namespace detail

  } // namespace ctrl

} // namespace gui
