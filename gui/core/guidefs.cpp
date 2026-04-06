/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     global gui defines and typedefs
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//

#include <logging/logger.h>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/guidefs.h"
#include "gui/core/context.h"
#include "gui/core/color.h"


namespace gui {

  // --------------------------------------------------------------------------
  pixel_format_t get_pixel_format (int pixel_format, core::byte_order_t byte_order_t) {
    switch (pixel_format) {
      case 1: return pixel_format_t::BW;
      case 8: return pixel_format_t::GRAY;
      case 24:
        switch (byte_order_t) {
          case core::byte_order_t::little_endian: return pixel_format_t::RGB;
          case core::byte_order_t::big_endian: return pixel_format_t::BGR;
        }
      case 32:
        switch (byte_order_t) {
          case core::byte_order_t::little_endian: return IF_QT_ELSE(pixel_format_t::RGBA, pixel_format_t::BGRA);
          case core::byte_order_t::big_endian: return IF_QT_ELSE(pixel_format_t::ABGR, pixel_format_t::ARGB);
        }
    }
    return pixel_format_t::Undefined;
  }

  // --------------------------------------------------------------------------
  core::byte_order_t get_pixel_format_byte_order (pixel_format_t px_fmt) {
    switch (px_fmt) {
      case pixel_format_t::RGB:
      case pixel_format_t::RGBA:
      case pixel_format_t::ARGB:
      default:
        return core::byte_order_t::big_endian;
      case pixel_format_t::BW:
      case pixel_format_t::GRAY:
      case pixel_format_t::BGR:
      case pixel_format_t::BGRA:
      case pixel_format_t::ABGR:
      return core::byte_order_t::little_endian;
    }
  }

  // --------------------------------------------------------------------------
  namespace core {

    // --------------------------------------------------------------------------
    namespace global {

      namespace {
        double scale_factor = 1.0;
      }

      void info (gui::os::instance i) {
        logging::debug() << "os::bitmap_bit_order_t: " << core::os::bitmap_bit_order;
        logging::debug() << "os::bitmap_byte_order_t: " << core::os::bitmap_byte_order;
        logging::debug() << "os::platform_t: " << core::os::system_platform;
        logging::debug() << "os::ui_t: " << core::os::system_ui;
        logging::debug() << "global::scale_factor: " << scale_factor;
        logging::debug() << "color::part: RGBA=" << static_cast<unsigned int>(color::part::red)
                                          << ":" << static_cast<unsigned int>(color::part::green)
                                          << ":" << static_cast<unsigned int>(color::part::blue)
                                          << ":" << static_cast<unsigned int>(color::part::alpha);
      }

      void set_scale_factor (double s) {
        scale_factor = s;
      }

      double get_scale_factor () {
        return scale_factor;
      }

    } // global

  } // core

} // gui

namespace std {

  namespace {

    const char* pixel_format_t_names[] = {
      "Undefined",
      "BW",
      "GRAY",
      "RGB",
      "BGR",
      "RGBA",
      "BGRA",
      "ARGB",
      "ABGR"
    };

    const char* orientation_t_names[] = {
      "vertical",
      "horizontal"
    };

    const char* origin_t_names[] = {
      "start",
      "center",
      "end"
    };

    const char* alignment_t_names[] = {
      "left",
      "hcenter",
      "right",
      "top",
      "vcenter",
      "bottom"
    };

  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::pixel_format_t pf) {
    return print_enum(out, pf, pixel_format_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::orientation_t o) {
    return print_enum(out, o, orientation_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::origin_t o) {
    return print_enum(out, o, origin_t_names);
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out,gui::alignment_t a) {
    return print_enum(out, a, alignment_t_names);
  }

} // namespace std
