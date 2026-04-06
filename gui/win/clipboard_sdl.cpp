/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     system clipboard helper
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_SDL

#include <limits>
#include <SDL_clipboard.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/clipboard.h"

namespace gui {

  namespace win {

    clipboard& clipboard::get () {
      static clipboard c;
      return c;
    }

    clipboard::clipboard ()
    {}

    void clipboard::set_text (window& win, const std::string& t) {
      SDL_SetClipboardText(t.c_str());
    }

    void clipboard::get_text (window& win, std::function<clipboard::text_callback>&& cb) {
      if (SDL_HasClipboardText()) {
        cb(SDL_GetClipboardText());
      }
    }

  } // win

} // gui

#endif // GUIPP_SDL
