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


namespace gui {

  namespace win {

    template<typename T>
    inline void default_tile_item_drawer (const T& t,
                                          const draw::graphics& g,
                                          const core::rectangle& place,
                                          const draw::brush& background,
                                          bool selected,
                                          bool) {
      paint::text_item(g, place, background, convert_to_string<T>(t), selected, text_origin::center);
    }

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
      super::create(super::clazz::get(), parent, place);
      adjust_scroll_bar();
    }

    template<orientation V>
    template<typename T, list_item_drawer<T> F>
    inline void basic_tile_view<V>::create (const container& parent,
                                            const core::rectangle& place,
                                            const simple_list_data<T, F>& data) {
      super::create(super::clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation V>
    inline void basic_tile_view<V>::set_item_size (const core::size& sz) {
      item_size = sz;
      adjust_scroll_bar();
    }

    template<orientation V>
    inline void basic_tile_view<V>::set_item_size_and_background (const core::size& sz, os::color background) {
      super::set_background(background);
      set_item_size(sz);
    }

    template<orientation V>
    void basic_tile_view<V>::adjust_scroll_bar () {
      if (super::is_scroll_bar_enabled()) {
        const auto ipl = get_items_per_line();
        const auto lines = (ipl > 0 ? div_ceil(super::get_count(), ipl) : 1);
        scroll_bar::type visible = ((get_line_size() + get_line_spacing()) * lines + get_line_border() * 2) - super::get_list_size();
        const bool show_scroll = (visible > super::zero);
        if (show_scroll) {
          super::create_scroll_bar();
        }
        super::scrollbar.set_max(std::max(visible, super::zero));
        super::scrollbar.set_visible(show_scroll);
      } else {
        super::redraw_later();
      }
    }

    template<orientation V>
    void basic_tile_view<V>::set_count (std::size_t count) {
      super::data.item_count = count;

      const auto ipl = get_items_per_line();
      const auto sz = super::get_list_size();
      const auto lines = (ipl > 0 ? div_ceil(count, ipl) : 1);
      scroll_bar::type visible = ((get_line_size() + get_line_spacing()) * lines + get_line_border() * 2) - sz;

      super::scrollbar.set_min_max_step(super::zero, std::max(visible, super::zero), sz);

      if (super::is_valid()) {
        const bool show_scroll = (visible > super::zero) && super::is_scroll_bar_enabled();
        if (show_scroll) {
          super::create_scroll_bar();
        }
        super::scrollbar.set_visible(show_scroll);
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
        const int per_line = static_cast<int>(get_items_per_line());
        const auto line = static_cast<int>((traits::get(pt) + super::get_scroll_pos() - get_line_border() + get_line_spacing()) / (get_line_size() + get_line_spacing()));
        const auto offs = static_cast<int>((traits::get_other(pt) - get_item_border() + get_item_spacing()) / (get_item_size() + get_item_spacing()));
        const auto idx = (line * per_line) + offs;
        return (idx < super::get_count()) && get_place_of_index(idx).is_inside(pt) ? idx : -1;
      }
      return -1;
    }

    template<orientation V>
    core::rectangle basic_tile_view<V>::get_place_of_index (int idx) {
      const auto per_line = get_items_per_line();
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = get_line_size();
      const auto isz = get_item_size();

      core::rectangle place;
      traits::set(place, (lsz + get_line_spacing()) * line - super::get_scroll_pos() + get_line_border(), lsz);
      traits::set_other(place, (isz + get_item_spacing()) * offs + get_item_border(), isz);
      return place;
    }

    template<orientation V>
    core::rectangle basic_tile_view<V>::get_full_place_of_index (int idx) {
      const auto per_line = get_items_per_line();
      const auto line = per_line > 0 ? static_cast<std::size_t>(idx) / per_line : 0;
      const auto offs = idx - (line * per_line);

      const auto lsz = get_line_size() + get_line_spacing();
      const auto isz = get_item_size() + get_item_spacing();

      core::rectangle place;
      traits::set(place, lsz * line - super::get_scroll_pos() + get_line_border(), lsz);
      traits::set_other(place, isz * offs + get_item_border(), isz);
      return place;
    }

    template<orientation V>
    void basic_tile_view<V>::set_selection (int sel, event_source notify) {
      const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
      if (super::data.selection != new_selection) {
        super::data.selection = new_selection;
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
      if (super::data.selection > -1) {
        const int line = super::get_selection() / static_cast<int>(get_items_per_line());

        const auto sz = super::get_list_size();
        const auto ls = get_line_size() + get_line_spacing();
        const auto sel_pos = (ls * line) + get_line_border();
        const auto sc_pos = super::get_scroll_pos();

        if (sel_pos < sc_pos) {
          set_scroll_pos(sel_pos);
        } else if (sel_pos + ls - sc_pos > sz) {
          set_scroll_pos(sel_pos + ls - sz);
        }
      }
    }

    template<orientation V>
    void basic_tile_view<V>::set_scroll_pos (pos_t pos) {
      const pos_t max_delta = std::max<pos_t>(super::zero, ((get_line_size() + get_line_spacing()) * get_line_count() + get_line_border() * 2) - super::get_list_size());
      auto value = std::min(std::max(super::zero, pos), max_delta);
      if (value != super::scrollbar.get_value()) {
        super::scrollbar.set_value(value);
      }
    }

    template<orientation V>
    void basic_tile_view<V>::paint (const draw::graphics& graph) {
      const core::rectangle area(super::client_size());

      draw::brush back_brush(super::get_background());

      const auto last = super::get_count();
      const auto lsz = get_line_size();
      if ((last < 1) || (lsz < 1)) {
        graph.fill(draw::rectangle(area), back_brush);
      } else {
        const auto list_sz = super::get_list_size();
        const auto isp = get_item_spacing();
        const auto lsp = get_line_spacing();
        const auto scp = super::get_scroll_pos();
        const auto lb = get_line_border();
        const int per_line = static_cast<int>(get_items_per_line());

        const int first_line = static_cast<int>((scp - lb + lsp) / (lsz + lsp));

        int idx = first_line * per_line;
        core::rectangle place = get_place_of_index(idx);

        const auto start = traits::get(place.top_left());

        for (; (idx < last) && (traits::get(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
          if (isp > 0) {
            traits::set_other(place, traits::get_other(place.bottom_right()), isp);
            graph.fill(draw::rectangle(place), back_brush);
          }
          place = get_place_of_index(idx + 1);
        }

        const int last_line = div_ceil(idx, per_line);

        place = get_full_place_of_index(idx);
        for (; (traits::get(place.top_left()) < list_sz); ++idx) {
          graph.fill(draw::rectangle(place), back_brush);
          place = get_full_place_of_index(idx + 1);
        }

        const auto ib = get_item_border();
        const auto isz = get_item_size();
        if (lsp > 0) {
          const auto lw = (isz + isp) * per_line;
          for (int line = first_line; line < last_line; ++line) {
            traits::set(place, (lsz + lsp) * line - scp + lb + lsz, lsp);
            traits::set_other(place, ib, lw);
            graph.fill(draw::rectangle(place), back_brush);
          }
        }

        const auto width = (isz + isp) * per_line + ib;
        const auto max_width = traits::get_other(area.bottom_right());
        if (max_width > width) {
          core::rectangle space = area;
          traits::set_other(space, width, max_width - width);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (ib > 0) {
          core::rectangle space = area;
          traits::set_other(space, traits::get(area.top_left()), ib);
          graph.fill(draw::rectangle(space), back_brush);
        }

        if (start > 0) {
          core::rectangle space = area;
          traits::set(space, traits::get_other(area.top_left()), start);
          graph.fill(draw::rectangle(space), back_brush);
        }
      }

      if (super::has_focus()) {
        draw::frame::dots(graph, area);
      }

    }

    template<orientation V>
    inline void basic_tile_view<V>::handle_wheel (const pos_t delta, const core::point&) {
      set_scroll_pos(super::get_scroll_pos() - (get_line_size() + get_line_spacing()) * delta);
      super::get_state().set_moved(true);
    }

    template<orientation V>
    void basic_tile_view<V>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = super::client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (super::get_last_mouse_point() != core::point::undefined) {
          pos_t delta = traits::get(super::get_last_mouse_point()) - traits::get(pt);
          set_scroll_pos(super::get_scroll_pos() + delta);
          super::get_state().set_moved(true);
        }
        super::data.last_mouse_point = pt;
      } else {
        super::set_hilite(get_index_at_point(pt));
      }
    }

    template<orientation V>
    void basic_tile_view<V>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
        const int new_selection = get_index_at_point(pt);
        if (new_selection != super::get_selection()) {
          set_selection(new_selection, event_source::mouse);
        } else if (control_key_bit_mask::is_set(keys)) {
          super::clear_selection(event_source::mouse);
        }
        super::redraw_later();
      }
      super::data.last_mouse_point = core::point::undefined;
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
        try_to_select(super::get_selection() - static_cast<int>((super::get_list_size() + get_line_spacing()) / (get_line_size() + get_line_spacing())), event_source::keyboard);
        break;
      case keys::page_down:
      case keys::numpad::page_down:
        try_to_select(super::get_selection() + static_cast<int>((super::get_list_size() + get_line_spacing()) / (get_line_size() + get_line_spacing())), event_source::keyboard);
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
      super::register_event_handler(REGISTER_FUNCTION, typename wheel_traits<V>::wheel_event_type(this, &basic_tile_view::handle_wheel));
      super::register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &basic_tile_view::handle_key));
      super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &basic_tile_view::handle_mouse_move));
      super::register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size &) {
        if (super::scrollbar.is_valid()) {
          super::scrollbar.place(super::get_scroll_bar_area());
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
    core::size::type basic_tile_view<orientation::horizontal>::get_item_size () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_item_size () const;

    template<>
    std::size_t basic_tile_view<orientation::horizontal>::get_items_per_line () const;

    template<>
    std::size_t basic_tile_view<orientation::vertical>::get_items_per_line () const;

    template<>
    core::size::type basic_tile_view<orientation::horizontal>::get_line_size () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_line_size () const;

    template<>
    void basic_tile_view<orientation::horizontal>::handle_direction_key (os::key_symbol key);

    template<>
    void basic_tile_view<orientation::vertical>::handle_direction_key (os::key_symbol key);

    template<>
    core::size::type basic_tile_view<orientation::horizontal>::get_item_border () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_item_border () const;

    template<>
    core::size::type basic_tile_view<orientation::horizontal>::get_line_border () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_line_border () const;

    template<>
    core::size::type basic_tile_view<orientation::horizontal>::get_item_spacing () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_item_spacing () const;

    template<>
    core::size::type basic_tile_view<orientation::horizontal>::get_line_spacing () const;

    template<>
    core::size::type basic_tile_view<orientation::vertical>::get_line_spacing () const;

  } // win

} // gui
