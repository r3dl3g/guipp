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

    inline void standard_layout::layout (const core::rectangle&)
    {}

    // --------------------------------------------------------------------------
    namespace detail {

      template<orientation_t O>
      inline orientation_layout<O>::orientation_layout (std::initializer_list<layout_function> list)
        : super(list)
      {}

      template<orientation_t O>
      inline std::size_t orientation_layout<O>::separator_count () const {
        return std::count_if(elements.begin(), elements.end(), [] (const layout_element& l) {
          return l.is_separator();
        });
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation_t::horizontal>::get_dimension1 (const core::point& pt) const -> type {
        return pt.x();
      }

      template<>
      inline auto orientation_layout<orientation_t::horizontal>::get_dimension2 (const core::point& pt) const -> type {
        return pt.y();
      }

      template<>
      inline core::size orientation_layout<orientation_t::horizontal>::make_size (type dim1, type dim2) const {
        return core::size(dim1, dim2);
      }

      template<>
      inline core::rectangle orientation_layout<orientation_t::horizontal>::get_sep_area (const core::rectangle& area, type s) const {
        return area.with_width(s);
      }

      template<>
      inline void orientation_layout<orientation_t::horizontal>::move_area (core::rectangle& area, type offs) const {
        area.move_x(offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation_t::vertical>::get_dimension1 (const core::point& pt) const -> type {
        return pt.y();
      }

      template<>
      inline auto orientation_layout<orientation_t::vertical>::get_dimension2 (const core::point& pt) const -> type {
        return pt.x();
      }

      template<>
      inline core::size orientation_layout<orientation_t::vertical>::make_size (type dim1, type dim2) const {
        return core::size(dim2, dim1);
      }

      template<>
      inline core::rectangle orientation_layout<orientation_t::vertical>::get_sep_area (const core::rectangle& area, type s) const {
        return area.with_height(s);
      }

      template<>
      inline void orientation_layout<orientation_t::vertical>::move_area (core::rectangle& area, type offs) const {
        area.move_y(offs);
      }

      // --------------------------------------------------------------------------
      template<orientation_t H, origin_t R>
      inline origin_layout<H, R>::origin_layout (std::initializer_list<layout_function> list)
        : super(list)
      {}

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::start>::init_area (type border, type dim1, type dim2,
                                                                                               const core::size& is,
                                                                                               const core::size&,
                                                                                               int,
                                                                                               std::size_t,
                                                                                               int,
                                                                                               std::size_t) const {
        return core::rectangle(core::point(border + dim1, border + dim2), is);
      }

      template<>
      inline void origin_layout<orientation_t::horizontal, origin_t::start>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::start>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::start>::init_area (type border, type dim1, type dim2,
                                                                                             const core::size& is,
                                                                                             const core::size&,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) const {
        return core::rectangle(core::point(border + dim2, border + dim1), is);
      }

      template<>
      inline void origin_layout<orientation_t::vertical, origin_t::start>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::start>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::end>::init_area (type border, type dim1, type dim2,
                                                                                             const core::size& is,
                                                                                             const core::size& sz,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) const {
        return core::rectangle(core::point(dim1 + sz.width() - is.width() - border, dim2 + border), is);
      }

      template<>
      inline void origin_layout<orientation_t::horizontal, origin_t::end>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::end>::get_sep_area (const core::rectangle& r, type s) const {
        return core::rectangle(r.x2() - s, r.y(), s, r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::end>::init_area (type border, type dim1, type dim2,
                                                                                           const core::size& is,
                                                                                           const core::size& sz,
                                                                                           int,
                                                                                           std::size_t,
                                                                                           int,
                                                                                           std::size_t) const {
        return core::rectangle(core::point(dim2 + border, dim1 + sz.height() - is.height() - border), is);
      }

      template<>
      inline void origin_layout<orientation_t::vertical, origin_t::end>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::end>::get_sep_area (const core::rectangle& r, type s) const {
        return core::rectangle(r.x(), r.y2() - s, r.width(), s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::center>::init_area (type border, type dim1, type dim2,
                                                                                                const core::size& is,
                                                                                                const core::size& sz,
                                                                                                int gap,
                                                                                                std::size_t count,
                                                                                                int sep,
                                                                                                std::size_t sep_count) const {
        const type space = (is.width() * (count - sep_count) + (count - 1) * gap + sep_count * sep);
        return core::rectangle(core::point(dim1 + (sz.width() - space) / 2, dim2 + border), is);
      }

      template<>
      inline void origin_layout<orientation_t::horizontal, origin_t::center>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::horizontal, origin_t::center>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::center>::init_area (type border, type dim1, type dim2,
                                                                                              const core::size& is,
                                                                                              const core::size& sz,
                                                                                              int gap,
                                                                                              std::size_t count,
                                                                                              int sep,
                                                                                              std::size_t sep_count) const {
        const type space = (is.height() * (count - sep_count) + (count - 1) * gap + sep_count * sep);
        return core::rectangle(core::point(dim2 + border, dim1 + (sz.height() - space) / 2), is);
      }

      template<>
      inline void origin_layout<orientation_t::vertical, origin_t::center>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation_t::vertical, origin_t::center>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

    } // namespace detail

  } // namespace layout

} // namespace gui
