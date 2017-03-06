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
#include <map>
#include <functional>
#include <memory>
//#include <mutex>


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

      inline event_container()
        : handle_event_stack_count(0)
      {}

      void register_event_handler(event_handler_function);
      void unregister_event_handler(event_handler_function);

      bool handle_event(const event& e, core::event_result& result);

    private:
      typedef std::vector<event_handler_function> event_handler_list;

      event_handler_list event_handlers;
      int                handle_event_stack_count;

      typedef std::pair<bool, event_handler_function> change_entry;
      typedef std::vector<change_entry> change_entry_list;

      typedef std::map<event_container*, change_entry_list> event_handler_change_map;

      static event_handler_change_map change_map;
    };

  } // core

} // gui
