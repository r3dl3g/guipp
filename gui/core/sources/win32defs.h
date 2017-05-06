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
#include <bits.h>

namespace gui {

  namespace os {

    const platform system_platform = platform::win32;
    const bit_order bitmap_bit_order = bit_order::msb;

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
    typedef LRESULT event_result;
    typedef WNDPROC event_callback;

    namespace win32 {
      typedef WPARAM event_param_1;
      typedef LPARAM event_param_2;

      typedef LOGBRUSH brush_type;
      typedef LOGPEN pen_type;
    }
    
#define IF_WIN32_ELSE(A, B) A

  } // os

} //gui
