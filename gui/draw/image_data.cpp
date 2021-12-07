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
 * @brief     direct access image data
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    constexpr mono mono_colors<false>::value[];

    constexpr mono mono_colors<true>::value[];

    // --------------------------------------------------------------------------
    std::ostream& operator<< (std::ostream& out, const mono& px) {
      out << static_cast<int>(px);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const gray& px) {
      out << static_cast<int>(px.value);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const rgb& px) {
      out << "r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const rgba& px) {
      out << "r:" << static_cast<int>(px.red)
        << ", g:" << static_cast<int>(px.green)
        << ", b:" << static_cast<int>(px.blue)
        << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const bgr& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const bgra& px) {
      out << "b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red)
          << ", a:" << static_cast<int>(px.alpha);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const argb& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", r:" << static_cast<int>(px.red)
          << ", g:" << static_cast<int>(px.green)
          << ", b:" << static_cast<int>(px.blue);
      return out;
    }

    std::ostream& operator<< (std::ostream& out, const abgr& px) {
      out << "a:" << static_cast<int>(px.alpha)
          << ", b:" << static_cast<int>(px.blue)
          << ", g:" << static_cast<int>(px.green)
          << ", r:" << static_cast<int>(px.red);
      return out;
    }

    // --------------------------------------------------------------------------
    mono operator+ (const mono& lhs, const mono& rhs) {
      return mono(static_cast<byte>(lhs) | static_cast<byte>(rhs));
    }

    gray operator+ (const gray& lhs, const gray& rhs) {
      return gray{static_cast<gray::type>(lhs.value + rhs.value)};
    }

    // --------------------------------------------------------------------------
    constexpr mono color<mono>::black;
    constexpr mono color<mono>::white;

    constexpr gray color<gray>::black;
    constexpr gray color<gray>::white;

    constexpr rgb color<rgb>::black;
    constexpr rgb color<rgb>::white;

    constexpr rgba color<rgba>::black;
    constexpr rgba color<rgba>::white;

    constexpr bgr color<bgr>::black;
    constexpr bgr color<bgr>::white;

    constexpr bgra color<bgra>::black;
    constexpr bgra color<bgra>::white;

    constexpr argb color<argb>::black;
    constexpr argb color<argb>::white;

    constexpr abgr color<abgr>::black;
    constexpr abgr color<abgr>::white;

    // --------------------------------------------------------------------------
  } // namespace pixel

} // namespace gui
