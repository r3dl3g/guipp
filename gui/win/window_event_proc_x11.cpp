/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     system specific window event handler function
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <set>
#include <thread>

# include <unistd.h>
#include <logging/logger.h>
#include <util/robbery.h>
#include <util/blocking_queue.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/window_event_proc.h"
#include "gui/win/dbg_win_message.h"
#include "gui/win/native.h"
#include "gui/win/clipboard.h"


namespace util {

  namespace robbery {

#ifdef _LIBCPP_THREAD
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::__id_>;
#elif linux
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
#endif // linux

    thread_id::type get_native_thread_id (std::thread::id& t) {
      return t.*rob(thread_id());
    }

  } // namespace robbery

} // namespace util

namespace gui {

  namespace win {

    namespace x11 {

      typedef util::blocking_queue<std::function<simple_action>> simple_action_queue;
      simple_action_queue queued_actions;

      core::native_rect get_expose_rect (core::event& e) {
        return get<core::native_rect, XExposeEvent>::param(e);
      }

    } // namespace x11
    
    namespace detail {
      bool check_expose (const core::event& e) {
        return (e.type == Expose) && (e.xexpose.count > 0);
      }

      inline win::window* get_event_window (const core::event& e) {
        switch (e.type) {
          case ConfigureNotify:
            return native::get_window(e.xconfigure.window);
          case NoExpose:
            return nullptr;
          default:
            return native::get_window(e.xany.window);
        }
      }

    } // namespace detail


    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, core::hot_key::call> hot_key_entry;
      typedef std::map<core::hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

      static std::thread::id main_thread_id;

      void install_message_filter () {}

      // --------------------------------------------------------------------------
      bool check_hot_key (const core::event& e) {
        if (e.type == KeyPress) {
          core::hot_key hk(get_key_symbol(e), get_key_state(e));
#if KEY_DEBUG
          logging::debug() << "Check hot key '" << hk.get_key_string() << "'";
#endif
          auto i = detail::hot_keys.find(hk);
          if (i != detail::hot_keys.end()) {
            i->second.second();
            return true;
          }
        }

        return false;
      }

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const core::hot_key& hk, const core::hot_key::call& fn, window* win) {
        os::window root = {};
#if KEY_DEBUG
        logging::debug() << "Register hot key '" << hk.get_key_string() << "'";
#endif
        auto dpy = core::global::get_instance();
        root = DefaultRootWindow(dpy);
        if (win && win->is_valid()) {
          auto& o = win->get_overlapped_window();
          o.add_event_mask(KeyPressMask);
          root = o.get_os_window();
        }
        //XGrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), hk.get_modifiers(), root, False, GrabModeAsync, GrabModeAsync);
        if (root) {
          detail::hot_keys.emplace(hk, std::make_pair(root, fn));
        } else {
          logging::warn() << "Root Windows is not yet created. Hotkey could not be registered!";
        }
      }

      void unregister_hot_key (const core::hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        os::window root = i->second.first;
        auto dpy = core::global::get_instance();
        XUngrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), hk.get_modifiers(), root);

        detail::hot_keys.erase(i);
      }

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

      window* get_current_focus_window () {
        Window focus = 0;
        int revert_to = 0;
        if (XGetInputFocus(core::global::get_instance(), &focus, &revert_to) < BadValue) {
          return native::get_window(focus);
        }
        return nullptr;
      }

      overlapped_window& get_application_main_window() {
        auto display = core::global::get_instance();

        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &nchildren)) {
          for (unsigned int i = 0; i < nchildren; ++i) {
            auto win = native::get_window(children[i]);
            if (win) {
              return win->get_overlapped_window();
            }
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }
      
    } // global

    bool is_button_event_outside (const window& w, const core::event& e) {
      switch (e.type) {
      case ButtonPress:
      case ButtonRelease: {
        if (e.xbutton.window == w.get_overlapped_window().get_os_window()) {
          return false;
        }
        int x, y;
        Window child;
        auto display = core::global::get_instance();
        XTranslateCoordinates(display,
                              e.xbutton.window,
                              DefaultRootWindow(display),
                              e.xbutton.x, e.xbutton.y,
                              &x, &y, &child);

        const core::rectangle area = w.absolute_geometry();
        const core::point pt = core::global::scale_from_native(core::native_point{x, y});
        return !area.is_inside(pt);
      }
      }
      return false;
    }

    // --------------------------------------------------------------------------
    void process_event (const core::event& e, gui::os::event_result& resultValue) {
      win::window* win = win::detail::get_event_window(e);

      if (win && win->is_valid()) {

        resultValue = 0;

        try {
          win->handle_event(e, resultValue);
        } catch (std::exception& ex) {
          logging::fatal() << "exception in run_main_loop: " << ex;
        } catch (...) {
          logging::fatal() << "Unknown exception in run_main_loop()";
        }
      }
    }

    inline bool is_expose_event (const core::event& e) {
      return (e.type == Expose);// || (e.type == GraphicsExpose);// || (e.type == NoExpose);
    }

    void wait_for_event (int fd, timeval timeout = {0, 10000}) {
      // Create a file description set containing fd
      fd_set read_fds;
      FD_ZERO(&read_fds);
      FD_SET(fd, &read_fds);

      // Wait for an event on the file descriptor or for timer expiration
      select(1, &read_fds, nullptr, nullptr, &timeout);
    }

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, const detail::filter_call& filter) {

      running = true;

      gui::os::instance display = core::global::get_instance();
      const int fd = ConnectionNumber(display);
      gui::os::event_result resultValue = 0;
      std::function<simple_action> action;

      while (running) {

        while (XPending(display) > 0) {

          core::event e;
          XNextEvent(display, &e);

          if (detail::check_expose(e) || detail::check_message_filter(e) || (filter && filter(e))) {
            continue;
          }

          if (is_expose_event(e)) {
            native::x11::invalidate_window(e.xany.window, x11::get_expose_rect(e));
          } else {
            process_event(e, resultValue);
          }
          if (protocol_message_matcher<core::x11::WM_DELETE_WINDOW>(e) && !resultValue) {
            running = false;
          }

          if ((e.type == ConfigureNotify) && running) {
            update_last_geometry(e.xconfigure.window, get<core::rectangle, XConfigureEvent>::param(e));
          }
        }

        if (x11::queued_actions.try_dequeue(action)) {
          action();
        } else {
          wait_for_event(fd);
        }

        native::x11::draw_invalidated_windows();

        XFlush(display);

      }

      return resultValue;
    }

    namespace {
      bool main_loop_is_running = false;
    }

    int run_main_loop () {
      main_loop_is_running = true;
      detail::main_thread_id = global::get_current_thread_id();
      return run_loop(main_loop_is_running, &detail::check_hot_key);
    }

    void quit_main_loop () {
      logging::trace() << "Received quit_main_loop()";
      main_loop_is_running = false;
      core::global::fini();
    }

    void run_on_main (const window& w, const std::function<void()>& action) {
      x11::queued_actions.enqueue(action);

      gui::os::instance display = core::global::get_instance();

      XClientMessageEvent event;
      memset(&event, 0, sizeof(event));

      event.type = ClientMessage;
      event.send_event = True;
      event.display = display;
      event.window = w.get_overlapped_window().get_os_window();
      event.format = 32;
      XSendEvent(display, event.window, False, 0, (XEvent*)&event);
    }
  }   // win
} // gui

#endif // GUIPP_X11
