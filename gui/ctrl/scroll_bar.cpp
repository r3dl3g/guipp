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
#include <util/string_util.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/button.h>


namespace gui {

#ifdef GUIPP_WIN
  namespace win {

    template<>
    float get_param<0, float>(const core::event& e) {
      return (float)e.wParam;
    }

  } // namespace win

#endif //GUIPP_WIN

  namespace ctrl {

#ifdef GUIPP_QT

    core::point::type get_scroll_value (const core::event& e) {
      return e.cast<win::QClientEvent>().l1();
    }

#endif // GUIPP_QT

    scroll_bar_data::scroll_bar_data ()
      : state(scrollbar_state::nothing)
      , hilite(scrollbar_state::nothing)
      , min(0)
      , max(100)
      , step(1)
      , page(10)
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
#ifdef GUIPP_X11
      static int initialized = detail::init_control_messages();
      (void)initialized;
#endif // GUIPP_X11
      on_lost_focus([&] () {
        invalidate();
      });
      on_mouse_leave([&] () {
        set_hilite(scrollbar_state::nothing);
        invalidate();
      });

      set_accept_focus(true);
    }

    void scroll_bar::create (const win::class_info& type,
                             win::container& parent,
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

    scroll_bar::type scroll_bar::get_page () const {
      return data.page;
    }

    scroll_bar::type scroll_bar::get_value () const {
      return data.value;
    }

    scrollbar_state scroll_bar::get_state () const {
      return data.state;
    }

    scrollbar_state scroll_bar::get_hilite () const {
      return data.hilite;
    }

    void scroll_bar::set_min (type mi) {
      if (data.min != mi) {
        data.min = mi;
        data.value = std::max(data.value, data.min);
        invalidate();
      }
    }

    void scroll_bar::set_max (type ma) {
      if (data.max != ma) {
        data.max = ma;
        data.value = std::min(data.value, data.max);
        invalidate();
      }
    }

    void scroll_bar::set_min_max (type mi, type ma) {
      if ((data.min != mi) || (data.max != ma)) {
        data.min = mi;
        data.max = ma;
        data.value = std::max(std::min(data.value, data.max), data.min);
        invalidate();
      }
    }

    void scroll_bar::set_step (type s) {
      data.step = s;
      data.page = std::max(s, data.page);
    }

    void scroll_bar::set_page (type p) {
      data.page = p;
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      set_step(s);
      set_min_max(mi, ma);
    }

    void scroll_bar::set_min_max_step_page (type mi, type ma, type s, type p) {
      set_page(p);
      set_min_max_step(mi, ma, s);
    }

    void scroll_bar::set_min_max_step_value (type mi, type ma, type s, type v) {
      set_min_max_step(mi, ma, s);
      set_value(v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      v = std::min(std::max(v, data.min), data.max);
      if (v != data.value) {
        data.value = v;
        invalidate();
        if (notify) {
          send_notify();
        }
      }
    }

    void scroll_bar::set_state (scrollbar_state s) {
      data.state = s;
    }

    void scroll_bar::set_hilite (scrollbar_state s) {
      data.hilite = s;
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
      super::notify_event(detail::SCROLLBAR_MESSAGE, static_cast<long>(data.value));
    }

    void scroll_bar::handle_wheel (const core::point::type delta, const core::point&) {
      if (is_enabled()) {
        clog::trace() << "scroll_bar::handle_wheel(" << delta << ") step: " << get_step();
        set_value(get_value() - delta * get_step(), true);
      }
    }

    void scroll_bar::on_scroll (scroll_event::function&& f) {
      on<scroll_event>(std::move(f));
    }

    namespace paint {

      namespace {
        std::string up_left_arrows[] = {
          "\xe2\x96\xb4", // up
          "\xe2\x97\x82", // left
          };
        std::string down_right_arrows[] = {
          "\xe2\x96\xbe",  // down
          "\xe2\x96\xb8", // right
        };
      }

      // --------------------------------------------------------------------------
      void scrollbar (const draw::graphics &g,
                      scrollbar_state state,
                      scrollbar_state hilite,
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
          paint::simple_frame(g, up, scrollbar_state::up_button == hilite);
          if (scrollbar_state::up_button == state) {
            draw::frame::sunken_relief(g, up.shrinked(core::size::two));
          }
          auto s = up_left_arrows[horizontal];
          g.text(draw::text_box(s, up, text_origin_t::center), draw::font::system(), col);
        }
        if (!down.empty()) {
          paint::simple_frame(g, down, scrollbar_state::down_button == hilite);
          if (scrollbar_state::down_button == state) {
            draw::frame::sunken_relief(g, down.shrinked(core::size::two));
          }
          auto s = down_right_arrows[horizontal];
          g.text(draw::text_box(s, down, text_origin_t::center), draw::font::system(), col);
        }
        if (!thumb.empty()) {
          paint::simple_frame(g, thumb, scrollbar_state::thumb_button == hilite, 3, horizontal ? 3 : 13);
          if (scrollbar_state::thumb_button == state) {
            draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
          }
          if (has_focus) {
            core::rectangle area = thumb;
            area.shrink({2, 2});
            g.frame(draw::rectangle(area), draw::pen(color::very_light_blue, 1, draw::pen::Style::solid));

          }
        }
      }

      void scrollbar_w95 (const draw::graphics &g,
                          scrollbar_state state,
                          scrollbar_state hilite,
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
          paint::button_frame_w95(g, up, true, false, scrollbar_state::up_button == hilite, false);
          if (scrollbar_state::up_button == state) {
            draw::frame::sunken_relief(g, up.shrinked(core::size::two));
          }
          auto s = up_left_arrows[horizontal];
          g.text(draw::text_box(s, up, text_origin_t::center), draw::font::system(), col);
        }
        if (!down.empty()) {
          paint::button_frame_w95(g, down, true, false, scrollbar_state::down_button == hilite, false);
          if (scrollbar_state::down_button == state) {
            draw::frame::sunken_relief(g, down.shrinked(core::size::two));
          }
          auto s = down_right_arrows[horizontal];
          g.text(draw::text_box(s, down, text_origin_t::center), draw::font::system(), col);
        }
        if (!thumb.empty()) {
          paint::button_frame_w95(g, thumb, true, false, scrollbar_state::thumb_button == hilite, false);
          if (scrollbar_state::thumb_button == state) {
            draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
          }
        }
      }

    } // paint

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::init () {
      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& r) {
        if (!left_btn_down_event::if_match_call(e, this, &basic_scroll_bar::handle_left_btn_down)) {
          if (!left_btn_up_event::if_match_call(e, this, &basic_scroll_bar::handle_left_btn_up)) {
            if (!wheel_x_event::if_match_call(e, this, &scroll_bar::handle_wheel)) {
              if (!mouse_move_event::if_match_call(e, this, &basic_scroll_bar::handle_mouse_move)) {
                any_key_up_event::if_match_call(e, this, &basic_scroll_bar::handle_any_key_up);
              }
            }
          }
        }
        return false;
      }),
      static_cast<os::event_id>(left_btn_down_event::mask | left_btn_up_event::mask | wheel_x_event::mask | mouse_move_event::mask | any_key_up_event::mask));
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::init () {
      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& r) {
        if (!left_btn_down_event::if_match_call(e, this, &basic_scroll_bar::handle_left_btn_down)) {
          if (!left_btn_up_event::if_match_call(e, this, &basic_scroll_bar::handle_left_btn_up)) {
            if (!wheel_y_event::if_match_call(e, this, &scroll_bar::handle_wheel)) {
              if (!mouse_move_event::if_match_call(e, this, &basic_scroll_bar::handle_mouse_move)) {
                any_key_up_event::if_match_call(e, this, &basic_scroll_bar::handle_any_key_up);
              }
            }
          }
        }
        return false;
      }),
      static_cast<os::event_id>(left_btn_down_event::mask | left_btn_up_event::mask | wheel_y_event::mask | mouse_move_event::mask | any_key_up_event::mask));
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (is_enabled()) {
        if (win::left_button_bit_mask::is_set(keys)) {
          // check if on thumb
          if (get_state() == scrollbar_state::thumb_button) {
            type delta = (pt.x() - get_last_mouse_point().x()) / get_scale();
            set_value(get_last_value() + delta, true);
          }
        } else {
          auto geo = get_geometry();
          if (up_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::up_button);
          } else if (down_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::down_button);
          } else if (thumb_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::thumb_button);
          } else if (page_up_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::page_up);
          } else if (page_down_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::page_down);
          } else {
            set_hilite(scrollbar_state::nothing);
          }
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      if (is_enabled()) {
        if (win::left_button_bit_mask::is_set(keys)) {
          // check if on thumb
          if (get_state() == scrollbar_state::thumb_button) {
            type delta = (pt.y() - get_last_mouse_point().y()) / get_scale();
            set_value(get_last_value() + delta, true);
          }
        } else {
          auto geo = get_geometry();
          if (up_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::up_button);
          } else if (down_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::down_button);
          } else if (thumb_button_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::thumb_button);
          } else if (page_up_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::page_up);
          } else if (page_down_place(geo).is_inside(pt)) {
            set_hilite(scrollbar_state::page_down);
          } else {
            set_hilite(scrollbar_state::nothing);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::handle_any_key_up (os::key_state, os::key_symbol key) {
      if (is_enabled()) {
        switch (key) {
        case win::keys::left:
        case win::keys::numpad::left:
          set_value(get_value() - get_step(), true);
          break;
        case win::keys::right:
        case win::keys::numpad::right:
          set_value(get_value() + get_step(), true);
          break;
        case win::keys::page_up:
        case win::keys::numpad::page_up:
          set_value(get_value() - get_page(), true);
          break;
        case win::keys::page_down:
        case win::keys::numpad::page_down:
          set_value(get_value() + get_page(), true);
          break;
        case win::keys::home:
        case win::keys::numpad::home:
          set_value(get_min(), true);
          break;
        case win::keys::end:
        case win::keys::numpad::end:
          set_value(get_min(), true);
          break;
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::handle_any_key_up (os::key_state, os::key_symbol key) {
      if (is_enabled()) {
        switch (key) {
        case win::keys::up:
        case win::keys::numpad::up:
          set_value(get_value() - get_step(), true);
          break;
        case win::keys::down:
        case win::keys::numpad::down:
          set_value(get_value() + get_step(), true);
          break;
        case win::keys::page_up:
        case win::keys::numpad::page_up:
          set_value(get_value() - get_page(), true);
          break;
        case win::keys::page_down:
        case win::keys::numpad::page_down:
          set_value(get_value() + get_page(), true);
          break;
        case win::keys::home:
        case win::keys::numpad::home:
          set_value(get_min(), true);
          break;
        case win::keys::end:
        case win::keys::numpad::end:
          set_value(get_min(), true);
          break;
        }
      }
    }

  } // ctrl

} // gui
