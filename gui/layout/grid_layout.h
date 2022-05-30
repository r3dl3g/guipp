/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     layout to position windows in a grid
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
#include <gui/layout/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    /** -------------------------------------------------------------------------
     * Linup all added elements with the same width and hight
     * from left to right, from top to down
     * ----------------------------------------------------------------------- */
    template<unsigned W,      /// Width of all added elements
             unsigned H,      /// Height of all added elements
             unsigned B = 0,  /// Border on all sides around the elements
             unsigned G = 0   /// Gap between the elements
             >
    class grid_lineup : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      static constexpr unsigned width = W;
      static constexpr unsigned height = H;
      static constexpr unsigned border = B;
      static constexpr unsigned gap = G;

      grid_lineup () = default;
      grid_lineup (const std::vector<layout_function>& list);

      void layout (const core::rectangle&) const;
    };

    /** -------------------------------------------------------------------------
     * Just a helper for templates
     * ----------------------------------------------------------------------- */
    template<unsigned W, unsigned H, unsigned B, unsigned G>
    struct is_layout<grid_lineup<W, H, B, G>> {
      enum {
        value = true
      };
    };

    /** -------------------------------------------------------------------------
     * Linup all added elements with a maximum number of columns
     * with the same width and hight from left to right, from top to down
     * ----------------------------------------------------------------------- */
    template<unsigned W,      /// Width of all added elements
             unsigned H,      /// Height of all added elements
             unsigned C,      /// Number of maximum columns
             unsigned B = 0,  /// Border on all sides around the elements
             unsigned G = 0   /// Gap between the elements
             >
    class fixed_grid_lineup : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      static constexpr unsigned width = W;
      static constexpr unsigned height = H;
      static constexpr unsigned columns = C;
      static constexpr unsigned border = B;
      static constexpr unsigned gap = G;

      fixed_grid_lineup () = default;
      fixed_grid_lineup (const std::vector<layout_function>& list);

      void layout (const core::rectangle&) const;
    };

    /** -------------------------------------------------------------------------
     * Just a helper for templates
     * ----------------------------------------------------------------------- */
    template<unsigned W, unsigned H, unsigned C, unsigned B, unsigned G>
    struct is_layout<fixed_grid_lineup<W, H, C, B, G>> {
      enum {
        value = true
      };
    };

    /** -------------------------------------------------------------------------
     * Linup all added elements with a maximum number of columns and rows
     * and adapth the width and hight to match the available space
     * ----------------------------------------------------------------------- */
    template<unsigned C,      /// Number of maximum columns
             unsigned R,      /// Number of maximum rows
             unsigned B = 0,  /// Border on all sides around the elements
             unsigned G = 0   /// Gap between the elements
             >
    class grid_adaption : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      static constexpr unsigned columns = C;
      static constexpr unsigned rows = R;
      static constexpr unsigned border = B;
      static constexpr unsigned gap = G;

      grid_adaption () = default;
      grid_adaption (const std::vector<layout_function>& list);

      void layout (const core::rectangle&) const;
    };

    /** -------------------------------------------------------------------------
     * Just a helper for templates
     * ----------------------------------------------------------------------- */
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
