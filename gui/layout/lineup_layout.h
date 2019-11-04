/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
    template<orientation H,
             unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    class lineup_layout : public detail::origin_layout<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      lineup_layout () = default;
      lineup_layout (std::initializer_list<layout_function> list);

      void layout (const core::rectangle& sz) const;
    };

    // --------------------------------------------------------------------------
    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    struct is_layout<lineup_layout<H, D, B, G, S, R>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    using horizontal_lineup = lineup_layout<orientation::horizontal, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    using vertical_lineup = lineup_layout<orientation::vertical, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<alignment a,
             unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2>
    using lineup = lineup_layout<alignment_orientation<a>::value, dim1, border, gap, sep, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/lineup_layout.inl>
