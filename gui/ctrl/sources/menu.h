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
                  const std::function<menu_action>& action)
        : label(label)
        , action(action)
        , sub_menu(false)
      {}

      menu_entry (const std::string& label,
                  bool sub_menu,
                  const std::function<menu_action>& action)
        : label(label)
        , action(action)
        , sub_menu(sub_menu)
      {}

      const std::string& get_label () const {
        return label;
      }

      bool is_sub_menu () const {
        return sub_menu;
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
      std::function<menu_action> action;
      bool sub_menu;
    };

    namespace paint {

      void menu_item (const menu_entry& e,
                      const draw::graphics& g,
                      const core::rectangle& r,
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

      static const int item_height = 20;
      typedef vlist<item_height, color::light_gray> list_type;

      popup_menu ();

      template<class T>
      void popup_at_list (const core::point& pt, T& main_list) {
        items.register_event_handler(selection_changed_event([&]() {
          int idx = items.get_selection();
          if (idx > -1) {
            menu_entry& e = data[idx];
            if (!e.is_sub_menu()) {
              main_list.clear_selection();
              end_modal();
              e.select(idx);
            }
          }
        }));
        items.register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          if (!items.place().is_inside(pt)) {
            main_list.clear_selection();
            end_modal();
          }
        }));
        items.register_event_handler(mouse_move_event([&](os::key_state, const core::point& p) {
          core::point pt = items.window_to_screen(p);
          core::rectangle r = main_list.absolute_place();
          if (r.is_inside(pt)) {
            int new_idx = main_list.get_index_at_point(main_list.screen_to_window(pt));
            if (main_list.get_selection() > -1) {
              if (main_list.get_selection() != new_idx) {
                end_modal();
                main_list.set_selection(new_idx);
              }
            } else if (main_list.get_hilite() > -1) {
              if (main_list.get_hilite() != new_idx) {
                main_list.set_hilite(new_idx);
                end_modal();
              }
            }
          }
        }));
      }

      void popup_at (const core::point& pt, popup_menu& parent) {
        popup_at_list(pt, parent.items);
        popup_at(pt);
      }

      void popup_at (const core::point& pt, main_menu& parent) {
        popup_at_list(pt, parent);
        popup_at(pt);
      }

      core::point sub_menu_position (int idx);

      void add_entries (const std::initializer_list<menu_entry>& menu_entries) {
        data.insert(data.end(), menu_entries);
      }

      void add_entry (const menu_entry& entry) {
        data.push_back(entry);
      }

      list_type items;

    private:
      void popup_at (const core::point& pt);

      core::size_type calc_width ();

      typedef simple_list_data<menu_entry, paint::menu_item> data_type;

      data_type data;
    };


  } // win

} // gui
