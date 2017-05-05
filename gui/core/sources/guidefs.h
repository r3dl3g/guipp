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
#include <vector>
#include <string>

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

  namespace os {

#ifdef WIN32

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
    
#define IF_WIN32(...) __VA_ARGS__
#define IF_X11(...)

#ifdef _MSC_VER
# if (_MSC_VER >= 1900)
# define IF_VC14(...) __VA_ARGS__
# else 
# define IF_VC14(...)
# endif
# if (_MSC_VER >= 1800) && (_MSC_VER < 1900)
# define IF_VC12(...) __VA_ARGS__
# define IF_NOT_VC12(...)
# else 
# define IF_VC12(...)
# define IF_NOT_VC12(...) __VA_ARGS__
# endif
#endif

#elif X11

    typedef Display* instance;
    typedef Window window;
    typedef Drawable drawable;
    typedef GC graphics;

    typedef unsigned long color;
    typedef unsigned int style;

    typedef Pixmap bitmap;
    typedef Pixmap icon;
    typedef Cursor cursor;
    typedef color brush;
    typedef XftPattern* font;
    typedef int menu;

    typedef short point_type;
    typedef unsigned short size_type;
    typedef XftFont* font_type;
    typedef unsigned int cursor_type;

    typedef unsigned int key_state;
    typedef KeySym key_symbol;

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
      typedef Visual* visual;
    }

#define IF_WIN32(...)
#define IF_X11(...) __VA_ARGS__
#define IF_VC12(...)
#define IF_VC14(...)
#define IF_NOT_VC12(...) __VA_ARGS__

#else

#pragma error "Unknown target system"

#endif

  } // os

  namespace core {

    namespace global {

      void init(os::instance instance);
      os::instance get_instance();

      void fini();
      void sync ();

      int get_device_bits_per_pixel ();

#ifdef X11
      os::x11::screen get_screen();
      void set_screen(os::x11::screen);

      os::x11::visual get_visual();
#endif // X11

    }

  } // core

  enum class BPP : int {
    Undefined = 0,
    BW = 1,
    GRAY = 8,
    RGB = 24,
    RGBA = 32
  };

  enum class orientation {
    vertical,
    horizontal
  };

  constexpr orientation operator! (orientation o) {
    return (o == orientation::vertical) ? orientation::horizontal : orientation::vertical;
  }

  enum class origin {
    start,
    center,
    end
  };

  enum class alignment {
    left,
    hcenter,
    right,
    top,
    vcenter,
    bottom
  };


  template<alignment> struct alignment_orientation {};

  template<> struct alignment_orientation<alignment::left> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<> struct alignment_orientation<alignment::hcenter> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<> struct alignment_orientation<alignment::right> {
    static constexpr orientation value = orientation::horizontal;
  };

  template<> struct alignment_orientation<alignment::top> {
    static constexpr orientation value = orientation::vertical;
  };

  template<> struct alignment_orientation<alignment::vcenter> {
    static constexpr orientation value = orientation::vertical;
  };

  template<> struct alignment_orientation<alignment::bottom> {
    static constexpr orientation value = orientation::vertical;
  };


  template<alignment> struct alignment_origin {};

  template<> struct alignment_origin<alignment::left> {
    static constexpr origin value = origin::start;
  };

  template<> struct alignment_origin<alignment::hcenter> {
    static constexpr origin value = origin::center;
  };

  template<> struct alignment_origin<alignment::right> {
    static constexpr origin value = origin::end;
  };

  template<> struct alignment_origin<alignment::top> {
    static constexpr origin value = origin::start;
  };

  template<> struct alignment_origin<alignment::vcenter> {
    static constexpr origin value = origin::center;
  };

  template<> struct alignment_origin<alignment::bottom> {
    static constexpr origin value = origin::end;
  };


} //gui

// this is the main function that has to be defined in every application
extern int gui_main(const std::vector<std::string>& args);

#ifndef NDEBUG
#define IF_DEBUG(a) a
#define IF_NDEBUG(a)
#define IF_DEBUG_ELSE(a, b) a
#else
#define IF_DEBUG(a)
#define IF_NDEBUG(a) a
#define IF_DEBUG_ELSE(a, b) b
#endif // NDEBUG

