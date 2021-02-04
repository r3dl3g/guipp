/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: window events
 *
 * @file
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
#include <gui/core/gui_types.h>
#include <gui/win/window_event_proc.h>


namespace gui {

  namespace win {

    class window;

    // --------------------------------------------------------------------------
    GUIPP_WIN_EXPORT os::key_state get_key_state (const core::event& e);
    GUIPP_WIN_EXPORT os::key_symbol get_key_symbol (const core::event& e);
    GUIPP_WIN_EXPORT std::string get_key_chars (const core::event& e);
    GUIPP_WIN_EXPORT core::point get_root_mouse_pos (const core::event& e);

    namespace event {

      // --------------------------------------------------------------------------
      template<core::event_matcher fnct>
      struct functor {
        inline bool operator() (const core::event& e) {
          return fnct(e);
        }
      };

    }

    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, long l1 = 0, long l2 = 0);
    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::size& sz);
    GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::rectangle& wr);

    GUIPP_WIN_EXPORT void provide_surface_for_event (os::surface*, const core::event&);
    GUIPP_WIN_EXPORT void reject_surface_for_event (const core::event&);
    GUIPP_WIN_EXPORT os::surface* get_surface_for_event (const core::event&);


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
