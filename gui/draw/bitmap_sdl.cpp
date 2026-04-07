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

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr) {
      return SDL_CreateRGBSurfaceWithFormat(0, bmi.width, bmi.height,
        bmi.depth(), static_cast<Uint32>(bmi.pixel_format));
    }

    void free_bitmap (os::bitmap& id) {
      if (id) {
        logging::debug() << "SDL_FreeSurface";
        SDL_FreeSurface(id);
        id = nullptr;
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        return draw::bitmap_info(id->w, id->h, static_cast<pixel_format_t>(id->format->format));
      }
      return {};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      if (id) {
        memcpy(id->pixels, data, bmi.mem_size());
      }
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      if (id) {
        auto bits = static_cast<const blob::value_type*>(id->pixels);
        data.assign(bits, bits + bmi.mem_size());
      } else {
        bmi = {};
        data.clear();
      }
    }

    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs) {
      SDL_BlitSurface(rhs.get_os_bitmap(), NULL, lhs.get_os_bitmap(), NULL);
    }

  } // namespace native

}

#endif // GUIPP_SDL
