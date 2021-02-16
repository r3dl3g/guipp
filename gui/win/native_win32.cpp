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

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/win/native.h>
#include <gui/win/overlapped_window.h>

#define NO_CAPTURExx


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace {
      std::map<const char*, class_info> window_class_info_map;
    }

    // --------------------------------------------------------------------------
    namespace native {

      struct disable_wheel_hook {
        disable_wheel_hook ()
          : hook_id(0)
        {}

        void enable ();
        void disable ();

      private:
        HHOOK hook_id;
      };

      disable_wheel_hook s_wheel_hook;

      LRESULT CALLBACK mouseInputHook (int nCode, WPARAM wParam, LPARAM lParam) {
        //"if nCode is less than zero, the hook procedure must pass the message to the CallNextHookEx function
        //without further processing and should return the value returned by CallNextHookEx"
        if ((nCode >= 0) && ((wParam == WM_MOUSEWHEEL || wParam == WM_MOUSEHWHEEL))) {
          return -1;
        }

        return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
      }

      void disable_wheel_hook::enable () {
        if (!hook_id) {
          hook_id = SetWindowsHookEx(WH_MOUSE_LL, mouseInputHook, NULL, 0);
        }
      }

      void disable_wheel_hook::disable () {
        if (hook_id) {
          UnhookWindowsHookEx(hook_id);
          hook_id = 0;
        }
      }

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
        SetWindowPos(w, nullptr, npt.x(), npt.y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
      }

      void resize (os::window w, const core::size& sz) {
        SetWindowPos(w, nullptr, 0, 0, sz.os_width(), sz.os_height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
      }

      void place (os::window w, const core::rectangle& r) {
        const auto nr = core::global::scale_to_native(r);
        MoveWindow(w, nr.x(), nr.y(), nr.width(), nr.height(), false);
      }

      void notify_move (window& w, const core::point& pt, const core::point&) {
        const auto npt = core::global::scale_to_native(pt);
        gui::os::event_result result = 0;
        gui::core::event e{0, WM_MOVE, 0, MAKELPARAM(npt.x(), npt.y())};
        w.handle_event(e, result);
      }

      void notify_resize (window& w, const core::size& sz, const core::size&) {
        gui::os::event_result result = 0;
        gui::core::event e{0, WM_SIZE, 0, MAKELPARAM(sz.os_width(), sz.os_height())};
        w.handle_event(e, result);
      }

      core::point get_geometry (os::window w) {
        RECT r;
        GetWindowRect(w, &r);
        return core::global::scale_from_native(core::native_point{os::get_x(r), os::get_y(r)});
      }

      void prepare (overlapped_window&) {}

      os::window create (const class_info& type,
                         const core::rectangle& r,
                         os::window parent_id,
                         win::overlapped_window& data) {
        auto display = core::global::get_instance();

        os::brush back = ((type.get_background() > 0) && (type.get_background() < 20))
                         ? reinterpret_cast<os::brush>(static_cast<LPARAM>(type.get_background()))
                         : (color::extract<color::part::alpha>(type.get_background()) == 0xff)
                           ? NULL : CreateSolidBrush(type.get_background());

        std::string name(type.get_class_name(), 0, 255);
        WNDCLASS wc = {
          /* Register the window class. */
          type.get_class_style(),
          win32::WindowEventProc,
          0,
          0,
          display,
          nullptr,
          type.get_cursor(),
          back,
          nullptr,
          name.c_str()
        };

        ATOM cls_id = RegisterClass(&wc);
        if (!cls_id) {
          auto error_no = GetLastError();
          if (error_no != 1410) {
            clog::trace() << getLastErrorText() << " class name length: " << name.length();
          }
        }
        const auto nr = core::global::scale_to_native(r);

        os::window id = CreateWindowEx(type.get_ex_style(),
                                       name.c_str(),
                                       nullptr,         // address of window text
                                       type.get_style(),  // window style
                                       nr.x(),        // horizontal position of window
                                       nr.y(),        // vertical position of window
                                       nr.width(),    // window width
                                       nr.height(),   // window height
                                       parent_id,       // handle of parent window
                                       NULL,            // handle of menu or child-window identifier
                                       display,         // handle of application instance
                                       &data);

//        SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)(&data));

        return id;
      }

      void destroy (os::window w) {
        if (w) {
          DestroyWindow(w);
        }
      }

      void close (os::window id) {
        if (id) {
          CloseWindow(id);
        }
      }

      void notify_close (window&) {}

      void prepare_win_for_event (const overlapped_window&) {}

      bool is_visible (os::window id) {
        return id && IsWindowVisible(id);
      }

      void set_visible (os::window id, bool s) {
        if (id) {
          ShowWindow(id, s ? SW_SHOWNA : SW_HIDE);
        }
      }

      void enable (overlapped_window& w, bool on) {
        gui::os::style ws = w.get_window_class().get_style();
        if ((ws & WS_POPUP) != WS_POPUP) {
          // For WS_POPUP EnableWindow(, false) causes an empty window.
          EnableWindow(w.get_os_window(), on);
        }
      }

      void to_front (os::window id) {
        if (id) {
          SetWindowPos(id, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }
      }

      void to_back (os::window id) {
        if (id) {
          SetWindowPos(id, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }
      }

      void take_focus (os::window id) {
        SetFocus(id);
      }

      void set_cursor (os::window id, const os::cursor& c) {
        SetClassLongPtr(id, GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(c));
      }

      void invalidate (os::window id, const core::native_rect& r) {
        RECT rect = gui::os::mk_rectangle(r.x(), r.y(), r.x2(), r.y2());
        InvalidateRect(id, &rect, TRUE);
      }

      void redraw (window&, os::window id, const core::native_rect& r) {
        RECT rect = gui::os::mk_rectangle(r.x(), r.y(), r.x2(), r.y2());
        RedrawWindow(id, &rect, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASENOW);
      }

      void prepare_accept_focus (os::window, bool) {
      }

      void capture_pointer (os::window id) {
        s_wheel_hook.enable();
        SetCapture(id);
      }

      void uncapture_pointer (os::window id) {
        ReleaseCapture();
        s_wheel_hook.disable();
      }

      os::window get_desktop_window () {
        return GetDesktopWindow();
      }

      core::size screen_size () {
        return core::size(gui::os::size{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
      }

      core::rectangle screen_area () {
        return core::rectangle(screen_size());
      }

      core::rectangle adjust_overlapped_area (const core::rectangle& r, const class_info& type) {
        const auto nr = core::global::scale_to_native(r);
        RECT rect = gui::os::mk_rectangle(nr.x(), nr.y(), nr.x2(), nr.y2());
        AdjustWindowRectEx(&rect, type.get_style(), FALSE, type.get_ex_style());
        return core::global::scale_from_native(core::native_rect(os::get_x(rect), os::get_y(rect),
                                                                 os::get_width(rect), os::get_height(rect)));
      }

      void prepare_overlapped (os::window id, os::window pid) {
      }

      os::window get_overlapped_parent (os::window pid) {
        return pid;
      }

      void set_title (os::window id, const std::string& title) {
        SendMessage(id, WM_SETTEXT, 0, (LPARAM)title.c_str());
      }

      std::string get_title (os::window id) {
        std::string s;
        s.resize(SendMessage(id, WM_GETTEXTLENGTH, 0, 0) + 1);
        SendMessage(id, WM_GETTEXT, (WPARAM)s.capacity(), (LPARAM)&s[0]);
        return s;
      }

      bool is_maximized (os::window id) {
        return IsZoomed(id) != FALSE;
      }

      bool is_minimized (os::window id) {
        return IsIconic(id) != FALSE;
      }

      bool is_top_most (os::window id) {
        return (GetWindowLong(id, GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
      }

      void minimize (os::window id) {
        ShowWindow(id, SW_MINIMIZE);
      }

      void maximize (os::window id) {
        ShowWindow(id, SW_MAXIMIZE);
      }

      void restore (os::window id) {
        ShowWindow(id, SW_RESTORE);
      }

      void set_top_most (os::window id, bool on) {
        SetWindowPos(id, on ? HWND_TOPMOST : HWND_NOTOPMOST,
                     0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }

      void prepare_main_window (os::window) {}
      void prepare_popup_window (os::window) {}
      void prepare_dialog_window (os::window, os::window) {}

      void erase (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        auto brush = CreateSolidBrush(c);
        auto old = SelectObject(gc, brush);
        Rectangle(gc, r.x(), r.y(), r.x2(), r.y2());
        SelectObject(gc, old);
        DeleteObject(brush);
      }

      void frame (os::drawable id, os::graphics gc, const core::native_rect& r, os::color c) {
        SelectObject(gc, GetStockObject(NULL_BRUSH));
        auto pen = CreatePen(PS_SOLID, 1, c);
        auto old = SelectObject(gc, pen);
        Rectangle(gc, r.x(), r.y(), r.x2(), r.y2());
        SelectObject(gc, old);
        DeleteObject(pen);
      }

      os::backstore create_surface (const core::native_size& size, os::window id) {
        auto dc = GetDC(NULL);
        auto bmp = CreateCompatibleBitmap(dc, size.width(), size.height());
        ReleaseDC(NULL, dc);
        return bmp;
      }

      void delete_surface (os::backstore id) {
        DeleteObject(id);
      }

      void copy_surface (os::bitmap src, os::drawable target, os::graphics context,
                         const core::native_point& from, const core::native_point& to,
                         const core::native_size& size) {
        auto dc = GetDC(NULL);
        auto sdc = CreateCompatibleDC(dc);
        auto old = SelectObject(sdc, src);
        SelectObject(context, target);
        BitBlt(context, to.x(), to.y(), size.width(), size.height(), sdc, from.x(), from.y(), SRCCOPY);
        SelectObject(sdc, old);
        DeleteDC(sdc);
        ReleaseDC(NULL, dc);
      }

      void send_client_message (window* win, os::message_type message, long l1, long l2) {
        if (win && win->is_valid()) {
          core::event e{ NULL, message, static_cast<WPARAM>(l1), static_cast<LPARAM>(l2) };
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, void* v1, void* v2) {
        if (win && win->is_valid()) {
          core::event e { NULL, message, (WPARAM)v1, (LPARAM)v2 };
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, const core::size& sz) {
        if (win && win->is_valid()) {
          os::size s = sz.os();
          long l2 = (long)s.cy << 16 | (long)s.cx;
          core::event e{ NULL, message, 0, static_cast<LPARAM>(l2)};
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

      void send_client_message (window* win, os::message_type message, const core::rectangle& wr) {
        if (win && win->is_valid()) {
          core::native_rect r = core::global::scale_to_native(wr);
          WINDOWPOS wp{NULL, NULL, r.x(), r.y(), static_cast<int>(r.width()), static_cast<int>(r.height()), 0};
          core::event e { NULL, message, 0, reinterpret_cast<LPARAM>(&wp)};
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

      void send_mouse_event (window* win, bool enter) {
        if (win && win->is_valid()) {
          core::event e{ NULL, (gui::os::event_id)(enter ? WM_MOUSEMOVE : WM_MOUSELEAVE), 0, 0};
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

      void send_notify_visibility (window* win, bool visible) {
        if (win && win->is_valid()) {
          core::event e{ NULL, WM_SHOWWINDOW, visible, 0};
          gui::os::event_result result;
          win->handle_event(e, result);
        }
      }

    } // namespace native

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#endif // GUIPP_WIN
