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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"

namespace gui {

  namespace win {

    struct menu_entry {
      typedef void(menu_action)(int idx);

      menu_entry (const std::string& label,
                  const std::function<menu_action>& action,
                  const std::string& hotkey = std::string(),
                  bool separator = false,
                  const std::string& icon = std::string(),
                  bool sub_menu = false)
        : label(label)
        , hotkey(hotkey)
        , icon(icon)
        , action(action)
        , sub_menu(sub_menu)
        , separator(separator)
      {}

      const std::string& get_label () const {
        return label;
      }

      void set_label (const std::string& l) {
        label = l;
      }

      const std::string& get_hotkey () const {
        return hotkey;
      }

      void set_hotkey (const std::string& h) {
        hotkey = h;
      }

      const std::string& get_icon () const {
        return icon;
      }

      void set_icon (const std::string& i) {
        icon = i;
      }

      bool is_sub_menu () const {
        return sub_menu;
      }

      bool has_separator () const {
        return separator;
      }

      void select (int idx) {
        if (!sub_menu && action) {
          action(idx);
        }
      }

      void hilite (int idx) {
        if (sub_menu && action) {
          action(idx);
        }
      }

    private:
      std::string label;
      std::string hotkey;
      std::string icon;
      std::function<menu_action> action;
      bool sub_menu;
      bool separator;
    };

    struct sub_menu_entry : public menu_entry {
    public:
      sub_menu_entry (const std::string& label,
                      const std::function<menu_action>& action,
                      bool separator = false,
                      const std::string& icon = std::string())
        : menu_entry(label, action, std::string(), separator, icon, true)
      {}
    };

    namespace paint {

      void menu_item (const menu_entry& e,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      core::point_type text_pos,
                      core::point_type hotkey_pos,
                      const draw::brush& background,
                      bool selected,
                      bool hilited);

      void main_menu_item (const menu_entry& e,
                           const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           bool selected,
                           bool hilited);

    }

    class main_menu : public hlist<50, color::light_gray> {
    public:
      typedef hlist<50, color::light_gray> super;

      main_menu ();

      core::point sub_menu_position (int idx);

      void add_entries (const std::initializer_list<menu_entry>& menu_entries) {
        data.insert(data.end(), menu_entries);
      }

      void add_entry (const menu_entry& entry) {
        data.push_back(entry);
      }

      void prepare () {
        super::set_drawer(data);
        super::set_count(data.size());
      }

    private:
      typedef simple_list_data<menu_entry, paint::main_menu_item> data_type;
      data_type data;
    };

    // --------------------------------------------------------------------------
    class popup_menu : public popup_window {
    public:
      typedef popup_window super;
      typedef void(close_fn)();
      typedef void(check_fn)(os::key_state, const core::point&);
      typedef std::function<close_fn> close_function;
      typedef std::function<check_fn> check_selection;

      static const int item_height = 20;
      typedef vlist<item_height, color::light_gray> list_type;

      popup_menu ();

      void popup_at (const core::point& pt, popup_menu& parent) {
        call_check_selection = [&](os::key_state state, const core::point& p) {
          check_mouse_action<popup_menu::list_type, false>(parent.items, state, p);
        };

        call_close_function = [&]() {
          parent.call_close_function();
          close();
        };

        popup_at(pt);
      }

      void popup_at (const core::point& pt, main_menu& parent) {
        call_check_selection = [&](os::key_state state, const core::point& p) {
          check_mouse_action<main_menu, true>(parent, state, p);
        };

        call_close_function = [&]() {
          parent.clear_selection();
          parent.clear_hilite();
          close();
        };

        popup_at(pt);
      }

      void close ();

      core::point sub_menu_position (int idx);

      void add_entries (const std::initializer_list<menu_entry>& menu_entries) {
        data.insert(data.end(), menu_entries);
      }

      void add_entry (const menu_entry& entry) {
        data.push_back(entry);
      }

      menu_entry& get_menu_entry (int i) {
        return data[i];
      }

      std::size_t get_menu_entry_count () const {
        return data.size();
      }

      list_type items;

    private:
      template<class T, bool M>
      void check_mouse_action(T& parent, os::key_state state, const core::point& p) {
        core::point pt = items.window_to_screen(p);
        core::rectangle r = parent.absolute_place();
        if (r.is_inside(pt)) {
          int new_idx = parent.get_index_at_point(parent.screen_to_window(pt));
          if (parent.get_selection() > -1) {
            if (parent.get_selection() != new_idx) {
              close();
              if (M) {
                parent.clear_hilite();
              }
              parent.set_selection(new_idx);
            }
          } else if (parent.get_hilite() > -1) {
            if (parent.get_hilite() != new_idx) {
              close();
              parent.set_hilite(new_idx);
            }
          } else if (M && (new_idx < 0)) {
            parent.clear_hilite();
          }
        } else if (left_button_bit_mask::is_set(state) &&
                   !items.place().is_inside(p) && call_close_function) {
          call_close_function();
        }
      }

      void draw_menu_item (int idx,
                           const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           bool selected,
                           bool hilited);

      void popup_at (const core::point& pt);

      core::size_type calc_width ();
      close_function call_close_function;
      check_selection call_check_selection;
      core::point_type text_pos;
      core::point_type hotkey_pos;

      typedef std::vector<menu_entry> data_type;

      data_type data;
    };


  } // win

} // gui
