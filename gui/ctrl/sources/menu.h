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
#include "control.h"
#include "bitmap.h"
#include "container.h"


namespace gui {

  namespace win {

    enum class menu_state : bool {
      disabled,
      enabled
    };

    // --------------------------------------------------------------------------
    struct menu_entry {
      typedef void(menu_action)();
      typedef draw::masked_bitmap icon_type;

      menu_entry (const std::string& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const hot_key& hotkey = hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled)
        : label(label)
        , hotkey(hotkey)
        , icon(icon)
        , action(action)
        , width(0)
        , separator(separator)
        , state(state)
        , menu_key(menu_key)
        , sub_menu(false)
      {}

      menu_entry (const menu_entry&);
      void operator= (const menu_entry&);

      menu_entry (menu_entry&&);
      void operator= (menu_entry&&);

      const std::string& get_label () const {
        return label;
      }

      const hot_key& get_hot_key () const {
        return hotkey;
      }

      const icon_type& get_icon () const {
        return icon;
      }

      core::size_type get_width () const {
        return width;
      }

      char get_menu_key () const {
        return menu_key;
      }

      const std::function<menu_action>& get_action () const {
        return action;
      }

      bool is_sub_menu () const {
        return sub_menu;
      }

      bool has_separator () const {
        return separator;
      }

      bool is_disabled () const {
        return state == menu_state::disabled;
      }

      bool is_enabled () const {
        return state == menu_state::enabled;
      }

      void set_label (const std::string& l) {
        label = l;
      }

      void set_icon (const icon_type& i) {
        icon = i;
      }

      void set_enabled (bool d) {
        state = d ? menu_state::enabled : menu_state::disabled;
      }

      void set_width (core::size_type w) {
        width = w;
      }

      void select () const {
        if (is_enabled() && action) {
          action();
        }
      }

      void check_hot_key (os::key_state, os::key_symbol);

    protected:
      menu_entry (bool sub_menu,
                  const std::string& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const hot_key& hotkey,
                  bool separator,
                  const icon_type& icon,
                  menu_state state)
        : label(label)
        , hotkey(hotkey)
        , icon(icon)
        , action(action)
        , menu_key(menu_key)
        , width(0)
        , separator(separator)
        , state(state)
        , sub_menu(sub_menu)
      {}

      menu_entry ()
        : menu_key(0)
        , width(0)
        , separator(false)
        , state(menu_state::enabled)
        , sub_menu(false)
      {}

    private:
      std::string label;
      hot_key hotkey;
      icon_type icon;
      std::function<menu_action> action;
      char menu_key;
      core::size_type width;
      bool separator;
      menu_state state;
      bool sub_menu;
    };

    // --------------------------------------------------------------------------
    struct sub_menu_entry : public menu_entry {
    public:
      sub_menu_entry (const std::string& label,
                      char menu_key,
                      const std::function<menu_action>& action,
                      bool separator = false,
                      const icon_type& icon = icon_type(),
                      menu_state state = menu_state::enabled)
        : menu_entry(true, label, menu_key, action, hot_key(), separator, icon, state)
      {}
    };

    // --------------------------------------------------------------------------
    struct main_menu_entry : public menu_entry {
    public:
      main_menu_entry (const std::string& label,
                       char menu_key,
                       const std::function<menu_action>& action,
                       menu_state state = menu_state::enabled)
        : menu_entry(true, label, menu_key, action, hot_key(), false, icon_type(), state)
      {}
    };

    namespace paint {

      // --------------------------------------------------------------------------
      void draw_menu_label (const draw::graphics& g,
                           const core::rectangle& r,
                           const std::string& label,
                           char menu_key,
                           os::color color);

      // --------------------------------------------------------------------------
      void menu_item (const draw::graphics& g,
                      const core::rectangle& r,
                      const draw::brush& background,
                      core::point_type text_pos,
                      core::point_type hotkey_pos,
                      const std::string& label,
                      char menu_key,
                      const draw::masked_bitmap& icon,
                      const hot_key& hotkey,
                      bool is_sub_menu,
                      bool separator,
                      bool selected,
                      bool hilited,
                      bool disabled);

      inline void menu_item (const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& background,
                             core::point_type text_pos,
                             core::point_type hotkey_pos,
                             const menu_entry& e,
                             bool selected,
                             bool hilited) {
        menu_item(g, r, background, text_pos, hotkey_pos,
                  e.get_label(), e.get_menu_key(), e.get_icon(),
                  e.get_hot_key(), e.is_sub_menu(), e.has_separator(),
                  selected, hilited, e.is_disabled());
      }

      // --------------------------------------------------------------------------
      void main_menu_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           char menu_key,
                           bool selected,
                           bool hilited,
                           bool disabled);

      inline void main_menu_item (const draw::graphics& g,
                                  const core::rectangle& r,
                                  const draw::brush& background,
                                  const menu_entry& e,
                                  bool selected,
                                  bool hilited) {
        main_menu_item(g, r, background, e.get_label(), e.get_menu_key(),
                       selected, hilited, e.is_disabled());
      }

      // --------------------------------------------------------------------------
    }

    class menu_data {
    public:
      typedef std::vector<menu_entry> vector;
      typedef vector::iterator iterator;
      typedef vector::const_iterator const_iterator;

      typedef void(close_fn)();
      typedef void(mouse_fn)(bool, const core::point&);
      typedef bool(key_fn)(os::key_symbol);

      typedef std::function<close_fn> close_call;
      typedef std::function<mouse_fn> mouse_call;
      typedef std::function<key_fn> key_call;

      menu_data (window* win)
        : win(win)
      {}

      menu_data (window* win, const menu_data& rhs)
        : win(win)
        , data(rhs.data)
      {}

      menu_data (window* win, menu_data&& rhs)
        : win(win)
        , data(std::move(rhs.data))
      {}

      ~menu_data () {
        unregister_hot_keys();
        unregister_menu_keys();
      }

      void add_entries (std::initializer_list<menu_entry> menu_entries);
      void add_entry (const menu_entry& entry);
      void add_entry (menu_entry&& entry);

      inline menu_entry& operator[] (std::size_t i) {
        return data.items[i];
      }

      inline const menu_entry& operator[] (std::size_t i) const {
        return data.items[i];
      }

      inline std::size_t size () const {
        return data.items.size();
      }

      const_iterator begin () const {
        return data.items.begin();
      }

      const_iterator end () const {
        return data.items.end();
      }

      int get_index_of (const menu_entry&) const;

      int get_selection () const;
      void set_selection (int, event_source);
      void clear_selection (event_source);
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

      void register_hot_keys (window*);
      void unregister_hot_keys ();

      void register_menu_keys (window*);
      void unregister_menu_keys ();

      void check_hot_key (os::key_state st, os::key_symbol sym);

    private:
      window* win;

      struct data {
        data ()
          : selection(-1)
          , hilite(-1)
        {}

        vector items;

        int selection;
        int hilite;

        close_call close_caller;
        mouse_call mouse_caller;
        key_call key_caller;

      } data;

    };

    // --------------------------------------------------------------------------
    class main_menu : public window {
    public:
      typedef window super;

      main_menu ();
      main_menu (const main_menu&);
      main_menu (main_menu&&);

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
      void init ();

      static const window_class clazz;
    };

    // --------------------------------------------------------------------------
    class popup_menu : public popup_window {
    public:
      typedef popup_window super;

      static const int item_height = 20;

      popup_menu ();
      popup_menu (const popup_menu&);
      popup_menu (popup_menu&&);

      popup_menu (std::initializer_list<menu_entry> entries)
        : popup_menu() {
        data.add_entries(entries);
      }

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
      void init ();

      void popup_at (window& parent, menu_data& parent_data, const core::point& pt);

      core::size_type calc_width ();

      struct positions {
        positions ()
          : text(10)
          , hotkey(0)
        {}

        core::point_type text;
        core::point_type hotkey;

      } pos;
    };

    // --------------------------------------------------------------------------

  } // win

} // gui
