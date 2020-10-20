/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <map>
#include <set>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <logging/logger.h>
#include <gui/win/container.h>
#include <gui/win/widget.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>


//#define NO_CAPTURE


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    widget::widget ()
      : parent(nullptr)
    {}

    widget::widget (const widget& rhs)
      : parent(rhs.parent)
      , flags(rhs.flags)
    {
      if (parent) {
        create(*parent, rhs.place());
      }
    }

    widget::widget (widget&& rhs)
      : parent(rhs.parent)
      , flags(std::move(rhs.flags))
    {
      if (parent) {
        parent->remove_widget(&rhs);
      }
    }

    widget::~widget () {
      destroy();
    }

    void widget::destroy () {
      if (parent) {
        parent->remove_widget(this);
        parent = nullptr;
      }
    }

    void widget::create (container& p,
                         const core::rectangle& r) {
      parent = &p;
      area = r;
      parent->add_widget(this);
      notify_event(core::x11::WM_CREATE_WINDOW, this, r);
    }

    core::point widget::window_to_screen (const core::point& pt) const {
      auto p = get_parent();
      return p ? p->client_to_screen(pt) : pt;
    }

    core::point widget::screen_to_window (const core::point& pt) const {
      auto p = get_parent();
      return p ? p->screen_to_client(pt) : pt;
    }

    void widget::register_event_handler (event_handler_function&& f, os::event_id mask) {
      events.register_event_handler(std::move(f));
#ifdef X11
      x11::prepare_win_for_event(parent, mask);
#endif // X11
    }

    const container* widget::get_root_window () const {
      auto parent = get_parent();
      if (parent) {
        return parent->get_root_window();
      }
      return nullptr;
    }

    const overlapped_window* widget::get_overlapped_window () const {
      auto parent = get_parent();
      if (parent) {
        return parent->get_overlapped_window();
      } else {
        return nullptr;
      }
    }

//    namespace {
//      std::set<const widget*> active_handler;
//    }

    bool widget::handle_event (const core::event& e, gui::os::event_result& result) const {
//      if (active_handler.find(this) != active_handler.end()) {
//        return false;
//      }
//      active_handler.insert(this);

      if (any_key_up_event::match(e)) {
        os::key_symbol key = get_key_symbol(e);
        if (key == keys::tab) {
          os::key_state state = get_key_state(e);
          shift_focus(shift_key_bit_mask::is_set(state));
        }
      }

//      clog::trace() << "handle_event: " << e;
      bool res = false;
      if (is_enabled()) {
        res = events.handle_event(e, result);
      }
//      active_handler.erase(this);
      return res;
    }

    void widget::shift_focus (bool backward) const {
      const container* parent = get_parent();
      if (parent) {
        parent->shift_focus(*this, backward);
        invalidate();
      }
    }

    bool widget::can_accept_focus () const {
      return is_focus_accepting() && is_enabled() && is_visible();
    }

    void widget::on_create (create_event::function&& f) {
      on<create_event>(std::move(f));
    }

    void widget::on_close (close_event::function&& f) {
      on<close_event>(std::move(f));
    }

    void widget::on_any_key_down (any_key_down_event::function&& f) {
      on<any_key_down_event>(std::move(f));
    }

    void widget::on_any_key_up (any_key_up_event::function&& f) {
      on<any_key_up_event>(std::move(f));
    }

    void widget::on_mouse_move (mouse_move_event::function&& f) {
      on<mouse_move_event>(std::move(f));
    }

    void widget::on_mouse_move_abs (mouse_move_abs_event::function&& f) {
      on<mouse_move_abs_event>(std::move(f));
    }

    void widget::on_left_btn_down (left_btn_down_event::function&& f) {
      on<left_btn_down_event>(std::move(f));
    }

    void widget::on_left_btn_up (left_btn_up_event::function&& f) {
      on<left_btn_up_event>(std::move(f));
    }

    void widget::on_right_btn_down (right_btn_down_event::function&& f) {
      on<right_btn_down_event>(std::move(f));
    }

    void widget::on_right_btn_up (right_btn_up_event::function&& f) {
      on<right_btn_up_event>(std::move(f));
    }

    void widget::on_middle_btn_down (middle_btn_down_event::function&& f) {
      on<middle_btn_down_event>(std::move(f));
    }

    void widget::on_middle_btn_up (middle_btn_up_event::function&& f) {
      on<middle_btn_up_event>(std::move(f));
    }

    void widget::on_btn_down (btn_down_event::function&& f) {
      on<btn_down_event>(std::move(f));
    }

    void widget::on_btn_up (btn_up_event::function&& f) {
      on<btn_up_event>(std::move(f));
    }

    void widget::on_left_btn_dblclk (left_btn_dblclk_event::function&& f) {
      on<left_btn_dblclk_event>(std::move(f));
    }

    void widget::on_right_btn_dblclk (right_btn_dblclk_event::function&& f) {
      on<right_btn_dblclk_event>(std::move(f));
    }

    void widget::on_middle_btn_dblclk (middle_btn_dblclk_event::function&& f) {
      on<middle_btn_dblclk_event>(std::move(f));
    }

    void widget::on_wheel_x (wheel_x_event::function&& f) {
      on<wheel_x_event>(std::move(f));
    }

    void widget::on_wheel_y (wheel_y_event::function&& f) {
      on<wheel_y_event>(std::move(f));
    }

    void widget::on_show (show_event::function&& f) {
      on<show_event>(std::move(f));
    }

    void widget::on_hide (hide_event::function&& f) {
      on<hide_event>(std::move(f));
    }

    void widget::on_set_focus (set_focus_event::function&& f) {
      on<set_focus_event>(std::move(f));
    }

    void widget::on_lost_focus (lost_focus_event::function&& f) {
      on<lost_focus_event>(std::move(f));
    }

    void widget::on_mouse_enter (mouse_enter_event::function&& f) {
      on<mouse_enter_event>(std::move(f));
    }

    void widget::on_mouse_leave (mouse_leave_event::function&& f) {
      on<mouse_leave_event>(std::move(f));
    }

    void widget::on_move (move_event::function&& f) {
      on<move_event>(std::move(f));
    }

    void widget::on_size (size_event::function&& f) {
      on<size_event>(std::move(f));
    }

    void widget::on_place (place_event::function&& f) {
      on<place_event>(std::move(f));
    }

    void widget::on_moving (moving_event::function&& f) {
      on<moving_event>(std::move(f));
    }

    void widget::on_sizing (sizing_event::function&& f) {
      on<sizing_event>(std::move(f));
    }

    void widget::on_placing (placing_event::function&& f) {
      on<placing_event>(std::move(f));
    }

    void widget::on_layout (layout_event::function&& f) {
      on<layout_event>(std::move(f));
    }

    void widget::on_paint (paint_event::function&& f) {
      on<paint_event>(std::move(f));
    }

    os::window widget::get_id () const {
      return parent ? parent->get_id() : 0;
    }

    bool widget::is_valid () const {
      return (parent != nullptr);
    }

    bool widget::has_focus () const {
      if (parent) {
        return parent->get_focus_widget() == this;
      }
      return false;
    }

    bool widget::is_child () const {
      return true;
    }

    bool widget::is_toplevel () const {
      return false;
    }

    bool widget::is_popup () const {
      return is_toplevel();
    }

    bool widget::has_border () const {
      return false;
    }

    void widget::set_parent (container& p) {
      destroy();
      parent = &p;
      if (parent) {
        parent->add_widget(this);
      }
    }

    container* widget::get_parent () {
      return parent;
    }

    const container* widget::get_parent () const {
      return parent;
    }

    bool widget::is_child_of (const container& parent) const {
      return get_parent() == &parent;
    }

    bool widget::is_visible () const {
      return (is_valid() && get_state().is_visible());
    }

    void widget::set_visible (bool s) {
      get_state().set_visible(s);
    }

    void widget::take_focus () {
      if (parent) {
        parent->set_focus_widget(this);
        invalidate();
      }
    }

    void widget::to_front () {
      if (parent) {
        parent->widget_to_front(this);
      }
    }

    void widget::to_back () {
      if (parent) {
        parent->widget_to_back(this);
      }
    }

    void widget::redraw () const {

      auto state = get_state();

      if (state.is_redraw_disabled() || !state.is_visible() || !is_valid()) {
        return;
      }

      XEvent event;
      XExposeEvent& e = event.xexpose;

      const auto p = place();

      e.type = Expose;
      e.serial = 0;
      e.send_event = true;
      e.display = core::global::get_instance();
      e.window = parent->get_id();
      e.x = p.os_x();
      e.y = p.os_y();
      e.width = p.os_width();
      e.height = p.os_height();
      e.count = 0;
      gui::os::event_result result;

      clog::trace() << "redraw: " << event;
      events.handle_event(event, result);

      core::global::sync();
    }

    void widget::invalidate () const {
      if (is_valid() && is_visible()) {
        parent->invalidate(place());
      }
    }

    core::size widget::size () const {
      return area.size();
    }

    core::point widget::position () const {
      return area.position();
    }

    core::rectangle widget::place () const {
      return area;
    }

    core::rectangle widget::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point widget::absolute_position () const {
      return client_to_screen(core::point::zero);
    }

    core::size widget::client_size () const {
      return area.size();
    }

    core::rectangle widget::client_area () const {
      return area;
    }

    void widget::move (const core::point& pt, bool repaint) {
      if (position() != pt) {
        area.set_position(pt);
        notify_move(pt);
        if (repaint) {
          invalidate();
        }
      }
    }

    void widget::resize (const core::size& sz, bool repaint) {
      if (sz.empty()) {
        if (is_visible()) {
          set_visible(false);
        }
      } else {
        if (size() != sz) {
          if (!is_visible()) {
            set_visible();
          }
          area.set_size(sz);
          notify_size(sz);
          if (repaint) {
            invalidate();
          }
        }
      }
    }

    void widget::place (const core::rectangle& r, bool repaint) {
      if (r.empty()) {
        if (is_visible()) {
          set_visible(false);
        }
      } else {
        if (!is_visible()) {
          set_visible();
        }
        area = r;
        notify_place(r);
        if (repaint) {
          invalidate();
        }
      }
    }

    core::point widget::client_to_screen (const core::point& pt) const {
      if (parent) {
        return parent->client_to_screen(pt);
      }
      return core::point::undefined;
    }

    core::point widget::screen_to_client (const core::point& pt) const {
      if (parent) {
        return parent->screen_to_client(pt);
      }
      return core::point::undefined;
    }

    void widget::set_cursor (os::cursor c) {
      if (parent) {
        return parent->set_cursor(c);
      }
    }

    void widget::capture_pointer () {
      if (parent) {
        return parent->capture_pointer();
      }
    }

    void widget::uncapture_pointer () {
      if (parent) {
        return parent->uncapture_pointer();
      }
    }
    
    void widget::notify_event (Atom message, long l1, long l2, long l3, long l4, long l5) const {
      XEvent event;
      XClientMessageEvent& client = event.xclient;

      client.type = ClientMessage;
      client.serial = 0;
      client.send_event = True;
      client.display = core::global::get_instance();
      client.window = get_id();
      client.message_type = message;
      client.format = 32;
      client.data.l[0] = l1;
      client.data.l[1] = l2;
      client.data.l[2] = l3;
      client.data.l[3] = l4;
      client.data.l[4] = l5;

      gui::os::event_result result = 0;
      handle_event(event, result);
    }

    void widget::notify_event (Atom message, const widget* w, const core::rectangle& rect) const {
      XRectangle r = rect;
      long l1 = (long)r.x << 16 | (long)r.y;
      long l2 = (long)r.width << 16 | (long)r.height;
      notify_event(message, reinterpret_cast<long>(w), l1, l2);
    }

    void widget::notify_event (Atom message, const core::rectangle& rect) const {
      XRectangle r = rect;
      long l1 = (long)r.x << 16 | (long)r.y;
      long l2 = (long)r.width << 16 | (long)r.height;
      notify_event(message, l2, l1, l2);
    }

    void widget::notify_size (const core::size& sz) const {
      XEvent event;
      XConfigureEvent& ev = event.xconfigure;
      ev.type  = ConfigureNotify;
      ev.width = sz.os_width();
      ev.height = sz.os_height();
      gui::os::event_result result = 0;
      notify_event(core::WM_LAYOUT_WINDOW, area);
      handle_event(event, result);
    }

    void widget::notify_move (const core::point& pt) const {
      XEvent event;
      XConfigureEvent& ev = event.xconfigure;
      ev.type  = ConfigureNotify;
      ev.x = pt.os_x();
      ev.y = pt.os_y();
      gui::os::event_result result = 0;
      notify_event(core::WM_LAYOUT_WINDOW, area);
      handle_event(event, result);
    }

    void widget::notify_place (const core::rectangle& r) const {
      XEvent event;
      XConfigureEvent& ev = event.xconfigure;
      ev.type  = ConfigureNotify;
      ev.x = r.os_x();
      ev.y = r.os_y();
      ev.width = r.os_width();
      ev.height = r.os_height();
      gui::os::event_result result = 0;
      notify_event(core::WM_LAYOUT_WINDOW, area);
      handle_event(event, result);
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
