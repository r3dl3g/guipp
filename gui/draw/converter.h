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
 * @brief     image_data converter
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/core/rectangle.h>
#include <gui/draw/image_data.h>


namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    namespace format {

      template<pixel_format_t From, pixel_format_t To>
      struct line {
        static void convert (typename draw::const_image_data<From>::row_type in,
                             typename draw::image_data<To>::row_type out,
                             uint32_t w);

        static void mask (typename draw::const_image_data<From>::row_type in,
                          typename draw::image_data<To>::row_type out,
                          uint32_t w, pixel::gray limit);
      };

      template<pixel_format_t From, pixel_format_t To>
      void convert (typename draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h);

      template<pixel_format_t From, pixel_format_t To>
      void mask (typename draw::const_image_data<From> in,
                 draw::image_data<To> out,
                 uint32_t w, uint32_t h, pixel::gray limit);

    } // namespace format

    // --------------------------------------------------------------------------
    namespace copy {

      template<pixel_format_t px_fmt>
      void row (typename draw::const_image_data<px_fmt>::row_type src,
                typename draw::image_data<px_fmt>::row_type dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w);

      template<pixel_format_t px_fmt>
      void sub (typename draw::const_image_data<px_fmt> src_data,
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

    template<pixel_format_t F, interpolation I = interpolation::nearest>
    struct stretch {

      static void sub (typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest);

      static void sub (typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data);

    }; // namespace stretch

    // --------------------------------------------------------------------------
    namespace brightness {

      template<pixel_format_t px_fmt>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, double f);

      template<pixel_format_t px_fmt>
      void adjust (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, double f);

    }

    // --------------------------------------------------------------------------
    namespace fill {

      template<pixel_format_t px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, const pixel_type& px);

      template<pixel_format_t px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void fill (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, const pixel_type& px);

    }

    // --------------------------------------------------------------------------
  } // namespace convert

} // namespace gui

#include <gui/draw/converter.inl>
