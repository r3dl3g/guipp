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
 * @brief     basic color definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/core/guidefs.h>


namespace gui {

  namespace color {

    typedef unsigned char type;

    struct color_parts {
      type red;
      type green;
      type blue;
      type alpha;
    };

    struct rgba_parts {
      static constexpr type red = 0;
      static constexpr type green = 8;
      static constexpr type blue = 16;
      static constexpr type alpha = 24;

      static constexpr color_parts get_color_parts() {
        return {red, green, blue, alpha};
      }
    };

    struct bgra_parts {
      static constexpr type blue = 0;
      static constexpr type green = 8;
      static constexpr type red = 16;
      static constexpr type alpha = 24;

      static constexpr color_parts get_color_parts() {
        return {red, green, blue, alpha};
      }
    };

    struct argb_parts {
      static constexpr type alpha = 0;
      static constexpr type red = 8;
      static constexpr type green = 16;
      static constexpr type blue = 24;

      static constexpr color_parts get_color_parts() {
        return {red, green, blue, alpha};
      }
    };

    struct abgr_parts {
      static constexpr type alpha = 0;
      static constexpr type blue = 8;
      static constexpr type green = 16;
      static constexpr type red = 24;

      static constexpr color_parts get_color_parts() {
        return {red, green, blue, alpha};
      }
    };

    struct gray_parts {
      static constexpr type alpha = 0;
      static constexpr type blue = 0;
      static constexpr type green = 0;
      static constexpr type red = 0;

      static constexpr color_parts get_color_parts() {
        return {red, green, blue, alpha};
      }
    };

    color_parts get_color_parts (pixel_format_t px_fmt);

    template<core::os::platform_t P = core::os::system_platform>
    struct part_defaults {};

    template<>
    struct part_defaults<core::os::platform_t::win32> : public rgba_parts {};

    template<>
    struct part_defaults<core::os::platform_t::cocoa> : public rgba_parts {};
    
    template<>
    struct part_defaults<core::os::platform_t::x11> : public bgra_parts {};

    template<>
    struct part_defaults<core::os::platform_t::qt> : public bgra_parts {};

    enum struct part : type {
      red = part_defaults<>::red,
      green = part_defaults<>::green,
      blue = part_defaults<>::blue,
      alpha = part_defaults<>::alpha
    };

    template<part P, type T>
    struct build_primary {
      static const os::color value = (os::color)T << static_cast<type>(P);
    };

    template<part P>
    using mask = build_primary<P, 0xff>;


    template<part P, os::color C>
    struct extract_primary {
      static constexpr type value = (type)((C & (mask<P>::value)) >> static_cast<type>(P));
    };


    template<type R, type G, type B>
    struct rgb {
      static constexpr os::color value = build_primary<part::red, R>::value |
                                         build_primary<part::green, G>::value |
                                         build_primary<part::blue, B>::value/* |
                                         mask<part::alpha>::value*/;
    };

    template<type V>
    struct rgb_gray {
      static constexpr os::color value = build_primary<part::red, V>::value |
                                         build_primary<part::green, V>::value |
                                         build_primary<part::blue, V>::value/* |
                                         mask<part::alpha>::value*/;
    };

    template<type R, type G, type B, type A>
    struct rgba {
      static constexpr os::color value = build_primary<part::red, R>::value |
                                         build_primary<part::green, G>::value |
                                         build_primary<part::blue, B>::value |
                                         build_primary<part::alpha, A>::value;
    };

    template<type V, type A>
    struct rgba_gray {
      static constexpr os::color value = build_primary<part::red, V>::value |
                                         build_primary<part::green, V>::value |
                                         build_primary<part::blue, V>::value |
                                         build_primary<part::alpha, A>::value;
    };

    template<part P>
    constexpr os::color build (type p) {
      return (os::color)p << static_cast<type>(P);
    }

    template<part P>
    constexpr type extract (os::color c) {
      return (type)((c & (mask<P>::value)) >> static_cast<type>(P));
    }

    constexpr os::color calc_rgb (type r, type g, type b) {
      return build<part::red>(r) | build<part::green>(g) | build<part::blue>(b)/* | mask<part::alpha>::value*/;
    }

    constexpr os::color calc_rgba (type r, type g, type b, type a) {
      return build<part::red>(r) | build<part::green>(g) | build<part::blue>(b) | build<part::alpha>(a);
    }

    constexpr os::color calc_rgb_gray (type v) {
      return calc_rgb(v, v, v);
    }

    constexpr os::color calc_rgba_gray (type v, type a) {
      return calc_rgba(v, v, v, a);
    }

    constexpr type calc_medium_gray (os::color c) {
      return static_cast<type>((static_cast<int>(extract<part::red>(c)) +
                                static_cast<int>(extract<part::green>(c)) +
                                static_cast<int>(extract<part::blue>(c))) / 3);
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

    constexpr type calc_weight_gray (os::color c) {
      return static_cast<type>(detail::fast_div_16(detail::fast_mul_5(extract<part::red>(c)) +
                                                   detail::fast_mul_9(extract<part::green>(c)) +
                                                   detail::fast_mul_2(extract<part::blue>(c))));
    }

    constexpr os::color remove_color (os::color c) {
      return calc_rgb_gray(calc_weight_gray(c));
    }

    constexpr os::color remove_transparency (os::color c) {
      return (c & ~mask<part::alpha>::value);
    }

    constexpr os::color add_transparency (os::color c, float faktor) {
      return remove_transparency(c) | build<part::alpha>(static_cast<type>((1.0F - faktor) * 0xff));
    }

    template<part P>
    constexpr type merge (os::color lhs, os::color rhs, float rfaktor = 0.5F) {
      return static_cast<type>(static_cast<float>(extract<P>(lhs)) * (1.0F - rfaktor) +
                               static_cast<float>(extract<P>(rhs)) * rfaktor);
    }

    constexpr os::color merge (os::color lhs, os::color rhs, float rfaktor = 0.5F) {
      return calc_rgba(merge<part::red>(lhs, rhs, rfaktor),
                       merge<part::green>(lhs, rhs, rfaktor),
                       merge<part::blue>(lhs, rhs, rfaktor),
                       merge<part::alpha>(lhs, rhs, rfaktor));
    }

    template<part P>
    constexpr type merge (os::color lhs, type rhs, float rfaktor = 0.5F) {
      return static_cast<type>(static_cast<float>(extract<P>(lhs)) * (1.0F - rfaktor) +
                               static_cast<float>(rhs) * rfaktor);
    }

    constexpr os::color merge (os::color lhs, type r, type g, type b, float rfaktor = 0.5F) {
      return calc_rgba(merge<part::red>(lhs, r, rfaktor),
                       merge<part::green>(lhs, g, rfaktor),
                       merge<part::blue>(lhs, b, rfaktor),
                       extract<part::alpha>(lhs));
    }

    constexpr os::color darker (os::color c, float faktor = 0.25F) {
      return merge(c, 0, 0, 0, faktor);
    }

    constexpr os::color lighter (os::color c, float faktor = 0.25F) {
      return merge(c, 0xff, 0xff, 0xff, faktor);
    }

    template<part P>
    constexpr type invert (os::color c) {
      return static_cast<type>(0xFF - extract<P>(c));
    }

    constexpr os::color invert (os::color c) {
      return calc_rgba(invert<part::red>(c),
                       invert<part::green>(c),
                       invert<part::blue>(c),
                       extract<part::alpha>(c));
    }

    constexpr int compare (os::color lhs, os::color rhs) {
      return ((static_cast<int>(extract<part::red>(lhs)) +
               static_cast<int>(extract<part::green>(lhs)) +
               static_cast<int>(extract<part::blue>(lhs))) -
              (static_cast<int>(extract<part::red>(rhs)) +
               static_cast<int>(extract<part::green>(rhs)) +
               static_cast<int>(extract<part::blue>(rhs))));
    }

    constexpr int compare_weight (os::color lhs, os::color rhs) {
      return static_cast<int>(calc_weight_gray(lhs)) -
             static_cast<int>(calc_weight_gray(rhs));
    }

    inline type get_gray (os::color c) {
      return calc_medium_gray(c);
    }

    inline type get_red (os::color c) {
      return extract<part::red>(c);
    }

    inline type get_green (os::color c) {
      return extract<part::green>(c);
    }

    inline type get_blue (os::color c) {
      return extract<part::blue>(c);
    }

    inline type get_alpha (os::color c) {
      return extract<part::alpha>(c);
    }

    inline bool is_transparent (os::color c) {
      return get_alpha(c) == 0xff;
    }

    constexpr os::color transparent = rgba_gray<0xff, 0xff>::value;

    constexpr os::color black = rgb_gray<0>::value;
    constexpr os::color very_very_dark_gray = rgb_gray<0x10>::value;
    constexpr os::color very_dark_gray = rgb_gray<0x20>::value;
    constexpr os::color dark_gray = rgb_gray<0x40>::value;
    constexpr os::color gray = rgb_gray<0x80>::value;
    constexpr os::color medium_gray = rgb_gray<0xA0>::value;
    constexpr os::color light_gray = rgb_gray<0xC0>::value;
    constexpr os::color very_light_gray = rgb_gray<0xE0>::value;
    constexpr os::color very_very_light_gray = rgb_gray<0xF0>::value;
    constexpr os::color white = rgb_gray<0xff>::value;

    constexpr os::color percent_grey (int f) {
      return calc_rgb_gray(static_cast<type>(0xff * f / 100));
    }

    constexpr os::color grey (float f) {
      return calc_rgb_gray(static_cast<type>(255.0F * f));
    }


    constexpr os::color red = rgb<0xff, 0, 0>::value;
    constexpr os::color green = rgb<0, 0xff, 0>::value;
    constexpr os::color blue = rgb<0, 0, 0xff>::value;
    constexpr os::color cyan = rgb<0, 0xff, 0xff>::value;
    constexpr os::color magenta = rgb<0xff, 0, 0xff>::value;
    constexpr os::color yellow = rgb<0xff, 0xff, 0>::value;
    constexpr os::color orange = rgb<0xff, 0x80, 0>::value;
    constexpr os::color brown = rgb<0xc0, 0x40, 0>::value;

    constexpr os::color dark_red = rgb<0xc0, 0, 0>::value;
    constexpr os::color dark_green = rgb<0, 0xc0, 0>::value;
    constexpr os::color dark_blue = rgb<0, 0, 0xc0>::value;
    constexpr os::color dark_cyan = rgb<0, 0xc0, 0xc0>::value;
    constexpr os::color dark_magenta = rgb<0xc0, 0, 0xc0>::value;
    constexpr os::color dark_yellow = rgb<0xc0, 0xc0, 0>::value;
    constexpr os::color dark_orange = rgb<0xc0, 0x60, 0>::value;
    constexpr os::color dark_brown = rgb<0x80, 0x30, 0>::value;

    constexpr os::color light_red = rgb<0xff, 0x80, 0x80>::value;
    constexpr os::color light_green = rgb<0x80, 0xff, 0x80>::value;
    constexpr os::color light_blue = rgb<0x40, 0x80, 0xff>::value;
    constexpr os::color light_cyan = rgb<0x80, 0xff, 0xff>::value;
    constexpr os::color light_magenta = rgb<0xff, 0x80, 0xff>::value;
    constexpr os::color light_yellow = rgb<0xff, 0xff, 0x80>::value;
    constexpr os::color light_orange = rgb<0xe0, 0x80, 0>::value;
    constexpr os::color light_brown = rgb<0xa0, 0x50, 0>::value;

    constexpr os::color very_light_red = rgb<0xff, 0xc0, 0xc0>::value;
    constexpr os::color very_light_green = rgb<0xc0, 0xff, 0xc0>::value;
    constexpr os::color very_light_blue = rgb<0x80, 0xc0, 0xff>::value;
    constexpr os::color very_light_cyan = rgb<0xc0, 0xff, 0xff>::value;
    constexpr os::color very_light_magenta = rgb<0xff, 0xc0, 0xff>::value;
    constexpr os::color very_light_yellow = rgb<0xff, 0xff, 0xc0>::value;
    constexpr os::color very_light_orange = rgb<0xf0, 0xa0, 0>::value;
    constexpr os::color very_light_brown = rgb<0xc0, 0x70, 0>::value;

    constexpr os::color very_dark_red = rgb<0x60, 0, 0>::value;
    constexpr os::color very_dark_green = rgb<0, 0x60, 0>::value;
    constexpr os::color very_dark_blue = rgb<0, 0, 0x60>::value;
    constexpr os::color very_dark_cyan = rgb<0, 0x60, 0x60>::value;
    constexpr os::color very_dark_magenta = rgb<0x60, 0, 0x60>::value;
    constexpr os::color very_dark_yellow = rgb<0x60, 0x60, 0>::value;
    constexpr os::color very_dark_orange = rgb<0x60, 0x30, 0>::value;
    constexpr os::color very_dark_brown = rgb<0x40, 0x18, 0>::value;

    GUIPP_CORE_EXPORT os::color scrollBarColor ();
    GUIPP_CORE_EXPORT os::color menuColor ();
    GUIPP_CORE_EXPORT os::color menuTextColor ();
    GUIPP_CORE_EXPORT os::color menuColorHighlight ();
    GUIPP_CORE_EXPORT os::color backGroundColor ();
    GUIPP_CORE_EXPORT os::color activeCaptionColor ();
    GUIPP_CORE_EXPORT os::color inActiveCaptionColor ();
    GUIPP_CORE_EXPORT os::color windowColor ();
    GUIPP_CORE_EXPORT os::color windowFrameColor ();
    GUIPP_CORE_EXPORT os::color windowTextColor ();
    GUIPP_CORE_EXPORT os::color captionTextColor ();
    GUIPP_CORE_EXPORT os::color inActiveCaptionTextColor ();
    GUIPP_CORE_EXPORT os::color activeBorderColor ();
    GUIPP_CORE_EXPORT os::color inActiveBorderColor ();
    GUIPP_CORE_EXPORT os::color workSpaceColor ();
    GUIPP_CORE_EXPORT os::color highLightColor ();
    GUIPP_CORE_EXPORT os::color highLightTextColor ();
    GUIPP_CORE_EXPORT os::color disabledTextColor ();
    GUIPP_CORE_EXPORT os::color buttonColor ();
    GUIPP_CORE_EXPORT os::color shadowColor ();
    GUIPP_CORE_EXPORT os::color buttonTextColor ();
    GUIPP_CORE_EXPORT os::color buttonHighLightColor ();

    // --------------------------------------------------------------------------
    template<bool W>
    struct GUIPP_CORE_EXPORT bw_colors {};

    template<>
    struct GUIPP_CORE_EXPORT bw_colors<false> {
      static constexpr os::color value[2] = {white, black};
    };

    template<>
    struct GUIPP_CORE_EXPORT bw_colors<true> {
      static constexpr os::color value[2] = {black, white};
    };

    using system_bw_colors = bw_colors<core::os::bitmap_bit_white>;

  } // color

} //gui

namespace std {

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::color::color_parts);

} // namespace std
