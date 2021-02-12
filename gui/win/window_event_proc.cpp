/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <set>
#include <thread>

#ifdef GUIPP_WIN
# include <windowsx.h>
#elif GUIPP_X11
# include <unistd.h>
#elif GUIPP_QT
# include <QtGui/QGuiApplication>
# include <QtCore/QEventLoop>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <util/robbery.h>
#include <util/blocking_queue.h>
#include <gui/win/overlapped_window.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/dbg_win_message.h>

namespace util {

  namespace robbery {

#ifdef _LIBCPP_THREAD
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::__id_>;
#elif linux
    using thread_id = jugger<std::thread::native_handle_type, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_M_thread>;
#elif USE_MINGW
    using thread_id = jugger<DWORD, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::mId>;
#elif WIN32
    using thread_id = jugger<_Thrd_id_t, std::thread::id>;
    template struct robber<thread_id, &std::thread::id::_Id>;
#endif // GUIPP_WIN

    thread_id::type get_native_thread_id (std::thread::id& t) {
      return t.*rob(thread_id());
    }

  } // namespace robbery

} // namespace basepp

namespace gui {

  namespace win {

#ifdef GUIPP_WIN
    namespace detail {

      const os::event_id ACTION_MESSAGE = WM_USER + 0x101;

      overlapped_window* get_window (os::window id) {
        return reinterpret_cast<overlapped_window*>(GetWindowLongPtr(id, GWLP_USERDATA));
      }

      void set_os_window (overlapped_window* win, os::window id) {
        SetWindowLongPtr(id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
        if (win) {
          win->set_os_window(id);
        }
      }

      void unset_os_window (os::window id) {
        SetWindowLongPtr(id, GWLP_USERDATA, 0);
      }

      bool check_expose (const core::event&) {
        return false;
      }

      bool handle_by_window (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, gui::os::event_result& resultValue) {
        overlapped_window* w = get_window(hwnd);
        if (w && w->is_valid()) {
          return w->handle_event(core::event(hwnd, msg, wParam, lParam), resultValue);
        }
        return false;
      }

      void set_window_id (LONG_PTR lParam, os::window id) {
        overlapped_window* w = reinterpret_cast<overlapped_window*>(lParam);
        set_os_window(w, id);
      }

    } // namespace detail

    namespace win32 {

      LRESULT CALLBACK WindowEventProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
        case WM_INITDIALOG:
//          SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
          detail::set_window_id(lParam, hwnd);
          break;
        case WM_CREATE: {
          CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
//          clog::trace() << "WM_CREATE: " << " (" << std::hex << wParam << ", " << lParam << ") CreateParams: " << cs->lpCreateParams;
          detail::set_window_id((LONG_PTR)cs->lpCreateParams, hwnd);
          break;
        }
# ifdef KEY_DEBUG
        case WM_KEYDOWN:
          clog::debug() << "Key down 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
        case WM_KEYUP:
          clog::debug() << "Key up 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
        case WM_CHAR:
          clog::debug() << "Char 0x" << std::hex << wParam << " received (0x" << lParam << ")";
          break;
# endif // KEY_DEBUG
        }

        gui::os::event_result result = 0;
        overlapped_window* w = detail::get_window(hwnd);
        if (w) {
//          clog::trace() << "window state:" << w->get_state();
          if (w->is_valid()) {
            w->handle_event(core::event(hwnd, msg, wParam, lParam), result);
            if (result) {
              return result;
            }
          }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
      }

    } // namespace win32

#elif GUIPP_X11

    namespace x11 {

      typedef std::map<os::window, XIC> window_ic_map;
      window_ic_map s_window_ic_map;

      XIC get_window_ic (os::window id) {
        auto i = s_window_ic_map.find(id);
        if (i != s_window_ic_map.end()) {
          return i->second;
        }
        return nullptr;
      }

      XIM s_im = nullptr;
      XIMStyle s_best_style = 0x0F1F;

      typedef util::blocking_queue<std::function<simple_action>> simple_action_queue;
      simple_action_queue queued_actions;

      typedef std::map<os::window, core::rectangle> window_rectangle_map;
      window_rectangle_map s_invalidated_windows;

      void invalidate_window (os::window id, const core::rectangle& r) {
        clog::trace() << "invalidate_window: " << id;
        s_invalidated_windows[id] |= r;
      }

      void validate_window (os::window id) {
        clog::trace() << "validate_window: " << id;
        s_invalidated_windows.erase(id);
      }

    } // namespace x11

    namespace detail {
#define USE_WINDOW_MAPx
#ifdef USE_WINDOW_MAP
      typedef std::map<os::window, win::window*> window_map;
      window_map global_window_map;

      window* get_window (os::window id) {
        return global_window_map[id];
      }

      void set_os_window (window* win, os::window id) {
        global_window_map[id] = win;
        if (win) {
          win->set_os_window(id);
        }
      }

      void unset_os_window (os::window id) {
        clear_last_place(id);
        global_window_map.erase(id);
      }

#else
      overlapped_window* get_window (os::window id) {
        Atom     actual_type = 0;
        int      actual_format = -1;
        unsigned long nitems = 0;
        unsigned long bytes = 0;
        unsigned char* data = nullptr;
        overlapped_window* win = nullptr;

        const int status = XGetWindowProperty(core::global::get_instance(), id,
                                              core::x11::GUI_LIB_WIN_PTR,
                                              0, 1024, False, AnyPropertyType,
                                              &actual_type, &actual_format,
                                              &nitems, &bytes, &data);
        if ((Success == status) && (nitems == sizeof(window*))) {
#ifdef LOG_GET_WINDOW_PROPERTY
          clog::debug() << "get window " << id << ": "
                   << (int)data[0] << ' ' << (int)data[1] << ' ' << (int)data[2] << ' ' << (int)data[3] << ' '
                   << (int)data[4] << ' ' << (int)data[5] << ' ' << (int)data[6] << ' ' << (int)data[7];
#endif //LOG_GET_WINDOW_PROPERTY
          win = *(overlapped_window**)data;
        }
        if (data) {
          XFree(data);
        }
        return win;
      }

      void set_os_window (overlapped_window* win, os::window id) {
        const auto* data = (const unsigned char*)&win;
#ifdef LOG_GET_WINDOW_PROPERTY
        clog::debug() << "set window " << id << ": "
                 << (int)data[0] << ' ' << (int)data[1] << ' ' << (int)data[2] << ' ' << (int)data[3] << ' '
                 << (int)data[4] << ' ' << (int)data[5] << ' ' << (int)data[6] << ' ' << (int)data[7];
#endif //LOG_GET_WINDOW_PROPERTY
        /*int status =*/ XChangeProperty(core::global::get_instance(), id,
                        core::x11::GUI_LIB_WIN_PTR,
                        XA_CARDINAL, 8, PropModeReplace,
                        data, sizeof(win));
        if (win) {
          win->set_os_window(id);
        }
      }

      void unset_os_window (os::window id) {
        clear_last_place(id);
      }

#endif // USE_WINDOW_MAP

      bool check_expose (const core::event& e) {
        return (e.type == Expose) && (e.xexpose.count > 0);
      }

      inline win::window* get_event_window (const core::event& e) {
        switch (e.type) {
          case ConfigureNotify:
            return get_window(e.xconfigure.window);
          case NoExpose:
            return nullptr;
          default:
            return get_window(e.xany.window);
        }
      }

    } // namespace detail

#elif GUIPP_QT

    namespace detail {

      void set_os_window (overlapped_window* win, os::window id) {
        if (win) {
          win->set_os_window(id);
        }
      }

      overlapped_window* get_window (os::window id) {
        return id ? id->get_window() : nullptr;
      }

      bool check_expose (const core::event& e) {
        return (e.type() == QEvent::Type::Paint);
      }

    } // namespace detail

#endif


    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::pair<os::window, core::hot_key::call> hot_key_entry;
      typedef std::map<core::hot_key, hot_key_entry> hot_key_map;
      hot_key_map hot_keys;

      static int g_next_filter_id = 1;
      static std::thread::id main_thread_id;

      typedef std::pair<int, filter_call> filter_call_entry;
      typedef std::vector<filter_call_entry> filter_list;
      filter_list message_filters;

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const core::hot_key& hk, const core::hot_key::call& fn, window* win) {
#ifdef GUIPP_WIN
        UINT modifiers = MOD_NOREPEAT;
        if (core::control_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_CONTROL;
        }
        if (core::alt_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_ALT;
        }
        if (core::system_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_WIN;
        }
        if (core::shift_key_bit_mask::is_set(hk.get_modifiers())) {
          modifiers |= MOD_SHIFT;
        }
        os::window root = NULL;
        if (win && win->is_valid()) {
          auto& o = win->get_overlapped_window();
          root = o.get_os_window();
        }
        RegisterHotKey(root, hk.get_key(), modifiers, hk.get_key());
#elif GUIPP_X11
        auto dpy = core::global::get_instance();
        os::window root = DefaultRootWindow(dpy);
        if (win && win->is_valid()) {
          auto& o = win->get_overlapped_window();
          o.add_event_mask(KeyPressMask);
          root = o.get_os_window();
        }
        XGrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), hk.get_modifiers(), root, False, GrabModeAsync, GrabModeAsync);
#elif GUIPP_QT
        os::window root = (os::window)QGuiApplication::topLevelWindows().first();
        if (win && win->is_valid()) {
          root = win->get_overlapped_window().get_os_window();
        }
#endif
        detail::hot_keys.emplace(hk, std::make_pair(root, fn));
      }

      void unregister_hot_key (const core::hot_key& hk) {
        auto i = detail::hot_keys.find(hk);
        if (i == detail::hot_keys.end()) {
          return;
        }

        os::window root = i->second.first;

#ifdef GUIPP_WIN
        UnregisterHotKey(root, hk.get_key());
#endif // GUIPP_WIN
#ifdef GUIPP_X11
        auto dpy = core::global::get_instance();
        XUngrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), hk.get_modifiers(), root);
#endif // GUIPP_X11

        detail::hot_keys.erase(i);
      }

      int register_message_filter (const detail::filter_call& filter) {
        detail::message_filters.emplace_back(std::make_pair(detail::g_next_filter_id, filter));
        return detail::g_next_filter_id++;
      }

      void unregister_message_filter (int id) {
        auto e = detail::message_filters.end();
        auto b = detail::message_filters.begin();
        auto i = std::find_if(b, e, [id](const detail::filter_call_entry & e)->bool {
                                return e.first == id;
                              });
        if (i != e) {
          detail::message_filters.erase(i);
        }
      }

#ifdef GUIPP_WIN
      window* get_current_focus_window () {
        return detail::get_window(GetFocus());
      }

      overlapped_window& get_application_main_window () {
        GUITHREADINFO info;
        memset(&info, 0, sizeof(info));
        info.cbSize = sizeof(GUITHREADINFO);
        if (GetGUIThreadInfo(util::robbery::get_native_thread_id(detail::main_thread_id), &info)) {
          HWND win = info.hwndActive;
          if (win) {
            HWND parent = GetParent(win);
            while (parent) {
              win = parent;
              parent = parent = GetParent(win);
            }
          }
          auto* w = detail::get_window(win);
          if (w) {
            return w->get_overlapped_window();
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }

      void register_utf8_window (const window&) {}

      void unregister_utf8_window (const window&) {}

#endif // GUIPP_WIN

#ifdef GUIPP_QT

      window* get_current_focus_window () {
        return detail::get_window(static_cast<os::window>(QGuiApplication::focusWindow()));
      }

      overlapped_window& get_application_main_window() {
        auto list = QGuiApplication::topLevelWindows();
        if (list.size() > 0) {
          auto* win = detail::get_window(static_cast<os::window>(list.first()));
          if (win) {
            return win->get_overlapped_window();
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }

      void register_utf8_window (const window&) {}

      void unregister_utf8_window (const window&) {}

#endif // GUIPP_QT

      std::thread::id get_current_thread_id () {
        return std::this_thread::get_id();
      }

#ifdef GUIPP_X11
      window* get_current_focus_window () {
        Window focus = 0;
        int revert_to = 0;
        if (XGetInputFocus(core::global::get_instance(), &focus, &revert_to) < BadValue) {
          return detail::get_window(focus);
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
            auto win = detail::get_window(children[i]);
            if (win) {
              return win->get_overlapped_window();
            }
          }
        }
        throw std::runtime_error("application_main_window could not be found!");
      }
      
      void register_utf8_window (const window& win) {
        os::window id = win.get_overlapped_window().get_os_window();

        if (!id) {
          return;
        }

        if (!x11::s_im) {
          x11::s_im = XOpenIM(core::global::get_instance(), nullptr, nullptr, nullptr);
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

      void unregister_utf8_window (const window& win) {
        os::window id = win.get_overlapped_window().get_os_window();

        auto i = x11::s_window_ic_map.find(id);
        if (i != x11::s_window_ic_map.end()) {
          XIC ic = i->second;
          if (ic) {
            XDestroyIC(ic);
          }
          x11::s_window_ic_map.erase(id);
        }
      }

#endif // GUIPP_X11

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
#ifdef GUIPP_WIN
      if (e.type == WM_HOTKEY) {
#elif GUIPP_X11
      if (e.type == KeyPress) {
#elif GUIPP_QT
      if (e.type() == QEvent::KeyPress) {
#endif
        core::hot_key hk(get_key_symbol(e), get_key_state(e));
        auto i = detail::hot_keys.find(hk);
        if (i != detail::hot_keys.end()) {
          i->second.second();
          return true;
        }
      }

      return false;
    }

#ifdef GUIPP_WIN
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
        GetWindowRect(w.get_overlapped_window().get_os_window(), &r);
        return !PtInRect(&r, pt);
      }
      }
      return false;
    }

#elif GUIPP_X11
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

        core::rectangle area = w.absolute_place();
        return !area.is_inside(core::point(os::point{static_cast<decltype(os::point::x)>(x),
                                                     static_cast<decltype(os::point::x)>(y)}));
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
          clog::fatal() << "exception in run_main_loop: " << ex;
        } catch (...) {
          clog::fatal() << "Unknown exception in run_main_loop()";
        }
      }
    }

    inline bool is_expose_event (const core::event& e) {
      return (e.type == Expose);// || (e.type == GraphicsExpose);// || (e.type == NoExpose);
    }

#elif GUIPP_QT

    bool is_button_event_outside (const window& w, const core::event& e) {
      switch (e.type()) {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease: {
          const QMouseEvent& me = e.cast<QMouseEvent>();
          return !w.absolute_place().is_inside(core::point(me.globalPos()));
        }
        default: break;
      }
      return false;
    }

#endif

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, const detail::filter_call& filter) {

      running = true;

#ifdef GUIPP_WIN
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
#elif GUIPP_X11
      gui::os::instance display = core::global::get_instance();
      gui::os::event_result resultValue = 0;

      core::event e;
      std::function<simple_action> action;

      // http://www.linuxquestions.org/questions/showthread.php?p=2431345#post2431345
      // Create a File Description Set containing x11_fd
      auto x11_fd = ConnectionNumber(display);

      fd_set in_fds;

      while (running) {

        while (x11::queued_actions.try_dequeue(action)) {
          action();
        }

        FD_ZERO(&in_fds);
        FD_SET(x11_fd, &in_fds);

        // Define our timeout. Ten milliseconds sounds good.
        timeval timeout {.tv_sec = 0, .tv_usec = 50000 };
        // Wait for next XEvent or a timer out
        const int num_ready_fds = select(x11_fd + 1, &in_fds, nullptr, nullptr, &timeout);

        if (num_ready_fds < 0) {
          clog::debug() << "select returned: " << num_ready_fds;
        } else if (num_ready_fds > 0) while (XPending(display) && running) {

          XNextEvent(display, &e);

          if (!win::is_frequent_event(e)) {
            clog::trace() << e;
          }

          if (filter && filter(e)) {
            continue;
          }

          if (is_expose_event(e)) {
            x11::invalidate_window(e.xany.window);
          } else {
            process_event(e, resultValue);
          }
          if (protocol_message_matcher<core::x11::WM_DELETE_WINDOW>(e) && !resultValue) {
            running = false;
          }

          if ((e.type == ConfigureNotify) && running) {
            update_last_place(e.xconfigure.window, get<core::rectangle, XConfigureEvent>::param(e));
          }

        };

        for (auto& w : x11::s_invalidated_windows) {
          win::window* win = detail::get_window(w.first);
          if (win && win->is_visible()) {
            win->redraw();
          }
        }
        x11::s_invalidated_windows.clear();

//        std::this_thread::sleep_for(std::chrono::milliseconds(10));

      }
      return resultValue;
#elif GUIPP_QT
      QEventLoop event_loop;
      return event_loop.exec(QEventLoop::AllEvents);
#endif // GUIPP_QT
    }

    namespace {
      bool main_loop_is_running = false;
    }

    int run_main_loop () {
      main_loop_is_running = true;
      detail::main_thread_id = global::get_current_thread_id();
#ifdef GUIPP_QT
      return gui::core::global::get_instance()->exec();
#else
      return run_loop(main_loop_is_running, [] (const core::event & e) {
        return detail::check_expose(e) || check_message_filter(e) || check_hot_key(e);
      });
#endif // GUIPP_QT
    }

    void quit_main_loop () {
      clog::debug() << "Received quit_main_loop()";
      main_loop_is_running = false;
      core::global::fini();
#ifdef GUIPP_QT
      gui::core::global::get_instance()->exit();
#endif // GUIPP_QT
    }

    void run_on_main (const std::function<void()>& action) {
#ifdef GUIPP_X11
      x11::queued_actions.enqueue(action);
#endif // GUIPP_X11
#ifdef GUIPP_WIN
      PostThreadMessage(util::robbery::get_native_thread_id(detail::main_thread_id),
                        detail::ACTION_MESSAGE,
                        0,
                        (ULONG_PTR)new std::function<void()>(action));
#endif // GUIPP_WIN
    }
  }   // win
} // gui
