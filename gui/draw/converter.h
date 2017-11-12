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
#include <gui/core/guidefs.h>
#include <gui/core/array_wrapper.h>


namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    typedef core::array_wrapper<byte> bytearray;
    typedef core::array_wrapper<const byte> cbytearray;

    // --------------------------------------------------------------------------
    namespace bpp {

      template<BPP W>
      void set (bytearray out, uint32_t x, byte v);

      template<BPP W>
      byte get (cbytearray in, uint32_t x);

      template<BPP From, BPP To>
      struct line {
        static void convert (cbytearray in, bytearray out, uint32_t w);
      };

      template<BPP From, BPP To>
      void convert (cbytearray src, bytearray dst, uint32_t w, uint32_t h, uint32_t src_bpl, uint32_t dst_bpl);

    } // namespace bpp

    // --------------------------------------------------------------------------
    namespace copy {

      template<BPP bpp>
      void row (cbytearray src, bytearray dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w);

      template<BPP bpp>
      void sub (cbytearray src_data, uint32_t src_bpl,
                bytearray dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0,
                uint32_t dest_x0, uint32_t dest_y0,
                uint32_t dest_w, uint32_t dest_h);


    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (cbytearray src_data, bytearray dest_data,
                uint32_t src_x0, uint32_t dest_x0,
                uint32_t src_w, uint32_t dest_w);

      template<BPP bpp>
      void sub (cbytearray src_data, uint32_t src_bpl,
                bytearray dest_data, uint32_t dest_bpl,
                uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h);

    } // namespace stretch

    // --------------------------------------------------------------------------
    namespace brightness {

      template<BPP bpp>
      void row (bytearray data, uint32_t w, float f);

      template<BPP bpp>
      void adjust (bytearray data, uint32_t w, uint32_t h, uint32_t bpl, float f);

    }

    // --------------------------------------------------------------------------
  } // namespace convert

} // namespace gui

#include <gui/draw/converter.inl>
