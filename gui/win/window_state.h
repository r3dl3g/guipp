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
// Common includes
//
#include <bitset>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui++-win-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    typedef std::bitset<sizeof(ulong) * 8> state_type;

    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT window_state {
    protected:
      struct flags {
        enum {
          focus_accepting = 0,
          redraw_disabled,
          window_disabled,
          is_visible,
          is_focused,
          is_overlapped,
          last_window_state_enum
        };
      };

    public:
      struct is {

        is (const state_type& state);

        bool enabled () const;
        bool visible () const;
        bool focused () const;
        bool focus_accepting () const;
        bool redraw_disabled () const;
        bool overlapped () const;

      protected:
        bool test (ulong) const;

      private:
        const state_type& state;
      };

      struct set {

        set (state_type& state);

        bool enable (bool on);
        bool visible (bool on);
        bool focused (bool on);
        bool accept_focus (bool a);
        bool disable_redraw (bool on = true);
        bool overlapped (bool on);

      protected:
        bool set_flag (ulong, bool a);

      private:
        state_type& state;
      };

    protected:
      friend struct is;
      friend struct set;

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui
