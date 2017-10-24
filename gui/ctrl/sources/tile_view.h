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
 * @brief     C++ API: list
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"


namespace gui {

  namespace win {

    template<typename T>
    void default_tile_item_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   bool selected,
                                   bool) {
      paint::text_item(g, place, background, convert_to_string<T>(t), selected, text_origin::center);
    }

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

      core::size_type get_item_size () const;
      core::size_type get_line_size () const;

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
      core::size_type get_item_border () const;
      core::size_type get_line_border () const;

      void init ();

      core::size item_size;
      core::size border;
      core::size spacing;
    };

    // --------------------------------------------------------------------------
    typedef basic_tile_view<orientation::horizontal> horizontal_tile_view;
    typedef basic_tile_view<orientation::vertical> vertical_tile_view;
    typedef vertical_tile_view tile_view;

    // --------------------------------------------------------------------------
    // inlines
    // --------------------------------------------------------------------------
    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (const core::size& item_size,
                                                os::color background,
                                                bool grab_focus)
      : super(background, grab_focus)
      , item_size(item_size)
    {
      init();
    }

    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (const basic_tile_view& rhs)
      : super(rhs)
      , item_size(rhs.item_size)
    {
      init();
    }

    template<orientation V>
    inline basic_tile_view<V>::basic_tile_view (basic_tile_view&& rhs)
      : super(std::move(rhs))
      , item_size(std::move(rhs.item_size))
    {
      init();
    }

    template<orientation V>
    inline void basic_tile_view<V>::create (const container& parent,
                                            const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      adjust_scroll_bar();
    }

    template<orientation V>
    template<typename T, list_item_drawer<T> F>
    inline void basic_tile_view<V>::create (const container& parent,
                                            const core::rectangle& place,
                                            const simple_list_data<T, F>& data) {
      super::create(clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation V>
    inline void basic_tile_view<V>::set_item_size (const core::size& sz) {
      item_size = sz;
      adjust_scroll_bar();
    }

    template<orientation V>
    inline void basic_tile_view<V>::set_item_size_and_background (const core::size& sz, os::color background) {
      set_background(background);
      set_item_size(sz);
    }

    template<orientation V>
    void basic_tile_view<V>::adjust_scroll_bar () {
      if (is_scroll_bar_enabled()) {
        const auto ipl = get_items_per_line();
        const auto lines = (ipl > 0 ? div_ceil(super::get_count(), ipl) : 1);
        scroll_bar::type visible = (get_line_size() * lines + get_line_border() * 2) - get_list_size();
        const bool show_scroll = (visible > zero);
        if (show_scroll) {
          create_scroll_bar();
        }
        scrollbar.set_max(std::max(visible, zero));
        scrollbar.set_visible(show_scroll);
      } else {
        super::redraw_later();
      }
    }

    template<orientation V>
    void basic_tile_view<V>::set_count (std::size_t count) {
      data.item_count = count;

      const auto ipl = get_items_per_line();
      const auto sz = get_list_size();
      const auto lines = (ipl > 0 ? div_ceil(count, ipl) : 1);
      scroll_bar::type visible = (get_line_size() * lines + get_line_border() * 2) - sz;

      scrollbar.set_min_max_step(zero, std::max(visible, zero), sz);

      if (super::is_valid()) {
        const bool show_scroll = (visible > zero) && super::is_scroll_bar_enabled();
        if (show_scroll) {
          create_scroll_bar();
        }
        scrollbar.set_visible(show_scroll);
        super::redraw_later();
      }
    }

    template<orientation V>
    template<typename F>
    inline void basic_tile_view<V>::set_data (const F& data) {
      super::set_drawer(data);
      set_count(data.size());
    }

    template<orientation V>
    core::rectangle basic_tile_view<V>::get_space () const {
      return super::client_area().shrinked(border);
    }

    template<orientation V>
    inline int basic_tile_view<V>::get_index_at_point (const core::point& pt) {
      if (super::client_area().is_inside(pt)) {
        const auto per_line = get_items_per_line();
        const auto line = static_cast<std::size_t>((get_dimension(pt) + get_scroll_pos() - get_line_border()) / get_line_size());
        const auto offs = static_cast<std::size_t>((get_other_dimension(pt) - get_item_border()) / get_item_size());
        const auto idx = (line * per_line) + offs;
        return static_cast<int>(idx < get_count() ? idx : -1);
      }
      return -1;
    }

    template<orientation V>
    core::rectangle basic_tile_view<V>::get_place_of_index (int idx) {
      const auto per_line = get_items_per_line();
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto last = line * per_line;
      const auto offs = idx - last;

      const auto ls = get_line_size();
      const auto is = get_item_size();

      core::rectangle place;
      set_dimension(place, ls * line - get_scroll_pos() + get_line_border(), ls);
      set_other_dimension(place, is * offs + get_item_border(), is);
      return place;
    }

    template<orientation V>
    void basic_tile_view<V>::set_selection (int sel, event_source notify) {
      const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
      if (data.selection != new_selection) {
        data.selection = new_selection;
        make_selection_visible();
        if (notify != event_source::logic) {
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          super::redraw_later();
        }
      }
    }

    template<orientation V>
    bool basic_tile_view<V>::try_to_select (int sel, event_source notify) {
      if ((sel >= 0) && (sel < super::get_count())) {
        set_selection(sel, notify);
        return true;
      }
      return false;
    }

    template<orientation V>
    void basic_tile_view<V>::make_selection_visible () {
      if (data.selection > -1) {
        const auto line = static_cast<std::size_t>(get_selection()) / get_items_per_line();

        const pos_t sel_pos = static_cast<pos_t>(get_line_size() * line) + get_line_border();
        const pos_t sz = get_list_size();

        if (sel_pos < get_scroll_pos()) {
          set_scroll_pos(sel_pos);
        } else if (sel_pos + get_item_size() - get_scroll_pos() > sz) {
          set_scroll_pos(sel_pos + get_item_size() - sz);
        }
      }
    }

    template<orientation V>
    void basic_tile_view<V>::set_scroll_pos (pos_t pos) {
      const pos_t max_delta = std::max<pos_t>(zero, (get_line_size() * get_line_count()) - get_list_size());
      auto value = std::min(std::max(zero, pos), max_delta);
      if (value != scrollbar.get_value()) {
        scrollbar.set_value(value, true);
      }
    }

    template<orientation V>
    void basic_tile_view<V>::paint (const draw::graphics& graph) {
      /*const */core::rectangle area(super::client_size());

      draw::brush back_brush(get_background());

      const auto last = super::get_count();
      const auto ls = get_line_size();
      if ((last < 1) || (ls < 1)) {
        graph.fill(draw::rectangle(area), back_brush);
      } else {
        const pos_t list_sz = get_list_size();
        const auto first_line = static_cast<std::size_t>((get_scroll_pos() - get_line_border()) / ls);

        auto idx = first_line;
        core::rectangle place = get_place_of_index(static_cast<int>(idx));

        const auto start = get_dimension(place.top_left());

        for (; (idx < last) && (get_dimension(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
          place = get_place_of_index(static_cast<int>(idx + 1));
        }

        for (; (get_dimension(place.top_left()) < list_sz); ++idx) {
          graph.fill(draw::rectangle(place), back_brush);
          place = get_place_of_index(static_cast<int>(idx + 1));
        }

        const auto width = get_item_size() * get_items_per_line() + get_item_border();
        const auto max_width = get_other_dimension(area.bottom_right());
        if (max_width > width) {
          core::rectangle space = area;
          set_other_dimension(space, width, max_width - width);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (get_item_border() > 0) {
          core::rectangle space = area;
          set_other_dimension(space, get_dimension(area.top_left()), get_item_border());
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (start > 0) {
          core::rectangle space = area;
          set_dimension(space, get_other_dimension(area.top_left()), start);
          graph.fill(draw::rectangle(space), back_brush);
        }
      }

      if (super::has_focus()) {
        draw::frame::dots(graph, area);
      }

    }

    template<orientation V>
    inline void basic_tile_view<V>::handle_wheel (const pos_t delta, const core::point&) {
      set_scroll_pos(get_scroll_pos() - get_line_size() * delta);
      data.moved = true;
    }

    template<orientation V>
    void basic_tile_view<V>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = super::client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (super::get_last_mouse_point() != core::point::undefined) {
          pos_t delta = get_dimension(super::get_last_mouse_point()) - get_dimension(pt);
          set_scroll_pos(get_scroll_pos() + delta);
          data.moved = true;
        }
        data.last_mouse_point = pt;
      }
      else {
        set_hilite(get_index_at_point(pt));
      }
    }

    template<orientation V>
    void basic_tile_view<V>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
        const int new_selection = get_index_at_point(pt);
        if (new_selection != super::get_selection()) {
          set_selection(new_selection, event_source::mouse);
        }
        else if (control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
        super::redraw_later();
      }
      data.last_mouse_point = core::point::undefined;
    }

    template<orientation V>
    inline std::size_t basic_tile_view<V>::get_line_count () const {
      return div_ceil(super::get_count(), get_items_per_line());
    }

    template<orientation V>
    void basic_tile_view<V>::handle_key (os::key_state,
                                         os::key_symbol key,
                                         const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case keys::page_up:
      case keys::numpad::page_up:
        try_to_select(super::get_selection() - static_cast<int>(get_list_size() / get_line_size()), event_source::keyboard);
        break;
      case keys::page_down:
      case keys::numpad::page_down:
        try_to_select(super::get_selection() + static_cast<int>(get_list_size() / get_line_size()), event_source::keyboard);
        break;
      case keys::home:
      case keys::numpad::home:
        try_to_select(0, event_source::keyboard);
        break;
      case keys::end:
      case keys::numpad::end:
        try_to_select(static_cast<int>(super::get_count()) - 1,
          event_source::keyboard);
        break;
      case keys::enter:
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        break;
      }
    }

    template<orientation V>
    void basic_tile_view<V>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &basic_tile_view::paint)));
      super::register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &basic_tile_view::handle_left_btn_up));
      super::register_event_handler(REGISTER_FUNCTION, typename traits::wheel_event_type(this, &basic_tile_view::handle_wheel));
      super::register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &basic_tile_view::handle_key));
      super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &basic_tile_view::handle_mouse_move));
      super::register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size &) {
        if (scrollbar.is_valid()) {
          scrollbar.place(get_scroll_bar_area());
        }
        adjust_scroll_bar();
      }));
    }

    template<orientation V>
    void basic_tile_view<V>::set_border (const core::size& sz) {
      border = sz;
      adjust_scroll_bar();
    }
    
    template<orientation V>
    void basic_tile_view<V>::set_spacing (const core::size& sz) {
      spacing = sz;
      adjust_scroll_bar();
    }

    template<orientation V>
    const core::size& basic_tile_view<V>::get_border () const {
      return border;
    }

    template<orientation V>
    const core::size& basic_tile_view<V>::get_spacing () const {
      return spacing;
    }

    // --------------------------------------------------------------------------
    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_item_size () const;

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_item_size () const;

    template<>
    std::size_t basic_tile_view<orientation::horizontal>::get_items_per_line () const;

    template<>
    std::size_t basic_tile_view<orientation::vertical>::get_items_per_line () const;

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_line_size () const;

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_line_size () const;

    template<>
    void basic_tile_view<orientation::horizontal>::handle_direction_key (os::key_symbol key);

    template<>
    void basic_tile_view<orientation::vertical>::handle_direction_key (os::key_symbol key);

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_item_border () const;

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_item_border () const;

    template<>
    core::size_type basic_tile_view<orientation::horizontal>::get_line_border () const;

    template<>
    core::size_type basic_tile_view<orientation::vertical>::get_line_border () const;
    // --------------------------------------------------------------------------

  } // win

} // gui
