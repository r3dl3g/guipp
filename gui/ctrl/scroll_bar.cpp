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
// Library includes
//
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/button.h>


namespace gui {

  namespace win {

#ifdef WIN32
    template<>
    float get_param<0, float>(const core::event& e) {
      return (float)e.wParam;
    }

#endif //WIN32

    scroll_bar_data::scroll_bar_data ()
      : state(scrollbar_state::nothing)
      , min(0)
      , max(100)
      , step(1)
      , value(0)
      , last_value(0)
    {}

    // --------------------------------------------------------------------------
    scroll_bar::scroll_bar () {
      init();
    }

    scroll_bar::scroll_bar (const scroll_bar& rhs)
      : super(rhs)
      , data(rhs.data)
    {
      init();
    }

    scroll_bar::scroll_bar (scroll_bar&& rhs)
      : super(std::move(rhs))
      , data(std::move(rhs.data))
    {
      init();
    }

    void scroll_bar::init () {
#ifdef X11
      static int initialized = detail::init_control_messages();
#endif // X11
      register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*) {
        redraw_later();
      }));

      set_accept_focus(true);
    }

    void scroll_bar::create (const class_info& type,
                             const container& parent,
                             const core::rectangle& place) {
      super::create(type, parent, place);
    }

    scroll_bar::type scroll_bar::get_min () const {
      return data.min;
    }

    scroll_bar::type scroll_bar::get_max () const {
      return data.max;
    }

    scroll_bar::type scroll_bar::get_step () const {
      return data.step;
    }

    scroll_bar::type scroll_bar::get_value () const {
      return data.value;
    }

    scrollbar_state scroll_bar::get_state () {
      return data.state;
    }

    void scroll_bar::set_min (type mi) {
      data.min = mi;
      data.value = std::max(data.value, data.min);
      redraw_later();
    }

    void scroll_bar::set_max (type ma) {
      data.max = ma;
      data.value = std::min(data.value, data.max);
      redraw_later();
    }

    void scroll_bar::set_min_max (type mi, type ma) {
      data.min = mi;
      data.max = ma;
      data.value = std::max(data.value, data.min);
      redraw_later();
    }

    void scroll_bar::set_step (type s) {
      data.step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      data.min = mi;
      data.max = ma;
      data.step = s;
      redraw_later();
    }

    void scroll_bar::set_min_max_step_value (type mi, type ma, type s, type v) {
      set_min_max_step(mi, ma, s);
      set_value(v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      v = std::min(std::max(v, data.min), data.max);
      if (v != data.value) {
        data.value = v;
        redraw_later();
        if (notify) {
          send_notify();
        }
      }
    }

    void scroll_bar::set_state (scrollbar_state s) {
      data.state = s;
    }

    scroll_bar::type scroll_bar::get_last_value () const {
      return data.last_value;
    }

    void scroll_bar::set_last_value (type last_value) {
      data.last_value = last_value;
    }

    core::point scroll_bar::get_last_mouse_point () const {
      return data.last_mouse_point;
    }

    void scroll_bar::set_last_mouse_point (core::point last_mouse_point) {
      data.last_mouse_point = last_mouse_point;
    }

    void scroll_bar::send_notify () {
      send_client_message(this, detail::SCROLLBAR_MESSAGE, static_cast<long>(data.value));
    }

    void scroll_bar::handle_wheel (const core::point_type delta, const core::point&) {
      if (is_enabled()) {
        set_value(get_value() - delta * get_step(), true);
      }
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void scrollbar (const draw::graphics &g,
                      scrollbar_state state,
                      bool is_enabled,
                      bool horizontal,
                      bool has_focus,
                      const core::rectangle& up,
                      const core::rectangle& down,
                      const core::rectangle& thumb,
                      const core::rectangle& page_up,
                      const core::rectangle& page_down) {
        if (!page_up.empty()) {
          g.fill(draw::rectangle(page_up),
                 state == scrollbar_state::page_up ? color::light_gray
                 : color::very_light_gray);
        }
        if (!page_down.empty()) {
          g.fill(draw::rectangle(page_down + core::size::one),
                 state == scrollbar_state::page_down ? color::light_gray
                 : color::very_light_gray);
        }
        os::color col = is_enabled ? color::black : color::gray;
        if (!up.empty()) {
//          paint::button_frame(g, up, button_state(state == scrollbar_state::up_button), false, true);
          g.fill(draw::rectangle(up), color::buttonColor());
          draw::frame::relief(g, up, state == scrollbar_state::up_button);
          core::rectangle r = up.shrinked({5, 5});
          if (!r.empty()) {
            std::vector<core::point> p;
            if (horizontal) {
              p = {{r.x(), r.center_y()}, r.top_right(), r.bottom_right()};
            } else {
              p = {{r.center_x(), r.y()}, r.bottom_right(), r.bottom_left()};
            }
            g.fill(draw::polygon(p), col);
          }
        }
        if (!down.empty()) {
//          paint::button_frame(g, down, button_state(state == scrollbar_state::down_button), false, true);
          g.fill(draw::rectangle(down), color::buttonColor());
          draw::frame::relief(g, down, state == scrollbar_state::down_button);
          core::rectangle r = down.shrinked({5, 5});
          if (!r.empty()) {
            std::vector<core::point> p;
            if (horizontal) {
              p = {r.top_left(), r.bottom_left(), {r.x2(), r.center_y()}};
            } else {
              p = {r.top_left(), r.top_right(), {r.center_x(), r.y2()}};
            }
            g.fill(draw::polygon(p), col);
          }
        }
        if (!thumb.empty()) {
//          paint::button_frame(g, thumb, button_state(state == scrollbar_state::thumb_button), false, true);
          g.fill(draw::rectangle(thumb), color::buttonColor());
          draw::frame::raised_relief(g, thumb);
          if (scrollbar_state::thumb_button == state) {
            draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
          }
          if (has_focus) {
            draw::frame::dots(g, thumb);
          }
        }
      }

    } // paint

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation::horizontal>::init () {
      register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_scroll_bar::handle_paint));
      register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &basic_scroll_bar::handle_left_btn_down));
      register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &basic_scroll_bar::handle_left_btn_up));
      register_event_handler(REGISTER_FUNCTION, wheel_x_event((super*)this, &scroll_bar::handle_wheel));
      register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &basic_scroll_bar::handle_mouse_move));
      register_event_handler(REGISTER_FUNCTION, any_key_up_event(this, &basic_scroll_bar::handle_any_key_up));
    }

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_paint (const draw::graphics& g) {
      auto geo = get_geometry();
      paint::scrollbar(g, get_state(), is_enabled(), true, has_focus(),
                       up_button_place(geo), down_button_place(geo),
                       thumb_button_place(geo), page_up_place(geo), page_down_place(geo));
    }

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (is_enabled() && left_button_bit_mask::is_set(keys)) {
        // check if on thumb
        if (get_state() == scrollbar_state::thumb_button) {
          type delta = (pt.x() - get_last_mouse_point().x()) / get_scale();
          set_value(get_last_value() + delta, true);
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation::horizontal>::handle_any_key_up (os::key_state, os::key_symbol key) {
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
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation::vertical>::init () {
      register_event_handler(REGISTER_FUNCTION, paint_event(this, &basic_scroll_bar::handle_paint));
      register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &basic_scroll_bar::handle_left_btn_down));
      register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &basic_scroll_bar::handle_left_btn_up));
      register_event_handler(REGISTER_FUNCTION, wheel_y_event((super*)this, &scroll_bar::handle_wheel));
      register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &basic_scroll_bar::handle_mouse_move));
      register_event_handler(REGISTER_FUNCTION, any_key_up_event(this, &basic_scroll_bar::handle_any_key_up));
    }

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_paint (const draw::graphics& g) {
      auto geo = get_geometry();
      paint::scrollbar(g, get_state(), is_enabled(), false, has_focus(),
                       up_button_place(geo), down_button_place(geo),
                       thumb_button_place(geo), page_up_place(geo), page_down_place(geo));
    }

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (is_enabled() && left_button_bit_mask::is_set(keys)) {
        // check if on thumb
        if (get_state() == scrollbar_state::thumb_button) {
          type delta = (pt.y() - get_last_mouse_point().y()) / get_scale();
          set_value(get_last_value() + delta, true);
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation::vertical>::handle_any_key_up (os::key_state, os::key_symbol key) {
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
    }

  } // win

} // gui
