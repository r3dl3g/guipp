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

    /**
    * To avoid crashes during handle_event when adding or remove new handlers,
    * this is done at the beginning of the dispatch loop, and only when not
    * in a recursive call to handle_event.
    */
    event_container::event_handler_change_map event_container::change_map;

    void event_container::register_event_handler (event_handler_function handler) {
      //LogDebug << "Queue new event handler add";
      change_map[this].push_back(make_pair(true, handler));
    }

    void event_container::unregister_event_handler (event_handler_function handler) {
      //LogDebug << "Queue old event handler erase";
      change_map[this].push_back(make_pair(false, handler));
    }

    bool event_container::handle_event(const event& ev, os::event_result& resultValue) {

      if (handle_event_stack_count == 0) {
        typedef event_handler_change_map::iterator iterator;
        iterator j = change_map.find(this);
        if (j != change_map.end()) {
          change_entry_list& list = j->second;
          for (change_entry e : list) {
            if (e.first) {  // add
              //LogDebug << "Add new event handler";
              event_handlers.push_back(e.second);
            } else {        // remove
              const auto end = event_handlers.end();
              const auto k = std::find_if(event_handlers.begin(), end, [&](const event_handler_function& rhs) {
                return (rhs.target_type() == e.second.target_type());
              });
              if (k != end) {
                //LogDebug << "Erase old event handler";
                event_handlers.erase(k);
              }
            }
          }
          change_map.erase(j);
        }
      }

      ++handle_event_stack_count;

      bool result = false;
      typedef event_handler_list::iterator iterator;
      for (iterator i = event_handlers.begin(), e = event_handlers.end(); i != e; ++i) {
        try {
          const event_handler_function& fn = *i;
          result |= fn(ev, resultValue);
        } catch (std::exception e) {
          LogFatal << "exception in event_container::handle_event:" << e.what();
        } catch (...) {
          LogFatal << "Unknown exception in event_container::handle_event()";
        }
      }

      --handle_event_stack_count;

      return result;
    }

  } // core

} // gui
