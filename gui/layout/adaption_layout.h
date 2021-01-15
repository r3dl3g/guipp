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
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/layout.h>

namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    /** -------------------------------------------------------------------------
     * Linup all added elements with the same adapted width and hight
     * to fill up the available space
     * ----------------------------------------------------------------------- */
    template<orientation_t H,     /// Orientation vertical or horizontal
             unsigned border = 0, /// Border on all sides around the elements
             unsigned gap = 0,    /// Gap between the elements
             unsigned sep = 2,    /// Size of separator
             unsigned min = 0,    /// Minimum size of an element
             unsigned max = std::numeric_limits<unsigned>::max(), /// Maximum size of an element
             origin_t R = origin_t::start /// origin where the elements begin to line up
             >
    class adaption_layout : public detail::origin_layout<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      adaption_layout () = default;
      adaption_layout (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;
    };

    /** -------------------------------------------------------------------------
     * Just a helper for templates
     * ----------------------------------------------------------------------- */
    template<orientation_t A, unsigned B, unsigned C, unsigned D, unsigned E, unsigned F, origin_t G>
    struct is_layout<adaption_layout<A, B, C, D, E, F, G>> {
      enum {
        value = true
      };
    };

    /** -------------------------------------------------------------------------
     * Linup all added elements horizontal with the same adapted width and hight
     * to fill up the available space
     * @see adaption_layout
     * ----------------------------------------------------------------------- */
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = std::numeric_limits<unsigned>::max(),
             origin_t o = origin_t::start>
    using horizontal_adaption = adaption_layout<orientation_t::horizontal, border, gap, sep, min, max, o>;

    /** -------------------------------------------------------------------------
     * Linup all added elements vertical with the same adapted width and hight
     * to fill up the available space
     * @see adaption_layout
     * ----------------------------------------------------------------------- */
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = std::numeric_limits<unsigned>::max(),
             origin_t o = origin_t::start>
    using vertical_adaption = adaption_layout<orientation_t::vertical, border, gap, sep, min, max, o>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/adaption_layout.inl>
