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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"
#include "window_event_proc.h"

namespace gui {

  bool handle_by_window(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, event_result& resultValue) {
    window* win = window::get(hwnd);
    if (win && win->is_valid()) {
      return win->handle_event(window_event(hwnd, msg, wParam, lParam), resultValue);
    }
    return false;
  }

  void set_id(window* win, window_id id) {
    win->id = id;
  }

  void set_window_id(LONG_PTR lParam, window_id id) {
    window* win = reinterpret_cast<window*>(lParam);
    set_id(win, id);
  }

  LRESULT CALLBACK WindowEventProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    event_result result = 0;
    switch (msg) {
      case WM_INITDIALOG:
        SetWindowLongPtr(hwnd, GWLP_USERDATA, lParam);
        set_window_id(lParam, hwnd);
        handle_by_window(hwnd, msg, wParam, lParam, result);
        return TRUE;
      case WM_DESTROY:
        if (handle_by_window(hwnd, msg, wParam, lParam, result)) {
          return result;
        } else {
          LONG style = GetWindowLong(hwnd, GWL_STYLE);
          if (!(style & (WS_CHILD | WS_POPUP))) {
            PostQuitMessage(0);
          }
        }
        return 0;
      case WM_CREATE: {
        CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
        set_window_id((LONG_PTR)cs->lpCreateParams, hwnd);
      }
      // fall throught!
      default:
        if (handle_by_window(hwnd, msg, wParam, lParam, result)) {
          return result;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
  }

}
