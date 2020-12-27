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
// Library includes
//
#include <gui/win/container.h>
#include <gui/draw/bitmap.h>
#include <gui/ctrl/control.h>


namespace gui {

  namespace ctrl {

    enum class menu_state : bool {
      disabled,
      enabled
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT menu_entry {
      typedef void (menu_action)();
      typedef draw::masked_bitmap icon_type;

      menu_entry (const text_source& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const core::hot_key& hotkey = core::hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled);

      menu_entry (const std::string& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const core::hot_key& hotkey = core::hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled);

      menu_entry (const menu_entry&) = default;
      menu_entry& operator= (const menu_entry&);

      menu_entry (menu_entry&&) = default;
      menu_entry& operator= (menu_entry&&) noexcept ;

      std::string get_label () const;
      const core::hot_key& get_hot_key () const;
      const icon_type& get_icon () const;
      core::size::type get_width () const;
      char get_menu_key () const;
      const std::function<menu_action>& get_action () const;

      bool is_sub_menu () const;
      bool is_disabled () const;
      bool is_enabled () const;
      bool has_separator () const;

      void set_label (const text_source& l);
      void set_icon (const icon_type& i);
      void set_enabled (bool d);
      void set_width (core::size::type w);

      void select () const;

      void check_hot_key (os::key_state, os::key_symbol);

    protected:
      menu_entry (bool sub_menu,
                  const text_source& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const core::hot_key& hotkey,
                  bool separator,
                  const icon_type& icon,
                  menu_state state);

      menu_entry ();

    private:
      text_source label;
      core::hot_key hotkey;
      icon_type icon;
      std::function<menu_action> action;
      char menu_key;
      core::size::type width;
      bool separator;
      menu_state state;
      bool sub_menu;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT sub_menu_entry : public menu_entry {
    public:
      sub_menu_entry (const text_source& label,
                      char menu_key,
                      const std::function<menu_action>& action,
                      bool separator = false,
                      const icon_type& icon = icon_type(),
                      menu_state state = menu_state::enabled);

      sub_menu_entry (const std::string& label,
                      char menu_key,
                      const std::function<menu_action>& action,
                      bool separator = false,
                      const icon_type& icon = icon_type(),
                      menu_state state = menu_state::enabled);
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT main_menu_entry : public menu_entry {
    public:
      main_menu_entry (const text_source& label,
                       char menu_key,
                       const std::function<menu_action>& action,
                       menu_state state = menu_state::enabled);

      main_menu_entry (const std::string& label,
                       char menu_key,
                       const std::function<menu_action>& action,
                       menu_state state = menu_state::enabled);
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT menu_data {
    public:
      typedef std::vector<menu_entry> vector;
      typedef vector::iterator iterator;
      typedef vector::const_iterator const_iterator;

      typedef void (close_fn)();
      typedef void (mouse_fn)(bool, const core::point&);
      typedef bool (key_fn)(os::key_symbol);

      typedef std::function<close_fn> close_call;
      typedef std::function<mouse_fn> mouse_call;
      typedef std::function<key_fn> key_call;

      explicit menu_data (win::window* win);
      menu_data (win::window* win, const menu_data& rhs);
      menu_data (win::window* win, menu_data&& rhs);

      ~menu_data ();

      void add_entries (std::initializer_list<menu_entry> menu_entries);
      void add_entry (const menu_entry& entry);
      void add_entry (menu_entry&& entry);

      menu_entry& operator[] (std::size_t i);
      const menu_entry& operator[] (std::size_t i) const;

      inline std::size_t size () const;

      const_iterator begin () const;
      const_iterator end () const;

      int get_index_of (const menu_entry&) const;

      int get_selection () const;
      void set_selection(int, event_source);
      void clear_selection(event_source);
      void rotate_selection (int delta = 1);

      int get_hilite () const;
      void set_hilite (int sel);
      void clear_hilite ();
      void rotate_hilite (int delta = 1);

      item_state get_item_state (int idx) const;

      void set_close_function(close_call);
      void clear_close_function ();

      void set_mouse_function(mouse_call);
      void clear_mouse_function ();

      void set_key_function(key_call);

      bool is_open () const;

      void close ();
      void handle_mouse (bool, const core::point&) const;
      bool handle_key (os::key_symbol) const;

      void init ();

      void register_hot_keys (win::window*);
      void unregister_hot_keys ();

      void register_menu_keys (win::window*);
      void unregister_menu_keys ();

      void check_hot_key (os::key_state st, os::key_symbol sym);

    private:
      win::window* win;

      struct data {
        data ();

        vector items;

        int selection;
        int hilite;

        close_call close_caller;
        mouse_call mouse_caller;
        key_call key_caller;

      } data;

    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT main_menu : public control {
    public:
      typedef control super;
      typedef no_erase_window_class<main_menu> clazz;

      main_menu ();
      main_menu (const main_menu&);
      main_menu (main_menu&&) noexcept ;

      using control::create;
      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (std::size_t idx) const;
      int get_index_at_point (const core::point& pt) const;

      int get_selection () const;
      core::point get_selection_position () const;

      menu_data data;

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT popup_menu : public win::popup_window {
    public:
      typedef win::popup_window super;

      static const int item_height = 20;

      popup_menu ();
      popup_menu (const popup_menu&);
      popup_menu (popup_menu&&) noexcept ;
      popup_menu (std::initializer_list<menu_entry> entries);

      void close ();

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (int idx) const;
      int get_index_at_point (const core::point& pt) const;

      int get_selection () const;
      core::point get_selection_position () const;

      void popup (popup_menu& parent);
      void popup (main_menu& parent);

      void popup_at (const core::point& pt, popup_menu& parent);
      void popup_at (const core::point& pt, main_menu& parent);
      void popup_at (const core::point& pt, win::container& parent);
      void popup_at (const core::point& pt, control& parent);

      menu_data data;

    private:
      void init ();

      void popup_at (win::container& parent, menu_data& parent_data, const core::point& pt);

      core::size::type calc_width ();

      struct positions {
        positions ();

        core::point::type text;
        core::point::type hotkey;

      } pos;
    };

  } // ctrl

} // gui

#include <gui/ctrl/menu.inl>
