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


    namespace bpp {

      template<>
      void set<BPP::RGB> (byteptr out, int x, byte v) {
        int p = x * 3;
        out[p + 2] = out[p + 1] = out[p] = v;
      }

      template<>
      void set<BPP::RGBA> (byteptr out, int x, byte v) {
        int p = x * 4;
        out[p + 2] = out[p + 1] = out[p] = v;
        out[p + 3] = 0;
      }

      template<>
      byte get<BPP::BW> (cbyteptr in, int x) {
#ifdef WIN32_OLD
        int shift = IF_WIN32(7 -) x % 8;
        int bit = (in[x / 8] & (0x01 << shift)) >> shift;
        return byte_values[bit];
#else
        const byte value = in[x / 8];
        const byte bit = x % 8;
        const byte bit_mask = system_bw_bits::mask[bit];
        const byte shift = system_bw_bits::shift[bit];
        const int b = (value & bit_mask) >> shift;
        return system_bw_bits::value[b];
#endif // X11
      }

      template<>
      byte get<BPP::RGB> (cbyteptr in, int x) {
        int spos = x * 3;
        int v0 = (int)in[spos];
        int v1 = (int)in[spos + 1];
        int v2 = (int)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      template<>
      byte get<BPP::RGBA> (cbyteptr in, int x) {
        int spos = x * 4;
        int v0 = (int)in[spos];
        int v1 = (int)in[spos + 1];
        int v2 = (int)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      void line_converter<BPP::RGB, BPP::RGBA>::convert (cbyteptr in, byteptr out, int w) {
        for (int x = 0; x < w; ++x) {
          reinterpret_cast<uint32_t*>(out)[x] = *reinterpret_cast<const uint32_t*>(in + x * 3) & 0x00ffffff;
        }
      }

      void line_converter<BPP::RGBA, BPP::RGB>::convert (cbyteptr in, byteptr out, int w) {
        for (int x = 0; x < w; ++x) {
          memcpy(out + x * 3, in + x * 4, 3);
        }
      }

    }
  }
}
