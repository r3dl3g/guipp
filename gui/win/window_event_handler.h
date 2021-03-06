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
 * @brief     generic window events
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstddef>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/core/event_handler.h>
#include <gui/core/context.h>
#include <gui/win/window_event_proc.h>


namespace gui {

  namespace win {

    class window;

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT os::key_state get_key_state (const core::event& e);
    GUIPP_WIN_EXPORT os::key_symbol get_key_symbol (const core::event& e);
    GUIPP_WIN_EXPORT std::string get_key_chars (const core::event& e);
    GUIPP_WIN_EXPORT core::native_point get_root_mouse_pos (const core::event& e);

    namespace event {

      // --------------------------------------------------------------------------
      template<core::event_matcher fnct>
      struct functor {
        inline bool operator() (const core::event& e) {
          return fnct(e);
        }
      };

    }

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT core::context* get_context (const core::event&);

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT bool is_mouse_event (const core::event&);
    GUIPP_WIN_EXPORT bool is_key_event (const core::event&);

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#ifdef GUIPP_WIN
# include <gui/win/window_event_handler_win32.h>
#elif GUIPP_X11
# include <gui/win/window_event_handler_x11.h>
#elif GUIPP_QT
# include <gui/win/window_event_handler_qt.h>
#else
# error Unknown target system
#endif // GUIPP_QT
