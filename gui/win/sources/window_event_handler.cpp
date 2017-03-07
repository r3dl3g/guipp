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


namespace gui {

  namespace win {

#ifdef WIN32
    // --------------------------------------------------------------------------
    template<>
    bool get_param1<bool>(const core::event& e) {
      return LOWORD(e.param_1) != 0;
    }
    // --------------------------------------------------------------------------
    template<>
    window* get_param1<window*>(const core::event& e) {
      return window::get((core::window_id)e.param_1);
    }
    // --------------------------------------------------------------------------
    template<>
    draw::graphics get_param1<draw::graphics>(const core::event& e) {
      return draw::graphics(e.id, (core::graphics_id)e.param_1);
    };
    // --------------------------------------------------------------------------
    template<>
    window* get_param2<window*>(const core::event& e) {
      return window::get((core::window_id)e.param_2);
    }
    // --------------------------------------------------------------------------
    window* get_window_from_cs(const core::event& e) {
      CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(e.param_2);
      return window::get(cs->hwndParent);
    }
    // --------------------------------------------------------------------------
    unsigned int get_flags_from_wp(const core::event& e) {
      WINDOWPOS* p = reinterpret_cast<WINDOWPOS*>(e.param_2);
      return p->flags;
    }

    static std::map<core::window_id, bool> s_mouse_inside;

	bool mouse_enter_matcher::operator() (const core::event& e) {
      switch (e.type) {
      case WM_MOUSEMOVE:
        if (!s_mouse_inside[e.id]) {
          s_mouse_inside[e.id] = true;
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
    void paint_caller::operator()(const core::event& e) {
      if (callback) {
        PAINTSTRUCT ps;
        core::graphics_id id = BeginPaint(e.id, &ps);
        callback(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
      }
    }

    // --------------------------------------------------------------------------
    void pos_changing_caller::operator() (const core::event& e) {
      if (callback) {
        LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.param_2);
        core::rectangle r = get_rect<WINDOWPOS>(e);
        callback(r);
        const core::point pos = r.position();
        const core::size sz = r.size();
        p->x = pos.x;
        p->y = pos.y;
        p->cx = sz.width;
        p->cy = sz.height;
      }
    }

    // --------------------------------------------------------------------------
    void get_minmax_caller::operator() (const core::event& e) {
      if (callback) {
        LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(e.param_2);
        core::size mi(info->ptMinTrackSize);
        core::size ma(info->ptMaxTrackSize);
        core::size sz(info->ptMaxSize);
        core::point pos(info->ptMaxPosition);
        callback(sz, pos, mi, ma);
        info->ptMinTrackSize = mi;
        info->ptMaxTrackSize = ma;
      }
    }

#elif X11
    // --------------------------------------------------------------------------
    paint_caller::~paint_caller() {
        if (gc) {
          if (core::global::get_instance()) {
            XFreeGC(core::global::get_instance(), gc);
          }
          gc = 0;
        }
    }

    void paint_caller::operator()(const core::event& e) {
      if (callback) {
        if (!gc) {
            gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
        }
        draw::graphics g(e.xexpose.window, gc);
        callback(g);
        XFlushGC(e.xexpose.display, gc);
      }
    }

    void send_client_message (window* win, Atom message, void* data) {
      core::instance_id display = core::global::get_instance();

      XClientMessageEvent client ;

      client.display      = display;
      client.window       = win->get_id();
      client.type         = ClientMessage ;
      client.format       = 8 ;
      client.message_type = message;

      /* Send the data off to the other process */
      XSendEvent(client.display, client.window, True, 0, (XEvent*)&client);
      XFlush(display);
    }

#endif // WIN32

  } // win

} // gui
