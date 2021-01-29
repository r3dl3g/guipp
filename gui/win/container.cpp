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
    }

    os::window container::get_os_window () const {
      return detail::get_os_window(*this);
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

    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window () {
      on_set_focus([&] () {
        notify_event(core::WM_LAYOUT_WINDOW, client_area());
      });
      on_size([&] (const core::size& sz) {
        area.set_size(sz);
#ifndef BUILD_FOR_ARM
        notify_event(core::WM_LAYOUT_WINDOW, core::rectangle(sz));
#endif
      });
      on_move([&](const core::point& pt) {
        area.set_position(pt);
      });
      on_key_down<core::keys::tab>([&] () {
        shift_focus(false);
      });
      on_key_down<core::keys::tab, core::state::shift>([&] () {
        shift_focus(true);
      });
    }

    os::window overlapped_window::get_os_window () const {
      return detail::get_os_window(*this);
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
      set_state().overlapped(true);
      create_internal(cls, native::get_desktop_window(), native::adjust_overlapped_area(r, cls));
    }

    void overlapped_window::create (const class_info& cls,
                                    container& parent,
                                    const core::rectangle& r) {
      set_state().overlapped(true);
      create_internal(cls,
                      native::get_overlapped_parent(detail::get_os_window(parent)),
                      native::adjust_overlapped_area(r, cls));
      native::prepare_overlapped(get_os_window(), detail::get_os_window(parent));
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
      invalidate();
#endif // GUIPP_X11
#ifdef GUIPP_QT
      event_loop.exit();
#endif // GUIPP_QT
    }

    void modal_window::run_modal (window& parent) {
      run_modal(parent, {});
    }

    void modal_window::run_modal (window& parent, const std::vector<hot_key_action>& hot_keys) {
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
      invalidate();

      is_modal = true;

#ifdef GUIPP_X11
#if defined(USE_INPUT_EATER)
      input_only_window input_eater(*parent.get_overlapped_window());
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
    void popup_window::create (const class_info& cls, container& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_popup_window(detail::get_os_window(*this));
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, container& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      native::prepare_dialog_window(detail::get_os_window(*this), detail::get_os_window(parent));
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
