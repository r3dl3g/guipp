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

      typedef os::color_part_type part_type;
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

      constexpr color (value_type value = 0)
        : value(value)
      {}

      constexpr color (part_type r, part_type g, part_type b)
        : value(os::create_rgb(r, g, b))
      {}

      constexpr color (part_type r, part_type g, part_type b, part_type alpha)
        : value(os::create_rgba(r, g, b, alpha))
      {}

      constexpr part_type r () const {
        return os::extract_red(value);
      }

      constexpr part_type g () const {
        return os::extract_green(value);
      }

      constexpr part_type b () const {
        return os::extract_blue(value);
      }

      constexpr part_type a () const {
        return os::extract_alpha(value);
      }

      constexpr operator value_type () const {
        return value;
      }

      color medium_gray () const {
        const part_type v = (part_type)(((int)r() + (int)g() + (int)b()) / 3);
        return color(v, v, v, a());
      }

      color transparency (double faktor) const {
        return color(r(), g(), b(), (part_type)(faktor * 255));
      }

      color darker (double faktor = 1.5) const {
        return color((part_type)((double)r() / faktor),
                     (part_type)((double)g() / faktor),
                     (part_type)((double)b() / faktor),
                     a());
      }

      color lighter (double faktor = 1.3333) const {
        return color((part_type)((double)r() * faktor),
                     (part_type)((double)g() * faktor),
                     (part_type)((double)b() * faktor),
                     a());
      }

      color merge (const color &rhs, double rfaktor = 0.5) const {
        const double lfaktor = 1.0 - rfaktor;
        return color((part_type)((double)rhs.r() * rfaktor + (double)r() * lfaktor),
                     (part_type)((double)rhs.g() * rfaktor + (double)g() * lfaktor),
                     (part_type)((double)rhs.b() * rfaktor + (double)b() * lfaktor),
                     (part_type)((double)rhs.a() * rfaktor + (double)a() * lfaktor));
      }

      color invert () const {
        return color(255 - r(), 255 - g(), 255 - b(), a());
      }

      const value_type value;
    };

    std::ostream& operator<<(std::ostream& out, const color& c);

  }

}
