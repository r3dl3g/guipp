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
 * @brief     Qt specific window class definitions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/win/window_class.h"


namespace gui {

  namespace win {

    namespace cls {

      // --------------------------------------------------------------------------
      template<typename T, os::color C = color::medium_gray>
      using main_window_class = window_class<T,
                                             C,
                                             cursor_type::arrow,
                                             SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS>;


      template<typename T, os::color C = color::light_gray>
      using popup_window_class = window_class<T,
                                              C,
                                              window_class_defaults<>::cursor,
                                              SDL_WINDOW_POPUP_MENU | SDL_WINDOW_BORDERLESS>;

      template<typename T, os::color C = color::light_gray>
      using tooltip_window_class = window_class<T,
                                                C,
                                                window_class_defaults<>::cursor,
                                                SDL_WINDOW_TOOLTIP | SDL_WINDOW_BORDERLESS>;

      template<typename T, os::color C = color::light_gray>
      using dialog_window_class = window_class<T,
                                               C,
                                               window_class_defaults<>::cursor,
                                               SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS>;

      template<typename T, os::color C = color::light_gray>
      using palette_window_class = window_class<T,
                                                C,
                                                window_class_defaults<>::cursor,
                                                SDL_WINDOW_UTILITY | SDL_WINDOW_BORDERLESS>;

      // --------------------------------------------------------------------------
    } // namespace cls

  } // namespace win

} // namespace gui
