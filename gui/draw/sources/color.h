/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
// Common includes
//
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace draw {

    struct /*immutable*/ color {

      typedef unsigned char byte;

      const static color black;
      const static color white;
      const static color red;
      const static color green;
      const static color blue;
      const static color cyan;
      const static color magenta;
      const static color yellow;
      const static color orange;
      const static color brown;
      const static color darkBlue;
      const static color darkGreen;
      const static color darkRed;
      const static color darkCyan;
      const static color darkMagenta;
      const static color darkYellow;
      const static color darkOrange;
      const static color darkBrown;
      const static color gray;
      const static color lightGray;
      const static color lightRed;
      const static color lightGreen;
      const static color lightBlue;
      const static color lightCyan;
      const static color lightMagenta;
      const static color lightYellow;
      const static color veryLightGray;
      const static color veryLightRed;
      const static color veryLightGreen;
      const static color veryLightBlue;
      const static color veryLightCyan;
      const static color veryLightMagenta;
      const static color veryLightYellow;
      const static color darkGray;

      const static color scrollBarColor;
      const static color menuColor;
      const static color menuTextColor;
      const static color backGroundColor;
      const static color activeCaptionColor;
      const static color inActiveCaptionColor;
      const static color windowColor;
      const static color windowFrameColor;
      const static color windowTextColor;
      const static color captionTextColor;
      const static color inActiveCaptionTextColor;
      const static color activeBorderColor;
      const static color inActiveBorderColor;
      const static color workSpaceColor;
      const static color highLightColor;
      const static color highLightTextColor;
      const static color disabledTextColor;
      const static color buttonColor;
      const static color shadowColor;
      const static color buttonTextColor;
      const static color buttonHighLightColor;

      static core::color_type value_of (byte r, byte g, byte b) {
        return r_::c(r) | g_::c(g) | b_::c(b);
      }

      inline color(core::color_type value = 0)
        : value(value)
      {}

      inline color(byte r, byte g, byte b)
        : value(value_of(r, g, b))
      {}

      inline color(byte r, byte g, byte b, byte alpha)
        : value(r_::c(r) | g_::c(g) | b_::c(b) | a_::c(alpha))
      {}

      inline byte r() const {
        return r_::v(value);
      }

      inline byte g() const {
        return g_::v(value);
      }

      inline byte b() const {
        return b_::v(value);
      }

      inline byte a() const {
        return a_::v(value);
      }

      inline operator core::color_type() const {
        return value;
      }

      inline color medium_gray() const {
        const byte v = (byte)(((int)r() + (int)g() + (int)b()) / 3);
        return{ v, v, v, a() };
      }

      inline color transparency(double faktor) const {
        return color(r(), g(), b(), (byte)(faktor * 255));
      }

      inline color darker(double faktor = 1.5) const {
        return color((byte)((double)r() / faktor),
                     (byte)((double)g() / faktor),
                     (byte)((double)b() / faktor),
                     a());
      }

      inline color lighter(double faktor = 1.3333) const {
        return color((byte)((double)r() * faktor),
                     (byte)((double)g() * faktor),
                     (byte)((double)b() * faktor),
                     a());
      }

      inline color merge(const color &rhs, double rfaktor = 0.5) const {
        double lfaktor = 1.0 - rfaktor;
        return color((byte)((double)rhs.r() * rfaktor + (double)r() * lfaktor),
                     (byte)((double)rhs.g() * rfaktor + (double)g() * lfaktor),
                     (byte)((double)rhs.b() * rfaktor + (double)b() * lfaktor),
                     (byte)((double)rhs.a() * rfaktor + (double)a() * lfaktor));
      }

      inline color invert() const {
        return color(255 - r(), 255 - g(), 255 - b(), a());
      }

    private:
      const core::color_type value;

      template<int shift>
      struct part {
        static core::color_type c(byte a) {
          return (core::color_type)a << shift;
        }

        static byte v(core::color_type a) {
          return (byte)((a & (0x000000ff << shift)) >> shift);
        }
      };

      typedef part<IF_WIN32(0) IF_X11(16)> r_;
      typedef part<8> g_;
      typedef part<IF_WIN32(16) IF_X11(0)> b_;
      typedef part<24> a_;

    };

    std::ostream& operator<<(std::ostream& out, const color& c);

  }

}
