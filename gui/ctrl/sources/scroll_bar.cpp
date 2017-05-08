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
* @brief     C++ API: basic controls
*
* @file
*/


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "scroll_bar.h"


namespace gui {

  namespace win {

#ifdef WIN32
    template<>
    float get_param<0, float>(const core::event& e) {
      return (float)e.wParam;
    }
#endif //WIN32

    scroll_bar::type scroll_bar::get_scroll_bar_width () {
      return scroll_bar::type(17);
    }

    // --------------------------------------------------------------------------
    scroll_bar::scroll_bar ()
      : min(0)
      , max(100)
      , step(10)
      , value(0)
      , state(Nothing_pressed)
    {
#ifdef X11
      detail::init_control_messages();
#endif // X11
    }

    void scroll_bar::create (const window_class& type,
                             const container& parent,
                             const core::rectangle& place) {
      super::create(type, parent, place);
    }

    scroll_bar::type scroll_bar::get_min () const {
      return min;
    }

    scroll_bar::type scroll_bar::get_max () const {
      return max;
    }

    scroll_bar::type scroll_bar::get_step () const {
      return step;
    }

    scroll_bar::type scroll_bar::get_value () const {
      return value;
    }

    scroll_bar::State scroll_bar::get_state () {
      return state;
    }

    void scroll_bar::set_min (type mi) {
      min = mi;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_max (type ma) {
      max = ma;
      value = std::min(value, max);
      redraw_later();
    }

    void scroll_bar::set_min_max (type mi, type ma) {
      min = mi;
      max = ma;
      value = std::max(value, min);
      redraw_later();
    }

    void scroll_bar::set_step (type s) {
      step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      min = mi;
      max = ma;
      step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step_value(type mi, type ma, type s, type v) {
      set_min_max_step(mi, ma, s);
      set_value(v);
    }

    void scroll_bar::set_value (type v) {
      set_value(v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      v = std::min(std::max(v, min), max);
      if (v != value) {
        value = v;
        redraw_later();
        if (notify) {
          send_client_message(this, detail::SCROLLBAR_MESSAGE, static_cast<long>(value));
        }
      }
    }

    void scroll_bar::set_state (State s) {
      state = s;
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void scrollbar (const draw::graphics &g,
                      scroll_bar::State state,
                      bool is_enabled,
                      bool horizontal,
                      const core::rectangle& up,
                      const core::rectangle& down,
                      const core::rectangle& thumb,
                      const core::rectangle& page_up,
                      const core::rectangle& page_down) {
        if (!page_up.empty()) {
          g.fill(draw::rectangle(page_up), state == scroll_bar::Page_up_pressed ? color::light_gray
                                                                                : color::very_light_gray);
        }
        if (!page_down.empty()) {
          g.fill(draw::rectangle(page_down + core::size::one), state == scroll_bar::Page_down_pressed ? color::light_gray
                                                                                    : color::very_light_gray);
        }
        os::color col = is_enabled ? color::black : color::gray;
        if (!up.empty()) {
          g.fill(draw::rectangle(up), color::buttonColor());
          draw::frame::relief(g, up, state == scroll_bar::Up_button_pressed);
          core::rectangle r = up.shrinked({5, 5});
          if (!r.empty()) {
            std::vector<core::point> p;
            if (horizontal) {
              p = { {r.x(), r.center_y()}, r.top_right(), r.bottom_right() };
            } else {
              p = { {r.center_x(), r.y()}, r.bottom_right(), r.bottom_left() };
            }
            g.fill(draw::polygon(p), col);
          }
        }
        if (!down.empty()) {
          g.fill(draw::rectangle(down), color::buttonColor());
          draw::frame::relief(g, down, state == scroll_bar::Down_button_pressed);
          core::rectangle r = down.shrinked({5, 5});
          if (!r.empty()) {
            std::vector<core::point> p;
            if (horizontal) {
              p = { r.top_left(), r.bottom_left(), {r.x2(), r.center_y()} };
            } else {
              p = { r.top_left(), r.top_right(), {r.center_x(), r.y2()} };
            }
            g.fill(draw::polygon(p), col);
          }
        }
        if (!thumb.empty()) {
          g.fill(draw::rectangle(thumb), color::buttonColor());
          draw::frame::raised_relief(g, thumb);
        }
      }
    } // paint

    namespace detail {

      template<>
      scroll_bar_class<orientation::vertical>::scroll_bar_class ()
        : no_erase_window_class("VSCROLLBAR++", color::very_light_gray)
      {}

      template<>
      scroll_bar_class<orientation::horizontal>::scroll_bar_class ()
        : no_erase_window_class("HSCROLLBAR++", color::very_light_gray)
      {}

      // --------------------------------------------------------------------------
      template<>
      scroll_barT<orientation::horizontal>::scroll_barT ()
        : last_position(0) {
        register_event_handler(paint_event([&](const draw::graphics& g){
          auto geo = get_geometry();
          auto up = up_button_place(geo);
          auto down = down_button_place(geo);
          auto thumb = thumb_button_place(geo);
          auto page_up = page_up_place(geo);
          auto page_down = page_down_place(geo);
          paint::scrollbar(g, get_state(), is_enabled(), true, up, down, thumb, page_up, page_down);
        }));
        register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          if (is_enabled()) {
            take_focus();
            last_mouse_point = pt;
            last_position = get_value();

            auto geo = get_geometry();

            if (up_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Up_button_pressed);
            } else if (down_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Down_button_pressed);
            } else if (thumb_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Thumb_button_pressed);
            } else if (page_up_place(geo).is_inside(last_mouse_point)) {
              set_state(Page_up_pressed);
            } else if (page_down_place(geo).is_inside(last_mouse_point)) {
              set_state(Page_down_pressed);
            } else {
              set_state(Nothing_pressed);
            }
            capture_pointer();
            redraw_later();
          }
        }));
        register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pt) {
          if (is_enabled()) {
            auto geo = get_geometry();
            switch (get_state()) {
              case Up_button_pressed:
                if (up_button_place(geo).is_inside(pt)) {
                  set_value(get_value() - 1, true);
                }
                break;
              case Down_button_pressed:
                if (down_button_place(geo).is_inside(pt)) {
                  set_value(get_value() + 1, true);
                }
                break;
              case Page_up_pressed:
                if (page_up_place(geo).is_inside(pt)) {
                  set_value(get_value() - get_step(), true);
                }
                break;
              case Page_down_pressed:
                if (page_down_place(geo).is_inside(pt)) {
                  set_value(get_value() + get_step(), true);
                }
                break;
            }
            set_state(Nothing_pressed);
            uncapture_pointer();
            redraw_later();
          }
        }));
        register_event_handler(wheel_x_event([&](const core::point::type dx, const core::point&){
          if (is_enabled()) {
            set_value(get_value() - dx, true);
          }
        }));
        register_event_handler(mouse_move_event([&](os::key_state keys, const core::point& pt) {
          if (is_enabled() && left_button_bit_mask::is_set(keys)) {
            // check if on thumb
            if (get_state() == Thumb_button_pressed) {
              type delta = (pt.x() - last_mouse_point.x()) / get_scale();
              set_value(last_position + delta, true);
            }
          }
        }));
        register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
          if (is_enabled()) {
            switch (key) {
              case keys::left:
              case keys::numpad::left:
                set_value(get_value() - 1, true);
                break;
              case keys::right:
              case keys::numpad::right:
                set_value(get_value() + 1, true);
                break;
              case keys::page_up:
              case keys::numpad::page_up:
                set_value(get_value() - get_step(), true);
                break;
              case keys::page_down:
              case keys::numpad::page_down:
                set_value(get_value() + get_step(), true);
                break;
              case keys::home:
              case keys::numpad::home:
                set_value(get_min(), true);
                break;
              case keys::end:
              case keys::numpad::end:
                set_value(get_min(), true);
                break;
            }
          }
        }));
      }

      template<>
      scroll_barT<orientation::vertical>::scroll_barT ()
        : last_position(0) {
        register_event_handler(paint_event([&](const draw::graphics& g){
          auto geo = get_geometry();
          auto up = up_button_place(geo);
          auto down = down_button_place(geo);
          auto thumb = thumb_button_place(geo);
          auto page_up = page_up_place(geo);
          auto page_down = page_down_place(geo);
          paint::scrollbar(g, get_state(), is_enabled(), false, up, down, thumb, page_up, page_down);
        }));
        register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          if (is_enabled()) {
            take_focus();
            last_mouse_point = pt;
            last_position = get_value();

            auto geo = get_geometry();

            if (up_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Up_button_pressed);
            } else if (down_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Down_button_pressed);
            } else if (thumb_button_place(geo).is_inside(last_mouse_point)) {
              set_state(Thumb_button_pressed);
            } else if (page_up_place(geo).is_inside(last_mouse_point)) {
              set_state(Page_up_pressed);
            } else if (page_down_place(geo).is_inside(last_mouse_point)) {
              set_state(Page_down_pressed);
            } else {
              set_state(Nothing_pressed);
            }
            if (get_state() != Nothing_pressed) {
              capture_pointer();
            }
            redraw_later();
          }
        }));
        register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pt) {
          if (is_enabled()) {
            auto geo = get_geometry();
            switch (get_state()) {
              case Up_button_pressed:
                if (up_button_place(geo).is_inside(pt)) {
                  set_value(get_value() - 1, true);
                }
                break;
              case Down_button_pressed:
                if (down_button_place(geo).is_inside(pt)) {
                  set_value(get_value() + 1, true);
                }
                break;
              case Page_up_pressed:
                if (page_up_place(geo).is_inside(pt)) {
                  set_value(get_value() - get_step(), true);
                }
                break;
              case Page_down_pressed:
                if (page_down_place(geo).is_inside(pt)) {
                  set_value(get_value() + get_step(), true);
                }
                break;
            }
            if (get_state() != Nothing_pressed) {
              set_state(Nothing_pressed);
              uncapture_pointer();
            }
            redraw_later();
          }
        }));
        register_event_handler(wheel_y_event([&](const core::point::type dy, const core::point&){
          if (is_enabled()) {
            set_value(get_value() - dy, true);
          }
        }));
        register_event_handler(mouse_move_event([&](os::key_state keys, const core::point& pt) {
          if (is_enabled() && left_button_bit_mask::is_set(keys)) {
            // check if on thumb
            if (get_state() == Thumb_button_pressed) {
              type delta = (pt.y() - last_mouse_point.y()) / get_scale();
              set_value(last_position + delta, true);
            }
          }
        }));
        register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
          if (is_enabled()) {
            switch (key) {
              case keys::up:
              case keys::numpad::up:
                set_value(get_value() - 1, true);
                break;
              case keys::down:
              case keys::numpad::down:
                set_value(get_value() + 1, true);
                break;
              case keys::page_up:
              case keys::numpad::page_up:
                set_value(get_value() - get_step(), true);
                break;
              case keys::page_down:
              case keys::numpad::page_down:
                set_value(get_value() + get_step(), true);
                break;
              case keys::home:
              case keys::numpad::home:
                set_value(get_min(), true);
                break;
              case keys::end:
              case keys::numpad::end:
                set_value(get_min(), true);
                break;
            }
          }
        }));
      }

    }

  } // win

} // gui
