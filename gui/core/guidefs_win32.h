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
 * @brief     C++ API: win32 typedefs
 *
 * @file
 */

#pragma once

#ifndef GUIPP_WIN
# error Include guidefs.h instead guidefs_win32.h!
#endif //GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
#undef NOMINMAX
#define NOMINMAX 1
#include <windows.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


#ifndef MK_MENU
# define MK_MENU 0x0080
#endif // MK_MENU
#ifndef MK_SYTEM
# define MK_SYTEM 0x0100
#endif // MK_SYTEM
#ifndef MK_NUM_LOCK
# define MK_NUM_LOCK 0x0200
#endif // MK_NUM_LOCK
#ifndef MK_SCROLL_LOCK
# define MK_SCROLL_LOCK 0x0400
#endif // MK_SCROLL_LOCK
#ifndef MK_CAPS_LOCK
# define MK_CAPS_LOCK 0x0800
#endif // MK_CAPS_LOCK

namespace gui {

  namespace os {

    typedef HINSTANCE instance;
    typedef HWND window;
    typedef HANDLE drawable;
    typedef HDC graphics;

    typedef COLORREF color;
    typedef UINT style;

    typedef HBITMAP backstore;
    typedef HBITMAP bitmap;
    typedef HICON icon;
    typedef HCURSOR cursor;
    typedef HBRUSH brush;
    typedef HFONT font;

    typedef LONG point_type;
    typedef LONG size_type;
    typedef LOGFONT font_type;
    typedef LPCSTR cursor_type;

    typedef unsigned int key_state;
    typedef unsigned int key_symbol;

    typedef POINT point;
    typedef SIZE size;
    typedef RECT rectangle;

    inline point_type get_x (const POINT& p) { return p.x; }
    inline point_type get_y (const POINT& p) { return p.y; }

    inline size_type get_width (const SIZE& s) { return s.cx; }
    inline size_type get_height (const SIZE& s) { return s.cy; }

    inline point_type get_x (const RECT& r) { return r.left; }
    inline point_type get_y (const RECT& r) { return r.top; }
    inline size_type get_width (const RECT& r) { return r.right - r.left; }
    inline size_type get_height (const RECT& r) { return r.bottom - r.top; }

    typedef UINT event_id;
    typedef UINT message_type;
    typedef LRESULT event_result;

    namespace win32 {
      typedef WPARAM wParam;
      typedef LPARAM lParam;

      typedef LOGBRUSH brush_type;
      typedef EXTLOGPEN pen_type;
      typedef HPEN pen;
    }

  } // os

} //gui
