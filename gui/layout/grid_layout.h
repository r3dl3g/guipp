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
    template<unsigned W, unsigned H, unsigned B = 0, unsigned G = 0>
    class grid_lineup : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      static constexpr unsigned width = W;
      static constexpr unsigned height = H;
      static constexpr unsigned border = B;
      static constexpr unsigned gap = G;

      grid_lineup () = default;
      grid_lineup (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;
    };

    // --------------------------------------------------------------------------
    template<unsigned W, unsigned H, unsigned B, unsigned G>
    struct is_layout<grid_lineup<W, H, B, G>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<unsigned C, unsigned R, unsigned B = 0, unsigned G = 0>
    class grid_adaption : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      static constexpr unsigned columns = C;
      static constexpr unsigned rows = R;
      static constexpr unsigned border = B;
      static constexpr unsigned gap = G;

      grid_adaption () = default;
      grid_adaption (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;
    };

    // --------------------------------------------------------------------------
    template<unsigned C, unsigned R, unsigned B, unsigned G>
    struct is_layout<grid_adaption<C, R, B, G>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/grid_layout.inl>
