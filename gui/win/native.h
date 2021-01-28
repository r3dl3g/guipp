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

    namespace native {

      GUIPP_WIN_EXPORT std::string get_class_name (os::window id);
      GUIPP_WIN_EXPORT const class_info& get_window_class (os::window id);

      GUIPP_WIN_EXPORT void move (os::window w, const core::point& pt);
      GUIPP_WIN_EXPORT void resize (os::window w, const core::size& sz);
      GUIPP_WIN_EXPORT void place (os::window w, const core::rectangle& r);

      GUIPP_WIN_EXPORT core::rectangle get_geometry (os::window wid);

      GUIPP_WIN_EXPORT void prepare (window* w);
      GUIPP_WIN_EXPORT void unprepare (window* w);

      GUIPP_WIN_EXPORT os::window create (const class_info& type,
                                          const core::rectangle& r,
                                          os::window parent_id,
                                          window* data);
      GUIPP_WIN_EXPORT void notify_created (window* w);
      GUIPP_WIN_EXPORT void destroy (os::window w);

      GUIPP_WIN_EXPORT void close (os::window id);
      GUIPP_WIN_EXPORT void notify_close (window& w);

      GUIPP_WIN_EXPORT bool is_visible (os::window id);
      GUIPP_WIN_EXPORT void set_visible (os::window id, bool s);

      GUIPP_WIN_EXPORT void enable (os::window id, bool on);

      GUIPP_WIN_EXPORT void to_front (os::window id);
      GUIPP_WIN_EXPORT void to_back (os::window id);

      GUIPP_WIN_EXPORT void take_focus (os::window id);
      GUIPP_WIN_EXPORT void set_cursor (os::window id, const os::cursor& c);

      GUIPP_WIN_EXPORT void invalidate (os::window id, const core::rectangle& p);
      GUIPP_WIN_EXPORT void redraw (const window& w, os::window id, const core::rectangle& p);

      GUIPP_WIN_EXPORT void prepare_accept_focus (os::window, bool);

      GUIPP_WIN_EXPORT void prepare_capture_pointer ();
      GUIPP_WIN_EXPORT void unprepare_capture_pointer ();

      GUIPP_WIN_EXPORT void capture_pointer (os::window id);
      GUIPP_WIN_EXPORT void uncapture_pointer (os::window id);

      GUIPP_WIN_EXPORT os::window get_desktop_window ();
      GUIPP_WIN_EXPORT core::size screen_size ();
      GUIPP_WIN_EXPORT core::rectangle screen_area ();

    } // namespace native

  } // namespace win

} // namespace gui
