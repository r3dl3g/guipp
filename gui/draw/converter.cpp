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
#include <algorithm>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/converter.h>


namespace gui {

  namespace convert {

    namespace bpp {

      template<>
      void set<BPP::BW>(bytearray out, uint32_t x, byte v) {
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
      void set<BPP::RGB>(bytearray out, uint32_t x, byte v) {
        uint32_t p = x * 3;
        out[p + 2] = out[p + 1] = out[p] = v;
      }

      template<>
      void set<BPP::RGBA>(bytearray out, uint32_t x, byte v) {
        uint32_t p = x * 4;
        out[p + 2] = out[p + 1] = out[p] = v;
        out[p + 3] = 0;
      }

      template<>
      byte get<BPP::BW>(cbytearray in, uint32_t x) {
        const byte value = in[x / 8];
        const byte bit = x % 8;
        const byte bit_mask = system_bw_bits::mask[bit];
        const byte shift = system_bw_bits::shift[bit];
        const uint32_t b = (value & bit_mask) >> shift;
        return system_bw_bits::value[b];
      }

      template<>
      byte get<BPP::RGB>(cbytearray in, uint32_t x) {
        uint32_t spos = x * 3;
        uint32_t v0 = (uint32_t)in[spos];
        uint32_t v1 = (uint32_t)in[spos + 1];
        uint32_t v2 = (uint32_t)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      template<>
      byte get<BPP::RGBA>(cbytearray in, uint32_t x) {
        uint32_t spos = x * 4;
        uint32_t v0 = (uint32_t)in[spos];
        uint32_t v1 = (uint32_t)in[spos + 1];
        uint32_t v2 = (uint32_t)in[spos + 2];
        return (v0 + v1 + v2) / 3;
      }

      void line<BPP::RGB, BPP::RGBA>::convert (cbytearray in, bytearray out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          reinterpret_cast<uint32_t*>(out.data())[x] = *reinterpret_cast<const uint32_t*>(in + x * 3) & 0x00ffffff;
        }
      }

      void line<BPP::RGBA, BPP::RGB>::convert (cbytearray in, bytearray out, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          memcpy(out + x * 3, in + x * 4, 3);
        }
      }

    } // namespace bpp

    // --------------------------------------------------------------------------
    namespace stretch {

      template<>
      void row<BPP::BW> (cbytearray src_data, bytearray dest_data,
                         uint32_t src_x0, uint32_t dest_x0,
                         uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = x * src_w / dest_w;
          byte b = convert::bpp::get<BPP::BW>(src_data, src_x0 + src_x);
          convert::bpp::set<BPP::BW>(dest_data, dest_x0 + x, b);
        }
      }

      template<>
      void row<BPP::GRAY> (cbytearray src_data, bytearray dest_data,
                           uint32_t src_x0, uint32_t dest_x0,
                           uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = x * src_w / dest_w;
          dest_data[dest_x0 + x] = src_data[src_x0 + src_x];
        }
      }

      template<>
      void row<BPP::RGB> (cbytearray src_data, bytearray dest_data,
                          uint32_t src_x0, uint32_t dest_x0
                          , uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = (src_x0 + x * src_w / dest_w) * 3;
          const uint32_t dest_x = dest_x0 + x * 3;
          dest_data[dest_x] = src_data[src_x];
          dest_data[dest_x + 1] = src_data[src_x + 1];
          dest_data[dest_x + 2] = src_data[src_x + 2];
        }
      }

      template<>
      void row<BPP::RGBA> (cbytearray src_data, bytearray dest_data,
                           uint32_t src_x0, uint32_t dest_x0,
                           uint32_t src_w, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = (src_x0 + x * src_w / dest_w) * 4;
          const uint32_t dest_x = dest_x0 + x * 4;
          dest_data[dest_x] = src_data[src_x];
          dest_data[dest_x + 1] = src_data[src_x + 1];
          dest_data[dest_x + 2] = src_data[src_x + 2];
          dest_data[dest_x + 3] = src_data[src_x + 3];
        }
      }

    } // namespace stretch

    namespace brightness {

      template<>
      void row<BPP::BW> (bytearray, uint32_t, float) {
      }

      template<>
      void row<BPP::GRAY> (bytearray data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = static_cast<byte>(std::min<int>(0xff, static_cast<int>(data[x] * f)));
        }
      }

      template<>
      void row<BPP::RGB> (bytearray data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          const uint32_t offs = x * 3;
          data[offs] = static_cast<byte>(data[offs] * f);
          data[offs + 1] = static_cast<byte>(data[offs + 1] * f);
          data[offs + 2] = static_cast<byte>(data[offs + 2] * f);
        }
      }

      template<>
      void row<BPP::RGBA> (bytearray data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          const uint32_t offs = x * 4;
          data[offs] = static_cast<byte>(data[offs] * f);
          data[offs + 1] = static_cast<byte>(data[offs + 1] * f);
          data[offs + 2] = static_cast<byte>(data[offs + 2] * f);
        }
      }

    } // namespace brightness

  } // namespace convert

} // namespace gui
