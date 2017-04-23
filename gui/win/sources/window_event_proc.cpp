/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
          case WM_DESTROY: {
            os::event_result result = 0;
            if (handle_by_window(hwnd, msg, wParam, lParam, result)) {
              return result;
            } else {
              LONG style = GetWindowLong(hwnd, GWL_STYLE);
              if (!(style & (WS_CHILD | WS_POPUP))) {
                PostQuitMessage(0);
              }
            }
            return 0;
          }
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
        global_window_map.erase(id);
      }

#endif // X11

    } // detail

#ifdef WIN32
    int run_main_loop() {
      MSG msg;
      while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      return (int)msg.wParam;
    }
#endif // WIN32

#ifdef X11

    int run_main_loop () {
      Atom wmDeleteMessage = XInternAtom(core::global::get_instance(), "WM_DELETE_WINDOW", False);

      os::event_result resultValue = 0;
      core::event e;
      bool running = true;
      while (running) {
//        while (XPending(core::global::get_instance())) {
          XNextEvent(core::global::get_instance(), &e);
          win::window* win = win::detail::get_window(e.xany.window);
          if (win && win->is_valid()) {
            if (e.type == CreateNotify) {
              XSetWMProtocols(e.xany.display, e.xany.window, &wmDeleteMessage, 1);
            } else if ((e.type == ClientMessage) && (e.xclient.data.l[0] == wmDeleteMessage)) {
              running = false;
            }
            try {
              win->handle_event(core::event(e), resultValue);
              XFlush(e.xany.display);
            } catch (std::exception e) {
              LogFatal << "exception in run_main_loop:" << e;
            } catch (...) {
              LogFatal << "Unknown exception in run_main_loop()";
            }
          }
//        }
      }
      return resultValue;
    }

#endif // X11

  } // win

} // gui
