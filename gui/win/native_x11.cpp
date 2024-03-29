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
 * @brief     X11 specific native system API calls
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/win/overlapped_window.h>


namespace gui {

  namespace win {

    namespace x11 {

# define XLIB_ERROR_CODE(a) case a: logging::fatal() << # a;break;

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
        //      logging::fatal() << "xlib Status failed";
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
      Atom NET_WM_STATE_FULLSCREEN = 0;

      int init_for_net_wm_state () {
        core::x11::init_atom(NET_WM_STATE, "_NET_WM_STATE");
        core::x11::init_atom(NET_WM_STATE_MAXIMIZED_HORZ, "_NET_WM_STATE_MAXIMIZED_HORZ");
        core::x11::init_atom(NET_WM_STATE_MAXIMIZED_VERT, "_NET_WM_STATE_MAXIMIZED_VERT");
        core::x11::init_atom(NET_WM_STATE_ABOVE, "_NET_WM_STATE_ABOVE");
        core::x11::init_atom(NET_WM_STATE_HIDDEN, "_NET_WM_STATE_HIDDEN");
        core::x11::init_atom(ATOM_ATOM, "ATOM");
        core::x11::init_atom(WM_SIZE_HINTS, "WM_SIZE_HINTS");
        core::x11::init_atom(NET_WM_STATE_FULLSCREEN, "_NET_WM_STATE_FULLSCREEN");
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
      std::map<const char*, class_info> window_class_info_map;
    }
    // --------------------------------------------------------------------------
    namespace native {

      // TODO Register window_class also for win::window!

      const class_info& get_window_class (const char* class_name) {
        return window_class_info_map[class_name];
      }

      void register_window_class (const class_info& type) {
        if (0 == window_class_info_map.count(type.get_class_name())) {
          window_class_info_map[type.get_class_name()] = type;
        }
      }

      void move (os::window w, const core::point& pt) {
        const auto npt = core::global::scale_to_native(pt);
        x11::check_return(XMoveWindow(core::global::get_instance(), w,
                                      npt.x(), npt.y()));
      }

      void resize (os::window w, const core::size& sz) {
        x11::check_return(XResizeWindow(core::global::get_instance(), w,
                                        sz.os_width(), sz.os_height()));
      }

      void geometry (os::window w, const core::rectangle& r) {
        const auto nr = core::global::scale_to_native(r);
        x11::check_return(XMoveResizeWindow(core::global::get_instance(), w,
                                            nr.x(), nr.y(), nr.width(), nr.height()));
      }

      void notify_move (window& win, const core::point& pt, const core::point&) {
        if (win.is_valid()) {
          const auto npt = core::global::scale_to_native(pt);

          XEvent event;
          XConfigureEvent& client = event.xconfigure;

          client.type = ConfigureNotify;
          client.serial = 0;
          client.send_event = 1;
          client.display = core::global::get_instance();
          client.event = 0;
          client.window = 0;
          client.x = npt.x();
          client.y = npt.y();
          client.width = 0;
          client.height = 0;
          client.border_width = 0;
          client.above = 0;
          client.override_redirect = 0;

          gui::os::event_result result = 0;
          win.handle_event(event, result);
        }
      }

      void notify_resize (window& win, const core::size& sz, const core::size&) {
        if (win.is_valid()) {
          XEvent event;
          XConfigureEvent& client = event.xconfigure;

          client.type = ConfigureNotify;
          client.serial = 0;
          client.send_event = 0;
          client.display = core::global::get_instance();
          client.event = 0;
          client.window = 0;
          client.x = 0;
          client.y = 0;
          client.width = sz.os_width();
          client.height = sz.os_height();
          client.border_width = 0;
          client.above = 0;
          client.override_redirect = 0;

          gui::os::event_result result = 0;
          win.handle_event(event, result);
        }
      }

      core::native_point get_position (os::window wid) {
        auto display = core::global::get_instance();
        Window root = DefaultRootWindow(display);
        Window child;
        int x = 0, y = 0;
        if (wid && x11::check_status(XTranslateCoordinates(display, wid, root, 0, 0,&x, &y, &child))) {
          return {x, y};
        }
        return core::native_point::zero;
      }

      core::size client_size (os::window, const core::size& sz) {
        return sz;
//        Window root;
//        int x, y;
//        unsigned int w, h, bw, d;
//        x11::check_status(XGetGeometry(core::global::get_instance(), id, &root,
//                                       &x, &y, &w, &h, &bw, &d));
//        return core::global::scale_from_native(core::native_rect(0, 0, w, h));
      }

      void prepare (overlapped_window& w) {
        static int initialized = core::x11::init_messages();
        (void)initialized;
      }

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         overlapped_window& data) {
        auto display = core::global::get_instance();
        auto screen = core::global::x11::get_screen();

        XSetWindowAttributes wa;

        unsigned long mask = CWEventMask;
        wa.event_mask = data.get_event_mask() | ExposureMask;

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

        mask |= (CWBackPixmap | CWBitGravity | CWBackingStore | CWColormap);
        wa.bit_gravity = NorthWestGravity;
        wa.backing_store = NotUseful;
        wa.background_pixmap = None;
        wa.colormap = XCreateColormap(display, DefaultRootWindow(display), visual, AllocNone);
        const auto nr = core::global::scale_to_native(r);

        os::window id = XCreateWindow(display,
                                      parent_id,
                                      nr.x(),
                                      nr.y(),
                                      std::max<gui::os::size_type>(nr.width(), 1),
                                      std::max<gui::os::size_type>(nr.height(), 1),
                                      0,
                                      depth,
                                      type.get_class_style(),
                                      visual,
                                      mask,
                                      &wa);

        detail::set_os_window(&data, id);

        return id;
      }

      void destroy (os::window w) {
        if (w) {
          x11::validate_window(w);
          x11::check_return(XDestroyWindow(core::global::get_instance(), w));
          detail::unset_os_window(w);
        }
      }

      void notify_close (window& w) {
        w.notify_event(core::x11::WM_PROTOCOLS, core::x11::WM_DELETE_WINDOW);
      }

      void prepare_win_for_event (const overlapped_window& win) {
        os::window id = win.get_os_window();
        if (id) {
          XSelectInput(core::global::get_instance(), id, win.get_event_mask());
        }
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
            logging::trace() << "Show window:" << id;
            x11::check_return(XMapWindow(core::global::get_instance(), id));
          } else {
            logging::trace() << "Hide window:" << id;
            x11::check_return(XUnmapWindow(core::global::get_instance(), id));
          }
        }
      }

      void enable (overlapped_window& w, bool on) {
        auto curs = w.get_window_class().get_cursor();
        if (curs) {
          unsigned long mask = CWCursor;
          XSetWindowAttributes wa = {0};
          wa.cursor = on ? curs : (os::cursor)win::cursor::arrow();
          x11::check_return(XChangeWindowAttributes(core::global::get_instance(), w.get_os_window(), mask, &wa));
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

      void invalidate (os::window id, const core::native_rect& r) {
        x11::invalidate_window(id, r);
      }

      void redraw (window& w, os::window id, const core::native_rect& r) {
        XEvent event;
        XExposeEvent& e = event.xexpose;
        e.type = Expose;
        e.serial = 0;
        e.send_event = true;
        e.display = core::global::get_instance();
        e.window = id;
        e.x = r.x();
        e.y = r.y();
        e.width = r.width();
        e.height = r.height();
        e.count = 0;
        gui::os::event_result result;

        w.handle_event(event, result);
      }

      void prepare_accept_focus (os::window, bool) {
      }

#define NO_CAPTUREx

      std::vector<os::window> capture_stack;

      inline void grab (os::window id) {
        x11::check_return(XGrabPointer(core::global::get_instance(), id,
                                       False,
                                       ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                       GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
      }

      inline void ungrab () {
        x11::check_return(XUngrabPointer(core::global::get_instance(), CurrentTime));
      }

#define NO_CAPTUREx

      void capture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (!capture_stack.empty()) {
          ungrab();
        }
        capture_stack.push_back(id);
        grab(id);
#endif // NO_CAPTURE
      }

      void uncapture_pointer (os::window id) {
#ifndef NO_CAPTURE
        if (!capture_stack.empty()) {
          ungrab();
          capture_stack.pop_back();
          if (!capture_stack.empty()) {
            grab(capture_stack.back());
          }
        }
#endif // NO_CAPTURE
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

      core::native_point surface_to_screen (os::window id, const core::native_point& pt) {
        return pt + get_position(id);
      }

      core::native_point screen_to_surface (os::window id, const core::native_point& pt) {
        return pt - get_position(id);
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

      bool is_fullscreen (os::window id) {
        return x11::query_net_wm_state(id, x11::NET_WM_STATE_FULLSCREEN);
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
                               x11::NET_WM_STATE_MAXIMIZED_VERT,
                               x11::NET_WM_STATE_FULLSCREEN);
      }

      void set_top_most (os::window id, bool on) {
        x11::send_net_wm_state(id,
                               on ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE,
                               x11::NET_WM_STATE_ABOVE);
      }

      void set_fullscreen (os::window id, bool on) {
#ifdef USE_OVERRIDE_REDIRECT
    XWindowAttributes wa = {};
    XGetWindowAttributes(display, get_os_window(), &wa);
    XSetWindowAttributes swa = {};
    swa.override_redirect = !wa.override_redirect;
    XChangeWindowAttributes(display, get_os_window(), CWOverrideRedirect, &swa);
#elif defined USE_WM_STATE_FULLSCREEN
    Atom wm_state       = XInternAtom(display, "_NET_WM_STATE", False);
    Atom wm_fullscreen  = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);

    XChangeProperty(display, get_os_window(), wm_state, XA_ATOM, 32,
                    PropModeReplace, (unsigned char *)&wm_fullscreen, 1);
#else
        XEvent x11_event;
        x11_event.xclient.type = ClientMessage;
        x11_event.xclient.serial = 0;
        x11_event.xclient.send_event = True;
        x11_event.xclient.window = id;
        x11_event.xclient.message_type = x11::NET_WM_STATE;
        x11_event.xclient.format = 32;
        x11_event.xclient.data.l[0] = on;
        x11_event.xclient.data.l[1] = x11::NET_WM_STATE_FULLSCREEN;
        x11_event.xclient.data.l[2] = 0;

        auto dpy = core::global::get_instance();
        XSendEvent(dpy, XDefaultRootWindow(dpy), False, SubstructureRedirectMask | SubstructureNotifyMask, &x11_event );
#endif
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

        XSetWindowAttributes swa;
        swa.override_redirect = 1;
        XChangeWindowAttributes(display, id, CWOverrideRedirect, &swa);
      }

      void prepare_tooltip_window (os::window id) {
        gui::os::instance display = core::global::get_instance();

        x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_TOOLTIP");

        XSetWindowAttributes swa;
        swa.override_redirect = 1;
        XChangeWindowAttributes(display, id, CWOverrideRedirect, &swa);
      }

      void prepare_dialog_window (os::window id, os::window pid) {
        gui::os::instance display = core::global::get_instance();
        x11::change_property(display, id, "_NET_WM_WINDOW_TYPE", "_NET_WM_WINDOW_TYPE_DIALOG");
        x11::change_property(display, id, "_NET_WM_STATE", "_NET_WM_STATE_MODAL");
        x11::change_property(display, id, "WM_CLIENT_LEADER", pid);
        x11::set_wm_protocols(display, id);
      }

      void erase (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        if (!color::is_transparent(c)) {
          gui::os::instance display = core::global::get_instance();
          const int sc = static_cast<int>(core::global::get_scale_factor());
          XSetForeground(display, gc, c);
          XSetBackground(display, gc, c);
          XFillRectangle(display, id, gc, r.x(), r.y(), r.width() - sc, r.height() - sc);
          XDrawRectangle(display, id, gc, r.x(), r.y(), r.width() - sc, r.height() - sc);
        }
      }

      void frame (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        if (!color::is_transparent(c)) {
          gui::os::instance display = core::global::get_instance();
          const int sc = static_cast<int>(core::global::get_scale_factor());
          XSetForeground(display, gc, c);
          XDrawRectangle(display, id, gc, r.x(), r.y(), r.width() - sc, r.height() - sc);
        }
      }

      os::backstore create_surface (const core::native_size& size, os::window id) {
        return XCreatePixmap(core::global::get_instance(), id, size.width(), size.height(),
                             core::global::get_device_depth());
      }

      void delete_surface (os::backstore id) {
        XFreePixmap(core::global::get_instance(), id);
      }

      void copy_surface (os::bitmap src, os::drawable target, os::graphics context,
                         const core::native_point& from, const core::native_point& to,
                         const core::native_size& size) {
        XCopyArea(core::global::get_instance(), src, target, context, from.x(), from.y(),
                  size.width(), size.height(), to.x(), to.y());
      }

      // --------------------------------------------------------------------------
      void send_client_message_ (window* win, Atom message, long l1 = 0, long l2 = 0, long l3 = 0, long l4 = 0, long l5 = 0) {
        if (win && win->is_valid()) {
          XEvent event;
          XClientMessageEvent& client = event.xclient;

          client.type = ClientMessage;
          client.serial = 0;
          client.send_event = True;
          client.display = core::global::get_instance();
          client.window = 0;
          client.message_type = message;
          client.format = 32;
          client.data.l[0] = l1;
          client.data.l[1] = l2;
          client.data.l[2] = l3;
          client.data.l[3] = l4;
          client.data.l[4] = l5;

          gui::os::event_result result = 0;
          win->handle_event(event, result);
        }
      }

      // --------------------------------------------------------------------------
      void send_client_message (window* win, os::message_type message, long l1, long l2) {
        send_client_message_(win, message, l1, l2);
      }

      void send_client_message (window* win, os::message_type message, void* v1, void* v2) {
        send_client_message_(win, message, reinterpret_cast<std::uintptr_t>(v1), reinterpret_cast<std::uintptr_t>(v2));
      }

      void send_client_message (window* win, os::message_type message, const core::size& sz) {
        os::size s = sz.os();
        send_client_message_(win, message, s.cx, s.cy);
      }

      void send_client_message (window* win, os::message_type message, const core::rectangle& wr) {
        const auto r = core::global::scale_to_native(wr);
        send_client_message_(win, message, r.x(), r.y(), r.width(), r.height());
      }

      void send_mouse_event (window* win, bool enter) {
        if (win && win->is_valid()) {
          XEvent event;
          XCrossingEvent& client = event.xcrossing;

          client.type = enter ? EnterNotify : LeaveNotify;
          client.serial = 0;
          client.send_event = True;
          client.display = core::global::get_instance();
          client.window = 0;
          client.mode = NotifyNormal;

          gui::os::event_result result = 0;
          win->handle_event(event, result);
        }
      }

      void send_notify_visibility (window* win, bool visible) {
        if (win && win->is_valid()) {
          XEvent event;
          XMapEvent& client = event.xmap;

          client.type = visible ? MapNotify : UnmapNotify;
          client.serial = 0;
          client.send_event = True;
          client.display = core::global::get_instance();
          client.window = 0;

          gui::os::event_result result = 0;
          win->handle_event(event, result);
        }
      }

      // --------------------------------------------------------------------------
      core::native_point get_current_pointer_pos (gui::os::window id) {
        gui::os::window root, child;
        int root_x, root_y, child_x, child_y;
        unsigned int mask;
        XQueryPointer(core::global::get_instance(), id,
                      &root, &child, &root_x, &root_y,
                      &child_x, &child_y, &mask);
        return {root_x, root_y};
      }

    } // namespace native

  } // namespace win

} // namespace gui

#endif // GUIPP_X11
