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
// Library includes
//
#include <util/bind_method.h>


namespace gui {

  namespace layout {

    template<orientation H, unsigned B, unsigned G, unsigned S, unsigned I, unsigned A, origin O>
    inline adaption_layout<H, B, G, S, I, A, O>::adaption_layout (std::initializer_list<layout_function> list)
      : super(list) {
    }

    template<orientation height, unsigned border, unsigned gap, unsigned sep, unsigned min, unsigned max, origin o>
    void adaption_layout<height, border, gap, sep, min, max, o>::layout (const core::rectangle& r) const {
      LogTrace << "adaption_layout::layout(" << r << ")";
      const auto& elements = super::get_elements();
      const std::size_t count = elements.size();
      const std::size_t sep_count = super::separator_count();
      const type border2 = (border * 2);
      const type offset_dim1 = super::get_dimension1(r.top_left());
      const type offset_dim2 = super::get_dimension2(r.top_left());
      const type space = super::get_dimension1(r.bottom_right()) - offset_dim1 - border2 - sep_count * sep;

      if (count) {
        const type dim1 = std::max<type>(type(min), std::min<type>(type(max), (space - (gap * (count - 1))) / (count - sep_count)));
        if (dim1 >= 1) {
          const type dim2 = super::get_dimension2(r.bottom_right()) - offset_dim2 - border2;
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
