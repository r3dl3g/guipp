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
 * @brief     C++ API: windows message debug helper
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
#include <gui/win/dbg_win_message.h>


namespace gui {

  namespace win {

    bool is_frequent_event (const core::event& e) {
#ifdef GUIPP_WIN
      switch (e.type) {
        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
        case WM_NCHITTEST:
        case WM_SETCURSOR:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        case WM_ENTERIDLE:
        case WM_CANCELMODE:
        case 0x0118:      // WM_SYSTIMER (caret blink)
#elif GUIPP_X11
      switch (e.type) {
        case ConfigureNotify:
        case MotionNotify:
        case EnterNotify:
        case LeaveNotify:
        case Expose:
        case NoExpose:
#elif GUIPP_QT
      switch (e.type()) {
        case QEvent::Type::MouseMove:
#endif
          return true;
        default:
          return false;
      }
    }

    bool is_none_client_event (const core::event& e) {
#ifdef GUIPP_WIN
      os::event_id id = e.type;
      return ((id >= WM_NCCREATE) && (id <= WM_NCACTIVATE)) ||
             ((id >= WM_NCMOUSEMOVE) && (id <= WM_NCMBUTTONDBLCLK)) ||
             ((id >= WM_NCXBUTTONDOWN) && (id <= WM_NCXBUTTONDBLCLK)) ||
#if(WINVER >= 0x0602)
             ((id >= WM_NCPOINTERUPDATE) && (id <= WM_NCPOINTERUP)) ||
#endif
             (id == WM_NCMOUSEHOVER) || (id == WM_NCMOUSELEAVE);
#else
      return false;
#endif
    }

  } // win

} // gui
