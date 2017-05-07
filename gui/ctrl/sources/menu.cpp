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

#include "menu.h"

namespace gui {

  namespace win {

    namespace paint {

      // --------------------------------------------------------------------------
      void main_menu_item (const menu_entry& e,
                           const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& b,
                           bool s,
                           bool h) {
        if (s) {
          g.fill(draw::rectangle(r), color::highLightColor());
        } else if (h) {
          g.fill(draw::rectangle(r), color::menuColorHighlight());
        } else {
          g.fill(draw::rectangle(r), b);
        }

        os::color col = e.is_disabled() ? color::disabledTextColor()
                                        : s ? color::highLightTextColor()
                                            : color::black;

        core::rectangle r2 = r + core::point(10, 0);
        g.text(draw::text_box(e.get_label(), r2, text_origin::vcenter_left),
               draw::font::menu(), col);
      }

      // --------------------------------------------------------------------------
      void menu_item (const menu_entry& e,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      core::point_type text_pos,
                      core::point_type hotkey_pos,
                      const draw::brush& b,
                      bool s,
                      bool h) {
        if (s) {
          if (e.has_separator()) {
            g.fill(draw::rectangle(r.with_height(2)), b);
            g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::highLightColor());
          } else {
            g.fill(draw::rectangle(r), color::highLightColor());
          }
        } else if (h) {
          if (e.has_separator()) {
            g.fill(draw::rectangle(r.with_height(2)), b);
            g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::menuColorHighlight());
          } else {
            g.fill(draw::rectangle(r), color::menuColorHighlight());
          }
        } else {
          g.fill(draw::rectangle(r), b);
        }

        core::rectangle r2 = r + core::point(text_pos, 0);

        if (e.has_separator()) {
          core::rectangle r3(core::point(text_pos - 2, r.y()), core::point(r.x2(), r.y() + 2));
          draw::frame::hgroove(g, r3);
          r2 += core::point(0, 2);
        }

        os::color col = e.is_disabled() ? color::disabledTextColor()
                                        : s ? color::highLightTextColor()
                                            : color::black;

        if (e.get_icon()) {
          core::size sz = e.get_icon().image.size();
          core::point_type x = (text_pos - sz.width()) / 2;
          core::point_type y = r.y() + (r.height() - sz.height()) / 2;
          g.copy_from(e.get_icon(), core::point(x, y));
        }
        g.text(draw::text_box(e.get_label(), r2, text_origin::vcenter_left),
               draw::font::menu(), col);

        if (!e.get_hotkey().empty()) {
          r2.x(hotkey_pos);
          g.text(draw::text_box(e.get_hotkey(), r2, text_origin::vcenter_left),
                 draw::font::menu(), col);
        }
        if (e.is_sub_menu()) {
          core::point_type y = r.center_y();
          core::point_type x = r.x2() - 8;
          g.fill(draw::polygon({ core::point(x, y - 4),
                                 core::point(x, y + 4),
                                 core::point(x + 4, y)}), color::black);
        }
      }

    }

    // --------------------------------------------------------------------------
    menu_entry::menu_entry (const menu_entry& rhs)
      : label(rhs.label)
      , hotkey(rhs.hotkey)
      , icon(rhs.icon)
      , action(rhs.action)
      , width(rhs.width)
      , separator(rhs.separator)
      , disabled(rhs.disabled)
      , sub_menu(rhs.sub_menu)
    {}

    void menu_entry::operator= (const menu_entry& rhs) {
      if (this != &rhs) {
        label = rhs.label;
        hotkey = rhs.hotkey;
        icon = rhs.icon;
        action = rhs.action;
        width = rhs.width;
        separator = rhs.separator;
        disabled = rhs.disabled;
        sub_menu = rhs.sub_menu;
      }
    }

    // --------------------------------------------------------------------------
    menu_entry::menu_entry (menu_entry&& rhs)
      : label(std::move(rhs.label))
      , hotkey(std::move(rhs.hotkey))
      , icon(std::move(rhs.icon))
      , action(std::move(rhs.action))
      , width(rhs.width)
      , separator(rhs.separator)
      , disabled(rhs.disabled)
      , sub_menu(rhs.sub_menu)
    {}

    void menu_entry::operator= (menu_entry&& rhs) {
      if (this != &rhs) {
        label = std::move(rhs.label);
        hotkey = std::move(rhs.hotkey);
        icon = std::move(rhs.icon);
        action = std::move(rhs.action);
        width = rhs.width;
        separator = rhs.separator;
        disabled = rhs.disabled;
        sub_menu = rhs.sub_menu;
      }
    }

    template<typename T>
    class in {
    public:
      in (const T& t)
        : value(t)
        , rval(false)
      {}

      in (T&& t)
        : value(t)
        , rval(true)
      {}

      bool rvalue () const        { return rval;  }
      bool lvalue () const        { return !rval; }

      operator const T& () const  { return value; }
      const T& get () const       { return value; }
      T&& rget () const           { return std::move(const_cast<T&>(value)); }

    private:
      const T& value;
      bool rval;
    };

    // --------------------------------------------------------------------------
    void menu_data::add_entries (std::initializer_list<menu_entry> new_entries) {
      const draw::font& f = draw::font::menu();

      for (const in<menu_entry>& i : new_entries) {
        if (i.rvalue()) {
          data.push_back(i.rget());
        } else {
          data.emplace_back(i.get());
        }
        menu_entry& entry = data.back();
        entry.set_width(f.get_text_size(entry.get_label()).width());
      }
    }

    void menu_data::add_entry (const menu_entry& entry) {
      data.push_back(entry);
      menu_entry& e = data.back();
      const draw::font& f = draw::font::menu();
      e.set_width(f.get_text_size(e.get_label()).width());
    }

    void menu_data::add_entry (menu_entry&& entry) {
      data.push_back(std::move(entry));
      menu_entry& e = data.back();
      const draw::font& f = draw::font::menu();
      e.set_width(f.get_text_size(e.get_label()).width());
    }

    int menu_data::get_selection () const {
      return selection;
    }

    void menu_data::set_selection (int sel) {
      int new_selection = std::max(-1, sel);
      if (new_selection >= size()) {
        new_selection = -1;
      }
      if ((new_selection != -1) && data[new_selection].is_disabled()) {
        return;
      }
      if (new_selection != selection) {
        close();
        selection = new_selection;
        hilite = new_selection;
        win->redraw_later();
        send_client_message(win, detail::SELECTION_CHANGE_MESSAGE);
      }
    }

    void menu_data::clear_selection () {
      set_selection(-1);
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
      while (data[next].is_disabled() && (next != last)) {
        next = rotate<int>(next, delta, sz);
      }
      set_selection(next);
    }

    void menu_data::rotate_hilite (int delta) {
      const int last = get_hilite();
      const int sz = static_cast<int>(size());
      int next = rotate<int>(last, delta, sz);
      while (data[next].is_disabled() && (next != last)) {
        next = rotate<int>(next, delta, sz);
      }
      set_hilite(next);
    }

    int menu_data::get_hilite () const {
      return hilite;
    }

    void menu_data::set_hilite (int sel) {
      int new_hilite = std::max(-1, sel);
      if (new_hilite >= size()) {
        new_hilite = -1;
      }
      if ((new_hilite != -1) && data[new_hilite].is_disabled()) {
        return;
      }
      if (hilite != new_hilite) {
        close();
        hilite = new_hilite;
        win->redraw_later();
        send_client_message(win, detail::HILITE_CHANGE_MESSAGE, true);
      }
    }

    void menu_data::clear_hilite () {
      hilite = -1;
      win->redraw_later();
      send_client_message(win, detail::HILITE_CHANGE_MESSAGE, false);
    }

    void menu_data::set_close_function (close_call fn) {
      close_caller = fn;
    }

    void menu_data::clear_close_function () {
      close_caller = nullptr;
      key_caller = nullptr;
    }

    void menu_data::set_mouse_function (mouse_call fn) {
      mouse_caller = fn;
    }

    void menu_data::clear_mouse_function () {
      mouse_caller = nullptr;
    }

    void menu_data::set_key_function (key_call fn) {
      key_caller = fn;
    }

    bool menu_data::is_open () {
      return (bool)close_caller;
    }

    void menu_data::close () {
      if (close_caller) {
        close_caller();
      }
      selection = -1;
      hilite = -1;
      close_caller = nullptr;
      key_caller = nullptr;
      win->redraw_later();
    }

    void menu_data::handle_mouse (bool b, const core::point& pt) {
      if (mouse_caller) {
        mouse_caller(b, pt);
      }
    }

    bool menu_data::handle_key (os::key_symbol k) {
      if (key_caller) {
        return key_caller(k);
      }
      return false;
    }

    void menu_data::init () {
      selection = -1;
      hilite = -1;
      close_caller = nullptr;
      key_caller = nullptr;
      mouse_caller = nullptr;
    }

    // --------------------------------------------------------------------------
    const window_class main_menu::clazz("main_menu", color::very_light_gray);

    main_menu::main_menu ()
      : data(this)
    {
      data.set_mouse_function(core::bind_method(this, &main_menu::handle_mouse));

      register_event_handler(paint_event(core::bind_method(this, &main_menu::paint)));

      register_event_handler(mouse_move_event([&](os::key_state, const core::point& pt) {
        data.handle_mouse(false, client_to_screen(pt));
      }));

      register_event_handler(mouse_leave_event([&]() {
        data.clear_hilite();
      }));

      register_event_handler(set_focus_event([&](window*) {
        if (data.get_hilite() == -1) {
          data.set_hilite(0);
        }
      }));

      register_event_handler(selection_changed_event([&]() {
        int idx = data.get_selection();
        if (idx > -1) {
          data[idx].select(idx);
        }
      }));

      register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
        take_focus();
        data.handle_mouse(true, client_to_screen(pt));
      }));

      register_event_handler(key_down_event([&](os::key_state,
                                                os::key_symbol key,
                                                const std::string&){
        handle_key(key);
      }));
    }

    void main_menu::handle_mouse (bool btn, const core::point& gpt){
      if (absolute_place().is_inside(gpt)) {
        const auto idx = get_index_at_point(screen_to_client(gpt));
        if (btn) {
          if (idx == data.get_selection()) {
            data.clear_selection();
          } else {
            data.set_selection(idx);
          }
        } else {
          if (data.is_open()) {
            if (idx != -1) {
              data.set_selection(idx);
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
        case keys::left:
        case keys::numpad::left:
          if (data.is_open()) {
            data.rotate_selection(-1);
          } else {
            data.rotate_hilite(-1);
          }
          return true;

        case keys::right:
        case keys::numpad::right:
          if (data.is_open()) {
            data.rotate_selection(1);
          } else {
            data.rotate_hilite(1);
          }
          return true;

        case keys::escape:
          data.close();
          redraw_later();
          return true;

        case keys::enter:
          if (!data.is_open() && (data.get_hilite() > -1)) {
            data.set_selection(data.get_hilite());
          }
          return true;
      }
      return false;
    }

    int main_menu::get_index_at_point (const core::point& pt) const {
      if (client_area().is_inside(pt)) {
        core::point_type pos = 0;
        auto max = data.size();
        for (int i = 0; i < max; ++i) {
          pos += data[i].get_width() + 20;
          if (pt.x() < pos) {
            return i;
          }
        }
      }
      return -1;
    }

    core::point main_menu::sub_menu_position (std::size_t idx) const {
      auto r = absolute_position();
      core::point_type pos = 0;
      auto max = std::min(idx, data.size());
      for (int i = 0; i < max; ++i) {
        pos += data[i].get_width() + 20;
      }
      return (r + core::point(pos, size().height()));
    }

    void main_menu::paint (const draw::graphics& g) {
      auto max = data.size();
      draw::brush background(color::menuColor());
      const core::rectangle area = client_area();
      core::rectangle r = area;
      for (int i = 0; i < max; ++i) {
        auto w = data[i].get_width() + 20;
        r.width(w);
        paint::main_menu_item(data[i], g, r, background,
                              (i == data.get_selection()), (i == data.get_hilite()));
        r.move_x(w);
      }
      if (r.x() < area.x2()) {
        g.fill(draw::rectangle(core::rectangle(r.top_left(), area.bottom_right())), background);
      }
    }

    // --------------------------------------------------------------------------
    popup_menu::popup_menu ()
      : data(this)
      , text_pos(10)
      , hotkey_pos(0)
    {
      register_event_handler(paint_event(core::bind_method(this, &popup_menu::paint)));

      register_event_handler(mouse_move_event([&](os::key_state, const core::point& pt) {
        data.handle_mouse(false, client_to_screen(pt));
      }));

      register_event_handler(mouse_leave_event([&](){
        data.clear_hilite();
      }));

      register_event_handler(selection_changed_event([&]() {
        int idx = data.get_selection();
        if (idx > -1) {
          if (!data[idx].is_sub_menu()) {
            data.handle_mouse(true, core::point::zero);
          }
          data[idx].select(idx);
        }
      }));

      register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
        data.handle_mouse(true, client_to_screen(pt));
      }));

      register_event_handler(lost_focus_event([&](window*) {
        redraw_later();
      }));

      register_event_handler(key_down_event([&](os::key_state,
                                                os::key_symbol key,
                                                const std::string&){
        handle_key(key);
      }));

      register_event_handler(show_event([&]() {
        capture_pointer();
      }));

    }

    bool popup_menu::handle_key (os::key_symbol key) {
      if (data.handle_key(key)) {
        return true;
      }
      switch (key) {
        case keys::left:
        case keys::numpad::left:
        case keys::escape:
          if (data.is_open()) {
            int idx = data.get_hilite();
            data.close();
            data.set_hilite(idx);
            redraw_later();
            return true;
          }
          break;
        case keys::up:
        case keys::numpad::up:
          data.rotate_hilite(-1);
          return true;

        case keys::right:
        case keys::numpad::right: {
          int idx = data.get_hilite();
          if (!data.is_open() && (idx != -1) && data[idx].is_sub_menu()) {
            data.set_selection(idx);
            return true;
          }
          break;
        }
        case keys::down:
        case keys::numpad::down:
          data.rotate_hilite(1);
          return true;

        case keys::enter:
          if (data.get_hilite() > -1) {
            data.set_selection(data.get_hilite());
          }
          return true;
      }
      return false;
    }

    void popup_menu::handle_mouse (bool btn, const core::point& gpt) {
      const auto idx = get_index_at_point(screen_to_client(gpt));
      if (btn) {
        if (!data.is_open() || (idx != data.get_hilite())) {
          data.set_selection(idx);
        }
      } else {
        if ((idx > -1) && data[idx].is_sub_menu()) {
          data.set_selection(idx);
        } else {
          data.set_hilite(idx);
        }
      }
    }

    void popup_menu::popup_at (const core::point& pt, window& parent) {
      data.init();
      data.set_hilite(0);
      data.set_mouse_function([&](bool btn, const core::point& gpt) {
        if (absolute_place().is_inside(gpt)) {
          handle_mouse(btn, gpt);
        } else if (btn) {
          close();
      }
      });
      create(parent, core::rectangle(pt, core::size(calc_width() + 2, static_cast<core::size_type>(data.size() * item_height + 2))));
      set_visible();
      run_modal();
    }

    void popup_menu::popup_at (window& parent, menu_data& parent_data, const core::point& pt) {
      data.init();
      data.set_hilite(0);
      parent_data.set_close_function([&]() {
        close();
        parent_data.clear_close_function();
      });
      parent_data.set_key_function(core::bind_method(this, &popup_menu::handle_key));

      data.set_mouse_function([&](bool btn, const core::point& gpt) {
        if (absolute_place().is_inside(gpt)) {
          handle_mouse(btn, gpt);
        } else {
          parent_data.handle_mouse(btn, gpt);
        }
      });
      create(parent, core::rectangle(pt, core::size(calc_width() + 2, static_cast<core::size_type>(data.size() * item_height + 2))));
      set_visible();
      run_modal();
    }

    core::point popup_menu::sub_menu_position (int idx) {
      auto r = absolute_position();
      return (r + core::point(size().width(), static_cast<core::point_type>(idx * item_height)));
    }

    int popup_menu::get_index_at_point (const core::point& pt) const {
      if (client_area().shrinked({ 1, 1 }).is_inside(pt)) {
        return std::min(static_cast<int>(data.size()), static_cast<int>((pt.y() - 1) / item_height));
      }
      return -1;
    }

    void popup_menu::close () {
      data.close();
      uncapture_pointer();
      set_visible(false);
      destroy();
      end_modal();
    }

    void popup_menu::paint (const draw::graphics& g) {
      draw::brush background(color::menuColor());
      const core::rectangle area = client_area();
      draw::frame::raised_relief(g, area);
      core::rectangle r = area.shrinked({1, 1});
      const auto count = data.size();
      for (int i = 0; i < count; ++i) {
        r.height(static_cast<core::size_type>(item_height));
        paint::menu_item(data[i], g, r, text_pos, hotkey_pos, background,
                         (i == data.get_selection()), (i == data.get_hilite()));
        r.move_y(static_cast<core::size_type>(item_height));
      }
    }

    core::size_type popup_menu::calc_width () {
      core::size_type label_width = 0;
      core::size_type hotkey_width = 0;
      bool has_sub = false;
      const draw::font& f = draw::font::menu();
      const auto count = data.size();
      for (int i = 0; i < count; ++i) {
        const menu_entry& e = data[i];
        label_width = std::max(label_width, e.get_width());
        has_sub |= e.is_sub_menu();
        hotkey_width = std::max(hotkey_width, f.get_text_size(e.get_hotkey()).width());
      }
      text_pos = 36;
      hotkey_pos = text_pos + label_width + 20;
      return hotkey_pos + (hotkey_width ? hotkey_width + 10 : 0) + (has_sub ? 20 : 0);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
