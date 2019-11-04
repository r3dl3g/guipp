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
#include <algorithm>

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
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = std::numeric_limits<unsigned>::max(),
             origin R = origin::start>
    class adaption_layout : public detail::origin_layout<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      adaption_layout () = default;
      adaption_layout (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;

    private:
      void init ();
    };

    template<orientation A, unsigned B, unsigned C, unsigned D, unsigned E, unsigned F, origin G>
    struct is_layout<adaption_layout<A, B, C, D, E, F, G>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = std::numeric_limits<unsigned>::max(),
             origin o = origin::start>
    using horizontal_adaption = adaption_layout<orientation::horizontal, border, gap, sep, min, max, o>;

    // --------------------------------------------------------------------------
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = std::numeric_limits<unsigned>::max(),
             origin o = origin::start>
    using vertical_adaption = adaption_layout<orientation::vertical, border, gap, sep, min, max, o>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/adaption_layout.inl>
