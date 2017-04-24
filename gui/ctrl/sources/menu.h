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
      typedef void(close_fn)();
      typedef void(check_fn)(const core::point&);
      typedef std::function<close_fn> close_function;
      typedef std::function<check_fn> check_selection;

      static const int item_height = 20;
      typedef vlist<item_height, color::light_gray> list_type;

      popup_menu ();

      void popup_at (const core::point& pt, popup_menu& parent) {
        call_check_selection = [&](const core::point& pt) {
          core::rectangle r = parent.items.absolute_place();
          if (r.is_inside(pt)) {
            int new_idx = parent.items.get_index_at_point(parent.items.screen_to_window(pt));
            if (parent.items.get_selection() > -1) {
              if (parent.items.get_selection() != new_idx) {
                parent.items.set_selection(new_idx);
                close();
              }
            } else if (parent.items.get_hilite() > -1) {
              if (parent.items.get_hilite() != new_idx) {
                parent.items.set_hilite(new_idx);
                close();
              }
            }
          }
        };

        call_close_function = [&]() {
          parent.call_close_function();
          close();
        };

        popup_at(pt);
      }

      void popup_at (const core::point& pt, main_menu& parent) {
        call_check_selection = [&](const core::point& pt) {
          core::rectangle r = parent.absolute_place();
          if (r.is_inside(pt)) {
            int new_idx = parent.get_index_at_point(parent.screen_to_window(pt));
            if (parent.get_selection() > -1) {
              if (parent.get_selection() != new_idx) {
                parent.set_selection(new_idx);
                close();
              }
            } else if (parent.get_hilite() > -1) {
              if (parent.get_hilite() != new_idx) {
                parent.set_hilite(new_idx);
                close();
              }
            }
          }
        };

        call_close_function = [&]() {
          parent.clear_selection();
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

      list_type items;

    private:
      void popup_at (const core::point& pt);

      core::size_type calc_width ();
      close_function call_close_function;
      check_selection call_check_selection;

      typedef simple_list_data<menu_entry, paint::menu_item> data_type;

      data_type data;
    };


  } // win

} // gui
