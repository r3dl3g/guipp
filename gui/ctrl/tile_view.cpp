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
    core::size_type basic_tile_view<orientation::horizontal>::get_item_size () const {
      return item_size.height();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_item_size () const {
      return item_size.width();
    }

    template<>
    std::size_t basic_tile_view<orientation::horizontal>::get_items_per_line () const {
      return static_cast<std::size_t>(std::max((client_size().height() - border.height() * 2 + spacing.height()) / (item_size.height() + spacing.height()), 1.0f));
    }

    template<>
    std::size_t basic_tile_view<orientation::vertical>::get_items_per_line () const {
      return static_cast<std::size_t>(std::max((client_size().width() - border.width() * 2 + spacing.width()) / (item_size.width() + spacing.width()), 1.0f));
    }

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_line_size () const {
      return item_size.width();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_line_size () const {
      return item_size.height();
    }

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_item_border () const {
      return border.height();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_item_border () const {
      return border.width();
    }

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_line_border () const {
      return border.width();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_line_border () const {
      return border.height();
    }

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_item_spacing () const {
      return spacing.height();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_item_spacing () const {
      return spacing.width();
    }

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_line_spacing () const {
      return spacing.width();
    }

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_line_spacing () const {
      return spacing.height();
    }

    template<>
    void basic_tile_view<orientation::horizontal>::handle_direction_key (os::key_symbol key) {
      switch (key) {
        case keys::left:
        case keys::numpad::left: {
          try_to_select(super::get_selection() - static_cast<int>(get_items_per_line()), event_source::keyboard);
          break;
        }
        case keys::right:
        case keys::numpad::right: {
          try_to_select(super::get_selection() + static_cast<int>(get_items_per_line()), event_source::keyboard);
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
          try_to_select(super::get_selection() - static_cast<int>(get_items_per_line()), event_source::keyboard);
          break;
        }
        case keys::down:
        case keys::numpad::down: {
          try_to_select(super::get_selection() + static_cast<int>(get_items_per_line()), event_source::keyboard);
          break;
        }
      }
    }


  } // win

} // gui
