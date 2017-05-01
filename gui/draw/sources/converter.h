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
    const byte bit_mask[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

    template<int W>
    void set (byteptr out, int x, byte v);

    template<>
    inline void set<8> (byteptr out, int x, byte v) {
      out[x] = v;
    }

    template<>
    void set<24> (byteptr out, int x, byte v);

    template<>
    void set<32> (byteptr out, int x, byte v);

    template<int W>
    byte get (cbyteptr in, int x);

    template<>
    byte get<1> (cbyteptr in, int x);

    template<>
    inline byte get<8> (cbyteptr in, int x) {
      return in[x];
    }

    template<>
    byte get<24> (cbyteptr in, int x);

    template<>
    byte get<32> (cbyteptr in, int x);

    template<int From, int To>
    struct line_converter {
      static void convert (cbyteptr in, byteptr out, int w) {
        for (int x = 0; x < w; ++x) {
          set<To>(out, x, get<From>(in, x));
        }
      }
    };

    template<int From>
    struct line_converter<From, 1> {
      static void convert (cbyteptr in, byteptr out, int w) {
        for (int x = 0; x < w; x += 8) {
//          byte ovalue = 0;
//          for (int s = 0; s < 8; ++s) {
//            ovalue = ovalue | ((get<From>(in, x + s) > 0x7f) ? 0x01 << s : 0);
//          }
          byte ovalue = (get<From>(in, x + 0) ? 0x01 : 0)
                      | (get<From>(in, x + 1) ? 0x02 : 0)
                      | (get<From>(in, x + 2) ? 0x04 : 0)
                      | (get<From>(in, x + 3) ? 0x08 : 0)
                      | (get<From>(in, x + 4) ? 0x10 : 0)
                      | (get<From>(in, x + 5) ? 0x20 : 0)
                      | (get<From>(in, x + 6) ? 0x40 : 0)
                      | (get<From>(in, x + 7) ? 0x80 : 0);
          out[x / 8] = ovalue;
        }
      }
    };

    template<>
    struct line_converter<24, 32> {
      static void convert (cbyteptr in, byteptr out, int w);
    };

    template<>
    struct line_converter<32, 24> {
      static void convert (cbyteptr in, byteptr out, int w);
    };

    template<int From, int To>
    struct converter {
      static void convert (const std::vector<char>& src, std::vector<char>& dst, int w, int h, int src_bpl, int dst_bpl) {
        for (int y = 0; y < h; ++y) {
          line_converter<From, To>::convert(reinterpret_cast<cbyteptr>(src.data() + (y * src_bpl)),
                                            reinterpret_cast<byteptr>(dst.data() + (y * dst_bpl)), w);
        }
      }
    };

  }

}
