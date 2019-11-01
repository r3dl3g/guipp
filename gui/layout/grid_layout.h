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
    template<unsigned width, unsigned height, unsigned border = 0, unsigned gap = 0>
    class grid_lineup : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_lineup () = default;
      grid_lineup (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;
    };

    // --------------------------------------------------------------------------
    template<unsigned columns, unsigned rows, unsigned border = 0, unsigned gap = 0>
    class grid_adaption : public layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_adaption () = default;
      grid_adaption (std::initializer_list<layout_function> list);

      void layout (const core::rectangle&) const;
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/grid_layout.inl>
