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
 * @brief     C++ API: window events
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bind_method.h>
#include <gui/core/event.h>
#include <gui/core/logger.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    typedef bool (event_matcher) (const event& e);

    // --------------------------------------------------------------------------
    template<os::event_id id>
    inline bool event_id_matcher (const event& e) {
      return (e.type == id);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    using param_getter = T (*)(const event&);

    // --------------------------------------------------------------------------
    template<typename ... Types>
    struct params {
      template<param_getter<Types>... Getter>
      struct getter {
        using callback = void(Types...);
        typedef std::function<callback> function;

        getter (const function& f)
          : f(f)
        {}

        getter (function&& f)
          : f(std::move(f))
        {}

        template<class C>
        getter (C* t, void(C::*method)(Types...))
          : f(core::bind_method(t, method))
        {}

        operator bool () const {
          return (bool)f;
        }

        void operator() (const event& e) {
          f(Getter(e) ...);
        }

      protected:
        function f;
      };
    };

    // --------------------------------------------------------------------------
    template<os::event_id E,
             os::event_id Mask = 0,
             typename C = params<>::getter<>,
             os::event_result R = 0,
             event_matcher Matcher = event_id_matcher<E>>
    struct event_handler {

      typedef C Caller;
      typedef typename Caller::function function;

      static constexpr os::event_id mask = Mask;

      event_handler (const function& cb)
        : caller(cb)
      {}

      event_handler (function&& cb)
        : caller(std::move(cb))
      {}

      template<typename T, typename F>
      event_handler (T* t, F f)
        : caller(core::bind_method(t, f))
      {}

      event_handler (const event_handler& rhs)
        : caller(rhs.caller)
      {}

      event_handler (event_handler&& rhs)
        : caller(std::move(rhs.caller))
      {}

      static bool match (const event& e) {
        return Matcher(e);
      }

      bool operator() (const event& e, os::event_result& result) {
        if (match(e) && caller) {
          if (e.type != IF_WIN32_ELSE(WM_MOUSEMOVE, MotionNotify)) {
            LogDebug << "Call " << e;
          }
          caller(e);
          result = R;
          return true;
        }
        return false;
      }

      os::event_id get_event_id () const {
        return E;
      }

    protected:
      Caller caller;
    };

  } // namespace core

} // namespace gui
