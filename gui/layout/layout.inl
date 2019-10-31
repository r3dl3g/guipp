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

    inline standard_layout::standard_layout (win::container*)
    {}

    // --------------------------------------------------------------------------
    inline layout_base::layout_base (win::container* m)
      : main(m)
    {}

    inline layout_base::layout_base (win::container* m, const layout_base&)
      : main(m)
    {}

    inline layout_base::layout_base (win::container* m, layout_base&&)
      : main(m)
    {}

    // --------------------------------------------------------------------------
    namespace detail {

      template<orientation O>
      inline orientation_layout<O>::orientation_layout (win::container* m)
        : super(m)
      {}

      template<orientation O>
      inline orientation_layout<O>::orientation_layout (win::container* m, const orientation_layout& rhs)
        : super(m, rhs)
      {}

      template<orientation O>
      inline orientation_layout<O>::orientation_layout (win::container* m, orientation_layout&& rhs)
        : super(m, std::move(rhs))
      {}

      template<orientation O>
      inline void orientation_layout<O>::add_separator (const win::window* w) {
        separators.push_back(w);
      }

      template<orientation O>
      inline bool orientation_layout<O>::is_separator (const win::window* w) const {
        separator_list::const_iterator end = separators.end();
        return std::find(separators.begin(), end, w) != end;
      }
      template<orientation O>

      inline std::size_t orientation_layout<O>::separator_count () const {
        return separators.size();
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation::horizontal>::get_dimension1 (const core::rectangle& sz) -> type {
        return sz.x2();
      }

      template<>
      inline auto orientation_layout<orientation::horizontal>::get_dimension2 (const core::rectangle& sz) -> type {
        return sz.y2();
      }

      template<>
      inline core::size orientation_layout<orientation::horizontal>::make_size (type dim1, type dim2) {
        return core::size(dim1, dim2);
      }

      template<>
      inline core::rectangle orientation_layout<orientation::horizontal>::get_sep_area (const core::rectangle& area, type s) {
        return area.with_width(s);
      }

      template<>
      inline void orientation_layout<orientation::horizontal>::move_area (core::rectangle& area, type offs) {
        area.move_x(offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation::vertical>::get_dimension1 (const core::rectangle& sz) -> type {
        return sz.y2();
      }

      template<>
      inline auto orientation_layout<orientation::vertical>::get_dimension2 (const core::rectangle& sz) -> type {
        return sz.x2();
      }

      template<>
      inline core::size orientation_layout<orientation::vertical>::make_size (type dim1, type dim2) {
        return core::size(dim2, dim1);
      }

      template<>
      inline core::rectangle orientation_layout<orientation::vertical>::get_sep_area (const core::rectangle& area, type s) {
        return area.with_height(s);
      }

      template<>
      inline void orientation_layout<orientation::vertical>::move_area (core::rectangle& area, type offs) {
        area.move_y(offs);
      }

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      inline origin_layout<H, R>::origin_layout (win::container* m)
        : super(m)
      {}

      template<orientation H, origin R>
      inline origin_layout<H, R>::origin_layout (win::container* m, const origin_layout& rhs)
        : super(m, rhs)
      {}

      template<orientation H, origin R>
      inline origin_layout<H, R>::origin_layout (win::container* m, origin_layout&& rhs)
        : super(m, std::move(rhs))
      {}

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::start>::init_area (type border,
                                                                                               const core::size& is,
                                                                                               const core::size&,
                                                                                               int,
                                                                                               std::size_t,
                                                                                               int,
                                                                                               std::size_t) {
        return core::rectangle(core::point(border, border), is);
      }

      template<>
      inline void origin_layout<orientation::horizontal, origin::start>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::start>::get_sep_area (const core::rectangle& area, type s) {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::start>::init_area (type border,
                                                                                             const core::size& is,
                                                                                             const core::size&,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) {
        return core::rectangle(core::point(border, border), is);
      }

      template<>
      inline void origin_layout<orientation::vertical, origin::start>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::start>::get_sep_area (const core::rectangle& area, type s) {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::end>::init_area (type border,
                                                                                             const core::size& is,
                                                                                             const core::size& sz,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) {
        return core::rectangle(core::point(sz.width() - is.width() - border, border), is);
      }

      template<>
      inline void origin_layout<orientation::horizontal, origin::end>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::end>::get_sep_area (const core::rectangle& r, type s) {
        return core::rectangle(r.x2() - s, r.y(), s, r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::end>::init_area (
          type border, const core::size& is, const core::size& sz, int, std::size_t, int, std::size_t) {
        return core::rectangle(core::point(border, sz.height() - is.height() - border), is);
      }

      template<>
      inline void origin_layout<orientation::vertical, origin::end>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::end>::get_sep_area (const core::rectangle& r, type s) {
        return core::rectangle(r.x(), r.y2() - s, r.width(), s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::center>::init_area (
        type border, const core::size& is, const core::size& sz, int gap, std::size_t count, int sep, std::size_t sep_count) {
        const type space = (is.width() * (count - sep_count) + (count - 1) * gap + sep_count * sep);
        return core::rectangle(core::point((sz.width() - space) / 2, border), is);
      }

      template<>
      inline void origin_layout<orientation::horizontal, origin::center>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::center>::get_sep_area (const core::rectangle& area, type s) {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::center>::init_area (
        type border, const core::size& is, const core::size& sz, int gap, std::size_t count, int sep, std::size_t sep_count) {
        const type space = (is.height() * (count - sep_count) + (count - 1) * gap + sep_count * sep);
        return core::rectangle(core::point(border, (sz.height() - space) / 2), is);
      }

      template<>
      inline void origin_layout<orientation::vertical, origin::center>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::center>::get_sep_area (const core::rectangle& area, type s) {
        return super::get_sep_area(area, s);
      }

    } // namespace detail

  } // namespace layout

} // namespace gui
