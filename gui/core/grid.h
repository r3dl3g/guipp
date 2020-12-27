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
 * @brief     C++ API: basic structures
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
#include <gui/core/rectangle.h>


namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<std::size_t X, std::size_t Y>
    struct grid {
      explicit grid (const rectangle& r);

      rectangle at (std::size_t x, std::size_t y) const;

      rectangle operator() (std::size_t x, std::size_t y) const;

    private:
      rectangle d;
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/grid.inl>
