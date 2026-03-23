/**
 * @copyright (c) 2026-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     browser specific gui defines and typedefs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

#ifndef GUIPP_JS
# error Include guidefs.h instead guidefs_js.h!
#endif //GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//
#include <emscripten/val.h>
#include <vector>
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/bits.h"


namespace gui {

  namespace os {

    template<typename T>
    struct handle {
      T id;

      handle (T t = {})
        : id(t)
      {}

      operator T () const {
        return id;
      }

    };

    using instance = emscripten::val; // document
    using window = emscripten::val;   // canvas
    using drawable = emscripten::val;
    using graphics = emscripten::val; // context 2d

    typedef uint32_t color;
    typedef int style;

    using backstore = drawable;
    using bitmap = emscripten::val;
    typedef int icon;
    typedef std::string cursor;
    typedef int brush;
    typedef std::string font;

    typedef int point_type;
    typedef int size_type;
    typedef int font_type;
    typedef std::string cursor_type;

    typedef unsigned int key_state;
    typedef unsigned int key_symbol;

    namespace js {
      struct point {
        int x;
        int y;
      };

      struct size {
        int cx;
        int cy;
      };

      struct rectangle {
        int x;
        int y;
        int x2;
        int y2;
      };

    } // namespace js

    typedef js::point point;
    typedef js::size size;
    typedef js::rectangle rectangle;

    inline point_type get_x (const point& p) { return p.x; }
    inline point_type get_y (const point& p) { return p.y; }

    inline size_type get_width (const size& s) { return s.cx; }
    inline size_type get_height (const size& s) { return s.cy; }

    inline point_type get_x (const rectangle& r) { return r.x; }
    inline point_type get_y (const rectangle& r) { return r.y; }
    inline point_type get_x2 (const rectangle& r) { return r.x2; }
    inline point_type get_y2 (const rectangle& r) { return r.y2; }
    inline size_type get_width (const rectangle& r) { return r.x2 - r.x; }
    inline size_type get_height (const rectangle& r) { return r.y2 - r.y; }

    inline rectangle mk_rectangle (point_type x, point_type y, point_type x2, point_type y2) {
      return rectangle{x, y, x2, y2};
    }

    namespace event_type {
      enum event_type {
        Create        =  1,
        Close         =  2,
        Destroy       =  3,
        Show          =  4,
        Hide          =  5,
        Layout        =  6,
        Paint         =  7,
        SetFocus      =  8,
        KillFocus     =  9,
        Move          = 10,
        Size          = 11,
        KeyDown       = 12,
        KeyUp         = 13,
        LButtonDown   = 14,
        LButtonUp     = 15,
        LButtonDblClk = 16,
        RButtonDown   = 17,
        RButtonUp     = 18,
        RButtonDblClk = 19,
        MButtonDown   = 20,
        MButtonUp     = 21,
        MButtonDblClk = 22,
        MouseMove     = 23,
        MouseEnter    = 24,
        MouseLeave    = 25,
        WheelH        = 26,
        WheelV        = 27,
        User          = 100
      };
    }

    typedef int event_id;
    typedef int message_type;
    typedef int event_result;

  } // os

} //gui
