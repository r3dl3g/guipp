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
        g.text(draw::text_box(e.get_label(), r2, draw::vcenter_left), draw::font::system(), s ? color::white : color::dark_gray);
      }

      void menu_item (const menu_entry& e,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      const draw::brush& b,
                      bool s,
                      bool h) {
        main_menu_item(e, g, r, b, s, h);
        if (e.is_sub_menu()) {
          core::point_type y = r.center_y();
          core::point_type x = r.x2() - 8;
          g.fill(draw::polygon({ core::point(x, y - 4), core::point(x, y + 4), core::point(x + 4, y)}), color::black);
        }
      }

    }

    main_menu::main_menu () {
      register_event_handler(selection_changed_event([&]() {
        int idx = get_selection();
        if (idx > -1) {
          data[idx].hilite(idx);
        }
      }));
    }

    core::point main_menu::sub_menu_position (int idx) {
      auto r = absolute_position();
      return (r + core::point(idx * item_size, size().height()));
    }

    popup_menu::popup_menu ()
      : items(false) {

      items.register_event_handler(mouse_move_event([&](os::key_state, const core::point& p) {
        if (call_check_selection) {
          call_check_selection(items.window_to_screen(p));
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

      items.register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
        if (!items.place().is_inside(pt) && call_close_function) {
          call_close_function();
        }
      }));

      items.register_event_handler(hilite_changed_event([&](bool on) {
        if (on) {
          int idx = items.get_hilite();
          if (idx > -1) {
            menu_entry& e = data[idx];
            e.hilite(idx);
          }
        }
      }));

      register_event_handler(create_event([&](win::window* w, const core::rectangle& r) {
        items.create(*this, core::rectangle(core::size(calc_width(), data.size() * item_height)), data);
      }));

      register_event_handler(show_event([&]() {
        items.set_visible();
        items.capture_pointer();
      }));

    }

    core::point popup_menu::sub_menu_position (int idx) {
      auto r = absolute_position();
      return (r + core::point(size().width(), idx * item_height));
    }

    void popup_menu::popup_at (const core::point& pt) {
      items.clear_selection();
      items.clear_hilite();
      items.set_drawer(data);
      items.set_count(data.size());

      create(core::rectangle(pt, core::size(calc_width(), data.size() * item_height)));
      set_visible();
      run_modal();
    }

    void popup_menu::close () {
      items.uncapture_pointer();
      items.destroy();
      destroy();
      end_modal();
    }

    core::size_type popup_menu::calc_width () {
      core::size_type w = 20;
      const draw::font& f = draw::font::system();
      for (const menu_entry& e : data) {
        w = std::max(w, f.get_text_size(e.get_label()).width() + 40 + (e.is_sub_menu() ? 20 : 0));
      }
      return w;
    }

  } // win

} // gui
