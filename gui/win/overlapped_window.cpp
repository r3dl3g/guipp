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
# include <QtGui/QBackingStore>
# include <QtGui/QPainter>
#endif // GUIPP_QT


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <gui/win/overlapped_window.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>
#include <gui/win/native.h>
#include <gui/win/dbg_win_message.h>

#if !defined(GUIPP_BUILD_FOR_MOBILE)
# define USE_INPUT_EATER
#endif

namespace gui {

  namespace win {
    // --------------------------------------------------------------------------
    class overlapped_context {
    public:
      overlapped_context ()
        : pixel_store(0)
        , gc(0)
      {}

      ~overlapped_context () {
        destroy();
      }

      core::context get_context () {
#ifdef GUIPP_QT
        return {pixel_store->paintDevice(), gc};
#else
        return {pixel_store, gc};
#endif
      }

      const core::native_size& get_size () const {
        return size;
      }

      void begin (const win::overlapped_window& w) {
        auto id = w.get_os_window();
        auto sz = core::global::scale_to_native(w.client_size());
        if (sz != size) {
          create(sz, id);
        }
#ifdef GUIPP_QT
        pixel_store->beginPaint(QRegion(0, 0, size.width(), size.height()));
        gc->begin(pixel_store->paintDevice());
#else
        native::erase(pixel_store, gc, core::native_rect(size), w.get_window_class().get_background());
#endif
      }

      void end (const win::overlapped_window& w) {
#ifdef GUIPP_X11
        auto display = core::global::get_instance();
        auto id = w.get_os_window();
        XSetWindowBackgroundPixmap(display, id, pixel_store);
        XClearWindow(display, id);
//        XFlush(display);
#elif GUIPP_QT
        gc->end();
        pixel_store->endPaint();
        pixel_store->flush(QRegion(0, 0, size.width(), size.height()), w.get_os_window());
#elif GUIPP_WIN
        auto id = w.get_os_window();
        PAINTSTRUCT ps;
        os::graphics pgc = BeginPaint(id, &ps);
        BitBlt(pgc, 0, 0, size.width(), size.height(), gc, 0, 0, SRCCOPY);
        EndPaint(id, &ps);
#endif
      }

    private:
      void create (const core::native_size& sz, os::window id) {
        destroy();
        size = sz;
        pixel_store = native::create_surface(size, id);
        gc = native::create_graphics_context(pixel_store);
      }

      void destroy () {
        if (gc) {
          native::delete_graphics_context(gc);
          gc = 0;
        }
        if (pixel_store) {
          native::delete_surface(pixel_store);
          pixel_store = 0;
        }
      }

      core::native_size size;
      os::backstore pixel_store;
      os::graphics gc;
    };
    // --------------------------------------------------------------------------
    namespace {
      std::vector<window*> capture_stack;
    }

    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window ()
      : id(0)
    {
      init();
    }
    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window (const overlapped_window& rhs)
      : super(rhs)
      , id(0)
    {
      init();
    }
    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window (overlapped_window&& rhs)
      : super(std::move(rhs))
      , id(std::move(rhs.id))
    {
      init();
    }
    // --------------------------------------------------------------------------
    overlapped_window::~overlapped_window () {
      destroy();
    }
    // --------------------------------------------------------------------------
    void overlapped_window::init () {
      focus_window = nullptr;
      capture_window = nullptr;
      mouse_window = nullptr;

      native::prepare(*this);

//      on_set_focus([&] () {
//        notify_event(core::WM_LAYOUT_WINDOW, client_area());
//      });
      on_size([&] (const core::size& sz) {
        area.set_size(sz);
#ifndef BUILD_FOR_ARM
        notify_event(core::WM_LAYOUT_WINDOW, client_area());
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
      on_lost_focus([&] () {
        if (!capture_stack.empty()) {
          capture_window = nullptr;
          capture_stack.clear();
          native::uncapture_pointer(get_os_window());
          native::unprepare_capture_pointer();
        }
      });
    }
    // --------------------------------------------------------------------------
    void overlapped_window::destroy () {
      native::destroy(get_os_window());
      id = 0;
    }
    // --------------------------------------------------------------------------
    void overlapped_window::close () {
      super::close();
      native::close(get_os_window());
    }
   // --------------------------------------------------------------------------
    overlapped_window::operator os::drawable() const {
#ifdef GUIPP_QT
      return get_context().get_context().drawable();
#else
      return id;
#endif
    }
    // --------------------------------------------------------------------------
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
    // --------------------------------------------------------------------------
    void overlapped_window::shift_focus (bool backward) {
      window_list_t children;
      collect_children(children, [] (const window* w) {
        auto state = w->get_state();
        return state.created() && state.visible() && state.enabled() && !state.overlapped();
      });
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
    // --------------------------------------------------------------------------
    void overlapped_window::create (const class_info& cls,
                                    const core::rectangle& r) {
      create_internal(cls, native::get_desktop_window(), native::adjust_overlapped_area(r, cls));
    }
    // --------------------------------------------------------------------------
    void overlapped_window::create (const class_info& cls,
                                    overlapped_window& parent,
                                    const core::rectangle& r) {
      set_parent(parent);
      create_internal(cls,
                      native::get_overlapped_parent(parent.get_os_window()),
                      native::adjust_overlapped_area(r, cls));
    }
    // --------------------------------------------------------------------------
    void overlapped_window::create_internal (const class_info& type,
                                             os::window parent_id,
                                             const core::rectangle& r) {

      if (get_os_window()) {
        destroy();
      }
      set_state().overlapped(true);

      collect_event_mask();
      set_state().created(true);
      id = native::create(type, r, parent_id, *this);
      super::create_internal(type, r);
      native::prepare_overlapped(get_os_window(), parent_id);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::add_event_mask (os::event_id mask) {
      receiver::add_event_mask(mask);
#ifdef GUIPP_X11
      if (is_valid()) {
        native::prepare_win_for_event(*this);
      }
#endif
    }
    // --------------------------------------------------------------------------
    void overlapped_window::remove_child (window* w) {
      if (w == capture_window) {
        uncapture_pointer(w);
      }
      if (w == focus_window) {
        focus_window = nullptr;
      }
      super::remove_child(w);
    }
    // --------------------------------------------------------------------------
    overlapped_context& overlapped_window::get_context () const {
      if (!surface) {
        surface = std::make_unique<overlapped_context>();
      }
      return *(surface.get());
    }
    // --------------------------------------------------------------------------
    void frame_window (core::context & cntxt, window* win, os::color col) {
      if (win) {
        native::frame(cntxt.drawable(), cntxt.graphics(),
                      core::global::scale_to_native(win->surface_area()), col);
      }
    }

#define SHOW_FOCUS
#define SHOW_MOUSE_WIN
#define SHOW_CAPTURE

    // --------------------------------------------------------------------------
    bool overlapped_window::handle_event (const core::event& e, gui::os::event_result& r) {
      // Todo: mouse enter, mouse leave, cursor
      // -> find top window at cursor position -> mouse enter/leave, cursor, mouse events.
      if (mouse_move_event::match(e) || (btn_down_event::match(e) || btn_up_event::match(e))) {
          if (capture_window && (capture_window != this)) {
            return capture_window->handle_event(e, r);
          } else {
            core::point pt = mouse_move_event::Caller::get_param<1>(e);
            window* win = window_at_point(pt);
            if (win && (win != this)) {
              if (mouse_window != win) {
                if (mouse_window) {
                  // Todo: notify mouse leave
                }
                mouse_window = win;
                // Todo: notify mouse enter
#ifdef SHOW_MOUSE_WIN
                mouse_window->invalidate();
#endif
              }
              set_cursor(mouse_window->get_cursor());
              return win->handle_event(e, r);
            } else {
              set_cursor(get_cursor());
            }
          }
      } else if ((any_key_down_event::match(e) || any_key_up_event::match(e)) && focus_window) {
        focus_window->handle_event(e, r);
      } else if (expose_event::match(e)) {

        overlapped_context& surface = get_context();
        surface.begin(*this);
        auto cntxt = surface.get_context();
        notify_event(core::WM_PAINT_WINDOW, reinterpret_cast<std::uintptr_t>(&cntxt));

#ifdef SHOW_FOCUS
        frame_window(cntxt, focus_window, color::green);
#endif
#ifdef SHOW_MOUSE_WIN
        frame_window(cntxt, mouse_window, color::red);
#endif
#ifdef SHOW_CAPTURE
        frame_window(cntxt, capture_window, color::blue);
#endif
        surface.end(*this);

        core::global::sync();

        return true;
      }
      return super::handle_event(e, r);
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::is_valid () const {
      return (get_os_window() != 0) && super::is_valid();
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_accept_focus (bool a) {
      native::prepare_accept_focus(get_os_window(), a);
      super::set_accept_focus(a);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::move_native (const core::point& pt) {
      native::move(get_os_window(), pt);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::resize_native (const core::size& sz) {
      native::resize(get_os_window(), sz);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::place_native (const core::rectangle& r) {
      native::place(get_os_window(), r);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_visible (bool s) {
      if (is_valid()) {
        native::set_visible(get_os_window(), s);
        super::set_visible(s);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::enable (bool on) {
      if (is_valid()) {
        native::enable(*this, on);
        super::enable(on);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::disable () {
      enable(false);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::to_front () {
      return native::to_front(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::to_back () {
      return native::to_back(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_title (const std::string& title) {
      native::set_title(get_os_window(), title);
    }
    // --------------------------------------------------------------------------
    std::string overlapped_window::get_title () const {
      return native::get_title(get_os_window());
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::is_minimized () const {
      return native::is_minimized(get_os_window());
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::is_maximized () const {
      return native::is_maximized(get_os_window());
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::is_top_most () const {
      return native::is_top_most(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::minimize () {
      native::minimize(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::maximize () {
      native::maximize(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::restore () {
      native::restore(get_os_window());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_top_most (bool toplevel) {
      native::set_top_most(get_os_window(), toplevel);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::invalidate () const {
      invalidate(place());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::invalidate (const core::rectangle& r) const {
      if (is_valid() && is_visible()) {
        clog::trace() << "invalidate: " << *this;
        native::invalidate(get_os_window(), r);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::redraw (const core::rectangle& r) {
      if (is_visible() && !get_state().redraw_disabled()) {
        clog::trace() << "redraw: " << *this;
        native::redraw(*this, get_os_window(), r);
      }
    }
    // --------------------------------------------------------------------------
    core::point overlapped_window::position () const {
      return native::get_geometry(get_os_window());
    }
    // --------------------------------------------------------------------------
    core::point overlapped_window::surface_offset () const {
      return core::point::zero;
    }
    // --------------------------------------------------------------------------
    core::size overlapped_window::client_size () const {
      return super::client_size();
    }
    // --------------------------------------------------------------------------
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
    // --------------------------------------------------------------------------
    window* overlapped_window::get_current_focus_window () const {
      return focus_window;
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_cursor (const cursor& c) {
      if (get_cursor() != c) {
        super::set_cursor(c);
        native::set_cursor(get_os_window(), c);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::capture_pointer (window* w) {
      if (is_valid()) {
        clog::trace() << "capture_pointer:" << *w;
        if (capture_stack.empty()) {
          native::prepare_capture_pointer();
        }
        capture_window = w;
        capture_stack.push_back(w);
        native::capture_pointer(get_os_window());
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::uncapture_pointer (window* w) {
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
            capture_window = nullptr;
          }
        } else {
          clog::warn() << "uncapture_pointer with empty capture stack!";
        }
      }
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
      get_os_window()->setModality(Qt::WindowModality::ApplicationModal);
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
      native::prepare_main_window(get_os_window());
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const class_info& cls, overlapped_window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_popup_window(get_os_window());
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, overlapped_window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_dialog_window(get_os_window(), parent.get_os_window());
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
