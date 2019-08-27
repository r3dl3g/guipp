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
    namespace format {

      template<PixelFormat From, PixelFormat To>
      struct line {
        static void convert (const typename draw::const_image_data<From>::row_type in,
                             typename draw::image_data<To>::row_type out,
                             uint32_t w);
      };

      template<PixelFormat From, PixelFormat To>
      void convert (const typename draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h);

    } // namespace format

    // --------------------------------------------------------------------------
    namespace copy {

      template<PixelFormat px_fmt>
      void row (const typename draw::const_image_data<px_fmt>::row_type src,
                typename draw::image_data<px_fmt>::row_type dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w);

      template<PixelFormat px_fmt>
      void sub (const typename draw::const_image_data<px_fmt> src_data,
                draw::image_data<px_fmt> dest_data,
                const core::native_point& src,
                const core::native_rect& dest);


    } // namespace copy

    // --------------------------------------------------------------------------
    enum class interpolation : unsigned char {
      nearest,
      bilinear,
      bicubic,
    };

    template<PixelFormat F, interpolation I = interpolation::nearest>
    struct stretch {

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest);

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data);

    }; // namespace stretch

    // --------------------------------------------------------------------------
    namespace brightness {

      template<PixelFormat px_fmt>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, float f);

      template<PixelFormat px_fmt>
      void adjust (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, float f);

    }

    // --------------------------------------------------------------------------
    namespace fill {

      template<PixelFormat px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, const pixel_type& px);

      template<PixelFormat px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void fill (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, const pixel_type& px);

    }

    // --------------------------------------------------------------------------
  } // namespace convert

} // namespace gui

#include <gui/draw/converter.inl>
