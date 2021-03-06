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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/color.h>


#define DEFINE_SYS_COLOR(name, SYS_NAME) \
  os::color name() { \
    return get_sys_color(SYS_NAME); \
  }

namespace gui {

  namespace color {

    color_parts get_color_parts (pixel_format_t px_fmt) {
      switch (px_fmt) {
        case pixel_format_t::RGB:
        case pixel_format_t::RGBA:
          return rgba_parts::get_color_parts();
        case pixel_format_t::BGR:
        case pixel_format_t::BGRA:
          return bgra_parts::get_color_parts();
        case pixel_format_t::ARGB:
          return argb_parts::get_color_parts();
        case pixel_format_t::ABGR:
          return abgr_parts::get_color_parts();
        default:
          return gray_parts::get_color_parts();
      }
    }

#ifdef GUIPP_WIN

    os::color get_sys_color (int c) {
      return GetSysColor(c);
    }

#endif // GUIPP_WIN

#if defined(GUIPP_X11) || defined(GUIPP_COCOA) || defined(GUIPP_QT) && !defined(GUIPP_WIN)

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
      COLOR_BTNHIGHLIGHT,
      COLOR_MENUHILIGHT
    };

    os::color get_sys_color (SystemColor c) {
      switch (c) {

      case COLOR_APPWORKSPACE:  return rgb<160, 160, 160>::value;
      case COLOR_WINDOWTEXT:    return black;
      case COLOR_HIGHLIGHT:     return rgb<48, 140, 198>::value;
      case COLOR_HIGHLIGHTTEXT: return white;
      case COLOR_BTNFACE:       return rgb<240, 240, 240>::value;
      case COLOR_BTNHIGHLIGHT:  return rgb<248, 248, 248>::value;
      case COLOR_WINDOW:        return white;
      case COLOR_GRAYTEXT:      return rgb_gray<128>::value;
      case COLOR_MENU:          return very_light_gray;
      case COLOR_MENUTEXT:      return black;
      case COLOR_MENUHILIGHT:   return rgb<48, 140, 198>::value;
      default: break;

//        XColor exact_def_return, screen_def_return;
//        XLookupColor(display, colormap, color_name, &exact_def_return, &screen_def_return);
//        return screen_def_return.pixel;
      }
      return black;
    }

#endif // GUIPP_X11

    DEFINE_SYS_COLOR(scrollBarColor, COLOR_SCROLLBAR)
    DEFINE_SYS_COLOR(menuColor, COLOR_MENU)
    DEFINE_SYS_COLOR(menuTextColor, COLOR_MENUTEXT)
    DEFINE_SYS_COLOR(menuColorHighlight, COLOR_MENUHILIGHT)
    DEFINE_SYS_COLOR(backGroundColor, COLOR_BACKGROUND)
    DEFINE_SYS_COLOR(activeCaptionColor, COLOR_ACTIVECAPTION)
    DEFINE_SYS_COLOR(inActiveCaptionColor, COLOR_INACTIVECAPTION)
    DEFINE_SYS_COLOR(windowColor, COLOR_WINDOW)
    DEFINE_SYS_COLOR(windowFrameColor, COLOR_WINDOWFRAME)
    DEFINE_SYS_COLOR(windowTextColor, COLOR_WINDOWTEXT)
    DEFINE_SYS_COLOR(captionTextColor, COLOR_CAPTIONTEXT)
    DEFINE_SYS_COLOR(inActiveCaptionTextColor, COLOR_INACTIVECAPTIONTEXT)
    DEFINE_SYS_COLOR(activeBorderColor, COLOR_ACTIVEBORDER)
    DEFINE_SYS_COLOR(inActiveBorderColor, COLOR_INACTIVEBORDER)
    DEFINE_SYS_COLOR(workSpaceColor, COLOR_APPWORKSPACE)
    DEFINE_SYS_COLOR(highLightColor, COLOR_HIGHLIGHT)
    DEFINE_SYS_COLOR(highLightTextColor, COLOR_HIGHLIGHTTEXT)
    DEFINE_SYS_COLOR(disabledTextColor, COLOR_GRAYTEXT)
    DEFINE_SYS_COLOR(buttonColor, COLOR_BTNFACE)
    DEFINE_SYS_COLOR(shadowColor, COLOR_BTNSHADOW)
    DEFINE_SYS_COLOR(buttonTextColor, COLOR_BTNTEXT)
    DEFINE_SYS_COLOR(buttonHighLightColor, COLOR_BTNHIGHLIGHT)

    constexpr os::color bw_colors<false>::value[];
    constexpr os::color bw_colors<true>::value[];

  } // color

} // gui

namespace std {

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, const gui::color::color_parts cp) {
    out << "color_parts: RGBA=" << std::hex << cp.red
                         << ":" << std::hex << cp.green
                         << ":" << std::hex << cp.blue
                         << ":" << std::hex << cp.alpha;
    return out;
  }

} // namespace std
