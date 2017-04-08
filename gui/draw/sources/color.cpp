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


namespace gui {

	namespace draw {

    const color color::black{ 0, 0, 0 };
    const color color::white{ 255, 255, 255 };
    const color color::gray{ 128, 128, 128 };
    const color color::red{ 255, 0, 0 };
    const color color::green{ 0, 255, 0 };
    const color color::blue{ 0, 0, 255 };
    const color color::cyan{ 0, 255, 255 };
    const color color::magenta{ 255, 0, 255 };
    const color color::yellow{ 255, 255, 0 };
    const color color::orange{ 255, 128, 0 };
    const color color::brown{ 192, 64, 0 };
    const color color::darkGray{ 64, 64, 64 };
    const color color::darkBlue{ 0, 0, 192 };
    const color color::darkGreen{ 0, 192, 0 };
    const color color::darkRed{ 192, 0, 0 };
    const color color::darkCyan{ 0, 192, 192 };
    const color color::darkMagenta{ 192, 0, 192 };
    const color color::darkYellow{ 192, 192, 0 };
    const color color::darkOrange{ 192, 96, 0 };
    const color color::darkBrown{ 128, 48, 0 };
    const color color::mediumGray{ 160, 160, 160 };
    const color color::lightGray{ 192, 192, 192 };
    const color color::lightRed{ 255, 128, 128 };
    const color color::lightGreen{ 128, 255, 128 };
    const color color::lightBlue{ 128, 128, 255 };
    const color color::lightCyan{ 128, 255, 255 };
    const color color::lightMagenta{ 255, 128, 255 };
    const color color::lightYellow{ 255, 255, 128 };
    const color color::veryLightGray{ 224, 224, 224 };
    const color color::veryLightRed{ 255, 192, 192 };
    const color color::veryLightGreen{ 192, 255, 192 };
    const color color::veryLightBlue{ 192, 192, 255 };
    const color color::veryLightCyan{ 192, 255, 255 };
    const color color::veryLightMagenta{ 255, 192, 255 };
    const color color::veryLightYellow{ 255, 255, 192 };
    const color color::veryVeryLightGray{ 240, 240, 240 };

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
        case COLOR_APPWORKSPACE:  return color::mediumGray;
        case COLOR_WINDOWTEXT:    return color::black;
        case COLOR_HIGHLIGHT:     return color::darkBlue;
        case COLOR_HIGHLIGHTTEXT: return color::white;
        case COLOR_BTNFACE:       return color::veryVeryLightGray;
        case COLOR_WINDOW:        return color::white;
      }
//        XColor exact_def_return, screen_def_return;
//        XLookupColor(display, colormap, color_name, &exact_def_return, &screen_def_return);
//        return screen_def_return.pixel;
        return c;
    }
#endif

    const color color::scrollBarColor(GetSysColor(COLOR_SCROLLBAR));
    const color color::menuColor(GetSysColor(COLOR_MENU));
    const color color::menuTextColor(GetSysColor(COLOR_MENUTEXT));
    const color color::backGroundColor(GetSysColor(COLOR_BACKGROUND));
    const color color::activeCaptionColor(GetSysColor(COLOR_ACTIVECAPTION));
    const color color::inActiveCaptionColor(GetSysColor(COLOR_INACTIVECAPTION));
    const color color::windowColor(GetSysColor(COLOR_WINDOW));
    const color color::windowFrameColor(GetSysColor(COLOR_WINDOWFRAME));
    const color color::windowTextColor(GetSysColor(COLOR_WINDOWTEXT));
    const color color::captionTextColor(GetSysColor(COLOR_CAPTIONTEXT));
    const color color::inActiveCaptionTextColor(GetSysColor(COLOR_INACTIVECAPTIONTEXT));
    const color color::activeBorderColor(GetSysColor(COLOR_ACTIVEBORDER));
    const color color::inActiveBorderColor(GetSysColor(COLOR_INACTIVEBORDER));
    const color color::workSpaceColor(GetSysColor(COLOR_APPWORKSPACE));
    const color color::highLightColor(GetSysColor(COLOR_HIGHLIGHT));
    const color color::highLightTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
    const color color::disabledTextColor(GetSysColor(COLOR_GRAYTEXT));
    const color color::buttonColor(GetSysColor(COLOR_BTNFACE));
    const color color::shadowColor(GetSysColor(COLOR_BTNSHADOW));
    const color color::buttonTextColor(GetSysColor(COLOR_BTNTEXT));
    const color color::buttonHighLightColor(GetSysColor(COLOR_BTNHIGHLIGHT));

    std::ostream& operator<<(std::ostream& out, const color& c) {
      out << std::hex << c.r() << ", " << c.g() << ", " << c.b() << ", " << c.a();
      return out;
    }

  }

}
