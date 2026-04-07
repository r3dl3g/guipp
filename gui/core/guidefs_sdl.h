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

#ifndef GUIPP_SDL
# error Include guidefs.h instead guidefs_sdl.h!
#endif //GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <vector>
#include <string>
#include <variant>
#include <SDL_render.h>
#include <SDL_mouse.h>
#include <SDL_keyboard.h>
extern "C" {
  #include "SDL_FontCache.h"
}


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/bits.h"


namespace gui {

  namespace os {

    using instance = int;
    using window = SDL_Window*;
    using backstore = SDL_Texture*;
    using bitmap = SDL_Texture*;
    using icon = SDL_Surface*;
    using graphics = SDL_Renderer*;
    using drawable = std::variant<SDL_Window*, SDL_Texture*, SDL_Surface*>;

    typedef Uint32 /* SDL_Color */ color;
    typedef Uint32 style;

    typedef SDL_Cursor* cursor;
    typedef SDL_Color brush;

    typedef int point_type;
    typedef int size_type;
    typedef SDL_SystemCursor cursor_type;

    typedef SDL_Keymod key_state;
    typedef SDL_Keycode key_symbol;

    namespace sdl {

      struct size {
        int w;
        int h;
      };

    } // namespace js

    typedef SDL_Point point;
    typedef sdl::size size;
    typedef SDL_Rect rectangle;
    typedef FC_Font* font_type;

    inline point_type get_x (const point& p) { return p.x; }
    inline point_type get_y (const point& p) { return p.y; }

    inline size_type get_width (const size& s) { return s.w; }
    inline size_type get_height (const size& s) { return s.w; }

    inline point_type get_x (const rectangle& r) { return r.x; }
    inline point_type get_y (const rectangle& r) { return r.y; }
    inline point_type get_x2 (const rectangle& r) { return r.x + r.w; }
    inline point_type get_y2 (const rectangle& r) { return r.y + r.h; }
    inline size_type get_width (const rectangle& r) { return r.w; }
    inline size_type get_height (const rectangle& r) { return r.h; }

    inline rectangle mk_rectangle (point_type x, point_type y, size_type w, size_type h) {
      return rectangle{x, y, w, h};
    }

    typedef Uint32 event_id;
    typedef Uint32 message_type;
    typedef int event_result;

  } // os

} //gui
