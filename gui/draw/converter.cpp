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
#include <gui/draw/converter.h>


namespace gui {

  namespace convert {

    namespace bpp {

      template<>
      void set<BPP::BW>(byteptr out, uint32_t x, byte v) {
        const uint32_t offset = x / 8;
        const byte value = out[offset];
        const byte bit = x % 8;
        const byte bit_mask = system_bw_bits::mask[bit];
        if (v) {
          out[offset] = value | bit_mask;
        } else {
          out[offset] = value & ~bit_mask;
        }
      }

      template<>
      void set<BPP::RGB>(byteptr out, uint32_t x, byte v) {
        uint32_t p = x * 3;
        out[p + 2] = out[p + 1] = out[p] = v;
      }

      template<>
      void set<BPP::RGBA>(byteptr out, uint32_t x, byte v) {
        uint32_t p = x * 4;
        out[p + 2] = out[p + 1] = out[p] = v;
        out[p + 3] = 0;
      }

      template<>
      byte get<BPP::BW>(cbyteptr in, uint32_t x) {
        const byte value = in[x / 8];
        const byte bit = x % 8;
        const byte bit_mask = system_bw_bits::mask[bit];
        const byte shift = system_bw_bits::shift[bit];
        const uint32_t b = (value & bit_mask) >> shift;
        return system_bw_bits::value[b];
      }

      template<>
      byte get<BPP::RGB>(cbyteptr in, uint32_t x) {
        uint32_t spos = x * 3;
        uint32_t v0 = (uint32_t)in[spos];
        uint32_t v1 = (uint32_t)in[spos + 1];
        uint32_t v2 = (uint32_t)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      template<>
      byte get<BPP::RGBA>(cbyteptr in, uint32_t x) {
        uint32_t spos = x * 4;
        uint32_t v0 = (uint32_t)in[spos];
        uint32_t v1 = (uint32_t)in[spos + 1];
        uint32_t v2 = (uint32_t)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      void line_converter<BPP::RGB, BPP::RGBA>::convert (cbyteptr in, byteptr out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          reinterpret_cast<uint32_t*>(out)[x] = *reinterpret_cast<const uint32_t*>(in + x * 3) & 0x00ffffff;
        }
      }

      void line_converter<BPP::RGBA, BPP::RGB>::convert (cbyteptr in, byteptr out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          memcpy(out + x * 3, in + x * 4, 3);
        }
      }

    }
  }
}
