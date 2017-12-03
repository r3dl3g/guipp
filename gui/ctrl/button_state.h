/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: button state
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window_state.h>


namespace gui {

  namespace win {

    class button_base;

     // --------------------------------------------------------------------------
    struct GUIPP_EXPORT button_state : public window_state {
      typedef window_state super;

      button_state (const button_base& win);

      bool is_pushed () const;
      bool is_hilited () const;
      bool is_checked () const;

      bool set_hilited (bool b);
      bool set_pushed (bool b);
      bool set_checked (bool b);

    protected:
      struct flags : public window_state::flags {
        enum {
          button_hilited = last_window_state_enum,
          button_pushed,
          button_checked,
          last_button_state_enum
        };
      };

    };

  } // namespace win

} // namespace gui
