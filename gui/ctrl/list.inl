/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      inline core::list_state::is list_base::get_state () const {
        return core::list_state::is(flags);
      }

      inline core::list_state::set list_base::set_state () {
        return core::list_state::set(flags);
      }

      inline std::size_t list_base::get_count () const {
        return data.items ? data.items().size() : 0;
      }

      inline int list_base::get_selection () const {
        return data.selection;
      }

      inline bool list_base::has_selection () const {
        return (data.selection > -1) && (data.selection < get_count());
      }

      inline int list_base::get_hilite () const {
        return data.hilite;
      }

      inline item_state list_base::get_item_state (int idx) const {
        return item_state(get_hilite() == idx, get_selection() == idx, !is_enabled());
      }

      inline os::color list_base::get_background () const {
        return data.background;
      }

      inline void list_base::set_background (os::color background) {
        data.background = background;
      }

      inline bool list_base::is_scroll_bar_enabled () const {
        return get_state().scroll_bar_enabled();
      }

      inline bool list_base::is_moved () const {
        return get_state().moved();
      }

      inline core::point list_base::get_last_mouse_point () const {
        return data.last_mouse_point;
      }

      inline bool list_base::is_valid_idx (int idx) const {
        return (idx > -1) && (idx < static_cast<int>(get_count()));
      }

      inline void list_base::draw_item (std::size_t idx,
                                        draw::graphics& g,
                                        const core::rectangle& place,
                                        const draw::brush& background,
                                        item_state state) const {
        if (data.items) {
          data.items().draw_at(idx, g, place, background, state);
        }
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline linear_list_traits<V>::linear_list_traits (size_type item_size)
      : item_size(item_size)
    {}

    template<orientation_t V>
    inline auto linear_list_traits<V>::get_invisible_size (const core::size& list_size,
                                                           size_t count) const -> size_type {
      return std::max(size_type(0), (item_size * size_type(count)) - super::get_1(list_size));
    }

    template<orientation_t V>
    inline int linear_list_traits<V>::get_index_at_point (const core::rectangle& /*list_size*/,
                                                          const core::point& pt,
                                                          size_type scroll_pos,
                                                          size_t /*count*/) const {
      return static_cast<int>((super::get_1(pt) + scroll_pos) / item_size);
    }

    template<orientation_t V>
    inline core::rectangle linear_list_traits<V>::get_place_of_index (const core::rectangle& list_size,
                                                                      int idx,
                                                                      size_type scroll_pos) const {
      core::rectangle place;
      super::set_1(place, super::get_1(list_size.position()) + item_size * idx - scroll_pos, item_size);
      super::set_2(place, super::get_2(list_size.position()), super::get_2(list_size.size()));
      return place;
    }

    template<orientation_t V>
    inline auto linear_list_traits<V>::get_offset_of_index (const core::size&, int idx) const -> size_type {
      return item_size * idx;
    }

    template<orientation_t V>
    inline auto linear_list_traits<V>::get_line_size () const -> size_type {
      return item_size;
    }

    template<orientation_t V>
    inline auto linear_list_traits<V>::get_item_dimension () const -> size_type {
      return item_size;
    }

    // --------------------------------------------------------------------------
    template<orientation_t V, typename T>
    inline basic_list<V, T>::basic_list (size_type item_size,
                                         os::color background,
                                         bool grab_focus)
      : super(background, grab_focus)
      , scrollbar(grab_focus)
      , traits(item_size)
    {
      init();
    }

    template<orientation_t V, typename T>
    inline basic_list<V, T>::basic_list (basic_list&& rhs) noexcept
      : super(std::move(rhs))
      , scrollbar(std::move(rhs.scrollbar))
      , traits(std::move(rhs.traits))
    {
      init();
    }

    template<orientation_t V, typename T>
    inline void basic_list<V, T>::create (win::container& parent,
                                          const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
      if (!r.empty()) {
        adjust_scroll_bar(r);
      }
    }

    template<orientation_t V, typename T>
    inline auto basic_list<V, T>::get_item_size () const -> size_type {
      return traits.item_size;
    }

    template<orientation_t V, typename T>
    inline core::size::type basic_list<V, T>::get_item_dimension () const {
      return traits.get_item_dimension();
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::create_scroll_bar (const core::rectangle& r) {
      if (!scrollbar.is_valid()) {
        scrollbar.create(*this, get_scroll_bar_area(r));
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::enable_scroll_bar (bool enable) {
      super::set_state().scroll_bar_enabled(enable);
      if (enable) {
        create_scroll_bar(client_area());
      }
      scrollbar.enable(enable);
      scrollbar.set_visible(enable && scrollbar.get_max());
    }

    template<orientation_t V, typename T>
    inline bool basic_list<V, T>::is_scroll_bar_visible () const {
      return scrollbar.is_visible();
    }

    template<orientation_t V, typename T>
    inline auto basic_list<V, T>::get_scroll_pos() const -> pos_t {
      return scrollbar.get_value();
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::clear_selection (event_source notify) {
      if (data.selection != -1) {
        data.selection = -1;
        if (notify != event_source::logic) {

          notify_selection_changed(notify);
          super::invalidate();
        }
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::set_hilite (int sel, bool notify) {
      int new_hilite = std::max(-1, sel);
      if (new_hilite >= static_cast<int>(super::get_count())) {
        new_hilite = -1;
      }
      if (super::get_hilite() != new_hilite) {
        data.hilite = new_hilite;
        if (notify) {
          super::notify_hilite_changed(new_hilite != -1);
          super::invalidate();
        }
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::clear_hilite (bool notify) {
      if (super::get_hilite() != -1) {
        data.hilite = -1;
        if (notify) {
          super::notify_hilite_changed(false);
          super::invalidate();
        }
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::init () {
      scrollbar.on_scroll([&] (pos_t) {
        super::invalidate();
      });
      if (scrollbar.is_focus_accepting()) {
        super::on_left_btn_down([&] (os::key_state, const core::point &) {
          super::take_focus();
        });
      }
      super::on_left_btn_dblclk([&] (os::key_state keys, const core::point & pt) {
        super::notify_selection_commit();
      });
      super::on_mouse_leave([&] () {
        clear_hilite();
      });
      super::on_left_btn_up(util::bind_method(this, &basic_list::handle_left_btn_up));
      super::on_wheel<V>(util::bind_method(this, &basic_list::handle_wheel));
      super::on_mouse_move(util::bind_method(this, &basic_list::handle_mouse_move));
      super::on_layout([&](const core::rectangle& r) {
        adjust_scroll_bar(r);
      });
    }

    template<orientation_t V, typename T>
    inline auto basic_list<V, T>::get_list_size () const -> pos_t {
      return traits.get_1(content_size());
    }

    template<orientation_t V, typename T>
    core::size basic_list<V, T>::content_size (const core::size& client_sz, bool scrollbar_visible) const {
      core::size sz = client_sz;
      if (scrollbar_visible) {
        traits.set_2(sz, traits.get_2(sz) - scroll_bar::get_scroll_bar_width());
      }
      return sz;
    }

    template<orientation_t V, typename T>
    core::size basic_list<V, T>::content_size () const {
      return content_size(client_size());
    }

    template<orientation_t V, typename T>
    core::size basic_list<V, T>::content_size (const core::size& sz) const {
      return content_size(sz, is_scroll_bar_visible());
    }

    template<orientation_t V, typename T>
    core::rectangle basic_list<V, T>::content_area (const core::rectangle& r) const {
      return r.with_size(content_size(r.size()));
    }

    template<orientation_t V, typename T>
    core::rectangle basic_list<V, T>::content_area () const {
      return super::client_area().with_size(content_size());
    }

    template<orientation_t V, typename T>
    core::rectangle basic_list<V, T>::get_scroll_bar_area (const core::rectangle& r) const {
      auto sz = static_cast<float>(scroll_bar::get_scroll_bar_width());
      core::rectangle r2 = r;
      traits.set_2(r2, traits.get_2(r.x2y2()) - sz, sz);
      return r2;
    }

    template<orientation_t V, typename T>
    template<typename U, list_item_drawer<U> F>
    inline void basic_list<V, T>::create (win::container& parent,
                                          const core::rectangle& place,
                                          std::function<list_data_provider> data) {
      super::create(clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation_t V, typename T>
    template<typename U, list_item_drawer<U> D>
    inline void basic_list<V, T>::set_data (const std::vector<U>& data) {
      super::data.items = indirect_list_data<U, D>(data);
      invalidate();
    }

    template<orientation_t V, typename T>
    template<typename U, list_item_drawer<U> D>
    inline void basic_list<V, T>::set_data (std::initializer_list<U> args) {
      super::data.items = const_list_data<U, D>(args);
      invalidate();
    }

    template<orientation_t V, typename T>
    inline void basic_list<V, T>::set_data (const std::function<list_data_provider>& dta) {
      super::data.items = dta;
      invalidate();
    }

    template<orientation_t V, typename T>
    inline void basic_list<V, T>::set_item_size (size_type item_size) {
      traits.item_size = item_size;
    }

    template<orientation_t V, typename T>
    inline void basic_list<V, T>::set_item_size_and_background (size_type item_size, os::color background) {
      traits.item_size = item_size;
      super::set_background(background);
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::adjust_scroll_bar () {
      adjust_scroll_bar(client_area());
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::adjust_scroll_bar (const core::rectangle& r) {
      const auto cs = content_size(r.size(), true);
      scroll_bar::type invisible = traits.get_invisible_size(cs, get_count());
      scrollbar.set_min_max_step_page(zero, std::max(invisible, zero), traits.get_line_size(), traits.get_1(cs));

      if (super::is_valid() && super::is_enabled()) {
        const bool show_scroll = (invisible > zero) && super::is_scroll_bar_enabled();
        if (show_scroll) {
          create_scroll_bar(r);
          scrollbar.place(get_scroll_bar_area(r), IF_WIN32_ELSE(true, false));
        }
        scrollbar.set_visible(show_scroll);
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::invalidate () {
      adjust_scroll_bar();
      super::invalidate();
    }

    template<orientation_t V, typename T>
    inline int basic_list<V, T>::get_index_at_point (const core::point& pt) {
      auto rect = content_area();
      if (rect.is_inside(pt)) {
        return traits.get_index_at_point(rect, pt - rect.position(), get_scroll_pos(), get_count());
      }
      return -1;
    }

    template<orientation_t V, typename T>
    core::rectangle basic_list<V, T>::get_place_of_index (int idx) {
      if (super::is_valid_idx(idx)) {
        return traits.get_place_of_index(content_area(), idx, get_scroll_pos());
      }
      return core::rectangle::zero;
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::set_selection (int sel, event_source notify) {
      const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
      if (super::data.selection != new_selection) {
        super::data.selection = new_selection;
        make_selection_visible();
        if (notify != event_source::logic) {
          notify_selection_changed(notify);
          super::invalidate();
        }
      }
    }

    template<orientation_t V, typename T>
    bool basic_list<V, T>::try_to_select (int sel, event_source notify) {
      if ((sel >= 0) && (sel < get_count())) {
        set_selection(sel, notify);
        return true;
      }
      return false;
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::make_selection_visible () {
      if (super::has_selection()) {
        const auto list_size = content_size();
        const auto list_sz = traits.get_1(list_size);
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

    template<orientation_t V, typename T>
    void basic_list<V, T>::set_scroll_pos (pos_t pos) {
      auto value = std::max(zero, std::min(pos, traits.get_invisible_size(content_size(), super::get_count())));
      if (value != scrollbar.get_value()) {
        scrollbar.set_value(value);
      }
    }

    template<orientation_t V, typename T>
    inline void basic_list<V, T>::handle_wheel (const pos_t delta, const core::point&) {
      set_scroll_pos(get_scroll_pos() - traits.get_line_size() * delta);
      super::set_state().moved(true);
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = surface_area();
      if (core::left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if ((super::get_last_mouse_point() != core::point::undefined) &&
            (super::get_last_mouse_point() != pt)) {
          super::set_cursor(win::cursor::move());
          pos_t delta = traits.get_1(super::get_last_mouse_point()) - traits.get_1(pt);
          set_scroll_pos(get_scroll_pos() + delta);
          super::set_state().moved(true);
        }
        super::data.last_mouse_point = pt;
      } else {
        set_hilite(get_index_at_point(super::surface_to_client(pt)));
      }
    }

    template<orientation_t V, typename T>
    void basic_list<V, T>::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
        const int new_selection = get_index_at_point(super::surface_to_client(pt));
        if (new_selection != super::get_selection()) {
          if ((new_selection < 0) || core::control_key_bit_mask::is_set(keys)) {
            clear_selection(event_source::mouse);
          } else {
            set_selection(new_selection, event_source::mouse);
          }
        } else if (core::control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      super::set_cursor(win::cursor::arrow());
      super::data.last_mouse_point = core::point::undefined;
    }

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline linear_list<V>::linear_list (core::size::type item_size,
                                        os::color background,
                                        bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<orientation_t V>
    inline linear_list<V>::linear_list (linear_list&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t V>
    void linear_list<V>::paint (draw::graphics& graph) {
      const core::rectangle area = super::content_area(super::surface_area());
      core::rectangle place = area;
      draw::clip clp(graph, area);

      draw::brush back_brush(super::get_background());

      const auto list_sz = super::traits.get_1(area.bottom_right());
      const auto last = super::get_count();
      const auto first = static_cast<decltype(last)>(super::get_scroll_pos() / super::get_item_dimension());

      super::traits.set_1(place,
                          super::traits.get_1(area.top_left()) + super::get_item_dimension() * first - super::get_scroll_pos(),
                          super::get_item_dimension());

      for (auto idx = first; (idx < last) && (super::traits.get_1(place.top_left()) < list_sz); ++idx) {
        super::draw_item(idx, graph, place, back_brush, super::get_item_state(static_cast<int>(idx)));
        super::traits.set_1(place, super::traits.get_1(place.top_left()) + super::get_item_dimension(), super::get_item_dimension());
      }

      const auto pos = super::traits.get_1(place.top_left());
      if (pos < list_sz) {
        super::traits.set_1(place, pos, list_sz - pos);
        graph.fill(draw::rectangle(place), back_brush);
      }

      if (super::is_focused()) {
        draw::frame::dots(graph, area);
      }

    }

    template<orientation_t V>
    void linear_list<V>::handle_key (os::key_state,
                                    os::key_symbol key,
                                    const std::string&) {
      handle_direction_key(key);
      switch (key) {
      case core::keys::page_up:
      case core::keys::numpad::page_up:
        super::set_selection(super::get_selection() -
                      static_cast<int>(super::get_list_size() / super::get_item_dimension()),
                      event_source::keyboard);
        break;
      case core::keys::page_down:
      case core::keys::numpad::page_down:
        super::set_selection(super::get_selection() +
                      static_cast<int>(super::get_list_size() / super::get_item_dimension()),
                      event_source::keyboard);
        break;
      case core::keys::home:
      case core::keys::numpad::home:
        super::set_selection(0, event_source::keyboard);
        break;
      case core::keys::end:
      case core::keys::numpad::end:
        super::set_selection(static_cast<int>(super::get_count()) - 1,
                      event_source::keyboard);
        break;
      case core::keys::enter:
        super::notify_selection_commit();
        break;
      }
    }

    template<orientation_t V>
    void linear_list<V>::init () {
      super::on_paint(draw::paint(this, &linear_list::paint));
      super::on_any_key_down(util::bind_method(this, &linear_list::handle_key));
    }

    // --------------------------------------------------------------------------
    inline edit_list::edit_list (core::size::type item_size,
                                 os::color background,
                                 bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    inline edit_list::edit_list (edit_list&& rhs) noexcept
      : super(std::move(rhs))
      , data(std::move(rhs.data))
    {
      init();
    }

    inline edit_list::data::data ()
      : enable_edit(true)
    {}

  } // namespace ctrl

} // namespace gui
