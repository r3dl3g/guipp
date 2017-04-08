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
#include<vector>
#include<string>

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#endif

#ifdef X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>
#endif

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

#ifdef WIN32

  namespace os {
    typedef HINSTANCE instance;
    typedef HWND window;
    typedef HANDLE drawable;
    typedef HDC graphics;

    typedef COLORREF color;
    typedef UINT style;

    typedef HICON icon;
    typedef HCURSOR cursor;
    typedef HBRUSH brush;
    typedef HFONT font;
    typedef HPEN pen;
    typedef HMENU menu;

    typedef LONG point_type;
    typedef LONG size_type;
    typedef LOGFONT font_type;

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
  }
    
#define IF_WIN32(...) __VA_ARGS__
#define IF_X11(...)

#elif X11

  namespace os {
    typedef Display* instance;
    typedef Window window;
    typedef Drawable drawable;
    typedef GC graphics;

    typedef unsigned long color;
    typedef unsigned int style;

    typedef Pixmap icon;
    typedef Cursor cursor;
    typedef color brush;
    typedef XftPattern* font;
    typedef int menu;

    typedef short point_type;
    typedef unsigned short size_type;
    typedef XftFont* font_type;

    typedef XPoint point;
    typedef struct {
      size_type cx;
      size_type cy;
    } size;
    typedef XRectangle rectangle;

    typedef int event_id;
    typedef int event_result;
    typedef void* event_callback;

    namespace x11 {
      typedef int screen;
    }
  }

#define IF_WIN32(...)
#define IF_X11(...) __VA_ARGS__
        
#else

#pragma error "Unknown target system"

#endif
  namespace core {

    namespace global {

      void init(os::instance instance);
      os::instance get_instance();

      void fini();

#ifdef X11
      os::x11::screen get_screen();
      void set_screen(os::x11::screen);
#endif // X11

    }

  } // core

} //gui

// this is the main function that has to be defined in every application
extern int gui_main(const std::vector<std::string>& args);

