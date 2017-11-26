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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/tile_view.h>


namespace gui {

  namespace win {

    template<>
    void basic_tile_view<orientation::horizontal>::handle_direction_key (os::key_symbol key) {
      switch (key) {
        case keys::left:
        case keys::numpad::left: {
          try_to_select(super::get_selection() - static_cast<int>(super::traits.get_items_per_line(super::client_size())), event_source::keyboard);
          break;
        }
        case keys::right:
        case keys::numpad::right: {
          try_to_select(super::get_selection() + static_cast<int>(super::traits.get_items_per_line(super::client_size())), event_source::keyboard);
          break;
        }
        case keys::up:
        case keys::numpad::up: {
          try_to_select(super::get_selection() - 1, event_source::keyboard);
          break;
        }
        case keys::down:
        case keys::numpad::down: {
          try_to_select(super::get_selection() + 1, event_source::keyboard);
          break;
        }
      }
    }

    template<>
    void basic_tile_view<orientation::vertical>::handle_direction_key (os::key_symbol key) {
      switch (key) {
        case keys::left:
        case keys::numpad::left: {
          try_to_select(super::get_selection() - 1, event_source::keyboard);
          break;
        }
        case keys::right:
        case keys::numpad::right: {
          try_to_select(super::get_selection() + 1, event_source::keyboard);
          break;
        }
        case keys::up:
        case keys::numpad::up: {
          try_to_select(super::get_selection() - static_cast<int>(super::traits.get_items_per_line(super::client_size())), event_source::keyboard);
          break;
        }
        case keys::down:
        case keys::numpad::down: {
          try_to_select(super::get_selection() + static_cast<int>(super::traits.get_items_per_line(super::client_size())), event_source::keyboard);
          break;
        }
      }
    }


  } // win

} // gui
