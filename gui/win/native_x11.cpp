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


#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>
#include <X11/cursorfont.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/win/native.h>
#include <gui/win/window.h>


namespace gui {

  namespace win {

    namespace x11 {

# define XLIB_ERROR_CODE(a) case a: clog::fatal() << # a;break;

      bool check_return (int r) {
        //# ifndef NDEBUG
        //        core::global::sync();
        //# endif
        switch (r) {
          case Success:
          case True:
            return true;
            XLIB_ERROR_CODE(BadValue)
                XLIB_ERROR_CODE(BadWindow)
                XLIB_ERROR_CODE(BadPixmap)
                XLIB_ERROR_CODE(BadAtom)
                XLIB_ERROR_CODE(BadCursor)
                XLIB_ERROR_CODE(BadFont)
                XLIB_ERROR_CODE(BadMatch)
                XLIB_ERROR_CODE(BadDrawable)
                XLIB_ERROR_CODE(BadAccess)
                XLIB_ERROR_CODE(BadAlloc)
                XLIB_ERROR_CODE(BadColor)
                XLIB_ERROR_CODE(BadGC)
                XLIB_ERROR_CODE(BadIDChoice)
                XLIB_ERROR_CODE(BadName)
                XLIB_ERROR_CODE(BadLength)
                XLIB_ERROR_CODE(BadImplementation)
        }
        return false;
      }

      bool check_status (Status s) {
        if (s) {
          return true;
        }
        //      clog::fatal() << "xlib Status failed";
        return false;
      }

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

    } // namespace x11

    // --------------------------------------------------------------------------
    namespace {
      std::map<std::string, class_info> window_class_info_map;
      std::map<os::window, std::string> window_class_map;
    }
    // --------------------------------------------------------------------------
    namespace native {

      std::string get_class_name (os::window id) {
        return window_class_map[id];
      }

      const class_info& get_window_class (os::window id) {
        return window_class_info_map[get_class_name(id)];
      }

      void move (os::window w, const core::point& pt) {
        x11::check_return(XMoveWindow(core::global::get_instance(), w,
                                      pt.os_x(), pt.os_y()));
      }

      void resize (os::window w, const core::size& sz) {
        x11::check_return(XResizeWindow(core::global::get_instance(), w,
                                        sz.os_width(), sz.os_height()));
      }

      void place (os::window w, const core::rectangle& r) {
        x11::check_return(XMoveResizeWindow(core::global::get_instance(), w,
                                            r.os_x(), r.os_y(), r.os_width(), r.os_height()));
      }

      core::rectangle get_geometry (os::window wid) {
        Window root = 0;
        int x = 0, y = 0;
        unsigned int width = 0, height = 0;
        unsigned int border_width = 0;
        unsigned int depth = 0;
        if (wid && x11::check_status(XGetGeometry(core::global::get_instance(), wid,
                                                  &root, &x, &y, &width, &height,
                                                  &border_width, &depth))) {
          return core::global::scale_from_native(core::native_rect{x, y, width, height});
        }
        return core::rectangle::def;
      }

      void prepare(window* w) {
        static int initialized = core::x11::init_messages();
        (void)initialized;
        x11::prepare_win_for_event(w, KeyPressMask);
      }

      void unprepare(window* w) {
        x11::unprepare_win(w);
      }

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         window* data) {
        auto display = core::global::get_instance();
        auto screen = core::global::x11::get_screen();

        unsigned long mask = 0;
        XSetWindowAttributes wa;

        XVisualInfo vinfo;
        XMatchVisualInfo(display, screen, 24, TrueColor, &vinfo);

        if (type.get_background() == color::transparent) {
          mask |= CWBackPixmap;
          wa.background_pixmap = None;
        } else {
          wa.background_pixel = type.get_background();
          wa.border_pixel = 0;
          mask |= (CWBackPixel | CWBorderPixel);
        }

        auto visual = vinfo.visual;
        auto depth = vinfo.depth;

        if (type.get_cursor()) {
          mask |= CWCursor;
          wa.cursor = type.get_cursor();
        }

        mask |= (CWBitGravity | CWBackingStore | CWColormap);
        wa.bit_gravity = ForgetGravity;
        wa.backing_store = NotUseful;

        mask |= CWColormap;
        wa.colormap = XCreateColormap(display, DefaultRootWindow(display), visual, AllocNone);

        os::window id = XCreateWindow(display,
                                      parent_id,
                                      r.os_x(),
                                      r.os_y(),
                                      std::max<gui::os::size_type>(r.os_width(), 1),
                                      std::max<gui::os::size_type>(r.os_height(), 1),
                                      0,
                                      depth,
                                      type.get_class_style(),
                                      visual,
                                      mask,
                                      &wa);

        detail::set_os_window(data, id);

        x11::prepare_win_for_event(data, KeyReleaseMask);

        if (0 == window_class_info_map.count(type.get_class_name())) {
          window_class_info_map[type.get_class_name()] = type;
        }
        window_class_map[id] = type.get_class_name();

        return id;
      }

      void notify_created(window* w) {
        w->notify_event(core::x11::WM_CREATE_WINDOW);
      }

      void destroy (os::window w) {
        if (w) {
          window_class_map.erase(w);
          x11::validate_window(w);
          x11::check_return(XDestroyWindow(core::global::get_instance(), w));
          detail::unset_os_window(w);
        }
      }

      void close (os::window id) {
      }

      void notify_close (window& w) {
        w.notify_event(core::x11::WM_PROTOCOLS, core::x11::WM_DELETE_WINDOW);
      }

      bool is_visible (os::window id) {
        if (id) {
          XWindowAttributes a = {0};
          int result = XGetWindowAttributes(core::global::get_instance(), id, &a);
          return (x11::check_status(result) && (a.map_state == IsViewable));
        }
        return false;
      }

      void set_visible (os::window id, bool s) {
        if (id) {
          if (s) {
            clog::trace() << "Show window:" << id;
            x11::check_return(XMapWindow(core::global::get_instance(), id));
          } else {
            clog::trace() << "Hide window:" << id;
            x11::check_return(XUnmapWindow(core::global::get_instance(), id));
          }
        }
      }

      void enable (os::window id, bool on) {
        auto curs = get_window_class(id).get_cursor();
        if (curs) {
          unsigned long mask = CWCursor;
          XSetWindowAttributes wa = {0};
          wa.cursor = on ? curs : (os::cursor)win::cursor::arrow();
          x11::check_return(XChangeWindowAttributes(core::global::get_instance(), id, mask, &wa));
        }
      }

      void to_front (os::window id) {
        x11::check_return(XRaiseWindow(core::global::get_instance(), id));
      }

      void to_back (os::window id) {
        x11::check_return(XLowerWindow(core::global::get_instance(), id));
      }

      void take_focus (os::window id) {
        x11::check_return(XSetInputFocus(core::global::get_instance(), id,
                                         RevertToParent, CurrentTime));
      }

      void set_cursor (os::window id, const os::cursor& c) {
        XDefineCursor(core::global::get_instance(), id, c);
      }

      void invalidate (os::window id, const core::rectangle& p) {
        x11::invalidate_window(id, p);
      }

      void redraw (const window& w, os::window id, const core::rectangle& p) {
        XEvent event;

        XExposeEvent& e = event.xexpose;
        e.type = Expose;
        e.serial = 0;
        e.send_event = true;
        e.display = core::global::get_instance();
        e.window = id;
        e.x = p.os_x();
        e.y = p.os_y();
        e.width = p.os_width();
        e.height = p.os_height();
        e.count = 0;
        gui::os::event_result result;

        w.handle_event(event, result);

        core::global::sync();
      }

      void prepare_accept_focus (os::window, bool) {
      }

      void prepare_capture_pointer () {
      }

      void unprepare_capture_pointer () {
      }

      void capture_pointer (os::window id) {
        x11::check_return(XGrabPointer(core::global::get_instance(), id,
                                       False,
                                       ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                       GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
      }

      void uncapture_pointer (os::window id) {
        x11::check_return(XUngrabPointer(core::global::get_instance(), CurrentTime));
      }

      os::window get_desktop_window () {
        return DefaultRootWindow(core::global::get_instance());
      }

      core::size screen_size () {
        auto dpy = core::global::get_instance();
        auto scr = core::global::x11::get_screen();
        auto width = static_cast<gui::os::size_type>(DisplayWidth(dpy, scr));
        auto height = static_cast<gui::os::size_type>(DisplayHeight(dpy, scr));
        return core::size(gui::os::size{ width, height });
      }

      core::rectangle screen_area () {
        return core::rectangle(screen_size());
      }

      core::rectangle adjust_overlapped_area (const core::rectangle& r, const class_info&) {
        return r;
      }

      void prepare_overlapped (os::window id, os::window pid) {
        static int initialized = x11::init_for_net_wm_state();
        (void)initialized;

        XSetTransientForHint(core::global::get_instance(), id, pid);
      }

      os::window get_overlapped_parent (os::window) {
        return native::get_desktop_window();
      }

      void set_title (os::window id, const std::string& title) {
        x11::check_status(XStoreName(core::global::get_instance(), id, title.c_str()));
      }

      std::string get_title (os::window id) {
        char *window_name;
        if (x11::check_status(XFetchName(core::global::get_instance(), id, &window_name))) {
          std::string s(window_name);
          XFree(window_name);
          return s;
        }
        return {};
      }

      bool is_maximized (os::window id) {
        return x11::query_net_wm_state(id, x11::NET_WM_STATE_MAXIMIZED_HORZ, x11::NET_WM_STATE_MAXIMIZED_VERT);
      }

      bool is_minimized (os::window id) {
        return x11::query_net_wm_state(id, x11::NET_WM_STATE_HIDDEN);
      }

      bool is_top_most (os::window id) {
        return x11::query_net_wm_state(id, x11::NET_WM_STATE_ABOVE);
      }

      void minimize (os::window id) {
        XIconifyWindow(core::global::get_instance(), id, core::global::x11::get_screen());
      }

      void maximize (os::window id) {
        x11::send_net_wm_state(id, _NET_WM_STATE_ADD,
                               x11::NET_WM_STATE_MAXIMIZED_HORZ,
                               x11::NET_WM_STATE_MAXIMIZED_VERT);
      }

      void restore (os::window id) {
        x11::send_net_wm_state(id, _NET_WM_STATE_REMOVE,
                               x11::NET_WM_STATE_MAXIMIZED_HORZ,
                               x11::NET_WM_STATE_MAXIMIZED_VERT);
      }

      void set_top_most (os::window id, bool on) {
        x11::send_net_wm_state(id,
                               on ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                               x11::NET_WM_STATE_ABOVE);
      }

      void prepare_main_window (os::window id) {
        gui::os::instance display = core::global::get_instance();

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
      }

      void prepare_popup_window (os::window id) {
        gui::os::instance display = core::global::get_instance();

        x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU");

        XSetWindowAttributes wa;
        wa.override_redirect = 1;
        XChangeWindowAttributes(display, id, CWOverrideRedirect, &wa);
      }

      void prepare_dialog_window (os::window id, os::window pid) {
        gui::os::instance display = core::global::get_instance();
        x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DIALOG");
        x11::change_property(display, id, "_NET_WM_STATE", "_NET_WM_STATE_MODAL");
        x11::change_property(display, id, "WM_CLIENT_LEADER", pid);
        x11::set_wm_protocols(display, id);
      }


    } // namespace native

  } // namespace win

} // namespace gui

#endif // GUIPP_X11
