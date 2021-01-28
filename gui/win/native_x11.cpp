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

    } // namespace native

  } // namespace win

} // namespace gui

#endif // GUIPP_X11
