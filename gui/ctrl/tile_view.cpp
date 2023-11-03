/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     tile view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/tile_view.h>


namespace gui {

  namespace ctrl {

    template<>
    int tile_list_traits<orientation_t::horizontal>::get_direction_step (os::key_symbol key,
                                                                         const core::size& list_size) const {
      switch (key) {
        case core::keys::left:
        case core::keys::numpad::left:
          return -static_cast<int>(get_items_per_line(list_size));
        case core::keys::right:
        case core::keys::numpad::right:
          return static_cast<int>(get_items_per_line(list_size));
        case core::keys::up:
        case core::keys::numpad::up:
          return -1;
        case core::keys::down:
        case core::keys::numpad::down:
          return 1;
      }
      return 0;
    }

    template<>
    int tile_list_traits<orientation_t::vertical>::get_direction_step (os::key_symbol key,
                                                                       const core::size& list_size) const {
      switch (key) {
        case core::keys::left:
        case core::keys::numpad::left:
          return -1;
        case core::keys::right:
        case core::keys::numpad::right:
          return 1;
        case core::keys::up:
        case core::keys::numpad::up:
          return -static_cast<int>(get_items_per_line(list_size));
        case core::keys::down:
        case core::keys::numpad::down:
          return static_cast<int>(get_items_per_line(list_size));
      }
      return 0;
    }


  } // ctrl

} // gui
