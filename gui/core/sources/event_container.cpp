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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "event_container.h"


namespace gui {

  namespace core {

    event_container::event_container()
      : event_handler_stores(nullptr) {
    }

    bool event_container::in_event_handle() const {
      return event_handler_stores != nullptr;
    }

    event_container::event_handler_ptr event_container::register_event_handler(event_handler_fnct handler) {
      event_handler_ptr ptr(new event_handler_fnct(handler));
      if (in_event_handle()) {
        event_handler_stores->push_back(std::make_pair(true, ptr));
        return ptr;
      }
      event_handlers.push_back(ptr);
      return ptr;
    }

    void event_container::unregister_event_handler(event_handler_ptr ptr) {
      if (in_event_handle()) {
        event_handler_stores->push_back(std::make_pair(false, ptr));
        return;
      }
      auto end = event_handlers.end();
      auto it = std::remove(event_handlers.begin(), end, ptr);
      if (it != end) {
        event_handlers.erase(it, end);
      }
    }

    bool event_container::handle_event(const event& e, core::event_result& resultValue) {

      bool my_event_handler_stores = false;

      if (!event_handler_stores) {
        // Make in_event_handle == true
        event_handler_stores = new event_handler_store_list();
        my_event_handler_stores = true;
      }

      bool result = false;
      for (auto i : event_handlers) {
        result |= i.get()->operator()(e, resultValue);
      }

      // only deepest call of handle_event manages the event_handler_stores
      if (my_event_handler_stores) {
        for (auto i : *event_handler_stores) {
          if (i.second) {
            if (i.first) {
              event_handlers.push_back(i.second);
            } else {
              auto end = event_handlers.end();
              event_handlers.erase(std::remove(event_handlers.begin(), end, i.second), end);
            }
          }
        }
        // Make in_event_handle == false
        delete event_handler_stores;
        event_handler_stores = nullptr;
      }
      return result;
    }

  } // core

} // gui
