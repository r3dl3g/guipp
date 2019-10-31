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
    inline layout_base::layout_base (std::initializer_list<layout_function> list) {
      add(list);
    }

    inline auto layout_base::get_elements () const -> const element_list& {
      return elements;
    }

    inline void layout_base::add (const layout_function& e, bool is_separator) {
      elements.emplace_back(layout_element(e, is_separator));
    }

    inline void layout_base::add (layout_function&& e, bool is_separator) {
      elements.emplace_back(layout_element(std::move(e), is_separator));
    }

    inline void layout_base::add (std::initializer_list<layout_function> list) {
      for (auto l : list) {
        add(l);
      }
    }

    inline void layout_base::add (std::initializer_list<win::window*> list) {
      for (win::window* w : list) {
        add(win(w));
      }
    }


    // --------------------------------------------------------------------------
    namespace detail {

      template<orientation O>
      inline orientation_layout<O>::orientation_layout (std::initializer_list<layout_function> list)
        : super(list)
      {}

      template<orientation O>
      inline std::size_t orientation_layout<O>::separator_count () const {
        return std::count_if(elements.begin(), elements.end(), [] (const layout_element& l) {
          return l.is_separator();
        });
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation::horizontal>::get_dimension1 (const core::point& pt) const -> type {
        return pt.x();
      }

      template<>
      inline auto orientation_layout<orientation::horizontal>::get_dimension2 (const core::point& pt) const -> type {
        return pt.y();
      }

      template<>
      inline core::size orientation_layout<orientation::horizontal>::make_size (type dim1, type dim2) const {
        return core::size(dim1, dim2);
      }

      template<>
      inline core::rectangle orientation_layout<orientation::horizontal>::get_sep_area (const core::rectangle& area, type s) const {
        return area.with_width(s);
      }

      template<>
      inline void orientation_layout<orientation::horizontal>::move_area (core::rectangle& area, type offs) const {
        area.move_x(offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline auto orientation_layout<orientation::vertical>::get_dimension1 (const core::point& pt) const -> type {
        return pt.y();
      }

      template<>
      inline auto orientation_layout<orientation::vertical>::get_dimension2 (const core::point& pt) const -> type {
        return pt.x();
      }

      template<>
      inline core::size orientation_layout<orientation::vertical>::make_size (type dim1, type dim2) const {
        return core::size(dim2, dim1);
      }

      template<>
      inline core::rectangle orientation_layout<orientation::vertical>::get_sep_area (const core::rectangle& area, type s) const {
        return area.with_height(s);
      }

      template<>
      inline void orientation_layout<orientation::vertical>::move_area (core::rectangle& area, type offs) const {
        area.move_y(offs);
      }

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      inline origin_layout<H, R>::origin_layout (std::initializer_list<layout_function> list)
        : super(list)
      {}

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::start>::init_area (type border, type dim1, type dim2,
                                                                                               const core::size& is,
                                                                                               const core::size&,
                                                                                               int,
                                                                                               std::size_t,
                                                                                               int,
                                                                                               std::size_t) const {
        return core::rectangle(core::point(border + dim1, border + dim2), is);
      }

      template<>
      inline void origin_layout<orientation::horizontal, origin::start>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::start>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::start>::init_area (type border, type dim1, type dim2,
                                                                                             const core::size& is,
                                                                                             const core::size&,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) const {
        return core::rectangle(core::point(border + dim2, border + dim1), is);
      }

      template<>
      inline void origin_layout<orientation::vertical, origin::start>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::start>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::end>::init_area (type border, type dim1, type dim2,
                                                                                             const core::size& is,
                                                                                             const core::size& sz,
                                                                                             int,
                                                                                             std::size_t,
                                                                                             int,
                                                                                             std::size_t) const {
        return core::rectangle(core::point(dim1 + sz.width() - is.width() - border, dim2 + border), is);
      }

      template<>
      inline void origin_layout<orientation::horizontal, origin::end>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::end>::get_sep_area (const core::rectangle& r, type s) const {
        return core::rectangle(r.x2() - s, r.y(), s, r.height());
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::end>::init_area (type border, type dim1, type dim2,
                                                                                           const core::size& is,
                                                                                           const core::size& sz,
                                                                                           int,
                                                                                           std::size_t,
                                                                                           int,
                                                                                           std::size_t) const {
        return core::rectangle(core::point(dim2 + border, dim1 + sz.height() - is.height() - border), is);
      }

      template<>
      inline void origin_layout<orientation::vertical, origin::end>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, -offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::end>::get_sep_area (const core::rectangle& r, type s) const {
        return core::rectangle(r.x(), r.y2() - s, r.width(), s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::center>::init_area (type border, type dim1, type dim2,
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
      inline void origin_layout<orientation::horizontal, origin::center>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::horizontal, origin::center>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::center>::init_area (type border, type dim1, type dim2,
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
      inline void origin_layout<orientation::vertical, origin::center>::move_area (core::rectangle& area, type offs) const {
        super::move_area(area, offs);
      }

      template<>
      inline core::rectangle origin_layout<orientation::vertical, origin::center>::get_sep_area (const core::rectangle& area, type s) const {
        return super::get_sep_area(area, s);
      }

    } // namespace detail

  } // namespace layout

} // namespace gui
