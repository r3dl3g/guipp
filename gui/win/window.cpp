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

    namespace {
      std::vector<os::window> capture_stack;
    }

    // --------------------------------------------------------------------------
    window::window ()
      : id(0)
      , area(core::rectangle::def)
      , parent(nullptr)
    {
      init();
    }

    window::window (const window& rhs)
      : id(0)
      , area(rhs.area)
      , parent(nullptr)
      , flags(rhs.flags)
    {
      init();
      if (rhs.is_valid()) {
        container* p = rhs.get_parent();
        create_internal(rhs.get_window_class(),
                        p ? detail::get_os_window(*p) : native::get_desktop_window(),
                        rhs.place());
        set_parent(*p);
      }
    }

    window::window (window&& rhs) noexcept
      : id(0)
      , area(rhs.area)
      , parent(nullptr)
      , flags(std::move(rhs.flags))
    {
      init();
      std::swap(id, rhs.id);
      if (rhs.parent) {
        rhs.parent->remove_child(&rhs);
        std::swap(parent, rhs.parent);
        rhs.parent->add_child(this);
      }
    }

    window::~window () {
      destroy();
      native::unprepare(this);
    }

    void window::init () {
      native::prepare(this);
      on_key_down<core::keys::tab>([&] () {
        get_overlapped_window()->shift_focus(false);
      });
      on_key_down<core::keys::tab, core::state::shift>([&] () {
        get_overlapped_window()->shift_focus(true);
      });
    }

    void window::create (const class_info& type,
                         container& p,
                         const core::rectangle& r) {
      if (p.is_valid()) {
        create_internal(type, detail::get_os_window(p), r);
        set_parent(p);
      }
    }

    void window::create_internal (const class_info& type,
                                  os::window parent_id,
                                  const core::rectangle& r) {

      if (get_os_window()) {
        destroy();
      }

      area = r;
      id = native::create(type, r, parent_id, this);
      set_state().created(true);
      native::notify_created(this);
    }

    void window::destroy () {
      native::destroy(get_os_window());
      id = 0;
    }

    void window::close () {
      native::notify_close(*this);
      native::close(get_os_window());
    }

    window::operator os::drawable() const {
      return id;
    }

    core::point window::window_to_screen (const core::point& pt) const {
      return parent ? parent->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window (const core::point& pt) const {
      return parent ? parent->screen_to_client(pt) : pt;
    }

    container* window::get_root_window () const {
      if (parent) {
        return parent->get_root_window();
      }
      return (container*)this;
    }

    overlapped_window* window::get_overlapped_window () const {
      if (get_state().overlapped()) {
        return (overlapped_window*)this;
      }
      if (parent) {
        return parent->get_overlapped_window();
      } else {
        return (overlapped_window*)this;
      }
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

    bool window::handle_event (const core::event& e, gui::os::event_result& result) const {

      auto_quard active_handler(this, IF_QT_ELSE(e.type(), e.type));

      if (!active_handler.insert()) {
        clog::warn() << "already in handle_event for window: " << this << " " << e;
        return false;
      }

      //      clog::trace() << "handle_event: for window: " << this << " " << e;
      bool res = false;
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
      native::prepare_accept_focus(get_os_window(), a);
      set_state().accept_focus(a);
    }

    bool window::is_focus_accepting () const {
      return get_state().focus_accepting();
    }

    void window::take_focus () {
      if (is_valid()) {
        set_state().focused(true);
        native::take_focus(get_os_window());
        invalidate();
      }
    }

    void window::notify_event_double (os::message_type message, double d1) {
      long l1 = static_cast<long>(d1 * 1000000.0);
      notify_event(message, l1, 0);
    }

    void window::notify_event (os::message_type message, long l1, long l2) {
      send_client_message(this, message, l1, l2);
    }

    void window::notify_event (os::message_type message, const core::rectangle& r) {
      send_client_message(this, message, r);
    }

    void window::set_parent (container& p) {
      if (parent) {
        parent->remove_child(this);
        parent = nullptr;
      }
      if (is_valid() && p.is_valid()) {
        parent = &p;
        p.add_child(this);
      }
    }

    container* window::get_parent () const {
      return parent;
    }

    bool window::is_child_of (const container& p) const {
      return (parent == &p);
    }

    window* window::get_current_focus_window () {
      return global::get_current_focus_window();
    }

    bool window::is_focused () const {
      return get_current_focus_window() == this;
    }

    bool window::is_valid () const {
      return (get_os_window() != 0);
    }

    bool window::is_visible () const {
//      return native::is_visible(get_os_window());
       return get_state().visible();
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        if (set_state().visible(s)) {
          native::set_visible(get_os_window(), s);
          if (s) {
            invalidate();
          }
        }
      }
    }

    void window::enable (bool on) {
      if (is_valid()) {
        if (set_state().enable(on)) {
          native::enable(get_os_window(), on);
          invalidate();
        }
      }
    }

    void window::to_front () {
      return native::to_front(get_os_window());
    }

    void window::to_back () {
      return native::to_back(get_os_window());
    }

    void window::set_cursor (const os::cursor& c) {
      if (is_valid()) {
        native::set_cursor(get_os_window(), c);
      }
    }

    void window::invalidate () const {
      if (is_valid() && is_visible()) {
        clog::trace() << "invalidate: " << *this;
        native::invalidate(get_os_window(), client_area());
      }
    }

    void window::redraw () const {
      if (is_visible() && !get_state().redraw_disabled()) {
        clog::trace() << "redraw: " << *this;
        native::redraw(*this, get_os_window(), client_area());
      }
    }

    core::size window::size () const {
      return area.size();
    }

    core::point window::position () const {
      return area.position();
    }

    core::rectangle window::place () const {
      return area;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      return client_to_screen(core::point::zero);
    }

    core::size window::client_size () const {
      return size();
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    core::point window::client_to_screen (const core::point& pt) const {
      if (parent) {
        return parent->client_to_screen(pt + position());
      }
      return pt + position();
    }

    core::point window::screen_to_client (const core::point& pt) const {
      if (parent) {
        return parent->screen_to_client(pt - position());
      }
      return pt - position();
    }

    void window::move (const core::point& pt, bool repaint) {
      if (position() != pt) {
        area.set_position(pt);
        if (is_valid()) {
          native::move(get_os_window(), pt);
          if (repaint) {
            invalidate();
          }
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
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
          if (is_valid()) {
            native::resize(get_os_window(), sz);
            if (repaint) {
              invalidate();
            }
          }
          notify_event(core::WM_LAYOUT_WINDOW, core::rectangle(sz));
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (r.empty()) {
        if (is_visible()) {
          set_visible(false);
        }
      } else {
        if (!is_visible()) {
          set_visible();
        }
        const auto current = place();
        if (current != r) {
          area = r;
          if (is_valid()) {
            native::place(get_os_window(), r);
            if (repaint) {
              invalidate();
            }
          }
          if (current.size() != r.size()) {
            notify_event(core::WM_LAYOUT_WINDOW, core::rectangle(r.size()));
          }
        }
      }
    }

    void window::capture_pointer () {
#ifndef NO_CAPTURE
      if (is_valid()) {
        auto id = get_os_window();
        clog::trace() << "capture_pointer:" << id;
        if (capture_stack.empty()) {
          native::prepare_capture_pointer();
        }
        capture_stack.push_back(id);
        native::capture_pointer(id);
      }
#endif // NO_CAPTURE
    }

    void window::uncapture_pointer () {
#ifndef NO_CAPTURE
      if (is_valid()) {
        if (!capture_stack.empty()) {
          auto id = get_os_window();
          if (capture_stack.back() != id) {
            clog::fatal() << "uncapture_pointer:" << id << " differs from stack back:(" << capture_stack.back() << ")";
          } else {
            clog::trace() << "uncapture_pointer:" << id;
          }
          native::uncapture_pointer(id);
          capture_stack.pop_back();
          if (!capture_stack.empty()) {
            clog::trace() << "re-capture_pointer:" << capture_stack.back();
            native::capture_pointer(capture_stack.back());
          } else {
            native::unprepare_capture_pointer();
          }
        }
      }
#endif // NO_CAPTURE
    }

    std::string window::get_class_name () const {
      return native::get_class_name(get_os_window());
    }

    const class_info& window::get_window_class () const {
      return native::get_window_class(get_os_window());
    }

    core::size window::screen_size () {
      return native::screen_size();
    }

    core::rectangle window::screen_area () {
      return native::screen_area();
    }

  } // namespace win

} // namespace gui
