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

    // --------------------------------------------------------------------------
    const gray_pixel gray_pixel::black = { 0 };
    const gray_pixel gray_pixel::white = { 0xff };

    bool gray_pixel::operator== (const gray_pixel& rhs) const {
      return value == rhs.value;
    }

    std::ostream& operator<< (std::ostream& out, const gray_pixel& px) {
      out << static_cast<int>(px.value);
      return out;
    }

    // --------------------------------------------------------------------------
    const rgb_pixel rgb_pixel::black = { 0, 0, 0 };
    const rgb_pixel rgb_pixel::white = { 0xff, 0xff, 0xff };

    bool rgb_pixel::operator== (const rgb_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue);
    }

    std::ostream& operator<< (std::ostream& out, const rgb_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    // --------------------------------------------------------------------------
    const bgr_pixel bgr_pixel::black = { 0, 0, 0 };
    const bgr_pixel bgr_pixel::white = { 0xff, 0xff, 0xff };

    bool bgr_pixel::operator== (const bgr_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue);
    }

    std::ostream& operator<< (std::ostream& out, const bgr_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    // --------------------------------------------------------------------------
    const rgba_pixel rgba_pixel::black = { 0, 0, 0, 0xff };
    const rgba_pixel rgba_pixel::white = { 0xff, 0xff, 0xff, 0xff };

    bool rgba_pixel::operator== (const rgba_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue) && (alpha == rhs.alpha);
    }

    std::ostream& operator<< (std::ostream& out, const rgba_pixel& px) {
      out << "r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue)
          << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    // --------------------------------------------------------------------------
    const bgra_pixel bgra_pixel::black = { 0, 0, 0, 0xff };
    const bgra_pixel bgra_pixel::white = { 0xff, 0xff, 0xff, 0xff };

    bool bgra_pixel::operator== (const bgra_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue) && (alpha == rhs.alpha);
    }

    std::ostream& operator<< (std::ostream& out, const bgra_pixel& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red)
          << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    // --------------------------------------------------------------------------
    const argb_pixel argb_pixel::black = { 0xff, 0, 0, 0 };
    const argb_pixel argb_pixel::white = { 0xff, 0xff, 0xff, 0xff };

    bool argb_pixel::operator== (const argb_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue) && (alpha == rhs.alpha);
    }

    std::ostream& operator<< (std::ostream& out, const argb_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    // --------------------------------------------------------------------------
    const abgr_pixel abgr_pixel::black = { 0xff, 0, 0, 0 };
    const abgr_pixel abgr_pixel::white = { 0xff, 0xff, 0xff, 0xff };

    bool abgr_pixel::operator== (const abgr_pixel& rhs) const {
      return (red == rhs.red) && (green == rhs.green) && (blue == rhs.blue) && (alpha == rhs.alpha);
    }

    std::ostream& operator<< (std::ostream& out, const abgr_pixel& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

  } // namespace pixel

} // namespace gui
