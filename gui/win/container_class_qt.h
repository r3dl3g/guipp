/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
      template<typename T, os::color C = color::medium_gray>
      using main_window_class = window_class<T,
                                             C,
                                             cursor_type::arrow,
                                             (Qt::WindowType)(Qt::WindowFlags::Int)(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowFullscreenButtonHint | Qt::WindowCloseButtonHint)>;


      template<typename T, os::color C = color::light_gray>
      using popup_window_class = window_class<T,
                                              C,
                                              window_class_defaults<>::cursor,
                                              (Qt::WindowType)(Qt::WindowFlags::Int)(Qt::Popup | Qt::WindowShadeButtonHint | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)>;

      template<typename T, os::color C = color::light_gray>
      using dialog_window_class = window_class<T,
                                               C,
                                               window_class_defaults<>::cursor,
                                               (Qt::WindowType)(Qt::WindowFlags::Int)(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowShadeButtonHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint)>;

      // --------------------------------------------------------------------------
    } // namespace cls

  } // namespace win

} // namespace gui
