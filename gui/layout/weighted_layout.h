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

    struct weight_element {

      weight_element (const layout_function& fkt, uint32_t fix)
        : fkt(fkt)
        , fix(fix)
        , weight(0.0F)
      {}

      weight_element (const layout_function& fkt, uint32_t fix, float weight)
        : fkt(fkt)
        , fix(fix)
        , weight(weight)
      {}

      layout_function fkt;
      uint32_t fix;
      float weight;
    };

    // --------------------------------------------------------------------------
    template<orientation_t H,
             unsigned B = 0,
             unsigned G = 0,
             origin_t R = origin_t::start>
    class weighted_layout {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      static constexpr unsigned border_width = B;
      static constexpr unsigned gap = G;

      weighted_layout () = default;
      weighted_layout (std::initializer_list<weight_element> list);

      void add (const weight_element& e);
      void add (weight_element&& e);

      void add (std::initializer_list<weight_element> list);

      void layout (const core::rectangle& r) const;

    private:
      std::vector<weight_element> elements;
    };

    // --------------------------------------------------------------------------
    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    struct is_layout<weighted_layout<H, B, G, R>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<unsigned B = 0, unsigned G = 0, origin_t R = origin_t::start>
    using horizontal_weighted = weighted_layout<orientation_t::horizontal, B, G, R>;

    // --------------------------------------------------------------------------
    template<unsigned B = 0, unsigned G = 0, origin_t R = origin_t::start>
    using vertical_weighted = weighted_layout<orientation_t::vertical, B, G, R>;

    // --------------------------------------------------------------------------
    template<alignment_t a, unsigned B = 0, unsigned G = 0>
    using weighted = weighted_layout<alignment_orientation<a>::value, B, G, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/weighted_layout.inl>
