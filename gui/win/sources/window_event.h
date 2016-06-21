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
* @brief     C++ API: window_event
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

  namespace win {

#ifdef WIN32
    struct window_event {
      inline window_event(core::window_id id,
                          core::event_id msg,
                          core::event_param_1 p1,
                          core::event_param_2 p2)
        : id(id)
        , msg(msg)
        , param_1(p1)
        , param_2(p2)
      {}

      core::window_id     id;
      core::event_id      msg;
      core::event_param_1 param_1;
      core::event_param_2 param_2;
    };
#elif X11
    typedef XEvent window_event;
#endif // WIN

    typedef bool(event_handler)(const window_event&, core::event_result&);

  } // win

} // gui
