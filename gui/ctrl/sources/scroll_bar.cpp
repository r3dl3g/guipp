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
      return (float)e.param_1;
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
      if (!detail::SCROLLBAR_MESSAGE) {
        detail::SCROLLBAR_MESSAGE = XInternAtom(core::global::get_instance(), "SCROLLBAR_MESSAGE", False);
      }
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
      void scrollbar (const draw::graphics &g,
                      scroll_bar::State state,
                      bool is_enabled,
                      const std::string& up_char,
                      const std::string& down_char,
                      const core::rectangle& up,
                      const core::rectangle& down,
                      const core::rectangle& thumb,
                      const core::rectangle& page_up,
                      const core::rectangle& page_down) {
        if (!page_up.empty()) {
          g.fill(draw::rectangle(page_up), state == scroll_bar::Page_up_pressed ? draw::color::lightGray()
                                                                                : draw::color::veryLightGray());
        }
        if (!page_down.empty()) {
          g.fill(draw::rectangle(page_down), state == scroll_bar::Page_down_pressed ? draw::color::lightGray()
                                                                                    : draw::color::veryLightGray());
        }
        if (!up.empty()) {
          g.fill(draw::rectangle(up), draw::color::buttonColor());
          draw::frame::relief(g, up, state == scroll_bar::Up_button_pressed);
          g.text(draw::text_box(up_char, up, draw::center),
                 draw::font::system(), is_enabled ? draw::color::black()
                                                  : draw::color::gray());
        }
        if (!down.empty()) {
          g.fill(draw::rectangle(down), draw::color::buttonColor());
          draw::frame::relief(g, down, state == scroll_bar::Down_button_pressed);
          g.text(draw::text_box(down_char, down, draw::center),
                 draw::font::system(), is_enabled ? draw::color::black()
                                                  : draw::color::gray());
        }
        if (!thumb.empty()) {
          g.fill(draw::rectangle(thumb), draw::color::buttonColor());
          draw::frame::raised_relief(g, thumb);
        }
      }
    } // paint

    namespace detail {

      template<>
      scroll_bar_class<false>::scroll_bar_class ()
        : window_class(custom_class("VSCROLLBAR++", 
#ifdef WIN32
          0,
          WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
          WS_EX_NOPARENTNOTIFY
#endif //WIN32
#ifdef X11
          draw::color::veryLightGray()
#endif // X11
        ))
      {}

      template<>
      scroll_bar_class<true>::scroll_bar_class ()
        : window_class(custom_class("HSCROLLBAR++", 
#ifdef WIN32
          0,
          WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
          WS_EX_NOPARENTNOTIFY
#endif //WIN32
#ifdef X11
          draw::color::veryLightGray()
#endif // X11
        ))
      {}

      template<>
      void scroll_bar_class<false>::prepare (window* win) const {
        window_class::prepare(win);
#ifdef X11
        unsigned long mask = CWBackPixmap;
        XSetWindowAttributes wa;
        wa.background_pixmap = None;
        XChangeWindowAttributes(core::global::get_instance(), win->get_id(), mask, &wa);
#endif // X11
      }

      template<>
      void scroll_bar_class<true>::prepare (window* win) const {
        window_class::prepare(win);
#ifdef X11
        unsigned long mask = CWBackPixmap;
        XSetWindowAttributes wa;
        wa.background_pixmap = None;
        XChangeWindowAttributes(core::global::get_instance(), win->get_id(), mask, &wa);
#endif // X11
      }

      // --------------------------------------------------------------------------
      const std::string left_char = "<";
      const std::string right_char = ">";
      const std::string up_char = u8"\u2227";
      const std::string down_char = u8"\u2228";

      // --------------------------------------------------------------------------
      template<>
      scroll_barT<true>::scroll_barT ()
        : last_position(0) {
        register_event_handler(paint_event([&](const draw::graphics& g){
          auto geo = get_geometry();
          auto up = up_button_place(geo);
          auto down = down_button_place(geo);
          auto thumb = thumb_button_place(geo);
          auto page_up = page_up_place(geo);
          auto page_down = page_down_place(geo);
          paint::scrollbar(g, get_state(), is_enabled(), left_char, right_char, up, down, thumb, page_up, page_down);
        }));
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
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
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pt) {
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
        }));
        register_event_handler(wheel_x_event([&](const core::point::type dx, const core::point&){
          set_value(get_value() - dx, true);
        }));
        register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
          if (left_button_bit_mask::is_set(keys)) {
            // check if on thumb
            if (get_state() == Thumb_button_pressed) {
              type delta = (pt.x() - last_mouse_point.x()) / get_scale();
              set_value(last_position + delta, true);
            }
          }
        }));
        register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
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
        }));
      }

      template<>
      scroll_barT<false>::scroll_barT ()
        : last_position(0) {
        register_event_handler(paint_event([&](const draw::graphics& g){
          auto geo = get_geometry();
          auto up = up_button_place(geo);
          auto down = down_button_place(geo);
          auto thumb = thumb_button_place(geo);
          auto page_up = page_up_place(geo);
          auto page_down = page_down_place(geo);
          paint::scrollbar(g, get_state(), is_enabled(), up_char, down_char, up, down, thumb, page_up, page_down);
        }));
        register_event_handler(left_btn_down_event([&](const core::point& pt) {
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
        }));
        register_event_handler(left_btn_up_event([&](const core::point& pt) {
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
        }));
        register_event_handler(wheel_y_event([&](const core::point::type dy, const core::point&){
          set_value(get_value() - dy, true);
        }));
        register_event_handler(mouse_move_event([&](unsigned int keys, const core::point& pt) {
          if (left_button_bit_mask::is_set(keys)) {
            // check if on thumb
            if (get_state() == Thumb_button_pressed) {
              type delta = (pt.y() - last_mouse_point.y()) / get_scale();
              set_value(last_position + delta, true);
            }
          }
        }));
        register_event_handler(key_up_event([&](os::key_state, os::key_symbol key){
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
        }));
      }

    }

  } // win

} // gui
