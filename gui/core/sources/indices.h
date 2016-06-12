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

// --------------------------------------------------------------------------
//
// Library includes
//

namespace gui {

  namespace core {

    namespace detail {

      template <std::size_t... Is>
      struct indices {};

      template <std::size_t N, std::size_t... Is>
      struct build_indices : build_indices < N - 1, N - 1, Is... > {};

      template <std::size_t... Is>
      struct build_indices<0, Is...> : indices < Is... >
      {};

    } // detail

  } // core

} //gui
