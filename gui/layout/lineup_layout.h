/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
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
#include <gui/layout/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation_t H,
             unsigned D,
             unsigned B = 0,
             unsigned G = 0,
             unsigned S = 2,
             origin_t R = origin_t::start>
    class lineup_layout : public detail::origin_layout<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      static constexpr unsigned dimension = D;
      static constexpr unsigned border_width = B;
      static constexpr unsigned gap = G;
      static constexpr unsigned separatior_width = S;

      lineup_layout () = default;
      lineup_layout (std::initializer_list<layout_function> list);

      void layout (const core::rectangle& r) const;
    };

    // --------------------------------------------------------------------------
    template<orientation_t H, unsigned D, unsigned B, unsigned G, unsigned S, origin_t R>
    struct is_layout<lineup_layout<H, D, B, G, S, R>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2, origin_t R = origin_t::start>
    using horizontal_lineup = lineup_layout<orientation_t::horizontal, D, B, G, S, R>;

    // --------------------------------------------------------------------------
    template<unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2, origin_t R = origin_t::start>
    using vertical_lineup = lineup_layout<orientation_t::vertical, D, B, G, S, R>;

    // --------------------------------------------------------------------------
    template<alignment_t a, unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2>
    using lineup = lineup_layout<alignment_orientation<a>::value, D, B, G, S, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/lineup_layout.inl>
