/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: bitmap converter
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "converter.h"


namespace gui {

  namespace convert {

    const byte byte_values[] = { 0, 0xff };

    template<>
    void set<24> (byteptr out, int x, byte v) {
      int p = x * 3;
      out[p + 2] = out[p + 1] = out[p] = v;
    }

    template<>
    void set<32> (byteptr out, int x, byte v) {
      int p = x * 4;
      out[p + 2] = out[p + 1] = out[p] = v;
      out[p + 3] = 0;
    }

    template<>
    byte get<1> (cbyteptr in, int x) {
      int shift = 7 - x % 8;
      int bit = (in[x / 8] & (0x01 << shift)) >> shift;
      return byte_values[bit];
    }

    template<>
    byte get<24> (cbyteptr in, int x) {
      int spos = x * 3;
      int v0 = (int)in[spos];
      int v1 = (int)in[spos + 1];
      int v2 = (int)in[spos + 2];
      return (v0 + v1 + v2) / 3;
    }

    template<>
    byte get<32> (cbyteptr in, int x) {
      int spos = x * 4;
      int v0 = (int)in[spos];
      int v1 = (int)in[spos + 1];
      int v2 = (int)in[spos + 2];
      return (v0 + v1 + v2) / 3;
    }

    void line_converter<24, 32>::convert (cbyteptr in, byteptr out, int w) {
      for (int x = 0; x < w; ++x) {
#ifdef WIN32
        out[x * 4] = in[x * 3 + 2];
        out[x * 4 + 1] = in[x * 3 + 1];
        out[x * 4 + 2] = in[x * 3];
#endif // WIN32
#ifdef X11
        reinterpret_cast<uint32_t*>(out)[x] = *reinterpret_cast<const uint32_t*>(in + x * 3) & 0x00ffffff;
#endif // X11
      }
    }

    void line_converter<32, 24>::convert (cbyteptr in, byteptr out, int w) {
      for (int x = 0; x < w; ++x) {
#ifdef WIN32
        out[x * 3] = in[x * 4 + 2];
        out[x * 3 + 1] = in[x * 4 + 1];
        out[x * 3 + 2] = in[x * 4];
#endif // WIN32
#ifdef X11
        memcpy(out + x * 3, in + x * 4, 3);
#endif // X11
      }
    }

  }
}
