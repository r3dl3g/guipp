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
 * @brief     C++ API: win32 event
 *
 * @file
 */

#pragma once

#include <gui/core/guidefs.h>


namespace gui {

  namespace core {

    namespace win32 {

      struct event {
        event (gui::os::window id,
               gui::os::event_id e,
               gui::os::win32::wParam p1,
               gui::os::win32::lParam p2);

        event (const MSG& msg);

        gui::os::window id;
        gui::os::event_id type;
        gui::os::win32::wParam wParam;
        gui::os::win32::lParam lParam;
      };

      // --------------------------------------------------------------------------
      //
      // inlines
      //
      inline event::event (gui::os::window id,
                           gui::os::event_id e,
                           gui::os::win32::wParam p1,
                           gui::os::win32::lParam p2)
        : id(id)
        , type(e)
        , wParam(p1)
        , lParam(p2)
      {}

      // --------------------------------------------------------------------------
      inline event::event (const MSG& msg)
        : id(msg.hwnd)
        , type(msg.message)
        , wParam(msg.wParam)
        , lParam(msg.lParam)
      {}

    } // namespace win32

  } // core

} // gui
