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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/keys.h>


namespace gui {

  namespace win {

    class window;

    namespace detail {

#ifdef WIN32

      LRESULT CALLBACK WindowEventProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // WIN32

      window* get_window (os::window id);
      void set_window (os::window id, window* win);
      void unset_window (os::window id);

      typedef bool (filter_fn)(const core::event&);
      typedef std::function<filter_fn> filter_call;

#ifdef X11
      XIC get_window_ic (os::window);
#endif // X11

    } // namespace detail

    namespace global {

      void register_hot_key (const hot_key&, const hot_key::call&, window* = nullptr);
      void unregister_hot_key (const hot_key&);

      int register_message_filter (detail::filter_call filter);
      void unregister_message_filter (int& id);

      window* get_current_focus_window ();

      void register_utf8_window (os::window);
      void unregister_utf8_window (os::window);

    } // namespace global

    bool is_button_event_outside (const window& w, const core::event& e);
    bool check_hot_key (const core::event& e);

    int run_loop (volatile bool& running, detail::filter_call filter = nullptr);

    int run_main_loop ();
    void quit_main_loop ();

  } // namespace win

} // namespace gui