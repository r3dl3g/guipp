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
 * @brief     list specific state informtion
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/list_state.h"


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    list_state::is::is (const state_type& state)
      : super::is(state)
    {}

    bool list_state::is::scroll_bar_enabled () const {
      return flags.scroll_bar_enabled;
    }
    
    // --------------------------------------------------------------------------
    list_state::set::set (state_type& state)
      : super::set(state)
    {}

    bool list_state::set::scroll_bar_enabled (bool on) {
      return (flags.scroll_bar_enabled == on ? false : flags.scroll_bar_enabled = on, true);
    }
    
    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const list_state::is& s) {
      out << static_cast<const window_state::is&>(s);
      if (s.scroll_bar_enabled()) out << " scroll_bar_enabled,";

      return out;
    }

  } // namespace core

} // namespace gui
