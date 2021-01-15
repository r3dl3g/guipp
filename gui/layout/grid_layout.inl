/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

  // --------------------------------------------------------------------------
  namespace layout {

    template<unsigned W, unsigned H, unsigned B, unsigned G>
    inline grid_lineup<W, H, B, G>::grid_lineup (std::initializer_list<layout_function> list)
      : super(list)
    {}

    template<unsigned W, unsigned H, unsigned B, unsigned G>
    void grid_lineup<W, H, B, G>::layout (const core::rectangle& r) const {
      clog::trace() << "grid_lineup::layout(" << r << ")";

      typedef core::rectangle::point_type ptype;
      typedef core::rectangle::size_type stype;

      const stype width = static_cast<stype>(W);
      const stype height = static_cast<stype>(H);
      const stype border = static_cast<stype>(B);
      const stype gap = static_cast<stype>(G);

      const auto& elements = super::get_elements();
      const type border2 = border * 2;
      const type xmax = r.width() - border2;
      const type ymax = r.height() - border2;

      if (elements.size()) {
        if ((xmax > border) && (ymax > border)) {
          const type xoffset = width + gap;
          const type yoffset = height + gap;
          core::rectangle area(border + r.x(), border + r.y(), width, height);
          for (auto const& e : elements) {
            e(area);
            area.move_x(xoffset);
            if (area.x2() > xmax) {
              area.move_to_x(border + r.x());
              area.move_y(yoffset);
            }
          }
        } else {
          for (auto const& e : elements) {
            e(core::rectangle::zero);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<unsigned W, unsigned H, unsigned C, unsigned B, unsigned G>
    inline fixed_grid_lineup<W, H, C, B, G>::fixed_grid_lineup (std::initializer_list<layout_function> list)
      : super(list)
    {}

    template<unsigned W, unsigned H, unsigned C, unsigned B, unsigned G>
    void fixed_grid_lineup<W, H, C, B, G>::layout (const core::rectangle& r) const {
      clog::trace() << "fixed_grid_lineup::layout(" << r << ")";

      typedef core::rectangle::point_type ptype;
      typedef core::rectangle::size_type stype;

      const stype width = static_cast<stype>(W);
      const stype height = static_cast<stype>(H);
      const stype border = static_cast<stype>(B);
      const stype gap = static_cast<stype>(G);

      const auto& elements = super::get_elements();
      const type border2 = border * 2;
      const type xmax = r.width() - border2;
      const type ymax = r.height() - border2;

      if (elements.size()) {
        if ((xmax > border) && (ymax > border)) {
          const type xoffset = width + gap;
          const type yoffset = height + gap;
          core::rectangle area(border + r.x(), border + r.y(), width, height);
          int column = 0;
          for (auto const& e : elements) {
            e(area);
            column++;
            if (column < columns) {
              area.move_x(xoffset);
            } else {
              column = 0;
              area.move_to_x(border + r.x());
              area.move_y(yoffset);
            }
          }
        } else {
          for (auto const& e : elements) {
            e(core::rectangle::zero);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<unsigned C, unsigned R, unsigned B, unsigned G>
    inline grid_adaption<C, R, B, G>::grid_adaption (std::initializer_list<layout_function> list)
      : super(list)
    {}

    template<unsigned columns, unsigned rows, unsigned border, unsigned gap>
    void grid_adaption<columns, rows, border, gap>::layout (const core::rectangle& r) const {
      clog::trace() << "grid_adaption::layout(" << r << ")";
      const auto& elements = super::get_elements();
      const type border2 = (border * 2);
      const type xspace = r.width() - border2;
      const type yspace = r.height() - border2;

      if (elements.size()) {
        if ((xspace > 0) && (yspace > 0)) {
          const type width = (xspace - (gap * (columns - 1))) / columns;
          const type height = (yspace - (gap * (rows - 1))) / rows;
          const type xoffset = width + gap;
          const type yoffset = height + gap;

          core::rectangle area(border + r.x(), border + r.y(), width, height);
          int column = 0;
          for (auto const& e : elements) {
            e(area);
            column++;
            if (column < columns) {
              area.move_x(xoffset);
            } else {
              column = 0;
              area.move_to_x(border + r.x());
              area.move_y(yoffset);
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
