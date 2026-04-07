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
#include <util/ostreamfmt.h>
#include <util/time_util.h>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/window_event_proc.h"
#include "gui/win/window_event_handler.h"
#include "gui/win/native.h"
#include "gui/win/dbg_win_message.h"


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
#ifdef GUIPP_SDL
    class overlapped_context {
    public:
      overlapped_context ()
        : gc(nullptr)
      {}

      ~overlapped_context () {
        if (gc) {
          core::native::delete_graphics_context(gc);
          gc = nullptr;
        }
      }

      core::context get_context () {
        return {pixel_store, gc};
      }

      void begin (const win::overlapped_window& w, const core::native_rect& r) {
        auto id = w.get_os_window();
        auto sz = core::global::scale_to_native(w.client_size());
        pixel_store = id;
        if (!gc) {
          gc = core::native::create_graphics_context(id);
          native::erase(pixel_store, gc, core::native_rect(sz), w.get_background());
        }
      }

#ifdef DEBUG_RECTANGLES
      typedef core::context end_return;
#else
      typedef bool end_return;
#endif

      end_return end (os::window id) {
# ifdef DEBUG_RECTANGLES
        return {id, gc};
# else
        return true;
#endif
      }

      void finish (end_return& ctx) {
        SDL_RenderPresent(gc);
      }

      gui::os::drawable get_drawable () {
        return pixel_store;
      }

    private:
      void destroy () {
        if (gc) {
          core::native::delete_graphics_context(gc);
          gc = 0;
        }
      }

    private:
      os::drawable pixel_store;
      os::graphics gc;
    };

#else
    class overlapped_context {
    public:
      overlapped_context ()
        : pixel_store({})
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
#elif GUIPP_SDL
        pixel_store = id;
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
        auto display = core::global::get_instance();
        XSetWindowBackgroundPixmap(display, id, pixel_store);
        XClearWindow(display, id);
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
#elif GUIPP_JS
        return true;
#elif GUIPP_SDL
        //SDL_UpdateWindowSurface(id);
        return true;
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
#elif GUIPP_X11
        XFlushGC(core::global::get_instance(), gc);
#elif GUIPP_SDL
        SDL_RenderPresent(gc);
#endif
      }

    private:
      friend class overlapped_window;
      void destroy () {
#ifdef GUIPP_JS
          gc = {};
          pixel_store = {};
#else
        if (gc) {
          core::native::delete_graphics_context(gc);
          gc = 0;
        }
# ifndef GUIPP_SDL
        if (pixel_store) {
          native::delete_surface(pixel_store);
          pixel_store = 0;
        }
# endif //GUIPP_SDL
#endif //GUIPP_JS
      }

    private:
      void create (const core::native_size& sz, os::window id) {
        destroy();
        size = sz;
        gc = core::native::create_graphics_context(IF_QT_ELSE(nullptr, get_drawable()));
#ifdef GUIPP_SDL
        pixel_store = id;
#else
        pixel_store = native::create_surface(size, id);
#endif
      }

      gui::os::drawable get_drawable () {
#ifdef GUIPP_QT
        return pixel_store->paintDevice();
#else
        return pixel_store;
#endif
      }

      core::native_size size;
      IF_SDL_ELSE(os::drawable, os::backstore) pixel_store;
      os::graphics gc;
#ifdef GUIPP_WIN
      PAINTSTRUCT ps;
#endif
    };
#endif
    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window ()
      : id(IF_JS_ELSE(emscripten::val::undefined(), 0))
    {
      init();
    }
    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window (const overlapped_window& rhs)
      : super(rhs)
      , id(IF_JS_ELSE(emscripten::val::undefined(), 0))
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
      id = IF_JS_ELSE(emscripten::val::undefined(), 0);
    }
   // --------------------------------------------------------------------------
    overlapped_window::operator os::drawable() const {
#ifdef GUIPP_QT
      return get_context().get_context().drawable();
#else
      return get_context().get_drawable();
#endif
    }
    // --------------------------------------------------------------------------
    void overlapped_window::create (const class_info& cls,
                                    const core::rectangle& r) {
      create_internal(cls,
                      native::get_desktop_window(), 
                      native::adjust_overlapped_area(r, cls));
    }
    // --------------------------------------------------------------------------
    void overlapped_window::create (const class_info& cls,
                                    container& parent,
                                    const core::rectangle& r) {
      set_parent(parent);
      create_internal(cls,
                      native::get_overlapped_parent(parent.get_overlapped_window().get_os_window()),
                      native::adjust_overlapped_area(r, cls));
    }
    // --------------------------------------------------------------------------
    void overlapped_window::create_internal (const class_info& type,
                                             os::window parent_id,
                                             const core::rectangle& r) {

      if (native::is_valid(get_os_window())) {
        destroy();
      }
      
      collect_event_mask();
      set_state().created(true);
#ifdef GUIPP_JS
      if (parent_id == native::get_desktop_window()) {
#endif //GUIPP_JS
        set_state().overlapped(true);
        id = native::create(type, r, parent_id, *this);
#ifdef GUIPP_JS
      }
#endif //GUIPP_JS
      super::set_window_class(type);
      super::create_internal(type, r);
      native::prepare_overlapped(get_os_window(), parent_id);
      invalidate();
#ifdef GUIPP_BUILD_FOR_MOBILE
      if (get_parent() == nullptr) {
        native::maximize(id);
      }
#endif
    }
    // --------------------------------------------------------------------------
    void overlapped_window::add_event_mask (os::event_id mask) {
      receiver::add_event_mask(mask);
      if (is_valid()) {
        native::prepare_win_for_event(*this);
      }
    }
    // --------------------------------------------------------------------------
    void overlapped_window::remove (window* w) {
      if (w == capture_window) {
        uncapture_pointer(w);
      }
      if (w == focus_window) {
        focus_window = nullptr;
      }
      if (w == mouse_window) {
        mouse_window = nullptr;
      }
      super::remove(w);
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
      if (is_enabled() && is_mouse_event(e)) {
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
      } else if (is_enabled() && any_key_down_event::match(e) && (any_key_down_event::Caller::get_param<1>(e) == core::keys::tab)) {
        shift_focus(any_key_down_event::Caller::get_param<0>(e) == core::state::shift);
      } else if (is_enabled() && is_key_event(e) && focus_window && (focus_window != this)) {
        focus_window->handle_event(e, r);
#ifdef GUIPP_WIN
      } else if (win32::window_pos_changed_event::match(e)) {
        area.set_size(core::global::scale_from_native(get_rect<WINDOWPOS>(e).size()));
#else
      } else if (size_event::match(e)) {
        area.set_size(size_event::Caller::get_param<0>(e));
#endif
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
      return native::is_valid(get_os_window()) && super::is_valid();
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
      invalidate();
    }
    // --------------------------------------------------------------------------
    void overlapped_window::geometry_native (const core::rectangle& r) {
      native::geometry(get_os_window(), r);
      invalidate();
    }
    // --------------------------------------------------------------------------
    void overlapped_window::set_visible (bool s) {
      if (is_valid()) {
        native::set_visible(get_os_window(), s);
        set_state().visible(s);
        if (s) {
          invalidate();
        }
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
        if (!r.empty()) {
          if (invalid_rect.empty()) {
            invalid_rect = r;
          } else {
            invalid_rect |= r;
          }
        }
        if (invalid_rect.empty()) {
          logging::trace() << "skip redraw, invalid_rect is empty " << this;
          return;
        }

        util::time::chronometer chrono;
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

        logging::trace() << "overlapped_window clip " << invalid_rect;
        core::clip clp(cntxt, invalid_rect);
        native::erase(cntxt.drawable(), cntxt.graphics(), invalid_rect, get_background());
        logging::trace() << "notify_event(paint_event)";

        notify_paint_event(cntxt, invalid_rect);
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
        logging::trace() << "overlapped_window::redraw finished in "<< chrono.stop();
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
    core::point overlapped_window::surface_to_popup (const core::native_point& pt) const {
      return IF_GUIPP_POPUP_OVERLAPP(surface_to_screen(pt), surface_to_client(pt));
    }
    // --------------------------------------------------------------------------
    core::point overlapped_window::get_current_pointer_pos () const {
      return surface_to_client(native::get_current_pointer_pos(get_os_window()));
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

#if GUIPP_X11 && USE_INPUT_EATER
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

#endif // GUIPP_X11 && USE_INPUT_EATER

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

    void modal_window::run_modal (container& parent) {
      run_modal(parent, {});
    }

    void modal_window::run_modal (container& parent, const std::vector<hot_key_action>& hot_keys) {
      logging::debug() << *this << " Enter modal loop with hot keys";

#ifdef GUIPP_QT
# if GUIPP_POPUP_OVERLAPP && !GUIPP_BUILD_FOR_MOBILE
      get_os_window()->setModality(Qt::WindowModality::ApplicationModal);
# endif
#endif
      set_visible();
      to_front();
      invalidate(surface_geometry());

      is_modal = true;

#if GUIPP_X11 && USE_INPUT_EATER
      input_only_window input_eater(parent.get_overlapped_window());
      input_eater.set_visible();
#endif // GUIPP_X11 && USE_INPUT_EATER

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
          if (is_hotkey_event(e)) {
            core::hot_key hk(get_key_symbol(e), get_key_state(e));
            for (hot_key_action k : hot_keys) {
              if (hk == k.hk) {
                k.fn();
                return true;
              }
            }
          }
          return false;
        });
      }
#endif // GUIPP_QT

#if GUIPP_X11 && USE_INPUT_EATER
      input_eater.set_visible(false);
#endif // GUIPP_X11 && USE_INPUT_EATER

      //parent.enable();
      parent.take_focus();

      logging::trace() << *this << " Exit modal loop";
    }

    // --------------------------------------------------------------------------
    void container_window::create (const class_info& cls,
                                   container& parent,
                                   const core::rectangle& r) {
        super::create(cls, parent, r);
    }

    void container_window::destroy() {
    }

    // --------------------------------------------------------------------------
    void container_window::set_title (const std::string& t) {
      title = t;
    }

    os::window container_window::get_os_window () const {
      return get_overlapped_window().get_os_window();
    }

    // --------------------------------------------------------------------------
    const std::string& container_window::get_title () const {
      return title;
    }

    // --------------------------------------------------------------------------
    void main_window::create (const class_info& cls, const core::rectangle& r) {
      super::create(cls, r);
      native::prepare_main_window(get_os_window());
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const class_info& cls,
                               container& parent,
                               const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_popup_window(get_os_window());
    }

    // --------------------------------------------------------------------------
    void tooltip_window::create (const class_info& cls,
                                 container& parent,
                                 const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_tooltip_window(get_os_window());
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls,
                                container& parent,
                                const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_dialog_window(get_os_window(),
                                    parent.get_overlapped_window().get_os_window());
    }

    // --------------------------------------------------------------------------
    void palette_window::create (const class_info& cls,
                                 container& parent,
                                 const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_palette_window(get_os_window());
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
