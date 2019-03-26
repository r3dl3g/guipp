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
 * @brief     C++ API: color definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include <gui++-draw-export.h>


namespace gui {

  // --------------------------------------------------------------------------
  enum class text_origin : unsigned short;

  // --------------------------------------------------------------------------
  enum class placement : unsigned short {
    left            = IF_WIN32_ELSE(DT_LEFT, 0x0000),
    hcenter         = IF_WIN32_ELSE(DT_CENTER, 0x0001),
    right           = IF_WIN32_ELSE(DT_RIGHT, 0x0002),

    top             = IF_WIN32_ELSE(DT_TOP, 0x0000),
    vcenter         = IF_WIN32_ELSE(DT_VCENTER, 0x0004),
    bottom          = IF_WIN32_ELSE(DT_BOTTOM, 0x0008)
  };

  // --------------------------------------------------------------------------
  inline constexpr unsigned short operator| (placement lhs, placement rhs) {
    return (static_cast<unsigned short>(lhs) | static_cast<unsigned short>(rhs));
  }

  inline constexpr unsigned short operator| (unsigned short lhs, placement rhs) {
    return (lhs | static_cast<unsigned short>(rhs));
  }

  inline constexpr bool operator== (text_origin lhs, placement rhs) {
    return (static_cast<unsigned short>(lhs) == static_cast<unsigned short>(rhs));
  }

  // --------------------------------------------------------------------------
  enum class line_handling : unsigned short {
    wordbreak       = IF_WIN32_ELSE(DT_WORDBREAK, 0x0010),
    singleline      = IF_WIN32_ELSE(DT_SINGLELINE, 0x0020)
  };

  inline constexpr unsigned short operator| (unsigned short lhs, line_handling rhs) {
    return (lhs | static_cast<unsigned int>(rhs));
  }

  // --------------------------------------------------------------------------
  enum class text_origin : unsigned short {
    top_left        = placement::top     | placement::left    | line_handling::wordbreak,
    top_hcenter     = placement::top     | placement::hcenter | line_handling::wordbreak,
    top_right       = placement::top     | placement::right   | line_handling::wordbreak,

    bottom_left     = placement::bottom  | placement::left    | line_handling::singleline,
    bottom_hcenter  = placement::bottom  | placement::hcenter | line_handling::singleline,
    bottom_right    = placement::bottom  | placement::right   | line_handling::singleline,

    vcenter_left    = placement::vcenter | placement::left    | line_handling::singleline,
    vcenter_right   = placement::vcenter | placement::right   | line_handling::singleline,
    center          = placement::vcenter | placement::hcenter | line_handling::singleline,

    h_align_mask    = placement::left    | placement::hcenter | placement::right,
    v_align_mask    = placement::top     | placement::vcenter | placement::bottom,

    undefined       = 0xFFFF,
  };

  // --------------------------------------------------------------------------
  inline constexpr text_origin operator& (text_origin lhs, text_origin rhs) {
    return text_origin(static_cast<unsigned int>(lhs) & static_cast<unsigned int>(rhs));
  }

  inline constexpr text_origin operator| (text_origin lhs, text_origin rhs) {
    return text_origin(static_cast<unsigned int>(lhs) | static_cast<unsigned int>(rhs));
  }

  // --------------------------------------------------------------------------
  inline constexpr bool origin_is_h_center (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::hcenter;
  }

  inline constexpr bool origin_is_left (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::left;
  }

  inline constexpr bool origin_is_right (text_origin origin) {
    return (origin & text_origin::h_align_mask) == placement::right;
  }

  inline constexpr bool origin_is_v_center (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::vcenter;
  }

  inline constexpr bool origin_is_top (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::top;
  }

  inline constexpr bool origin_is_bottom (text_origin origin) {
    return (origin & text_origin::v_align_mask) == placement::bottom;
  }

} // gui

