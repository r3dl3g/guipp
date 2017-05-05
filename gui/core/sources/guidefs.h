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

  typedef uint8_t byte;
  typedef byte* byteptr;
  typedef const byte* cbyteptr;

  enum class bit_order : bool {
    lsb,
    msb
  };

  namespace os {

#ifdef WIN32

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

    const bit_order bitmap_bit_order = bit_order::lsb;

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

  template<byte bit>
  struct msb_bit_mask {
    static constexpr byte shift = 7 - bit;
    static constexpr byte value = 0x01 << shift;
  };

  template<byte bit>
  struct lsb_bit_mask {
    static constexpr byte shift = bit;
    static constexpr byte value = 0x01 << shift;
  };

  template<byte bit, bit_order O = os::bitmap_bit_order> struct bitmap_bit_mask {};

  template<byte bit> struct bitmap_bit_mask<bit, bit_order::lsb> : lsb_bit_mask<bit> {};
  template<byte bit> struct bitmap_bit_mask<bit, bit_order::msb> : msb_bit_mask<bit> {};


  template<bit_order O = os::bitmap_bit_order> struct bw_bits {};

  template<> struct bw_bits<bit_order::lsb> {
    static constexpr byte value[2] = { 0, 0xff };
    static constexpr byte adapt (byte v) {
      return v;
    }
  };

  template<> struct bw_bits<bit_order::msb> {
    static constexpr byte value[2] = { 0xff, 0 };
    static constexpr byte adapt(byte v) {
      return v ^ 0xff;
    }
  };


  struct system_bw_bits : public bw_bits<os::bitmap_bit_order> {
    static constexpr byte mask[8] = {
      bitmap_bit_mask<0>::value,
      bitmap_bit_mask<1>::value,
      bitmap_bit_mask<2>::value,
      bitmap_bit_mask<3>::value,
      bitmap_bit_mask<4>::value,
      bitmap_bit_mask<5>::value,
      bitmap_bit_mask<6>::value,
      bitmap_bit_mask<7>::value
    };
    static constexpr byte shift[8] = {
      bitmap_bit_mask<0>::shift,
      bitmap_bit_mask<1>::shift,
      bitmap_bit_mask<2>::shift,
      bitmap_bit_mask<3>::shift,
      bitmap_bit_mask<4>::shift,
      bitmap_bit_mask<5>::shift,
      bitmap_bit_mask<6>::shift,
      bitmap_bit_mask<7>::shift
    };
  };


  constexpr byte reverse_lookup_table[16] = {
      0x0, 0x8, 0x4, 0xc, 0x2, 0xa, 0x6, 0xe,
      0x1, 0x9, 0x5, 0xd, 0x3, 0xb, 0x7, 0xf
  };

  constexpr byte reverse_bit_order (byte n) {
     // Reverse the top and bottom nibble then swap them.
     return (reverse_lookup_table[n & 0b1111] << 4) | reverse_lookup_table[n >> 4];
  }


  enum class BPP : unsigned char {
    Undefined = 0,
    BW = 1,
    GRAY = 8,
    RGB = 24,
    RGBA = 32
  };

  enum class orientation : bool {
    vertical,
    horizontal
  };

  constexpr orientation operator! (orientation o) {
    return (o == orientation::vertical) ? orientation::horizontal : orientation::vertical;
  }

  enum class origin : unsigned char {
    start,
    center,
    end
  };

  enum class alignment : unsigned char {
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

