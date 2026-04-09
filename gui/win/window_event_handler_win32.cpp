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
 * @brief     generic window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
# include <windowsx.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/window_event_handler.h"
#include "gui/win/overlapped_window.h"
#include "gui/win/native.h"
#include <util/string_util.h>


namespace gui {

  namespace win {

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
      return native::get_window((os::window)e.wParam);
    }

    // --------------------------------------------------------------------------
    template<>
    os::window get_param<0, os::window>(const core::event& e) {
      return (os::window)e.wParam;
    }

    // --------------------------------------------------------------------------
    template<>
    os::graphics get_param<0, os::graphics>(const core::event& e) {
      return (os::graphics)e.wParam;
    }

    // --------------------------------------------------------------------------
    template<>
    window* get_param<1, window*>(const core::event& e) {
      return native::get_window((os::window)e.lParam);
    }

    // --------------------------------------------------------------------------
    template<>
    core::point get_param<1>(const core::event& e) {
      return core::global::scale_from_native(get_param<1, core::native_point>(e));
    }

    // --------------------------------------------------------------------------
    template<>
    core::size get_param<1>(const core::event& e) {
      return core::global::scale_from_native(get_param<1, core::native_size>(e));
    }

    // --------------------------------------------------------------------------
    template<>
    core::rectangle get_param<1>(const core::event& e) {
      return core::global::scale_from_native(get_param<1, core::native_rect>(e));
    }

    // --------------------------------------------------------------------------
    template<>
    core::native_point get_param<1>(const core::event& e) {
      return core::native_point(GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam));
    }

    // --------------------------------------------------------------------------
    template<>
    core::native_size get_param<1>(const core::event& e) {
      return core::native_size(GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam));
    }

    // --------------------------------------------------------------------------
    template<>
    core::native_rect get_param<1>(const core::event& e) {
      return get_rect<WINDOWPOS>(e);
    }

    // --------------------------------------------------------------------------
    template<>
    const core::native_rect* get_param<1>(const core::event& e) {
      return (const core::native_rect*)(e.lParam);
    }

    // --------------------------------------------------------------------------
    core::native_point get_root_mouse_pos (const core::event& e) {
      POINT pt = {GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)};
      ClientToScreen(e.id, &pt);
      return core::native_point(pt.x, pt.y);
    }

    // --------------------------------------------------------------------------
    window* get_window_from_cs (const core::event& e) {
      CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(e.lParam);
      return native::get_window(cs->hwndParent);
    }

    // --------------------------------------------------------------------------
    window* get_window_from_id (const core::event& e) {
      return native::get_window(e.id);
    }

    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp (const core::event& e) {
      WINDOWPOS* p = reinterpret_cast<WINDOWPOS*>(e.lParam);
      return p->flags;
    }

    // --------------------------------------------------------------------------
    core::native_point::type get_wheel_delta (const core::event& e) {
      return static_cast<core::native_point::type>(core::signum(GET_WHEEL_DELTA_WPARAM(e.wParam)));
    }

    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event&) {
      return core::global::get_key_state();
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
      WCHAR wbuffer[4] = {0};

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
      return util::string::utf16_to_utf8(std::wstring(wbuffer, len));
    }

    // --------------------------------------------------------------------------
    template<>
    bool any_button_matcher<true> (const core::event& e) {
      return (e.type == WM_LBUTTONDOWN) || (e.type == WM_RBUTTONDOWN) || (e.type == WM_MBUTTONDOWN);
    }

    template<>
    bool any_button_matcher<false> (const core::event& e) {
      return (e.type == WM_LBUTTONUP) || (e.type == WM_RBUTTONUP) || (e.type == WM_MBUTTONUP);
    }

    // --------------------------------------------------------------------------
    bool match_key (const core::event& e, os::event_id id, os::key_symbol symbol) {
      return (e.type == id) && (get_key_symbol(e) == symbol);
    }

    // --------------------------------------------------------------------------
    static std::map<os::window, bool> s_mouse_inside;

    bool mouse_enter_matcher (const core::event& e) {
      switch (e.type) {
      case WM_MOUSEMOVE:
        if (!s_mouse_inside[e.id]) {
          s_mouse_inside[e.id] = true;
          TRACKMOUSEEVENT tme;
          memset(&tme, 0, sizeof(tme));
          tme.cbSize = sizeof(TRACKMOUSEEVENT);
          tme.dwFlags = TME_LEAVE;
          tme.hwndTrack = e.id;
          TrackMouseEvent(&tme);
          return true;
        }
        break;
      case WM_MOUSEHOVER:
        s_mouse_inside[e.id] = true;
        return true;
      case WM_MOUSELEAVE:
        s_mouse_inside[e.id] = false;
        break;
      }
      return false;
    }

    // --------------------------------------------------------------------------
    core::context* get_context (const core::event& e) {
      return (core::context*)(e.wParam);
    }

    // --------------------------------------------------------------------------
    void pos_changing_getter::operator() (const core::event& e) {
      if (callback) {
        LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.lParam);
        core::rectangle r = core::global::scale_from_native(get_rect<WINDOWPOS>(e));
        callback(r);
        p->x = static_cast<int>(r.x());
        p->y = static_cast<int>(r.y());
        p->cx = static_cast<int>(r.width());
        p->cy = static_cast<int>(r.height());
      }
    }

    // --------------------------------------------------------------------------
    void minmax_getter::operator() (const core::event& e) {
      if (callback) {
        LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(e.lParam);
        core::size mi = core::global::scale_from_native(core::native_size(info->ptMinTrackSize));
        core::size ma = core::global::scale_from_native(core::native_size(info->ptMaxTrackSize));
        core::size sz = core::global::scale_from_native(core::native_size(info->ptMaxSize));
        core::point pos = core::global::scale_from_native(core::native_point(info->ptMaxPosition.x, info->ptMaxPosition.y));
        callback(sz, pos, mi, ma);
        info->ptMinTrackSize = core::global::scale_to_native(mi).os_point();
        info->ptMaxTrackSize = core::global::scale_to_native(ma).os_point();
      }
    }
    // --------------------------------------------------------------------------
    bool is_mouse_event (const core::event& e) {
      switch (e.type) {
        case WM_MOUSEMOVE:
        case WM_MOUSELEAVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDBLCLK:
        case WM_MOUSEHWHEEL:
          return true;
        default:
          return false;
      }
    }

    core::native_point get_mouse_event_point (const core::event& e) {
      return mouse_move_event::Caller::get_param<1>(e);
    }

    bool is_key_event (const core::event& e) {
      switch (e.type) {
        case WM_KEYDOWN:
        case WM_KEYUP:
          return true;
        default:
          return false;
      }
    }

    bool is_hotkey_event (const core::event& e) {
      return (e.type == WM_KEYDOWN);
    }

  } // win

} // gui

#endif // Win32
