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
* @brief     C++ API: event
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  namespace core {

#ifdef WIN32
    struct event {
      inline event(os::window id,
                   os::event_id e,
                   os::win32::event_param_1 p1,
                   os::win32::event_param_2 p2)
        : id(id)
        , type(e)
        , param_1(p1)
        , param_2(p2)
      {}

      os::window               id;
      os::event_id             type;
      os::win32::event_param_1 param_1;
      os::win32::event_param_2 param_2;
    };

#endif // Win32
#ifdef X11
    typedef XEvent event;

#endif // WIN

    inline os::event_id get_event_id (const event& e) {
      return e.type;
    }

    typedef bool(event_handler_callback)(const event&, os::event_result&);

  } // core

} // gui
