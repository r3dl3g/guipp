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

namespace gui {

  namespace core {

    // --------------------------------------------------------------------------
    template<std::size_t X, std::size_t Y>
    grid<X, Y>::grid (const rectangle& r)
      : d(r.x(), r.y(), r.width() / X, r.height() / Y)
    {}

    template<std::size_t X, std::size_t Y>
    rectangle grid<X, Y>::at (std::size_t x, std::size_t y) const {
      if (x >= X) {
        throw std::out_of_range("grid<X, Y> range_check: x >= X");
      }
      if (y >= Y) {
        throw std::out_of_range("grid<X, Y> range_check: y >= Y");
      }
      return operator()(x, y);
    }

    template<std::size_t X, std::size_t Y>
    rectangle grid<X, Y>::operator() (std::size_t x, std::size_t y) const {
      return { d.position() + size{d.width() * x, d.height() * y}, d.size() };
    }

    template<std::size_t X, std::size_t Y>
    void grid<X, Y>::for_each (std::function<void(std::size_t, std::size_t, rectangle)> fn) {
      for (std::size_t y = 0; y < Y; ++y) {
        for (std::size_t x = 0; x < X; ++x) {
          fn(x, y, operator()(x, y));
        }
      }
    }

  } // namespace core

} // namespace gui
