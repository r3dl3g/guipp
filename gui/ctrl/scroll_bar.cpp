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
 * @brief     scrollbar control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/button.h>


namespace gui {

  namespace ctrl {

#ifdef GUIPP_QT

    core::point::type get_scroll_value (const core::event& e) {
      return *(core::point::type*)e.cast<win::QClientEvent>().l1();
    }

#endif // GUIPP_QT

    scroll_bar_data::scroll_bar_data ()
      : selection(scrollbar_item::nothing)
      , hilite(scrollbar_item::nothing)
      , min(0)
      , max(1)
      , step(0.01F)
      , page(0.1F)
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

    scroll_bar::scroll_bar (scroll_bar&& rhs) noexcept
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
        set_hilite(scrollbar_item::nothing);
        invalidate();
      });
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

    scrollbar_item scroll_bar::get_selection () const {
      return data.selection;
    }

    scrollbar_item scroll_bar::get_hilite () const {
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
    }

    void scroll_bar::set_page (type p) {
      data.page = p;
    }

    void scroll_bar::set_min_max_step (type mi, type ma, type s) {
      set_step(s);
      set_min_max(mi, ma);
    }

    void scroll_bar::set_min_max_page (type mi, type ma, type s) {
      set_page(s);
      set_min_max(mi, ma);
    }

    void scroll_bar::set_min_max_step_page (type mi, type ma, type s, type p) {
      set_page(p);
      set_min_max_step(mi, ma, s);
    }

    void scroll_bar::set_step_page (type s, type p) {
      set_step(s);
      set_page(p);
    }

    void scroll_bar::set_min_max_step_value (type mi, type ma, type s, type v) {
      set_min_max_step(mi, ma, s);
      set_value(v, false);
    }

    void scroll_bar::set_min_max_page_value (type mi, type ma, type s, type v) {
      set_min_max_page(mi, ma, s);
      set_value(v, false);
    }

    void scroll_bar::set_value (type v, bool notify) {
      v = std::min(std::max(v, data.min), data.max);
      if (v != data.value) {
        data.value = v;
        invalidate();
        if (notify) {
          notify_scroll();
        }
      }
    }

    void scroll_bar::set_selection (scrollbar_item s) {
      data.selection = s;
    }

    void scroll_bar::set_hilite (scrollbar_item s) {
      data.hilite = s;
    }

    scroll_bar::type scroll_bar::get_last_value () const {
      return data.last_value;
    }

    void scroll_bar::set_last_value (type last_value) {
      data.last_value = last_value;
    }

    core::native_point scroll_bar::get_last_mouse_point () const {
      return data.last_mouse_point;
    }

    void scroll_bar::set_last_mouse_point (core::native_point last_mouse_point) {
      data.last_mouse_point = last_mouse_point;
    }

    void scroll_bar::notify_scroll () {
      super::notify_event_float(detail::SCROLLBAR_MESSAGE, data.value);
    }

    void scroll_bar::handle_wheel (const core::native_point::type delta, const core::native_point&) {
      if (is_enabled()) {
        logging::trace() << "scroll_bar::handle_wheel(" << delta << ") step: " << get_step();
        set_value(get_value() - delta * get_step(), true);
      }
    }

    void scroll_bar::on_scroll (scroll_event::function&& f) {
      on<scroll_event>(std::move(f));
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::init () {
      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& /*r*/) {
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
      super::on_paint(draw::paint(this, &basic_scroll_bar::handle_paint));
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::init () {
      using namespace win;
      super::register_event_handler(event_handler_function([&] (const core::event& e, gui::os::event_result& /*r*/) {
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
      super::on_paint(draw::paint(this, &basic_scroll_bar::handle_paint));
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::handle_mouse_move (os::key_state keys, const core::native_point& npt) {
      if (is_enabled()) {
        if (core::left_button_bit_mask::is_set(keys)) {
          // check if on thumb
          if (get_selection() == scrollbar_item::thumb_button) {
            type delta = core::global::scale_from_native<type>(npt.x() - get_last_mouse_point().x()) / get_scale();
            set_value(get_last_value() + delta, true);
          }
        } else {
          auto pt = surface_to_client(npt);
          auto geo = get_metric();
          if (up_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::up_button);
          } else if (down_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::down_button);
          } else if (thumb_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::thumb_button);
          } else if (page_up_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::page_up);
          } else if (page_down_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::page_down);
          } else {
            set_hilite(scrollbar_item::nothing);
          }
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::handle_mouse_move (os::key_state keys, const core::native_point& npt) {
      if (is_enabled()) {
        if (core::left_button_bit_mask::is_set(keys)) {
          // check if on thumb
          if (get_selection() == scrollbar_item::thumb_button) {
            type delta = core::global::scale_from_native<type>(npt.y() - get_last_mouse_point().y()) / get_scale();
            set_value(get_last_value() + delta, true);
          }
        } else {
          auto pt = surface_to_client(npt);
          auto geo = get_metric();
          if (up_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::up_button);
          } else if (down_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::down_button);
          } else if (thumb_button_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::thumb_button);
          } else if (page_up_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::page_up);
          } else if (page_down_geometry(geo).is_inside(pt)) {
            set_hilite(scrollbar_item::page_down);
          } else {
            set_hilite(scrollbar_item::nothing);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void basic_scroll_bar<orientation_t::horizontal>::handle_any_key_up (os::key_state state, os::key_symbol key) {
      if (state != core::state::none) {
        return;
      }
      if (is_enabled()) {
        switch (key) {
        case core::keys::left:
        case core::keys::numpad::left:
          set_value(get_value() - get_step(), true);
          break;
        case core::keys::right:
        case core::keys::numpad::right:
          set_value(get_value() + get_step(), true);
          break;
        case core::keys::page_up:
        case core::keys::numpad::page_up:
          set_value(get_value() - get_page(), true);
          break;
        case core::keys::page_down:
        case core::keys::numpad::page_down:
          set_value(get_value() + get_page(), true);
          break;
        case core::keys::home:
        case core::keys::numpad::home:
          set_value(get_min(), true);
          break;
        case core::keys::end:
        case core::keys::numpad::end:
          set_value(get_min(), true);
          break;
        }
      }
    }

    template<>
    void basic_scroll_bar<orientation_t::vertical>::handle_any_key_up (os::key_state state, os::key_symbol key) {
      if (state != core::state::none) {
        return;
      }
      if (is_enabled()) {
        switch (key) {
        case core::keys::up:
        case core::keys::numpad::up:
          set_value(get_value() - get_step(), true);
          break;
        case core::keys::down:
        case core::keys::numpad::down:
          set_value(get_value() + get_step(), true);
          break;
        case core::keys::page_up:
        case core::keys::numpad::page_up:
          set_value(get_value() - get_page(), true);
          break;
        case core::keys::page_down:
        case core::keys::numpad::page_down:
          set_value(get_value() + get_page(), true);
          break;
        case core::keys::home:
        case core::keys::numpad::home:
          set_value(get_min(), true);
          break;
        case core::keys::end:
        case core::keys::numpad::end:
          set_value(get_min(), true);
          break;
        }
      }
    }

  } // ctrl

} // gui
