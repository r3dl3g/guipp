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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/menu.h>


using namespace gui::win;


namespace gui {

  namespace ctrl {

    namespace paint {

      void draw_menu_label (const draw::graphics& g,
                            const core::rectangle& r,
                            const std::string& l,
                            char k,
                            os::color c) {
        const draw::font& f = draw::font::menu();
        g.text(draw::text_box(l, r, text_origin::vcenter_left), f, c);

        if (k) {
          auto i = l.find(k);
          if (i != std::string::npos) {
            core::rectangle r1 = r;
            g.text(draw::bounding_box(l.substr(0, i + 1), r1, text_origin::vcenter_left), f, c);
            core::point::type x1 = r.x() + 1;
            core::point::type x2 = r1.x2() - 1;
            core::point::type y = r1.y2();
            if (i > 0) {
              core::rectangle r0 = r;
              g.text(draw::bounding_box(l.substr(0, i), r0, text_origin::vcenter_left), f, c);
              x1 = r0.x2();
            }
            g.draw_lines({core::point(x1, y), core::point(x2, y)}, c);
          }
        }

      }

      // --------------------------------------------------------------------------
      void main_menu_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           char menu_key,
                           bool selected,
                           bool hilited,
                           bool disabled) {
        if (selected) {
          g.fill(draw::rectangle(r), color::highLightColor());
        } else if (hilited) {
          g.fill(draw::rectangle(r), color::menuColorHighlight());
        } else {
          g.fill(draw::rectangle(r), background);
        }

        os::color col = disabled ? color::disabledTextColor()
                        : selected ? color::highLightTextColor()
                        : color::black;

        core::rectangle r2 = r + core::point(10, 0);
        draw_menu_label(g, r2, label, menu_key, col);
      }

      // --------------------------------------------------------------------------
      void menu_item (const draw::graphics& g,
                      const core::rectangle& r,
                      const draw::brush& background,
                      core::point::type text_pos,
                      core::point::type hotkey_pos,
                      const std::string& label,
                      char menu_key,
                      const draw::masked_bitmap& icon,
                      const hot_key& hotkey,
                      bool is_sub_menu,
                      bool separator,
                      bool selected,
                      bool hilited,
                      bool disabled) {
        if (selected) {
          if (separator) {
            g.fill(draw::rectangle(r.with_height(2)), background);
            g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::highLightColor());
          } else {
            g.fill(draw::rectangle(r), color::highLightColor());
          }
        } else if (hilited) {
          if (separator) {
            g.fill(draw::rectangle(r.with_height(2)), background);
            g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::menuColorHighlight());
          } else {
            g.fill(draw::rectangle(r), color::menuColorHighlight());
          }
        } else {
          g.fill(draw::rectangle(r), background);
        }

        core::rectangle r2 = r + core::point(text_pos, 0);

        if (separator) {
          core::rectangle r3(core::point(text_pos - 2, r.y()), core::point(r.x2(), r.y() + 2));
          draw::frame::hgroove(g, r3);
          r2 += core::point(0, 2);
        }

        os::color col = disabled ? color::disabledTextColor()
                        : selected ? color::highLightTextColor()
                        : color::black;

        if (icon) {
          core::size sz = icon.image.size();
          core::point::type x = (text_pos - sz.width()) / 2;
          core::point::type y = r.y() + (r.height() - sz.height()) / 2;
          g.copy_from(icon, core::point(x, y));
        }

        draw_menu_label(g, r2, label, menu_key, col);

        if (!hotkey.empty()) {
          r2.x(hotkey_pos);
          g.text(draw::text_box(hotkey.get_key_string(), r2, text_origin::vcenter_left),
                 draw::font::menu(), col);
        }
        if (is_sub_menu) {
          core::point::type y = r.center_y();
          core::point::type x = r.x2() - 8;
          g.fill(draw::polygon({core::point(x, y - 4),
                                core::point(x, y + 4),
                                core::point(x + 4, y)}), color::black);
        }
      }

    }

    // --------------------------------------------------------------------------
    menu_entry::menu_entry (const std::string& label,
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
      , sub_menu(false)
    {}

    menu_entry::menu_entry (const menu_entry& rhs)
      : label(rhs.label)
      , hotkey(rhs.hotkey)
      , icon(rhs.icon)
      , action(rhs.action)
      , menu_key(rhs.menu_key)
      , width(rhs.width)
      , separator(rhs.separator)
      , state(rhs.state)
      , sub_menu(rhs.sub_menu)
    {}

    menu_entry::menu_entry (menu_entry&& rhs)
      : label(std::move(rhs.label))
      , hotkey(std::move(rhs.hotkey))
      , icon(std::move(rhs.icon))
      , action(std::move(rhs.action))
      , menu_key(rhs.menu_key)
      , width(rhs.width)
      , separator(rhs.separator)
      , state(rhs.state)
      , sub_menu(rhs.sub_menu)
    {}

    menu_entry::menu_entry (bool sub_menu,
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

    menu_entry::menu_entry ()
      : menu_key(0)
      , width(0)
      , separator(false)
      , state(menu_state::enabled)
      , sub_menu(false)
    {}

    void menu_entry::operator= (const menu_entry& rhs) {
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
    }

    void menu_entry::operator= (menu_entry&& rhs) {
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

    void menu_data::add_entries (std::initializer_list<menu_entry> new_entries) {
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
        close();
        data.selection = new_selection;
        data.hilite = new_selection;
        win->redraw_now();
        send_client_message(win, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(src));
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
        close();
        data.hilite = new_hilite;
        win->redraw();
        send_client_message(win, detail::HILITE_CHANGE_MESSAGE, true);
      }
    }

    void menu_data::clear_hilite () {
      data.hilite = -1;
      win->redraw();
      send_client_message(win, detail::HILITE_CHANGE_MESSAGE, false);
    }

    void menu_data::close () {
      if (data.close_caller) {
        data.close_caller();
      }
      data.selection = -1;
      data.hilite = -1;
      data.close_caller = nullptr;
      data.key_caller = nullptr;
      win->redraw();
    }

    void menu_data::handle_mouse (bool b, const core::point& pt) {
      if (data.mouse_caller) {
        data.mouse_caller(b, pt);
      }
    }

    bool menu_data::handle_key (os::key_symbol k) {
      if (data.key_caller) {
        return data.key_caller(k);
      }
      return false;
    }

    void menu_data::init () {
      data.selection = -1;
      data.hilite = -1;
      data.close_caller = nullptr;
      data.key_caller = nullptr;
      data.mouse_caller = nullptr;
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
          global::register_hot_key(hot_key(i.get_menu_key(), state::alt), [&, idx]() {
            win->take_focus();
            set_selection(idx, event_source::keyboard);
          }, w);
        }
      }
    }

    void menu_data::unregister_menu_keys () {
      for (auto& i : data.items) {
        if (i.get_menu_key()) {
          global::unregister_hot_key(hot_key(i.get_menu_key(), state::alt));
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

    // --------------------------------------------------------------------------
    main_menu::main_menu ()
      : data(this)
    {
      init();
    }

    main_menu::main_menu (const main_menu& rhs)
      : super(rhs)
      , data(this, rhs.data)
    {
      init();
    }

    main_menu::main_menu (main_menu&& rhs)
      : super(std::move(rhs))
      , data(this, std::move(rhs.data))
    {
      init();
    }

    void main_menu::init () {
      set_accept_focus(true);
      data.set_mouse_function(basepp::bind_method(this, &main_menu::handle_mouse));

      on_paint(draw::buffered_paint(this, &main_menu::paint));

      on_mouse_move_abs([&] (os::key_state, const core::point & pt) {
        data.handle_mouse(false, pt);
      });

      on_mouse_leave(basepp::bind_method(&data, &menu_data::clear_hilite));

      on_set_focus([&] (window*) {
        if (data.get_hilite() == -1) {
          data.set_hilite(0);
        }
      });

      on_lost_focus([&] (window*) {
        if (data.get_hilite() > -1) {
          data.clear_hilite();
        }
      });

      on_selection_changed([&] (event_source) {
        int idx = data.get_selection();
        if (idx > -1) {
          data[idx].select();
        }
      });

      on_left_btn_down([&] (os::key_state, const core::point & pt) {
        take_focus();
        data.handle_mouse(true, client_to_screen(pt));
      });

      on_any_key_down([&] (os::key_state, os::key_symbol key, const std::string &) {
        handle_key(key);
      });

      on_create([&] (window *, const core::rectangle &) {
        data.register_menu_keys(this);
      });
    }

    void main_menu::handle_mouse (bool btn, const core::point& gpt){
      if (absolute_place().is_inside(gpt)) {
        const auto idx = get_index_at_point(screen_to_client(gpt));
        if (btn) {
          if (idx == data.get_selection()) {
            data.clear_selection(event_source::mouse);
          } else {
            data.set_selection(idx, event_source::mouse);
          }
        } else {
          if (data.is_open()) {
            if (idx != -1) {
              data.set_selection(idx, event_source::mouse);
            }
          } else {
            data.set_hilite(idx);
          }
        }
      } else if (btn) {
        data.close();
      }
    }

    bool main_menu::handle_key (os::key_symbol key) {
      if (data.handle_key(key)) {
        return true;
      }
      switch (key) {
      case win::keys::left:
      case win::keys::numpad::left:
        if (data.is_open()) {
          data.rotate_selection(-1);
        } else {
          data.rotate_hilite(-1);
        }
        return true;

      case win::keys::right:
      case win::keys::numpad::right:
        if (data.is_open()) {
          data.rotate_selection(1);
        } else {
          data.rotate_hilite(1);
        }
        return true;

      case win::keys::up:
      case win::keys::numpad::up:
      case win::keys::escape:
        data.close();
        redraw();
        return true;

      case win::keys::down:
      case win::keys::numpad::down:
      case win::keys::enter:
        if (!data.is_open() && (data.get_hilite() > -1)) {
          data.set_selection(data.get_hilite(), event_source::keyboard);
        }
        return true;
      }
      return false;
    }

    int main_menu::get_index_at_point (const core::point& pt) const {
      if (client_area().is_inside(pt)) {
        core::point::type pos = 0;
        int idx = -1;
        for (auto& i : data) {
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
      auto r = absolute_position();
      core::point::type pos = 0;
      for (auto& i : data) {
        if (idx < 1) {
          break;
        }
        pos += i.get_width() + 20;
        --idx;
      }
      return (r + core::point(pos, size().height()));
    }

    void main_menu::paint (const draw::graphics& g) {
      draw::brush back_brush(color::menuColor());
      const core::rectangle area = client_area();
      core::rectangle r = area;
      int idx = -1;
      for (auto& i : data) {
        ++idx;
        auto w = i.get_width() + 20;
        r.width(w);
        paint::main_menu_item(g, r, back_brush, i.get_label(), i.get_menu_key(),
                              (idx == data.get_selection()),
                              (idx == data.get_hilite()), i.is_disabled());
        r.move_x(w);
      }
      if (r.x() < area.x2()) {
        g.fill(draw::rectangle(core::rectangle(r.top_left(), area.bottom_right())), back_brush);
      }
    }

    // --------------------------------------------------------------------------
    popup_menu::popup_menu ()
      : data(this)
    {
      init();
    }

    popup_menu::popup_menu (const popup_menu& rhs)
      : super(rhs)
      , data(this, rhs.data)
    {
      init();
    }

    popup_menu::popup_menu (popup_menu&& rhs)
      : super(std::move(rhs))
      , data(this, std::move(rhs.data))
    {
      init();
    }

    void popup_menu::init () {
      on_paint(draw::paint(basepp::bind_method(this, &popup_menu::paint)));

      on_mouse_move_abs([&](os::key_state, const core::point & pt) {
        data.handle_mouse(false, pt);
      });

      on_mouse_leave(basepp::bind_method(&data, &menu_data::clear_hilite));

      on<selection_changed_event>([&](event_source) {
        int idx = data.get_selection();
        if (idx > -1) {
          if (!data[idx].is_sub_menu()) {
            data.handle_mouse(true, core::point::zero);
          }
          data[idx].select();
        }
      });

      on_left_btn_down([&](os::key_state, const core::point & pt) {
        data.handle_mouse(true, client_to_screen(pt));
      });

      on_lost_focus([&](window*) {
        redraw();
      });

      on_any_key_down([&] (os::key_state, os::key_symbol key, const std::string &) {
        handle_key(key);
      });

      on_show([&] () {
        capture_pointer();
      });

//      on_create([&](window*, const core::rectangle&){
//        data.register_menu_keys();
//      });
    }

    bool compare_menu_key (os::key_symbol key, const menu_entry& e) {
      return toupper(key) == toupper(e.get_menu_key());
    }

    bool popup_menu::handle_key (os::key_symbol key) {
      if (data.handle_key(key)) {
        return true;
      }
      switch (key) {
      case win::keys::left:
      case win::keys::numpad::left:
      case win::keys::escape:
        if (data.is_open()) {
          int idx = data.get_hilite();
          data.close();
          data.set_hilite(idx);
          redraw();
          return true;
        }
        break;
      case win::keys::up:
      case win::keys::numpad::up:
        data.rotate_hilite(-1);
        return true;

      case win::keys::right:
      case win::keys::numpad::right: {
        int idx = data.get_hilite();
        if (!data.is_open() && (idx > -1) && data[idx].is_sub_menu()) {
          data.set_selection(idx, event_source::keyboard);
          return true;
        }
        break;
      }
      case win::keys::down:
      case win::keys::numpad::down:
        data.rotate_hilite(1);
        return true;

      case win::keys::enter:
        if (data.get_hilite() > -1) {
          data.set_selection(data.get_hilite(), event_source::keyboard);
        }
        return true;

      default: {
        int idx = -1;
        for (auto& i : data) {
          ++idx;
          if (compare_menu_key(key, i)) {
            data.set_selection(idx, event_source::keyboard);
            return true;
          }
        }
        break;
      }
      }
      return false;
    }

    void popup_menu::handle_mouse (bool btn, const core::point& gpt) {
      const auto idx = get_index_at_point(screen_to_client(gpt));
      if (btn) {
        if (!data.is_open() || (idx != data.get_hilite())) {
          data.set_selection(idx, event_source::mouse);
        }
      } else {
        if ((idx > -1) && data[idx].is_sub_menu()) {
          data.set_selection(idx, event_source::mouse);
        } else {
          data.set_hilite(idx);
        }
      }
    }

    void popup_menu::popup_at (const core::point& pt, window& parent) {
      data.init();
      data.set_hilite(0);
      data.set_mouse_function([&] (bool btn, const core::point & gpt) {
        if (absolute_place().is_inside(gpt)) {
          handle_mouse(btn, gpt);
        } else if (btn) {
          close();
        }
      });
      create(parent, core::rectangle(pt, core::size(calc_width() + 2, static_cast<core::size::type>(data.size() * item_height + 2))));
      set_visible();
      run_modal();
    }

    void popup_menu::popup_at (window& parent, menu_data& parent_data, const core::point& pt) {
      data.init();
      data.set_hilite(0);
      parent_data.set_close_function([&] () {
        close();
        parent_data.clear_close_function();
      });
      parent_data.set_key_function(basepp::bind_method(this, &popup_menu::handle_key));

      data.set_mouse_function([&] (bool btn, const core::point & gpt) {
        if (absolute_place().is_inside(gpt)) {
          handle_mouse(btn, gpt);
        } else {
          parent_data.handle_mouse(btn, gpt);
        }
      });
      create(parent, core::rectangle(pt, core::size(calc_width() + 2, static_cast<core::size::type>(data.size() * item_height + 2))));
      set_visible();
      run_modal();
    }

    core::point popup_menu::sub_menu_position (int idx) const {
      auto r = absolute_position();
      return (r + core::point(size().width(), static_cast<core::point::type>(idx * item_height)));
    }

    int popup_menu::get_index_at_point (const core::point& pt) const {
      if (client_area().shrinked({1, 1}).is_inside(pt)) {
        return std::min(static_cast<int>(data.size()), static_cast<int>((pt.y() - 1) / item_height));
      }
      return -1;
    }

    void popup_menu::close () {
      get_state().disable_redraw();
      data.close();
      uncapture_pointer();
      set_visible(false);
      destroy();
      end_modal();
    }

    void popup_menu::paint (const draw::graphics& g) {
      draw::brush back_brush(color::menuColor());
      const core::rectangle area = client_area();
      draw::frame::raised_relief(g, area);
      core::rectangle r = area.shrinked({1, 1});
      int idx = -1;
      for (auto& i : data) {
        ++idx;
        r.height(static_cast<core::size::type>(item_height));
        paint::menu_item(g, r, back_brush, pos.text, pos.hotkey,
                         i.get_label(), i.get_menu_key(), i.get_icon(),
                         i.get_hot_key(), i.is_sub_menu(), i.has_separator(),
                         (idx == data.get_selection()), (idx == data.get_hilite()),
                         i.is_disabled());
        r.move_y(static_cast<core::size::type>(item_height));
      }
    }

    core::size::type popup_menu::calc_width () {
      core::size::type label_width = 0;
      core::size::type hotkey_width = 0;
      bool has_sub = false;
      const draw::font& f = draw::font::menu();
      for (auto& i : data) {
        label_width = std::max(label_width, i.get_width());
        has_sub |= i.is_sub_menu();
        hotkey_width = std::max(hotkey_width, f.get_text_size(i.get_hot_key().get_key_string()).width());
      }
      pos.text = 36;
      pos.hotkey = pos.text + label_width + 20;
      return pos.hotkey + (hotkey_width ? hotkey_width + 10 : 0) + (has_sub ? 20 : 0);
    }

    // --------------------------------------------------------------------------

  } // ctrl

} // gui
