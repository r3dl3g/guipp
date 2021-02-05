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
 * @brief     C++ API: x11 typedefs
 *
 * @file
 */

#pragma once

#ifndef GUIPP_X11
# error Include guidefs.h instead guidefs_x11.h!
#endif // GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_USE_XCB
# include <X11/Xlib-xcb.h>
#endif // GUIPP_USE_XCB
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#ifdef GUIPP_USE_XFT
#include <X11/Xft/Xft.h>
#include <X11/extensions/Xrender.h>
#endif // GUIPP_USE_XFT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  namespace os {

    typedef Display* instance;
    typedef Window window;
    typedef Drawable drawable;
    typedef GC graphics;

    typedef unsigned long color;
    typedef unsigned int style;

    typedef Pixmap backstore;
    typedef Pixmap bitmap;
    typedef Pixmap icon;
    typedef Cursor cursor;
    typedef color brush;
#ifdef GUIPP_USE_XFT
    typedef XftPattern* font;
#else
    typedef Font font;
#endif // GUIPP_USE_XFT

    typedef short point_type;
    typedef unsigned short size_type;
#ifdef GUIPP_USE_XFT
    typedef XftFont* font_type;
#else
    typedef XFontStruct* font_type;
#endif // GUIPP_USE_XFT
    typedef unsigned int cursor_type;

    typedef unsigned int key_state;
    typedef KeySym key_symbol;

    typedef XPoint point;
    typedef struct {
      size_type cx;
      size_type cy;
    } size;
    typedef XRectangle rectangle;

    inline point_type get_x (const XPoint& p) { return p.x; }
    inline point_type get_y (const XPoint& p) { return p.y; }

    inline size_type get_width (const size& s) { return s.cx; }
    inline size_type get_height (const size& s) { return s.cy; }

    inline point_type get_x (const XRectangle& r) { return r.x; }
    inline point_type get_y (const XRectangle& r) { return r.y; }
    inline size_type get_width (const XRectangle& r) { return r.width; }
    inline size_type get_height (const XRectangle& r) { return r.height; }


    typedef int event_id;
    typedef Atom message_type;
    typedef int event_result;

    namespace x11 {
      typedef int screen;
      typedef Visual* visual;
    } // x11

  } // os

  namespace core {

    namespace global {

      namespace x11 {
        gui::os::x11::screen get_screen ();
        void set_screen(gui::os::x11::screen);
        gui::os::x11::visual get_visual ();
      }

    } // global

  } // core

} //gui

#ifndef GUIPP_USE_XFT

#define FC_WEIGHT_THIN       1
#define FC_WEIGHT_ULTRALIGHT 2
#define FC_WEIGHT_LIGHT      3
#define FC_WEIGHT_REGULAR    4
#define FC_WEIGHT_MEDIUM     5
#define FC_WEIGHT_SEMIBOLD   6
#define FC_WEIGHT_BOLD       7
#define FC_WEIGHT_ULTRABOLD  8
#define FC_WEIGHT_HEAVY      9

#endif // GUIPP_USE_XFT
