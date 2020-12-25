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

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window_state.h>
#include <gui++-ctrl-export.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      struct GUIPP_CTRL_EXPORT list_state : public win::window_state {
        typedef win::window_state super;

        struct GUIPP_CTRL_EXPORT is : public super::is {

          is (const win::state_type& state);

          bool moved () const;
          bool scroll_bar_enabled () const;
          bool grab_focus () const;

        };

        struct GUIPP_CTRL_EXPORT set : public super::set  {

          set (win::state_type& state);

          bool moved (bool b);
          bool scroll_bar_enabled (bool b);
          bool grab_focus (bool b);

        };

      protected:
        friend struct is;
        friend struct set;

        struct flags : public win::window_state::flags {
          enum {
            mouse_moved = last_window_state_enum,
            scroll_bar_enabled,
            grab_focus,
            last_list_state_enum
          };
        };

      };

    } // namespace detail

  } // namespace ctrl

} // namespace gui
