/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: bitmap converter
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/array_wrapper.h>
#include <gui/core/guidefs.h>
#include <gui/core/rectangle.h>
#include <gui/draw/image_data.h>


namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    namespace bpp {

      template<BPP From, BPP To>
      struct line {
        static void convert (const draw::const_image_row<From> in,
                             draw::image_row<To> out,
                             uint32_t w);
      };

      template<BPP From, BPP To>
      void convert (const draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h);

    } // namespace bpp

    // --------------------------------------------------------------------------
    namespace copy {

      template<BPP bpp>
      void row (const draw::const_image_row<bpp> src,
                draw::image_row<bpp> dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w);

      template<BPP bpp>
      void sub (const draw::const_image_data<bpp> src_data,
                draw::image_data<bpp> dest_data,
                const core::uint32_point& src,
                const core::uint32_rect& dest);


    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (const draw::const_image_row<bpp> src,
                draw::image_row<bpp> dst,
                uint32_t src_x0, uint32_t dest_x0,
                uint32_t src_w, uint32_t dest_w);

      template<BPP bpp>
      void sub (const draw::const_image_data<bpp> src_data,
                draw::image_data<bpp> dest_data,
                const core::uint32_rect& src,
                const core::uint32_rect& dest);

    } // namespace stretch

    // --------------------------------------------------------------------------
    namespace brightness {

      template<BPP bpp>
      void row (draw::image_row<bpp> data, uint32_t w, float f);

      template<BPP bpp>
      void adjust (draw::image_data<bpp> data, uint32_t w, uint32_t h, float f);

    }

    // --------------------------------------------------------------------------
  } // namespace convert

} // namespace gui

#include <gui/draw/converter.inl>
