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
* @brief     C++ API: basic window
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstddef>
#include <vector>
#include <functional>
#include <memory>
#include <mutex>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "event.h"


namespace gui {

  namespace core {

    class event_container {
    public:
      typedef std::function<event_handler> event_handler_function;

      void register_event_handler(event_handler_function);
      void unregister_event_handler(event_handler_function);

      bool handle_event(const event& e, core::event_result& result);

    private:
      typedef std::pair<bool, event_handler_function> list_entry;
      typedef std::vector<list_entry> event_handler_list;

      event_handler_list event_handlers;
      std::mutex event_handlers_mutex;
    };

  } // core

} // gui
