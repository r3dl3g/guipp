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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/overlapped_window.h>
#include <gui/draw/bitmap.h>
#include <gui/ctrl/control.h>


namespace gui {

  namespace ctrl {

    enum class menu_state : bool {
      disabled,
      enabled
    };

    typedef void (menu_action)();

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT menu_entry {
      typedef draw::masked_bitmap icon_type;

      menu_entry (const text_source& label,
                  char menu_key,
                  std::function<menu_action> action,
                  const core::hot_key& hotkey = core::hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled);

      menu_entry (const std::string& label,
                  char menu_key,
                  std::function<menu_action> action,
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

      menu_entry (bool sub_menu,
                  const text_source& label,
                  char menu_key,
                  std::function<menu_action> action,
                  const core::hot_key& hotkey = core::hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled);

      menu_entry ();

    private:
      text_source label;
      core::hot_key hotkey;
      icon_type icon;
      std::function<menu_action> action;
      core::size::type width;
      char menu_key;
      menu_state state;
      bool separator;
      bool sub_menu;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT menu_data {
    public:
      typedef std::vector<menu_entry> vector;
      typedef vector::iterator iterator;
      typedef vector::const_iterator const_iterator;

      ~menu_data ();

      void add_entries (const std::vector<menu_entry>& menu_entries);
      void add_entry (const menu_entry& entry);
      void add_entry (menu_entry&& entry);

      menu_entry& at (std::size_t i);
      const menu_entry& at (std::size_t i) const;

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

      void set_parent (menu_data*);
      void clear_parent ();

      void set_current_child (menu_data*);
      void clear_current_child (menu_data*);

      bool is_open () const;
      bool has_parent () const;

      virtual void close ();

      void close_current_child ();
      void close_parent ();

      void init ();

      void register_hot_keys (win::window*);
      void unregister_hot_keys ();

      void register_menu_keys (win::window*);
      void unregister_menu_keys ();

      void check_hot_key (os::key_state st, os::key_symbol sym);

      virtual win::window& view () = 0;
      virtual bool handle_key (os::key_symbol) = 0;

    protected:
      void handle_mouse (bool, const core::native_point&);
      virtual int get_index_at_point (const core::point& pt) const = 0;

      struct data {
        data ();

        vector items;

        int selection;
        int hilite;

        menu_data* parent;
        menu_data* current_child;

      } data;

    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT main_menu : public menu_data {
    public:
      typedef menu_data super;
      typedef win::no_erase_window_class<main_menu> clazz;

      main_menu ();

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void paint (draw::graphics& g);

      core::point sub_menu_position (std::size_t idx) const;
      int get_index_at_point (const core::point& pt) const override;

      core::point get_selection_position () const;

      win::window& view () override;
      bool handle_key (os::key_symbol) override;

    private:
      ctrl::control window;

      void init ();

    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT popup_menu : public menu_data {
    public:
      typedef menu_data super;

      static const int item_height = 20;

      popup_menu ();
      popup_menu (const std::vector<menu_entry>& entries);
      ~popup_menu ();

      void close () override;

      void paint (draw::graphics& g);

      core::point sub_menu_position (int idx) const;
      int get_index_at_point (const core::point& pt) const override;

      core::point get_selection_position () const;

      void popup (popup_menu& parent);
      void popup (main_menu& parent);

      void popup_at (const core::point& pt, popup_menu& parent);
      void popup_at (const core::point& pt, main_menu& parent);
      void popup_at (const core::point& pt, win::window& parent);

      win::window& view () override;
      bool handle_key (os::key_symbol) override;

    private:
      win::popup_window window;

      void init ();

      void popup_at (win::container& parent, menu_data& parent_data, const core::point& pt);

      void register_message_filter ();
      void unregister_message_filter ();

      core::size::type calc_width ();

      struct positions {
        positions ();

        core::point::type text;
        core::point::type hotkey;

      } pos;

      int message_filter_id;
    };

    // --------------------------------------------------------------------------
    GUIPP_CTRL_EXPORT
    menu_entry sub_menu_entry (const text_source& label,
                               char menu_key,
                               std::function<menu_action> action,
                               bool separator = false,
                               const menu_entry::icon_type& icon = menu_entry::icon_type(),
                               menu_state state = menu_state::enabled);

    GUIPP_CTRL_EXPORT
    menu_entry sub_menu_entry (const std::string& label,
                               char menu_key,
                               std::function<menu_action> action,
                               bool separator = false,
                               const menu_entry::icon_type& icon = menu_entry::icon_type(),
                               menu_state state = menu_state::enabled);

    // --------------------------------------------------------------------------
    GUIPP_CTRL_EXPORT
    menu_entry main_menu_entry (const text_source& label,
                                char menu_key,
                                std::function<menu_action> action,
                                menu_state state = menu_state::enabled);

    GUIPP_CTRL_EXPORT
    menu_entry main_menu_entry (const std::string& label,
                                char menu_key,
                                std::function<menu_action> action,
                                menu_state state = menu_state::enabled);

    GUIPP_CTRL_EXPORT
    menu_entry main_menu_entry (const text_source& label,
                                char menu_key,
                                main_menu& main,
                                popup_menu& sub);

    GUIPP_CTRL_EXPORT
    menu_entry main_menu_entry (const std::string& label,
                                char menu_key,
                                main_menu& main,
                                popup_menu& sub);

  } // ctrl

} // gui

#include <gui/ctrl/menu.inl>
