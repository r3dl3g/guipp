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
 * @brief     C++ API: x11 typedefs
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xft/Xft.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/bits.h>


namespace gui {

  namespace os {

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

  } // os

  namespace core {

    namespace global {

      os::x11::screen get_screen ();
      void set_screen(os::x11::screen);

      os::x11::visual get_visual ();

    } // global

  } // core

} //gui
