/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: tile view
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    template<typename T>
    void default_tile_item_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   bool selected,
                                   bool);

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             list_item_drawer<T> F = default_tile_item_drawer<T>>
      using simple_tile_data = simple_list_data<T, F>;

    // --------------------------------------------------------------------------
    template<orientation V>
    struct tile_list_traits : public orientation_traits<V> {
      typedef orientation_traits<V> super;
      typedef core::size size_type;
      typedef core::size::type dim_type;

      tile_list_traits (const size_type& item_size);

      dim_type get_invisible_size (const core::size& list_size, size_t count) const;

      dim_type get_offset_of_index(const core::size& list_size, int idx) const;

      int get_index_at_point (const core::size& list_size,
                              const core::point& pt,
                              dim_type scroll_pos,
                              size_t count) const;

      core::rectangle get_place_of_index (const core::size& list_size,
                                          int idx,
                                          dim_type scroll_pos) const;

      dim_type get_line_size () const;
      dim_type get_line_border () const;
      dim_type get_line_spacing () const;

      dim_type get_item_size () const;
      dim_type get_item_border () const;
      dim_type get_item_spacing () const;

      std::size_t get_items_per_line (const core::size& list_size) const;
      std::size_t get_line_count (size_t count, const core::size& list_size) const;

      core::size item_size;
      core::size border;
      core::size spacing;
    };

    // --------------------------------------------------------------------------
    template<orientation V>
    class basic_tile_view : public basic_list<V, tile_list_traits<V>> {
    public:
      typedef basic_list<V, tile_list_traits<V>> super;

      basic_tile_view (const core::size& item_size = { 20, 20 },
                       os::color background = color::white,
                       bool grab_focus = true);

      basic_tile_view (const basic_tile_view& rhs);

      basic_tile_view (basic_tile_view&& rhs);

      void set_border (const core::size& sz);
      void set_spacing (const core::size& sz);
      
      const core::size& get_border () const;
      const core::size& get_spacing () const;

      std::size_t get_line_count () const;

      void paint (const draw::graphics& graph);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void handle_direction_key (os::key_symbol key);

      core::rectangle get_full_place_of_index (int idx);

      void init ();

    };

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void basic_tile_view<orientation::horizontal>::handle_direction_key (os::key_symbol key);

    template<>
    GUIPP_CTRL_EXPORT void basic_tile_view<orientation::vertical>::handle_direction_key (os::key_symbol key);

    // --------------------------------------------------------------------------
    typedef basic_tile_view<orientation::horizontal> horizontal_tile_view;
    typedef basic_tile_view<orientation::vertical> vertical_tile_view;
    typedef vertical_tile_view tile_view;

  } // ctrl

} // gui

#include <gui/ctrl/tile_view.inl>
