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
      using main_window_class = window_class<T, C>;

      template<typename T, os::color C = color::light_gray>
      using popup_window_class = window_class<T, C>;

      template<typename T, os::color C = color::light_gray>
      using dialog_window_class = window_class<T, C>;

      // --------------------------------------------------------------------------
    } // namespace cls

  } // namespace win

} // namespace gui
