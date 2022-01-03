/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     window state information
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>
#include <cstring>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui++-core-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    union GUIPP_CORE_EXPORT state_type {
      struct {
        bool created:1;
        bool focus_accepting:1;
        bool redraw_disabled:1;
        bool window_disabled:1;
        bool is_visible:1;
        bool is_focused:1;
        bool is_overlapped:1;
        bool button_hilited:1;
        bool button_pushed:1;
        bool button_checked:1;
        bool mouse_moved:1;
        bool scroll_bar_enabled:1;
        bool grab_focus:1;
      };
      unsigned short flags;

      state_type ();
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT window_state {
    public:
      struct GUIPP_CORE_EXPORT is {

        explicit is (const state_type& state);

        bool created () const;
        bool enabled () const;
        bool visible () const;
        bool focused () const;
        bool focus_accepting () const;
        bool redraw_disabled () const;
        bool overlapped () const;

      protected:
        const state_type& flags;
      };

      struct GUIPP_CORE_EXPORT set {

        explicit set (state_type& state);

        bool created (bool on);
        bool enable (bool on);
        bool visible (bool on);
        bool focused (bool on);
        bool accept_focus (bool a);
        bool disable_redraw (bool on = true);
        bool overlapped (bool on);

      protected:
        state_type& flags;
      };

    };

    std::ostream& operator<< (std::ostream& out, const window_state::is&);

    // --------------------------------------------------------------------------
  } // namespace core

} // namespace gui
