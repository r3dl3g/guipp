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
 * @brief     Calling method helper function
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>

namespace gui {

  namespace core {

    template<class T, typename R, typename ... FArgs>
    std::function<R(FArgs ...)> bind_method (T* t, R (T::*method)(FArgs ...)) {
      return [=](FArgs&& ... args) {
        return (t->*method)(std::forward<decltype(args)>(args) ...);
      };
    }

    template<class T, typename R, typename ... FArgs>
    std::function<R(FArgs ...)> bind_method (const T* t, R (T::*method)(FArgs ...) const) {
      return [=](FArgs&& ... args) {
        return (t->*method)(std::forward<decltype(args)>(args) ...);
      };
    }

  } // core

} // gui
