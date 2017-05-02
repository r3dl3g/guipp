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


// --------------------------------------------------------------------------
//
// Library includes
//
#include "event.h"
#include "bind_method.h"


namespace gui {

  namespace core {

    class event_container {
    public:
      typedef std::function<event_handler_callback> event_handler_function;

      void register_event_handler(const event_handler_function&);
      void register_event_handler(event_handler_function&&);

      template<typename T>
      void register_event_handler(T* t, bool(T::*method)(const core::event&, os::event_result& result)) {
        register_event_handler(bind_method(t, method));
      }

      void unregister_event_handler(const event_handler_function&);

      bool handle_event(const event& e, os::event_result& result);

    private:
      typedef std::vector<event_handler_function> event_handler_list;

      event_handler_list event_handlers;
    };

  } // core

} // gui
