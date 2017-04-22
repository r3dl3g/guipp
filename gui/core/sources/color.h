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


    constexpr color_part_type calc_medium_gray (os::color c) {
      return static_cast<color_part_type>((static_cast<int>(extract_red(c)) +
                                           static_cast<int>(extract_green(c)) +
                                           static_cast<int>(extract_blue(c))) / 3);
    }

    namespace detail {
      constexpr int fast_mul_5 (int v) {
        return (v << 2) + v;
      }

      constexpr int fast_mul_9 (int v) {
        return (v << 3) + v;
      }

      constexpr int fast_mul_2 (int v) {
        return v << 1;
      }

      constexpr int fast_div_16 (int v) {
        return v >> 4;
      }
    }

    constexpr color_part_type calc_weigth_gray (os::color c) {
      return static_cast<color_part_type>(detail::fast_div_16(detail::fast_mul_5(extract_red(c)) +
                                                              detail::fast_mul_9(extract_green(c)) +
                                                              detail::fast_mul_2(extract_blue(c))));
    }

    constexpr os::color add_transparency (os::color c, float faktor) {
      return (c & ~(alpha_primary<0xFF>::value)) | build_alpha_primary(static_cast<color_part_type>(faktor * 255));
    }

    constexpr os::color remove_transparency (os::color c) {
      return (c & ~(alpha_primary<0xFF>::value));
    }

    constexpr os::color merge (os::color lhs, os::color rhs, float rfaktor = 0.5F) {
      return create_rgba(static_cast<color_part_type>(static_cast<float>(extract_red(lhs)) * (1.0F - rfaktor) + static_cast<float>(extract_red(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<float>(extract_green(lhs)) * (1.0F - rfaktor) + static_cast<float>(extract_green(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<float>(extract_blue(lhs)) * (1.0F - rfaktor) + static_cast<float>(extract_blue(rhs)) * rfaktor),
                         static_cast<color_part_type>(static_cast<float>(extract_alpha(lhs)) * (1.0F - rfaktor) + static_cast<float>(extract_alpha(rhs)) * rfaktor));
    }

    constexpr os::color merge (os::color lhs, color_part_type r, color_part_type g, color_part_type b, float rfaktor = 0.5F) {
      return create_rgba(static_cast<color_part_type>(static_cast<float>(extract_red(lhs)) * (1.0 - rfaktor) + static_cast<float>(r) * rfaktor),
                         static_cast<color_part_type>(static_cast<float>(extract_green(lhs)) * (1.0 - rfaktor) + static_cast<float>(g) * rfaktor),
                         static_cast<color_part_type>(static_cast<float>(extract_blue(lhs)) * (1.0 - rfaktor) + static_cast<float>(b) * rfaktor),
                         extract_alpha(lhs));
    }

    constexpr os::color darker (os::color c, float faktor = 0.25F) {
      return merge(c, 0, 0, 0, faktor);
    }

    constexpr os::color lighter (os::color c, float faktor = 0.25F) {
      return merge(c, 0xff, 0xff, 0xff, faktor);
    }

    constexpr os::color invert (os::color c) {
      return create_rgba(0xFF - extract_red(c), 0xFF - extract_green(c), 0xFF - extract_blue(c), extract_alpha(c));
    }

    constexpr int compare (os::color lhs, os::color rhs) {
      return ((static_cast<int>(extract_red(lhs)) +
               static_cast<int>(extract_green(lhs)) +
               static_cast<int>(extract_blue(lhs))) -
              (static_cast<int>(extract_red(rhs)) +
               static_cast<int>(extract_green(rhs)) +
               static_cast<int>(extract_blue(rhs))));
    }

    constexpr int compare_weigth (os::color lhs, os::color rhs) {
      return static_cast<int>(calc_weigth_gray(lhs)) -
             static_cast<int>(calc_weigth_gray(rhs));
    }

    const os::color black = rgb_gray<0>::value;
    const os::color white = rgb_gray<0xff>::value;
    const os::color gray = rgb_gray<0x80>::value;
    const os::color dark_gray = rgb_gray<0x40>::value;
    const os::color very_dark_gray = rgb_gray<0x20>::value;
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
