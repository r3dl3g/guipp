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
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "indices.h"

namespace gui {

  namespace core {

    namespace detail {

#ifdef X11
# define _Ph _Placeholder
#endif // X11

      template<typename T, std::size_t... Is, typename F, typename... Args>
      auto bind_method(T* t, indices<Is...>, F& f, Args&&... args)
        -> decltype(std::bind(f, t, std::forward<Args>(args)..., std::_Ph < Is + 1 > {}...)) {
        return std::bind(f, t, std::forward<Args>(args)..., std::_Ph < Is + 1 > {}...);
      };

    } // detail

    template<typename T, typename R, typename... FArgs>
    auto bind_method(T* t, R(T::*f)(FArgs...))
      -> decltype(detail::bind_method(t, detail::build_indices < sizeof...(FArgs) > {}, f)) {
      return detail::bind_method(t, detail::build_indices < sizeof...(FArgs) > {}, f);
    }

  } // core

} // gui
