/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: window event handler function
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#ifdef WIN32
# include <windowsx.h>
#endif // WIN32
#ifdef X11
# include <unistd.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <base/robbery.h>
#include <gui/win/window.h>
#include <gui/win/window_event_proc.h>

namespace basepp {

  namespace robbery {

#ifdef _LIBCPP_THREAD
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::__id_>;
#elif X11
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
#elif WIN32
    using thread_id = jugger<_Thrd_id_t, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_Id>;
#endif // WIN32

    thread_id::type get_native_thread_id (std::thread::id& t) {
      return t.*rob(thread_id());
    }

  } // namespace robbery

} // namespace basepp

namespace gui {


  namespace win {

    namespace detail {

      void set_id (window* w, os::window id) {
        w->id = id;
      }

    } // namespace detail

#ifdef WIN32
    namespace detail {

      const os::event_id ACTION_MESSAGE = WM_USER + 0x101;

      window* get_window (os::window id) {
        return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
      }

      void set_window (os::window id, window* win) {
        SetWindowLongPtr(id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
      }

      void unset_window (os::window id) {
        SetWindowLongPtr(id, GWLP_USERDATA, 0);
      }

      bool check_expose (const core::event&) {
        return false;
      }

      bool handle_by_window (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, os::event_result& resultValue) {
        window* w = get_window(hwnd);
        if (w && w->is_valid()) {
          return w->handle_event(core::event(hwnd, msg, wParam, lParam), resultValue);
        }
        return false;
      }

      void set_window_id (LONG_PTR lParam, os::window id) {
        window* w = reinterpret_cast<window*>(lParam);
        set_id(w, id);
      }

    } // namespace detail

    namespace win32 {

      LRESULT CALLBACK WindowEventProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_INITDIALOG:
          SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
          detail::set_window_id(lParam, hwnd);
          break;
        case WM_CREATE: {
          CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
          SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
          detail::set_window_id((LONG_PTR)cs->lpCreateParams, hwnd);
          break;
        }
# ifdef KEY_DEBUG
        case WM_KEYDOWN:
          LogDebug << "Key down 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
        case WM_KEYUP:
          LogDebug << "Key up 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
        case WM_CHAR:
          LogDebug << "Char 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
# endif // KEY_DEBUG
        }

        os::event_result result = 0;
        window* w = detail::get_window(hwnd);
        if (w && w->is_valid()) {
          w->handle_event(core::event(hwnd, msg, wParam, lParam), result);
          if (result) {
            return result;
          }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
      }

    } // namespace win32

#endif // WIN32
#ifdef X11

    namespace x11 {

      typedef std::map<os::window, XIC> window_ic_map;
      window_ic_map s_window_ic_map;

      XIC get_window_ic (os::window id) {
        window_ic_map::iterator i = s_window_ic_map.find(id);
        if (i != s_window_ic_map.end()) {
          return i->second;
        }
        return nullptr;
      }

      XIM s_im = nullptr;
      XIMStyle s_best_style = 0x0F1F;

      typedef basepp::blocking_queue<std::function<simple_action>> simple_action_queue;
      simple_action_queue queued_actions;

    } // namespace x11

    namespace detail {

      typedef std::map<os::window, win::window*> window_map;
      window_map global_window_map;

      window* get_window (os::window id) {
        return global_window_map[id];
      }

      void set_window (os::window id, window* win) {
        global_window_map[id] = win;
      }

      void unset_window (os::window id) {
        clear_last_place(id);
        global_window_map.erase(id);
      }

      bool check_expose (const core::event& e) {
        return (e.type == Expose) && (e.xexpose.count > 0);
      }

      inline win::window* get_event_window (const core::event& e) {
        switch (e.type) {
        case ConfigureNotify: return get_window(e.xconfigure.window);
        default: return get_window(e.xany.window);
        }
      }

    } // namespace detail

#endif // X11


    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, hot_key::call> hot_key_entry;
      typedef std::map<hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

      static int g_next_filter_id = 1;
      static std::thread::id main_thread_id;

      typedef std::pair<int, filter_call> filter_call_entry;
      typedef std::vector<filter_call_entry> filter_list;
      filter_list message_filters;

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const hot_key& hk, const hot_key::call& fn, window* win) {
#ifdef WIN32
        UINT modifiers = MOD_NOREPEAT;
        if (control_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_CONTROL;
        }
        if (alt_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_ALT;
        }
        if (system_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_WIN;
        }
        if (shift_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_SHIFT;
        }
        os::window root = win ? win->get_id() : NULL;
        RegisterHotKey(root, hk.get_key(), modifiers, hk.get_key());
#endif // WIN32
#ifdef X11
        auto dpy = core::global::get_instance();
        os::window root = win ? win->get_id() : DefaultRootWindow(dpy);
        XGrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), AnyModifier, root, False, GrabModeAsync, GrabModeAsync);
        if (win && win->is_valid()) {
          x11::prepare_win_for_event(win, KeyPressMask);
        }
#endif // X11

        detail::hot_keys.emplace(hk, std::make_pair(root, fn));
      }

      void unregister_hot_key (const hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        os::window root = i->second.first;

#ifdef WIN32
        UnregisterHotKey(root, hk.get_key());
#endif // WIN32
#ifdef X11
        auto dpy = core::global::get_instance();
        XUngrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), AnyModifier, root);
#endif // X11

        detail::hot_keys.erase(i);
      }

      int register_message_filter (detail::filter_call filter) {
        detail::message_filters.emplace_back(std::make_pair(detail::g_next_filter_id, filter));
        return detail::g_next_filter_id++;
      }

      void unregister_message_filter (int& id) {
        auto e = detail::message_filters.end();
        auto b = detail::message_filters.begin();
        auto i = std::find_if(b, e, [id](const detail::filter_call_entry & e)->bool {
                                return e.first == id;
                              });
        if (i != e) {
          detail::message_filters.erase(i);
        }
        id = 0;
      }

#ifdef WIN32
      window* get_current_focus_window () {
        return detail::get_window(GetFocus());
      }

      window* get_application_main_window() {
        GUITHREADINFO info;
        memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(GUITHREADINFO);
        if (GetGUIThreadInfo(basepp::robbery::get_native_thread_id(detail::main_thread_id), &info)) {
          HWND win = info.hwndActive;
          if (win) {
            HWND parent = GetParent(win);
            while (parent) {
              win = parent;
              parent = parent = GetParent(win);
            }
          }
          return detail::get_window(win);
        }
        return nullptr;
      }

      void register_utf8_window (os::window) {}

      void unregister_utf8_window (os::window) {}

#endif // WIN32

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

#ifdef X11
      window* get_current_focus_window () {
        Window focus = 0;
        int revert_to = 0;
        if (XGetInputFocus(core::global::get_instance(), &focus, &revert_to) < BadValue) {
          return detail::get_window(focus);
        }
        return nullptr;
      }

      window* get_application_main_window() {
        auto display = core::global::get_instance();

        Window root = 0;
        Window parent = 0;
        Window *children = 0;
        unsigned int nchildren = 0;

        if (XQueryTree(display, DefaultRootWindow(display), &root, &parent, &children, &nchildren)) {
          for (unsigned int i = 0; i < nchildren; ++i) {
            auto win = detail::get_window(children[i]);
            if (win) {
              return win;
            }
          }
        }
        return nullptr;
      }
      
      void register_utf8_window (os::window id) {
        if (!id) {
          return;
        }

        if (!x11::s_im) {
          x11::s_im = XOpenIM(core::global::get_instance(), NULL, NULL, NULL);
          XIMStyle app_supported_styles = XIMPreeditNone | XIMPreeditNothing | XIMPreeditArea |
                                          XIMStatusNone | XIMStatusNothing | XIMStatusArea;

          XIMStyles *im_supported_styles;
          /* figure out which styles the IM can support */
          XGetIMValues(x11::s_im, XNQueryInputStyle, &im_supported_styles, NULL);
          auto count = im_supported_styles->count_styles;
          for (decltype(count) i = 0; i < count; ++i) {
            XIMStyle style = im_supported_styles->supported_styles[i];
            if ((style & app_supported_styles) == style) { /* if we can handle it */
              x11::s_best_style = std::min(style, x11::s_best_style);
            }
          }
          XFree(im_supported_styles);
        }

        XIC ic = XCreateIC(x11::s_im, XNInputStyle, x11::s_best_style, XNClientWindow, id, NULL);
        x11::s_window_ic_map[id] = ic;
      }

      void unregister_utf8_window (os::window id) {
        x11::window_ic_map::iterator i = x11::s_window_ic_map.find(id);
        if (i != x11::s_window_ic_map.end()) {
          XIC ic = i->second;
          if (ic) {
            XDestroyIC(ic);
          }
          x11::s_window_ic_map.erase(id);
        }
      }

#endif // X11

    } // global

    // --------------------------------------------------------------------------
    bool check_message_filter (const core::event& ev) {
      for (std::size_t i = 0, e = detail::message_filters.size(); i != e; ++i) {
        detail::filter_call& f = detail::message_filters[i].second;
        if (f(ev)) {
          return true;
        }
      }
      return false;
    }

    // --------------------------------------------------------------------------
    bool check_hot_key (const core::event& e) {
#ifdef WIN32
      if (e.type == WM_HOTKEY) {
#endif // WIN32
#ifdef X11
      if (e.type == KeyPress) {
#endif // X11
      hot_key hk(get_key_symbol(e), get_key_state(e));
      auto i = detail::hot_keys.find(hk);
      if (i != detail::hot_keys.end()) {
        i->second.second();
        return true;
      }
    }

    return false;
  }

#ifdef WIN32
  bool is_button_event_outside (const window& w, const core::event& e) {
    switch (e.type) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MBUTTONDBLCLK: {
      POINT pt = {GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)};
      ClientToScreen(e.id, &pt);
      RECT r;
      GetWindowRect(w.get_id(), &r);
      return !PtInRect(&r, pt);
    }
    }
    return false;
  }

#endif // WIN32
#ifdef X11
  bool is_button_event_outside (const window& w, const core::event& e) {
    switch (e.type) {
    case ButtonPress:
    case ButtonRelease: {
      if (e.xbutton.window == w.get_id()) {
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

      core::rectangle area = w.absolute_place();
      return !area.is_inside(core::point(x, y));
    }
    }
    return false;
  }

#endif // X11

  struct in_event_handle_state {
    inline in_event_handle_state (const window& win)
      : state(win.get_state())
    {
      state.set_in_event_handle(true);
    }

    inline ~in_event_handle_state () {
      state.set_in_event_handle(false);
    }

    window_state state;
  };


  // --------------------------------------------------------------------------
  int run_loop (volatile bool& running, detail::filter_call filter) {

    running = true;

#ifdef WIN32
    MSG msg;
    while (running && GetMessage(&msg, nullptr, 0, 0)) {

      if (msg.message == detail::ACTION_MESSAGE) {
        std::function<void()>* action = (std::function<void()>*)msg.lParam;
        if (action && *action) {
          (*action)();
          delete action;
        }
        continue;
      }

      TranslateMessage(&msg);

      if (filter && filter(msg)) {
        continue;
      }

      DispatchMessage(&msg);
    }
    return (int)msg.wParam;
#endif // WIN32

#ifdef X11
    os::instance display = core::global::get_instance();
    os::event_result resultValue = 0;

    core::event e;
    std::function<simple_action> action;

    auto x11_fd = ConnectionNumber(display);

    // Create a File Description Set containing x11_fd
    fd_set in_fds;
    FD_ZERO(&in_fds);
    FD_SET(x11_fd, &in_fds);

    while (running) {

      // Set our timeout.  Ten milliseconds sounds good.
      timeval timeout {0, 10000};

      // Wait for X Event or a Timer
      select(x11_fd + 1, &in_fds, NULL, NULL, &timeout);

      while (x11::queued_actions.try_dequeue(action)) {
        action();
      }

      while (XPending(display)) {
        XNextEvent(display, &e);

        if (filter && filter(e)) {
          continue;
        }

        win::window* win = win::detail::get_event_window(e);

        if (win && win->is_valid()) {

          resultValue = 0;

          try {
            in_event_handle_state guard(*win);
            win->handle_event(e, resultValue);
          } catch (std::exception& ex) {
            LogFatal << "exception in run_main_loop: " << ex;
          } catch (...) {
            LogFatal << "Unknown exception in run_main_loop()";
          }

          if (protocol_message_matcher<x11::WM_DELETE_WINDOW>(e) && !resultValue) {
            running = false;
          }

//          core::global::sync();

          if (e.type == ConfigureNotify) {
            update_last_place(e.xconfigure.window, get<core::rectangle, XConfigureEvent>::param(e));
          }
        }
      }
    }
    return resultValue;
#endif // X11
  }

  namespace {
    bool main_loop_is_running = false;
  }

  int run_main_loop () {
    main_loop_is_running = true;
    detail::main_thread_id = global::get_current_thread_id();
    return run_loop(main_loop_is_running, [] (const core::event & e) {
      return detail::check_expose(e) || check_message_filter(e) || check_hot_key(e);
    });
  }

  void quit_main_loop () {
    main_loop_is_running = false;
  }

  void run_on_main (std::function<void()> action) {
#ifdef X11
    x11::queued_actions.enqueue(action);
#endif // X11
#ifdef WIN32
    PostThreadMessage(basepp::robbery::get_native_thread_id(detail::main_thread_id),
                      detail::ACTION_MESSAGE,
                      0,
                      (ULONG_PTR)new std::function<void()>(action));
#endif // WIN32
  }

}   // win

} // gui
