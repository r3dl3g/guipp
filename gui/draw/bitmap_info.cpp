/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <map>

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

#ifdef GUIPP_X11

    uint32_t bitmap_calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt) {
      switch (px_fmt) {
      case pixel_format_t::BW:
        return up_modulo<8, 4>(w);
      case pixel_format_t::GRAY:
        return up_modulo<1, 4>(w);
      case pixel_format_t::RGB:
      case pixel_format_t::BGR:
        return up_modulo<1, 4>(w * 3);
      case pixel_format_t::RGBA:
      case pixel_format_t::ARGB:
      case pixel_format_t::BGRA:
      case pixel_format_t::ABGR:
        return up_modulo<1, 4>(w * 4);
      default:
        break;
      }
      return -1;
    }

#endif // !GUIPP_X11

#if GUIPP_WIN

    uint32_t bitmap_calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt) {
      switch (px_fmt) {
      case pixel_format_t::BW:
        return up_modulo<8, 2>(w);
      case pixel_format_t::GRAY:
        return up_modulo<1, 2>(w);
      case pixel_format_t::RGB:
      case pixel_format_t::BGR:
        return up_modulo<1, 2>(w * 3);
      case pixel_format_t::RGBA:
      case pixel_format_t::ARGB:
      case pixel_format_t::BGRA:
      case pixel_format_t::ABGR:
        return up_modulo<1, 2>(w * 4);
      default:
        break;
      }
      return ((((w * color_depths[static_cast<byte>(px_fmt)]) + 31) & ~31) >> 3);
    }

#endif // GUIPP_WIN

#ifdef GUIPP_QT

    uint32_t bitmap_calc_bytes_per_line (uint32_t w, pixel_format_t fmt) {
      return QImage(w, 1, bitmap_info::convert(fmt)).bytesPerLine();
    }

    pixel_format_t bitmap_info::convert (QImage::Format f) {
      switch (f) {
        case QImage::Format_Mono:
        case QImage::Format_MonoLSB:
          return pixel_format_t::BW;
        case QImage::Format_Grayscale8: return pixel_format_t::GRAY;
        case QImage::Format_RGB888: return pixel_format_t::RGB;
        case QImage::Format_RGBA8888: return pixel_format_t::RGBA;
        case QImage::Format_ARGB32: return pixel_format_t::ARGB;
        default:
          return pixel_format_t::RGBA;
      }
    }

     QImage::Format bitmap_info::convert (pixel_format_t f) {
      switch (f) {
        case pixel_format_t::BW:
          return (core::os::bitmap_bit_order == core::bit_order_t::msb_first ? QImage::Format_Mono
                                                                             : QImage::Format_MonoLSB);
        case pixel_format_t::GRAY: return QImage::Format_Grayscale8;
        case pixel_format_t::RGB: return QImage::Format_RGB888;
        case pixel_format_t::ARGB: return QImage::Format_ARGB32;
        default:
        case pixel_format_t::RGBA: return QImage::Format_RGBA8888;
      }
    }

#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    bitmap_info::bitmap_info ()
      : width(0)
      , height(0)
      , pixel_format(pixel_format_t::Undefined)
      , bytes_per_line(0)
    {}

    bitmap_info::bitmap_info (const core::native_size& sz, pixel_format_t px_fmt)
      : width(sz.width())
      , height(sz.height())
      , pixel_format(px_fmt)
      , bytes_per_line(bitmap_calc_bytes_per_line(sz.width(), px_fmt))
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, pixel_format_t px_fmt)
      : width(w)
      , height(h)
      , pixel_format(px_fmt)
      , bytes_per_line(bpl)
    {}

    bitmap_info::bitmap_info (uint32_t w, uint32_t h, pixel_format_t px_fmt)
      : width(w)
      , height(h)
      , pixel_format(px_fmt)
      , bytes_per_line(bitmap_calc_bytes_per_line(w, px_fmt))
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
