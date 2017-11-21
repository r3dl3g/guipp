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

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class window;

    // --------------------------------------------------------------------------
    struct window_state {
      window_state (const window& win)
        : win(win)
      {}

      bool is_visible () const;
      bool has_focus () const;
      bool is_enabled () const;
      bool is_focus_accepting () const;
      bool is_redraw_disabled () const;

      bool set_enable (bool on);
      bool set_accept_focus (bool a);
      bool disable_redraw (bool on = true);

    protected:
      struct flags {
        enum {
          focus_accepting = 0,
          redraw_disabled,
          window_disabled,
          last_window_state_enum
        };
      };

      bool get_flag (byte bit) const;
      bool set_flag (byte bit, bool a);

      window& get_win ();

      const window& win;
    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui
