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


namespace gui {

  namespace layout {

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m)
      : super(m) {
      init();
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m, const lineup_layout& rhs)
      : super(m, rhs) {
      init();
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m, lineup_layout&& rhs)
      : super(m, std::move(rhs)) {
      init();
    }

    template<orientation height, unsigned dim1, unsigned border, unsigned gap, unsigned sep, origin rows>
    void lineup_layout<height, dim1, border, gap, sep, rows>::layout (const core::size& sz) {
      std::vector<win::window*> children = super::get_children();
      const std::size_t count = children.size();
      const std::size_t sep_count = super::separator_count();
      const type border2 = (border * 2);
      const type space = super::get_dimension1(sz) - border2;

      if (count) {
        if (space > 0) {
          const type dim2 = super::get_dimension2(sz) - border2;
          const type offset = dim1 + gap;
          const type sep_offset = gap + sep;

          core::rectangle area = super::init_area(border, super::make_size(dim1, dim2), sz,
                                                  gap, count, sep, sep_count);
          for (win::window* win : children) {
            if (super::is_separator(win)) {
              super::place_child(win, super::get_sep_area(area, type(sep)));
              super::move_area(area, sep_offset);
            } else {
              super::place_child(win, area);
              super::move_area(area, offset);
            }
          }
        } else {
          super::hide_children(children);
        }
      }
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline void lineup_layout<H, D, B, G, S, R>::init () {
      super::init(core::bind_method(this, &lineup_layout::layout), [&] () {
        layout(super::get_main_size());
      });
    }

  } // namespace layout

} // namespace gui
