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
    struct menu_entry {
      typedef void (menu_action)();
      typedef draw::masked_bitmap icon_type;

      menu_entry (const std::string& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const win::hot_key& hotkey = win::hot_key(),
                  bool separator = false,
                  const icon_type& icon = icon_type(),
                  menu_state state = menu_state::enabled);

      menu_entry (const menu_entry&);
      void operator= (const menu_entry&);

      menu_entry (menu_entry&&);
      void operator= (menu_entry&&);

      const std::string& get_label () const;
      const win::hot_key& get_hot_key () const;
      const icon_type& get_icon () const;
      core::size_type get_width () const;
      char get_menu_key () const;
      const std::function<menu_action>& get_action () const;

      bool is_sub_menu () const;
      bool is_disabled () const;
      bool is_enabled () const;
      bool has_separator () const;

      void set_label (const std::string& l);
      void set_icon (const icon_type& i);
      void set_enabled (bool d);
      void set_width (core::size_type w);

      void select () const;

      void check_hot_key (os::key_state, os::key_symbol);

    protected:
      menu_entry (bool sub_menu,
                  const std::string& label,
                  char menu_key,
                  const std::function<menu_action>& action,
                  const win::hot_key& hotkey,
                  bool separator,
                  const icon_type& icon,
                  menu_state state);

      menu_entry ();

    private:
      std::string label;
      win::hot_key hotkey;
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
                      menu_state state = menu_state::enabled);
    };

    // --------------------------------------------------------------------------
    struct main_menu_entry : public menu_entry {
    public:
      main_menu_entry (const std::string& label,
                       char menu_key,
                       const std::function<menu_action>& action,
                       menu_state state = menu_state::enabled);
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
                      const win::hot_key& hotkey,
                      bool is_sub_menu,
                      bool separator,
                      bool selected,
                      bool hilited,
                      bool disabled);

      // --------------------------------------------------------------------------
      void main_menu_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           char menu_key,
                           bool selected,
                           bool hilited,
                           bool disabled);

      // --------------------------------------------------------------------------
    } // namespace paint

    // --------------------------------------------------------------------------
    class menu_data {
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

      menu_data (win::window* win);
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
      void set_selection(int, win::event_source);
      void clear_selection(win::event_source);
      void rotate_selection (int delta = 1);

      int get_hilite () const;
      void set_hilite (int sel);
      void clear_hilite ();
      void rotate_hilite (int delta = 1);

      void set_close_function(close_call);
      void clear_close_function ();

      void set_mouse_function(mouse_call);
      void clear_mouse_function ();

      void set_key_function(key_call);

      bool is_open ();

      void close ();
      void handle_mouse (bool, const core::point&);
      bool handle_key (os::key_symbol);

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
    class main_menu : public win::window {
    public:
      typedef window super;
      typedef win::window_class<main_menu, color::very_light_gray> clazz;

      main_menu ();
      main_menu (const main_menu&);
      main_menu (main_menu&&);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (std::size_t idx) const;
      int get_index_at_point (const core::point& pt) const;

      menu_data data;

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    class popup_menu : public win::popup_window {
    public:
      typedef win::popup_window super;

      static const int item_height = 20;

      popup_menu ();
      popup_menu (const popup_menu&);
      popup_menu (popup_menu&&);
      popup_menu (std::initializer_list<menu_entry> entries);

      void close ();

      void paint (const draw::graphics& g);
      bool handle_key (os::key_symbol);
      void handle_mouse (bool, const core::point&);

      core::point sub_menu_position (int idx);
      int get_index_at_point (const core::point& pt) const;

      void popup_at (const core::point& pt, popup_menu& parent);
      void popup_at (const core::point& pt, main_menu& parent);
      void popup_at (const core::point& pt, win::window& parent);

      menu_data data;

    private:
      void init ();

      void popup_at (win::window& parent, menu_data& parent_data, const core::point& pt);

      core::size_type calc_width ();

      struct positions {
        positions ();

        core::point_type text;
        core::point_type hotkey;

      } pos;
    };

    // --------------------------------------------------------------------------
    // inlines
    inline const std::string& menu_entry::get_label () const {
      return label;
    }

    inline const win::hot_key& menu_entry::get_hot_key () const {
      return hotkey;
    }

    inline auto menu_entry::get_icon() const->const icon_type& {
      return icon;
    }

    inline core::size_type menu_entry::get_width () const {
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

    inline void menu_entry::set_label (const std::string& l) {
      label = l;
    }

    inline void menu_entry::set_icon (const icon_type& i) {
      icon = i;
    }

    inline void menu_entry::set_enabled (bool d) {
      state = d ? menu_state::enabled : menu_state::disabled;
    }

    inline void menu_entry::set_width (core::size_type w) {
      width = w;
    }

    inline void menu_entry::select () const {
      if (is_enabled() && action) {
        action();
      }
    }

    // --------------------------------------------------------------------------
    inline sub_menu_entry::sub_menu_entry (const std::string& label,
                                           char menu_key,
                                           const std::function<menu_action>& action,
                                           bool separator,
                                           const icon_type& icon,
                                           menu_state state)
      : menu_entry(true, label, menu_key, action, win::hot_key(), separator, icon, state)
    {}

    // --------------------------------------------------------------------------
    inline main_menu_entry::main_menu_entry (const std::string& label,
                                             char menu_key,
                                             const std::function<menu_action>& action,
                                             menu_state state)
      : menu_entry(true, label, menu_key, action, win::hot_key(), false, icon_type(), state)
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

    inline void menu_data::clear_selection (win::event_source src) {
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

    // --------------------------------------------------------------------------
    inline popup_menu::popup_menu (std::initializer_list<menu_entry> entries)
      : popup_menu() {
      data.add_entries(entries);
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

    // --------------------------------------------------------------------------

  } // ctrl

} // gui
