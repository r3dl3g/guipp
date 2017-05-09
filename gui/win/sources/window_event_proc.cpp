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
#include <logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_proc.h"

namespace gui {

  namespace win {

    namespace detail {

      void set_id (window* w,
                   os::window id) {
        w->id = id;
      }

#ifdef WIN32

      window* get_window (os::window id) {
        return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
      }

      void set_window(os::window id, window* win) {
        SetWindowLongPtr(id, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win));
      }
      
      void unset_window(os::window id) {
        SetWindowLongPtr(id, GWLP_USERDATA, 0);
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

      LRESULT CALLBACK WindowEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch (msg) {
          case WM_INITDIALOG:
            SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
            set_window_id(lParam, hwnd);
            break;
          case WM_CREATE: {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            set_window_id((LONG_PTR)cs->lpCreateParams, hwnd);
            break;
          }
#ifdef KEY_DEBUG
          case WM_KEYDOWN:
            LogDebug << "Key down 0x" << std::hex << wParam << " received (0x" << lParam << ")";
            break;
          case WM_KEYUP:
            LogDebug << "Key up 0x" << std::hex << wParam << " received (0x" << lParam << ")";
            break;
          case WM_CHAR:
            LogDebug << "Char 0x" << std::hex << wParam << " received (0x" << lParam << ")";
            break;
#endif // KEY_DEBUG
        }

        os::event_result result = 0;
        window* w = get_window(hwnd);
        if (w && w->is_valid()) {
          w->handle_event(core::event(hwnd, msg, wParam, lParam), result);
          if (result) {
            return result;
          }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
      }

#endif // WIN32
#ifdef X11
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

#endif // X11

    } // detail

    // --------------------------------------------------------------------------
    namespace detail {

      typedef std::map<hot_key, hot_key::call> hot_key_map;
      hot_key_map hot_keys;

    }

    namespace global {

      // --------------------------------------------------------------------------
      void register_hot_key (const hot_key& hk, const hot_key::call& fn, window* win) {
        detail::hot_keys[hk] = fn;

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
        RegisterHotKey(win ? win->get_id() : NULL, hk.get_key(), modifiers, hk.get_key());
#endif // WIN32
#ifdef X11
        auto dpy = core::global::get_instance();
        os::window root = win ? win->get_id() : DefaultRootWindow(dpy);
        XGrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), AnyModifier, root, False, GrabModeAsync, GrabModeAsync);
        XSelectInput(dpy, root, KeyPressMask );
#endif // X11
      }

      void unregister_hot_key (const hot_key& hk, window* win) {
        detail::hot_keys.erase(hk);

#ifdef WIN32
        RegisterHotKey(win ? win->get_id() : NULL, hk.get_key());
#endif // WIN32
#ifdef X11
        auto dpy = core::global::get_instance();
        os::window root = win ? win->get_id() : DefaultRootWindow(dpy);
        XUngrabKey(dpy, XKeysymToKeycode(dpy, hk.get_key()), AnyModifier, root);
#endif // X11
      }
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
          i->second();
          return true;
        }
      }
      return false;
    }

    // --------------------------------------------------------------------------
    int run_loop (volatile bool& running, detail::filter_call filter) {

      running = true;

#ifdef WIN32
      MSG msg;
      while (running && GetMessage(&msg, nullptr, 0, 0)) {
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

      detail::init_message(detail::WM_DELETE_WINDOW, "WM_DELETE_WINDOW");

      os::event_result resultValue = 0;

      core::event e;

      while (running) {
        XNextEvent(display, &e);

        if (filter && filter(e)) {
          continue;
        }

        win::window* win = win::detail::get_window(e.xany.window);

        if (win && win->is_valid()) {
          if ((e.type == ClientMessage) && (e.xclient.data.l[0] == detail::WM_DELETE_WINDOW)) {
            running = false;
          }

          try {
            win->handle_event(e, resultValue);
          } catch (std::exception& ex) {
            LogFatal << "exception in run_main_loop: " << ex;
          } catch (...) {
            LogFatal << "Unknown exception in run_main_loop()";
          }

          core::global::sync();

          if (e.type == ConfigureNotify) {
              update_last_place(e.xconfigure.window, core::rectangle(e.xconfigure));
          }
        }
      }
      return resultValue;
#endif // X11
    }

    int run_main_loop () {
      bool running = true;

      return run_loop(running, [](const core::event& e) {
        return check_hot_key(e);
      });

    }

  } // win

} // gui
