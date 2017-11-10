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
// Library includes
//
#include <gui/core/guidefs.h>


namespace gui {

  namespace convert {

    namespace bpp {

      template<BPP W>
      void set (byteptr out, uint32_t x, byte v);

      template<BPP W>
      byte get (cbyteptr in, uint32_t x);

      template<BPP From, BPP To>
      struct line {
        static void convert (cbyteptr in, byteptr out, uint32_t w);
      };

      template<BPP From, BPP To>
      void convert (const blob& src, blob& dst, uint32_t w, uint32_t h, uint32_t src_bpl, uint32_t dst_bpl);

    } // namespace bpp

    namespace copy {

      // --------------------------------------------------------------------------
      template<BPP bpp>
      void row (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<BPP bpp>
      void sub (const blob& src_data, uint32_t src_bpl,
                blob& dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0,
                uint32_t dest_x0, uint32_t dest_y0,
                uint32_t dest_w, uint32_t dest_h);


    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (const blob& src_data, blob& dest_data,
                uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w);

      template<BPP bpp>
      void sub (const blob& src_data, uint32_t src_bpl,
                blob& dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h);

      template<>
      void sub<BPP::BW> (const blob& src_data, uint32_t src_bpl,
                         blob& dest_data, uint32_t dest_bpl,
                         uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                         uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h);

    } // namespace stretch

    namespace brightness {

      template<BPP bpp>
      void row (byteptr data, uint32_t offset, uint32_t w, float f);

      template<BPP bpp>
      void adjust (blob& data, uint32_t w, uint32_t h, uint32_t bpl, float f);

    }


    // --------------------------------------------------------------------------
    //
    // inlines
    //
    namespace bpp {

      template<>
      inline void set<BPP::GRAY>(byteptr out, uint32_t x, byte v) {
        out[x] = v;
      }

      template<>
      void set<BPP::BW>(byteptr out, uint32_t x, byte v);

      template<>
      void set<BPP::RGB>(byteptr out, uint32_t x, byte v);

      template<>
      void set<BPP::RGBA>(byteptr out, uint32_t x, byte v);

      template<>
      inline byte get<BPP::GRAY>(cbyteptr in, uint32_t x) {
        return in[x];
      }

      template<>
      byte get<BPP::BW>(cbyteptr in, uint32_t x);

      template<>
      byte get<BPP::RGB>(cbyteptr in, uint32_t x);

      template<>
      byte get<BPP::RGBA>(cbyteptr in, uint32_t x);

      template<BPP From, BPP To>
      void line<From, To>::convert (cbyteptr in, byteptr out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          set<To>(out, x, get<From>(in, x));
        }
      }

      template<>
      struct line<BPP::RGB, BPP::RGBA> {
        static void convert (cbyteptr in, byteptr out, uint32_t w);
      };

      template<>
      struct line<BPP::RGBA, BPP::RGB> {
        static void convert (cbyteptr in, byteptr out, uint32_t w);
      };

      template<BPP From>
      struct line<From, BPP::BW> {
        static void convert (cbyteptr in, byteptr out, uint32_t w) {
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
        static void convert (cbyteptr in, byteptr out, uint32_t w) {
          for (uint_fast32_t x = 0; x < w; x += 8) {
            out[x / 8] = in[x / 8];
          }
        }

      };

      template<BPP From, BPP To>
      void convert (const blob& src, blob& dst, uint32_t w, uint32_t h, uint32_t src_bpl, uint32_t dst_bpl) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          line<From, To>::convert(src.data() + (y * src_bpl), dst.data() + (y * dst_bpl), w);
        }
      }

    } // namespace bpp

    namespace copy {

      template<>
      inline void row<BPP::BW> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          byte b = bpp::get<BPP::BW>(src, src_x0 + x);
          bpp::set<BPP::BW>(dst, dest_x0 + x, b);
        }
      }

      template<>
      inline void row<BPP::GRAY> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0, src + src_x0, dest_w);
      }

      template<>
      inline void row<BPP::RGB> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 3, src + src_x0 * 3, dest_w * 3);
      }

      template<>
      inline void row<BPP::RGBA> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 4, src + src_x0 * 4, dest_w * 4);
      }

      template<BPP bpp>
      void sub (const blob& src_data, uint32_t src_bpl,
                blob& dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0,
                uint32_t dest_x0, uint32_t dest_y0,
                uint32_t dest_w, uint32_t dest_h) {
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          cbyteptr src = src_data.data() + (src_y0 + y) * src_bpl;
          byteptr dst = dest_data.data() + (dest_y0 + y) * dest_bpl;
          row<bpp>(src, dst, src_x0, dest_x0, dest_w);
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void sub (const blob& src_data, uint32_t src_bpl,
                blob& dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h) {
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          const uint32_t src_y = src_y0 + y * src_h / dest_h;
          const uint32_t src_offs = src_y * src_bpl;
          const uint32_t dest_offs = (dest_y0 + y) * dest_bpl;
          row<bpp>(src_data, dest_data, src_offs, dest_offs, src_x0, dest_x0, src_w, dest_w);
        }
      }

    } // namespace stretch

    namespace brightness {

      template<>
      void row<BPP::GRAY> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<>
      void row<BPP::RGB> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<>
      void row<BPP::RGBA> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<BPP bpp>
      void adjust (blob& data, uint32_t w, uint32_t h, uint32_t bpl, float f) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          const uint32_t offs = y * bpl;
          row<bpp>(data.data(), offs, w, f);
        }
      }

    }

  } // namespace convert

} // namespace gui
