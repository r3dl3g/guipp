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
#include "graphics.h"


namespace gui {

  namespace convert {

    typedef uint8_t byte;
    typedef byte* byteptr;
    typedef const byte* cbyteptr;
    constexpr byte bit_mask[8] = IF_X11({ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 })
                                 IF_WIN32({ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 });

    template<BPP W>
    void set (byteptr out, int x, byte v);

    template<>
    inline void set<BPP::GRAY> (byteptr out, int x, byte v) {
      out[x] = v;
    }

    template<>
    void set<BPP::RGB> (byteptr out, int x, byte v);

    template<>
    void set<BPP::RGBA> (byteptr out, int x, byte v);

    template<BPP W>
    byte get (cbyteptr in, int x);

    template<>
    byte get<BPP::BW> (cbyteptr in, int x);

    template<>
    inline byte get<BPP::GRAY> (cbyteptr in, int x) {
      return in[x];
    }

    template<>
    byte get<BPP::RGB> (cbyteptr in, int x);

    template<>
    byte get<BPP::RGBA> (cbyteptr in, int x);

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
          byte ovalue = (get<From>(in, x + 0) ? bit_mask[0] : 0)
                      | (get<From>(in, x + 1) ? bit_mask[1] : 0)
                      | (get<From>(in, x + 2) ? bit_mask[2] : 0)
                      | (get<From>(in, x + 3) ? bit_mask[3] : 0)
                      | (get<From>(in, x + 4) ? bit_mask[4] : 0)
                      | (get<From>(in, x + 5) ? bit_mask[5] : 0)
                      | (get<From>(in, x + 6) ? bit_mask[6] : 0)
                      | (get<From>(in, x + 7) ? bit_mask[7] : 0);
          out[x / 8] = ovalue IF_WIN32(^ 0xff);
        }
      }
    };

    template<BPP From, BPP To>
    struct converter {
      typedef line_converter<From, To> my_line_converter;
      static void convert (const std::vector<char>& src, std::vector<char>& dst, int w, int h, int src_bpl, int dst_bpl) {
        for (int y = 0; y < h; ++y) {
          my_line_converter::convert(reinterpret_cast<cbyteptr>(src.data() + (y * src_bpl)),
                                     reinterpret_cast<byteptr>(dst.data() + (y * dst_bpl)), w);
        }
      }
    };

  }

}
