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
* @brief     C++ API: basic typedefs
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef WIN32
#include <windows.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

  namespace core {

#ifdef WIN32

    typedef HWND window_id;
    typedef UINT event_id;
    typedef WPARAM event_param_1;
    typedef LPARAM event_param_2;
    typedef LRESULT event_result;

    typedef UINT windows_style;

    typedef HICON icon_id;
    typedef HCURSOR cursor_id;
    typedef HBRUSH brush_id;
    typedef HINSTANCE instance_id;
    typedef HFONT font_id;
    typedef HPEN pen_id;
    typedef HDC graphics_id;

    typedef POINT point_type;
    typedef SIZE size_type;
    typedef RECT rectangle_type;
    typedef LOGFONT font_type;
    typedef LOGPEN pen_type;

    typedef COLORREF color_type;

#else

#pragma error "Unknown target system"

#endif

  } // core

} //gui
