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
#include <gui/win/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    template<unsigned width, unsigned height, unsigned border = 0, unsigned gap = 0>
    class grid_lineup : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_lineup (win::container* m);
      grid_lineup (win::container* m, const grid_lineup& rhs);
      grid_lineup (win::container* m, grid_lineup&& rhs);

      void layout (const core::rectangle& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    template<unsigned columns, unsigned rows, unsigned border = 0, unsigned gap = 0>
    class grid_adaption : protected layout_base {
    public:
      typedef core::size::type type;
      typedef layout_base super;

      grid_adaption (win::container* m);
      grid_adaption (win::container* m, const grid_adaption& rhs);
      grid_adaption (win::container* m, grid_adaption&& rhs);

      void layout (const core::rectangle& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/win/grid_layout.inl>
