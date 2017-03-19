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
#include "control.h"
#include "window_event_proc.h"

namespace gui {

  namespace win {

    namespace detail {

      void set_id (window* w,
                   core::window_id id) {
        w->id = id;
      }

#ifdef WIN32

      window* get__window (core::window_id id) {
        return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
      }

      bool handle_by_window (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, core::event_result& resultValue) {
        window* w = get_window(hwnd);
        if (w && w->is_valid()) {
          return w->handle_event(core::event(hwnd, msg, wParam, lParam), resultValue);
        }
        return false;
      }

      void set_window_id (LONG_PTR lParam, core::window_id id) {
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
            core::event_result result = 0;
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
          case WM_COMMAND: {
            HWND id = (HWND)lParam;
            if (id && (id != hwnd)) {
              // forward to child
              return SendMessage(id, msg, wParam, lParam);
            }
            break;
          }
          case WM_MEASUREITEM: {
            PMEASUREITEMSTRUCT pmis = (PMEASUREITEMSTRUCT)lParam;
            const core::size& sz = owner_draw::get_item_size((int)pmis->CtlID);
            if (sz.height() || sz.width()) {
              pmis->itemHeight = sz.height();
              pmis->itemWidth = sz.width();
              return true;
            }
            break;
          }
          case WM_DRAWITEM: {
            PDRAWITEMSTRUCT pdis = (PDRAWITEMSTRUCT)lParam;
            HWND id = pdis->hwndItem;
            if (id && (id != hwnd)) {
              // forward to child
              return SendMessage(id, msg, wParam, lParam);
            }
            break;
          }
          case WM_VSCROLL:
          case WM_HSCROLL: {
            HWND id = (HWND)lParam;
            if (id && (id != hwnd)) {
              SendMessage(id, msg, wParam, lParam);
              return 0;
            }
            break;
          }
          case WM_CREATE: {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
            set_window_id((LONG_PTR)cs->lpCreateParams, hwnd);
            break;
          }
        }

        core::event_result result = 0;
        window* w = get_window(hwnd);
        if (w && w->is_valid()) {
          w->handle_event(core::event(hwnd, msg, wParam, lParam), result);

          const window_class* cls = w->get_window_class();
          if (cls && cls->get_callback() && (cls->get_callback() != WindowEventProc)) {
            return CallWindowProc(cls->get_callback(), hwnd, msg, wParam, lParam);
          }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
      }

#endif // WIN32
#ifdef X11
      typedef std::map<core::window_id, win::window*> window_map;
      window_map global_window_map;

      window* get_window (core::window_id id) {
        return global_window_map[id];
      }

      void set_window (core::window_id id, window* win) {
        global_window_map[id] = win;
      }

      void unset_window (core::window_id id) {
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

      core::event_result resultValue = 0;
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
