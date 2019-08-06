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


namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    namespace format {

      template<PixelFormat From>
      struct line<From, PixelFormat::BW> {
        static inline void convert (const draw::const_image_row<From> in,
                                    draw::image_row<PixelFormat::BW> out,
                                    uint32_t w) {
          for (uint_fast32_t x = 0; x < w; ++x) {
            out[x] = get_bw(in[x]);
          }
        }
      };

      template<PixelFormat From, PixelFormat To>
      void line<From, To>::convert (const draw::const_image_row<From> in,
                                    draw::image_row<To> out,
                                    uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          out[x] = in[x];
        }
      }

//      template<>
//      struct line<PixelFormat::BW, PixelFormat::BW> {
//        static inline void convert (const draw::const_image_row<PixelFormat::BW> in,
//                                    draw::image_row<PixelFormat::BW> out,
//                                    uint32_t w) {
//          for (uint_fast32_t x = 0; x < w; ++x) {
//            out[x] = in[x];
//          }
//        }
//      };

      template<PixelFormat From, PixelFormat To>
      void convert (const draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          line<From, To>::convert(in.row(y), out.row(y), w);
        }
      }


    } // namespace format

    namespace copy {

      template<PixelFormat px_fmt>
      void row (const draw::const_image_row<px_fmt> src,
                draw::image_row<px_fmt> dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          dst[dest_x0 + x] = src[src_x0 + x];
        }
      }

      template<PixelFormat px_fmt>
      void sub (const draw::const_image_data<px_fmt> src_data,
                draw::image_data<px_fmt> dest_data,
                const core::uint32_point& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          row<px_fmt>(src_data.row(src.y() + y),
                   dest_data.row(dest.y() + y),
                   src.x(), dest.x(), dest.width());
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<PixelFormat px_fmt>
      void row (const draw::const_image_row<px_fmt> src,
                draw::image_row<px_fmt> dst,
                uint32_t src_x0, uint32_t dest_x0,
                uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = (x * src_w / dest_w);
          dst[dest_x0 + x] = src[src_x0 + src_x];
        }
      }

      template<PixelFormat px_fmt>
      void sub (const draw::const_image_data<px_fmt> src_data,
                draw::image_data<px_fmt> dest_data,
                const core::uint32_rect& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          const uint32_t src_y = y * src.height() / dest.height();
          row<px_fmt>(src_data.row(src.y() + src_y),
                   dest_data.row(dest.y() + y),
                   src.x(), dest.x(), src.width(), dest.width());
        }
      }

    } // namespace stretch

    namespace brightness {

      template<PixelFormat px_fmt>
      void row (draw::image_row<px_fmt> data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = data[x] * f;
        }
      }

      template<PixelFormat px_fmt>
      void adjust (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, float f) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<px_fmt>(data.row(y), w, f);
        }
      }

    }

    namespace fill {

      template<PixelFormat px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void row (draw::image_row<px_fmt> data, uint32_t w, const pixel_type& px) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = px;
        }
      }

      template<PixelFormat px_fmt, typename pixel_type = typename draw::BPP2Pixel<px_fmt>::pixel>
      void fill (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, const pixel_type& px) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<px_fmt>(data.row(y), w, px);
        }
      }

    }

  } // namespace convert

} // namespace gui
