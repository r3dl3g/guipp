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
# include <X11/cursorfont.h>
#endif // X11


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <gui/win/container.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>


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

    void collect_children_deep (std::vector<window*>& list, const window& win) {
      if (win.is_valid()) {
        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (x11::check_status(XQueryTree(core::global::get_instance(),
                                         win.get_id(),
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
      }
    }

    std::vector<window*> get_deep_children (const window& win) {
      std::vector<window*> list;
      collect_children_deep(list, win);
      return list;
    }

    void container::set_children_visible (bool show) {
      if (show) {
        x11::check_return(XMapSubwindows(core::global::get_instance(), get_id()));
        for(window* win : get_deep_children(*this)) {
          win->get_state().set_visible(true);
        }
      } else {
        x11::check_return(XUnmapSubwindows(core::global::get_instance(), get_id()));
        for(window* win : get_deep_children(*this)) {
          win->get_state().set_visible(false);
        }
      }
    }

    std::vector<window*> container::get_children () const {
      std::vector<window*> list;

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
#ifdef X11
      static int initialized = x11::init_for_net_wm_state();
      (void)initialized;
#endif // X11

      set_accept_focus(true);
      on_set_focus([&](window* w){
        if (w == this) {
          forward_focus(shift_key_bit_mask::is_set(core::global::get_key_state()));
        }
      });
      on_show([&]() {
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
      auto i = std::find(begin, end, focus);
      if (i != end) {
        do {
          ++i;
        } while ((i != end) && !(*i)->can_accept_focus());
        if (i != end) {
          (*i)->take_focus();
          return true;
        }
      }
      return false;
    }

    void container::shift_focus (const window& focus, bool backward) const {
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

    void container::forward_focus (bool backward) const {
      std::vector<window*> children = get_children();
      if (children.size() > 0) {
        if (backward) {
          for (auto i = children.rbegin(), e = children.rend(); i != e; ++i) {
            window* next = *i;
            if (next->can_accept_focus()) {
              next->take_focus();
              return;
            }
          }
        } else {
          for (auto i = children.begin(), e = children.end(); i != e; ++i) {
            window* next = *i;
            if (next->can_accept_focus()) {
              next->take_focus();
              return;
            }
          }
        }
      }
      window::shift_focus(backward);
    }

    // --------------------------------------------------------------------------
    overlapped_window::overlapped_window () {
      on_set_focus([&] (win::window* prev) {
        send_client_message(this, core::WM_LAYOUT_WINDOW, client_area());
      });
#ifndef BUILD_FOR_ARM
      on_size([&] (const core::size& sz) {
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(sz));
      });
#endif
    }

#ifdef WIN32
    void overlapped_window::create (const class_info& type,
                                    const window& parent,
                                    const core::rectangle& r) {
      window::create(type, parent.get_id(), r);
    }

    void overlapped_window::create (const class_info& type,
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

#endif // WIN32

#ifdef X11
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
      super::create(cls, DefaultRootWindow(core::global::get_instance()), r);
      get_state().set_overlapped(true);
    }

    void overlapped_window::create (const class_info& cls,
                                    const window& parent,
                                    const core::rectangle& r) {
      gui::os::instance display = core::global::get_instance();
      super::create(cls, DefaultRootWindow(display), r);
      get_state().set_overlapped(true);
      XSetTransientForHint(display, get_id(), parent.get_id());
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
      container& parent;
//      std::string old_value;

    public:
      input_only_window (container& parent)
        : parent(parent)
      {
        super::create(clazz::get(), parent, parent.client_area());
        to_front();
//        auto dpy = core::global::get_instance();
//        old_value = get_property(dpy, parent.get_id(), "_NET_WM_WINDOW_TYPE");
//        change_property(dpy, parent.get_id(), "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_UTILITY");
      }

//      ~input_only_window () {
//        change_property(core::global::get_instance(), parent.get_id(), "_NET_WM_WINDOW_TYPE", old_value.c_str());
//      }
    };


#endif //X11
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
#ifdef WIN32
      on_close([&]() {
        is_modal = false;
      });
#endif // WIN32
    }

    void modal_window::end_modal () {
      is_modal = false;
#ifdef X11
      invalidate();
#endif // X11
    }

    void modal_window::run_modal (const window& parent) {
      run_modal(parent, {});
    }

    void modal_window::run_modal (const window& parent, const std::vector<hot_key_action>& hot_keys) {
      LogTrace << *this << " Enter modal loop";

      invalidate();

      is_modal = true;

#ifdef X11
      input_only_window input_eater(*parent.get_overlapped_window());
      input_eater.set_visible();

      run_loop(is_modal, [&](const core::event & e)->bool {
#endif // X11

#ifdef WIN32
        run_loop(is_modal, [&](const core::event & e)->bool {
          if (e.type == WM_KEYDOWN) {
#endif // WIN32
#ifdef X11
        if (e.type == KeyPress) {
#endif // X11011
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

      input_eater.set_visible(false);

      LogTrace << *this << " Exit modal loop";
    }

    // --------------------------------------------------------------------------
    void main_window::create (const class_info& cls, const core::rectangle& r) {
      super::create(cls, r);
#ifdef X11
      gui::os::instance display = core::global::get_instance();

      change_property(display, get_id(), "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_NORMAL");
      set_wm_protocols(display, get_id());

      XWMHints* hints = XGetWMHints(display, get_id());
      if (!hints) {
        hints = XAllocWMHints();
      }
      hints->flags |= InputHint;
      hints->input = True;
      XSetWMHints(display, get_id(), hints);
#endif // X11
    }

    // --------------------------------------------------------------------------
    void popup_window::create (const class_info& cls, const window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
#ifdef X11
      gui::os::instance display = core::global::get_instance();

      change_property(display, get_id(), "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU");

      XSetWindowAttributes wa;
      wa.override_redirect = 1;
      XChangeWindowAttributes(display, get_id(), CWOverrideRedirect, &wa);
#endif // X11
    }

    // --------------------------------------------------------------------------
    void dialog_window::create (const class_info& cls, const window& parent, const core::rectangle& r) {
      super::create(cls, parent, r);
#ifdef X11
      gui::os::instance display = core::global::get_instance();
      change_property(display, get_id(), "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DIALOG");
      change_property(display, get_id(), "_NET_WM_STATE", "_NET_WM_STATE_MODAL");
      change_property(display, get_id(), "WM_CLIENT_LEADER", parent.get_id());
      set_wm_protocols(display, get_id());
#endif // X11
    }

    // --------------------------------------------------------------------------

  } // win

} // gui
