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
 * @brief     C++ API: menu
 *
 * @file
 */

#pragma once


namespace gui {

  namespace ctrl {

    inline std::string menu_entry::get_label () const {
      return label();
    }

    inline const win::hot_key& menu_entry::get_hot_key () const {
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
    inline sub_menu_entry::sub_menu_entry (const text_source& label,
                                           char menu_key,
                                           const std::function<menu_action>& action,
                                           bool separator,
                                           const icon_type& icon,
                                           menu_state state)
      : menu_entry(true, label, menu_key, action, win::hot_key(), separator, icon, state)
    {}

    inline sub_menu_entry::sub_menu_entry (const std::string& label,
                                           char menu_key,
                                           const std::function<menu_action>& action,
                                           bool separator,
                                           const icon_type& icon,
                                           menu_state state)
      : menu_entry(true, const_text(label), menu_key, action, win::hot_key(), separator, icon, state)
    {}

    // --------------------------------------------------------------------------
    inline main_menu_entry::main_menu_entry (const text_source& label,
                                             char menu_key,
                                             const std::function<menu_action>& action,
                                             menu_state state)
      : menu_entry(true, label, menu_key, action, win::hot_key(), false, icon_type(), state)
    {}

    inline main_menu_entry::main_menu_entry (const std::string& label,
                                             char menu_key,
                                             const std::function<menu_action>& action,
                                             menu_state state)
      : menu_entry(true, const_text(label), menu_key, action, win::hot_key(), false, icon_type(), state)
    {}

    // --------------------------------------------------------------------------
    inline menu_data::menu_data (win::window* win)
      : win(win)
    {}

    inline menu_data::menu_data (win::window* win, const menu_data& rhs)
      : win(win)
      , data(rhs.data)
    {}

    inline menu_data::menu_data (win::window* win, menu_data&& rhs)
      : win(win)
      , data(std::move(rhs.data))
    {}

    inline menu_entry& menu_data::operator[] (std::size_t i) {
      return data.items[i];
    }

    inline const menu_entry& menu_data::operator[] (std::size_t i) const {
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

    inline void menu_data::set_close_function (close_call fn) {
      data.close_caller = fn;
    }

    inline void menu_data::clear_close_function () {
      data.close_caller = nullptr;
      data.key_caller = nullptr;
    }

    inline void menu_data::set_mouse_function (mouse_call fn) {
      data.mouse_caller = fn;
    }

    inline void menu_data::clear_mouse_function () {
      data.mouse_caller = nullptr;
    }

    inline void menu_data::set_key_function (key_call fn) {
      data.key_caller = fn;
    }

    inline bool menu_data::is_open () {
      return (bool)data.close_caller;
    }

    inline menu_data::data::data ()
      : selection(-1)
      , hilite(-1)
    {}

    // --------------------------------------------------------------------------
    inline void main_menu::create (const win::container& parent,
                                   const core::rectangle& place) {
      win::window::create(clazz::get(), parent, place);
    }

    inline int main_menu::get_selection () const {
      return data.get_selection();
    }

    inline core::point main_menu::get_selection_position () const {
      return sub_menu_position(get_selection());
    }

    // --------------------------------------------------------------------------
    inline popup_menu::popup_menu (std::initializer_list<menu_entry> entries)
      : popup_menu() {
      data.add_entries(entries);
    }

    inline int popup_menu::get_selection () const {
      return data.get_selection();
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
      popup_at(parent, parent.data, pt);
    }

    inline void popup_menu::popup_at (const core::point& pt, main_menu& parent) {
      popup_at(parent, parent.data, pt);
    }

    inline popup_menu::positions::positions ()
      : text(10)
      , hotkey(0)
    {}

  } // ctrl

} // gui
