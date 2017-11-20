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


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      inline list_state::list_state (const list_base& win)
        : super(win)
      {}

      // --------------------------------------------------------------------------
      inline const list_state list_base::get_state () const {
        return list_state(*this);
      }

      inline list_state list_base::get_state () {
        return list_state(*this);
      }

      inline std::size_t list_base::get_count () const {
        return data.item_count;
      }

      inline int list_base::get_selection () const {
        return data.selection;
      }

      inline int list_base::get_hilite () const {
        return data.hilite;
      }

      inline os::color list_base::get_background () const {
        return data.background;
      }

      inline void list_base::set_background (os::color background) {
        data.background = background;
      }

      inline bool list_base::is_scroll_bar_enabled () const {
        return get_state().is_scroll_bar_enabled();
      }

      inline bool list_base::is_moved () const {
        return get_state().is_moved();
      }

      inline core::point list_base::get_last_mouse_point () const {
        return data.last_mouse_point;
      }

      inline bool list_base::is_valid_idx (int idx) const {
        return (idx > -1) && (idx < static_cast<int>(get_count()));
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T, list_item_drawer<T> F>
    inline simple_list_data<T, F>::simple_list_data ()
    {}

    template<typename T, list_item_drawer<T> F>
    inline simple_list_data<T, F>::simple_list_data (std::initializer_list<T> args)
      : super(args)
    {}

    template<typename T, list_item_drawer<T> F>
    inline simple_list_data<T, F>::simple_list_data (iterator b, iterator e)
      : super(b, e)
    {}

    template<typename T, list_item_drawer<T> F>
    template<size_t N>
    inline simple_list_data<T, F>::simple_list_data (const T(&t)[N])
      : super(t, t + N)
    {}

    template<typename T, list_item_drawer<T> F>
    template<typename L>
    inline void simple_list_data<T, F>::update_list (L& l) {
      l.set_count(super::size());
    }

    template<typename T, list_item_drawer<T> F>
    inline void simple_list_data<T, F>::operator() (std::size_t idx,
                                                    const draw::graphics& g,
                                                    const core::rectangle& place,
                                                    const draw::brush& background,
                                                    bool selected,
                                                    bool hilited) {
      F(super::at(idx), g, place, background, selected, hilited);
    }

    // --------------------------------------------------------------------------
    template<orientation V>
    inline basic_list<V>::basic_list (os::color background,
                                      bool grab_focus)
      : super(background, grab_focus)
      , scrollbar(grab_focus)
    {
      init();
    }

    template<orientation V>
    inline basic_list<V>::basic_list (const basic_list& rhs)
      : super(rhs)
      , scrollbar(rhs.scrollbar)
    {
      init();
    }

    template<orientation V>
    inline basic_list<V>::basic_list (basic_list&& rhs)
      : super(std::move(rhs))
      , scrollbar(std::move(rhs.scrollbar))
    {
      init();
    }

    template<orientation V>
    void basic_list<V>::create_scroll_bar () {
      if (!scrollbar.is_valid()) {
        scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
      }
    }

    template<orientation V>
    void basic_list<V>::enable_scroll_bar (bool enable) {
      super::get_state().set_scroll_bar_enabled(enable);
      if (enable) {
        create_scroll_bar();
      }
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<orientation V>
    inline bool basic_list<V>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<orientation V>
    inline auto basic_list<V>::get_scroll_pos() const->pos_t {
      return scrollbar.get_value();
    }

    template<orientation V>
    void basic_list<V>::clear_selection (event_source notify) {
      if (data.selection != -1) {
        data.selection = -1;
        if (notify != event_source::logic) {
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          super::redraw_later();
        }
      }
    }

    template<orientation V>
    void basic_list<V>::set_hilite (int sel, bool notify) {
      int new_hilite = std::max(-1, sel);
      if (new_hilite >= static_cast<int>(super::get_count())) {
        new_hilite = -1;
      }
      if (super::get_hilite() != new_hilite) {
        data.hilite = new_hilite;
        if (notify) {
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, new_hilite != -1);
          super::redraw_later();
        }
      }
    }

    template<orientation V>
    void basic_list<V>::clear_hilite (bool notify) {
      if (super::get_hilite() != -1) {
        data.hilite = -1;
        if (notify) {
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, false);
          super::redraw_later();
        }
      }
    }

    template<orientation V>
    void basic_list<V>::init () {
      scrollbar.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (pos_t) {
        super::redraw_later();
      }));
      if (scrollbar.is_focus_accepting()) {
        super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&] (os::key_state, const core::point &) {
          super::take_focus();
        }));
      }
      super::register_event_handler(REGISTER_FUNCTION, left_btn_dblclk_event([&] (os::key_state keys, const core::point & pt) {
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
      }));
      super::register_event_handler(REGISTER_FUNCTION, mouse_leave_event([&] () {
        clear_hilite();
      }));
    }

    // --------------------------------------------------------------------------
    template<>
    core::rectangle basic_list<orientation::horizontal>::get_scroll_bar_area () const;

    template<>
    auto basic_list<orientation::horizontal>::get_list_size () const -> pos_t;

    template<>
    auto basic_list<orientation::horizontal>::get_dimension (const core::point &) const -> pos_t;

    template<>
    auto basic_list<orientation::horizontal>::get_other_dimension (const core::point &) const -> pos_t;

    template<>
    void basic_list<orientation::horizontal>::set_dimension (core::rectangle &, pos_t, pos_t) const;

    template<>
    void basic_list<orientation::horizontal>::set_other_dimension (core::rectangle &, pos_t, pos_t) const;

    template<>
    core::size basic_list<orientation::horizontal>::client_size () const;

    // --------------------------------------------------------------------------
    template<>
    core::rectangle basic_list<orientation::vertical>::get_scroll_bar_area () const;

    template<>
    auto basic_list<orientation::vertical>::get_list_size () const->pos_t;

    template<>
    auto basic_list<orientation::vertical>::get_dimension (const core::point &) const->pos_t;

    template<>
    auto basic_list<orientation::vertical>::get_other_dimension (const core::point &) const->pos_t;

    template<>
    void basic_list<orientation::vertical>::set_dimension (core::rectangle &, pos_t, pos_t) const;

    template<>
    void basic_list<orientation::vertical>::set_other_dimension (core::rectangle &, pos_t, pos_t) const;

    template<>
    core::size basic_list<orientation::vertical>::client_size () const;

    // --------------------------------------------------------------------------
    template<orientation V>
    inline lines_list<V>::lines_list (core::size::type item_size,
                                      os::color background,
                                      bool grab_focus)
      : super(background, grab_focus)
      , item_size(item_size)
    {
      init();
    }

    template<orientation V>
    inline lines_list<V>::lines_list (const lines_list& rhs)
      : super(rhs)
      , item_size(rhs.item_size)
    {
      init();
    }

    template<orientation V>
    inline lines_list<V>::lines_list (lines_list&& rhs)
      : super(std::move(rhs))
      , item_size(std::move(rhs.item_size))
    {
      init();
    }

    template<orientation V>
    inline void lines_list<V>::create (const container& parent,
                                       const core::rectangle& place) {
      super::create(super::clazz::get(), parent, place);
      adjust_scroll_bar();
    }

    template<orientation V>
    template<typename T, list_item_drawer<T> F>
    inline void lines_list<V>::create (const container& parent,
                                       const core::rectangle& place,
                                       const simple_list_data<T, F>& data) {
      super::create(super::clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation V>
    inline core::size::type lines_list<V>::get_item_size () const {
      return item_size;
    }

    template<orientation V>
    inline void lines_list<V>::set_item_size (core::size::type item_size) {
      this->item_size = item_size;
    }

    template<orientation V>
    inline void lines_list<V>::set_item_size_and_background (core::size::type item_size, os::color background) {
      this->item_size = item_size;
      super::set_background(background);
    }

    template<orientation V>
    void lines_list<V>::adjust_scroll_bar () {
      if (super::is_scroll_bar_enabled()) {
        scroll_bar::type visible = (get_item_size() * super::get_count()) - super::get_list_size();
        const bool show_scroll = (visible > super::zero);
        if (show_scroll) {
          super::create_scroll_bar();
        }
        super::scrollbar.set_max(std::max(visible, super::zero));
        super::scrollbar.set_visible(show_scroll);
      }
    }

    template<orientation V>
    void lines_list<V>::set_count (std::size_t count) {
      super::data.item_count = count;

      const pos_t sz = super::get_list_size();
      const pos_t visible = (get_item_size() * static_cast<pos_t>(count)) - sz;

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
    inline void lines_list<V>::set_data (const F& data) {
      super::set_drawer(data);
      set_count(data.size());
    }

    template<orientation V>
    inline int lines_list<V>::get_index_at_point (const core::point& pt) {
      if (super::client_area().is_inside(pt)) {
        return static_cast<int>((super::get_dimension(pt) + super::get_scroll_pos()) / get_item_size());
      }
      return -1;
    }

    template<orientation V>
    core::rectangle lines_list<V>::get_place_of_index (int idx) {
      if (super::is_valid_idx(idx)) {
        core::rectangle place(super::client_size());
        const auto is = get_item_size();
        super::set_dimension(place, is * idx - super::get_scroll_pos(), is);
        return place;
      }
      return core::rectangle::zero;
    }

    template<orientation V>
    void lines_list<V>::set_selection (int sel, event_source notify) {
      const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count()));
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
    void lines_list<V>::make_selection_visible () {
      if (super::data.selection > -1) {
        const pos_t sel_pos = static_cast<pos_t>(get_item_size() * super::get_selection());
        const pos_t sz = super::get_list_size();

        if (sel_pos < super::get_scroll_pos()) {
          set_scroll_pos(sel_pos);
        } else if (sel_pos + get_item_size() - super::get_scroll_pos() > sz) {
          set_scroll_pos(sel_pos + get_item_size() - sz);
        } else {
          super::redraw_later();
        }
      }
    }

    template<orientation V>
    void lines_list<V>::set_scroll_pos (pos_t pos) {
      const pos_t max_delta =
        std::max(super::zero, (get_item_size() * (pos_t)super::get_count()) - super::get_list_size());
      auto value = std::min(std::max(super::zero, pos), max_delta);
      if (value != super::scrollbar.get_value()) {
        super::scrollbar.set_value(value);
      }
    }

    template<orientation V>
    void lines_list<V>::paint (const draw::graphics& graph) {
      const core::rectangle area(super::client_size());
      core::rectangle place = area;

      draw::brush back_brush(super::get_background());

      const pos_t list_sz = super::get_list_size();
      const auto last = super::get_count();
      const auto first = static_cast<decltype(last)>(super::get_scroll_pos() / get_item_size());

      super::set_dimension(place, get_item_size() * first - super::get_scroll_pos(), get_item_size());

      for (auto idx = first; (idx < last) && (super::get_dimension(place.top_left()) < list_sz); ++idx) {
        super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
        super::set_dimension(place, super::get_dimension(place.top_left()) + get_item_size(), get_item_size());
      }

      if (place.y() < area.y2()) {
        graph.fill(draw::rectangle(core::rectangle(place.top_left(), area.bottom_right())), back_brush);
      }

      if (super::has_focus()) {
        draw::frame::dots(graph, area);
      }

    }

    template<orientation V>
    inline void lines_list<V>::handle_wheel (const pos_t delta, const core::point&) {
      set_scroll_pos(super::get_scroll_pos() - get_item_size() * delta);
      super::get_state().set_moved(true);
    }

    template<orientation V>
    void lines_list<V>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = super::client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (super::get_last_mouse_point() != core::point::undefined) {
          pos_t delta = super::get_dimension(super::get_last_mouse_point()) - super::get_dimension(pt);
          set_scroll_pos(super::get_scroll_pos() + delta);
          super::get_state().set_moved(true);
        }
        super::data.last_mouse_point = pt;
      }
      else {
        super::set_hilite(get_index_at_point(pt));
      }
    }

    template<orientation V>
    void lines_list<V>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
        const int new_selection = get_index_at_point(pt);
        if (new_selection != super::get_selection()) {
          if (new_selection < 0) {
            super::clear_selection(event_source::mouse);
          } else {
            set_selection(new_selection, event_source::mouse);
          }
        } else if (control_key_bit_mask::is_set(keys)) {
          super::clear_selection(event_source::mouse);
        }
        super::redraw_later();
      }
      super::data.last_mouse_point = core::point::undefined;
    }

    template<orientation V>
    void lines_list<V>::handle_key (os::key_state,
                                    os::key_symbol key,
                                    const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case keys::page_up:
      case keys::numpad::page_up:
        set_selection(super::get_selection() -
                      static_cast<int>(super::get_list_size() / get_item_size()),
                      event_source::keyboard);
        break;
      case keys::page_down:
      case keys::numpad::page_down:
        set_selection(super::get_selection() +
                      static_cast<int>(super::get_list_size() / get_item_size()),
                      event_source::keyboard);
        break;
      case keys::home:
      case keys::numpad::home:
        set_selection(0, event_source::keyboard);
        break;
      case keys::end:
      case keys::numpad::end:
        set_selection(static_cast<int>(super::get_count()) - 1,
                      event_source::keyboard);
        break;
      case keys::enter:
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        break;
      }
    }

    template<orientation V>
    void lines_list<V>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &lines_list::paint)));
      super::register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &lines_list::handle_left_btn_up));
      super::register_event_handler(REGISTER_FUNCTION, typename super::traits::wheel_event_type(this, &lines_list::handle_wheel));
      super::register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &lines_list::handle_key));
      super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &lines_list::handle_mouse_move));
      super::register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size &) {
        if (super::scrollbar.is_valid()) {
          super::scrollbar.place(super::get_scroll_bar_area());
        }
        adjust_scroll_bar();
      }));
    }

    // --------------------------------------------------------------------------
    template<>
    void lines_list<orientation::horizontal>::handle_direction_key (os::key_symbol key);

    template<>
    void lines_list<orientation::vertical>::handle_direction_key (os::key_symbol key);

    // --------------------------------------------------------------------------
    inline edit_list::edit_list (core::size::type item_size,
                                 os::color background,
                                 bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    inline edit_list::edit_list (const edit_list& rhs)
      : super(rhs)
      , data(rhs.data)
    {
      init();
    }

    inline edit_list::edit_list (edit_list&& rhs)
      : super(std::move(rhs))
      , data(std::move(rhs.data))
    {
      init();
    }

    inline edit_list::data::data ()
      : enable_edit(true)
    {}

  } // win

} // gui
