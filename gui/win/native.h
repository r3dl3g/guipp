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
 * @brief     C++ API: native Api calls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui++-win-export.h>


namespace gui {

  namespace win {

    class window;
    class class_info;
    class overlapped_window;

    namespace native {

      GUIPP_WIN_EXPORT const class_info& get_window_class (const char* class_name);
      GUIPP_WIN_EXPORT void register_window_class (const class_info& type);

      GUIPP_WIN_EXPORT void move (os::window id, const core::point& pt);
      GUIPP_WIN_EXPORT void resize (os::window id, const core::size& sz);
      GUIPP_WIN_EXPORT void geometry (os::window id, const core::rectangle& r);

      GUIPP_WIN_EXPORT void notify_move (window& w, const core::point& pt, const core::point& old);
      GUIPP_WIN_EXPORT void notify_resize (window& w, const core::size& sz, const core::size& old);

      GUIPP_WIN_EXPORT core::native_point get_position (os::window id);
      GUIPP_WIN_EXPORT core::size client_size (os::window id, const core::size& sz);

      GUIPP_WIN_EXPORT void prepare (overlapped_window& w);

      GUIPP_WIN_EXPORT os::window create (const class_info& type,
                                          const core::rectangle& r,
                                          os::window parent_id,
                                          overlapped_window& data);
      GUIPP_WIN_EXPORT void destroy (os::window w);

      GUIPP_WIN_EXPORT void close (os::window id);
      GUIPP_WIN_EXPORT void notify_close (window& w);

      GUIPP_WIN_EXPORT void prepare_win_for_event (const overlapped_window& win);

      GUIPP_WIN_EXPORT bool is_visible (os::window id);
      GUIPP_WIN_EXPORT void set_visible (os::window id, bool s);

      GUIPP_WIN_EXPORT void enable (overlapped_window& w, bool on);

      GUIPP_WIN_EXPORT void to_front (os::window id);
      GUIPP_WIN_EXPORT void to_back (os::window id);

      GUIPP_WIN_EXPORT void take_focus (os::window id);
      GUIPP_WIN_EXPORT void set_cursor (os::window id, const os::cursor& c);

      GUIPP_WIN_EXPORT void invalidate (os::window id, const core::native_rect& p);
      GUIPP_WIN_EXPORT void redraw (window& w, os::window id, const core::native_rect& p);

      GUIPP_WIN_EXPORT void prepare_accept_focus (os::window, bool);

      GUIPP_WIN_EXPORT void capture_pointer (os::window id);
      GUIPP_WIN_EXPORT void uncapture_pointer (os::window id);

      GUIPP_WIN_EXPORT os::window get_desktop_window ();
      GUIPP_WIN_EXPORT core::size screen_size ();
      GUIPP_WIN_EXPORT core::rectangle screen_area ();

      GUIPP_WIN_EXPORT core::rectangle adjust_overlapped_area (const core::rectangle& p, const class_info& type);
      GUIPP_WIN_EXPORT void prepare_overlapped (os::window id, os::window pid);
      GUIPP_WIN_EXPORT os::window get_overlapped_parent (os::window pid);

      GUIPP_WIN_EXPORT core::native_point surface_to_screen (os::window id, const core::native_point&);
      GUIPP_WIN_EXPORT core::native_point screen_to_surface (os::window id, const core::native_point&);

      GUIPP_WIN_EXPORT void set_title (os::window id, const std::string& title);
      GUIPP_WIN_EXPORT std::string get_title (os::window id);

      GUIPP_WIN_EXPORT bool is_maximized (os::window id);
      GUIPP_WIN_EXPORT bool is_minimized (os::window id);
      GUIPP_WIN_EXPORT bool is_top_most (os::window id);

      GUIPP_WIN_EXPORT void minimize (os::window id);
      GUIPP_WIN_EXPORT void maximize (os::window id);
      GUIPP_WIN_EXPORT void restore (os::window id);
      GUIPP_WIN_EXPORT void set_top_most (os::window id, bool on);

      GUIPP_WIN_EXPORT void prepare_main_window (os::window id);
      GUIPP_WIN_EXPORT void prepare_popup_window (os::window id);
      GUIPP_WIN_EXPORT void prepare_dialog_window (os::window id, os::window pid);

      GUIPP_WIN_EXPORT void erase (os::drawable id, os::graphics gc,
                                   const core::native_rect& r, os::color c);
      GUIPP_WIN_EXPORT void frame (os::drawable id, os::graphics gc,
                                   const core::native_rect& r, os::color c);

      GUIPP_WIN_EXPORT os::backstore create_surface (const core::native_size& size, os::window id);
      GUIPP_WIN_EXPORT void delete_surface (os::backstore);

      GUIPP_WIN_EXPORT void copy_surface (os::bitmap src, os::drawable target,
                                          os::graphics context,
                                          const core::native_point& from,
                                          const core::native_point& to,
                                          const core::native_size& size);

      GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, long l1 = 0, long l2 = 0);
      GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, void* v1, void* v2);
      GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::size& sz);
      GUIPP_WIN_EXPORT void send_client_message (window* win, os::message_type message, const core::rectangle& wr);

      GUIPP_WIN_EXPORT void send_mouse_event (window* win, bool enter);
      GUIPP_WIN_EXPORT void send_notify_visibility (window* win, bool visible);

    } // namespace native

  } // namespace win

} // namespace gui
