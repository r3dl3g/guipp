/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <base/logger.h>
#include <gui/core/event_container.h>

namespace gui {

  namespace core {

    void event_container::register_event_handler (const event_handler_function& handler) {
      event_handlers.reserve(8);
      event_handlers.emplace_back(handler);
    }

    void event_container::register_event_handler (event_handler_function&& handler) {
      event_handlers.reserve(8);
      event_handlers.emplace_back(handler);
    }

    void event_container::unregister_event_handler (const event_handler_function& handler) {
      const auto end = event_handlers.end();
      const auto k = std::find_if(event_handlers.begin(), end, [&](const event_handler_function &rhs) {
        return (rhs.target_type() == handler.target_type());
      });
      if (k != end) {
        event_handlers.erase(k);
      }
    }

    bool event_container::handle_event (const event& ev, os::event_result& resultValue) const {

      bool result = false;

      // to avoid a creash when the event_handlers vector is changed during handle_event
      // we iterate with an old style index counter and check against size.
      typedef event_handler_list::size_type size_type;
      for (size_type i = 0; i < event_handlers.size(); ++i) {
        try {
          result |= event_handlers[i](ev, resultValue);
        } catch (std::exception& e) {
          LogFatal << "exception in event_container::handle_event: " << e;
        } catch (...) {
          LogFatal << "Unknown exception in event_container::handle_event()";
        }
      }

      return result;
    }

  } // core

} // gui
