/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     X11 specific window class definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
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
      template<typename T, os::color C = color::very_light_gray>
      using main_window_class = window_class<T, C>;

      template<typename T, os::color C = color::very_light_gray>
      using popup_window_class = window_class<T, C>;

      template<typename T, os::color C = color::dark_gray>
      using tooltip_window_class = window_class<T, C>;

      template<typename T, os::color C = color::very_light_gray>
      using dialog_window_class = window_class<T, C>;

      // --------------------------------------------------------------------------
    } // namespace cls

  } // namespace win

} // namespace gui
