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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"
#include "label.h"
#include "button.h"
#include "container.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    class drop_down {
    public:
      drop_down (win::container* m)
        : data(m) {
        init();
      }

      drop_down (win::container* m, const drop_down&)
        : data(m) {
        init();
      }

      drop_down (win::container* m, drop_down&&)
        : data(m) {
        init();
      }

      drop_down (const drop_down&) = delete;
      drop_down (drop_down&&) = delete;

      void operator= (const drop_down&) = delete;
      void operator= (drop_down&&) = delete;

      void layout (const core::size& s);
      void init (win::window* button);

      core::rectangle label_place (const core::size& s);
      core::rectangle button_place (const core::size& s);

    private:
      void init ();

      struct data {
        data (win::container* m)
          : main(m)
          , button(nullptr)
        {}

        win::container* main;
        win::window* button;

      } data;
    };

  } // layout

  namespace win {

    // --------------------------------------------------------------------------
    namespace paint {

      void drop_down_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           bool selected,
                           bool hilited);

      void drop_down_button (const draw::graphics& graph,
                             const core::rectangle& r,
                             const button_state& state,
                             bool is_open,
                             bool focused,
                             bool enabled);

    } // paint

    template<typename T>
    void drop_down_item_drawer (const T& t,
                                const draw::graphics& g,
                                const core::rectangle& r,
                                const draw::brush& b,
                                bool selected,
                                bool hilited) {
      paint::drop_down_item(g, r, b, convert_to_string(t),
                            selected, hilited);
    }

    // --------------------------------------------------------------------------
    template<typename T,
             void (D)(const T&,
                      const draw::graphics&,
                      const core::rectangle&,
                      const draw::brush&,
                      bool, bool) = drop_down_item_drawer<T>>
    class drop_down_list : public group_window<layout::drop_down> {
    public:
      typedef group_window<layout::drop_down> super;
      typedef vertical_list list_type;

      typedef T(get_data_t)(std::size_t);
      typedef std::function<get_data_t> data_provider;

      drop_down_list (core::size_type item_size = 20,
                      os::color background = color::white)
        : data(item_size, background)
        , me(this, &drop_down_list::handle_move)
      {
        init();
      }

      drop_down_list (const drop_down_list& rhs)
        : super(rhs)
        , data(rhs.data)
        , me(this, &drop_down_list::handle_move)
      {
        init();
      }

      drop_down_list (drop_down_list&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
        , me(this, &drop_down_list::handle_move)
      {
        init();
      }

      void init () {
        super::get_layout().init(&(data.button));

        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &drop_down_list::paint));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point&) {
          toggle_popup();
          super::take_focus();
          super::redraw_later();
        }));
        super::register_event_handler(REGISTER_FUNCTION, create_event(this, &drop_down_list::create_children));

        data.button.register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph) {
          paint::drop_down_button(graph,
                                  data.button.client_area(),
                                  data.button.get_state(),
                                  is_popup_visible(),
                                  data.button.has_focus(),
                                  data.button.is_enabled());
        }));
        data.button.register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &drop_down_list::toggle_popup));

        data.items.register_event_handler(REGISTER_FUNCTION, selection_changed_event(this, &drop_down_list::handle_selection_changed));
        data.button.register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*) {
          super::redraw_later();
        }));
        data.button.register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &drop_down_list::handle_key));

        data.items.set_drawer([&](std::size_t idx,
                              const draw::graphics& g,
                              const core::rectangle& r,
                              const draw::brush& b,
                              bool selected,
                              bool hilited) {
          D(data.source(idx), g, r, b, selected, hilited);
        });
      }

      void paint (const draw::graphics& graph) {
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

      void handle_key (os::key_state state,
                       os::key_symbol key,
                       const std::string& t) {
        if (is_popup_visible()) {
          if (key == keys::tab) {
            hide_popup();
          }
          data.items.handle_key(state, key, t);
        } else {
          switch (key) {
            case keys::down:
            case keys::numpad::down:
              show_popup();
              break;
          }
        }
      }

      void handle_selection_changed (event_source src) {
        int idx = data.items.get_selection();
        if (idx > -1) {
          data.selection = idx;
          super::redraw_later();
          if (src == event_source::mouse) {
            hide_popup();
          }
        }
      }

      T get_selected_item () const {
        int idx = get_selection();
        if (idx > -1) {
          return data.source(idx);
        }
        return T();
      }

      void set_data (const data_provider& d, std::size_t count) {
        data.source = d;
        data.items.set_count(count);
      }

      void set_data (data_provider&& d, std::size_t count) {
        std::swap(data.source, d);
        data.items.set_count(count);
      }

      core::rectangle get_popup_place () const {
        core::rectangle place = super::absolute_place();
        place.move_y(place.height());
        place.height(core::size_type(data.visible_items * data.items.get_item_size()));
        return place;
      }

      void show_popup () {
        if (!data.popup.is_valid()) {
          create_popup(get_popup_place());
        } else {
          data.popup.place(get_popup_place());
        }
        data.items.set_selection(data.selection, event_source::logic);
        data.items.make_selection_visible();
        data.popup.set_visible();
        data.button.redraw_later();
      }

      void hide_popup () {
        data.popup.set_visible(false);
        data.button.redraw_later();
      }

      void set_selection (int idx, event_source src) {
        data.selection = idx;
        if (is_popup_visible()) {
          data.items.set_selection(idx, src);
        }
      }

      void set_visible_items (int n) {
        data.visible_items = n;
        if (is_popup_visible()) {
          data.popup.place(get_popup_place());
        }
      }

      inline int get_selection () const {
        return data.selection;
      }

      inline int get_visible_items () const {
        return data.visible_items;
      }

      inline bool is_popup_visible () const {
        return data.popup.is_visible();
      }

      inline void toggle_popup () {
        if (is_popup_visible()) {
          hide_popup();
        } else {
          show_popup();
        }
      }

      inline void set_drawer (const std::function<win::list::draw_list_item>& drawer) {
        data.items.set_drawer(drawer);
      }

      inline void set_drawer (std::function<win::list::draw_list_item>&& drawer) {
        data.items.set_drawer(std::move(drawer));
      }

      inline void set_count (std::size_t n) {
        data.items.set_count(n);
      }

      ~drop_down_list () {
        auto* root = super::get_root();
        if (root) {
          root->unregister_event_handler(me);
        }
      }

    private:
      void create_children(window*, const core::rectangle& r) {
        data.button.create(*this, get_layout().button_place(r.size()));
        data.button.set_visible();
      }

      void create_popup (const core::rectangle& place) {
        data.popup.register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size& sz) {
          data.items.place(core::rectangle(sz));
        }));
        data.popup.register_event_handler(REGISTER_FUNCTION, create_event([&](window*, const core::rectangle& r) {
          data.items.create(data.popup, core::rectangle(r.size()));
          data.items.set_visible();
        }));
        data.popup.register_event_handler(REGISTER_FUNCTION, show_event([&]() {
          data.filter_id = global::register_message_filter([&](const core::event& e) -> bool {
            if (is_button_event_outside(data.popup, e)) {
              hide_popup();
              return true;
            }
            return false;
          });
        }));
        data.popup.register_event_handler(REGISTER_FUNCTION, hide_event([&]() {
          if (data.filter_id) {
            global::unregister_message_filter(data.filter_id);
          }
        }));
        data.popup.create(*this, place);

        auto* root = super::get_root();
        if (root) {
          root->register_event_handler(REGISTER_FUNCTION, me);
        }

      }

      void handle_move (const core::point&) {
        if (is_popup_visible()) {
            data.popup.place(get_popup_place());
          }
      }

      struct data {
        data (core::size_type item_size = 20,
              os::color background = color::white)
          : items(item_size, background, false)
          , selection(-1)
          , visible_items(5)
          , filter_id(-1)
        {}

        data_provider source;
        push_button button;
        popup_window popup;
        list_type items;
        int selection;
        int visible_items;
        int filter_id;

      } data;

      move_event me;

    };

  } // win

} // gui
