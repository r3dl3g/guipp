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
#include <gui/win/window.h>

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
    core::point get_param<1, core::point>(const core::event& e) {
      return core::global::scale_from_native(core::native_point(GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)));
    }

    // --------------------------------------------------------------------------
    template<>
    core::size get_param<1, core::size>(const core::event& e) {
      return core::global::scale_from_native(core::native_size(GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)));
    }

    // --------------------------------------------------------------------------
    template<>
    core::rectangle get_param<1>(const core::event& e) {
      return get_rect<WINDOWPOS>(e);
    }

    // --------------------------------------------------------------------------
    core::point get_root_mouse_pos (const core::event& e) {
      POINT pt = {GET_X_LPARAM(e.lParam), GET_Y_LPARAM(e.lParam)};
      ClientToScreen(e.id, &pt);
      return core::global::scale_from_native(core::native_point(pt));
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
    core::point::type get_wheel_delta (const core::event& e) {
      return static_cast<core::point::type>(core::signum(GET_WHEEL_DELTA_WPARAM(e.wParam)));
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
    void os_paint_getter::operator() (const core::event& e) {
      if (f) {
        PAINTSTRUCT ps;
        os::graphics id = BeginPaint(e.id, &ps);
        f(e.id, id);
        EndPaint(e.id, &ps);
      }
    }

    // --------------------------------------------------------------------------
    void pos_changing_getter::operator() (const core::event& e) {
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
    void minmax_getter::operator() (const core::event& e) {
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

    void send_client_message (window* win, os::message_type message, long l1, long l2) {
      if (win && win->is_valid()) {
        gui::os::event_result result;
        core::event e{ detail::get_window_id(*win), message, static_cast<WPARAM>(l1), static_cast<LPARAM>(l2) };
        win->handle_event(e, result);
      }
    }

    void send_client_message (window* win, os::message_type message, const core::size& sz) {
      if (win && win->is_valid()) {
        gui::os::event_result result;
        os::size s = sz;
        long l2 = (long)s.cy << 16 | (long)s.cx;
        core::event e{ detail::get_window_id(*win), message, 0, static_cast<LPARAM>(l2)};
        win->handle_event(e, result);
      }
    }

    void send_client_message (window* win, os::message_type message, const core::rectangle& wr) {
      if (win && win->is_valid()) {
        gui::os::event_result result;
        core::native_rect r = core::global::scale_to_native(wr);
        auto id = detail::get_window_id(*win);
        WINDOWPOS wp{id, NULL, r.x(), r.y(), static_cast<int>(r.width()), static_cast<int>(r.height()), 0};
        core::event e { id, message, 0, reinterpret_cast<LPARAM>(&wp)};
        win->handle_event(e, result);
      }
    }

    void post_client_message (window* win, os::message_type message, long l1, long l2) {
      send_client_message(win, message, l1, l2);
//      if (win && win->is_valid()) {
//        PostMessage(win->get_id(), message, static_cast<WPARAM>(l1), static_cast<LPARAM>(l2));
//      }
    }

#endif // Win32
#ifdef GUIPP_X11
    namespace x11 {
      // --------------------------------------------------------------------------
      void send_client_message (window* win, Atom message, long l1, long l2, long l3, long l4, long l5) {
        if (win && win->is_valid()) {
          XEvent event;
          XClientMessageEvent& client = event.xclient;

          client.type = ClientMessage;
          client.serial = 0;
          client.send_event = True;
          client.display = core::global::get_instance();
          client.window = detail::get_window_id(*win);
          client.message_type = message;
          client.format = 32;
          client.data.l[0] = l1;
          client.data.l[1] = l2;
          client.data.l[2] = l3;
          client.data.l[3] = l4;
          client.data.l[4] = l5;

//          gui::os::event_result result = 0;
//          win->handle_event(event, result);
          /* Send the data off to the other process */
          XSendEvent(client.display, client.window, True, 0, &event);

          //        core::global::sync();
        }
      }

      void send_client_message (window* win, Atom message, const window* w, const core::rectangle& rect) {
        XRectangle r = rect;
        long l1 = (long)r.x << 16 | (long)r.y;
        long l2 = (long)r.width << 16 | (long)r.height;
        send_client_message(win, message, detail::get_window_id(*w), l1, l2);
      }

      void post_client_message (window* win, Atom message, long l1, long l2) {
        x11::send_client_message(win, message, l1, l2);
      }

      namespace {
        std::map<const receiver*, os::event_id> window_event_mask;
      }

      void prepare_win_for_event (const receiver* win, os::event_id mask) {
        os::window id = win ? detail::get_window_id(*static_cast<const window*>(win)) : 0;
        if (id) {
          auto i = window_event_mask.find(static_cast<const window*>(win));
          if (i != window_event_mask.end()) {
            i->second |= mask;
            XSelectInput(core::global::get_instance(), id, i->second);
          } else {
            window_event_mask[win] = mask;
            XSelectInput(core::global::get_instance(), id, mask);
          }
        } else {
          os::event_id& old_mask = window_event_mask[win];
          old_mask |= mask;
        }
      }

      void unprepare_win (const receiver* win) {
        window_event_mask.erase(static_cast<const window*>(win));
      }

    } // namespace x11

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
    os::graphics get_graphics (const core::event& e) {
      return DefaultGCOfScreen(DefaultScreenOfDisplay(e.xany.display));
    }

    // --------------------------------------------------------------------------
    os::window get_draw_window (const core::event& e) {
      return e.xany.window;
    }

    // --------------------------------------------------------------------------
    void send_client_message (window* win, os::message_type message, long l1, long l2) {
      x11::send_client_message(win, message, l1, l2);
    }

    void send_client_message (window* win, os::message_type message, const core::size& sz) {
      os::size s = sz.os();
      long l0 = (long)s.cx << 16 | (long)s.cy;
      x11::send_client_message(win, message, l0);
    }

    void send_client_message (window* win, os::message_type message, const core::rectangle& wr) {
      os::size s = wr.size().os();
      os::point p = wr.position().os();
      long l0 = (long)p.x << 16 | (long)p.y;
      long l1 = (long)s.cx << 16 | (long)s.cy;
      x11::send_client_message(win, message, l1, l0, l1);
    }

    void post_client_message (window* win, Atom message, long l1, long l2) {
      x11::post_client_message(win, message, l1, l2);
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      long p = e.xclient.data.l[1];
      long s = e.xclient.data.l[2];
      short x = p >> 16;
      short y = p & 0xffff;
      unsigned short w = s >> 16;
      unsigned short h = s & 0xffff;
      os::rectangle r = {x, y, w, h};
      return core::rectangle(r);
    }

    // --------------------------------------------------------------------------
    core::size get_client_data_size (const core::event& e) {
      long s = e.xclient.data.l[0];
      unsigned short w = s >> 16;
      unsigned short h = s & 0xffff;
      os::size sz = {w, h};
      return core::size(sz);
    }

    // --------------------------------------------------------------------------
    template<>
    window* get_client_data<0, window*>(const core::event& e) {
      os::window id = e.xclient.data.l[0];
      return detail::get_window(id);
    }

    // --------------------------------------------------------------------------
    window* get_current_focus_window (const core::event&) {
      return global::get_current_focus_window();
    }

    // --------------------------------------------------------------------------
    core::point get_root_mouse_pos (const core::event& e) {
      auto me = (event_type_cast<XMotionEvent>(e));
      return core::global::scale_from_native(core::native_point{me.x_root, me.y_root});
    }

    // --------------------------------------------------------------------------
    std::map<Window, core::rectangle> s_last_place;

    template<>
    const core::size& get_last_place<core::size>(os::window w) {
      return s_last_place[w].size();
    }

    template<>
    const core::point& get_last_place<core::point>(os::window w) {
      return s_last_place[w].position();
    }

    template<>
    const core::rectangle& get_last_place<core::rectangle>(os::window w) {
      return s_last_place[w];
    }

    void update_last_place (os::window w, const core::rectangle& r) {
      s_last_place[w] = r;
    }

    void clear_last_place (os::window w) {
      s_last_place.erase(w);
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

    core::point::type get_wheel_delta_x (const core::event& e) {
      auto we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().x() / 15;
      }
      return numPixels.x();
    }

    core::point::type get_wheel_delta_y (const core::event& e) {
      auto we = e.cast<QWheelEvent>();
      os::point numPixels = we.pixelDelta();
      if (numPixels.isNull()) {
        return we.angleDelta().y() / 15;
      }
      return numPixels.y();
    }

    // --------------------------------------------------------------------------
    core::rectangle get_client_data_rect (const core::event& e) {
      return e.cast<QClientEvent>().rect();
    }

    os::graphics get_graphics (const core::event& e) {
      return nullptr;//get_draw_window(e)->painter();
    }

    os::window get_draw_window (const core::event& e) {
      return e.id;
    }

    // --------------------------------------------------------------------------
    void send_client_message (window* win, os::message_type message, const core::rectangle& r) {
      if (win && win->is_valid()) {
        gui::os::event_result result;
        QClientEvent e(static_cast<QEvent::Type>(message), r);
        win->handle_event(gui::core::event{detail::get_window_id(*win), &e}, result);
      }
    }

    void send_client_message (window* win, os::message_type message, const core::size& sz) {
      send_client_message(win, message, core::rectangle(sz));
    }

    void send_client_message (window* win, os::message_type message, long l1, long l2) {
      if (win && win->is_valid()) {
        gui::os::event_result result;
        QClientEvent e(static_cast<QEvent::Type>(message), l1, l2);
        win->handle_event(gui::core::event{detail::get_window_id(*win), &e}, result);
      }
    }

    void post_client_message (window* win, os::message_type message, long l1, long l2) {
      send_client_message(win, message, l1, l2);
    }
    // --------------------------------------------------------------------------
#endif // GUIPP_QT

  } // win

} // gui
