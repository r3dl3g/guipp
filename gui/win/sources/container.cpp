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

#ifdef X11
#include <X11/cursorfont.h>
#endif // X11
// --------------------------------------------------------------------------
//
// Library includes
//
#include "container.h"
#include "window_event_proc.h"
#include "window_event_handler.h"
#include "ostreamfmt.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------

#ifdef WIN32

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return is_valid() && child.is_valid() && IsChild(get_id(), child.get_id()) != FALSE;
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

#endif // WIN32

#ifdef X11

    extern bool check_xlib_return (int r);
    extern bool check_xlib_status (Status s);

    // --------------------------------------------------------------------------
    bool container::is_parent_of (const window& child) const {
      return child.get_parent() == this;
    }

    void container::set_children_visible (bool show) {
      if (show) {
        check_xlib_return(XMapSubwindows(core::global::get_instance(), get_id()));
      } else {
        check_xlib_return(XUnmapSubwindows(core::global::get_instance(), get_id()));
      }
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;

      if (is_visible()) {
        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (check_xlib_status(XQueryTree(core::global::get_instance(),
                                         get_id(),
                                         &root,
                                         &parent,
                                         &children,
                                         &nchildren))) {
          for (unsigned int n = 0; n < nchildren; ++n) {
            list.push_back(detail::get_window(children[n]));
          }
        }
      }
      return list;
    }

#endif // X11

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

    void container::init () {
      set_accept_focus(true);
      register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window* w){
        if (w == this) {
          forward_focus(shift_key_bit_mask::is_set(core::global::get_key_state()));
        }
      }));
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
      auto i = std::find(begin, end, focus);
      if (i != end) {
        do {
          ++i;
        } while ((i != end) && !(*i)->accept_focus());
        if (i != end) {
          (*i)->take_focus();
          return true;
        }
      }
      return false;
    }

    void container::shift_focus (window& focus, bool backward) {
      std::vector<window*> children = get_children();
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

    void container::forward_focus (bool backward) {
      std::vector<window*> children = get_children();
      if (children.size() > 0) {
        if (backward) {
          for (auto i = children.rbegin(), e = children.rend(); i != e; ++i) {
            window* next = *i;
            if (next->accept_focus()) {
              next->take_focus();
              return;
            }
          }
        } else {
          for (auto i = children.begin(), e = children.end(); i != e; ++i) {
            window* next = *i;
            if (next->accept_focus()) {
              next->take_focus();
              return;
            }
          }
        }
      }
      window::shift_focus(backward);
    }

    // --------------------------------------------------------------------------
#ifdef WIN32
    void overlapped_window::create (const window_class_info& type,
                                    const window& parent,
                                    const core::rectangle& r) {
      window::create(type, parent.get_id(), r);
    }

    void overlapped_window::create (const window_class_info& type,
                                    const core::rectangle& r) {
      window::create(type, GetDesktopWindow(), r);
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

    bool overlapped_window::is_top_most() const {
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
    }

    bool overlapped_window::is_minimized() const {
      return IsIconic(get_id()) != FALSE;
    }

    bool overlapped_window::is_maximized() const {
      return IsZoomed(get_id()) != FALSE;
    }

    void overlapped_window::minimize() {
      ShowWindow(get_id(), SW_MINIMIZE);
    }

    void overlapped_window::maximize() {
      ShowWindow(get_id(), SW_MAXIMIZE);
    }

    void overlapped_window::restore() {
      ShowWindow(get_id(), SW_RESTORE);
    }

    void overlapped_window::set_top_most(bool toplevel) {
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
#endif // WIN32

#ifdef X11
    void overlapped_window::create (const window_class_info& clazz,
                                    const core::rectangle& r) {
      super::create(clazz, DefaultRootWindow(core::global::get_instance()), r);
    }

    void overlapped_window::create (const window_class_info& clazz,
                                    const window& parent,
                                    const core::rectangle& r) {
      os::instance display = core::global::get_instance();
      super::create(clazz, DefaultRootWindow(display), r);
      XSetTransientForHint(display, get_id(), parent.get_id());
    }

    void overlapped_window::set_title (const std::string& title) {
      check_xlib_status(XStoreName(core::global::get_instance(), get_id(), title.c_str()));
    }

    std::string overlapped_window::get_title () const {
      char *window_name;
      check_xlib_status(XFetchName(core::global::get_instance(), get_id(), &window_name));
      return std::string(window_name);
    }

    Atom ATOM_ATOM = 0;
    Atom NET_WM_STATE = 0;
    Atom NET_WM_STATE_MAXIMIZED_HORZ = 0;
    Atom NET_WM_STATE_MAXIMIZED_VERT = 0;
    Atom NET_WM_STATE_ABOVE = 0;
    Atom NET_WM_STATE_HIDDEN = 0;

#ifndef _NET_WM_STATE_REMOVE
# define _NET_WM_STATE_REMOVE        0    /* remove/unset property */
#endif
#ifndef _NET_WM_STATE_ADD
# define _NET_WM_STATE_ADD           1    /* add/set property */
#endif
#ifndef _NET_WM_STATE_TOGGLE
# define _NET_WM_STATE_TOGGLE        2    /* toggle property  */
#endif

    void init_for_net_wm_state () {
      auto dpy = core::global::get_instance();

      detail::init_atom(NET_WM_STATE, "_NET_WM_STATE");
      detail::init_atom(NET_WM_STATE_MAXIMIZED_HORZ, "_NET_WM_STATE_MAXIMIZED_HORZ");
      detail::init_atom(NET_WM_STATE_MAXIMIZED_VERT, "_NET_WM_STATE_MAXIMIZED_VERT");
      detail::init_atom(NET_WM_STATE_ABOVE, "_NET_WM_STATE_ABOVE");
      detail::init_atom(NET_WM_STATE_HIDDEN, "_NET_WM_STATE_HIDDEN");
      detail::init_atom(ATOM_ATOM, "ATOM");
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

    bool overlapped_window::is_maximized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_MAXIMIZED_HORZ, NET_WM_STATE_MAXIMIZED_VERT);
    }

    bool overlapped_window::is_top_most () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_ABOVE);
    }

    bool overlapped_window::is_minimized () const {
      init_for_net_wm_state();
      return query_net_wm_state(get_id(), NET_WM_STATE_HIDDEN);
    }

    void overlapped_window::minimize () {
      XIconifyWindow(core::global::get_instance(), get_id(), core::global::get_screen());
    }

    void send_net_wm_state (os::window id,
                            long action,
                            Atom a1,
                            Atom a2 = 0,
                            Atom a3 = 0) {
      auto dpy = core::global::get_instance();

      XEvent xev;
      memset(&xev, 0, sizeof(xev));
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

    void overlapped_window::maximize () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_ADD,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);

    }

    void overlapped_window::restore () {
      init_for_net_wm_state();
      send_net_wm_state(get_id(), _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_MAXIMIZED_HORZ,
                        NET_WM_STATE_MAXIMIZED_VERT);
    }

    void overlapped_window::set_top_most (bool toplevel) {
      init_for_net_wm_state();
      send_net_wm_state(get_id(),
                        toplevel ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                        NET_WM_STATE_ABOVE);
    }

#endif //X11

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
#ifdef WIN32
      register_event_handler(REGISTER_FUNCTION, close_event([&]() {
        is_modal = false;
      }));
#endif // WIN32
    }

    void modal_window::end_modal () {
      is_modal = false;
#ifdef X11
      redraw_now();
#endif // X11
    }

#ifdef X11
    bool is_deeper_window (os::window main, os::window sub) {
      if (sub == main) {
        return false;
      }

      Window root = 0, parent = 0;
      Window *children = 0;
      unsigned int nchildren = 0;

      XQueryTree(core::global::get_instance(), sub, &root, &parent, &children, &nchildren);
      if (parent) {
        return is_deeper_window(main, parent);
      }
      return true;
    }
#endif // X11

    void modal_window::run_modal () {
      LogDebug << "Enter modal loop";

      os::window win = get_id();

      redraw_later();

      is_modal = true;

      run_loop(is_modal, [&, win](const core::event& e) -> bool {
#ifdef X11
        switch (e.type) {
          case MotionNotify:
          case ButtonPress:
          case ButtonRelease:
          case FocusIn:
          case FocusOut:
          case EnterNotify:
          case LeaveNotify:
            return is_deeper_window(win, e.xany.window);
          break;
        }
#endif // X11

#ifdef WIN32
        if (e.type == WM_HOTKEY) {
#endif // WIN32
#ifdef X11
        if ((e.type == KeyPress) && !is_deeper_window(win, e.xany.window)) {
#endif // X11
          return check_hot_key(e);
        }
        return false;
      });

      LogDebug << "Exit modal loop";
    }

    // --------------------------------------------------------------------------
    void main_window::create (const core::rectangle& r) {
      super::create(clazz::get(), r);
#ifdef X11
      os::instance display = core::global::get_instance();
      Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
      Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NORMAL", False);
      XChangeProperty(display, get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

      Atom protocols[] = {
        XInternAtom(display, "WM_TAKE_FOCUS", False),
        detail::WM_DELETE_WINDOW,
      };
      XSetWMProtocols(display, get_id(), protocols, 2);
      XWMHints* hints = XGetWMHints(display, get_id());
      if (!hints) {
        hints = XAllocWMHints();
      }
      hints->flags |= InputHint;
      hints->input = True;
      XSetWMHints(display, get_id(), hints);
#endif
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const window& parent, const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
#ifdef X11
      os::instance display = core::global::get_instance();
      Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
      Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU", False);
      XChangeProperty(display, get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

      XSetWindowAttributes wa;
      wa.override_redirect = 1;
      XChangeWindowAttributes(display, get_id(), CWOverrideRedirect, &wa);
#endif
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const window& parent, const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
#ifdef X11
      os::instance display = core::global::get_instance();
      Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
      Atom value = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DIALOG", False);
      XChangeProperty(display, get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

      type = XInternAtom(display, "_NET_WM_STATE", False);
      value = XInternAtom(display, "_NET_WM_STATE_MODAL", False),
        XChangeProperty(display, get_id(), type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);

      type = XInternAtom(display, "WM_CLIENT_LEADER", False);
      XChangeProperty(display, get_id(), type, XA_WINDOW, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&parent_id), 1);
#endif
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
