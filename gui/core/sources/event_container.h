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


// --------------------------------------------------------------------------
//
// Library includes
//
#include "event.h"
#include "easy_bind.h"


namespace gui {

  namespace core {

    class event_container {
    public:
      typedef std::function<event_handler> event_handler_fnct;
      typedef std::shared_ptr<event_handler_fnct> event_handler_ptr;

      event_container();

      event_handler_ptr register_event_handler(event_handler_fnct);
      void unregister_event_handler(event_handler_ptr);

      bool handle_event(const event& e, core::event_result& result);

    protected:
      bool in_event_handle() const;

    private:
      typedef std::vector<event_handler_ptr> event_handler_list;
      typedef std::pair<bool, event_handler_ptr> event_handler_store;
      typedef std::vector<event_handler_store> event_handler_store_list;

      event_handler_list event_handlers;
      event_handler_store_list* event_handler_stores;
    };

  } // core

} // gui
