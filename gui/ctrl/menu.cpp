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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/frames.h>
#include <gui/draw/drawers.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/look/menu.h>


using namespace gui::win;


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    menu_entry::menu_entry (const core::text_source& label,
                            char menu_key,
                            std::function<menu_action> action,
                            const core::hot_key& hotkey,
                            bool separator,
                            const icon_type& icon,
                            menu_state state)
      : label(label)
      , hotkey(hotkey)
      , icon(icon)
      , action(std::move(action))
      , menu_key(menu_key)
      , width(0)
      , separator(separator)
      , state(state)
      , sub_menu(false)
    {}

    menu_entry::menu_entry (const std::string& label,
                            char menu_key,
                            std::function<menu_action> action,
                            const core::hot_key& hotkey,
                            bool separator,
                            const icon_type& icon,
                            menu_state state)
      : label(core::const_text(label))
      , hotkey(hotkey)
      , icon(icon)
      , action(std::move(action))
      , menu_key(menu_key)
      , width(0)
      , separator(separator)
      , state(state)
      , sub_menu(false)
    {}

    menu_entry::menu_entry (bool sub_menu,
                            const core::text_source& label,
                            char menu_key,
                            std::function<menu_action> action,
                            const core::hot_key& hotkey,
                            bool separator,
                            const icon_type& icon,
                            menu_state state)
      : label(label)
      , hotkey(hotkey)
      , icon(icon)
      , action(std::move(action))
      , menu_key(menu_key)
      , width(0)
      , separator(separator)
      , state(state)
      , sub_menu(sub_menu)
    {}

    menu_entry::menu_entry ()
      : menu_key(0)
      , width(0)
      , separator(false)
      , state(menu_state::enabled)
      , sub_menu(false)
    {}

    menu_entry& menu_entry::operator= (const menu_entry& rhs) {
      if (this != &rhs) {
        label = rhs.label;
        hotkey = rhs.hotkey;
        icon = rhs.icon;
        action = rhs.action;
        menu_key = rhs.menu_key;
        width = rhs.width;
        separator = rhs.separator;
        state = rhs.state;
        sub_menu = rhs.sub_menu;
      }
      return *this;
    }

    menu_entry& menu_entry::operator= (menu_entry&& rhs) noexcept {
      if (this != &rhs) {
        label = std::move(rhs.label);
        hotkey = std::move(rhs.hotkey);
        icon = std::move(rhs.icon);
        action = std::move(rhs.action);
        menu_key = rhs.menu_key;
        width = rhs.width;
        separator = rhs.separator;
        state = rhs.state;
        sub_menu = rhs.sub_menu;
      }
      return *this;
    }

    void menu_entry::check_hot_key (os::key_state m, os::key_symbol k) {
      if (action && hotkey.match(m, k)) {
        action();
      }
    }

    // --------------------------------------------------------------------------
    menu_data::~menu_data () {
      unregister_hot_keys();
      unregister_menu_keys();
    }

    void menu_data::add_entries (const std::vector<menu_entry>& new_entries) {
      const draw::font& f = draw::font::menu();

      for (auto& i : new_entries) {
        data.items.push_back(i);
        menu_entry& entry = data.items.back();
        entry.set_width(f.get_text_size(entry.get_label()).width());
      }
    }

    void menu_data::add_entry (const menu_entry& entry) {
      data.items.push_back(entry);
      menu_entry& e = data.items.back();
      const draw::font& f = draw::font::menu();
      e.set_width(f.get_text_size(e.get_label()).width());
    }

    void menu_data::add_entry (menu_entry&& entry) {
      data.items.push_back(std::move(entry));
      menu_entry& e = data.items.back();
      const draw::font& f = draw::font::menu();
      e.set_width(f.get_text_size(e.get_label()).width());
    }

    void menu_data::set_selection (int sel, event_source src) {
      int new_selection = std::max(-1, sel);
      if (new_selection >= static_cast<int>(size())) {
        new_selection = -1;
      }
      if ((new_selection != -1) && data.items[new_selection].is_disabled()) {
        return;
      }
      if (new_selection != data.selection) {
        logging::trace() << "menu_data::set_selection(" << new_selection << ") from " << data.selection;
        close_current_child();
        data.selection = new_selection;
        data.hilite = new_selection;
        view().invalidate();
        logging::trace() << "menu_data::set_selection notify_event SELECTION_CHANGE_MESSAGE";
        if (new_selection > -1) {
          if (!at(new_selection).is_sub_menu()) {
            close_parent();
          }
          at(new_selection).select();
        }
        view().notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(src));
      }
    }

    template<typename T>
    T rotate (T n, T delta, T max) {
      n += delta;
      if (n < 0) {
        n = max - 1;
      } else if (n > max - 1) {
        n = 0;
      }
      return n;
    }

    void menu_data::rotate_selection (int delta) {
      const int last = get_selection();
      const int sz = static_cast<int>(size());
      int next = rotate<int>(last, delta, sz);
      while (data.items[next].is_disabled() && (next != last)) {
        next = rotate<int>(next, delta, sz);
      }
      set_selection(next, event_source::keyboard);
    }

    void menu_data::rotate_hilite (int delta) {
      const int last = get_hilite();
      const int sz = static_cast<int>(size());
      int next = rotate<int>(last, delta, sz);
      while (data.items[next].is_disabled() && (next != last)) {
        next = rotate<int>(next, delta, sz);
      }
      set_hilite(next);
    }

    void menu_data::set_hilite (int sel) {
      int new_hilite = std::max(-1, sel);
      if (new_hilite >= static_cast<int>(size())) {
        new_hilite = -1;
      }
      if ((new_hilite != -1) && data.items[new_hilite].is_disabled()) {
        return;
      }
      if (data.hilite != new_hilite) {
        logging::trace() << "menu_data::set_hilite(" << new_hilite << ") from " << data.hilite;
        const bool was_open = is_open();
        close_current_child();
        data.hilite = new_hilite;
        view().invalidate();
        logging::trace() << "menu_data::set_hilite notify_event HILITE_CHANGE_MESSAGE";
        if ((new_hilite > -1) && at(new_hilite).is_sub_menu() && was_open) {
          data.selection = new_hilite;
          at(new_hilite).select();
        } else {
          data.selection = -1;
        }
        view().notify_event(detail::HILITE_CHANGE_MESSAGE, true);
      }
    }

    void menu_data::clear_hilite () {
      logging::trace() << "menu_data::clear_hilite()";
      data.hilite = -1;
      view().invalidate();
      logging::trace() << "menu_data::clear_hilite notify_event HILITE_CHANGE_MESSAGE";
      view().notify_event(detail::HILITE_CHANGE_MESSAGE, false);
    }

    void menu_data::close () {
      logging::trace() << "menu_data::close()";
      data.selection = -1;
      data.hilite = -1;
      view().invalidate();
      close_current_child();
    }

    void menu_data::close_current_child () {
      logging::trace() << "menu_data::close_current_child()";
      if (is_open()) {
        data.current_child->close();
      }
    }

    void menu_data::close_parent () {
      logging::trace() << "menu_data::close_parent()";
      if (has_parent()) {
        data.parent->close_parent();
      } else {
        close();
      }
    }

    void menu_data::init () {
      data.selection = -1;
      data.hilite = -1;

      view().on_mouse_move([&](os::key_state, const core::native_point& pt) {
        handle_mouse(false, pt);
      });

      view().on_left_btn_down([&] (os::key_state, const core::native_point& pt) {
        view().take_focus();
        handle_mouse(true, pt);
      });

      view().on_any_key_down([&] (os::key_state state, os::key_symbol key, const std::string &) {
        if (state == core::state::none) {
          handle_key(key);
        }
      });

      view().on_mouse_leave([&] () {
        logging::trace() << "menu_data::on_mouse_leave() -> clear_hilite()";
        clear_hilite();
      });


    }

    void menu_data::check_hot_key (os::key_state st, os::key_symbol sym) {
      for (auto& i : data.items) {
        i.check_hot_key(st, sym);
      }
    }

    void menu_data::register_hot_keys (window* w) {
      for (auto& i : data.items) {
        if (!i.get_hot_key().empty()) {
          global::register_hot_key(i.get_hot_key(), i.get_action(), w);
        }
      }
    }

    void menu_data::unregister_hot_keys () {
      for (auto& i : data.items) {
        if (!i.get_hot_key().empty()) {
          global::unregister_hot_key(i.get_hot_key());
        }
      }
    }

    void menu_data::register_menu_keys (window* w) {
      int idx = -1;
      for (auto& i : data.items) {
        ++idx;
        if (i.get_menu_key()) {
          global::register_hot_key(core::hot_key(i.get_menu_key(), core::state::alt), [&, idx]() {
            view().take_focus();
            set_selection(idx, event_source::keyboard);
          }, w);
        }
      }
    }

    void menu_data::unregister_menu_keys () {
      for (auto& i : data.items) {
        if (i.get_menu_key()) {
          global::unregister_hot_key(core::hot_key(i.get_menu_key(), core::state::alt));
        }
      }
    }

    int menu_data::get_index_of (const menu_entry& e) const {
      int idx = -1;
      for (auto& i : data.items) {
        ++idx;
        if (&i == &e) {
          return idx;
        }
      }
      return idx;
    }

    item_state menu_data::get_item_state (int idx) const {
      return item_state(get_hilite() == idx, get_selection() == idx, data.items[idx].is_disabled());
    }

    void menu_data::handle_mouse (bool btn, const core::native_point& gpt){
      if (view().surface_geometry().is_inside(gpt)) {
        const auto idx = get_index_at_point(view().surface_to_client(gpt));
        logging::trace() << "menu_data::handle_mouse at:" << gpt << " -> index:" << idx << " in window " << view();
        if (btn) {
          if (idx == get_selection()) {
            clear_selection(event_source::mouse);
          } else {
            set_selection(idx, event_source::mouse);
          }
        } else {
          set_hilite(idx);
        }
      } else if (btn) {
        close();
      }
    }

    // --------------------------------------------------------------------------
    main_menu::main_menu () {
      init();
    }

    win::window& main_menu::view () {
      return window;
    }

    void main_menu::init () {
      super::init();
      window.on_paint(draw::paint(this, &main_menu::paint));

      window.on_set_focus([&] () {
        if (get_hilite() == -1) {
          set_hilite(0);
        }
      });

      window.on_lost_focus([&] () {
        if (get_hilite() > -1) {
          clear_hilite();
        }
      });

      window.on_create([&] () {
        register_menu_keys(&window);
      });
    }

    bool main_menu::handle_key (os::key_symbol key) {
      if (is_open() && data.current_child->handle_key(key)) {
        return true;
      }
      if (!window.is_focused()) {
        return false;
      }
      switch (key) {
      case core::keys::left:
      case core::keys::numpad::left:
        if (is_open()) {
          rotate_selection(-1);
        } else {
          rotate_hilite(-1);
        }
        return true;

      case core::keys::right:
      case core::keys::numpad::right:
        if (is_open()) {
          rotate_selection(1);
        } else {
          rotate_hilite(1);
        }
        return true;

      case core::keys::up:
      case core::keys::numpad::up:
      case core::keys::escape:
        close_current_child();
        window.invalidate();
        return true;

      case core::keys::down:
      case core::keys::numpad::down:
      case core::keys::enter:
        if (get_hilite() > -1) {
          set_selection(get_hilite(), event_source::keyboard);
          return true;
        }
      }
      return false;
    }

    int main_menu::get_index_at_point (const core::point& pt) const {
      if (window.client_geometry().is_inside(pt)) {
        core::point::type pos = 0;
        int idx = -1;
        for (auto& i : data.items) {
          ++idx;
          pos += i.get_width() + 20;
          if (pt.x() < pos) {
            return idx;
          }
        }
      }
      return -1;
    }

    core::point main_menu::sub_menu_position (std::size_t idx) const {
      auto r = window.absolute_position();
      core::point::type pos = 0;
      for (auto& i : data.items) {
        if (idx < 1) {
          break;
        }
        pos += i.get_width() + 20;
        --idx;
      }
      return (r + core::point(pos, window.size().height()));
    }

    void main_menu::paint (draw::graphics& g) {
      draw::brush back_brush(color::menuColor());
      const core::rectangle area = window.client_geometry();
      core::rectangle r = area;
      int idx = -1;
      for (auto& i : data.items) {
        ++idx;
        auto w = i.get_width() + 20;
        r.width(w);
        look::main_menu_item(g, r, back_brush, i.get_label(), i.get_menu_key(),
                             get_item_state(idx));
        r.move_x(w);
      }
      if (r.x() < area.x2()) {
        g.fill(draw::rectangle(core::rectangle(r.top_left(), core::size(area.x2(), area.y2()))), back_brush);
      }
    }

    // --------------------------------------------------------------------------
    popup_menu::popup_menu () {
      init();
    }

    popup_menu::~popup_menu () {
      unregister_message_filter();
    }

    win::window& popup_menu::view () {
      return window;
    }

    void popup_menu::init () {
      super::init();
      message_filter_id = -1;

      window.on_paint(draw::paint(this, &popup_menu::paint));

      window.on_lost_focus([&] () {
        this->window.invalidate();
      });

      window.on_show([&] () {
        this->window.invalidate();
        register_message_filter();
      });
    }

    void popup_menu::register_message_filter () {
      unregister_message_filter();
      message_filter_id = win::global::register_message_filter([&] (const core::event& e) {
        if (win::left_btn_down_event::match(e)) {
          auto gpt = win::left_btn_down_event::Caller::get_param<1>(e);
          if (!window.surface_geometry().is_inside(gpt)) {
            close();
            return true;
          }
        }
        return false;
      });
      logging::trace() << "popup_menu::register_message_filter " << message_filter_id;
    }

    void popup_menu::unregister_message_filter () {
      if (message_filter_id != -1) {
        logging::trace() << "popup_menu::unregister_message_filter " << message_filter_id;
        win::global::unregister_message_filter(message_filter_id);
        message_filter_id = -1;
      }
    }

    bool compare_menu_key (os::key_symbol key, const menu_entry& e) {
      return toupper(key) == toupper(e.get_menu_key());
    }

    bool popup_menu::handle_key (os::key_symbol key) {
      if (is_open() && data.current_child->handle_key(key)) {
        return true;
      }
      switch (key) {
      case core::keys::left:
      case core::keys::numpad::left:
      case core::keys::escape:
        if (is_open()) {
          int idx = get_hilite();
          close_current_child();
          set_hilite(idx);
          window.invalidate();
          return true;
        }
        break;

        case core::keys::right:
        case core::keys::numpad::right: {
          int idx = get_hilite();
          if (!is_open() && (idx > -1) && at(idx).is_sub_menu()) {
            set_selection(idx, event_source::keyboard);
            return true;
          }
          break;
        }

      case core::keys::up:
      case core::keys::numpad::up:
          if (is_open()) {
            rotate_selection(-1);
          } else {
            rotate_hilite(-1);
          }
        return true;

      case core::keys::down:
      case core::keys::numpad::down:
          if (is_open()) {
            rotate_selection(1);
          } else {
            rotate_hilite(1);
          }
        return true;

      case core::keys::enter:
        if (get_hilite() > -1) {
          set_selection(get_hilite(), event_source::keyboard);
        }
        return true;

      default: {
        int idx = -1;
        for (auto& i : data.items) {
          ++idx;
          if (compare_menu_key(key, i)) {
            set_selection(idx, event_source::keyboard);
            return true;
          }
        }
        break;
      }
      }
      return false;
    }

    void popup_menu::popup_at (const core::point& pt, win::window& parent) {
      window.set_state().disable_redraw(false);
      set_hilite(0);
      logging::trace() << "popup_menu::popup_at(" << pt << ") -> run_modal";
      auto& root = parent.get_overlapped_window();
      window.create(root, core::rectangle(parent.client_to_screen(pt), core::size(calc_width() + 2, static_cast<core::size::type>(size() * item_height + 2))));
      window.set_visible();
    }

    void popup_menu::popup_at (container& parent, menu_data& parent_data, const core::point& pt) {
      window.set_state().disable_redraw(false);
      set_hilite(0);

      set_parent(&parent_data);
      parent_data.set_current_child(this);

      logging::trace() << "popup_menu::popup_at(" << pt << ") none-modal";
      auto& root = parent.get_overlapped_window();
      window.create(root, core::rectangle(pt, core::size(calc_width() + 2, static_cast<core::size::type>(size() * item_height + 2))));
      window.set_visible();
    }

    core::point popup_menu::sub_menu_position (int idx) const {
      auto r = window.absolute_position();
      return (r + core::point(window.size().width(), static_cast<core::point::type>(idx * item_height)));
    }

    int popup_menu::get_index_at_point (const core::point& pt) const {
      if (window.client_geometry().is_inside(pt)) {
        return std::min(static_cast<int>(size()), static_cast<int>((pt.y() - 1) / item_height));
      }
      return -1;
    }

    void popup_menu::close () {
      logging::trace() << "popup_menu::close";
      window.set_state().disable_redraw();
      if (has_parent()) {
        data.parent->clear_current_child(this);
        clear_parent();;
      }
      super::close();
      unregister_message_filter();
      window.set_visible(false);
      window.destroy();
    }

    void popup_menu::paint (draw::graphics& g) {
      draw::brush back_brush(color::menuColor());
      const core::rectangle area = window.client_geometry();
      draw::frame::raised_relief(g, area);
      core::rectangle r = area.shrinked({1, 1});
      int idx = -1;
      for (auto& i : data.items) {
        ++idx;
        r.height(static_cast<core::size::type>(item_height));
        look::menu_item(g, r, back_brush, pos.text, pos.hotkey,
                        i.get_label(), i.get_menu_key(), i.get_icon(),
                        i.get_hot_key(), i.is_sub_menu(), i.has_separator(),
                        get_item_state(idx));
        r.move_y(static_cast<core::size::type>(item_height));
      }
    }

    core::size::type popup_menu::calc_width () {
      core::size::type label_width = 0;
      core::size::type hotkey_width = 0;
      bool has_sub = false;
      const draw::font& f = draw::font::menu();
      for (auto& i : data.items) {
        label_width = std::max(label_width, i.get_width());
        has_sub |= i.is_sub_menu();
        hotkey_width = std::max(hotkey_width, f.get_text_size(i.get_hot_key().get_key_string()).width());
      }
      pos.text = 36;
      pos.hotkey = pos.text + label_width + 20;
      return pos.hotkey + (hotkey_width ? hotkey_width + 10 : 0) + (has_sub ? 20 : 0);
    }

    // --------------------------------------------------------------------------
    menu_entry sub_menu_entry (const core::text_source& label,
                               char menu_key,
                               std::function<menu_action> action,
                               bool separator,
                               const menu_entry::icon_type& icon,
                               menu_state state) {
      return menu_entry(true, label, menu_key, std::move(action), core::hot_key(), separator, icon, state);
    }

    menu_entry sub_menu_entry (const std::string& label,
                               char menu_key,
                               std::function<menu_action> action,
                               bool separator,
                               const menu_entry::icon_type& icon,
                               menu_state state) {
      return menu_entry(true, core::const_text(label), menu_key, std::move(action), core::hot_key(), separator, icon, state);
    }

    // --------------------------------------------------------------------------
    menu_entry main_menu_entry (const core::text_source& label,
                                char menu_key,
                                std::function<menu_action> action,
                                menu_state state) {
      return menu_entry(true, label, menu_key, std::move(action), core::hot_key(), false, menu_entry::icon_type(), state);
    }

    menu_entry main_menu_entry (const std::string& label,
                                char menu_key,
                                std::function<menu_action> action,
                                menu_state state) {
      return menu_entry(true, core::const_text(label), menu_key, std::move(action), core::hot_key(), false, menu_entry::icon_type(), state);
    }

    menu_entry main_menu_entry (const core::text_source& label,
                                char menu_key,
                                main_menu& main,
                                popup_menu& sub) {
      return menu_entry(true, label, menu_key, [&] () {
        sub.popup(main);
      });
    }

    menu_entry main_menu_entry (const std::string& label,
                                char menu_key,
                                main_menu& main,
                                popup_menu& sub) {
      return menu_entry(true, core::const_text(label), menu_key, [&] () {
        sub.popup(main);
      });
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui
