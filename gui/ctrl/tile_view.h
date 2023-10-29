/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     tile view control
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
    template<typename T>
    void default_tile_item_drawer (const T& t,
                                   draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   item_state state);


    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct tile_list_traits : public uniform_list_traits<V> {
      typedef uniform_list_traits<V> super;
      typedef core::size size_type;
      typedef core::size::type dim_type;

      explicit tile_list_traits (const size_type& item_size);

      dim_type get_offset_of_index(const core::size& list_size, int idx) const;

      int get_index_at_point (const core::rectangle& list_size,
                              const core::point& pt,
                              const core::point& scroll_pos,
                              size_t count) const;

      core::rectangle get_geometry_of_index (const core::rectangle& list_size,
                                             int idx,
                                             const core::point& scroll_pos) const;

      dim_type get_line_size () const;
      dim_type get_line_border () const;
      dim_type get_line_spacing () const;

      dim_type get_item_dimension () const;
      dim_type get_item_border () const;
      dim_type get_item_spacing () const;

      std::size_t get_items_per_line (const core::size& list_size) const;
      std::size_t get_line_count (size_t count, const core::size& list_size) const;

      int get_direction_step (os::key_symbol key, const core::size& list_size) const;

      core::size item_size;
      core::size border;
      core::size spacing;
    };

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT int tile_list_traits<orientation_t::horizontal>::get_direction_step (os::key_symbol, const core::size&) const;

    template<>
    GUIPP_CTRL_EXPORT int tile_list_traits<orientation_t::vertical>::get_direction_step (os::key_symbol, const core::size&) const;

    // --------------------------------------------------------------------------
    template<orientation_t V, typename S = core::selector::single>
    class basic_tile_view : public uniform_list<V, tile_list_traits<V>, S> {
    public:
      typedef uniform_list<V, tile_list_traits<V>, S> super;

      explicit basic_tile_view (const core::size& item_size = { 20, 20 },
                                os::color background = color::white,
                                bool grab_focus = true);

      basic_tile_view (const basic_tile_view& rhs);

      basic_tile_view (basic_tile_view&& rhs) noexcept ;

      void set_border (const core::size& sz);
      void set_spacing (const core::size& sz);
      
      const core::size& get_border () const;
      const core::size& get_spacing () const;

      std::size_t get_line_count () const;

      core::rectangle get_virtual_geometry (const core::rectangle&) const;
      core::size get_scroll_steps () const;

      void paint (draw::graphics& graph);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void handle_direction_key (os::key_symbol key);

      core::rectangle get_full_place_of_index (int idx);

      void init ();

    };

    // --------------------------------------------------------------------------
    template<typename S>
    using horizontal_tile_view_t = basic_tile_view<orientation_t::horizontal, S>;

    template<typename S>
    using vertical_tile_view_t = basic_tile_view<orientation_t::vertical, S>;

    template<typename S>
    using horizontal_scrollable_tile_view_t = virtual_view<horizontal_tile_view_t<S>>;

    template<typename S>
    using vertical_scrollable_tile_view_t = virtual_view<vertical_tile_view_t<S>>;

    template<typename S>
    using tile_view_s = vertical_scrollable_tile_view_t<S>;

    // --------------------------------------------------------------------------
    using horizontal_tile_view = horizontal_tile_view_t<core::selector::single>;
    using vertical_tile_view = vertical_tile_view_t<core::selector::single>;

    using horizontal_scrollable_tile_view = horizontal_scrollable_tile_view_t<core::selector::single>;
    using vertical_scrollable_tile_view = vertical_scrollable_tile_view_t<core::selector::single>;

    using tile_view = vertical_scrollable_tile_view;

    // --------------------------------------------------------------------------
    using horizontal_multi_tile_view = horizontal_tile_view_t<core::selector::multi>;
    using vertical_multi_tile_view = vertical_tile_view_t<core::selector::multi>;

    using horizontal_scrollable_multi_tile_view = horizontal_scrollable_tile_view_t<core::selector::multi>;
    using vertical_scrollable_multi_tile_view = vertical_scrollable_tile_view_t<core::selector::multi>;

    using multi_tile_view = vertical_scrollable_multi_tile_view;

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/tile_view.inl>
