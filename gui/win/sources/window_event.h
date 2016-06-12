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

    class event_handler {
    public:
      virtual bool handle_event(const window_event& e, core::event_result& result) = 0;
    };

  } // win

} // gui
