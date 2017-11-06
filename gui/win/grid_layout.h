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

      void layout (const core::size& sz);

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

      void layout (const core::size& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    template<unsigned W, unsigned H, unsigned B, unsigned G>
    inline grid_lineup<W, H, B, G>::grid_lineup (win::container* m)
      : super(m) {
      init();
    }

    template<unsigned W, unsigned H, unsigned B, unsigned G>
    inline grid_lineup<W, H, B, G>::grid_lineup (win::container* m, const grid_lineup& rhs)
      : super(m, rhs) {
      init();
    }

    template<unsigned W, unsigned H, unsigned B, unsigned G>
    inline grid_lineup<W, H, B, G>::grid_lineup (win::container* m, grid_lineup&& rhs)
      : super(m, std::move(rhs)) {
      init();
    }

    template<unsigned width, unsigned height, unsigned border, unsigned gap>
    void grid_lineup<width, height, border, gap>::layout (const core::size& sz) {
      std::vector<win::window*> children = get_children();
      const type xmax = sz.width() - border;
      const type ymax = sz.height() - border;

      if (children.size()) {
        if ((xmax > border) && (ymax > border)) {
          const type xoffset = width + gap;
          const type yoffset = height + gap;
          core::rectangle area(border, border, width, height);
          for (win::window* win : children) {
            place_child(win, area);
            area.move_x(xoffset);
            if (area.x2() > xmax) {
              area.move_to_x(border);
              area.move_y(yoffset);
            }
          }
        } else {
          hide_children(children);
        }
      }
      update();
    }

    template<unsigned W, unsigned H, unsigned B, unsigned G>
    inline void grid_lineup<W, H, B, G>::init () {
      super::init(core::bind_method(this, &grid_lineup::layout), [&] () {
        layout(get_main_size());
      });
    }

    // --------------------------------------------------------------------------
    template<unsigned C, unsigned R, unsigned B, unsigned G>
    inline grid_adaption<C, R, B, G>::grid_adaption (win::container* m)
      : super(m) {
      init();
    }

    template<unsigned C, unsigned R, unsigned B, unsigned G>
    inline grid_adaption<C, R, B, G>::grid_adaption (win::container* m, const grid_adaption& rhs)
      : super(m, rhs) {
      init();
    }

    template<unsigned C, unsigned R, unsigned B, unsigned G>
    inline grid_adaption<C, R, B, G>::grid_adaption (win::container* m, grid_adaption&& rhs)
      : super(m, std::move(rhs)) {
      init();
    }

    template<unsigned columns, unsigned rows, unsigned border, unsigned gap>
    void grid_adaption<columns, rows, border, gap>::layout (const core::size& sz) {
      std::vector<win::window*> children = get_children();
      const type border2 = (border * 2);
      const type xspace = sz.width() - border2;
      const type yspace = sz.height() - border2;

      if (children.size()) {
        if ((xspace > 0) && (yspace > 0)) {
          const type width = (xspace - (gap * (columns - 1))) / columns;
          const type height = (yspace - (gap * (rows - 1))) / rows;
          const type xoffset = width + gap;
          const type yoffset = height + gap;

          core::rectangle area(border, border, width, height);
          int column = 0;
          for (win::window* win : children) {
            place_child(win, area);
            column++;
            if (column < columns) {
              area.move_x(xoffset);
            } else {
              column = 0;
              area.move_to_x(border);
              area.move_y(yoffset);
            }
          }
        } else {
          hide_children(children);
        }
      }
      update();
    }

    template<unsigned C, unsigned R, unsigned B, unsigned G>
    inline void grid_adaption<C, R, B, G>::init () {
      super::init(core::bind_method(this, &grid_adaption::layout), [&] () {
        layout(get_main_size());
      });
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
