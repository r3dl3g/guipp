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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "color.h"

#ifdef WIN32
# define DEFINE_SYS_COLOR(name, SYS_NAME) \
   os::color name () {\
     static os::color c(gui::color::get_sys_color(SYS_NAME));\
     return c;\
   }

#endif // WIN32

#ifdef X11
# define DEFINE_SYS_COLOR(name, SYS_NAME) \
   os::color name () {\
     static os::color c(gui::color::get_sys_color(gui::color::SYS_NAME));\
     return c;\
   }
#endif // X11

namespace gui {

  namespace color {

#ifdef WIN32

    os::color get_sys_color(int c) {
      return GetSysColor(c);
    }

#endif // WIN32

#ifdef X11

    os::color get_sys_color(SystemColor c) {
      switch (c) {

        case COLOR_APPWORKSPACE:  return rgb_color<160, 160, 160>::value;
        case COLOR_WINDOWTEXT:    return black;
        case COLOR_HIGHLIGHT:     return rgb_color<0, 0, 224>::value;
        case COLOR_HIGHLIGHTTEXT: return white;
        case COLOR_BTNFACE:       return rgb_color<240, 240, 240>::value;
        case COLOR_BTNHIGHLIGHT:  return rgb_color<248, 248, 248>::value;
        case COLOR_WINDOW:        return white;
        case COLOR_GRAYTEXT:      return rgb_gray<128>::value;

//        XColor exact_def_return, screen_def_return;
//        XLookupColor(display, colormap, color_name, &exact_def_return, &screen_def_return);
//        return screen_def_return.pixel;
      }
      return black;
    }

#endif // X11

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

  } // color

} // gui

