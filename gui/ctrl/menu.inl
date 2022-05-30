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
 * @brief     menu control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace ctrl {

    inline std::string menu_entry::get_label () const {
      return label();
    }

    inline const core::hot_key& menu_entry::get_hot_key () const {
      return hotkey;
    }

    inline auto menu_entry::get_icon() const->const icon_type& {
      return icon;
    }

    inline core::size::type menu_entry::get_width () const {
      return width;
    }

    inline char menu_entry::get_menu_key () const {
      return menu_key;
    }

    inline auto menu_entry::get_action() const->const std::function<menu_action>&{
      return action;
    }

    inline bool menu_entry::is_sub_menu () const {
      return sub_menu;
    }

    inline bool menu_entry::has_separator () const {
      return separator;
    }

    inline bool menu_entry::is_disabled () const {
      return state == menu_state::disabled;
    }

    inline bool menu_entry::is_enabled () const {
      return state == menu_state::enabled;
    }

    inline void menu_entry::set_label (const text_source& l) {
      label = l;
    }

    inline void menu_entry::set_icon (const icon_type& i) {
      icon = i;
    }

    inline void menu_entry::set_enabled (bool d) {
      state = d ? menu_state::enabled : menu_state::disabled;
    }

    inline void menu_entry::set_width (core::size::type w) {
      width = w;
    }

    inline void menu_entry::select () const {
      if (is_enabled() && action) {
        action();
      }
    }

    // --------------------------------------------------------------------------
    inline menu_entry& menu_data::at (std::size_t i) {
      return data.items[i];
    }

    inline const menu_entry& menu_data::at (std::size_t i) const {
      return data.items[i];
    }

    inline std::size_t menu_data::size () const {
      return data.items.size();
    }

    inline auto menu_data::begin() const->const_iterator {
      return data.items.begin();
    }

    inline auto menu_data::end() const->const_iterator {
      return data.items.end();
    }

    inline int menu_data::get_selection () const {
      return data.selection;
    }

    inline void menu_data::clear_selection (event_source src) {
      set_selection(-1, src);
    }

    inline int menu_data::get_hilite () const {
      return data.hilite;
    }

    inline void menu_data::set_parent (menu_data* m) {
      data.parent = m;
    }

    inline void menu_data::clear_parent () {
      data.parent = nullptr;
    }

    inline void menu_data::set_current_child (menu_data* m) {
      data.current_child = m;
    }

    inline void menu_data::clear_current_child (menu_data* m) {
      if (data.current_child == m) {
        data.current_child = nullptr;
      }
    }

    inline bool menu_data::is_open () const {
      return (bool)data.current_child;
    }

    inline bool menu_data::has_parent () const {
      return (bool)data.parent;
    }

    inline menu_data::data::data ()
      : selection(-1)
      , hilite(-1)
      , parent(nullptr)
      , current_child(nullptr)
    {}

    // --------------------------------------------------------------------------
    inline void main_menu::create (win::container& parent,
                                   const core::rectangle& place) {
      window.create(clazz::get(), parent, place);
    }

    inline core::point main_menu::get_selection_position () const {
      return sub_menu_position(get_selection());
    }

    // --------------------------------------------------------------------------
    inline popup_menu::popup_menu (const std::vector<menu_entry>& entries)
      : popup_menu() {
      add_entries(entries);
    }

    inline core::point popup_menu::get_selection_position () const {
      return sub_menu_position(get_selection());
    }

    inline void popup_menu::popup (popup_menu& parent) {
      popup_at(parent.get_selection_position(), parent);
    }

    inline void popup_menu::popup (main_menu& parent) {
      popup_at(parent.get_selection_position(), parent);
    }

    inline void popup_menu::popup_at (const core::point& pt, popup_menu& parent) {
      popup_at(*parent.view().get_parent(), parent, pt);
    }

    inline void popup_menu::popup_at (const core::point& pt, main_menu& parent) {
      popup_at(*parent.view().get_parent(), parent, pt);
    }

    inline popup_menu::positions::positions ()
      : text(10)
      , hotkey(0)
    {}

  } // ctrl

} // gui
