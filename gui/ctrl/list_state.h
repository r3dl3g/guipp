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
#include <gui/win/widget_state.h>
#include <gui++-ctrl-export.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      class list_base;

      // --------------------------------------------------------------------------
      struct GUIPP_CTRL_EXPORT list_state : public win::widget_state {
        typedef win::widget_state super;

        list_state (const list_base& win);

        bool is_moved () const;
        bool is_scroll_bar_enabled () const;
        bool is_grab_focus () const;

        bool set_moved (bool b);
        bool set_scroll_bar_enabled (bool b);
        bool set_grab_focus (bool b);

      protected:
        struct flags : public win::widget_state::flags {
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
