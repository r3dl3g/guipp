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
 * @brief     calculator for grad aligment
 *
 * @license   MIT license. See accompanying file LICENSE.
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

      void for_each (std::function<void(std::size_t, std::size_t, rectangle)> fn);

      template<typename I = int>
      void for_each (std::function<void(I, rectangle)> fn) {
        for (std::size_t y = 0; y < Y; ++y) {
          const std::size_t dy = y * X;
          for (std::size_t x = 0; x < X; ++x) {
            fn(I(x + dy), operator()(x, y));
          }
        }
      }

    private:
      rectangle d;
    };

    // --------------------------------------------------------------------------

  } // namespace core

} // namespace gui

#include <gui/core/grid.inl>
