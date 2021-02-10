/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
#include <gui/win/native.h>

#define NO_CAPTURExx


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    window::window ()
      : area(core::rectangle::def)
      , parent(nullptr)
      , class_name(nullptr)
      , cursor()
    {
      init();
    }

    window::window (const window& rhs)
      : area(rhs.area)
      , parent(nullptr)
      , flags(rhs.flags)
      , class_name(nullptr)
      , cursor()
    {
      init();
      if (rhs.is_valid()) {
        container* p = rhs.get_parent();
        if (p) {
          create(rhs.get_window_class(), *p, rhs.place());
        }
      }
    }

    window::window (window&& rhs) noexcept
      : area(std::move(rhs.area))
      , parent(std::move(rhs.parent))
      , flags(std::move(rhs.flags))
      , class_name(rhs.class_name)
      , cursor(rhs.cursor)
    {
      init();
      if (parent) {
        parent->remove_child(&rhs);
        parent->add_child(this);
      }
    }

    window::~window () {
      if (parent) {
        parent->remove_child(this);
        parent = nullptr;
      }
      set_state().created(false);
    }

    void window::init () {
      on_key_down<core::keys::tab>([&] () {
        get_overlapped_window().shift_focus(false);
      });
      on_key_down<core::keys::tab, core::state::shift>([&] () {
        get_overlapped_window().shift_focus(true);
      });
    }

    void window::create (const class_info& type,
                         container& p,
                         const core::rectangle& r) {
      if (p.is_valid()) {
        set_parent(p);
        create_internal(type, r);
        p.add_event_mask(get_event_mask());
      }
    }

    void window::create_internal (const class_info& type,
                                  const core::rectangle& r) {
      class_name = type.get_class_name();
      area = r;
      set_state().created(true);
      notify_event(core::WM_CREATE_WINDOW);
    }

    void window::close () {
      native::notify_close(*this);
    }

    const window& window::get_root_window () const {
      if (parent) {
        return parent->get_root_window();
      }
      return *this;
    }

    overlapped_window& window::get_overlapped_window () const {
      const overlapped_window* o = dynamic_cast<const overlapped_window*>(this);
      if (o) {
        return *const_cast<overlapped_window*>(o);
      }
      if (parent) {
        return parent->get_overlapped_window();
      } else {
        throw std::runtime_error("Window has no overlapped parent!");
      }
    }

    bool window::has_overlapped_window () const {
      if (dynamic_cast<const overlapped_window*>(this)) {
        return true;
      }
      if (parent) {
        return parent->has_overlapped_window();
      } else {
        return false;
      }
    }

    void window::add_event_mask (os::event_id mask) {
      super::add_event_mask(mask);
      if (is_valid()) {
        get_overlapped_window().add_event_mask(get_event_mask());
      }
    }

    os::event_id window::collect_event_mask () const {
      return get_event_mask();
    }

    struct GUIPP_WIN_EXPORT auto_quard : public std::pair<const window*, os::event_id> {
      typedef std::pair<const window*, os::event_id> super;

      auto_quard (const window* win, os::event_id id)
        : super(win, id)
      {}

      ~auto_quard () {
        if (iter.second) {
          active_events.erase(iter.first);
        }
      }

      bool insert () {
        iter = active_events.insert(*this);
        return iter.second;
      }

    private:
      std::pair<std::set<auto_quard>::iterator, bool> iter;
      static std::set<auto_quard> active_events;
    };

    std::set<auto_quard> auto_quard::active_events;

    bool window::handle_event (const core::event& e, gui::os::event_result& result) {

      auto_quard active_handler(this, IF_QT_ELSE(e.type(), e.type));

      if (!active_handler.insert()) {
        clog::warn() << "already in handle_event for window: " << this << " " << e;
        return false;
      }

//      clog::trace() << "handle_event: for window: " << this << " " << e;
      bool res = false;
//      if (paint_event::match(e)) {
//        clog::info() << "window::handle_event paint";
//      }

      if (is_enabled() || paint_event::match(e)) {
        res = events.handle_event(e, result);
      }

      return res;
    }

    void window::focus_lost () {
      set_state().focused(false);
      invalidate();
    }

    bool window::can_accept_focus () const {
      return is_focus_accepting() && is_enabled() && is_visible();
    }

    void window::set_accept_focus (bool a) {
      set_state().accept_focus(a);
    }

    bool window::is_focus_accepting () const {
      return get_state().focus_accepting();
    }

    void window::take_focus () {
      if (is_valid()) {
        set_state().focused(true);
        get_overlapped_window().set_focus_window(this);
        invalidate();
      }
    }

    void window::notify_event_double (os::message_type message, double d1) {
      long l1 = static_cast<long>(d1 * 1000000.0);
      notify_event(message, l1, 0);
    }

    void window::notify_event (os::message_type message, long l1, long l2) {
      native::send_client_message(this, message, l1, l2);
    }

    void window::notify_event (os::message_type message, const core::rectangle& r) {
      native::send_client_message(this, message, r);
    }

    void window::set_parent (container& p) {
      if (parent) {
        parent->remove_child(this);
        parent = nullptr;
      }
      parent = &p;
      p.add_child(this);
    }

    container* window::get_parent () const {
      return parent;
    }

    bool window::is_child_of (const container& p) const {
      return (parent == &p);
    }

    window* window::get_current_focus_window () const {
      return get_overlapped_window().get_current_focus_window();
    }

    bool window::is_focused () const {
      return get_current_focus_window() == this;
    }

    bool window::is_valid () const {
      return get_state().created() && has_overlapped_window();
    }

    bool window::is_visible () const {
       return get_state().visible();
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        if (set_state().visible(s)) {
          invalidate();
        }
      }
    }

    void window::enable (bool on) {
      if (set_state().enable(on)) {
        if (is_valid()) {
          invalidate();
        }
      }
    }

    void window::to_front () {
      if (get_parent()) {
        get_parent()->to_front(this);
      }
    }

    void window::to_back () {
      if (get_parent()) {
        get_parent()->to_back(this);
      }
    }

    void window::set_cursor (const os::cursor& c) {
      cursor = c;
    }

    const os::cursor& window::get_cursor () const {
      return cursor;
    }

    void window::invalidate () const {
      if (is_valid() && is_visible()) {
        clog::trace() << "invalidate: " << *this;
        get_overlapped_window().invalidate(client_area());
      }
    }

    void window::redraw () const {
      if (is_visible() && !get_state().redraw_disabled()) {
        clog::trace() << "redraw: " << *this;
        get_overlapped_window().redraw(client_area());
      }
    }

    core::size window::size () const {
      return area.size();
    }

    core::size window::client_size () const {
      return area.size();
    }

    core::point window::position () const {
      return area.position();
    }

    core::point window::absolute_position () const {
      if (dynamic_cast<const overlapped_window*>(this)) {
        return position();
      } else {
        return surface_offset() + get_overlapped_window().position();
      }
    }

    core::point window::surface_offset () const {
      return area.position() + (parent ? parent->surface_offset() : core::point::zero);
    }

    core::rectangle window::surface_area () const {
      return core::rectangle(surface_offset(), client_size());
    }

    core::rectangle window::place () const {
      return area;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    core::point window::window_to_screen (const core::point& pt) const {
      return parent ? parent->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window (const core::point& pt) const {
      return parent ? parent->screen_to_client(pt) : pt;
    }

    core::point window::client_to_screen (const core::point& pt) const {
      return pt + absolute_position();
    }

    core::point window::screen_to_client (const core::point& pt) const {
      return pt - absolute_position();
    }

    core::point window::client_to_surface (const core::point& pt) const {
      return pt + surface_offset();
    }

    core::point window::surface_to_client (const core::point& pt) const {
      return pt - surface_offset();
    }

    core::point window::surface_to_screen (const core::point& pt) const {
      return pt - surface_offset() + absolute_position();
    }

    void window::move_native (const core::point&) {}
    void window::resize_native (const core::size&) {}
    void window::place_native (const core::rectangle&) {}

    void window::move (const core::point& pt, bool repaint) {
      const auto previous = position();
      if (previous != pt) {
        area.set_position(pt);
        if (is_valid()) {
          move_native(pt);
          if (repaint) {
            invalidate();
          }
        }
        native::notify_move(*this, pt, previous);
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      const auto previous = size();
      if (previous != sz) {
        area.set_size(sz);
        if (sz.empty()) {
          if (is_visible()) {
            set_visible(false);
          }
        } else {
          if (!is_visible()) {
            set_visible();
          }
          if (is_valid()) {
            resize_native(sz);
            if (repaint) {
              invalidate();
            }
          }
        }
        native::notify_resize(*this, sz, previous);
        notify_event(core::WM_LAYOUT_WINDOW, client_area());
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      const auto previous = place();
      if (previous != r) {
        area = r;
        if (r.empty()) {
          if (is_visible()) {
            set_visible(false);
          }
        } else {
          if (!is_visible()) {
            set_visible();
          }
          if (is_valid()) {
            place_native(area);
            if (repaint) {
              invalidate();
            }
          }
        }
        if (previous.position() != area.position()) {
          native::notify_move(*this, area.position(), previous.position());
        }
        if (previous.size() != area.size()) {
          native::notify_resize(*this, area.size(), previous.size());
          notify_event(core::WM_LAYOUT_WINDOW, client_area());
        }
      }
    }

    void window::capture_pointer () {
      get_overlapped_window().capture_pointer(this);
    }

    void window::uncapture_pointer () {
      get_overlapped_window().uncapture_pointer(this);
    }

    const char* window::get_class_name () const {
      return class_name;
    }

    const class_info& window::get_window_class () const {
      return native::get_window_class(get_class_name());
    }

    core::size window::screen_size () {
      return native::screen_size();
    }

    core::rectangle window::screen_area () {
      return native::screen_area();
    }

  } // namespace win

} // namespace gui
