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
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (std::initializer_list<layout_function> list)
      : super(list)
    {}

    template<orientation height, unsigned dim1, unsigned border, unsigned gap, unsigned sep, origin rows>
    void lineup_layout<height, dim1, border, gap, sep, rows>::layout (const core::rectangle& r) const {
      LogTrace << "lineup_layout::layout(" << r << ")";
      const auto& elements = super::get_elements();
      const std::size_t count = elements.size();
      const std::size_t sep_count = super::separator_count();
      const type border2 = (border * 2);
      const type offset_dim1 = super::get_dimension1(r.top_left());
      const type offset_dim2 = super::get_dimension2(r.top_left());
      const type space = super::get_dimension1(r.x2y2()) - offset_dim1 - border2;

      if (count) {
        if (space > 0) {
          const type dim2 = super::get_dimension2(r.x2y2()) - offset_dim2 - border2;
          const type offset = dim1 + gap;
          const type sep_offset = gap + sep;

          core::rectangle area = super::init_area(border, offset_dim1, offset_dim2,
                                                  super::make_size(dim1, dim2), r.size(),
                                                  gap, count, sep, sep_count);
          for (auto const& e : elements) {
            if (e.is_separator()) {
              e(super::get_sep_area(area, type(sep)));
              super::move_area(area, sep_offset);
            } else {
              e(area);
              super::move_area(area, offset);
            }
          }
        } else {
          for (auto const& e : elements) {
            e(core::rectangle::zero);
          }
        }
      }
    }

  } // namespace layout

} // namespace gui
