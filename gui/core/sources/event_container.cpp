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

    void event_container::register_event_handler (event_handler_function handler) {
      std::lock_guard<std::mutex> lock(event_handlers_mutex);
      event_handlers.push_back(handler);
    }

    void event_container::unregister_event_handler (event_handler_function handler) {
      std::lock_guard<std::mutex> lock(event_handlers_mutex);
      const auto end = event_handlers.end();
      const auto i = std::find_if(event_handlers.begin(), end, [&](const event_handler_function& rhs) {
		return (rhs.target_type() == handler.target_type());
      });
      if (i != end) {
        event_handlers.erase(i);
      }
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
          result |= i(e, resultValue);
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
