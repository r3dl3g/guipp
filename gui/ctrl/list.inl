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
    template<>
    inline core::point::type
    orientation_traits<orientation::horizontal>::get (const core::point& pt) {
      return pt.x();
    }

    template<>
    inline core::size::type
    orientation_traits<orientation::horizontal>::get (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline core::point::type
    orientation_traits<orientation::horizontal>::get_other (const core::point& pt) {
      return pt.y();
    }

    template<>
    inline core::size::type
    orientation_traits<orientation::horizontal>::get_other (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set (core::point& pt,
                                                      core::point::type dim,
                                                      core::point::type other) {
      pt.x(dim);
      pt.y(other);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set (core::point& pt,
                                                      core::point::type dim) {
      pt.x(dim);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set_other (core::point& pt,
                                                            core::point::type other) {
      pt.y(other);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set (core::size& sz,
                                                      core::size::type dim,
                                                      core::size::type other) {
      sz.width(dim);
      sz.height(other);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set (core::size& sz,
                                                      core::size::type dim) {
      sz.width(dim);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set_other (core::size& sz,
                                                            core::size::type other) {
      sz.height(other);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set (core::rectangle& r,
                                                      core::point::type v,
                                                      core::size::type s) {
      r.x(v);
      r.width(s);
    }

    template<>
    inline void
    orientation_traits<orientation::horizontal>::set_other (core::rectangle& r,
                                                            core::point::type v,
                                                            core::size::type s) {
      r.y(v);
      r.height(s);
    }

    // --------------------------------------------------------------------------
    template<>
    inline core::point::type
    orientation_traits<orientation::vertical>::get (const core::point& pt) {
      return pt.y();
    }

    template<>
    inline core::size::type
    orientation_traits<orientation::vertical>::get (const core::size& sz) {
      return sz.height();
    }

    template<>
    inline core::point::type
    orientation_traits<orientation::vertical>::get_other (const core::point& pt) {
      return pt.x();
    }

    template<>
    inline core::size::type
    orientation_traits<orientation::vertical>::get_other (const core::size& sz) {
      return sz.width();
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set (core::point& pt,
                                                    core::point::type dim,
                                                    core::point::type other) {
      pt.y(dim);
      pt.x(other);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set (core::point& pt,
                                                    core::point::type dim) {
      pt.y(dim);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set_other (core::point& pt,
                                                          core::point::type other) {
      pt.x(other);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set (core::size& sz,
                                                    core::size::type dim,
                                                    core::size::type other) {
      sz.height(dim);
      sz.width(other);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set (core::size& sz,
                                                    core::size::type dim) {
      sz.height(dim);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set_other (core::size& sz,
                                                          core::size::type other) {
      sz.width(other);
   }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set (core::rectangle& r,
                                                    core::point::type v,
                                                    core::size::type s) {
      r.y(v);
      r.height(s);
    }

    template<>
    inline void
    orientation_traits<orientation::vertical>::set_other (core::rectangle& r,
                                                          core::point::type v,
                                                          core::size::type s) {
      r.x(v);
      r.width(s);
    }

    // --------------------------------------------------------------------------
    template<orientation V>
    inline linear_list_traits<V>::linear_list_traits (size_type item_size)
      : item_size(item_size)
    {}

    template<orientation V>
    inline auto linear_list_traits<V>::get_invisible_size (const core::size& list_size,
                                                           size_t count) const -> size_type {
      return std::max(size_type(0), (item_size * size_type(count)) - super::get(list_size));
    }

    template<orientation V>
    inline int linear_list_traits<V>::get_index_at_point (const core::size& /*list_size*/,
                                                          const core::point& pt,
                                                          size_type scroll_pos,
                                                          size_t /*count*/) const {
      return static_cast<int>((super::get(pt) + scroll_pos) / item_size);
    }

    template<orientation V>
    inline core::rectangle linear_list_traits<V>::get_place_of_index (const core::size& list_size,
                                                                      int idx,
                                                                      size_type scroll_pos) const {
      core::rectangle place;
      super::set(place, item_size * idx - scroll_pos, item_size);
      super::set_other(place, 0, super::get_other(list_size));
      return place;
    }

    template<orientation V>
    inline auto linear_list_traits<V>::get_offset_of_index (const core::size&, int idx) const -> size_type {
      return item_size * idx;
    }

    template<orientation V>
    inline auto linear_list_traits<V>::get_line_size () const -> size_type {
      return item_size;
    }

    // --------------------------------------------------------------------------
    template<orientation V, typename T>
    inline basic_list<V, T>::basic_list (size_type item_size,
                                         os::color background,
                                         bool grab_focus)
      : super(background, grab_focus)
      , scrollbar(grab_focus)
      , traits(item_size)
    {
      init();
    }

    template<orientation V, typename T>
    inline basic_list<V, T>::basic_list (const basic_list& rhs)
      : super(rhs)
      , scrollbar(rhs.scrollbar)
      , traits(rhs.traits)
    {
      init();
    }

    template<orientation V, typename T>
    inline basic_list<V, T>::basic_list (basic_list&& rhs)
      : super(std::move(rhs))
      , scrollbar(std::move(rhs.scrollbar))
      , traits(std::move(rhs.traits))
    {
      init();
    }

    template<orientation V, typename T>
    inline void basic_list<V, T>::create (const container& parent,
                                       const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      adjust_scroll_bar(place.size());
    }

    template<orientation V, typename T>
    inline core::size::type basic_list<V, T>::get_item_size () const {
      return traits.item_size;
    }

    template<orientation V, typename T>
    void basic_list<V, T>::create_scroll_bar (const core::size& sz) {
      if (!scrollbar.is_valid()) {
        scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area(sz));
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::enable_scroll_bar (bool enable) {
      super::get_state().set_scroll_bar_enabled(enable);
      if (enable) {
        create_scroll_bar(client_size());
      }
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<orientation V, typename T>
    inline bool basic_list<V, T>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<orientation V, typename T>
    inline auto basic_list<V, T>::get_scroll_pos() const -> pos_t {
      return scrollbar.get_value();
    }

    template<orientation V, typename T>
    void basic_list<V, T>::clear_selection (event_source notify) {
      if (data.selection != -1) {
        data.selection = -1;
        if (notify != event_source::logic) {
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          super::redraw();
        }
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::set_hilite (int sel, bool notify) {
      int new_hilite = std::max(-1, sel);
      if (new_hilite >= static_cast<int>(super::get_count())) {
        new_hilite = -1;
      }
      if (super::get_hilite() != new_hilite) {
        data.hilite = new_hilite;
        if (notify) {
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, new_hilite != -1);
          super::redraw();
        }
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::clear_hilite (bool notify) {
      if (super::get_hilite() != -1) {
        data.hilite = -1;
        if (notify) {
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, false);
          super::redraw();
        }
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::init () {
      scrollbar.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (pos_t) {
        super::redraw();
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
      super::register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &basic_list::handle_left_btn_up));
      super::register_event_handler(REGISTER_FUNCTION, typename wheel_traits<V>::wheel_event_type(this, &basic_list::handle_wheel));
      super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &basic_list::handle_mouse_move));
      super::register_event_handler(REGISTER_FUNCTION, layout_event([&](const core::size& sz) {
        adjust_scroll_bar(sz);
      }));
    }

    template<orientation V, typename T>
    inline auto basic_list<V, T>::get_list_size () const -> pos_t {
      return traits.get(content_size());
    }

    template<orientation V, typename T>
    core::size basic_list<V, T>::content_size () const {
      core::size sz = super::client_size();
      if (is_scroll_bar_visible()) {
        traits.set_other(sz, traits.get_other(sz) - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

    template<orientation V, typename T>
    core::rectangle basic_list<V, T>::content_area () const {
      return core::rectangle(content_size());
    }

    template<orientation V, typename T>
    core::rectangle basic_list<V, T>::get_scroll_bar_area (const core::size& s) const {
      core::rectangle r(s);
      float sz = static_cast<float>(scroll_bar::get_scroll_bar_width());
      traits.set_other(r, traits.get_other(r.size()) - sz, sz);
      return r;
    }

    template<orientation V, typename T>
    template<typename U, list_item_drawer<U> F>
    inline void basic_list<V, T>::create (const container& parent,
                                          const core::rectangle& place,
                                          const simple_list_data<U, F>& data) {
      super::create(clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation V, typename T>
    inline void basic_list<V, T>::set_item_size (size_type item_size) {
      traits.item_size = item_size;
    }

    template<orientation V, typename T>
    inline void basic_list<V, T>::set_item_size_and_background (size_type item_size, os::color background) {
      traits.item_size = item_size;
      super::set_background(background);
    }

    template<orientation V, typename T>
    void basic_list<V, T>::adjust_scroll_bar () {
      adjust_scroll_bar(client_size());
    }

    template<orientation V, typename T>
    void basic_list<V, T>::adjust_scroll_bar (const core::size& sz) {
      if (super::is_scroll_bar_enabled()) {
        scroll_bar::type invisible = traits.get_invisible_size(sz, super::get_count());
        const bool show_scroll = (invisible > zero);
        if (show_scroll) {
          create_scroll_bar(sz);
          scrollbar.place(get_scroll_bar_area(sz), IF_WIN32_ELSE(true, false));
          scrollbar.set_max(std::max(invisible, zero));
        }
        scrollbar.set_visible(show_scroll);
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::set_count (std::size_t count) {
      super::data.item_count = count;

      const auto sz = client_size();
      scroll_bar::type invisible = traits.get_invisible_size(sz, count);
      scrollbar.set_min_max_step_page(zero, std::max(invisible, zero), traits.get_line_size(), traits.get(sz));

      if (super::is_valid()) {
        const bool show_scroll = (invisible > zero) && super::is_scroll_bar_enabled();
        if (show_scroll) {
          create_scroll_bar(sz);
          scrollbar.place(get_scroll_bar_area(sz), IF_WIN32_ELSE(true, false));
        }
        scrollbar.set_visible(show_scroll);
        super::redraw();
      }
    }

    template<orientation V, typename T>
    template<typename F>
    inline void basic_list<V, T>::set_data (const simple_list_data<F>& data) {
      super::set_drawer(data);
      set_count(data.size());
    }

    template<orientation V, typename T>
    template<typename F>
    inline void basic_list<V, T>::set_data (std::initializer_list<F> args) {
      super::set_drawer(simple_list_data<F>(args));
      set_count(args.size());
    }

    template<orientation V, typename T>
    inline int basic_list<V, T>::get_index_at_point (const core::point& pt) {
      auto rect = content_area();
      if (rect.is_inside(pt)) {
        return traits.get_index_at_point(rect.size(), pt, get_scroll_pos(), get_count());
      }
      return -1;
    }

    template<orientation V, typename T>
    core::rectangle basic_list<V, T>::get_place_of_index (int idx) {
      if (super::is_valid_idx(idx)) {
        return traits.get_place_of_index(content_size(), idx, get_scroll_pos());
      }
      return core::rectangle::zero;
    }

    template<orientation V, typename T>
    void basic_list<V, T>::set_selection (int sel, event_source notify) {
      const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
      if (super::data.selection != new_selection) {
        super::data.selection = new_selection;
        make_selection_visible();
        if (notify != event_source::logic) {
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          super::redraw();
        }
      }
    }

    template<orientation V, typename T>
    bool basic_list<V, T>::try_to_select (int sel, event_source notify) {
      if ((sel >= 0) && (sel < get_count())) {
        set_selection(sel, notify);
        return true;
      }
      return false;
    }

    template<orientation V, typename T>
    void basic_list<V, T>::make_selection_visible () {
      if (super::data.selection > -1) {
        const auto list_size = content_size();
        const auto list_sz = traits.get(list_size);
        const auto scroll_pos = get_scroll_pos();
        const auto line_size = traits.get_line_size();
        const auto sel_pos = traits.get_offset_of_index(list_size, super::get_selection());

        if (sel_pos < scroll_pos) {
          set_scroll_pos(sel_pos);
        } else if (sel_pos + line_size - scroll_pos > list_sz) {
          set_scroll_pos(sel_pos + line_size - list_sz);
        }
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::set_scroll_pos (pos_t pos) {
      auto value = std::max(zero, std::min(pos, traits.get_invisible_size(content_size(), super::get_count())));
      if (value != scrollbar.get_value()) {
        scrollbar.set_value(value);
      }
    }

    template<orientation V, typename T>
    inline void basic_list<V, T>::handle_wheel (const pos_t delta, const core::point&) {
      set_scroll_pos(get_scroll_pos() - traits.get_line_size() * delta);
      super::get_state().set_moved(true);
    }

    template<orientation V, typename T>
    void basic_list<V, T>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = content_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (super::get_last_mouse_point() != core::point::undefined) {
          super::set_cursor(cursor::move());
          pos_t delta = traits.get(super::get_last_mouse_point()) - traits.get(pt);
          set_scroll_pos(get_scroll_pos() + delta);
          super::get_state().set_moved(true);
        }
        super::data.last_mouse_point = pt;
      } else {
        set_hilite(get_index_at_point(pt));
      }
    }

    template<orientation V, typename T>
    void basic_list<V, T>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
        const int new_selection = traits.get_index_at_point(content_size(), pt, get_scroll_pos(), get_count());
        if (new_selection != super::get_selection()) {
          if ((new_selection < 0) || control_key_bit_mask::is_set(keys)) {
            clear_selection(event_source::mouse);
          } else {
            set_selection(new_selection, event_source::mouse);
          }
        } else if (control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      super::set_cursor(cursor::arrow());
      super::data.last_mouse_point = core::point::undefined;
    }

    // --------------------------------------------------------------------------
    template<orientation V>
    inline linear_list<V>::linear_list (core::size::type item_size,
                                        os::color background,
                                        bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<orientation V>
    inline linear_list<V>::linear_list (const linear_list& rhs)
      : super(rhs)
    {
      init();
    }

    template<orientation V>
    inline linear_list<V>::linear_list (linear_list&& rhs)
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation V>
    void linear_list<V>::paint (const draw::graphics& graph) {
      const core::rectangle area(super::content_size());
      core::rectangle place = area;

      draw::brush back_brush(super::get_background());

      const auto list_sz = super::get_list_size();
      const auto last = super::get_count();
      const auto first = static_cast<decltype(last)>(super::get_scroll_pos() / super::get_item_size());

      super::traits.set(place, super::get_item_size() * first - super::get_scroll_pos(), super::get_item_size());

      for (auto idx = first; (idx < last) && (super::traits.get(place.top_left()) < list_sz); ++idx) {
        super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
        super::traits.set(place, super::traits.get(place.top_left()) + super::get_item_size(), super::get_item_size());
      }

      if (place.y() < area.y2()) {
        graph.fill(draw::rectangle(core::rectangle(place.top_left(), area.bottom_right())), back_brush);
      }

      if (super::has_focus()) {
        draw::frame::dots(graph, area);
      }

    }

    template<orientation V>
    void linear_list<V>::handle_key (os::key_state,
                                    os::key_symbol key,
                                    const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case keys::page_up:
      case keys::numpad::page_up:
        super::set_selection(super::get_selection() -
                      static_cast<int>(super::get_list_size() / super::get_item_size()),
                      event_source::keyboard);
        break;
      case keys::page_down:
      case keys::numpad::page_down:
        super::set_selection(super::get_selection() +
                      static_cast<int>(super::get_list_size() / super::get_item_size()),
                      event_source::keyboard);
        break;
      case keys::home:
      case keys::numpad::home:
        super::set_selection(0, event_source::keyboard);
        break;
      case keys::end:
      case keys::numpad::end:
        super::set_selection(static_cast<int>(super::get_count()) - 1,
                      event_source::keyboard);
        break;
      case keys::enter:
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        break;
      }
    }

    template<orientation V>
    void linear_list<V>::init () {
      super::register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &linear_list::paint)));
      super::register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &linear_list::handle_key));
    }

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

  } // namespace win

} // namespace gui
