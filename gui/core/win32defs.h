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
 * @brief     C++ API: win32 typedefs
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#define NOMINMAX
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

    typedef HBITMAP bitmap;
    typedef HICON icon;
    typedef HCURSOR cursor;
    typedef HBRUSH brush;
    typedef HFONT font;
    typedef HPEN pen;
    typedef HMENU menu;

    typedef LONG point_type;
    typedef LONG size_type;
    typedef LOGFONT font_type;
    typedef LPCSTR cursor_type;

    typedef unsigned int key_state;
    typedef unsigned int key_symbol;

    typedef POINT point;
    typedef SIZE size;
    typedef RECT rectangle;

    typedef UINT event_id;
    typedef UINT message_type;
    typedef LRESULT event_result;

    namespace win32 {
      typedef WPARAM wParam;
      typedef LPARAM lParam;

      typedef LOGBRUSH brush_type;
      typedef EXTLOGPEN pen_type;
    }

  } // os

} //gui
