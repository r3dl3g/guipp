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
      typedef os::color value_type;

      static color black ();
      static color white ();
      static color red ();
      static color green ();
      static color blue ();
      static color cyan ();
      static color magenta ();
      static color yellow ();
      static color orange ();
      static color brown ();
      static color darkGray ();
      static color darkBlue ();
      static color darkGreen ();
      static color darkRed ();
      static color darkCyan ();
      static color darkMagenta ();
      static color darkYellow ();
      static color darkOrange ();
      static color darkBrown ();
      static color gray ();
      static color mediumGray ();
      static color lightGray ();
      static color lightRed ();
      static color lightGreen ();
      static color lightBlue ();
      static color lightCyan ();
      static color lightMagenta ();
      static color lightYellow ();
      static color veryLightGray ();
      static color veryLightRed ();
      static color veryLightGreen ();
      static color veryLightBlue ();
      static color veryLightCyan ();
      static color veryLightMagenta ();
      static color veryLightYellow ();
      static color veryVeryLightGray ();

      static color scrollBarColor();
      static color menuColor();
      static color menuTextColor();
      static color backGroundColor();
      static color activeCaptionColor();
      static color inActiveCaptionColor();
      static color windowColor();
      static color windowFrameColor();
      static color windowTextColor();
      static color captionTextColor();
      static color inActiveCaptionTextColor();
      static color activeBorderColor();
      static color inActiveBorderColor();
      static color workSpaceColor();
      static color highLightColor();
      static color highLightTextColor();
      static color disabledTextColor();
      static color buttonColor();
      static color shadowColor();
      static color buttonTextColor();
      static color buttonHighLightColor();

      static value_type value_of (byte r, byte g, byte b) {
        return r_::c(r) | g_::c(g) | b_::c(b);
      }

      inline color(value_type value = 0)
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

      inline operator value_type () const {
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

      const value_type value;

    private:
      template<int shift>
      struct part {
        static value_type c(byte a) {
          return (value_type)a << shift;
        }

        static byte v(value_type a) {
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
