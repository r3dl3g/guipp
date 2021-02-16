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
 * @brief     C++ API: basic window
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_WIN
# include <windowsx.h>
#endif // GUIPP_WIN

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window_event_handler.h>
#include <gui/win/overlapped_window.h>

#ifdef GUIPP_WIN
# include <util/string_util.h>
#endif // GUIPP_WIN


namespace gui {

  namespace win {

#ifdef GUIPP_WIN
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
      return detail::get_window((os::window)e.lParam);
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
    core::native_rect* get_param<1>(const core::event& e) {
      return (core::native_rect*)e.lParam;
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
      return detail::get_window(cs->hwndParent);
    }

    // --------------------------------------------------------------------------
    window* get_window_from_id (const core::event& e) {
      return detail::get_window(e.id);
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
      case WM_MOUSELEAVE:
        s_mouse_inside[e.id] = false;
        break;
      }
      return false;
    }

    // --------------------------------------------------------------------------
    core::context* get_context (const core::event& e) {
      return (core::context*)e.wParam;
    }

    // --------------------------------------------------------------------------
    void pos_changing_getter::operator() (const core::event& e) {
      if (callback) {
        LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.lParam);
        core::rectangle r = core::global::scale_from_native(get_rect<WINDOWPOS>(e));
        callback(r);
        p->x = r.x();
        p->y = r.y();
        p->cx = r.width();
        p->cy = r.height();
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

    bool is_key_event (const core::event& e) {
      switch (e.type) {
        case WM_KEYDOWN:
        case WM_KEYUP:
          return true;
        default:
          return false;
      }
    }


#endif // Win32
#ifdef GUIPP_X11
    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return static_cast<os::key_state>(get_event_state<XKeyEvent>(e));
    }

    // --------------------------------------------------------------------------
    os::key_symbol get_key_symbol (const core::event& e) {
      return XLookupKeysym(const_cast<XKeyEvent*>(&e.xkey), 0);
    }

    // --------------------------------------------------------------------------
    std::string get_key_chars (const core::event& e) {
      XIC ic = x11::get_window_ic(e.xany.window);
      if (ic) {
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
    core::context* get_context (const core::event& e) {
      return get_client_data<0, core::context*>(e);
    }
    // --------------------------------------------------------------------------
    core::native_rect* get_paint_rect (const core::event& e) {
      return get_client_data<1, core::native_rect*>(e);
    }
    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      auto& l = e.xclient.data.l;
      return core::global::scale_from_native(core::native_rect{static_cast<short>(l[0]), static_cast<short>(l[1]),
                                                               static_cast<unsigned short>(l[2]), static_cast<unsigned short>(l[3])});
    }
    // --------------------------------------------------------------------------
    core::size get_client_data_size (const core::event& e) {
      auto& l = e.xclient.data.l;
      return core::size(os::size{static_cast<unsigned short>(l[0]), static_cast<unsigned short>(l[1])});
    }
    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*>(const core::event& e) {
      os::window id = e.xclient.data.l[0];
      return detail::get_window(id);
    }
    // --------------------------------------------------------------------------
    template<>
    core::point::type get_client_data<0, core::point::type>(const core::event& e) {
      os::window l1 = e.xclient.data.l[0];
      double d1 = static_cast<double>(l1) / 1000000.0;
      return d1;
    }
    // --------------------------------------------------------------------------
    window* get_current_focus_window (const core::event&) {
      return global::get_current_focus_window();
    }
    // --------------------------------------------------------------------------
    core::native_point get_root_mouse_pos (const core::event& e) {
      auto me = (event_type_cast<XMotionEvent>(e));
      return core::native_point{me.x_root, me.y_root};
    }
    // --------------------------------------------------------------------------
    std::map<Window, core::rectangle> s_last_place;
    // --------------------------------------------------------------------------
    template<>
    const core::size& get_last_place<core::size>(os::window w) {
      return s_last_place[w].size();
    }
    // --------------------------------------------------------------------------
    template<>
    const core::point& get_last_place<core::point>(os::window w) {
      return s_last_place[w].position();
    }
    // --------------------------------------------------------------------------
    template<>
    const core::rectangle& get_last_place<core::rectangle>(os::window w) {
      return s_last_place[w];
    }
    // --------------------------------------------------------------------------
    void update_last_place (os::window w, const core::rectangle& r) {
      s_last_place[w] = r;
    }
    // --------------------------------------------------------------------------
    void clear_last_place (os::window w) {
      s_last_place.erase(w);
    }
    // --------------------------------------------------------------------------
    bool is_mouse_event (const core::event& e) {
      switch (e.type) {
        case MotionNotify:
        case ButtonPress:
        case ButtonRelease:
        case EnterNotify:
        case LeaveNotify:
          return true;
        default:
          return false;
      }
    }

    bool is_key_event (const core::event& e) {
      switch (e.type) {
        case KeyPress:
        case KeyRelease:
          return true;
        default:
          return false;
      }
    }


#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    os::key_state get_key_state (const core::event& e) {
      return e.cast<QKeyEvent>().modifiers();
    }

    os::key_symbol get_key_symbol (const core::event& e) {
      return e.cast<QKeyEvent>().key();
    }

    std::string get_key_chars (const core::event& e) {
      return e.cast<QKeyEvent>().text().toStdString();
    }

    // --------------------------------------------------------------------------
    QClientEvent::QClientEvent (Type type, const core::rectangle& rect)
      : QEvent(type)
      , m_rect(rect)
      , m_l1(0)
      , m_l2(0)
    {}

    QClientEvent::QClientEvent (Type type, long l1, long l2)
      : QEvent(type)
      , m_l1(l1)
      , m_l2(l2)
    {}

    QClientEvent::QClientEvent (Type type)
      : QEvent(type)
      , m_l1(0)
      , m_l2(0)
    {}

    QClientEvent::~QClientEvent ()
    {}

    const core::rectangle &QClientEvent::rect() const {
      return m_rect;
    }

    long QClientEvent::l1 () const {
      return m_l1;
    }

    long QClientEvent::l2 () const {
      return m_l2;
    }

    // --------------------------------------------------------------------------
    template<>
    long get_param<0> (const core::event& e) {
      return e.cast<QClientEvent>().l1();
    }

    template<>
    long get_param<1> (const core::event& e) {
      return e.cast<QClientEvent>().l2();
    }

    core::native_point::type get_wheel_delta_x (const core::event& e) {
      auto we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().x() / 15;
      }
      return numPixels.x();
    }

    core::native_point::type get_wheel_delta_y (const core::event& e) {
      auto we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().y() / 15;
      }
      return numPixels.y();
    }

    core::point get_move_point (const core::event& e) {
      const auto p = e.cast<QMoveEvent>().pos();
      return core::global::scale_from_native(core::native_point(p.x(), p.y()));
    }

    core::native_point get_wheel_point (const core::event& e) {
#if QT_VERSION > QT_VERSION_CHECK(5, 13, 0)
      const auto pt = e.cast<QWheelEvent>().position();
      return core::native_point(pt.x(), pt.y());
#else
      return core::native_point(e.cast<QWheelEvent>().x(), e.cast<QWheelEvent>().y());
#endif
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      return e.cast<QClientEvent>().rect();
    }

//    os::graphics get_graphics (const core::event& e) {
//      return nullptr;//get_draw_window(e)->painter();
//    }

//    os::window get_draw_window (const core::event& e) {
//      return e.id;
//    }

    core::context* get_context (const core::event& e) {
      return (core::context*)e.cast<QClientEvent>().l1();
    }

    core::native_rect* get_paint_rect (const core::event& e) {
      return (core::native_rect*)(e.cast<QClientEvent>().l2());
    }

    bool is_mouse_event (const core::event& e) {
      switch (e.type()) {
        case QEvent::MouseMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::DragEnter:
        case QEvent::DragMove:
        case QEvent::DragLeave:
//        case QEvent::Enter:
//        case QEvent::Leave:
        case QEvent::MouseTrackingChange:
        case QEvent::Wheel:
          return true;
        default:
          return false;
      }
    }

    bool is_key_event (const core::event& e) {
      switch (e.type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
          return true;
        default:
          return false;
      }
    }

    // --------------------------------------------------------------------------
#endif // GUIPP_QT

  } // win

} // gui
