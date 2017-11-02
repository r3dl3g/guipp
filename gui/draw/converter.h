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
#include <gui/draw/graphics.h>


namespace gui {

  namespace convert {

    namespace bpp {

      template<BPP W>
      void set (byteptr out, int x, byte v);

      template<>
      inline void set<BPP::GRAY>(byteptr out, int x, byte v) {
        out[x] = v;
      }

      template<>
      void set<BPP::BW>(byteptr out, int x, byte v);

      template<>
      void set<BPP::RGB>(byteptr out, int x, byte v);

      template<>
      void set<BPP::RGBA>(byteptr out, int x, byte v);

      template<BPP W>
      byte get (cbyteptr in, int x);

      template<>
      byte get<BPP::BW>(cbyteptr in, int x);

      template<>
      inline byte get<BPP::GRAY>(cbyteptr in, int x) {
        return in[x];
      }

      template<>
      byte get<BPP::RGB>(cbyteptr in, int x);

      template<>
      byte get<BPP::RGBA>(cbyteptr in, int x);

      template<BPP From, BPP To>
      struct line_converter {
        static void convert (cbyteptr in, byteptr out, int w) {
          for (int x = 0; x < w; ++x) {
            set<To>(out, x, get<From>(in, x));
          }
        }

      };

      template<>
      struct line_converter<BPP::RGB, BPP::RGBA> {
        static void convert (cbyteptr in, byteptr out, int w);
      };

      template<>
      struct line_converter<BPP::RGBA, BPP::RGB> {
        static void convert (cbyteptr in, byteptr out, int w);
      };

      template<BPP From>
      struct line_converter<From, BPP::BW> {
        static void convert (cbyteptr in, byteptr out, int w) {
          for (int x = 0; x < w; x += 8) {
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
      struct line_converter<BPP::BW, BPP::BW> {
        static void convert (cbyteptr in, byteptr out, int w) {
          for (int x = 0; x < w; x += 8) {
            out[x / 8] = in[x / 8];
          }
        }

      };
    }

    template<BPP From, BPP To>
    struct bpp_converter {
      typedef bpp::line_converter<From, To> line_converter;
      static void convert (const std::vector<char>& src, std::vector<char>& dst, int w, int h, int src_bpl, int dst_bpl) {
        for (int y = 0; y < h; ++y) {
          line_converter::convert(reinterpret_cast<cbyteptr>(src.data() + (y * src_bpl)),
                                  reinterpret_cast<byteptr>(dst.data() + (y * dst_bpl)), w);
        }
      }

    };

  }

}
