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
#include <gui/win/container.h>
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
//      on_paint([&] (os::window id, os::graphics gc) {
//        native::erase(id, gc, surface_area().os(), get_window_class().get_background());
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
      return children;
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
      auto i = std::find(children.begin(), children.end(), w);
      if (i == children.end()) {
        children.push_back(w);
        invalidate();
      }
    }

    void container::remove_child (window* w) {
      auto i = std::find(children.begin(), children.end(), w);
      if (i != children.end()) {
        children.erase(i);
      }
    }

    void container::to_front (window* w) {
      remove_child(w);
      children.push_back(w);
      invalidate();
    }

    void container::to_back (window* w) {
      remove_child(w);
      children.insert(children.begin(), w);
      invalidate();
    }

    template<typename T>
    struct reverse {
      explicit reverse (T& iterable)
        : iterable{iterable}
      {}

      inline auto begin() const {
        return std::rbegin(iterable);
      }

      inline auto end() const {
        return std::rend(iterable);
      }

    private:
      T& iterable;
    };

    bool container::handle_event (const core::event& e, gui::os::event_result& r) {
      if (paint_event::match(e)) {
        // ToDo: clip graphics output region to childs surface area
        // ToDo Problem: clips are stored in stack in draw::graphics.
        core::context* cntxt = paint_event::Caller::get_param<0>(e);
        core::clip clp(*cntxt, surface_area());
        bool ret = super::handle_event(e, r);
        for (auto& w : children) {
          auto state = w->get_state();
          if (state.created() && state.visible() && !state.overlapped()) {
            core::clip clp2(*cntxt, w->surface_area());
            ret |= w->handle_event(e, r);
          }
        }
        return ret;
      } else if (mouse_move_event::match(e)) {
        core::point pt = mouse_move_event::Caller::get_param<1>(e);
        for (auto& w : reverse(children)) {
          auto state = w->get_state();
          if (state.created() && state.visible() && state.enabled() && !state.overlapped() && w->surface_area().is_inside(pt)) {
            if (w->handle_event(e, r)) {
              return true;
            }
          }
        }
      } else if (btn_down_event::match(e) || btn_up_event::match(e)) {
        core::point pt = btn_down_event::Caller::get_param<1>(e);
        for (auto& w : reverse(children)) {
          auto state = w->get_state();
          if (state.created() && state.visible() && state.enabled() && !state.overlapped() && w->surface_area().is_inside(pt)) {
            if (w->handle_event(e, r)) break;
          }
        }
      }
      return super::handle_event(e, r);;
    }

    os::event_id container::collect_event_mask () const {
      os::event_id mask = get_event_mask();
      for (auto& w : children) {
        if (!w->get_state().overlapped()) {
          mask |= w->collect_event_mask();
        }
      }
      return mask;
    }

    // --------------------------------------------------------------------------
    // TODO: In Qt we need a QBackingStore in an overlapped_window.
    // target must be an own object type instead just os::bitmap. F.e. os::raster.
    class private_surface {
    public:
      private_surface ()
        : pixel_store(0)
        , gc(0)
      {}

      void prepare (const core::native_size& sz, os::window id) {
        if (sz != size) {
          create(sz, id);
        }
      }

      ~private_surface () {
        destroy();
      }

      core::context get_context () {
#ifdef GUIPP_X11
        return {pixel_store, gc};
#elif GUIPP_QT
        return {pixel_store->paintDevice(), gc};
#else
        return {pixel_store, gc};
#endif
      }

      const core::native_size& get_size () const {
        return size;
      }

      void begin (const win::overlapped_window& w) {
#ifdef GUIPP_X11
        native::erase(pixel_store, gc, core::native_rect(size), w.get_window_class().get_background());
#elif GUIPP_QT
        pixel_store->beginPaint(QRegion(0, 0, size.width(), size.height()));
        gc->begin(pixel_store->paintDevice());
#else
        native::erase(pixel_store, gc, core::native_rect(size), w.get_window_class().get_background());
#endif
      }

      void end (const win::overlapped_window& w) {
#ifdef GUIPP_X11
        auto display = core::global::get_instance();
        auto id = detail::get_os_window(w);
        XSetWindowBackgroundPixmap(display, id, pixel_store);
        XClearWindow(display, id);
#elif GUIPP_QT
        gc->end();
        pixel_store->endPaint();
        pixel_store->flush(QRegion(0, 0, size.width(), size.height()), detail::get_os_window(w));
#else
        auto id = detail::get_os_window(w);
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
      return get_context().get_context().id;
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
                      native::get_overlapped_parent(detail::get_os_window(parent)),
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
    private_surface& overlapped_window::get_context () const {
      auto size = core::global::scale_to_native(client_size());
      if (!surface) {
        surface = std::make_unique<private_surface>();
      }
      surface->prepare(size, get_os_window());
      return *(surface.get());
    }
    // --------------------------------------------------------------------------
    bool overlapped_window::handle_event (const core::event& e, gui::os::event_result& r) {
      if (mouse_move_event::match(e) && capture_window && (capture_window != this)) {
        return capture_window->handle_event(e, r);
      } else if ((any_key_down_event::match(e) || any_key_up_event::match(e)) && focus_window) {
        focus_window->handle_event(e, r);
      } else if (expose_event::match(e)) {

        private_surface& surface = get_context();
        surface.begin(*this);
        auto cntxt = surface.get_context();
        notify_event(core::WM_PAINT_WINDOW, reinterpret_cast<std::uintptr_t>(&cntxt));
        surface.end(*this);

        core::global::sync();

        return true;
      } else {
        return super::handle_event(e, r);
      }
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
    void overlapped_window::set_cursor (const os::cursor& c) {
      super::set_cursor(c);
      native::set_cursor(get_os_window(), c);
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
      detail::get_os_window(*this)->setModality(Qt::WindowModality::ApplicationModal);
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
