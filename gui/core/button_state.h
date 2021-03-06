/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://wwrothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     button state
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/window_state.h>


namespace gui {

  namespace core {

     // --------------------------------------------------------------------------
    struct GUIPP_CORE_EXPORT button_state : public window_state {
      typedef window_state super;

      struct GUIPP_CORE_EXPORT is : public super::is {

        explicit is (const state_type& state);

        bool pushed () const;
        bool hilited () const;
        bool checked () const;

      };

      struct GUIPP_CORE_EXPORT set : public super::set  {

        explicit set (state_type& state);

        bool hilited (bool b);
        bool pushed (bool b);
        bool checked (bool b);

      };

      struct GUIPP_CORE_EXPORT temp : public set, is  {

        explicit temp ();
        explicit temp (bool enabled, bool hilited, bool pushed, bool checked);

      private:
        state_type state;
      };
    };

    std::ostream& operator<< (std::ostream& out, const button_state::is&);

  } // namespace core

} // namespace gui
