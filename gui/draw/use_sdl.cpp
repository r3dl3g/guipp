/**
 * @copyright (c) 2016-2026 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     Helper to use pen, brush and font
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#if GUIPP_SDL

#include <SDL2_gfxPrimitives.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/use.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/font.h"

namespace gui {

  namespace draw {
    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen& p) {
      auto c = p.color();
      SDL_SetRenderDrawColor(g, static_cast<int>(color::extract<color::part::red>(c)),
                                static_cast<int>(color::extract<color::part::green>(c)),
                                static_cast<int>(color::extract<color::part::blue>(c)),
                                static_cast<int>(color::extract<color::part::alpha>(c)));
    }

    template<>
    void Use<brush>::set (const brush& b) {
      auto c = b.color();
      SDL_SetRenderDrawColor(g, static_cast<int>(color::extract<color::part::red>(c)),
                                static_cast<int>(color::extract<color::part::green>(c)),
                                static_cast<int>(color::extract<color::part::blue>(c)),
                                static_cast<int>(color::extract<color::part::alpha>(c)));
    }

    template<>
    void Use<font>::set (const font& f) {
      // TBD
    }

    
  } // namespace draw
  
} // namespace gui

#endif // GUIPP_SDL