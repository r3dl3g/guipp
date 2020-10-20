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
#include <gui++-win-export.h>

namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class widget;

    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT widget_state {
      widget_state (const widget& win)
        : win(win)
      {}

      bool is_visible () const;
      bool has_focus () const;
      bool is_enabled () const;
      bool is_focus_accepting () const;
      bool is_redraw_disabled () const;

      bool set_enable (bool on);
      bool set_visible (bool on);
      bool set_accept_focus (bool a);
      bool disable_redraw (bool on = true);

      bool set_needs_redraw (bool on) const;
      bool needs_redraw () const;

    protected:
      struct flags {
        enum {
          focus_accepting = 0,
          redraw_disabled,
          window_disabled,
          needs_redraw,
          is_visible,
          last_window_state_enum
        };
      };

      bool get_flag (byte bit) const;
      bool set_flag (byte bit, bool a) const;

      widget& get_win () const;

      const widget& win;
    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui