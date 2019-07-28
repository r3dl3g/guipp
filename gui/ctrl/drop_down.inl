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
 * @brief     C++ API: drop down list
 *
 * @file
 */

#pragma once


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    inline drop_down::drop_down (win::container* m)
      : data(m) {
      init();
    }

    inline drop_down::drop_down (win::container* m, const drop_down&)
      : data(m) {
      init();
    }

    inline drop_down::drop_down (win::container* m, drop_down&&)
      : data(m) {
      init();
    }

    inline drop_down::data::data (win::container* m)
      : main(m)
      , button(nullptr)
    {}

  } // layout

  // --------------------------------------------------------------------------
  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T, drop_down_drawer<T> D>
    inline drop_down_list<T, D>::drop_down_list (core::size::type item_size,
                                                 os::color background)
      : data(item_size, background)
      , me(basepp::bind_method(this, &drop_down_list::handle_move))
    {
      init();
    }

    template<typename T, drop_down_drawer<T> D>
    inline drop_down_list<T, D>::drop_down_list (const drop_down_list& rhs)
      : super(rhs)
      , data(rhs.data)
      , me(basepp::bind_method(this, &drop_down_list::handle_move))
    {
      init();
    }

    template<typename T, drop_down_drawer<T> D>
    inline drop_down_list<T, D>::drop_down_list (drop_down_list&& rhs)
      : super(std::move(rhs))
      , data(std::move(rhs.data))
      , me(basepp::bind_method(this, &drop_down_list::handle_move))
    {
      init();
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::init () {
      super::get_layout().init(&(data.button));

      super::on_paint(draw::paint(basepp::bind_method(this, &drop_down_list::paint)));
      super::on_lost_focus([&] (window*) {
        super::invalidate();
      });
      super::on_left_btn_down([&](os::key_state, const core::point &) {
        toggle_popup();
        super::take_focus();
      });

      super::on_wheel<orientation::vertical>(basepp::bind_method(this, &drop_down_list::handle_wheel));
      super::on_create(basepp::bind_method(this, &drop_down_list::create_children));

      data.button.on_paint(draw::paint([&](const draw::graphics & graph) {
        paint::drop_down_button(graph,
                                data.button.client_area(),
                                data.button.get_state(),
                                is_popup_visible());
      }));
      data.button.on_clicked(basepp::bind_method(this, &drop_down_list::toggle_popup));

      data.button.on_lost_focus([&] (window*) {
        super::invalidate();
      });
      data.button.on_any_key_down(basepp::bind_method(this, &drop_down_list::handle_key));

      data.items.on_selection_changed(basepp::bind_method(this, &drop_down_list::handle_selection_changed));
      data.items.set_drawer([&](std::size_t idx,
                                const draw::graphics & g,
                                const core::rectangle & r,
                                const draw::brush & b,
                                bool selected,
                                bool hilited) {
        D(data.source(idx), g, r, b, selected, hilited);
      });
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::paint (const draw::graphics& graph) {
      core::rectangle area = super::client_area();
      draw::frame::sunken_deep_relief(graph, area);
      bool has_f = data.button.has_focus();
      if (data.selection > -1) {
        D(data.source(data.selection),
          graph,
          super::get_layout().label_place(super::client_size()),
          data.items.get_background(),
          false,
          has_f);
      } else if (has_f) {
        draw::frame::dots(graph, area);
      }
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::handle_key (os::key_state state,
                                           os::key_symbol key,
                                           const std::string& t) {
      if (is_popup_visible()) {
        if (key == win::keys::tab) {
          hide_popup();
        }
        data.items.handle_key(state, key, t);
      } else {
        switch (key) {
        case win::keys::down:
        case win::keys::numpad::down:
          show_popup();
          break;
        }
      }
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::handle_selection_changed (event_source src) {
      int idx = data.items.get_selection();
      if (idx > -1) {
        data.selection = idx;
        super::invalidate();
        if (src == event_source::mouse) {
          hide_popup();
        }
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline auto drop_down_list<T, D>::get_selected_item() const->T {
      int idx = get_selection();
      if (idx > -1) {
        return data.source(idx);
      }
      return T();
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_selected_item (T v) {
      for (int i = 0; i < data.items.get_count(); ++i) {
        if (data.source(i) == v) {
          set_selection(i, event_source::logic);
          return;
        }
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_data (const data_provider& d, std::size_t count) {
      data.source = d;
      data.items.set_count(count);
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_data (data_provider&& d, std::size_t count) {
      std::swap(data.source, d);
      data.items.set_count(count);
    }

    template<typename T, drop_down_drawer<T> D>
    inline core::rectangle drop_down_list<T, D>::get_popup_place () const {
      core::rectangle place = super::absolute_place();
      place.move_y(place.height());
      place.height(core::size::type(data.visible_items * data.items.get_item_size()));
      return place;
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::show_popup () {
      if (!data.popup.is_valid()) {
        create_popup(get_popup_place());
      } else {
        data.popup.place(get_popup_place());
      }
      data.items.set_selection(data.selection, event_source::logic);
      data.items.make_selection_visible();
      data.popup.set_visible();
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::hide_popup () {
      data.popup.set_visible(false);
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_selection (int idx, event_source src) {
      data.selection = std::max(-1, std::min(idx, static_cast<int>(data.items.get_count())));
      if (is_popup_visible()) {
        data.items.set_selection(idx, src);
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_visible_items (int n) {
      data.visible_items = n;
      if (is_popup_visible()) {
        data.popup.place(get_popup_place());
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline int drop_down_list<T, D>::get_selection () const {
      return data.selection;
    }

    template<typename T, drop_down_drawer<T> D>
    inline int drop_down_list<T, D>::get_visible_items () const {
      return data.visible_items;
    }

    template<typename T, drop_down_drawer<T> D>
    inline bool drop_down_list<T, D>::is_popup_visible () const {
      return data.popup.is_visible();
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::toggle_popup () {
      if (is_popup_visible()) {
        hide_popup();
      } else {
        show_popup();
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_drawer (const std::function<list::draw_list_item>& drawer) {
      data.items.set_drawer(drawer);
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_drawer (std::function<list::draw_list_item>&& drawer) {
      data.items.set_drawer(std::move(drawer));
    }

    template<typename T, drop_down_drawer<T> D>
    inline void drop_down_list<T, D>::set_count (std::size_t n) {
      data.items.set_count(n);
    }

    template<typename T, drop_down_drawer<T> D>
    auto drop_down_list<T, D>::items () -> list_type& {
      return data.items;
    }

    template<typename T, drop_down_drawer<T> D>
    auto drop_down_list<T, D>::items () const -> const list_type& {
      return data.items;
    }

    template<typename T, drop_down_drawer<T> D>
    drop_down_list<T, D>::~drop_down_list () {
      auto* root = super::get_root();
      if (root) {
        root->unregister_event_handler<win::move_event>(me);
      }
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::create_children (window*, const core::rectangle& r) {
      data.button.create(*this, get_layout().button_place(r.size()));
      data.button.set_visible();
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::create_popup (const core::rectangle& place) {
      data.popup.on_size([&] (const core::size & sz) {
        data.items.place(core::rectangle(sz));
      });
      data.popup.on_create([&] (window *, const core::rectangle & r) {
        data.items.create(data.popup, core::rectangle(r.size()));
        data.items.set_visible();
      });
      data.popup.on_show([&] () {
        data.filter_id = win::global::register_message_filter([&] (const core::event & e)->bool {
          if (is_button_event_outside(data.popup, e)) {
            hide_popup();
            return true;
          }
          return false;
        });
      });
      data.popup.on_hide([&] () {
        if (data.filter_id) {
          win::global::unregister_message_filter(data.filter_id);
        }
      });
      data.popup.create(*this, place);

      auto* root = super::get_root();
      if (root) {
        root->on<win::move_event>(me);
      }
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::handle_move (const core::point&) {
      if (is_popup_visible()) {
        data.popup.place(get_popup_place());
      }
    }

    template<typename T, drop_down_drawer<T> D>
    void drop_down_list<T, D>::handle_wheel (const core::point::type delta, const core::point&) {
      if (!is_popup_visible()) {
        set_selection(get_selection() + static_cast<int>(delta), event_source::mouse);
        super::invalidate();
      }
    }

    template<typename T, drop_down_drawer<T> D>
    inline drop_down_list<T, D>::data::data (core::size::type item_size,
                                             os::color background)
      : items(item_size, background, false)
      , selection(-1)
      , visible_items(5)
      , filter_id(-1)
    {}

  } // ctrl

} // gui
