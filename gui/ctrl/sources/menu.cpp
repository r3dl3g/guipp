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
          g.fill(draw::rectangle(r), color::dark_gray);
        } else if (h) {
          g.fill(draw::rectangle(r), color::very_light_gray);
        } else {
          g.fill(draw::rectangle(r), b);
        }
        core::rectangle r2 = r + core::point(10, 0);
        g.text(draw::text_box(e.get_label(), r2, draw::vcenter_left), draw::font::menu(), s ? color::white : color::dark_gray);
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
          g.fill(draw::rectangle(r), color::dark_gray);
        } else if (h) {
          g.fill(draw::rectangle(r), color::very_light_gray);
        } else {
          g.fill(draw::rectangle(r), b);
        }

        if (!e.get_icon().empty()) {
          g.text(draw::text_box(e.get_icon(), r.with_width(text_pos - 6), draw::center), draw::font::menu(), s ? color::white : color::dark_gray);
        }
        core::rectangle r2 = r + core::point(text_pos, 0);
        if (e.has_separator()) {
          core::rectangle r3 = r2.with_height(2);
          r3.move_x(-2);
          draw::frame::hgroove(g, r3);
        }

        g.text(draw::text_box(e.get_label(), r2, draw::vcenter_left), draw::font::menu(), s ? color::white : color::dark_gray);

        if (!e.get_hotkey().empty()) {
          r2.x(hotkey_pos);
          g.text(draw::text_box(e.get_hotkey(), r2, draw::vcenter_left), draw::font::menu(), s ? color::white : color::dark_gray);
        }
        if (e.is_sub_menu()) {
          core::point_type y = r.center_y();
          core::point_type x = r.x2() - 8;
          g.fill(draw::polygon({ core::point(x, y - 4), core::point(x, y + 4), core::point(x + 4, y)}), color::black);
        }
      }

    }

    // --------------------------------------------------------------------------
    const window_class main_menu::clazz("main_menu", color::light_gray);

    main_menu::main_menu ()
      : selection(-1)
      , hilite(-1)
      , is_open(false)
    {
      register_event_handler(paint_event([&](const draw::graphics& g) {
        auto max = std::min(data.size(), widths.size());
        draw::brush background(color::light_gray);
        const core::rectangle area = client_area();
        core::rectangle r = area;
        for (int i = 0; i < max; ++i) {
          auto w = widths[i];
          r.width(w);
          paint::main_menu_item(data[i], g, r, background, (i == selection), (i == hilite));
          r.move_x(w);
        }
        if (r.x() < area.x2()) {
          g.fill(draw::rectangle(core::rectangle(r.top_left(), area.bottom_right())), background);
        }
      }));

      register_event_handler(mouse_move_event([&](os::key_state state, const core::point& pt) {
        auto new_hilite = get_index_at_point(pt);
        if (hilite != new_hilite) {
          hilite = new_hilite;
          redraw_later();
        }
      }));

      register_event_handler(mouse_leave_event([&](){
        hilite = -1;
        if (!is_open) {
          clear_selection();
        }
      }));

      register_event_handler(selection_changed_event([&]() {
        int idx = get_selection();
        if (idx > -1) {
          is_open = true;
          data[selection].select(selection);
          is_open = false;
        }
      }));

      register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
        set_selection(get_index_at_point(pt));
      }));
    }

    int main_menu::get_index_at_point (const core::point& pt) const {
      if (client_area().is_inside(pt)) {
        core::point_type pos = 0;
        auto max = widths.size();
        for (int i = 0; i < max; ++i) {
          pos += widths[i];
          if (pt.x() < pos) {
            return i;
          }
        }
      }
      return -1;
    }

    int main_menu::get_selection () const {
      return selection;
    }

    void main_menu::set_selection (int i) {
      if ((i > -2) && (i < data.size()) && (i != selection)) {
        selection = i;
        redraw_later();
        send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      }
    }

    void main_menu::clear_selection () {
      hilite = -1;
      if (selection != -1) {
        selection = -1;
        redraw_later();
        send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      }
    }

    core::point main_menu::sub_menu_position (std::size_t idx) const {
      auto r = absolute_position();
      core::point_type pos = 0;
      auto max = std::min(idx, widths.size());
      for (int i = 0; i < max; ++i) {
        pos += widths[i];
      }
      return (r + core::point(pos, size().height()));
    }

    void main_menu::add_entries (const std::initializer_list<menu_entry>& menu_entries) {
      data.insert(data.end(), menu_entries);
      const draw::font& f = draw::font::menu();
      for (const menu_entry& e : menu_entries) {
        widths.push_back(f.get_text_size(e.get_label()).width() + 20);
      }
    }

    void main_menu::add_entry (const menu_entry& entry) {
      data.push_back(entry);
      const draw::font& f = draw::font::menu();
      widths.push_back(f.get_text_size(entry.get_label()).width() + 20);
    }

    // --------------------------------------------------------------------------
    popup_menu::popup_menu ()
      : items(false) 
      , text_pos(10)
      , hotkey_pos(0)
    {

      items.register_event_handler(mouse_move_event([&](os::key_state state, const core::point& pt) {
        if (call_check_selection) {
          call_check_selection(0, pt);
        }
      }));

      items.register_event_handler(selection_changed_event([&]() {
        int idx = items.get_selection();
        if (idx > -1) {
          menu_entry& e = data[idx];
          if (!e.is_sub_menu()) {
            if (call_close_function) {
              call_close_function();
            }
            e.select(idx);
          }
        }
      }));

      items.register_event_handler(left_btn_down_event([&](os::key_state state, const core::point& pt) {
        if (call_check_selection) {
          call_check_selection(1, pt);
        }
      }));

      items.register_event_handler(hilite_changed_event([&](bool on) {
        if (on) {
          int idx = items.get_hilite();
          if (idx > -1) {
            data[idx].hilite(idx);
          }
        }
      }));

      register_event_handler(create_event([&](win::window* w, const core::rectangle& r) {
        items.create(*this, core::rectangle(r.size()));
        items.set_drawer(core::bind_method(this, &popup_menu::draw_menu_item));
        items.set_count(data.size());
      }));

      register_event_handler(show_event([&]() {
        items.set_visible();
        items.capture_pointer();
      }));

    }

    core::point popup_menu::sub_menu_position (int idx) {
      auto r = absolute_position();
      return (r + core::point(size().width() - 1, static_cast<core::point_type>(idx * item_height + 1)));
    }

    void popup_menu::popup_at (const core::point& pt) {
      items.clear_selection();
      items.clear_hilite();
      create(core::rectangle(pt, core::size(calc_width(), static_cast<core::size_type>(data.size() * item_height))));
      set_visible();
      run_modal();
    }

    void popup_menu::close () {
      items.uncapture_pointer();
      set_visible(false);
      items.destroy();
      destroy();
      end_modal();
    }

    core::size_type popup_menu::calc_width () {
      core::size_type label_width = 0;
      core::size_type hotkey_width = 0;
      bool has_sub = false;
      const draw::font& f = draw::font::menu();
      for (const menu_entry& e : data) {
        label_width = std::max(label_width, f.get_text_size(e.get_label()).width());
        has_sub |= e.is_sub_menu();
        hotkey_width = std::max(hotkey_width, f.get_text_size(e.get_hotkey()).width());
      }
      text_pos = 36;
      hotkey_pos = text_pos + label_width + 20;
      return hotkey_pos + (hotkey_width ? hotkey_width + 10 : 0) + (has_sub ? 20 : 0);
    }

    void popup_menu::draw_menu_item (int idx,
                                     const draw::graphics& g,
                                     const core::rectangle& r,
                                     const draw::brush& b,
                                     bool s,
                                     bool h) {
      paint::menu_item(data.at(idx), g, r, text_pos, hotkey_pos, b, s, h);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
