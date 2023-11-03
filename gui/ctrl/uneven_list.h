/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     list control with entries uneven in size
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      template<orientation_t O>
      struct uneven_list_traits {
        typedef core::orientation_traits<O> otraits;
        typedef core::size::type size_type;
        typedef core::size::type dim_type;
        typedef dim_type (size_function)(int, const core::size&);
        typedef std::function<size_function> size_fn;

        uneven_list_traits ();

        dim_type get_offset_of_index (const core::size& list_size, int idx) const;

        int get_index_at_point (const core::rectangle& list_size,
                                const core::point& pt,
                                const core::point& scroll_pos,
                                size_t /*count*/) const;

        core::rectangle get_geometry_of_index (const core::rectangle& list_size,
                                              int idx,
                                              const core::point& scroll_pos) const;

        std::size_t get_items_per_page (const core::size& page_size, int idx) const;

        dim_type get_list_dimension (const list_base& list) const;

        dim_type get_line_size () const;
        dim_type get_item_dimension (int idx, const core::size&) const;

        int get_direction_step (os::key_symbol key, const core::size& list_size) const;

        size_fn size;
      };


    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O, typename S = core::selector::single>
    class uneven_list : public detail::selectable_list<detail::uneven_list_traits<O>, O, S> {
    public:
      typedef detail::selectable_list<detail::uneven_list_traits<O>, O, S> super;
      typedef detail::uneven_list_traits<O> traits_type;
      typedef typename traits_type::size_type size_type;
      typedef typename traits_type::dim_type dim_type;
      typedef typename traits_type::size_fn size_fn;
      typedef basic_scroll_bar<O> scroll_bar_type;

      uneven_list (os::color background = color::white,
                   bool grab_focus = true);

      explicit uneven_list (size_fn fn,
                            os::color background = color::white,
                            bool grab_focus = true);

      core::rectangle get_virtual_geometry (const core::rectangle&) const;
      core::size get_scroll_steps () const;

      void paint (draw::graphics& graph);

      void set_size_function (size_fn fn);

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    template<typename S>
    using horizontal_uneven_list_t = uneven_list<orientation_t::horizontal, S>;

    template<typename S>
    using vertical_uneven_list_t = uneven_list<orientation_t::vertical, S>;

    template<typename S>
    using horizontal_scrollable_uneven_list_t = virtual_view<horizontal_uneven_list_t<S>>;

    template<typename S>
    using vertical_scrollable_uneven_list_t = virtual_view<vertical_uneven_list_t<S>>;

    template<typename S>
    using uneven_list_s = vertical_scrollable_uneven_list_t<S>;

    // --------------------------------------------------------------------------
    using horizontal_uneven_list = horizontal_uneven_list_t<core::selector::single>;
    using vertical_uneven_list = vertical_uneven_list_t<core::selector::single>;

    using horizontal_scrollable_uneven_list = horizontal_scrollable_uneven_list_t<core::selector::single>;
    using vertical_scrollable_uneven_list = vertical_scrollable_uneven_list_t<core::selector::single>;

    // --------------------------------------------------------------------------
    using horizontal_multi_uneven_list = horizontal_uneven_list_t<core::selector::multi>;
    using vertical_multi_uneven_list = vertical_uneven_list_t<core::selector::multi>;

    using horizontal_scrollable_multi_uneven_list = horizontal_scrollable_uneven_list_t<core::selector::multi>;
    using vertical_scrollable_multi_uneven_list = vertical_scrollable_uneven_list_t<core::selector::multi>;

    using multi_uneven_list = vertical_scrollable_multi_uneven_list;

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/uneven_list.inl>
