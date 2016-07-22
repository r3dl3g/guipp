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


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "event_container.h"


namespace gui {

  namespace core {

    event_container::event_handler_ptr event_container::register_event_handler(event_handler_fnct handler) {
      event_handler_ptr ptr(new event_handler_fnct(handler));
      {
        std::lock_guard<std::mutex> lock(event_handlers_mutex);
        event_handlers.push_back(ptr);
      }
      return ptr;
    }

    void event_container::unregister_event_handler(event_handler_ptr ptr) {
      std::lock_guard<std::mutex> lock(event_handlers_mutex);
      auto end = event_handlers.end();
      event_handlers.erase(std::remove(event_handlers.begin(), end, ptr), end);
    }

    bool event_container::handle_event(const event& e, core::event_result& resultValue) {

      event_handler_list temp_event_handlers;

      {
        std::lock_guard<std::mutex> lock(event_handlers_mutex);
        temp_event_handlers = event_handlers;
      }

      bool result = false;
      for (auto i : temp_event_handlers) {
        try {
          result |= i.get()->operator()(e, resultValue);
        } catch (std::exception e) {
          LogFatal << "exception in event_container::handle_event:" << e;
        } catch (...) {
          LogFatal << "Unknown exception in event_container::handle_event()";
        }
      }

      return result;
    }

  } // core

} // gui
