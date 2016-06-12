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
* @brief     C++ API: windows message debug helper
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "guidefs.h"


namespace gui {

  bool is_frequent_event(event_id e);
  bool is_none_client_event(event_id e);

  struct EventId {
    EventId(event_id e)
      : id(e)
    {}

    event_id id;
  };

}

namespace std {

  std::ostream& operator<<(std::ostream& out, const gui::EventId& e);

}
