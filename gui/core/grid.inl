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
    template<std::size_t X, std::size_t Y, typename R>
    grid<X, Y, R>::grid (const rect& r)
      : d(r.x(), r.y(), r.width() / X, r.height() / Y)
    {}

    template<std::size_t X, std::size_t Y, typename R>
    auto grid<X, Y, R>::at (std::size_t x, std::size_t y) const -> rect {
      if (x >= X) {
        throw std::out_of_range("grid<X, Y, R> range_check: x >= X");
      }
      if (y >= Y) {
        throw std::out_of_range("grid<X, Y, R> range_check: y >= Y");
      }
      return operator()(x, y);
    }

    template<std::size_t X, std::size_t Y, typename R>
    auto grid<X, Y, R>::operator() (std::size_t x, std::size_t y) const -> rect {
      typedef typename rect::point_t::size_t size;
      typedef typename size::type sz_t;
      return { d.position() + size{static_cast<sz_t>(d.width() * x), static_cast<sz_t>(d.height() * y)}, d.size() };
    }

    template<std::size_t X, std::size_t Y, typename R>
    void grid<X, Y, R>::for_each (std::function<void(std::size_t, std::size_t, rect)> fn) {
      for (std::size_t y = 0; y < Y; ++y) {
        for (std::size_t x = 0; x < X; ++x) {
          fn(x, y, operator()(x, y));
        }
      }
    }

  } // namespace core

} // namespace gui
