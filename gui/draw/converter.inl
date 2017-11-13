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

      template<>
      inline void set<BPP::GRAY>(bytearray out, uint32_t x, byte v) {
        out[x] = v;
      }

      template<>
      void set<BPP::BW>(bytearray out, uint32_t x, byte v);

      template<>
      void set<BPP::RGB>(bytearray out, uint32_t x, byte v);

      template<>
      void set<BPP::RGBA>(bytearray out, uint32_t x, byte v);

      template<>
      inline byte get<BPP::GRAY>(cbytearray in, uint32_t x) {
        return in[x];
      }

      template<>
      byte get<BPP::BW>(cbytearray in, uint32_t x);

      template<>
      byte get<BPP::RGB>(cbytearray in, uint32_t x);

      template<>
      byte get<BPP::RGBA>(cbytearray in, uint32_t x);

      template<BPP From, BPP To>
      void line<From, To>::convert (cbytearray in, bytearray out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          set<To>(out, x, get<From>(in, x));
        }
      }

      template<>
      struct line<BPP::RGB, BPP::RGBA> {
        static void convert (cbytearray in, bytearray out, uint32_t w);
      };

      template<>
      struct line<BPP::RGBA, BPP::RGB> {
        static void convert (cbytearray in, bytearray out, uint32_t w);
      };

      template<BPP From>
      struct line<From, BPP::BW> {
        static void convert (cbytearray in, bytearray out, uint32_t w) {
          for (uint_fast32_t x = 0; x < w; x += 8) {
            byte ovalue = (get<From>(in, x + 0) ? bitmap_bit_mask<0>::value : 0)
                        | (get<From>(in, x + 1) ? bitmap_bit_mask<1>::value : 0)
                        | (get<From>(in, x + 2) ? bitmap_bit_mask<2>::value : 0)
                        | (get<From>(in, x + 3) ? bitmap_bit_mask<3>::value : 0)
                        | (get<From>(in, x + 4) ? bitmap_bit_mask<4>::value : 0)
                        | (get<From>(in, x + 5) ? bitmap_bit_mask<5>::value : 0)
                        | (get<From>(in, x + 6) ? bitmap_bit_mask<6>::value : 0)
                        | (get<From>(in, x + 7) ? bitmap_bit_mask<7>::value : 0);
            out[x / 8] = system_bw_bits::adapt(ovalue);
          }
        }

      };

      template<>
      struct line<BPP::BW, BPP::BW> {
        static void convert (cbytearray in, bytearray out, uint32_t w) {
          for (uint_fast32_t x = 0; x < w; x += 8) {
            const uint_fast32_t i = x / 8;
            out[i] = in[i];
          }
        }

      };

      template<BPP From, BPP To>
      void convert (cbytearray src, bytearray dst, uint32_t w, uint32_t h, uint32_t src_bpl, uint32_t dst_bpl) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          line<From, To>::convert(src.sub(y * src_bpl, src_bpl), dst.sub(y * dst_bpl, dst_bpl), w);
        }
      }

    } // namespace bpp

    namespace copy {

      template<>
      inline void row<BPP::BW> (cbytearray src, bytearray dst,
                                uint32_t src_x0, uint32_t dest_x0, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          byte b = bpp::get<BPP::BW>(src, src_x0 + x);
          bpp::set<BPP::BW>(dst, dest_x0 + x, b);
        }
      }

      template<>
      inline void row<BPP::GRAY> (cbytearray src, bytearray dst,
                                  uint32_t src_x0, uint32_t dest_x0, uint32_t w) {
        memcpy(dst + dest_x0, src + src_x0, w);
      }

      template<>
      inline void row<BPP::RGB> (cbytearray src, bytearray dst,
                                 uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 3, src + src_x0 * 3, dest_w * 3);
      }

      template<>
      inline void row<BPP::RGBA> (cbytearray src, bytearray dst,
                                  uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 4, src + src_x0 * 4, dest_w * 4);
      }

      template<BPP bpp>
      void sub (cbytearray src_data, uint32_t src_bpl,
                bytearray dest_data, uint32_t dest_bpl,
                const core::uint32_point& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          row<bpp>(src_data.sub((src.y() + y) * src_bpl, src_bpl),
                   dest_data.sub((dest.y() + y) * dest_bpl, dest_bpl),
                   src.x(), dest.x(), dest.width());
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void sub (cbytearray src_data, uint32_t src_bpl,
                bytearray dest_data, uint32_t dest_bpl,
                const core::uint32_rect& src,
                const core::uint32_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          const uint32_t src_y = y * src.height() / dest.height();
          row<bpp>(src_data.sub((src.y() + src_y) * src_bpl, src_bpl),
                   dest_data.sub((dest.y() + y) * dest_bpl, dest_bpl),
                   src.x(), dest.x(), src.width(), dest.width());
        }
      }

    } // namespace stretch

    namespace brightness {

      template<>
      void row<BPP::GRAY> (bytearray data, uint32_t w, float f);

      template<>
      void row<BPP::RGB> (bytearray data, uint32_t w, float f);

      template<>
      void row<BPP::RGBA> (bytearray data, uint32_t w, float f);

      template<BPP bpp>
      void adjust (bytearray data, uint32_t w, uint32_t h, uint32_t bpl, float f) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<bpp>(data.sub(y * bpl, bpl), w, f);
        }
      }

    }

  } // namespace convert

} // namespace gui
