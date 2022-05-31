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
 * @brief     layout to place windows with a specific weight inside the parent
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace layout {

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    inline weighted_layout<H, B, G, R>::weighted_layout (const std::vector<weight_element>& list)
      : elements(list)
    {}

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    void weighted_layout<H, B, G, R>::add (const weight_element& e) {
      elements.emplace_back(e);
    }

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    void weighted_layout<H, B, G, R>::add (weight_element&& e) {
      elements.emplace_back(std::move(e));
    }

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    void weighted_layout<H, B, G, R>::add (const std::vector<weight_element>& list) {
      elements.insert(elements.end(), list.begin(), list.end());
    }

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    void weighted_layout<H, B, G, R>::remove_all () {
      elements.clear();
    }

    template<orientation_t H, unsigned B, unsigned G, origin_t R>
    void weighted_layout<H, B, G, R>::layout (const core::rectangle& r) const {
      logging::trace() << "weighted_layout::layout(" << r << ")";

      typedef detail::origin_layout<H, R> layout;

      const std::size_t count = elements.size();
      const type border2 = (border_width * 2);
      const type offset_dim1 = layout::get_dimension1(r.top_left());
      const type offset_dim2 = layout::get_dimension2(r.top_left());
      type space = layout::get_dimension1(r.x2y2()) - offset_dim1 - border2 - ((elements.size() - 1) * gap);

      if (space > 0) {
        float weights = 0.0F;
        for (auto const& e : elements) {
          space -= e.fix;
          weights += e.weight;
        }
        if (space < 0) {
          space = 0;
        }

        const type dim2 = layout::get_dimension2(r.x2y2()) - offset_dim2 - border2;

        core::rectangle area;
        for (auto const& e : elements) {
          const type dim1 = e.fix + static_cast<type>(space * e.weight / weights);
          const type offset = dim1 + gap;
          if (area.empty()) {
            area = layout::init_area(border_width, offset_dim1, offset_dim2,
                                     layout::make_size(dim1, dim2),
                                     r.size(), gap, count, 0, 0);
          } else {
            area.set_size(layout::make_size(dim1, dim2));
          }
          e.fkt(area);
          layout::move_area(area, offset);
        }
      } else {
        for (auto const& e : elements) {
          e.fkt(core::rectangle::zero);
        }
      }
    }

  } // namespace layout

} // namespace gui
