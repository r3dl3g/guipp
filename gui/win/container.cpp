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
#include <string.h>

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

#ifdef GUIPP_WIN

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return is_valid() && child.is_valid() && IsChild(get_id(), detail::get_window_id(child)) != FALSE;
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;
      os::window id = GetWindow(get_id(), GW_CHILD);
      while (id) {
        list.push_back(detail::get_window(id));
        id = GetWindow(id, GW_HWNDNEXT);
      }
      return list;
    }

    void container::set_children_visible (bool show) {
      std::vector<window*> children = get_children();
      for (window* win : children) {
        win->set_visible(show);
      }
    }

#endif // GUIPP_WIN

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

    namespace x11 {

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

    }

    namespace x11 {
      bool check_return (int r);
      bool check_status (Status s);
    }

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    void collect_children_deep (container::window_list_t& list, const window& win) {
      if (win.is_valid()) {
        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (x11::check_status(XQueryTree(core::global::get_instance(),
                                         detail::get_window_id(win),
                                         &root,
                                         &parent,
                                         &children,
                                         &nchildren))) {
          for (unsigned int n = 0; n < nchildren; ++n) {
            window* sub = detail::get_window(children[n]);
            if (sub) {
              list.push_back(sub);
              collect_children_deep(list, *sub);
            }
          }
        }
        if (children) {
          XFree(children);
        }
      }
    }

    container::window_list_t get_deep_children (const window& win) {
      container::window_list_t list;
      collect_children_deep(list, win);
      return list;
    }

    void container::set_children_visible (bool show) {
      if (show) {
        x11::check_return(XMapSubwindows(core::global::get_instance(), get_id()));
        for(window* win : get_deep_children(*this)) {
          win->set_state().visible(true);
        }
      } else {
        x11::check_return(XUnmapSubwindows(core::global::get_instance(), get_id()));
        for(window* win : get_deep_children(*this)) {
          win->set_state().visible(false);
        }
      }
    }

    container::window_list_t container::get_children () const {
      window_list_t list;

      if (is_valid()) {
        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (x11::check_status(XQueryTree(core::global::get_instance(),
                                         get_id(),
                                         &root,
                                         &parent,
                                         &children,
                                         &nchildren))) {
          for (unsigned int n = 0; n < nchildren; ++n) {
            window* sub = detail::get_window(children[n]);
            if (sub) {
              list.push_back(sub);
            }
          }
        }
        if (children) {
          XFree(children);
        }
      }
      return list;
    }

#endif // GUIPP_X11

#ifdef GUIPP_QT

    bool container::is_parent_of (const window& child) const {
      return is_valid() && child.is_valid() && (get_id() == detail::get_window_id(child)->get_parent());
    }

    container::window_list_t container::get_children () const {
      container::window_list_t list;
      if (is_valid()) {
        for (auto child :get_id()->children()) {
          list.push_back(detail::get_window(static_cast<os::window>(child)));
        }
      }
      return list;
    }

    void get_deep_children (os::window win, std::vector<os::window>& list) {
      for (auto child : win->children()) {
        os::window w = dynamic_cast<os::window>(child);
        if (w) {
          list.push_back(w);
          get_deep_children(w, list);
        }
      }
    }

    void container::set_children_visible (bool show) {
      std::vector<os::window> children;
      get_deep_children(get_id(), children);
      for (os::window child : children) {
        child->setVisible(show);
      }
    }

#endif // GUIPP_QT

    container::container () {
      init();
    }

    container::container (const container& rhs)
      : super(rhs) {
      init();
    }

    container::container (container&& rhs)
      : super(std::move(rhs)) {
      init();
    }

    os::window container::get_id () const {
      return detail::get_window_id(*this);
    }

    void container::init () {
#ifdef GUIPP_X11
      static int initialized = x11::init_for_net_wm_state();
      (void)initialized;
#endif // GUIPP_X11

      set_accept_focus(true);
      on_set_focus([&] () {
        forward_focus(shift_key_bit_mask::is_set(core::global::get_key_state()));
      });
      on_show([&] () {
        set_children_visible();
      });
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

    template<typename iterator>
    bool iterate_focus (iterator begin, iterator end, const window* focus) {
      const auto start = std::find(begin, end, focus);
      auto i = start;
      if (i != end) {
        ++i;
        auto count = std::distance(begin, end);
        while (count) {
          if (i == end) {
            i = begin;
          }
          if ((*i)->can_accept_focus()) {
            (*i)->take_focus();
            return true;
          }
          ++i;
          --count;
        }
      }
      return false;
    }

    void container::shift_focus (const window& focus, bool backward) const {
      window_list_t children = get_children();
      if (children.size() > 0) {
        if (backward) {
          if (iterate_focus(children.rbegin(), children.rend(), &focus)) {
            return;
          }
        } else {
          if (iterate_focus(children.begin(), children.end(), &focus)) {
            return;
          }
        }
      }
      auto parent = get_parent();
      if (parent) {
        parent->shift_focus(*this, backward);
      } else {
        forward_focus(backward);
      }
    }

    void container::forward_focus (bool backward) const {
      window::shift_focus(backward);
    }

    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window () {
      on_set_focus([&] () {
        send_client_message(this, core::WM_LAYOUT_WINDOW, client_area());
      });
#ifndef BUILD_FOR_ARM
      on_size([&] (const core::size& sz) {
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(sz));
      });
#endif
    }

    os::window overlapped_window::get_id () const {
      return detail::get_window_id(*this);
    }

#ifdef GUIPP_WIN
    void overlapped_window::create (const class_info& type,
                                    container& parent,
                                    const core::rectangle& r) {
      auto rect = r.os();
      AdjustWindowRectEx(&rect, type.get_style(), FALSE, type.get_ex_style());
      window::create_internal(type, detail::get_window_id(parent), core::rectangle(rect));
    }

    void overlapped_window::create (const class_info& type,
                                    const core::rectangle& r) {
      window::create_internal(type, GetDesktopWindow(), r);
    }

    void overlapped_window::set_title (const std::string& title) {
      SendMessage(get_id(), WM_SETTEXT, 0, (LPARAM)title.c_str());
    }

    std::string overlapped_window::get_title () const {
      std::string s;
      s.resize(SendMessage(get_id(), WM_GETTEXTLENGTH, 0, 0) + 1);
      SendMessage(get_id(), WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
      return s;
    }

    bool overlapped_window::is_top_most () const {
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool overlapped_window::is_minimized () const {
      return IsIconic(get_id()) != FALSE;
    }

    bool overlapped_window::is_maximized () const {
      return IsZoomed(get_id()) != FALSE;
    }

    void overlapped_window::minimize () {
      ShowWindow(get_id(), SW_MINIMIZE);
    }

    void overlapped_window::maximize () {
      ShowWindow(get_id(), SW_MAXIMIZE);
    }

    void overlapped_window::restore () {
      ShowWindow(get_id(), SW_RESTORE);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

#endif // GUIPP_WIN

#ifdef GUIPP_X11
    // --------------------------------------------------------------------------
    template<typename T>
    void change_property (gui::os::instance display, os::window id, const char* type, T value);

    template<>
    void change_property<const char*>(gui::os::instance display, os::window id, const char* type, const char* value) {
      Atom t = XInternAtom(display, type, False);
      Atom v = XInternAtom(display, value, False);
      XChangeProperty(display, id, t, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&v), 1);
    }

    template<>
    void change_property<os::window>(gui::os::instance display, os::window id, const char* type, os::window value) {
      Atom t = XInternAtom(display, type, False);
      XChangeProperty(display, id, t, XA_WINDOW, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);
    }

    std::string get_property (gui::os::instance display, os::window id, const char* name) {
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

    void set_wm_protocols (gui::os::instance display, os::window id) {
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

      bool ret_a1 = (a1 ? false : true);
      bool ret_a2 = (a2 ? false : true);
      bool ret_a3 = (a3 ? false : true);
      if (Success == XGetWindowProperty(dpy, id, x11::NET_WM_STATE,
                                        0, 99, false, AnyPropertyType,
                                        &actual_type_return,
                                        &actual_format_return,
                                        &nitems_return,
                                        &bytes_after_return,
                                        &prop_return)) {
        if (actual_type_return == x11::ATOM_ATOM) {
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
      xev.xclient.message_type = x11::NET_WM_STATE;
      xev.xclient.format = 32;
      xev.xclient.data.l[0] = action;
      xev.xclient.data.l[1] = a1;
      xev.xclient.data.l[2] = a2;
      xev.xclient.data.l[3] = a3;

      XSendEvent(dpy, DefaultRootWindow(dpy), False, SubstructureNotifyMask, &xev);
    }

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
      XSetTransientForHint(display, get_id(), detail::get_window_id(parent));
    }

    void overlapped_window::set_title (const std::string& title) {
      x11::check_status(XStoreName(core::global::get_instance(), get_id(), title.c_str()));
    }

    std::string overlapped_window::get_title () const {
      char *window_name;
      x11::check_status(XFetchName(core::global::get_instance(), get_id(), &window_name));
      return std::string(window_name);
    }

    bool overlapped_window::is_maximized () const {
      return query_net_wm_state(get_id(), x11::NET_WM_STATE_MAXIMIZED_HORZ, x11::NET_WM_STATE_MAXIMIZED_VERT);
    }

    bool overlapped_window::is_top_most () const {
      return query_net_wm_state(get_id(), x11::NET_WM_STATE_ABOVE);
    }

    bool overlapped_window::is_minimized () const {
      return query_net_wm_state(get_id(), x11::NET_WM_STATE_HIDDEN);
    }

    void overlapped_window::minimize () {
      XIconifyWindow(core::global::get_instance(), get_id(), core::global::x11::get_screen());
    }

    void overlapped_window::maximize () {
      send_net_wm_state(get_id(), _NET_WM_STATE_ADD,
                        x11::NET_WM_STATE_MAXIMIZED_HORZ,
                        x11::NET_WM_STATE_MAXIMIZED_VERT);

    }

    void overlapped_window::restore () {
      send_net_wm_state(get_id(), _NET_WM_STATE_REMOVE,
                        x11::NET_WM_STATE_MAXIMIZED_HORZ,
                        x11::NET_WM_STATE_MAXIMIZED_VERT);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      send_net_wm_state(get_id(),
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
      input_only_window (container& parent) {
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
        get_id()->setWindowTitle(QString::fromStdString(title));
      }
    }

    std::string overlapped_window::get_title () const {
      if (is_valid()) {
        return get_id()->windowTitle().toStdString();
      }
      return {};
    }

    bool overlapped_window::is_top_most () const {
      return is_valid() && ((get_id()->windowFlags() & Qt::WindowStaysOnTopHint) == Qt::WindowStaysOnTopHint);
    }

    bool overlapped_window::is_minimized () const {
      return is_valid() && get_id()->isMinimized();
    }

    bool overlapped_window::is_maximized () const {
      return is_valid() && get_id()->isMaximized();
    }

    void overlapped_window::minimize () {
      if (is_valid()) {
        get_id()->showMinimized();
      }
    }

    void overlapped_window::maximize () {
      if (is_valid()) {
        get_id()->showMaximized();
      }
    }

    void overlapped_window::restore () {
      if (is_valid()) {
        get_id()->showNormal();
      }
    }

    void overlapped_window::set_top_most (bool toplevel) {
      if (is_valid()) {
        get_id()->setWindowFlag(Qt::WindowStaysOnTopHint, toplevel);
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

    modal_window::modal_window (modal_window&& rhs)
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
      detail::get_window_id(*this)->setWindowModality(Qt::WindowModality::ApplicationModal);
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
            hot_key hk(get_key_symbol(e), get_key_state(e));
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

      os::window id = detail::get_window_id(*this);

      change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_NORMAL");
      set_wm_protocols(display, id);

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

      os::window id = detail::get_window_id(*this);

      change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU");

      XSetWindowAttributes wa;
      wa.override_redirect = 1;
      XChangeWindowAttributes(display, id, CWOverrideRedirect, &wa);
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, container& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
      gui::os::instance display = core::global::get_instance();
#ifdef GUIPP_X11
      auto id = detail::get_window_id(*this);
      change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DIALOG");
      change_property(display, id, "_NET_WM_STATE", "_NET_WM_STATE_MODAL");
      change_property(display, id, "WM_CLIENT_LEADER", detail::get_window_id(parent));
      set_wm_protocols(display, id);
#endif // GUIPP_X11
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
