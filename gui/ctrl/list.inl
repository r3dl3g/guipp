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
      inline void list_base::create (win::container& parent,
                                     const core::rectangle& r) {
        super::create(clazz::get(), parent, r);
      }

      inline void list_base::create (win::container& parent,
                                     const core::rectangle& place,
                                     std::function<list_data_provider> data) {
        super::create(clazz::get(), parent, place);
        set_data(data);
      }

      template<typename U, list_item_drawer<U> D, typename C>
      inline void list_base::set_data (const C& dta) {
        data.items = indirect_list_data<U, D, C>(dta);
        super::invalidate();
      }

      template<typename U, list_item_drawer<U> D>
      inline void list_base::set_data (std::initializer_list<U> args) {
        data.items = const_list_data<U, D>(std::move(args));
        super::invalidate();
      }

      inline void list_base::set_data (const std::function<list_data_provider>& dta) {
        data.items = dta;
        super::invalidate();
      }

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
      template<typename T, orientation_t O>
      inline oriented_list<T, O>::oriented_list (os::color background,
                                                 bool grab_focus)
        : super(background, grab_focus)
      {
        init();
      }

      template<typename T, orientation_t O>
      inline oriented_list<T, O>::oriented_list (oriented_list&& rhs) noexcept
        : super(std::move(rhs))
        , traits(std::move(rhs.traits))
      {
        init();
      }

      template<typename T, orientation_t O>
      auto oriented_list<T, O>::get_list_size () const -> dim_type {
        return otraits::get_1(super::client_size());
      }

      template<typename T, orientation_t O>
      int oriented_list<T, O>::get_index_at_point (const core::point& pt) {
        auto rect = super::client_geometry();
        if (rect.is_inside(pt)) {
          return traits.get_index_at_point(rect, pt - rect.position(),
                                           super::get_scroll_pos(), super::get_count());
        }
        return -1;
      }

      template<typename T, orientation_t O>
      core::rectangle oriented_list<T, O>::get_geometry_of_index (int idx) {
        if (super::is_valid_idx(idx)) {
          return traits.get_geometry_of_index(super::client_geometry(), idx,
                                              super::get_scroll_pos());
        }
        return core::rectangle::zero;
      }

      template<typename T, orientation_t O>
      void oriented_list<T, O>::make_entry_visible (int sel_idx,
                                                core::selection_adjustment adjust) {
        const auto list_size = super::client_size();
        const auto scroll_pos = get_scroll_offset();

        const auto new_pos = core::get_adjusted_scroll_position(otraits::get_1(list_size),
                                                                traits.get_line_size(),
                                                                scroll_pos,
                                                                traits.get_offset_of_index(list_size, sel_idx),
                                                                adjust);
        if (new_pos != scroll_pos) {
          set_scroll_offset(traits.get_list_dimension(*this), new_pos);
        }
      }

      template<typename T, orientation_t O>
      void oriented_list<T, O>::handle_mouse_move (os::key_state keys,
                                               const core::native_point& pt) {
        const auto r = super::surface_geometry();
        if (core::left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
          if ((super::get_last_mouse_point() != core::native_point::undefined) &&
              (super::get_last_mouse_point() != pt)) {
            super::set_cursor(win::cursor::move());
            auto delta = otraits::get_1(super::get_last_mouse_point()) - otraits::get_1(pt);
            set_scroll_offset(traits.get_list_dimension(*this),
                              get_scroll_offset() + core::global::scale_from_native<pos_t>(delta));
            super::set_state().moved(true);
          }
          super::data.last_mouse_point = pt;
        } else {
          super::set_hilite(get_index_at_point(super::surface_to_client(pt)));
        }
      }

      template<typename T, orientation_t O>
      void oriented_list<T, O>::init () {
        super::on_mouse_move(util::bind_method(this, &oriented_list::handle_mouse_move));
      }

      template<typename T, orientation_t O>
      inline auto oriented_list<T, O>::get_scroll_offset () const -> dim_type {
        return otraits::get_1(super::get_scroll_pos());
      }

      template<typename T, orientation_t O>
      inline void oriented_list<T, O>::set_scroll_offset (dim_type pos_max,
                                                          dim_type pos) {
        auto value = std::max(dim_type(0), std::min(pos, static_cast<dim_type>(pos_max - otraits::get_1(super::client_size()))));
        auto pt = super::get_scroll_pos();
        otraits::set_1(pt, value);
        super::set_scroll_pos(pt);
        super::notify_scroll(value);
      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t O, typename S>
      inline selectable_list<T, O, S>::selectable_list (os::color background,
                                                        bool grab_focus)
        : super(background, grab_focus)
        , adjustment(core::selection_adjustment::next)
      {
        init();
      }

      template<typename T, orientation_t O, typename S>
      inline selectable_list<T, O, S>::selectable_list (selectable_list&& rhs) noexcept
        : super(std::move(rhs))
        , selection(std::move(rhs.selection))
        , adjustment(rhs.adjustment)
      {
        init();
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::clear_selection (event_source notify) {
        if (selection.has_selection()) {
          selection.clear_selection();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
            super::invalidate();
          }
        }
      }

      template<typename T, orientation_t O, typename S>
      inline auto selectable_list<T, O, S>::get_selection () const -> const selector_type& {
        return selection;
      }

      template<typename T, orientation_t O, typename S>
      inline auto selectable_list<T, O, S>::get_selection () -> selector_type& {
        return selection;
      }

      template<typename T, orientation_t O, typename S>
      inline bool selectable_list<T, O, S>::has_selection () const {
        return selection.has_selection();
      }

      template<typename T, orientation_t O, typename S>
      inline item_state selectable_list<T, O, S>::get_item_state (int idx) const {
        return item_state(super::get_hilite() == idx,
                          selection.is_selected(idx),
                          !super::is_enabled());
      }

      template<typename T, orientation_t O, typename S>
      inline bool selectable_list<T, O, S>::can_select_multi () const {
        return selection.is_multi_select();
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::set_selection_adjustment (core::selection_adjustment adjust) {
        adjustment = adjust;
      }

      template<typename T, orientation_t O, typename S>
      core::selection_adjustment selectable_list<T, O, S>::get_selection_adjustment () const {
        return adjustment;
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::make_selection_visible () {
        if (has_selection()) {
          super::make_entry_visible(get_selection().get_first_index(),
                                    get_selection_adjustment());
        }
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::set_selection (int sel, event_source notify, bool add) {
        const int new_selection = std::min(std::max(0, sel), 
                                           static_cast<int>(super::get_count() - 1));
        if (add) {
          selection.expand_to(new_selection);
        } else {
          clear_selection(event_source::logic);
          selection.set_selected(new_selection);
        }
        make_selection_visible();
        super::invalidate();
        if (notify != event_source::logic) {
          super::notify_selection_changed(notify);
        }
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::select (int sel, event_source notify) {
        const int new_selection = std::min(std::max(0, sel),
                                           static_cast<int>(super::get_count() - 1));
        if (!selection.is_selected(new_selection)) {
          selection.set_selected(new_selection);
          make_selection_visible();
          super::invalidate();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
          }
        }
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::select_all (event_source notify) {
        selection.select_range(0, static_cast<int>(super::get_count()) - 1);
        super::invalidate();
        if (notify != event_source::logic) {
          super::notify_selection_changed(notify);
        }
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::unselect (int sel, event_source notify) {
        if (selection.is_selected(sel)) {
          selection.set_unselected(sel);
          super::invalidate();
          if (notify != event_source::logic) {
            super::notify_selection_changed(notify);
          }
        }
      }

      template<typename T, orientation_t O, typename S>
      bool selectable_list<T, O, S>::try_to_select (int sel, event_source notify) {
        if ((sel >= 0) && (sel < super::get_count())) {
          set_selection(sel, notify);
          return true;
        }
        return false;
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::handle_left_btn_up (os::key_state state,
                                                      const core::native_point& pt) {
        if (!super::is_moved() && 
            (super::get_last_mouse_point() != core::native_point::undefined)) {
          const int new_selection = super::get_index_at_point(super::surface_to_client(pt));
  #ifdef GUIPP_BUILD_FOR_MOBILE
          if (get_selection().is_selected(new_selection)) {
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

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::init () {
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

        super::on_left_btn_up(util::bind_method(this,
                                                &selectable_list::handle_left_btn_up));
        super::on_any_key_down(util::bind_method(this, &selectable_list::handle_key));
      }

      template<typename T, orientation_t O, typename S>
      void selectable_list<T, O, S>::handle_key (os::key_state state,
                                                 os::key_symbol key,
                                                 const std::string&) {
        if (core::control_key_bit_mask::is_set(state)) {
          switch (key) {
            case core::keys::a:
              select_all(event_source::keyboard);
              break;
          }
        } else {
          const int step = super::traits.get_direction_step(key, super::client_size());
          const bool shift_pressed = core::shift_key_bit_mask::is_set(state);
          if (step) {
            set_selection(get_selection().get_last_selected_index() + step,
                          event_source::keyboard, shift_pressed);
          }
          switch (key) {
            case core::keys::page_up:
            case core::keys::numpad::page_up:
              set_selection(get_selection().get_first_index() - 
                              static_cast<int>(super::traits.get_items_per_page(super::client_size(),
                                get_selection().get_last_selected_index())),
                            event_source::keyboard, shift_pressed);
              break;
            case core::keys::page_down:
            case core::keys::numpad::page_down:
              set_selection(get_selection().get_first_index() +
                              static_cast<int>(super::traits.get_items_per_page(super::client_size(),
                                get_selection().get_last_selected_index())),
                            event_source::keyboard, shift_pressed);
              break;
            case core::keys::home:
            case core::keys::numpad::home:
              set_selection(0, event_source::keyboard, shift_pressed);
              break;
            case core::keys::end:
            case core::keys::numpad::end:
              set_selection(static_cast<int>(super::get_count()) - 1,
                                  event_source::keyboard, shift_pressed);
              break;
            case core::keys::enter:
              if (!shift_pressed && has_selection()) {
                super::notify_selection_commit();
              }
              break;
          }
        }
      }

      // --------------------------------------------------------------------------
    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T, orientation_t O, typename S>
    inline uniform_list<T, O, S>::uniform_list (os::color background,
                                                bool grab_focus)
      : super(background, grab_focus)
    {}

    template<typename T, orientation_t O, typename S>
    inline auto uniform_list<T, O, S>::get_item_dimension () const -> dim_type {
      return super::traits.get_item_dimension();
    }

    template<typename T, orientation_t O, typename S>
    inline auto uniform_list<T, O, S>::get_item_size () const -> size_type{
      return super::traits.item_size;
    }

    template<typename T, orientation_t O, typename S>
    inline void uniform_list<T, O, S>::set_item_size (size_type item_size) {
      super::traits.item_size = item_size;
    }

    template<typename T, orientation_t O, typename S>
    inline void uniform_list<T, O, S>::set_item_size_and_background (size_type item_size, os::color background) {
      super::traits.item_size = item_size;
      super::set_background(background);
    }

    // --------------------------------------------------------------------------
    template<orientation_t O>
    inline linear_list_traits<O>::linear_list_traits (dim_type item_size)
      : item_size(item_size)
    {}

    template<orientation_t O>
    inline int linear_list_traits<O>::get_index_at_point (const core::rectangle& /*list_size*/,
                                                          const core::point& pt,
                                                          const core::point& scroll_pos,
                                                          size_t count) const {
      const auto idx = static_cast<int>((otraits::get_1(pt) + otraits::get_1(scroll_pos)) / item_size);
      return idx < count ? idx : -1;
    }

    template<orientation_t O>
    inline core::rectangle linear_list_traits<O>::get_geometry_of_index (const core::rectangle& list_size,
                                                                         int idx,
                                                                         const core::point& scroll_pos) const {
      core::rectangle place;
      otraits::set_1(place, otraits::get_1(list_size.position()) + item_size * idx - otraits::get_1(scroll_pos), item_size);
      otraits::set_2(place, otraits::get_2(list_size.position()) - otraits::get_2(scroll_pos), otraits::get_2(list_size.size()));
      return place;
    }

    template<orientation_t O>
    inline auto linear_list_traits<O>::get_offset_of_index (const core::size&, int idx) const -> dim_type {
      return item_size * idx;
    }

    template<orientation_t O>
    inline auto linear_list_traits<O>::get_list_dimension (const detail::list_base& list) const -> dim_type {
      return get_item_dimension() * list.get_count();
    }

    template<orientation_t O>
    inline auto linear_list_traits<O>::get_line_size () const -> dim_type {
      return item_size;
    }

    template<orientation_t O>
    inline auto linear_list_traits<O>::get_item_dimension () const -> dim_type {
      return item_size;
    }

    template<orientation_t O>
    std::size_t linear_list_traits<O>::get_items_per_page (const core::size& page_size, int) const {
      return static_cast<int>(otraits::get_1(page_size) / get_item_dimension());
    }

    template<orientation_t O>
    int linear_list_traits<O>::get_direction_step (os::key_symbol key, const core::size&) const {
      return get_step(key);
    }

    // --------------------------------------------------------------------------
    template<orientation_t O, typename S>
    inline linear_list<O, S>::linear_list (core::size::type item_size,
                                           os::color background,
                                           bool grab_focus)
      : super(background, grab_focus)
    {
      super::set_item_size(item_size);
      init();
    }

    template<orientation_t O, typename S>
    inline linear_list<O, S>::linear_list (linear_list&& rhs) noexcept
      : super(std::move(rhs))
    {
      init();
    }

    template<orientation_t O, typename S>
    inline core::rectangle linear_list<O, S>::get_virtual_geometry (const core::rectangle& area) const {
      core::rectangle place;
      super::otraits::set_1(place, 0, super::traits.get_list_dimension(*this));
      super::otraits::set_2(place, 0, super::otraits::get_2(area.size()));
      return place;
    }

    template<orientation_t O, typename S>
    core::size linear_list<O, S>::get_scroll_steps () const {
      core::size sz;
      super::otraits::set_1(sz, super::get_item_dimension());
      super::otraits::set_2(sz, 1);
      return sz;
    }

    template<orientation_t O, typename S>
    void linear_list<O, S>::paint (draw::graphics& graph) {
      const core::rectangle area = super::client_geometry();
      core::rectangle place = area;

      draw::brush back_brush(super::get_background());

      const auto list_sz = super::otraits::get_1(area.x2y2());
      const auto last = super::get_count();
      const auto isz = super::get_item_dimension();
      const auto sp1 = super::otraits::get_1(super::get_scroll_pos());
      const auto sp2 = super::otraits::get_2(super::get_scroll_pos());
      const auto first = static_cast<decltype(last)>(sp1 / isz);

      super::otraits::set_1(place, -sp1 + isz * first, isz);
      super::otraits::set_2(place, -sp2, super::otraits::get_2(area.size()) + sp2);

      for (auto idx = first; (idx < last) && (super::otraits::get_1(place.top_left()) < list_sz); ++idx) {
        super::draw_item(idx, graph, place, back_brush, super::get_item_state(static_cast<int>(idx)));
        super::otraits::set_1(place, super::otraits::get_1(place.top_left()) + isz, isz);
      }

      const auto pos = super::otraits::get_1(place.top_left());
      if (pos < list_sz) {
        super::otraits::set_1(place, pos, list_sz - pos);
        graph.fill(draw::rectangle(place), back_brush);
      }

      if (super::is_focused()) {
        draw::frame::dots(graph, area);
      }

    }


    template<orientation_t O, typename S>
    void linear_list<O, S>::init () {
      super::on_paint(draw::paint(this, &linear_list::paint));
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
