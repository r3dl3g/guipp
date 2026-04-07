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
 * @brief     graphic functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_SDL

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <SDL_video.h>
#include <SDL_render.h>

#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/graphics.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/shared_datamap.h"
#include "gui/draw/drawers.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/use.h"


namespace gui {

  namespace draw {

    graphics& graphics::draw_pixel (const core::native_point& pt, os::color c) {

      SDL_Rect rect = { pt.x(), pt.y(), 1, 1 };
      SDL_SetRenderDrawColor(gc(), static_cast<int>(color::extract<color::part::red>(c)),
                                   static_cast<int>(color::extract<color::part::green>(c)),
                                   static_cast<int>(color::extract<color::part::blue>(c)),
                                   static_cast<int>(color::extract<color::part::alpha>(c)));
      SDL_RenderFillRect(gc(), &rect);

      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {

      SDL_Rect rect = { pt.x(), pt.y(), 1, 1 };
      Uint32 pixel = 0;
      SDL_RenderReadPixels(gc(), &rect, 0, &pixel, 1);
      return pixel;
    }

    graphics& graphics::draw_lines (const std::vector<core::point>& points,
                                    const pen& p) {
      return frame(polyline(points), p);
    }

    void graphics::flush () {
      SDL_RenderPresent(gc());
    }

    int graphics::depth () const {
      return 32;
    }

    core::native_rect graphics::native_area () const {
      int w = 0, h = 0;
      SDL_RenderGetLogicalSize(gc(), &w, &h);
      return {0, 0, static_cast<core::native_rect::size_type>(w), static_cast<core::native_rect::size_type>(h)};
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      return copy_from(src.target(), r, pt);
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      return copy_from(bmp.image, core::native_rect(bmp.image.native_size()), pt);
    }

    graphics& graphics::copy_from (os::drawable src,
                                   const core::native_rect& r,
                                   const core::native_point& pt,
                                   const copy_mode) {
      SDL_Rect srcrect = { r.x(), r.y(), static_cast<int>(r.width()), static_cast<int>(r.height()) };
      SDL_Rect destrect = { pt.x(), pt.y(), static_cast<int>(r.width()), static_cast<int>(r.height()) };
      SDL_Surface* surface = nullptr;

      if (std::holds_alternative<gui::os::window>(src)) {
        auto win = std::get<gui::os::window>(src);
        surface = SDL_GetWindowSurface(win);
      } if (std::holds_alternative<gui::os::bitmap>(src)) {
        surface = std::get<gui::os::bitmap>(src);
      }

      if (surface) {
        auto texture = SDL_CreateTextureFromSurface(gc(), surface);
        SDL_RenderCopy(gc(), texture, &srcrect, &destrect);
        logging::debug() << "graphics::copy_from(drawable)->SDL_DestroyTexture";
        SDL_DestroyTexture(texture);
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap, const core::rectangle& src, const core::point& pt) {
      return copy_from(pixmap, core::global::scale_to_native(src), core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap,
                                   const core::native_rect& r,
                                   const core::native_point& pt) {
      SDL_Rect srcrect = { r.x(), r.y(), static_cast<int>(r.width()), static_cast<int>(r.height()) };
      SDL_Rect destrect = { pt.x(), pt.y(), static_cast<int>(r.width()), static_cast<int>(r.height()) };
      SDL_Surface* surface = pixmap.get_os_bitmap();
      auto texture = SDL_CreateTextureFromSurface(gc(), surface);
      SDL_RenderCopy(gc(), texture, &srcrect, &destrect);
        logging::debug() << "graphics::copy_from(pixmap)->SDL_DestroyTexture";
      SDL_DestroyTexture(texture);
      return *this;
    }

    graphics& graphics::draw_streched (const draw::pixmap& pixmap,
                                       const core::native_rect& dst,
                                       const core::native_point& src,
                                       const std::string& filter) {
      const auto sz = pixmap.native_size();
      SDL_Rect srcrect = { 0, 0, static_cast<int>(sz.width()), static_cast<int>(sz.height()) };
      SDL_Rect destrect = { dst.x(), dst.y(), static_cast<int>(dst.width()), static_cast<int>(dst.height()) };
      SDL_Surface* surface = pixmap.get_os_bitmap();
      auto texture = SDL_CreateTextureFromSurface(gc(), surface);
      SDL_SetTextureBlendMode(texture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
      if (filter == "Nearest") {
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_ScaleModeNearest);
      } else if (filter == "Linear") {
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_ScaleModeLinear);
      } else /* if (filter == "Best") */ {
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_ScaleModeBest);
      }
      SDL_RenderCopy(gc(), texture, &srcrect, &destrect);
        logging::debug() << "graphics::draw_streched()->SDL_DestroyTexture";
      SDL_DestroyTexture(texture);
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      SDL_Rect rect = r.os(context());
      SDL_BlendMode invertMode = SDL_ComposeCustomBlendMode(
          SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR, // Source Factor
          SDL_BLENDFACTOR_ZERO,                // Dest Factor
          SDL_BLENDOPERATION_ADD,              // Color Operation
          SDL_BLENDFACTOR_ONE,                 // Alpha Source Factor
          SDL_BLENDFACTOR_ZERO,                // Alpha Dest Factor
          SDL_BLENDOPERATION_ADD               // Alpha Operation
      );

      SDL_SetRenderDrawBlendMode(gc(), invertMode);
      SDL_SetRenderDrawColor(gc(), 255, 255, 255, 255);
      SDL_RenderFillRect(gc(), &rect);      // auto rect = core::global::scale_to_native(r);
    }

    std::vector<std::string> graphics::get_filter_list(os::drawable d) {
      static std::vector<std::string> list = {"Nearest", "Linear", "Best"};
      return list;
    }

    // --------------------------------------------------------------------------

  }

}

#endif // GUIPP_SDL
