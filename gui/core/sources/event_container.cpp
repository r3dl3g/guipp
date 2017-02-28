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
      event_handlers.push_back(std::make_pair(true, handler));
    }

    void event_container::unregister_event_handler (event_handler_function handler) {
      std::lock_guard<std::mutex> lock(event_handlers_mutex);
      const auto end = event_handlers.end();
      const auto i = std::find_if(event_handlers.begin(), end, [&](const list_entry& rhs) {
        if (rhs.first) {
          return (rhs.second.target_type() == handler.target_type());
        }
        return false;
      });
      if (i != end) {
        event_handlers.erase(i);
      } else {
        event_handlers.push_back(std::make_pair(false, handler));
      }
    }

    bool event_container::handle_event(const event& e, core::event_result& resultValue) {

      event_handler_list temp_event_handlers;

      {
        std::lock_guard<std::mutex> lock(event_handlers_mutex);
        temp_event_handlers.swap(event_handlers);
      }

      bool result = false;
      for (list_entry i : temp_event_handlers) {
        try {
          result |= i.second(e, resultValue);
        } catch (std::exception e) {
          LogFatal << "exception in event_container::handle_event:" << e;
        } catch (...) {
          LogFatal << "Unknown exception in event_container::handle_event()";
        }
      }

      std::lock_guard<std::mutex> lock(event_handlers_mutex);
      temp_event_handlers.swap(event_handlers);

      for (list_entry i : temp_event_handlers) {
        try {
          if (i.first) { // entry added during dispatch loop
            event_handlers.push_back(i);
          } else {       // entry removed during dispatch loop
            const auto end = event_handlers.end();
            const auto j = std::find_if(event_handlers.begin(), end, [&](const list_entry& rhs) {
              return (rhs.second.target_type() == i.second.target_type());
            });
            if (j != end) {
              event_handlers.erase(j);
            }
          }
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
