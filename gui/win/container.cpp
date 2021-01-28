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

#ifdef GUIPP_X11
# include <X11/cursorfont.h>
#endif // GUIPP_X11
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

#if !defined(GUIPP_BUILD_FOR_MOBILE)
# define USE_INPUT_EATER
#endif

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

    namespace x11 {

#ifdef GUIPP_X11

# ifndef _NET_WM_STATE_REMOVE
#  define _NET_WM_STATE_REMOVE        0   /* remove/unset property */
# endif
# ifndef _NET_WM_STATE_ADD
#  define _NET_WM_STATE_ADD           1   /* add/set property */
# endif
# ifndef _NET_WM_STATE_TOGGLE
#  define _NET_WM_STATE_TOGGLE        2   /* toggle property  */
# endif

      Atom ATOM_ATOM = 0;
      Atom NET_WM_STATE = 0;
      Atom NET_WM_STATE_MAXIMIZED_HORZ = 0;
      Atom NET_WM_STATE_MAXIMIZED_VERT = 0;
      Atom NET_WM_STATE_ABOVE = 0;
      Atom NET_WM_STATE_HIDDEN = 0;
      Atom WM_SIZE_HINTS = 0;

      int init_for_net_wm_state () {
        core::x11::init_atom(NET_WM_STATE, "_NET_WM_STATE");
        core::x11::init_atom(NET_WM_STATE_MAXIMIZED_HORZ, "_NET_WM_STATE_MAXIMIZED_HORZ");
        core::x11::init_atom(NET_WM_STATE_MAXIMIZED_VERT, "_NET_WM_STATE_MAXIMIZED_VERT");
        core::x11::init_atom(NET_WM_STATE_ABOVE, "_NET_WM_STATE_ABOVE");
        core::x11::init_atom(NET_WM_STATE_HIDDEN, "_NET_WM_STATE_HIDDEN");
        core::x11::init_atom(ATOM_ATOM, "ATOM");
        core::x11::init_atom(WM_SIZE_HINTS, "WM_SIZE_HINTS");
        return 1;
      }

      bool check_return (int r);
      bool check_status (Status s);

      // --------------------------------------------------------------------------
      template<typename T>
      void change_property (os::instance display, os::window id, const char* type, T value);

      template<>
      void change_property<const char*>(os::instance display, os::window id, const char* type, const char* value) {
        Atom t = XInternAtom(display, type, False);
        Atom v = XInternAtom(display, value, False);
        XChangeProperty(display, id, t, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&v), 1);
      }

      template<>
      void change_property<os::window>(os::instance display, os::window id, const char* type, os::window value) {
        Atom t = XInternAtom(display, type, False);
        XChangeProperty(display, id, t, XA_WINDOW, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);
      }

      std::string get_property (os::instance display, os::window id, const char* name) {
        Atom prop_name = XInternAtom(display, name, False);
        Atom actual_type;
        int actual_format;
        unsigned long nitems, bytes_after;
        unsigned char *data = 0;
        std::string str;
        if ((Success == XGetWindowProperty(display, id, prop_name, 0, 1024, false,
                                           XA_ATOM, &actual_type, &actual_format,
                                           &nitems, &bytes_after, &data))
            && (nitems > 0)) {
          if (actual_type == XA_ATOM) {
            str = XGetAtomName(display, *(Atom*)data);
          }
          XFree(data);
        }
        return str;
      }

      void set_wm_protocols (os::instance display, os::window id) {
        Atom protocols[] = {
          core::x11::WM_TAKE_FOCUS,
          core::x11::WM_DELETE_WINDOW,
        };
        XSetWMProtocols(display, id, protocols, 2);
      }

      bool query_net_wm_state (os::window id,
                                 Atom a1,
                                 Atom a2 = 0,
                                 Atom a3 = 0) {
        auto dpy = core::global::get_instance();

        Atom actual_type_return;
        int actual_format_return;
        unsigned long nitems_return;
        unsigned long bytes_after_return;
        unsigned char *prop_return;

        bool ret_a1 = a1 == 0;
        bool ret_a2 = a2 == 0;
        bool ret_a3 = a3 == 0;
        if (Success == XGetWindowProperty(dpy, id, NET_WM_STATE,
                                          0, 99, false, AnyPropertyType,
                                          &actual_type_return,
                                          &actual_format_return,
                                          &nitems_return,
                                          &bytes_after_return,
                                          &prop_return)) {
          if (actual_type_return == ATOM_ATOM) {
            Atom* atoms = (Atom*)prop_return;
            for (unsigned long i = 0; i < nitems_return; ++i) {
              ret_a1 |= (atoms[i] == a1);
              ret_a2 |= (atoms[i] == a2);
              ret_a3 |= (atoms[i] == a3);
            }
          }
        }
        return ret_a1 && ret_a2 && ret_a3;
      }

      void send_net_wm_state (os::window id,
                                long action,
                                Atom a1,
                                Atom a2 = 0,
                                Atom a3 = 0) {
        auto dpy = core::global::get_instance();

        XEvent xev;
        memset(&xev, 0, sizeof (xev));
        xev.type = ClientMessage;
        xev.xclient.window = id;
        xev.xclient.message_type = NET_WM_STATE;
        xev.xclient.format = 32;
        xev.xclient.data.l[0] = action;
        xev.xclient.data.l[1] = a1;
        xev.xclient.data.l[2] = a2;
        xev.xclient.data.l[3] = a3;

        XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureNotifyMask, &xev);
      }

#endif // GUIPP_X11

    }

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
#ifdef GUIPP_X11
      static int initialized = x11::init_for_net_wm_state();
      (void)initialized;
#endif // GUIPP_X11

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

#ifdef GUIPP_WIN
    void overlapped_window::create (const class_info& type,
                                    container& parent,
                                    const core::rectangle& r) {
      auto rect = r.os();
      AdjustWindowRectEx(&rect, type.get_style(), FALSE, type.get_ex_style());
      window::create_internal(type, detail::get_os_window(parent), core::rectangle(rect));
    }

    void overlapped_window::create (const class_info& type,
                                    const core::rectangle& r) {
      window::create_internal(type, GetDesktopWindow(), r);
    }

    void overlapped_window::set_title (const std::string& title) {
      SendMessage(get_os_window(), WM_SETTEXT, 0, (LPARAM)title.c_str());
    }

    std::string overlapped_window::get_title () const {
      std::string s;
      s.resize(SendMessage(get_os_window(), WM_GETTEXTLENGTH, 0, 0) + 1);
      SendMessage(get_os_window(), WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
      return s;
    }

    bool overlapped_window::is_top_most () const {
      return (GetWindowLong(get_os_window(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool overlapped_window::is_minimized () const {
      return IsIconic(get_os_window()) != FALSE;
    }

    bool overlapped_window::is_maximized () const {
      return IsZoomed(get_os_window()) != FALSE;
    }

    void overlapped_window::minimize () {
      ShowWindow(get_os_window(), SW_MINIMIZE);
    }

    void overlapped_window::maximize () {
      ShowWindow(get_os_window(), SW_MAXIMIZE);
    }

    void overlapped_window::restore () {
      ShowWindow(get_os_window(), SW_RESTORE);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      SetWindowPos(get_os_window(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

#endif // GUIPP_WIN

#ifdef GUIPP_X11

    void overlapped_window::create (const class_info& cls,
                                    const core::rectangle& r) {
      create_internal(cls, DefaultRootWindow(core::global::get_instance()), r);
      set_state().overlapped(true);
    }

    void overlapped_window::create (const class_info& cls,
                                    container& parent,
                                    const core::rectangle& r) {
      gui::os::instance display = core::global::get_instance();
      create_internal(cls, DefaultRootWindow(display), r);
      set_state().overlapped(true);
      XSetTransientForHint(display, get_os_window(), detail::get_os_window(parent));
    }

    void overlapped_window::set_title (const std::string& title) {
      x11::check_status(XStoreName(core::global::get_instance(), get_os_window(), title.c_str()));
    }

    std::string overlapped_window::get_title () const {
      char *window_name;
      x11::check_status(XFetchName(core::global::get_instance(), get_os_window(), &window_name));
      return std::string(window_name);
    }

    bool overlapped_window::is_maximized () const {
      return x11::query_net_wm_state(get_os_window(), x11::NET_WM_STATE_MAXIMIZED_HORZ, x11::NET_WM_STATE_MAXIMIZED_VERT);
    }

    bool overlapped_window::is_top_most () const {
      return x11::query_net_wm_state(get_os_window(), x11::NET_WM_STATE_ABOVE);
    }

    bool overlapped_window::is_minimized () const {
      return x11::query_net_wm_state(get_os_window(), x11::NET_WM_STATE_HIDDEN);
    }

    void overlapped_window::minimize () {
      XIconifyWindow(core::global::get_instance(), get_os_window(), core::global::x11::get_screen());
    }

    void overlapped_window::maximize () {
      x11::send_net_wm_state(get_os_window(), _NET_WM_STATE_ADD,
                             x11::NET_WM_STATE_MAXIMIZED_HORZ,
                             x11::NET_WM_STATE_MAXIMIZED_VERT);

    }

    void overlapped_window::restore () {
      x11::send_net_wm_state(get_os_window(), _NET_WM_STATE_REMOVE,
                             x11::NET_WM_STATE_MAXIMIZED_HORZ,
                             x11::NET_WM_STATE_MAXIMIZED_VERT);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      x11::send_net_wm_state(get_os_window(),
                             toplevel ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                             x11::NET_WM_STATE_ABOVE);
    }

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

#ifdef GUIPP_QT
    void overlapped_window::create (const class_info& type,
                                    container& parent,
                                    const core::rectangle& r) {
      if (parent.is_valid()) {
        super::create(type, parent, r);
      } else {
        create(type, r);
      }
    }

    void overlapped_window::create (const class_info& type,
                                    const core::rectangle& r) {
      window::create_internal(type, nullptr, r);
    }

    void overlapped_window::set_title (const std::string& title) {
      if (is_valid()) {
        get_os_window()->setWindowTitle(QString::fromStdString(title));
      }
    }

    std::string overlapped_window::get_title () const {
      if (is_valid()) {
        return get_os_window()->windowTitle().toStdString();
      }
      return {};
    }

    bool overlapped_window::is_top_most () const {
      return is_valid() && ((get_os_window()->windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint);
    }

    bool overlapped_window::is_minimized () const {
      return is_valid() && get_os_window()->isMinimized();
    }

    bool overlapped_window::is_maximized () const {
      return is_valid() && get_os_window()->isMaximized();
    }

    void overlapped_window::minimize () {
      if (is_valid()) {
        get_os_window()->showMinimized();
      }
    }

    void overlapped_window::maximize () {
      if (is_valid()) {
        get_os_window()->showMaximized();
      }
    }

    void overlapped_window::restore () {
      if (is_valid()) {
        get_os_window()->showNormal();
      }
    }

    void overlapped_window::set_top_most (bool toplevel) {
      if (is_valid()) {
        get_os_window()->setWindowFlag(Qt::WindowStaysOnTopHint, toplevel);
      }
    }
#endif // GUIPP_QT

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
#ifdef GUIPP_X11
      gui::os::instance display = core::global::get_instance();

      os::window id = detail::get_os_window(*this);

      x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_NORMAL");
      x11::set_wm_protocols(display, id);

      XWMHints* hints = XGetWMHints(display, id);
      if (!hints) {
        hints = XAllocWMHints();
      }
      hints->flags |= InputHint;
      hints->input = True;
      XSetWMHints(display, id, hints);
      XFree(hints);
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const class_info& cls, container& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
#ifdef GUIPP_X11
      gui::os::instance display = core::global::get_instance();

      os::window id = detail::get_os_window(*this);

      x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU");

      XSetWindowAttributes wa;
      wa.override_redirect = 1;
      XChangeWindowAttributes(display, id, CWOverrideRedirect, &wa);
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, container& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
#ifdef GUIPP_X11
      gui::os::instance display = core::global::get_instance();
      auto id = detail::get_os_window(*this);
      x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DIALOG");
      x11::change_property(display, id, "_NET_WM_STATE", "_NET_WM_STATE_MODAL");
      x11::change_property(display, id, "WM_CLIENT_LEADER", detail::get_os_window(parent));
      x11::set_wm_protocols(display, id);
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
