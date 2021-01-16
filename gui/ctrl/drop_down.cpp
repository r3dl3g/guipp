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
 * @brief     C++ API: drop down list
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/drop_down.h>


namespace gui {

  namespace layout {

    core::rectangle drop_down::label_place (const core::rectangle& r) {
      core::size::type h = r.height() - 4;
      return core::rectangle(r.top_left() + core::point(2, 2), core::size(r.width() - h - 4, h));
    }

    namespace detail {

      template<look::look_and_feel_t L = look::system_look_and_feel>
      inline core::rectangle button_place (const core::rectangle& r) {
        core::size::type h = r.height();
        return core::rectangle(r.top_left() + core::point(r.width() - h, 0), core::size(h, h));
      }

      template<>
      inline core::rectangle button_place<look::look_and_feel_t::w95> (const core::rectangle& r) {
        core::size::type h = r.height() - 4;
        return core::rectangle(r.top_left() + core::point(r.width() - h - 2, 2), core::size(h, h));
      }

      template<>
      inline core::rectangle button_place<look::look_and_feel_t::w10> (const core::rectangle& r) {
        core::size::type h = r.height() - 2;
        return core::rectangle(r.top_left() + core::point(r.width() - h - 1, 1), core::size(h, h));
      }

      template<>
      inline core::rectangle button_place<look::look_and_feel_t::osx> (const core::rectangle& r) {
        core::size::type h = r.height();
        return core::rectangle(r.top_left() + core::point(r.width() - 17, 0), core::size(17, h));
      }

    } // namespace detail

    core::rectangle drop_down::button_place (const core::rectangle& r) {
      return detail::button_place<>(r);
    }

  } // layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    drop_down_list::drop_down_list (core::size::type item_size,
                                    os::color background)
      : data(item_size, background)
      , me(util::bind_method(this, &drop_down_list::handle_move))
    {
      init();
    }

    drop_down_list::drop_down_list (drop_down_list&& rhs) noexcept
      : super(std::move(rhs))
      , data(std::move(rhs.data))
      , me(util::bind_method(this, &drop_down_list::handle_move))
    {
      init();
    }

    void drop_down_list::init () {

      super::on_paint(draw::paint(util::bind_method(this, &drop_down_list::paint)));

      super::on_lost_focus([&] () {
        super::invalidate();
      });

      super::on_clicked(util::bind_method(this, &drop_down_list::toggle_popup));
      super::on_wheel<orientation_t::vertical>(util::bind_method(this, &drop_down_list::handle_wheel));
      super::on_any_key_down(util::bind_method(this, &drop_down_list::handle_key));
      data.items.on_selection_changed(util::bind_method(this, &drop_down_list::handle_selection_changed));
    }

    void drop_down_list::paint (const draw::graphics& graph) {
      core::rectangle area = super::client_area();
      look::drop_down(graph, area, get_state());
      look::drop_down_button(graph,
                             layout::drop_down::button_place(area),
                             get_state(),
                             is_popup_visible());
      if (data.selection > -1) {
        data.items.draw_item(data.selection,
                             graph,
                             layout::drop_down::label_place(area),
                             data.items.get_background(),
                             item_state(is_focused(), false, !is_enabled()));
      }
    }

    void drop_down_list::handle_key (os::key_state state,
                                           os::key_symbol key,
                                           const std::string& t) {
      if (is_popup_visible()) {
        if (key == core::keys::tab) {
          hide_popup();
        }
        data.items.handle_key(state, key, t);
      } else {
        switch (key) {
        case core::keys::down:
        case core::keys::numpad::down:
          show_popup();
          break;
        }
      }
    }

    void drop_down_list::handle_selection_changed (event_source src) {
      int idx = data.items.get_selection();
      if (idx > -1) {
        data.selection = idx;
        super::invalidate();
        if (src == event_source::mouse) {
          hide_popup();
        }
      }
    }

    core::rectangle drop_down_list::get_popup_place () const {
      core::rectangle place = super::absolute_place();
      place.move_y(place.height());
      place.height(core::size::type(data.visible_items * data.items.get_item_dimension()));
      return place;
    }

    void drop_down_list::show_popup () {
      if (!data.popup.is_valid()) {
        create_popup(get_popup_place());
      } else {
        data.popup.place(get_popup_place());
      }
      data.items.set_selection(data.selection, event_source::logic);
      data.items.make_selection_visible();
      data.popup.set_visible();
    }

    void drop_down_list::hide_popup () {
      data.popup.set_visible(false);
      invalidate();
    }

    void drop_down_list::set_selection (int idx, event_source src) {
      data.selection = std::max(-1, std::min(idx, static_cast<int>(data.items.get_count())));
      if (is_popup_visible()) {
        data.items.set_selection(idx, src);
      }
    }

    void drop_down_list::set_visible_items (int n) {
      data.visible_items = n;
      if (is_popup_visible()) {
        data.popup.place(get_popup_place());
      }
    }

    int drop_down_list::get_selection () const {
      return data.selection;
    }

    int drop_down_list::get_visible_items () const {
      return data.visible_items;
    }

    bool drop_down_list::is_popup_visible () const {
      return data.popup.is_visible();
    }

    void drop_down_list::toggle_popup () {
      if (is_popup_visible()) {
        hide_popup();
      } else {
        show_popup();
      }
    }

    auto drop_down_list::items () -> list_type& {
      return data.items;
    }

    auto drop_down_list::items () const -> const list_type& {
      return data.items;
    }

    drop_down_list::~drop_down_list () {
      auto parent = super::get_parent();
      if (parent) {
        auto* root = parent->get_overlapped_window();
        if (root) {
          root->template unregister_event_handler<win::move_event>(me);
        }
      }
    }

    void drop_down_list::create_popup (const core::rectangle& place) {
      data.popup.on_size([&] (const core::size & sz) {
        data.items.place(core::rectangle(sz));
      });
      data.popup.on_create([&] () {
        data.items.create(data.popup, data.popup.client_area());
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
          data.filter_id = 0;
        }
      });
      data.popup.create(*super::get_parent(), place);

      auto* root = super::get_parent()->get_overlapped_window();
      if (root) {
        root->template on<win::move_event>(me);
      }
    }

    void drop_down_list::handle_move (const core::point&) {
      if (is_popup_visible()) {
        data.popup.place(get_popup_place());
      }
    }

    void drop_down_list::handle_wheel (const core::point::type delta, const core::point&) {
      if (!is_popup_visible()) {
        set_selection(get_selection() + static_cast<int>(delta), event_source::mouse);
        super::invalidate();
      }
    }

    drop_down_list::data::data (core::size::type item_size,
                                os::color background)
      : items(item_size, background, false)
      , selection(-1)
      , visible_items(5)
      , filter_id(-1)
    {}

  } // ctrl

} // gui
