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
#include <cstring>

#ifdef GUIPP_QT
# include <QtCore/QEventLoop>
#endif // GUIPP_QT


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <gui/win/container.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>
#include <gui/win/native.h>

#if !defined(GUIPP_BUILD_FOR_MOBILE)
# define USE_INPUT_EATER
#endif

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

    container::container () {
      init();
    }

    container::container (const container& rhs)
      : super(rhs) {
      init();
    }

    container::container (container&& rhs) noexcept
      : super(std::move(rhs)) {
      init();
      for(auto& w : rhs.children) {
        w->set_parent(*this);
      }
    }

    void container::init () {
//      set_accept_focus(true);
//      on_set_focus([&] () {
//        shift_focus(core::shift_key_bit_mask::is_set(core::global::get_key_state()));
//      });
      on_show([&] () {
        set_children_visible();
      });
    }

    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    bool container::is_sub_window (const window* child) const {
      if (!child) {
        return false;
      }
      if (child == this) {
        return true;
      }
      return is_sub_window(child->get_parent());
    }

    void container::set_children_visible (bool show) {
      for(window* win : children) {
        win->set_visible(show);
      }
    }

    container::window_list_t container::get_children () const {
      return container::window_list_t(children.begin(), children.end());
    }

    void container::collect_children (window_list_t& list) const {
      for (window* win : children) {
        const container* cont = dynamic_cast<const container*>(win);
        if (cont) {
          cont->collect_children(list);
        } else {
          list.push_back(win);
        }
      }
    }

    void container::add_child (window* w) {
      children.insert(children.end(), w);
    }

    void container::remove_child (window* w) {
      children.erase(w);
    }

    void container::to_front (window* w) {
      remove_child(w);
      add_child(w);
    }

    void container::to_back (window* w) {
      remove_child(w);
      children.insert(children.begin(), w);
    }

    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window ()
      : id(0)
    {
      init();
    }

    overlapped_window::overlapped_window (const overlapped_window& rhs)
      : super(rhs)
      , id(0)
    {
      init();
    }

    overlapped_window::overlapped_window (overlapped_window&& rhs)
      : super(std::move(rhs))
      , id(std::move(rhs.id))
    {
      init();
    }

    void overlapped_window::init () {
      focus_window = nullptr;
      capture_window = nullptr;

      native::prepare(*this);

      on_set_focus([&] () {
        notify_event(core::WM_LAYOUT_WINDOW, client_area());
      });
      on_size([&] (const core::size& sz) {
        area.set_size(sz);
#ifndef BUILD_FOR_ARM
        notify_event(core::WM_LAYOUT_WINDOW, core::rectangle(sz));
#endif
      });
      on_move([&] (const core::point& pt) {
        area.set_position(pt);
      });
      on_key_down<core::keys::tab>([&] () {
        shift_focus(false);
      });
      on_key_down<core::keys::tab, core::state::shift>([&] () {
        shift_focus(true);
      });
    }

    overlapped_window::~overlapped_window () {
      destroy();
      native::unprepare(*this);
    }

    void overlapped_window::destroy () {
      native::destroy(get_os_window());
      id = 0;
    }

    void overlapped_window::close () {
      super::close();
      native::close(get_os_window());
    }

    overlapped_window::operator os::drawable() const {
      return id;
    }

    template<typename iterator>
    iterator focus_next (iterator i, iterator end) {
      while (i != end) {
        window* win = *i;
        if (win->can_accept_focus()) {
          win->take_focus();
          return i;
        }
        ++i;
      }
      return end;
    }

    template<typename iterator>
    iterator iterate_focus (iterator begin, iterator end, window* current_focus) {
      auto i = std::find(begin, end, current_focus);
      if (i != end) {
        ++i;
      }
      auto found = i;
      if (i != end) {
        found = focus_next(i, end);
        if (found == end) {
          found = focus_next(begin, i);
        }
      } else {
        found = focus_next(begin, end);
      }

      if (found != end) {
        if (current_focus) {
          current_focus->focus_lost();
        }
      }
      return found;
    }

    void overlapped_window::shift_focus (bool backward) {
      window_list_t children;
      collect_children(children);
      if (!children.empty()) {
        if (backward) {
          auto end = std::rend(children);
          if (iterate_focus(std::rbegin(children), end, get_current_focus_window()) != end) {
            return;
          }
        } else {
          auto end = std::end(children);
          if (iterate_focus(std::begin(children), end, get_current_focus_window()) != end) {
            return;
          }
        }
      }
    }

    void overlapped_window::create (const class_info& cls,
                                    const core::rectangle& r) {
      create_internal(cls, native::get_desktop_window(), native::adjust_overlapped_area(r, cls));
    }

    void overlapped_window::create (const class_info& cls,
                                    overlapped_window& parent,
                                    const core::rectangle& r) {
      set_parent(parent);
      create_internal(cls,
                      native::get_overlapped_parent(detail::get_os_window(parent)),
                      native::adjust_overlapped_area(r, cls));
    }

    void overlapped_window::create_internal (const class_info& type,
                                             os::window parent_id,
                                             const core::rectangle& r) {

      if (get_os_window()) {
        destroy();
      }
      set_state().overlapped(true);

      id = native::create(type, r, parent_id, this);
      native::prepare_overlapped(get_os_window(), parent_id);
      super::create_internal(type, r);
    }

    bool overlapped_window::is_valid () const {
      return (get_os_window() != 0) && super::is_valid();
    }

    void overlapped_window::set_accept_focus (bool a) {
      native::prepare_accept_focus(get_os_window(), a);
      super::set_accept_focus(a);
    }

    void overlapped_window::move_native (const core::point& pt) {
      native::move(get_os_window(), pt);
    }

    void overlapped_window::resize_native (const core::size& sz) {
      native::resize(get_os_window(), sz);
    }

    void overlapped_window::place_native (const core::rectangle& r) {
      native::place(get_os_window(), r);
    }

    void overlapped_window::set_visible (bool s) {
      if (is_valid()) {
        native::set_visible(get_os_window(), s);
        super::set_visible(s);
      }
    }

    void overlapped_window::enable (bool on) {
      if (is_valid()) {
        native::enable(*this, on);
        super::enable(on);
      }
    }

    void overlapped_window::disable () {
      enable(false);
    }

    void overlapped_window::to_front () {
      return native::to_front(get_os_window());
    }

    void overlapped_window::to_back () {
      return native::to_back(get_os_window());
    }

    void overlapped_window::set_title (const std::string& title) {
      native::set_title(get_os_window(), title);
    }

    std::string overlapped_window::get_title () const {
      return native::get_title(get_os_window());
    }

    bool overlapped_window::is_minimized () const {
      return native::is_minimized(get_os_window());
    }

    bool overlapped_window::is_maximized () const {
      return native::is_maximized(get_os_window());
    }

    bool overlapped_window::is_top_most () const {
      return native::is_top_most(get_os_window());
    }

    void overlapped_window::minimize () {
      native::minimize(get_os_window());
    }

    void overlapped_window::maximize () {
      native::maximize(get_os_window());
    }

    void overlapped_window::restore () {
      native::restore(get_os_window());
    }

    void overlapped_window::set_top_most (bool toplevel) {
      native::set_top_most(get_os_window(), toplevel);
    }

    void overlapped_window::invalidate () const {
      invalidate(place());
    }

    void overlapped_window::invalidate (const core::rectangle& r) const {
      if (is_valid() && is_visible()) {
        clog::trace() << "invalidate: " << *this;
        native::invalidate(get_os_window(), r);
      }
    }

    void overlapped_window::redraw (const core::rectangle& r) const {
      if (is_visible() && !get_state().redraw_disabled()) {
        clog::trace() << "redraw: " << *this;
        native::redraw(*this, get_os_window(), r);
      }
    }
    void overlapped_window::set_focus_window (window* w) {
      if (focus_window != w) {
        if (focus_window) {
          focus_window->focus_lost();
        }
        focus_window = w;
        if (focus_window) {
          native::take_focus(get_os_window());
        }
      }
    }

    window* overlapped_window::get_focus_window () const {
      return focus_window;
    }

    void overlapped_window::set_cursor (const os::cursor& c) {
      super::set_cursor(c);
      native::set_cursor(get_os_window(), c);
    }

    namespace {
      std::vector<window*> capture_stack;
    }

    void overlapped_window::capture_pointer (window* w) {
#ifndef NO_CAPTURE
      if (is_valid()) {
        clog::trace() << "capture_pointer:" << *w;
        if (capture_stack.empty()) {
          native::prepare_capture_pointer();
        }
        capture_window = w;
        capture_stack.push_back(w);
        native::capture_pointer(get_os_window());
      }
#endif // NO_CAPTURE
    }

    void overlapped_window::uncapture_pointer (window* w) {
#ifndef NO_CAPTURE
      if (is_valid()) {
        if (!capture_stack.empty()) {
          if (capture_stack.back() != w) {
            clog::fatal() << "uncapture_pointer:" << w << " differs from stack back:(" << capture_stack.back() << ")";
          } else {
            clog::trace() << "uncapture_pointer:" << w;
          }
          native::uncapture_pointer(get_os_window());
          capture_stack.pop_back();
          if (!capture_stack.empty()) {
            clog::trace() << "re-capture_pointer:" << capture_stack.back();
            capture_window = capture_stack.back();
            native::capture_pointer(get_os_window());
          } else {
            native::unprepare_capture_pointer();
            capture_window = nullptr;          }
        } else {
          clog::warn() << "uncapture_pointer with empty capture stack!";
        }
      }
#endif // NO_CAPTURE
    }

    bool overlapped_window::handle_event (const core::event& e, gui::os::event_result& r) const {
      bool ret = super::handle_event(e, r);
      if (paint_event::match(e)) {
        for (auto& w : get_children()) {
          w->handle_event(e, r);
        }

      }
      return ret;
    }

#ifdef GUIPP_X11
    // --------------------------------------------------------------------------
    class input_only_window : public window {
      using clazz = window_class<input_only_window,
                                 color::transparent,
                                 win::cursor_type::wait,
                                 win::window_class_defaults<>::style,
                                 win::window_class_defaults<>::ex_style,
                                 InputOutput>;
      using super = window;

    public:
      explicit input_only_window (container& parent) {
        super::create(clazz::get(), parent, parent.client_area());
        to_front();
      }
    };

#endif // GUIPP_X11

    // --------------------------------------------------------------------------
    modal_window::modal_window ()
      : is_modal(false)
    {
      init();
    }

    modal_window::modal_window (const modal_window& rhs)
      : super(rhs)
    {
      init();
    }

    modal_window::modal_window (modal_window&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    void modal_window::init () {
#ifdef GUIPP_WIN
      on_close([&]() {
        is_modal = false;
      });
#endif // GUIPP_WIN
    }

    void modal_window::end_modal () {
      is_modal = false;
#ifdef GUIPP_X11
      invalidate(client_area());
#endif // GUIPP_X11
#ifdef GUIPP_QT
      event_loop.exit();
#endif // GUIPP_QT
    }

    void modal_window::run_modal (overlapped_window& parent) {
      run_modal(parent, {});
    }

    void modal_window::run_modal (overlapped_window& parent, const std::vector<hot_key_action>& hot_keys) {
      clog::debug() << *this << " Enter modal loop with hot keys";

#ifdef GUIPP_QT
# ifndef GUIPP_BUILD_FOR_MOBILE
      detail::get_os_window(*this)->setWindowModality(Qt::WindowModality::ApplicationModal);
# endif
#else
      parent.disable();
#endif
      set_visible();
      to_front();
      invalidate(client_area());

      is_modal = true;

#ifdef GUIPP_X11
#if defined(USE_INPUT_EATER)
      input_only_window input_eater(parent.get_overlapped_window());
      input_eater.set_visible();
#endif // USE_INPUT_EATER

#endif // GUIPP_X11

#ifdef GUIPP_QT
      event_loop.exec(QEventLoop::DialogExec);
#else
      if (hot_keys.empty()) {
        run_loop(is_modal);
      } else {
        run_loop(is_modal, [&](const core::event & e)->bool {

#ifdef GUIPP_WIN
          if (e.type == WM_KEYDOWN) {
#elif GUIPP_X11
          if (e.type == KeyPress) {
#elif GUIPP_QT
          if (e.type() == QEvent::KeyPress) {
#endif // GUIPP_QT
            core::hot_key hk(get_key_symbol(e), get_key_state(e));
            for (hot_key_action k : hot_keys) {
              if (hk == k.hk) {
                k.fn();
                return true;
              }
            }
          }
          return detail::check_expose(e);
        });
      }
#endif // GUIPP_QT

#ifdef GUIPP_X11
#if defined(USE_INPUT_EATER)
      input_eater.set_visible(false);
#endif // USE_INPUT_EATER
#endif // GUIPP_X11

      parent.enable();
      parent.take_focus();

      clog::trace() << *this << " Exit modal loop";
    }

    // --------------------------------------------------------------------------
    void main_window::create (const class_info& cls, const core::rectangle& r) {
      super::create(cls, r);
      native::prepare_main_window(detail::get_os_window(*this));
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const class_info& cls, overlapped_window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_popup_window(detail::get_os_window(*this));
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, overlapped_window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_dialog_window(detail::get_os_window(*this), detail::get_os_window(parent));
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
