/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     window that can overlap windows of other applications
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/core/native.h>
#include <gui/win/overlapped_window.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>
#include <gui/win/native.h>
#include <gui/win/dbg_win_message.h>

#if !defined(GUIPP_BUILD_FOR_MOBILE)
# define USE_INPUT_EATER
#endif

#define DEBUG_RECTANGLESx
#ifdef DEBUG_RECTANGLES
#define SHOW_FOCUS
#define SHOW_MOUSE_WIN
#define SHOW_CAPTURE
#define SHOW_CLIP_RECT
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
        return {get_drawable(), gc};
      }

      const core::native_size& get_size () const {
        return size;
      }

      void begin (const win::overlapped_window& w, const core::native_rect& r) {
        auto id = w.get_os_window();
        auto sz = core::global::scale_to_native(w.client_size());
        bool create_new = sz != size;
        if (create_new) {
          create(sz, id);
        }
#ifdef GUIPP_QT
        pixel_store->beginPaint(QRegion(r.x(), r.y(), r.width(), r.height()));
        gc->begin(get_drawable());
#endif
        if (create_new) {
          native::erase(get_drawable(), gc, core::native_rect(sz), w.get_background());
        }
      }

#ifdef DEBUG_RECTANGLES
      typedef core::context end_return;
#else
      typedef bool end_return;
#endif

      end_return end (os::window id) {
#ifdef GUIPP_X11
//        native::copy_surface(pixel_store, id, gc, core::native_point::zero, core::native_point::zero, size);
        auto display = core::global::get_instance();
        XSetWindowBackgroundPixmap(display, id, pixel_store);
        XClearWindow(display, id);
        XFlush(display);
# ifdef DEBUG_RECTANGLES
        return {id};
# else
        return true;
# endif
#elif GUIPP_QT
# ifdef DEBUG_RECTANGLES
        return {get_drawable(), gc};
# else
        return true;
# endif
#elif GUIPP_WIN
        os::graphics pgc = BeginPaint(id, &ps);
        BitBlt(pgc, 0, 0, size.width(), size.height(), gc, 0, 0, SRCCOPY);
# ifdef DEBUG_RECTANGLES
        return {id, pgc};
# else
        EndPaint(id, &ps);
        return true;
# endif
#endif
      }

      void finish (end_return& ctx) {
#ifdef GUIPP_QT
        gc->end();
        pixel_store->endPaint();
        pixel_store->flush(QRegion(0, 0, size.width(), size.height()));
#elif GUIPP_WIN
# ifdef DEBUG_RECTANGLES
        EndPaint((os::window)ctx.drawable(), &ps);
# endif
#endif
      }

    private:
      friend class overlapped_window;
      void destroy () {
        if (gc) {
          core::native::delete_graphics_context(gc);
          gc = 0;
        }
        if (pixel_store) {
          native::delete_surface(pixel_store);
          pixel_store = 0;
        }
      }

    private:
      void create (const core::native_size& sz, os::window id) {
        destroy();
        size = sz;
        pixel_store = native::create_surface(size, id);
        gc = core::native::create_graphics_context(IF_QT_ELSE(nullptr, get_drawable()));
      }

      gui::os::drawable get_drawable () {
#ifdef GUIPP_QT
        return pixel_store->paintDevice();
#else
        return pixel_store;
#endif
      }

      core::native_size size;
      os::backstore pixel_store;
      os::graphics gc;
#ifdef GUIPP_WIN
      PAINTSTRUCT ps;
#endif
    };
    // --------------------------------------------------------------------------
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
    }
    // --------------------------------------------------------------------------
    void overlapped_window::destroy () {
      surface.reset();
      native::destroy(get_os_window());
      auto s = set_state();
      s.visible(false);
      s.created(false);
      id = 0;
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
#ifdef GUIPP_BUILD_FOR_MOBILE
      if (get_parent() == nullptr) {
        native::maximize(id);
      }
#endif
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
      if (w == mouse_window) {
        mouse_window = nullptr;
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
    void overlapped_window::set_mouse_window (window* win) {
      if (mouse_window != win) {
        if (mouse_window) {
          mouse_window->notify_mouse_event(false);
        }
        mouse_window = win;
        if (mouse_window) {
          mouse_window->notify_mouse_event(true);
        }
#ifdef SHOW_MOUSE_WIN
        invalidate();
#endif
      }
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::handle_event (const core::event& e, gui::os::event_result& r) {
      if (is_mouse_event(e)) {
          if (capture_window && (capture_window != this)) {
            return capture_window->handle_event(e, r);
          } else {
            core::native_point pt = mouse_move_event::Caller::get_param<1>(e);
            window* win = window_at_point(pt);
            if (win == this) {
              win = nullptr;
            }
            set_mouse_window(win);
            native::set_cursor(get_os_window(), mouse_window ? mouse_window->get_cursor() : get_cursor());
            if (mouse_window && (mouse_window != this)) {
              return mouse_window->handle_event(e, r);
            }
          }
      } else if (any_key_down_event::match(e) && (any_key_down_event::Caller::get_param<1>(e) == core::keys::tab)) {
        shift_focus(any_key_down_event::Caller::get_param<0>(e) == core::state::shift);
      } else if (is_key_event(e) && focus_window && (focus_window != this)) {
        focus_window->handle_event(e, r);
      } else if (size_event::match(e)) {
        area.set_size(size_event::Caller::get_param<0>(e));
#ifndef BUILD_FOR_ARM
        notify_layout();
#endif
        invalidate();
      } else if (move_event::match(e)) {
        area.set_position(move_event::Caller::get_param<0>(e));
      } else if (lost_focus_event::match(e)) {
        set_mouse_window(nullptr);
        if (!capture_stack.empty()) {
          capture_window = nullptr;
          capture_stack.clear();
          native::uncapture_pointer(get_os_window());
        }
      } else if (mouse_leave_event::match(e)) {
        set_mouse_window(nullptr);
      } else if (show_event::match(e)) {
        set_state().visible(true);
        invalidate();
      } else if (hide_event::match(e)) {
        set_state().visible(false);
#ifdef GUIPP_WIN
      } else if (core::event_handler<WM_PAINT>::match(e)) {
        redraw(invalid_rect);
#elif GUIPP_QT
      } else if (e.type() == QEvent::UpdateRequest) {
        redraw(invalid_rect);
      } else if ((e.type() == QEvent::Expose) || (e.type() == QEvent::OrientationChange)) {
        const auto r = get_os_window()->geometry();
        const core::native_rect nr(r.x(), r.y(), r.width(), r.height());
        area = core::global::scale_from_native(nr);
        redraw(nr);
#endif // GUIPP_WIN
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
    void overlapped_window::geometry_native (const core::rectangle& r) {
      native::geometry(get_os_window(), r);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_visible (bool s) {
      if (is_valid()) {
        native::set_visible(get_os_window(), s);
        set_state().visible(s);
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
    bool overlapped_window::is_fullscreen () const {
      return native::is_fullscreen(get_os_window());
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
    void overlapped_window::set_fullscreen (bool fullscreen) {
      native::set_fullscreen(get_os_window(), fullscreen);
    }
    // --------------------------------------------------------------------------
    void overlapped_window::invalidate () {
      invalidate(surface_geometry());
    }
    // --------------------------------------------------------------------------
    void overlapped_window::invalidate (const core::native_rect& r) {
      if (is_valid() && is_visible()) {
        if (invalid_rect.empty()) {
          invalid_rect = r;
        } else {
          invalid_rect |= r;
        }
        logging::trace() << "invalidate: region " << r << " -> " << invalid_rect << " in window " << this;
        native::invalidate(get_os_window(), invalid_rect);
      } else {
        logging::trace() << "ignore invalidate request, state: " << get_state();
      }
    }
    // --------------------------------------------------------------------------
    void frame_window (core::context& ctx, window* win, os::color col) {
      if (win) {
        native::frame(ctx.drawable(), ctx.graphics(), win->surface_geometry(), col);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::redraw (const core::native_rect& r) {
      if (is_visible() && !get_state().redraw_disabled()) {
        if (invalid_rect.empty()) {
          invalid_rect = r;
        } else {
          invalid_rect |= r;
        }
        if (invalid_rect.empty()) {
          logging::trace() << "skip redraw, invalid_rect is empty " << this;
          return;
        }

#ifdef GUIPP_QT
        if (!get_os_window()->isExposed()) {
          logging::trace() << "skip redraw, window is not exposed " << this;
          return;
        }
#endif
        logging::trace() << "redraw region " << r << " -> " << invalid_rect << " in window " << this;

        overlapped_context& surface = get_context();
        surface.begin(*this, invalid_rect);
        auto cntxt = surface.get_context();

        core::clip clp(cntxt, invalid_rect);
        native::erase(cntxt.drawable(), cntxt.graphics(), invalid_rect, get_background());
        notify_event(core::WM_PAINT_WINDOW, &cntxt, &invalid_rect);
        auto wctxt = surface.end(get_os_window());

#if defined(SHOW_FOCUS) || defined(SHOW_MOUSE_WIN) || defined(SHOW_CAPTURE) || defined(SHOW_CLIP_RECT)

        const auto offs = surface_position();
        cntxt.set_offset(offs.x(), offs.y());

#ifdef SHOW_FOCUS
        frame_window(wctxt, focus_window, color::green);
#endif
#ifdef SHOW_MOUSE_WIN
        frame_window(wctxt, mouse_window, color::red);
#endif
#ifdef SHOW_CAPTURE
        frame_window(wctxt, capture_window, color::blue);
#endif
#ifdef SHOW_CLIP_RECT
        native::frame(wctxt.drawable(), wctxt.graphics(), invalid_rect, color::cyan);
#endif
#endif // defined(SHOW_FOCUS) || defined(SHOW_MOUSE_WIN) || defined(SHOW_CAPTURE) || defined(SHOW_CLIP_RECT)

//        clp.unclip();

        surface.finish(wctxt);

        invalid_rect = core::native_rect::zero;
      } else {
        logging::trace() << "ignore redraw, state: " << get_state() << " in window " << this;
      }
    }
    // --------------------------------------------------------------------------
    core::point overlapped_window::position () const {
      return core::global::scale_from_native(native::get_position(get_os_window()));
    }
    // --------------------------------------------------------------------------
    core::native_point overlapped_window::surface_position () const {
      return core::native_point::zero;
    }
    // --------------------------------------------------------------------------
    core::point overlapped_window::surface_to_screen (const core::native_point& pt) const {
      return core::global::scale_from_native(native::surface_to_screen(get_os_window(), pt));
    }
    // --------------------------------------------------------------------------
    core::native_point overlapped_window::screen_to_surface (const core::point& pt) const {
      return native::screen_to_surface(get_os_window(), core::global::scale_to_native(pt));
    }
    // --------------------------------------------------------------------------
    core::size overlapped_window::client_size () const {
      return native::client_size(get_os_window(), super::client_size());
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
        logging::trace() << "capture_pointer:" << *w;
        if (capture_stack.empty()) {
          logging::trace() << "capture_pointer for overlapped_window " << this;
          native::capture_pointer(get_os_window());
        }
        capture_window = w;
        capture_stack.push_back(w);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::uncapture_pointer (window* w) {
      if (is_valid()) {
        if (!capture_stack.empty()) {
          if (capture_stack.back() != w) {
            logging::fatal() << "uncapture_pointer:" << w << " differs from stack back:(" << capture_stack.back() << ")";
          } else {
            logging::trace() << "uncapture_pointer:" << w;
          }
          capture_stack.pop_back();
          if (!capture_stack.empty()) {
            logging::trace() << "re-capture_pointer:" << capture_stack.back();
            capture_window = capture_stack.back();
          } else {
            logging::trace() << "uncapture_pointer for overlapped_window " << this;
            native::uncapture_pointer(get_os_window());
            capture_window = nullptr;
          }
        } else {
          logging::warn() << "uncapture_pointer with empty capture stack!";
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
        super::create(clazz::get(), parent, parent.client_geometry());
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
      invalidate(surface_geometry());
#endif // GUIPP_X11
#ifdef GUIPP_QT
      event_loop.exit();
#endif // GUIPP_QT
    }

    void modal_window::run_modal (overlapped_window& parent) {
      run_modal(parent, {});
    }

    void modal_window::run_modal (overlapped_window& parent, const std::vector<hot_key_action>& hot_keys) {
      logging::debug() << *this << " Enter modal loop with hot keys";

#ifdef GUIPP_QT
# ifndef GUIPP_BUILD_FOR_MOBILE
      get_os_window()->setModality(Qt::WindowModality::ApplicationModal);
# endif
#else
      parent.disable();
#endif
      set_visible();
      to_front();
      invalidate(surface_geometry());

      is_modal = true;

#ifdef GUIPP_X11
#if defined(USE_INPUT_EATER)
      input_only_window input_eater(parent.get_overlapped_window());
      input_eater.set_visible();
#endif // USE_INPUT_EATER

#endif // GUIPP_X11

#ifdef GUIPP_QT
      for (auto& hk : hot_keys) {
        global::register_hot_key(hk.hk, hk.fn, this);
      }
      event_loop.exec(QEventLoop::DialogExec);
      for (auto& hk : hot_keys) {
        global::unregister_hot_key(hk.hk);
      }
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

      logging::trace() << *this << " Exit modal loop";
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
