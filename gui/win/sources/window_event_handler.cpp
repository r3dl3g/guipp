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

    // --------------------------------------------------------------------------
    bool paint_event::operator()(const core::event& e, core::event_result& result) {
      if ((e.msg == WM_PAINT) && callback) {
        PAINTSTRUCT ps;
        core::graphics_id id = BeginPaint(e.id, &ps);
        callback(draw::graphics(e.id, id));
        EndPaint(e.id, &ps);
        result = 0;
        return true;
      }
      return false;
    }

    // --------------------------------------------------------------------------
    bool pos_changing_event::operator()(const core::event& e, core::event_result& result) {
      if ((e.msg == WM_WINDOWPOSCHANGING) && callback) {
        LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.param_2);
        core::rectangle r = get_rect<WINDOWPOS>(e);
        unsigned int flags = get_flags_from_wp(e);
        callback(flags, r);
        p->flags = flags;
        const core::point pos = r.position();
        const core::size sz = r.size();
        p->x = pos.x;
        p->y = pos.y;
        p->cx = sz.width;
        p->cy = sz.height;
        result = 0;
        return true;
      }
      return false;
    }
    // --------------------------------------------------------------------------
    bool get_minmax_event::operator()(const core::event& e, core::event_result& result) {
      if ((e.msg == WM_GETMINMAXINFO) && callback) {
        LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(e.param_2);
        core::size mi(info->ptMinTrackSize);
        core::size ma(info->ptMaxTrackSize);
        core::size sz(info->ptMaxSize);
        core::point pos(info->ptMaxPosition);
        callback(sz, pos, mi, ma);
        info->ptMinTrackSize = mi;
        info->ptMaxTrackSize = ma;
        result = 0;
        return true;
      }
      return false;
    }
#elif X11
    // --------------------------------------------------------------------------
    bool paint_event::operator()(const core::event& e, core::event_result& result) {
      if ((e.type == Expose) && callback) {
        core::graphics_id gc = XCreateGC(e.xexpose.display, e.xexpose.window, 0, 0);
        draw::graphics g(e.xexpose.window, gc);
        callback(g);
        XFreeGC(e.xexpose.display, gc);
        result = 0;
        return true;
      }
      return false;
    }
    
#endif // WIN32

  } // win

} // gui
