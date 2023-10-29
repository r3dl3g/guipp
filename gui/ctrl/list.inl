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
 * @brief     list control
 *
 * @license   MIT license. See accompanying file LICENSE.
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

      inline int list_base::get_hilite () const {
        return data.hilite;
      }

      inline bool list_base::is_scroll_bar_enabled () const {
        return get_state().scroll_bar_enabled();
      }

      inline bool list_base::is_moved () const {
        return get_state().moved();
      }

      inline core::native_point list_base::get_last_mouse_point () const {
        return data.last_mouse_point;
      }

      inline const core::point& list_base::get_scroll_pos () const {
        return data.offset;
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

      // --------------------------------------------------------------------------
      template<typename T, typename S>
      inline selectable_list<T, S>::selectable_list (size_type item_size,
                                                     os::color background,
                                                     bool grab_focus)
        : super(background, grab_focus)
        , adjustment(core::selection_adjustment::next)
        , traits(item_size)
      {
        init();
      }

      template<typename T, typename S>
      inline selectable_list<T, S>::selectable_list (selectable_list&& rhs) noexcept
        : super(std::move(rhs))
        , selection(std::move(rhs.selection))
        , adjustment(rhs.adjustment)
        , traits(std::move(rhs.traits))
      {
        init();
      }

      template<typename T, typename S>
      void selectable_list<T, S>::clear_selection (event_source notify) {
        if (selection.has_selection()) {
          selection.clear_selection();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
            super::invalidate();
          }
        }
      }

      template<typename T, typename S>
      inline auto selectable_list<T, S>::get_selection () const -> const selector_type& {
        return selection;
      }

      template<typename T, typename S>
      inline auto selectable_list<T, S>::get_selection () -> selector_type& {
        return selection;
      }

      template<typename T, typename S>
      inline bool selectable_list<T, S>::has_selection () const {
        return selection.has_selection();
      }

      template<typename T, typename S>
      inline item_state selectable_list<T, S>::get_item_state (int idx) const {
        return item_state(get_hilite() == idx, selection.is_selected(idx), !is_enabled());
      }

      template<typename T, typename S>
      inline bool selectable_list<T, S>::can_select_multi () const {
        return selection.is_multi_select();
      }

      template<typename T, typename S>
      void selectable_list<T, S>::set_selection_adjustment (core::selection_adjustment adjust) {
        adjustment = adjust;
      }

      template<typename T, typename S>
      core::selection_adjustment selectable_list<T, S>::get_selection_adjustment () const {
        return adjustment;
      }

      template<typename T, typename S>
      void selectable_list<T, S>::make_entry_visible () {
        const int sel_idx = get_selection().get_first_index();
        const auto list_size = super::client_size();
        const auto scroll_pos = traits.get_scroll_pos(*this);

        const auto new_pos = core::get_adjusted_scroll_position(traits.get_1(list_size),
                                                                traits.get_line_size(),
                                                                scroll_pos,
                                                                traits.get_offset_of_index(list_size, sel_idx),
                                                                get_selection_adjustment());
        if (new_pos != scroll_pos) {
          traits.set_scroll_pos(*this, traits.get_line_size(), new_pos);
        }
      }

      template<typename T, typename S>
      void selectable_list<T, S>::set_selection (int sel, event_source notify, bool add) {
        const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
        if (add) {
          selection.expand_to(new_selection);
        } else {
          clear_selection(notify);
          selection.set_selected(new_selection);
        }
        make_entry_visible();
        super::invalidate();
        if (notify != event_source::logic) {
          super::notify_selection_changed(notify);
        }
      }

      template<typename T, typename S>
      void selectable_list<T, S>::select (int sel, event_source notify) {
        const int new_selection = std::min(std::max(0, sel), static_cast<int>(super::get_count() - 1));
        if (!selection.is_selected(new_selection)) {
          selection.set_selected(new_selection);
          make_entry_visible();
          super::invalidate();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
          }
        }
      }

      template<typename T, typename S>
      void selectable_list<T, S>::select_all (event_source notify) {
        selection.select_range(0, static_cast<int>(super::get_count()) - 1);
        super::invalidate();
        if (notify != event_source::logic) {
          super::notify_selection_changed(notify);
        }
      }

      template<typename T, typename S>
      void selectable_list<T, S>::unselect (int sel, event_source notify) {
        if (selection.is_selected(sel)) {
          selection.set_unselected(sel);
          super::invalidate();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
          }
        }
      }

      template<typename T, typename S>
      bool selectable_list<T, S>::try_to_select (int sel, event_source notify) {
        if ((sel >= 0) && (sel < super::get_count())) {
          set_selection(sel, notify);
          return true;
        }
        return false;
      }

      template<typename T, typename S>
      void selectable_list<T, S>::make_selection_visible () {
        if (has_selection()) {
          make_entry_visible();
        }
      }

      template<typename T, typename S>
      auto selectable_list<T, S>::get_list_size () const -> dim_type {
        return traits.get_1(super::client_size());
      }

      template<typename T, typename S>
      int selectable_list<T, S>::get_index_at_point (const core::point& pt) {
        auto rect = super::client_geometry();
        if (rect.is_inside(pt)) {
          return traits.get_index_at_point(rect, pt - rect.position(), super::get_scroll_pos(), super::get_count());
        }
        return -1;
      }

      template<typename T, typename S>
      core::rectangle selectable_list<T, S>::get_geometry_of_index (int idx) {
        if (super::is_valid_idx(idx)) {
          return traits.get_geometry_of_index(super::client_geometry(), idx, super::get_scroll_pos());
        }
        return core::rectangle::zero;
      }

      template<typename T, typename S>
      void selectable_list<T, S>::handle_mouse_move (os::key_state keys, const core::native_point& pt) {
        const auto r = super::surface_geometry();
        if (core::left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
          if ((super::get_last_mouse_point() != core::native_point::undefined) &&
              (super::get_last_mouse_point() != pt)) {
            super::set_cursor(win::cursor::move());
            auto delta = traits.get_1(super::get_last_mouse_point()) - traits.get_1(pt);
            traits.set_scroll_pos(*this,
                                        traits.get_line_size(),
                                        traits.get_scroll_pos(*this) + core::global::scale_from_native<pos_t>(delta));
            super::set_state().moved(true);
          }
          super::data.last_mouse_point = pt;
        } else {
          super::set_hilite(get_index_at_point(super::surface_to_client(pt)));
        }
      }

      template<typename T, typename S>
      void selectable_list<T, S>::handle_left_btn_up (os::key_state state, const core::native_point& pt) {
        if (!super::is_moved() && (super::get_last_mouse_point() != core::native_point::undefined)) {
          const int new_selection = get_index_at_point(super::surface_to_client(pt));
  #ifdef GUIPP_BUILD_FOR_MOBILE
          if (super::get_selection().is_selected(new_selection)) {
            // second selection -> commit
            super::notify_selection_commit();
  #else // !GUIPP_BUILD_FOR_MOBILE
          const bool shift_pressed = core::shift_key_bit_mask::is_set(state);
          const bool ctrl_pressed = core::control_key_bit_mask::is_set(state);

          if (ctrl_pressed) {
            if (shift_pressed) {
              set_selection(new_selection, event_source::mouse, shift_pressed);
            } else {
              if (get_selection().is_selected(new_selection)) {
                unselect(new_selection, event_source::mouse);
              } else {
                select(new_selection, event_source::mouse);
              }
            }
  #endif // GUIPP_BUILD_FOR_MOBILE
          } else {
            set_selection(new_selection, event_source::mouse, shift_pressed);
          }
        }
        super::set_cursor(win::cursor::arrow());
        super::data.last_mouse_point = core::native_point::undefined;
      }

      template<typename T, typename S>
      void selectable_list<T, S>::init () {
  #ifndef GUIPP_BUILD_FOR_MOBILE
        super::on_left_btn_dblclk([&] (os::key_state, const core::native_point&) {
          if (has_selection()) {
            super::notify_selection_commit();
          }
        });
  #endif // GUIPP_BUILD_FOR_MOBILE
        super::on_mouse_leave([&] () {
          super::clear_hilite();
        });
        super::on_size ([&] (const core::size&) {
          make_selection_visible();
        });

        super::on_left_btn_up(util::bind_method(this, &selectable_list::handle_left_btn_up));
        super::on_mouse_move(util::bind_method(this, &selectable_list::handle_mouse_move));
      }

      // --------------------------------------------------------------------------
    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline auto uniform_list_traits<V>::get_scroll_pos (const detail::list_base& list) const -> dim_type {
      return super::get_1(list.get_scroll_pos());
    }

    template<orientation_t V>
    inline void uniform_list_traits<V>::set_scroll_pos (detail::list_base& list, dim_type line_size, dim_type pos) {
      auto value = std::max(dim_type(0), std::min(pos, static_cast<dim_type>(line_size * list.get_count() - super::get_1(list.client_size()))));
      auto pt = list.get_scroll_pos();
      super::set_1(pt, value);
      list.set_scroll_pos(pt);
      list.notify_scroll(value);
    }

    // --------------------------------------------------------------------------
    template<orientation_t V>
    inline linear_list_traits<V>::linear_list_traits (size_type item_size)
      : item_size(item_size)
    {}

    template<orientation_t V>
    inline int linear_list_traits<V>::get_index_at_point (const core::rectangle& /*list_size*/,
                                                          const core::point& pt,
                                                          const core::point& scroll_pos,
                                                          size_t /*count*/) const {
      return static_cast<int>((super::get_1(pt) + super::get_1(scroll_pos)) / item_size);
    }

    template<orientation_t V>
    inline core::rectangle linear_list_traits<V>::get_geometry_of_index (const core::rectangle& list_size,
                                                                         int idx,
                                                                         const core::point& scroll_pos) const {
      core::rectangle place;
      super::set_1(place, super::get_1(list_size.position()) + item_size * idx - super::get_1(scroll_pos), item_size);
      super::set_2(place, super::get_2(list_size.position()) - super::get_2(scroll_pos), super::get_2(list_size.size()));
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
    template<>
    template<typename T>
    void linear_list_traits<orientation_t::horizontal>::handle_direction_key (T& list, bool shift_pressed, os::key_symbol key) {
      switch (key) {
      case core::keys::left:
      case core::keys::numpad::left:
        list.set_selection(list.get_selection().get_first_index() - 1, event_source::keyboard, shift_pressed);
        break;
      case core::keys::right:
      case core::keys::numpad::right:
        list.set_selection(list.get_selection().get_last_index() + 1, event_source::keyboard, shift_pressed);
        break;
      }
    }

    template<>
    template<typename T>
    void linear_list_traits<orientation_t::vertical>::handle_direction_key (T& list, bool shift_pressed, os::key_symbol key) {
      switch (key) {
      case core::keys::up:
      case core::keys::numpad::up:
        list.set_selection(list.get_selection().get_first_index() - 1, event_source::keyboard, shift_pressed);
        break;
      case core::keys::down:
      case core::keys::numpad::down:
        list.set_selection(list.get_selection().get_last_index() + 1, event_source::keyboard, shift_pressed);
        break;
      }
    }

    // --------------------------------------------------------------------------
    template<orientation_t V, typename T, typename S>
    inline uniform_list<V, T, S>::uniform_list (size_type item_size,
                                                os::color background,
                                                bool grab_focus)
      : super(item_size, background, grab_focus)
    {}

    template<orientation_t V, typename T, typename S>
    inline void uniform_list<V, T, S>::create (win::container& parent,
                                               const core::rectangle& r) {
      super::create(clazz::get(), parent, r);
    }

    template<orientation_t V, typename T, typename S>
    template<typename U, list_item_drawer<U> F>
    inline void uniform_list<V, T, S>::create (win::container& parent,
                                               const core::rectangle& place,
                                               std::function<list_data_provider> data) {
      super::create(clazz::get(), parent, place);
      set_data(data);
    }

    template<orientation_t V, typename T, typename S>
    template<typename U, list_item_drawer<U> D, typename C>
    inline void uniform_list<V, T, S>::set_data (const C& data) {
      super::data.items = indirect_list_data<U, D, C>(data);
      super::invalidate();
    }

    template<orientation_t V, typename T, typename S>
    template<typename U, list_item_drawer<U> D>
    inline void uniform_list<V, T, S>::set_data (std::initializer_list<U> args) {
      super::data.items = const_list_data<U, D>(std::move(args));
      super::invalidate();
    }

    template<orientation_t V, typename T, typename S>
    inline void uniform_list<V, T, S>::set_data (const std::function<list_data_provider>& dta) {
      super::data.items = dta;
      super::invalidate();
    }

    template<orientation_t V, typename T, typename S>
    inline core::size::type uniform_list<V, T, S>::get_item_dimension () const {
      return super::traits.get_item_dimension();
    }

    template<orientation_t V, typename T, typename S>
    inline auto uniform_list<V, T, S>::get_item_size () const -> size_type{
      return super::traits.item_size;
    }

    template<orientation_t V, typename T, typename S>
    inline void uniform_list<V, T, S>::set_item_size (size_type item_size) {
      super::traits.item_size = item_size;
    }

    template<orientation_t V, typename T, typename S>
    inline void uniform_list<V, T, S>::set_item_size_and_background (size_type item_size, os::color background) {
      super::traits.item_size = item_size;
      super::set_background(background);
    }

    // --------------------------------------------------------------------------
    template<orientation_t V, typename S>
    inline linear_list<V, S>::linear_list (core::size::type item_size,
                                           os::color background,
                                           bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<orientation_t V, typename S>
    inline linear_list<V, S>::linear_list (linear_list&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t V, typename S>
    inline core::rectangle linear_list<V, S>::get_virtual_geometry (const core::rectangle&) const {
      core::rectangle place;
      super::super::traits.set_1(place, 0, super::super::traits.get_item_dimension() * super::get_count());
      super::super::traits.set_2(place, 0, 0);
      return place;
    }

    template<orientation_t V, typename S>
    core::size linear_list<V, S>::get_scroll_steps () const {
      core::size sz;
      super::super::traits.set_1(sz, super::super::traits.get_item_dimension());
      super::super::traits.set_2(sz, 1);
      return sz;
    }

    template<orientation_t V, typename S>
    void linear_list<V, S>::paint (draw::graphics& graph) {
      const core::rectangle area = super::client_geometry();
      core::rectangle place = area;

      draw::brush back_brush(super::get_background());

      const auto list_sz = super::traits.get_1(area.x2y2());
      const auto last = super::get_count();
      const auto isz = super::get_item_dimension();
      const auto sp1 = super::traits.get_1(super::get_scroll_pos());
      const auto sp2 = super::traits.get_2(super::get_scroll_pos());
      const auto first = static_cast<decltype(last)>(sp1 / isz);

      super::traits.set_1(place, -sp1 + isz * first, isz);
      super::traits.set_2(place, -sp2, super::traits.get_2(area.size()) + sp2);

      for (auto idx = first; (idx < last) && (super::traits.get_1(place.top_left()) < list_sz); ++idx) {
        super::draw_item(idx, graph, place, back_brush, super::get_item_state(static_cast<int>(idx)));
        super::traits.set_1(place, super::traits.get_1(place.top_left()) + isz, isz);
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

    template<orientation_t V, typename S>
    void linear_list<V, S>::handle_key (os::key_state state,
                                        os::key_symbol key,
                                        const std::string&) {
      if (core::control_key_bit_mask::is_set(state)) {
        switch (key) {
          case core::keys::a:
            super::select_all(event_source::keyboard);
            break;
        }
      } else {
        const bool shift_pressed = core::shift_key_bit_mask::is_set(state);
        if ((0 == state) || (core::state::shift == state)) {
          super::traits.handle_direction_key(*this, shift_pressed, key);
          switch (key) {
            case core::keys::page_up:
            case core::keys::numpad::page_up:
              super::set_selection(super::get_selection().get_first_index() -
                                   static_cast<int>(super::get_list_size() / super::get_item_dimension()),
                                   event_source::keyboard, shift_pressed);
              break;
            case core::keys::page_down:
            case core::keys::numpad::page_down:
              super::set_selection(super::get_selection().get_first_index() +
                                   static_cast<int>(super::get_list_size() / super::get_item_dimension()),
                                   event_source::keyboard, shift_pressed);
              break;
            case core::keys::home:
            case core::keys::numpad::home:
              super::set_selection(0, event_source::keyboard, shift_pressed);
              break;
            case core::keys::end:
            case core::keys::numpad::end:
              super::set_selection(static_cast<int>(super::get_count()) - 1,
                                   event_source::keyboard, shift_pressed);
              break;
            case core::keys::enter:
              if (!shift_pressed && super::has_selection()) {
                super::notify_selection_commit();
              }
              break;
          }
        }
      }
    }

    template<orientation_t V, typename S>
    void linear_list<V, S>::init () {
      super::on_paint(draw::paint(this, &linear_list::paint));
      super::on_any_key_down(util::bind_method(this, &linear_list::handle_key));
      super::on_show([&] () {
        super::invalidate();
      });
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
