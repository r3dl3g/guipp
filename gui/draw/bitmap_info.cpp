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
 * @brief     C++ API: bitmap
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <math.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/bitmap_info.h>


namespace gui {

  namespace draw {

    namespace {

      template<uint32_t D, uint32_t M>
      uint32_t up_modulo (uint32_t v) {
        uint32_t r = (v + D - 1) / D;
        return r + (r % M ? M - r % M : 0);
      }

#ifdef X11

      uint32_t bitmap_calc_bytes_per_line (uint32_t w, BPP bpp) {
        switch (bpp) {
        case BPP::BW:
          return up_modulo<8, 4>(w);
        case BPP::GRAY:
          return up_modulo<1, 4>(w);
        case BPP::RGB:
          return up_modulo<1, 4>(w * 3);
        case BPP::RGBA:
          return up_modulo<1, 4>(w * 4);
        case BPP::Undefined:
          break;
        }
        return -1;
      }

#endif // !X11

#if WIN32

      uint32_t bitmap_calc_bytes_per_line (uint32_t w, BPP bpp) {
        switch (bpp) {
        case BPP::BW:
          return up_modulo<8, 2>(w);
        case BPP::GRAY:
          return up_modulo<1, 2>(w);
        case BPP::RGB:
          return up_modulo<1, 2>(w * 3);
        case BPP::RGBA:
          return up_modulo<1, 2>(w * 4);
        case BPP::Undefined:
          break;
        }
        return ((((w * static_cast<byte>(bpp)) + 31) & ~31) >> 3);
      }

#endif // WIN32

    }

    // --------------------------------------------------------------------------
    bitmap_info::bitmap_info ()
      : width(0)
      , height(0)
      , bytes_per_line(0)
      , bits_per_pixel(BPP::Undefined)
    {}

    bitmap_info::bitmap_info (const core::uint32_size& sz, BPP bpp)
      : width(sz.width())
      , height(sz.height())
      , bytes_per_line(bitmap_calc_bytes_per_line(sz.width(), bpp))
      , bits_per_pixel(bpp)
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, BPP bpp)
      : width(w)
      , height(h)
      , bytes_per_line(bpl)
      , bits_per_pixel(bpp)
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, BPP bpp)
      : width(w)
      , height(h)
      , bytes_per_line(bitmap_calc_bytes_per_line(w, bpp))
      , bits_per_pixel(bpp)
    {}

    bool bitmap_info::operator== (const bitmap_info& rhs) const {
      return (width == rhs.width) && (height == rhs.height) && (bits_per_pixel == rhs.bits_per_pixel);
    }

    // --------------------------------------------------------------------------
  } // namespace draw

} // namespace gui
