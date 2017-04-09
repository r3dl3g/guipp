/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
* @brief     C++ API: basic window types
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "color.h"

#define DEFINE_SYS_COLOR(name, SYS_NAME) \
   color color::name () {\
     static color c(GetSysColor(SYS_NAME));\
     return c;\
   }

#define DEFINE_RGB_COLOR(name, R, G, B) \
   color color::name () {\
     static color c{ R, G, B };\
     return c;\
   }

namespace gui {

  namespace draw {

    DEFINE_RGB_COLOR(black, 0, 0, 0)
    DEFINE_RGB_COLOR(white, 255, 255, 255)
    DEFINE_RGB_COLOR(gray, 128, 128, 128)
    DEFINE_RGB_COLOR(red, 255, 0, 0)
    DEFINE_RGB_COLOR(green, 0, 255, 0)
    DEFINE_RGB_COLOR(blue, 0, 0, 255)
    DEFINE_RGB_COLOR(cyan, 0, 255, 255)
    DEFINE_RGB_COLOR(magenta, 255, 0, 255)
    DEFINE_RGB_COLOR(yellow, 255, 255, 0)
    DEFINE_RGB_COLOR(orange, 255, 128, 0)
    DEFINE_RGB_COLOR(brown, 192, 64, 0)
    DEFINE_RGB_COLOR(darkGray, 64, 64, 64)
    DEFINE_RGB_COLOR(darkBlue, 0, 0, 192)
    DEFINE_RGB_COLOR(darkGreen, 0, 192, 0)
    DEFINE_RGB_COLOR(darkRed, 192, 0, 0)
    DEFINE_RGB_COLOR(darkCyan, 0, 192, 192)
    DEFINE_RGB_COLOR(darkMagenta, 192, 0, 192)
    DEFINE_RGB_COLOR(darkYellow, 192, 192, 0)
    DEFINE_RGB_COLOR(darkOrange, 192, 96, 0)
    DEFINE_RGB_COLOR(darkBrown, 128, 48, 0)
    DEFINE_RGB_COLOR(mediumGray, 160, 160, 160)
    DEFINE_RGB_COLOR(lightGray, 192, 192, 192)
    DEFINE_RGB_COLOR(lightRed, 255, 128, 128)
    DEFINE_RGB_COLOR(lightGreen, 128, 255, 128)
    DEFINE_RGB_COLOR(lightBlue, 128, 128, 255)
    DEFINE_RGB_COLOR(lightCyan, 128, 255, 255)
    DEFINE_RGB_COLOR(lightMagenta, 255, 128, 255)
    DEFINE_RGB_COLOR(lightYellow, 255, 255, 128)
    DEFINE_RGB_COLOR(veryLightGray, 224, 224, 224)
    DEFINE_RGB_COLOR(veryLightRed, 255, 192, 192)
    DEFINE_RGB_COLOR(veryLightGreen, 192, 255, 192)
    DEFINE_RGB_COLOR(veryLightBlue, 192, 192, 255)
    DEFINE_RGB_COLOR(veryLightCyan, 192, 255, 255)
    DEFINE_RGB_COLOR(veryLightMagenta, 255, 192, 255)
    DEFINE_RGB_COLOR(veryLightYellow, 255, 255, 192)
    DEFINE_RGB_COLOR(veryVeryLightGray, 240, 240, 240)

#ifdef X11
    enum SystemColor : os::color {
        COLOR_SCROLLBAR,
        COLOR_MENU,
        COLOR_MENUTEXT,
        COLOR_BACKGROUND,
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

    os::color GetSysColor(SystemColor c) {
      switch (c) {
        case COLOR_APPWORKSPACE:  return color::mediumGray();
        case COLOR_WINDOWTEXT:    return color::black();
        case COLOR_HIGHLIGHT:     return color::darkBlue();
        case COLOR_HIGHLIGHTTEXT: return color::white();
        case COLOR_BTNFACE:       return color::veryVeryLightGray();
        case COLOR_WINDOW:        return color::white();
      }
//        XColor exact_def_return, screen_def_return;
//        XLookupColor(display, colormap, color_name, &exact_def_return, &screen_def_return);
//        return screen_def_return.pixel;
        return c;
    }
#endif

   DEFINE_SYS_COLOR(scrollBarColor, COLOR_SCROLLBAR)
   DEFINE_SYS_COLOR(menuColor, COLOR_MENU)
   DEFINE_SYS_COLOR(menuTextColor, COLOR_MENUTEXT)
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

    std::ostream& operator<<(std::ostream& out, const color& c) {
      out << std::hex << c.r() << ", " << c.g() << ", " << c.b() << ", " << c.a();
      return out;
    }

  }

}
