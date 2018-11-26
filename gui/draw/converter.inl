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
    namespace bpp {

      template<BPP From, BPP To>
      void line<From, To>::convert (const draw::const_image_row<From> in,
                                    draw::image_row<To> out,
                                    uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          out[x] = in[x];
        }
      }

      template<>
      struct line<BPP::BW, BPP::BW> {
        static inline void convert (const draw::const_image_row<BPP::BW> in,
                                    draw::image_row<BPP::BW> out,
                                    uint32_t w) {
          for (uint_fast32_t x = 0; x < w; ++x) {
            out[x] = in[x];
          }
        }
      };

      template<BPP From>
      struct line<From, BPP::BW> {
        static inline void convert (const draw::const_image_row<From> in,
                                    draw::image_row<BPP::BW> out,
                                    uint32_t w) {
          for (uint_fast32_t x = 0; x < w; ++x) {
            out[x] = in[x].get_bw();
          }
        }
      };

      template<BPP From, BPP To>
      void convert (const draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          line<From, To>::convert(in.row(y), out.row(y), w);
        }
      }


    } // namespace bpp

    namespace copy {

      template<BPP bpp>
      void row (const draw::const_image_row<bpp> src,
                draw::image_row<bpp> dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          dst[dest_x0 + x] = src[src_x0 + x];
        }
      }

      template<BPP bpp>
      void sub (const draw::const_image_data<bpp> src_data,
                draw::image_data<bpp> dest_data,
                const core::uint32_point& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          row<bpp>(src_data.row(src.y() + y),
                   dest_data.row(dest.y() + y),
                   src.x(), dest.x(), dest.width());
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (const draw::const_image_row<bpp> src,
                draw::image_row<bpp> dst,
                uint32_t src_x0, uint32_t dest_x0,
                uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = (x * src_w / dest_w);
          dst[dest_x0 + x] = src[src_x0 + src_x];
        }
      }

      template<BPP bpp>
      void sub (const draw::const_image_data<bpp> src_data,
                draw::image_data<bpp> dest_data,
                const core::uint32_rect& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          const uint32_t src_y = y * src.height() / dest.height();
          row<bpp>(src_data.row(src.y() + src_y),
                   dest_data.row(dest.y() + y),
                   src.x(), dest.x(), src.width(), dest.width());
        }
      }

    } // namespace stretch

    namespace brightness {

      template<BPP bpp>
      void row (draw::image_row<bpp> data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = data[x] * f;
        }
      }

      template<BPP bpp>
      void adjust (draw::image_data<bpp> data, uint32_t w, uint32_t h, float f) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<bpp>(data.row(y), w, f);
        }
      }

    }

  } // namespace convert

} // namespace gui
