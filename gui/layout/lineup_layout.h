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

    /** -------------------------------------------------------------------------
     * Linup all added elements with a fix width and hight until the available
     * space is filled up.
     * ----------------------------------------------------------------------- */
    template<orientation_t H, /// Orientation vertical or horizontal
             unsigned D,      /// Fix dimension of all added elements in direction of orientation
             unsigned B = 0,  /// Border on all sides around the elements
             unsigned G = 0,  /// Gap between the elements
             unsigned S = 2,  /// Size of separator
             origin_t R = origin_t::start /// origin where the elements begin to line up
             >
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

    /** -------------------------------------------------------------------------
     * Just a helper for templates
     * ----------------------------------------------------------------------- */
    template<orientation_t H, unsigned D, unsigned B, unsigned G, unsigned S, origin_t R>
    struct is_layout<lineup_layout<H, D, B, G, S, R>> {
      enum {
        value = true
      };
    };

    /** -------------------------------------------------------------------------
     * Linup all added elements horizontal with a fix width and hight until the
     * available space is filled up.
     * @see lineup_layout
     * ----------------------------------------------------------------------- */
    template<unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2, origin_t R = origin_t::start>
    using horizontal_lineup = lineup_layout<orientation_t::horizontal, D, B, G, S, R>;

    /** -------------------------------------------------------------------------
     * Linup all added elements vertical with a fix width and hight until the
     * available space is filled up.
     * @see lineup_layout
     * ----------------------------------------------------------------------- */
    template<unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2, origin_t R = origin_t::start>
    using vertical_lineup = lineup_layout<orientation_t::vertical, D, B, G, S, R>;

    /** -------------------------------------------------------------------------
     * Linup all added elements with a fix width and hight until the available
     * space is filled up depending on given alignment_t.
     * @see lineup_layout
     * ----------------------------------------------------------------------- */
    template<alignment_t a, unsigned D, unsigned B = 0, unsigned G = 0, unsigned S = 2>
    using lineup = lineup_layout<alignment_orientation<a>::value, D, B, G, S, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/lineup_layout.inl>
