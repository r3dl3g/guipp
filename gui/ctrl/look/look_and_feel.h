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
 * @brief     look and feel enum
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#if defined(GUIPP_LOOK_AND_FEEL_APPLE)
      look_and_feel_t::osx;
#elif defined(GUIPP_LOOK_AND_FEEL_WIN32)
      look_and_feel_t::w10;
#elif defined(GUIPP_LOOK_AND_FEEL_UNIX)
      look_and_feel_t::metal;
#elif defined(GUIPP_LOOK_AND_FEEL_W95)
      look_and_feel_t::w95;
#elif defined(__APPLE__)
      look_and_feel_t::osx;
#elif defined(WIN32)
      look_and_feel_t::w10;
#elif defined(unix)
      look_and_feel_t::metal;
#else
      look_and_feel_t::w95;
#endif

  }

} // namespace gui
