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

#ifdef X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

  namespace core {

#ifdef WIN32

    typedef HWND window_id;
    typedef HANDLE drawable_id;
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
    typedef LOGBRUSH brush_type;

    typedef COLORREF color_type;
    
#define IF_WIN32(...) __VA_ARGS__
#define IF_X11(...)

#elif X11

    typedef Window window_id;
    typedef Drawable drawable_id;
    typedef int event_id;
    typedef int event_result;

    typedef unsigned int windows_style;
    typedef unsigned long color_type;

    typedef Pixmap icon_id;
    typedef Cursor cursor_id;
    typedef color_type brush_id;
    typedef Display* instance_id;
    typedef int screen_id;
    typedef Font font_id;
    typedef GC graphics_id;

    typedef XPoint point_type;
    typedef struct {
        unsigned int cx;
        unsigned int cy;
    } size_type;
    typedef XRectangle rectangle_type;
    typedef XFontStruct* font_type;

#define IF_WIN32(...)
#define IF_X11(...) __VA_ARGS__
        
#else

#pragma error "Unknown target system"

#endif
    namespace global {

      void init(core::instance_id instance);
      core::instance_id get_instance();

#ifdef X11
      screen_id get_screen();
      void set_screen(screen_id);
#endif // X11

    }

  } // core

} //gui
