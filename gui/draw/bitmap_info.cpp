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
    }

#ifdef X11

      uint32_t bitmap_calc_bytes_per_line (uint32_t w, PixelFormat px_fmt) {
        switch (px_fmt) {
        case PixelFormat::BW:
          return up_modulo<8, 4>(w);
        case PixelFormat::GRAY:
          return up_modulo<1, 4>(w);
        case PixelFormat::RGB:
        case PixelFormat::BGR:
          return up_modulo<1, 4>(w * 3);
        case PixelFormat::RGBA:
        case PixelFormat::ARGB:
        case PixelFormat::BGRA:
        case PixelFormat::ABGR:
          return up_modulo<1, 4>(w * 4);
        default:
          break;
        }
        return -1;
      }

#endif // !X11

#if WIN32

      uint32_t bitmap_calc_bytes_per_line (uint32_t w, PixelFormat px_fmt) {
        switch (px_fmt) {
        case PixelFormat::BW:
          return up_modulo<8, 2>(w);
        case PixelFormat::GRAY:
          return up_modulo<1, 2>(w);
        case PixelFormat::RGB:
        case PixelFormat::BGR:
          return up_modulo<1, 2>(w * 3);
        case PixelFormat::RGBA:
        case PixelFormat::ARGB:
        case PixelFormat::BGRA:
        case PixelFormat::ABGR:
          return up_modulo<1, 2>(w * 4);
        default:
          break;
        }
        return ((((w * color_depths[static_cast<byte>(px_fmt)]) + 31) & ~31) >> 3);
      }

#endif // WIN32

    // --------------------------------------------------------------------------
    bitmap_info::bitmap_info ()
      : width(0)
      , height(0)
      , bytes_per_line(0)
      , pixel_format(PixelFormat::Undefined)
    {}

    bitmap_info::bitmap_info (const core::native_size& sz, PixelFormat px_fmt)
      : width(sz.width())
      , height(sz.height())
      , bytes_per_line(bitmap_calc_bytes_per_line(sz.width(), px_fmt))
      , pixel_format(px_fmt)
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, PixelFormat px_fmt)
      : width(w)
      , height(h)
      , bytes_per_line(bpl)
      , pixel_format(px_fmt)
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, PixelFormat px_fmt)
      : width(w)
      , height(h)
      , bytes_per_line(bitmap_calc_bytes_per_line(w, px_fmt))
      , pixel_format(px_fmt)
    {}

    bool bitmap_info::operator== (const bitmap_info& rhs) const {
      return (width == rhs.width) && (height == rhs.height) && (pixel_format == rhs.pixel_format) && (bytes_per_line == rhs.bytes_per_line);
    }

    std::ostream& operator<< (std::ostream& out, const bitmap_info& bmi) {
      out << "bitmap_info{" << bmi.width << ", " << bmi.height << ", " << bmi.bytes_per_line << ", " << bmi.pixel_format << "}";
      return out;
    }

    // --------------------------------------------------------------------------
  } // namespace draw

} // namespace gui
