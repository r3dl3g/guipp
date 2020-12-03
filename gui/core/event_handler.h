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
#include <util/bind_method.h>
#include <gui/core/event.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    typedef bool (event_matcher) (const event& e);

    // --------------------------------------------------------------------------
    template<gui::os::event_id id>
    struct event_id_matcher {
      inline bool operator() (const event& e) {
        return (IF_QT_ELSE(e.type(), e.type) == id);
      }
    };

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
          : f(util::bind_method(t, method))
        {}

        operator bool () const {
          return (bool)f;
        }

        void operator() (const event& e) {
          f(Getter(e) ...);
        }

        template<typename T, typename F>
        static void call (const event& e, T* t, F f) {
          (t->*f)(Getter(e) ...);
        }

        static void call_fn (const event& e, function f) {
          f(Getter(e) ...);
        }

      protected:
        function f;
      };
    };

    // --------------------------------------------------------------------------
    template<gui::os::event_id E,
             gui::os::event_id Mask = 0,
             typename C = params<>::getter<>,
             gui::os::event_result R = 0,
             typename M = event_id_matcher<E>>
    struct event_handler {

      typedef C Caller;
      typedef M Matcher;
      typedef typename Caller::function function;

      static constexpr gui::os::event_id mask = Mask;

      event_handler (const function& cb)
        : caller(cb)
      {}

      event_handler (function&& cb)
        : caller(std::move(cb))
      {}

      template<typename T, typename F>
      event_handler (T* t, F f)
        : caller(util::bind_method(t, f))
      {}

      event_handler (const event_handler& rhs)
        : caller(rhs.caller)
      {}

      event_handler (event_handler&& rhs)
        : caller(std::move(rhs.caller))
      {}

      static bool match (const event& e) {
        Matcher m;
        return m(e);
      }

      template<typename T, typename F>
      static bool if_match_call (const event& e, T* t, F f) {
        if (match(e)) {
          Caller::call(e, t, f);
          return true;
        }
        return false;
      }

      static bool if_match_call_fn (const event& e, function f) {
        if (match(e)) {
          Caller::call_fn(e, f);
          return true;
        }
        return false;
      }

      bool operator() (const event& e, gui::os::event_result& result) {
        if (matcher(e) && caller) {
#ifdef WIN32
        if (e.type != WM_MOUSEMOVE) {
#endif // WIN32
#ifdef X11
        if (e.type != MotionNotify) {
#endif // X11
#ifdef QT_WIDGETS_LIB
        if (e.type() != QEvent::Type::MouseMove) {
#endif // QT_WIDGETS_LIB
            clog::trace() << "Call " << e;
          }
          caller(e);
          result = R;
          return true;
        }
        return false;
      }

      gui::os::event_id get_event_id () const {
        return E;
      }

    protected:
      Caller caller;
      Matcher matcher;
    };

  } // namespace core

} // namespace gui
