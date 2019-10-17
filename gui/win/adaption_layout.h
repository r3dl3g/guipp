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
#include <gui/win/layout.h>

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

      adaption_layout (win::container* m);
      adaption_layout (win::container* m, const adaption_layout& rhs);
      adaption_layout (win::container* m, adaption_layout&& rhs);

      void layout (const core::rectangle& sz);

    private:
      void init ();
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

#include <gui/win/adaption_layout.inl>
