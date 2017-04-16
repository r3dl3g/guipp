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

    typedef Pixmap icon;
    typedef Cursor cursor;
    typedef color brush;
    typedef XftPattern* font;
    typedef int menu;

    typedef short point_type;
    typedef unsigned short size_type;
    typedef XftFont* font_type;
    typedef unsigned int cursor_type;

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

#define IF_WIN32(...)
#define IF_X11(...) __VA_ARGS__
#define IF_VC12(...)
#define IF_VC14(...)
#define IF_NOT_VC12(...) __VA_ARGS__

#else

#pragma error "Unknown target system"

#endif

#ifdef WIN32

color get_sys_color(int);

#endif // WIN32

#ifdef X11
    enum SystemColor {
        COLOR_SCROLLBAR,
        COLOR_BACKGROUND,
        COLOR_MENU,
        COLOR_MENUTEXT,
        COLOR_ACTIVECAPTION,
        COLOR_INACTIVECAPTION,
        COLOR_WINDOW,
        COLOR_WINDOWFRAME,
        COLOR_WINDOWTEXT,
        COLOR_CAPTIONTEXT,
        COLOR_INACTIVECAPTIONTEXT,
        COLOR_ACTIVEBORDER,
        COLOR_INACTIVEBORDER,
        COLOR_APPWORKSPACE,
        COLOR_HIGHLIGHT,
        COLOR_HIGHLIGHTTEXT,
        COLOR_GRAYTEXT,
        COLOR_BTNFACE,
        COLOR_BTNSHADOW,
        COLOR_BTNTEXT,
        COLOR_BTNHIGHLIGHT
    };

    color get_sys_color(SystemColor);
#endif // X11

    typedef unsigned char color_part_type;

    namespace detail {

#ifdef WIN32
      const int red_shift = 0;
      const int green_shift  = 8;
      const int blue_shift = 16;
      const int alpha_shift = 24;
#endif // WIN32
#ifdef X11
      const int red_shift = 16;
      const int green_shift  = 8;
      const int blue_shift = 0;
      const int alpha_shift = 24;
#endif // X11

      template<color_part_type P, int S>
      struct rgb_primary {
        static const color value = (color)P << S;
      };

      template<color C, int S>
      struct extract_rgb_primary {
        static const color_part_type value = (color_part_type)((C & (detail::rgb_primary<0xff, S>::value)) >> S);
      };

      template<int S>
      constexpr color shift_color_part (color_part_type p) {
        return (color)p << S;
      }

      template<int S>
      constexpr color_part_type split_color_part (color c) {
        return (color_part_type)((c & (detail::rgb_primary<0xff, S>::value)) >> S);
      }

    }

    template<color_part_type P>
    using red_primary = detail::rgb_primary<P, detail::red_shift>;

    template<color_part_type P>
    using green_primary = detail::rgb_primary<P, detail::green_shift>;

    template<color_part_type P>
    using blue_primary = detail::rgb_primary<P, detail::blue_shift>;

    template<color_part_type P>
    using alpha_primary = detail::rgb_primary<P, detail::alpha_shift>;


    template<color_part_type R, color_part_type G, color_part_type B>
    struct rgb_color {
      static const color value = red_primary<R>::value |
                                 green_primary<G>::value |
                                 blue_primary<B>::value;
    };

    template<color_part_type R, color_part_type G, color_part_type B, color_part_type A>
    struct rgba_color {
      static const color value = red_primary<R>::value |
                                 green_primary<G>::value |
                                 blue_primary<B>::value |
                                 alpha_primary<A>::value;
    };

    const color black = rgb_color<0, 0, 0>::value;
    const color white = rgb_color<0xff, 0xff, 0xff>::value;


    template<color C>
    using extract_red_primary = detail::extract_rgb_primary<C, detail::red_shift>;

    template<color C>
    using extract_green_primary = detail::extract_rgb_primary<C, detail::green_shift>;

    template<color C>
    using extract_blue_primary = detail::extract_rgb_primary<C, detail::blue_shift>;

    template<color C>
    using extract_alpha_primary = detail::extract_rgb_primary<C, detail::alpha_shift>;


    constexpr color build_red_primary (color_part_type p) {
      return detail::shift_color_part<detail::red_shift>(p);
    }

    constexpr color build_green_primary (color_part_type p) {
      return detail::shift_color_part<detail::green_shift>(p);
    }

    constexpr color build_blue_primary (color_part_type p) {
      return detail::shift_color_part<detail::blue_shift>(p);
    }

    constexpr color build_alpha_primary (color_part_type p) {
      return detail::shift_color_part<detail::alpha_shift>(p);
    }


    constexpr color_part_type extract_red (color c) {
      return detail::split_color_part<detail::red_shift>(c);
    }

    constexpr color_part_type extract_green (color c) {
      return detail::split_color_part<detail::green_shift>(c);
    }

    constexpr color_part_type extract_blue (color c) {
      return detail::split_color_part<detail::blue_shift>(c);
    }

    constexpr color_part_type extract_alpha (color c) {
      return detail::split_color_part<detail::alpha_shift>(c);
    }


    constexpr color rgb (color_part_type r, color_part_type g, color_part_type b) {
      return build_red_primary(r) | build_green_primary(g) | build_blue_primary(b);
    }

    constexpr color rgba (color_part_type r, color_part_type g, color_part_type b, color_part_type a) {
      return build_red_primary(r) | build_green_primary(g) | build_blue_primary(b) | build_alpha_primary(a);
    }

    constexpr color rgb_gray (color_part_type v) {
      return rgb(v, v, v);
    }

    constexpr color rgba_gray (color_part_type v, color_part_type a) {
      return rgba(v, v, v, a);
    }


  } // os

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

