/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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
      inline event(window_id id,
                   event_id msg,
                   event_param_1 p1,
                   event_param_2 p2)
        : id(id)
        , msg(msg)
        , param_1(p1)
        , param_2(p2)
      {}

      window_id     id;
      event_id      msg;
      event_param_1 param_1;
      event_param_2 param_2;
    };
#elif X11
    typedef XEvent event;
#endif // WIN

    typedef bool(event_handler)(const event&, event_result&);

  } // core

} // gui
