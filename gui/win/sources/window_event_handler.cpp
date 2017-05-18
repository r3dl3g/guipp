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
* @brief     C++ API: basic window
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
#include "window_event_handler.h"
#include "window.h"
#ifdef WIN32
#include <string_util.h>
#endif // WIN32


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<>
    bool get_param<0, bool>(const core::event& e) {
      return LOWORD(e.wParam) != 0;
    }
    // --------------------------------------------------------------------------
    template<>
    unsigned int get_param<0, unsigned int>(const core::event& e) {
      return (unsigned int)e.wParam;
    }
    // --------------------------------------------------------------------------
    template<>
    int get_param<0, int>(const core::event& e) {
      return (int)e.wParam;
    }
    // --------------------------------------------------------------------------
    template<>
    window* get_param<0, window*>(const core::event& e) {
      return detail::get_window((os::window)e.wParam);
    }
    // --------------------------------------------------------------------------
    template<>
    os::graphics get_param<0, os::graphics>(const core::event& e) {
      return (os::graphics)e.wParam;
    }
    // --------------------------------------------------------------------------
    template<>
    window* get_param<1, window*>(const core::event& e) {
      return detail::get_window((os::window)e.lParam);
    }
    // --------------------------------------------------------------------------
    template<>
    core::point get_param<1, core::point>(const core::event& e) {
      return core::point(e.lParam);
    }
    // --------------------------------------------------------------------------
    template<>
    core::size get_param<1, core::size>(const core::event& e) {
      return core::size(e.lParam);
    }
    // --------------------------------------------------------------------------
    core::point get_root_mouse_pos (const core::event& e) {
      POINT pt = { GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam) };
      ClientToScreen(e.id, &pt);
      return core::point(pt);
    }
    // --------------------------------------------------------------------------
    window* get_window_from_cs(const core::event& e) {
      CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(e.lParam);
      return detail::get_window(cs->hwndParent);
    }
    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp(const core::event& e) {
      WINDOWPOS* p = reinterpret_cast<WINDOWPOS*>(e.lParam);
      return p->flags;
    }
    // --------------------------------------------------------------------------
    core::point_type get_wheel_delta(const core::event& e) {
      return static_cast<core::point_type>(signum(GET_WHEEL_DELTA_WPARAM(e.wParam)));
    }
    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return static_cast<os::key_state>((GetKeyState(VK_SHIFT) & 0x8000 ? MK_SHIFT : 0) |
                                        (GetKeyState(VK_CONTROL) & 0x8000 ? MK_CONTROL : 0) |
                                        (GetKeyState(VK_MENU) & 0x8000 ? MK_MENU : 0) |
                                        (GetKeyState(VK_LWIN) & 0x8000 ? MK_SYTEM : 0) |
                                        (GetKeyState(VK_RWIN) & 0x8000 ? MK_SYTEM : 0));
    }

    // --------------------------------------------------------------------------
    int get_key_repeat_count (const core::event& e) {
      return e.lParam & 0x0ffff;
    }
    // --------------------------------------------------------------------------
    unsigned int get_key_scan_code (const core::event& e) {
      UINT sc = (e.lParam >> 16) & 0x01FF;
      return sc;
    }
    // --------------------------------------------------------------------------
    os::key_symbol get_key_symbol (const core::event& e) {
      return (os::key_symbol)e.wParam;
    }
    // --------------------------------------------------------------------------
    std::string get_key_chars (const core::event& e) {
      WCHAR wbuffer[4] = { 0 };

      BYTE kb[256];
      GetKeyboardState(kb);

      UINT vk = get_key_symbol(e);
      UINT sc = get_key_scan_code(e);
      int len = ToUnicode(vk, sc, kb, wbuffer, 4, 0);

      //WORD buffer = 0;
      //int len2 = ToAscii(vk, sc, kb, &buffer, 0);

      //UINT sc2 = MapVirtualKey(e.wParam, MAPVK_VK_TO_VSC);
      //UINT vk2 = MapVirtualKey(sc, MAPVK_VSC_TO_VK);
      //wchar_t ch = MapVirtualKey(e.wParam, MAPVK_VK_TO_CHAR);
      return ibr::string::utf16_to_utf8(std::wstring(wbuffer, len));
    }
    // --------------------------------------------------------------------------
    static std::map<os::window, bool> s_mouse_inside;

    bool mouse_enter_matcher::operator() (const core::event& e) {
      switch (e.type) {
      case WM_MOUSEMOVE:
        if (!s_mouse_inside[e.id]) {
          s_mouse_inside[e.id] = true;
          TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), 0 };
          tme.dwFlags = TME_LEAVE;
          tme.hwndTrack = e.id;
          TrackMouseEvent(&tme);
          return true;
        }
        break;
      case WM_MOUSELEAVE:
        s_mouse_inside[e.id] = false;
        break;
      }
      return false;
    }

    // --------------------------------------------------------------------------
    void os_paint_caller::operator()(const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(id);
        EndPaint(e.id, &ps);
      }
    }

    // --------------------------------------------------------------------------
    void pos_changing_caller::operator() (const core::event& e) {
      if (f) {
        LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.lParam);
        core::rectangle r = get_rect<WINDOWPOS>(e);
        f(r);
        p->x = r.os_x();
        p->y = r.os_y();
        p->cx = r.os_width();
        p->cy = r.os_height();
      }
    }

    // --------------------------------------------------------------------------
    void get_minmax_caller::operator() (const core::event& e) {
      if (callback) {
        LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(e.lParam);
        core::size mi(info->ptMinTrackSize);
        core::size ma(info->ptMaxTrackSize);
        core::size sz(info->ptMaxSize);
        core::point pos(info->ptMaxPosition);
        callback(sz, pos, mi, ma);
        info->ptMinTrackSize = mi;
        info->ptMaxTrackSize = ma;
      }
    }

    void send_client_message (window* win, os::event_id message, long l1, long l2) {
      SendMessage(win->get_id(), message, static_cast<WPARAM>(l1), static_cast<LPARAM>(l2));
    }

#endif // Win32
#ifdef X11
    namespace detail {
      Atom WM_CREATE_WINDOW = 0;
      Atom WM_DELETE_WINDOW = 0;
      Atom WM_PROTOCOLS = 0;
      std::map<Window, XIC> s_window_ic_map;

      void init_message (Atom& message, const char* name) {
        if (!message) {
          message = XInternAtom(core::global::get_instance(), name, False);
        }
      }
    }
    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return static_cast<os::key_state>(get_state<XKeyEvent>(e));
    }
    // --------------------------------------------------------------------------
    os::key_symbol get_key_symbol (const core::event& e) {
      return XLookupKeysym(const_cast<XKeyEvent*>(&e.xkey), 0);
    }
    // --------------------------------------------------------------------------
    std::string get_key_chars (const core::event& e) {
      std::map<Window, XIC>::iterator i = detail::s_window_ic_map.find(e.xany.window);
      if (i != detail::s_window_ic_map.end()) {
        XIC ic = i->second;
        Status status;
        char text[8] = {0};
        KeySym keySym = 0;
        int len = Xutf8LookupString(ic, const_cast<XKeyEvent*>(&e.xkey), text, 8, &keySym, &status);
        if ((status == XLookupChars) || (status == XLookupBoth)) {
          return std::string(text, len);
        }
      } else {
        char text[8] = {0};
        KeySym keySym = 0;
        int len = XLookupString(const_cast<XKeyEvent*>(&e.xkey), text, 8, &keySym, 0);
        return std::string(text, len);
      }
      return std::string();
    }

    // --------------------------------------------------------------------------
    template<>
    os::graphics get_param<0, os::graphics> (const core::event& e) {
      return DefaultGCOfScreen(DefaultScreenOfDisplay(e.xany.display));
    }

    // --------------------------------------------------------------------------
    void send_client_message (window* win, Atom message, long l1, long l2, long l3, long l4, long l5) {
      if (win && win->is_valid()) {
        XClientMessageEvent client;

        client.type         = ClientMessage;
        client.serial       = 0;
        client.display      = core::global::get_instance();
        client.window       = win->get_id();
        client.message_type = message;
        client.format       = 32;
        client.data.l[0]    = l1;
        client.data.l[1]    = l2;
        client.data.l[2]    = l3;
        client.data.l[3]    = l4;
        client.data.l[4]    = l5;

        /* Send the data off to the other process */
        XSendEvent(client.display, client.window, True, 0, (XEvent*)&client);

//        core::global::sync();
      }
    }

    void send_client_message (window* win, Atom message, window* w, const core::rectangle& rect) {
      XRectangle r = rect;
      long l1 = (long)r.x << 16 | (long)r.y;
      long l2 = (long)r.width << 16 | (long)r.height;
      send_client_message(win, message, w->get_id(), l1, l2);
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect(const core::event& e) {
      long p = e.xclient.data.l[1], s = e.xclient.data.l[2];
      short x = p >> 16;
      short y = p & 0xffff;
      unsigned short w = s >> 16;
      unsigned short h = s & 0xffff;
      os::rectangle r = { x, y, w, h };
      return core::rectangle(r);
    }

    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*>(const core::event& e) {
      os::window id = e.xclient.data.l[0];
      return detail::get_window(id);
    }

    // --------------------------------------------------------------------------
    core::point get_root_mouse_pos (const core::event& e) {
      auto me = (cast_event_type<XMotionEvent>(e));
      return core::point(me.x_root, me.y_root);
    }

    // --------------------------------------------------------------------------
    std::map<Window, core::size> s_last_size;
    std::map<Window, core::point> s_last_pos;
    std::map<Window, core::rectangle> s_last_place;

    template<>
    core::size& get_last_place<core::size>(Window w) {
      return s_last_size[w];
    }

    template<>
    core::point& get_last_place<core::point>(Window w) {
      return s_last_pos[w];
    }

    template<>
    core::rectangle& get_last_place<core::rectangle>(Window w) {
      return s_last_place[w];
    }

    void update_last_place (os::window w, const core::rectangle& r) {
      get_last_place<core::size>(w) = r.size();
      get_last_place<core::point>(w) = r.position();
      get_last_place<core::rectangle>(w) = r;
    }

    void clear_last_place (os::window w) {
      s_last_size.erase(w);
      s_last_pos.erase(w);
      s_last_place.erase(w);
    }


#endif // X11

  } // win

} // gui
