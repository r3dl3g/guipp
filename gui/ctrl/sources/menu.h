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

    // --------------------------------------------------------------------------
    struct menu_entry {
      typedef void(menu_action)(int idx);

      menu_entry (const std::string& label,
                  const std::function<menu_action>& action,
                  const std::string& hotkey = std::string(),
                  bool separator = false,
                  const std::string& icon = std::string(),
                  bool disabled = false)
        : label(label)
        , hotkey(hotkey)
        , icon(icon)
        , action(action)
        , width(0)
        , separator(separator)
        , disabled(disabled)
        , sub_menu(false)
      {}

      const std::string& get_label () const {
        return label;
      }

      const std::string& get_hotkey () const {
        return hotkey;
      }

      const std::string& get_icon () const {
        return icon;
      }

      core::size_type get_width () const {
        return width;
      }

      bool is_sub_menu () const {
        return sub_menu;
      }

      bool has_separator () const {
        return separator;
      }

      bool is_disabled () const {
        return disabled;
      }

      void set_label (const std::string& l) {
        label = l;
      }

      void set_icon (const std::string& i) {
        icon = i;
      }

      void set_disabled (bool d) {
        disabled = d;
      }

      void set_width (core::size_type w) {
        width = w;
      }

      void select (int idx) {
        if (!disabled && action) {
          action(idx);
        }
      }

    protected:
      menu_entry (bool sub_menu,
                  const std::string& label,
                  const std::function<menu_action>& action,
                  const std::string& hotkey,
                  bool separator,
                  const std::string& icon,
                  bool disabled)
        : label(label)
        , hotkey(hotkey)
        , icon(icon)
        , action(action)
        , width(0)
        , separator(separator)
        , disabled(disabled)
        , sub_menu(sub_menu)
      {}

    private:
      std::string label;
      std::string hotkey;
      std::string icon;
      std::function<menu_action> action;
      core::size_type width;
      bool separator;
      bool disabled;
      bool sub_menu;
    };

    // --------------------------------------------------------------------------
    struct sub_menu_entry : public menu_entry {
    public:
      sub_menu_entry (const std::string& label,
                      const std::function<menu_action>& action,
                      bool separator = false,
                      const std::string& icon = std::string(),
                      bool disabled = false)
        : menu_entry(true, label, action, std::string(), separator, icon, disabled)
      {}
    };

    // --------------------------------------------------------------------------
    struct main_menu_entry : public menu_entry {
    public:
      main_menu_entry (const std::string& label,
                       const std::function<menu_action>& action,
                       bool disabled = false)
        : menu_entry(true, label, action, std::string(), false, std::string(), disabled)
      {}
    };

    namespace paint {

      // --------------------------------------------------------------------------
      void menu_item (const menu_entry& e,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      core::point_type text_pos,
                      core::point_type hotkey_pos,
                      const draw::brush& background,
                      bool selected,
                      bool hilited);

      // --------------------------------------------------------------------------
      void main_menu_item (const menu_entry& e,
                           const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           bool selected,
                           bool hilited);

    }

    class menu_data {
    public:
      typedef std::vector<menu_entry> vector;
      typedef vector::iterator iterator;

      typedef void(close_fn)();
      typedef void(mouse_fn)(bool, const core::point&);
      typedef bool(key_fn)(os::key_symbol);

      typedef std::function<close_fn> close_call;
      typedef std::function<mouse_fn> mouse_call;
      typedef std::function<key_fn> key_call;

      menu_data (window* win)
        : selection(-1)
        , hilite(-1)
        , win(win)
      {}

      void add_entries (const std::initializer_list<menu_entry>& menu_entries);
      void add_entry (const menu_entry& entry);

      inline menu_entry& operator[] (int i) {
        return data[i];
      }

      inline const menu_entry& operator[] (int i) const {
        return data[i];
      }

      inline std::size_t size () const {
        return data.size();
      }

      int get_selection () const;
      void set_selection (int);
      void clear_selection ();
      void rotate_selection (int delta = 1);

      int get_hilite () const;
      void set_hilite (int sel);
      void clear_hilite ();
      void rotate_hilite (int delta = 1);

      void set_close_function (close_call);
      void clear_close_function ();

      void set_mouse_function (mouse_call);
      void clear_mouse_function ();

      void set_key_function (key_call);

      bool is_open ();

      void close ();
      void handle_mouse (bool, const core::point&);
      bool handle_key (os::key_symbol);

      void init ();

    private:
      int selection;
      int hilite;

      close_call close_caller;
      mouse_call mouse_caller;
      key_call key_caller;

      window* win;
      vector data;
    };

    // --------------------------------------------------------------------------
    class main_menu : public window {
    public:
      typedef window super;

      main_menu ();

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        window::create(clazz, parent, place);
      }

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (std::size_t idx) const;
      int get_index_at_point (const core::point& pt) const;

      menu_data data;

    private:
      static const window_class clazz;
    };

    // --------------------------------------------------------------------------
    class popup_menu : public popup_window {
    public:
      typedef popup_window super;

      static const int item_height = 20;

      popup_menu ();

      void close ();

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (int idx);
      int get_index_at_point (const core::point& pt) const;

      void popup_at (const core::point& pt, popup_menu& parent) {
        popup_at(parent, parent.data, pt);
      }

      void popup_at (const core::point& pt, main_menu& parent) {
        popup_at(parent, parent.data, pt);
      }

      void popup_at (const core::point& pt, window& parent);

      menu_data data;

    private:
      void popup_at (window& parent, menu_data& parent_data, const core::point& pt);

      core::size_type calc_width ();

      core::point_type text_pos;
      core::point_type hotkey_pos;
    };

    // --------------------------------------------------------------------------

  } // win

} // gui
