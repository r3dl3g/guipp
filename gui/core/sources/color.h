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
* @brief     C++ API: basic color definitions
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace color {
#ifdef WIN32

    os::color get_sys_color(int);

#endif // WIN32

#ifdef X11
    enum SystemColor {
      COLOR_SCROLLBAR,
      COLOR_BACKGROUND,
      COLOR_MENU,
      COLOR_MENUTEXT,
      COLOR_ACTIVECAPTION,
      COLOR_INACTIVECAPTION,
      COLOR_WINDOW,
      COLOR_WINDOWFRAME,
      COLOR_WINDOWTEXT,
      COLOR_CAPTIONTEXT,
      COLOR_INACTIVECAPTIONTEXT,
      COLOR_ACTIVEBORDER,
      COLOR_INACTIVEBORDER,
      COLOR_APPWORKSPACE,
      COLOR_HIGHLIGHT,
      COLOR_HIGHLIGHTTEXT,
      COLOR_GRAYTEXT,
      COLOR_BTNFACE,
      COLOR_BTNSHADOW,
      COLOR_BTNTEXT,
      COLOR_BTNHIGHLIGHT
    };

    os::color get_sys_color(SystemColor);
#endif // X11

    typedef unsigned char color_part_type;

    namespace detail {

#ifdef WIN32
      const int red_shift = 0;
      const int green_shift  = 8;
      const int blue_shift = 16;
      const int alpha_shift = 24;
#endif // WIN32
#ifdef X11
      const int red_shift = 16;
      const int green_shift  = 8;
      const int blue_shift = 0;
      const int alpha_shift = 24;
#endif // X11

      template<color_part_type P, int S>
      struct rgb_primary {
        static const os::color value = (os::color)P << S;
      };

      template<os::color C, int S>
      struct extract_rgb_primary {
        static const color_part_type value = (color_part_type)((C & (detail::rgb_primary<0xff, S>::value)) >> S);
      };

      template<int S>
      constexpr os::color shift_color_part (color_part_type p) {
        return (os::color)p << S;
      }

      template<int S>
      constexpr color_part_type split_color_part (os::color c) {
        return (color_part_type)((c & (detail::rgb_primary<0xff, S>::value)) >> S);
      }

    }

    template<color_part_type P>
    using red_primary = detail::rgb_primary<P, detail::red_shift>;

    template<color_part_type P>
    using green_primary = detail::rgb_primary<P, detail::green_shift>;

    template<color_part_type P>
    using blue_primary = detail::rgb_primary<P, detail::blue_shift>;

    template<color_part_type P>
    using alpha_primary = detail::rgb_primary<P, detail::alpha_shift>;


    template<color_part_type R, color_part_type G, color_part_type B>
    struct rgb_color {
      static const os::color value = red_primary<R>::value |
                                 green_primary<G>::value |
                                 blue_primary<B>::value;
    };

    template<color_part_type V>
    struct rgb_gray {
      static const os::color value = red_primary<V>::value |
                                 green_primary<V>::value |
                                 blue_primary<V>::value;
    };

    template<color_part_type R, color_part_type G, color_part_type B, color_part_type A>
    struct rgba_color {
      static const os::color value = red_primary<R>::value |
                                 green_primary<G>::value |
                                 blue_primary<B>::value |
                                 alpha_primary<A>::value;
    };

    template<color_part_type V, color_part_type A>
    struct rgba_gray {
      static const os::color value = red_primary<V>::value |
                                 green_primary<V>::value |
                                 blue_primary<V>::value |
                                 alpha_primary<A>::value;
    };

    template<os::color C>
    using extract_red_primary = detail::extract_rgb_primary<C, detail::red_shift>;

    template<os::color C>
    using extract_green_primary = detail::extract_rgb_primary<C, detail::green_shift>;

    template<os::color C>
    using extract_blue_primary = detail::extract_rgb_primary<C, detail::blue_shift>;

    template<os::color C>
    using extract_alpha_primary = detail::extract_rgb_primary<C, detail::alpha_shift>;


    constexpr os::color build_red_primary (color_part_type p) {
      return detail::shift_color_part<detail::red_shift>(p);
    }

    constexpr os::color build_green_primary (color_part_type p) {
      return detail::shift_color_part<detail::green_shift>(p);
    }

    constexpr os::color build_blue_primary (color_part_type p) {
      return detail::shift_color_part<detail::blue_shift>(p);
    }

    constexpr os::color build_alpha_primary (color_part_type p) {
      return detail::shift_color_part<detail::alpha_shift>(p);
    }


    constexpr color_part_type extract_red (os::color c) {
      return detail::split_color_part<detail::red_shift>(c);
    }

    constexpr color_part_type extract_green (os::color c) {
      return detail::split_color_part<detail::green_shift>(c);
    }

    constexpr color_part_type extract_blue (os::color c) {
      return detail::split_color_part<detail::blue_shift>(c);
    }

    constexpr color_part_type extract_alpha (os::color c) {
      return detail::split_color_part<detail::alpha_shift>(c);
    }


    constexpr os::color create_rgb (color_part_type r, color_part_type g, color_part_type b) {
      return build_red_primary(r) | build_green_primary(g) | build_blue_primary(b);
    }

    constexpr os::color create_rgba (color_part_type r, color_part_type g, color_part_type b, color_part_type a) {
      return build_red_primary(r) | build_green_primary(g) | build_blue_primary(b) | build_alpha_primary(a);
    }

    constexpr os::color create_gray (color_part_type v) {
      return create_rgb(v, v, v);
    }

    constexpr os::color create_gray_aplha (color_part_type v, color_part_type a) {
      return create_rgba(v, v, v, a);
    }


    constexpr os::color calc_medium_gray (os::color c) {
      return create_gray_aplha(static_cast<color_part_type>((static_cast<os::color>(extract_red(c)) +
                                                             static_cast<os::color>(extract_green(c)) +
                                                             static_cast<os::color>(extract_blue(c))) / 3),
                               extract_alpha(c));
    }

    constexpr os::color add_transparency (os::color c, double faktor) {
      return (c & ~(alpha_primary<0xFF>::value)) | build_alpha_primary(static_cast<color_part_type>(faktor * 255));
    }

    constexpr os::color darker (os::color c, double faktor = 1.5) {
      return create_rgba(static_cast<color_part_type>(static_cast<double>(extract_red(c)) / faktor),
                         static_cast<color_part_type>(static_cast<double>(extract_green(c)) / faktor),
                         static_cast<color_part_type>(static_cast<double>(extract_blue(c)) / faktor),
                         extract_alpha(c));
    }

    constexpr os::color lighter (os::color c, double faktor = 1.3333) {
      return create_rgba(static_cast<color_part_type>(static_cast<double>(extract_red(c)) * faktor),
                         static_cast<color_part_type>(static_cast<double>(extract_green(c)) * faktor),
                         static_cast<color_part_type>(static_cast<double>(extract_blue(c)) * faktor),
                         extract_alpha(c));
    }

    constexpr os::color merge (os::color lhs, os::color rhs, double rfaktor = 0.5) {
      return create_rgba(static_cast<color_part_type>(static_cast<double>(extract_red(lhs)) * (1.0 - rfaktor) + static_cast<double>(extract_red(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<double>(extract_green(lhs)) * (1.0 - rfaktor) + static_cast<double>(extract_green(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<double>(extract_blue(lhs)) * (1.0 - rfaktor) + static_cast<double>(extract_blue(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<double>(extract_alpha(lhs)) * (1.0 - rfaktor) + static_cast<double>(extract_alpha(rhs)) * rfaktor));
    }

    constexpr os::color invert (os::color c) {
      return create_rgba(0xFF - extract_red(c), 0xFF - extract_green(c), 0xFF - extract_blue(c), extract_alpha(c));
    }


    const os::color black = rgb_gray<0>::value;
    const os::color white = rgb_gray<0xff>::value;
    const os::color gray = rgb_gray<0x80>::value;
    const os::color dark_gray = rgb_gray<0x40>::value;
    const os::color medium_gray = rgb_gray<0xA0>::value;
    const os::color light_gray = rgb_gray<0xC0>::value;
    const os::color very_light_gray = rgb_gray<0xE0>::value;
    const os::color very_very_light_gray = rgb_gray<0xF0>::value;

    const os::color red = rgb_color<255, 0, 0>::value;
    const os::color green = rgb_color<0, 255, 0>::value;
    const os::color blue = rgb_color<0, 0, 255>::value;
    const os::color cyan = rgb_color<0, 255, 255>::value;
    const os::color magenta = rgb_color<255, 0, 255>::value;
    const os::color yellow = rgb_color<255, 255, 0>::value;
    const os::color orange = rgb_color<255, 128, 0>::value;
    const os::color brown = rgb_color<192, 64, 0>::value;

    const os::color dark_red = rgb_color<192, 0, 0>::value;
    const os::color dark_green = rgb_color<0, 192, 0>::value;
    const os::color dark_blue = rgb_color<0, 0, 192>::value;
    const os::color dark_cyan = rgb_color<0, 192, 192>::value;
    const os::color dark_magenta = rgb_color<192, 0, 192>::value;
    const os::color dark_yellow = rgb_color<192, 192, 0>::value;
    const os::color dark_orange = rgb_color<192, 96, 0>::value;
    const os::color dark_brown = rgb_color<128, 48, 0>::value;

    const os::color light_red = rgb_color<255, 128, 128>::value;
    const os::color light_green = rgb_color<128, 255, 128>::value;
    const os::color light_blue = rgb_color<128, 128, 255>::value;
    const os::color light_cyan = rgb_color<128, 255, 255>::value;
    const os::color light_magenta = rgb_color<255, 128, 255>::value;
    const os::color light_yellow = rgb_color<255, 255, 128>::value;

    const os::color very_light_red = rgb_color<255, 192, 192>::value;
    const os::color very_light_green = rgb_color<192, 255, 192>::value;
    const os::color very_light_blue = rgb_color<192, 192, 255>::value;
    const os::color very_light_cyan = rgb_color<192, 255, 255>::value;
    const os::color very_light_magenta = rgb_color<255, 192, 255>::value;
    const os::color very_light_yellow = rgb_color<255, 255, 192>::value;

    os::color scrollBarColor();
    os::color menuColor();
    os::color menuTextColor();
    os::color backGroundColor();
    os::color activeCaptionColor();
    os::color inActiveCaptionColor();
    os::color windowColor();
    os::color windowFrameColor();
    os::color windowTextColor();
    os::color captionTextColor();
    os::color inActiveCaptionTextColor();
    os::color activeBorderColor();
    os::color inActiveBorderColor();
    os::color workSpaceColor();
    os::color highLightColor();
    os::color highLightTextColor();
    os::color disabledTextColor();
    os::color buttonColor();
    os::color shadowColor();
    os::color buttonTextColor();
    os::color buttonHighLightColor();

  } // color

} //gui
