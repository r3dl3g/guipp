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

  // --------------------------------------------------------------------------
  bool get_param1<bool>::operator()(const window_event& e) const {
    return LOWORD(e.param_1) != 0;
  }
  // --------------------------------------------------------------------------
  window* get_param1<window*>::operator()(const window_event& e) const {
    return window::get((window_id)e.param_1);
  }
  // --------------------------------------------------------------------------
  window* get_param2<window*>::operator()(const window_event& e) const {
    return window::get((window_id)e.param_2);
  }
  // --------------------------------------------------------------------------
  window* get_window_from_cs::operator()(const window_event& e) const {
    CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(e.param_2);
    return window::get(cs->hwndParent);
  }
  // --------------------------------------------------------------------------
  unsigned int get_flags_from_wp::operator()(const window_event& e) const {
    WINDOWPOS* p = reinterpret_cast<WINDOWPOS*>(e.param_2);
    return p->flags;
  }

  // --------------------------------------------------------------------------
  bool pos_changing_event::handle_event(const window_event& e, event_result& result) {
    if ((e.msg == WM_WINDOWPOSCHANGING) && callback) {
      LPWINDOWPOS p = reinterpret_cast<LPWINDOWPOS>(e.param_2);
      rectangle r = get_rectangle_from<WINDOWPOS>()(e);
      unsigned int flags = get_flags_from_wp()(e);
      callback(flags, r);
      p->flags = flags;
      p->x = r.position.x;
      p->y = r.position.y;
      p->cx = r.size.width;
      p->cy = r.size.height;
      result = 0;
      return true;
    }
    return false;
  }
  // --------------------------------------------------------------------------
  bool get_minmax_event::handle_event(const window_event& e, event_result& result) {
    if ((e.msg == WM_GETMINMAXINFO) && callback) {
      LPMINMAXINFO info = reinterpret_cast<LPMINMAXINFO>(e.param_2);
      size mi(info->ptMinTrackSize);
      size ma(info->ptMaxTrackSize);
      size sz(info->ptMaxSize);
      position pos(info->ptMaxPosition);
      callback(sz, pos, mi, ma);
      info->ptMinTrackSize = mi;
      info->ptMaxTrackSize = ma;
      result = 0;
      return true;
    }
    return false;
  }

}
