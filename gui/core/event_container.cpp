/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     event registrar
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/win/window_event_proc.h>
#include <gui/core/event_container.h>
#include <logging/logger.h>

namespace gui {

  namespace core {

    event_container::event_container ()
    {}

    void event_container::register_event_handler (const event_handler_function& handler) {
      event_handlers.reserve(8);
      event_handlers.emplace_back(handler);
    }

    void event_container::register_event_handler (event_handler_function&& handler) {
      event_handlers.reserve(8);
      event_handlers.emplace_back(std::move(handler));
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

    bool event_container::handle_event (const event& ev, gui::os::event_result& resultValue) {

      bool result = false;

      // to avoid a creash when the event_handlers vector is changed during handle_event
      // we iterate with an old style index counter and check against size.
      typedef event_handler_list::size_type size_type;
      for (size_type i = 0; i < event_handlers.size(); ++i) {
        try {
          result |= event_handlers[i](ev, resultValue);
        } catch (std::exception& ex) {
          logging::fatal() << "exception in event_container::handle_event: " << ex;
          gui::win::global::notify_error_handler(ev, ex);
        } catch (...) {
          logging::fatal() << "Unknown exception in event_container::handle_event()";
          gui::win::global::notify_error_handler(ev, std::runtime_error("Unknown exception"));
        }
      }

      return result;
    }

  } // core

} // gui
