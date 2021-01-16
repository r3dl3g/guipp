/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    struct item_state {
      enum states {
        normal                    = 0x0,
        hilited                   = 0x1,
        selected                  = 0x2,
        hilited_selected          = 0x3,

        disabled                  = 0x4,
        disabled_hilited          = 0x5,
        disabled_selected         = 0x6,
        disabled_hilited_selected = 0x7
      };

      inline item_state (states state)
        : state(state)
      {}

      inline item_state (bool hilite, bool select, bool disable)
        : state((states)((int)hilite | ((int)select << 1) | ((int)disable << 2)))
      {}

      inline item_state operator& (item_state r) const {
        return item_state(static_cast<item_state::states>(state & r.state));
      }

      inline bool operator== (item_state r) const {
        return state == r.state;
      }

      inline operator states() const {
        return state;
      }

      inline bool is_normal () const {
        return (state & hilited_selected) == normal;
      }

      inline bool is_selected () const {
        return (state & selected) == selected;
      }

      inline bool is_hilited () const {
        return (state & hilited) == hilited;
      }

      inline bool is_disabled () const {
        return (state & disabled) == disabled;
      }

    private:
      states state;
    };

  } // namespace ctrl

} // namespace gui
