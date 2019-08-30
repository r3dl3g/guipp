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
 * @brief     C++ API: bitmap definition
 *
 * @file
 */

 // --------------------------------------------------------------------------
 //
 // Common includes
 //
 
 // --------------------------------------------------------------------------
//
// Library includes
//
#include "image_data.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace pixel {

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const bw_pixel& px) {
      out << static_cast<bool>(px);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const gray_pixel& px) {
      out << static_cast<int>(px.value);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const rgb_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const rgba_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
        << ", g:" << static_cast<int>(px.green)
        << ", b:" << static_cast<int>(px.blue)
        << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const bgr_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const bgra_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red)
          << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const argb_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const abgr_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    // --------------------------------------------------------------------------
    bw_pixel operator+ (const bw_pixel& lhs, const bw_pixel& rhs) {
      return bw_pixel(static_cast<bool>(lhs) | static_cast<bool>(rhs));
    }

    gray_pixel operator+ (const gray_pixel& lhs, const gray_pixel& rhs) {
      return gray_pixel{static_cast<gray_pixel::type>(lhs.value + rhs.value)};
    }

    // --------------------------------------------------------------------------
    constexpr bw_pixel color<bw_pixel>::black;
    constexpr bw_pixel color<bw_pixel>::white;

    constexpr gray_pixel color<gray_pixel>::black;
    constexpr gray_pixel color<gray_pixel>::white;

    constexpr rgb_pixel color<rgb_pixel>::black;
    constexpr rgb_pixel color<rgb_pixel>::white;

    constexpr rgba_pixel color<rgba_pixel>::black;
    constexpr rgba_pixel color<rgba_pixel>::white;

    constexpr bgr_pixel color<bgr_pixel>::black;
    constexpr bgr_pixel color<bgr_pixel>::white;

    constexpr bgra_pixel color<bgra_pixel>::black;
    constexpr bgra_pixel color<bgra_pixel>::white;

    constexpr argb_pixel color<argb_pixel>::black;
    constexpr argb_pixel color<argb_pixel>::white;

    constexpr abgr_pixel color<abgr_pixel>::black;
    constexpr abgr_pixel color<abgr_pixel>::white;

    // --------------------------------------------------------------------------
  } // namespace pixel

} // namespace gui
