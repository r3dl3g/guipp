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
#include <gui++-look-export.h>


namespace gui {

  namespace ctrl {

     // --------------------------------------------------------------------------
    struct GUIPP_LOOK_EXPORT button_state : public win::window_state {
      typedef window_state super;

      struct GUIPP_LOOK_EXPORT is : public super::is {

        is (const win::state_type& state);

        bool pushed () const;
        bool hilited () const;
        bool checked () const;

      };

      struct GUIPP_LOOK_EXPORT set : public super::set  {

        set (win::state_type& state);

        bool hilited (bool b);
        bool pushed (bool b);
        bool checked (bool b);

      };

    protected:
      friend struct is;
      friend struct set;

      struct flags : public window_state::flags {
        enum {
          button_hilited = last_window_state_enum,
          button_pushed,
          button_checked,
          last_button_state_enum
        };
      };

    };

  } // namespace ctrl

} // namespace gui
