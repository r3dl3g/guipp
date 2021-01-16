/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: look and feel enum
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/bits.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    enum class look_and_feel_t {
      w95,
      w10,
      metal,
      osx
    };

    const look_and_feel_t system_look_and_feel =
#ifdef __APPLE__
      look_and_feel_t::osx;
#elif WIN32
      look_and_feel_t::w10;
#elif unix
      look_and_feel_t::metal;
#else
      look_and_feel_t::w95;
#endif

  }

} // namespace gui
