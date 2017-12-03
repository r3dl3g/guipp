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
 * @brief     C++ API: window event handler function
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef X11
# include <map>
#endif // X11
#include <thread>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/keys.h>


namespace gui {

  namespace win {

    class window;

#ifdef WIN32
    namespace win32 {

      GUIPP_EXPORT LRESULT CALLBACK WindowEventProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    } // namespace win32
#endif // WIN32

#ifdef X11
    namespace x11 {

      GUIPP_EXPORT XIC get_window_ic (os::window);

    } // namespace x11
#endif // X11

    namespace detail {

      GUIPP_EXPORT window* get_window (os::window id);
      GUIPP_EXPORT void set_window (os::window id, window* win);
      GUIPP_EXPORT void unset_window (os::window id);

      GUIPP_EXPORT bool check_expose (const core::event& e);

      typedef bool (filter_fn)(const core::event&);
      typedef std::function<filter_fn> filter_call;

    } // namespace detail

    namespace global {

      GUIPP_EXPORT void register_hot_key (const hot_key&, const hot_key::call&, window* = nullptr);
      GUIPP_EXPORT void unregister_hot_key (const hot_key&);

      GUIPP_EXPORT int register_message_filter (detail::filter_call filter);
      GUIPP_EXPORT void unregister_message_filter (int& id);

      GUIPP_EXPORT window* get_current_focus_window ();
      GUIPP_EXPORT window* get_application_main_window ();

      GUIPP_EXPORT std::thread::id get_current_thread_id ();

      GUIPP_EXPORT void register_utf8_window (os::window);
      GUIPP_EXPORT void unregister_utf8_window (os::window);

    } // namespace global

    GUIPP_EXPORT bool is_button_event_outside (const window& w, const core::event& e);
    GUIPP_EXPORT bool check_hot_key (const core::event& e);

    GUIPP_EXPORT int run_loop (volatile bool& running, detail::filter_call filter = nullptr);

    GUIPP_EXPORT int run_main_loop ();
    GUIPP_EXPORT void quit_main_loop ();

    typedef void (simple_action)();

    GUIPP_EXPORT void run_on_main (std::function<simple_action> action);

  } // namespace win

} // namespace gui
