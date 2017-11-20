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

  namespace win {

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
    class basic_tile_view : public basic_list<V> {
    public:
      typedef basic_list<V> super;
      typedef typename super::pos_t pos_t;

      basic_tile_view (const core::size& item_size = { 20, 20 },
                       os::color background = color::white,
                       bool grab_focus = true);

      basic_tile_view (const basic_tile_view& rhs);

      basic_tile_view (basic_tile_view&& rhs);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T,
               list_item_drawer<T> F = default_tile_item_drawer<T> >
      void create (const container& parent,
                   const core::rectangle& place,
                   const simple_list_data<T, F>& data);

      void set_item_size (const core::size& item_size);
      void set_item_size_and_background (const core::size& item_size, os::color background);

      void set_border (const core::size& sz);
      void set_spacing (const core::size& sz);
      
      const core::size& get_border () const;
      const core::size& get_spacing () const;

      void adjust_scroll_bar ();
      void set_scroll_pos (pos_t pos);

      void set_count (std::size_t count);

      template<typename F>
      void set_data (const F& data);

      core::rectangle get_space () const;

      int get_index_at_point (const core::point& pt);
      core::rectangle get_place_of_index (int idx);

      std::size_t get_items_per_line () const;
      std::size_t get_line_count () const;

      core::size::type get_item_size () const;
      core::size::type get_line_size () const;

      bool try_to_select (int sel, event_source notify);
      void set_selection (int sel, event_source notify);
      void make_selection_visible ();

      void paint (const draw::graphics& graph);

      void handle_wheel (const pos_t delta, const core::point&);
      void handle_mouse_move (os::key_state keys, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void handle_direction_key (os::key_symbol key);
      core::rectangle get_full_place_of_index (int idx);

      core::size::type get_item_border () const;
      core::size::type get_line_border () const;

      core::size::type get_item_spacing () const;
      core::size::type get_line_spacing () const;

      void init ();

      core::size item_size;
      core::size border;
      core::size spacing;
    };

    // --------------------------------------------------------------------------
    typedef basic_tile_view<orientation::horizontal> horizontal_tile_view;
    typedef basic_tile_view<orientation::vertical> vertical_tile_view;
    typedef vertical_tile_view tile_view;

  } // win

} // gui

#include <gui/ctrl/tile_view.inl>
