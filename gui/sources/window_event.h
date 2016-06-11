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

  struct window_event {
    inline window_event(window_id id_, event_id msg_, event_param_1 p1, event_param_2 p2)
      : id(id_)
      , msg(msg_)
      , param_1(p1)
      , param_2(p2)
    {}

    window_id     id;
    event_id      msg;
    event_param_1 param_1;
    event_param_2 param_2;
  };

  class event_handler {
  public:
    virtual bool handle_event(const window_event& e, event_result& result) = 0;
  };

}


