/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window_class.h>


namespace gui {

  namespace win {

    namespace cls {

      // --------------------------------------------------------------------------
      template<typename T, os::color C = (os::color)(COLOR_APPWORKSPACE + 1)>
      using main_window_class = window_class<T,
                                             C,
                                             cursor_type::arrow,
                                             WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                                             WS_EX_APPWINDOW | WS_EX_WINDOWEDGE | WS_EX_COMPOSITED>;


      template<typename T, os::color C = color::light_gray>
      using popup_window_class = window_class<T,
                                              C,
                                              window_class_defaults<>::cursor,
                                              WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                              WS_EX_PALETTEWINDOW | WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED,
                                              CS_DBLCLKS | CS_DROPSHADOW>;

      template<typename T, os::color C = color::light_gray>
      using dialog_window_class = window_class<T,
                                               C,
                                               window_class_defaults<>::cursor,
                                               WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SYSMENU |
                                               WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
                                               WS_EX_NOPARENTNOTIFY | WS_EX_COMPOSITED>;

      // --------------------------------------------------------------------------
    } // namespace cls

  } // namespace win

} // namespace gui
