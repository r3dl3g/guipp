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
 * @brief     monochrome and coloured fast bitmaps
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

 #ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <string>
#include <algorithm>
#include <map>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/pen.h"
#include "gui/draw/use.h"


namespace gui {

  namespace native {

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr ptr) {
      auto texture = SDL_CreateTexture(core::native::sdl::get_font_renderer(), 
                                       static_cast<Uint32>(bmi.pixel_format),
                                       SDL_TEXTUREACCESS_TARGET, bmi.width, bmi.height);
      if (ptr) {
        SDL_UpdateTexture(texture, NULL, ptr, bmi.bytes_per_line);
      }

      return texture;
    }

    void free_bitmap (os::bitmap& id) {
      if (id) {
        logging::trace() << "free_bitmap->SDL_DestroyTexture";
        SDL_DestroyTexture(id);
        id = nullptr;
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        Uint32 format;
        int access, w, h;
        SDL_QueryTexture(id, &format, &access, &w, &h);
        return draw::bitmap_info(w, h, static_cast<pixel_format_t>(format));
      }
      return {};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      if (id) {
        draw::bitmap_info ti = bitmap_get_info(id);
        if (ti == bmi) {
          SDL_UpdateTexture(id, NULL, data, bmi.bytes_per_line);
        }
      }
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      if (id) {
        bmi = bitmap_get_info(id);
        void *pixels;
        int pitch;
        SDL_LockTexture(id, NULL, &pixels, &pitch);
        bmi.bytes_per_line = pitch;
        auto bits = static_cast<const blob::value_type*>(pixels);
        data.assign(bits, bits + pitch * bmi.height);
      } else {
        bmi = {};
        data.clear();
      }
    }

    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs) {
      auto renderer = core::native::sdl::get_font_renderer();
      SDL_SetRenderTarget(renderer, rhs.get_os_bitmap());
      SDL_RenderCopy(renderer, lhs.get_os_bitmap(), NULL, NULL);
      SDL_SetRenderTarget(renderer, NULL);
    }

  } // namespace native

}

#endif // GUIPP_SDL
