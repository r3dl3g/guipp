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
#include <gui/core/event.h>
#include <base/bind_method.h>


#ifndef REGISTER_FUNCTION
# ifdef __PRETTY_FUNCTION__
#  define REGISTER_FUNCTION __PRETTY_FUNCTION__
# elif defined(__FUNCSIG__)
#  define REGISTER_FUNCTION __FUNCSIG__
# else
#  define REGISTER_FUNCTION __func__
# endif // __FUNCSIG__
#endif // !REGISTER_FUNCTION

namespace gui {

  namespace core {

    class GUIPP_CORE_EXPORT event_container {
    public:
      typedef std::function<event_handler_callback> event_handler_function;

      void register_event_handler (const event_handler_function&);
      void register_event_handler (event_handler_function &&);

      template<typename T>
      void register_event_handler (T* t,
                                   bool (T::*method)(const core::event &, os::event_result &));

      void unregister_event_handler (const event_handler_function&);

      bool handle_event (const event& e, os::event_result& result) const;

    private:

      typedef std::vector<event_handler_function> event_handler_list;

      event_handler_list event_handlers;

    };

  } // core

} // gui

#include <gui/core/event_container.inl>
