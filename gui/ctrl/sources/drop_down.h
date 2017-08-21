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

namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    class drop_down {
    public:
      drop_down (win::container* m);

      void layout (const core::size& s);
      void init (win::window* button);

      core::rectangle label_place (const core::size& s);
      core::rectangle button_place (const core::size& s);

    private:
      win::container* main;
      win::window* button;
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
                             bool is_open,
                             bool enabled = true,
                             bool focused = false,
                             bool hilited = false,
                             bool pushed = false);

      void drop_down_button (const draw::graphics& g,
                             const win::button& btn,
                             bool is_open);

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
      typedef vlist list_type;

      typedef T(get_data_t)(std::size_t);
      typedef std::function<get_data_t> data_provider;

      drop_down_list (core::size_type item_size = 20,
                      os::color background = color::white)
        : items(item_size, background, false)
        , selection(-1)
        , visible_items(5)
        , filter_id(-1)
        , me([&](const core::point&) {
          if (is_popup_visible()) {
            popup.place(get_popup_place());
          }
        })
      {
        super::get_layout().init(&button);

        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &drop_down_list::paint));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point&) {
          toggle_popup();
          super::take_focus();
          super::redraw_later();
        }));
        super::register_event_handler(REGISTER_FUNCTION, create_event(this, &drop_down_list::create_children));

        button.register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph) {
          paint::drop_down_button(graph, button, is_popup_visible());
        }));
        button.register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &drop_down_list::toggle_popup));

        items.register_event_handler(REGISTER_FUNCTION, selection_changed_event(this, &drop_down_list::handle_selection_changed));
        button.register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*) {
          super::redraw_later();
        }));
        button.register_event_handler(REGISTER_FUNCTION, key_down_event(this, &drop_down_list::handle_key));

        items.set_drawer([&](std::size_t idx,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             bool selected,
                             bool hilited) {
          D(data(idx), g, r, b, selected, hilited);
        });
      }

      void paint (const draw::graphics& graph) {
        core::rectangle area = super::client_area();
        draw::frame::sunken_deep_relief(graph, area);
        bool has_f = button.has_focus();
        if (selection > -1) {
          D(data(selection), graph, super::get_layout().label_place(super::client_size()), items.get_background(), false, has_f);
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
          items.handle_key(state, key, t);
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
        int idx = items.get_selection();
        if (idx > -1) {
          selection = idx;
          super::redraw_later();
          if (src == event_source::mouse) {
            hide_popup();
          }
        }
      }

      T get_selected_item () const {
        int idx = get_selection();
        if (idx > -1) {
          return data(idx);
        }
        return T();
      }

      void set_data (const data_provider& d, std::size_t count) {
        data = d;
        items.set_count(count);
      }

      void set_data (data_provider&& d, std::size_t count) {
        std::swap(data, d);
        items.set_count(count);
      }

      core::rectangle get_popup_place () const {
        core::rectangle place = super::absolute_place();
        place.move_y(place.height());
        place.height(core::size_type(visible_items * items.get_item_size()));
        return place;
      }

      void show_popup () {
        if (!popup.is_valid()) {
          create_popup(get_popup_place());
        } else {
          popup.place(get_popup_place());
        }
        items.set_selection(selection, event_source::logic);
        items.make_selection_visible();
        popup.set_visible();
        button.redraw_later();
      }

      void hide_popup () {
        popup.set_visible(false);
        button.redraw_later();
      }

      void set_selection (int idx, event_source src) {
        selection = idx;
        if (is_popup_visible()) {
          items.set_selection(idx, src);
        }
      }

      void set_visible_items (int n) {
        visible_items = n;
        if (is_popup_visible()) {
          popup.place(get_popup_place());
        }
      }

      inline int get_selection () const {
        return selection;
      }

      inline int get_visible_items () const {
        return visible_items;
      }

      inline bool is_popup_visible () const {
        return popup.is_visible();
      }

      inline void toggle_popup () {
        if (is_popup_visible()) {
          hide_popup();
        } else {
          show_popup();
        }
      }

      inline void set_drawer (const std::function<win::list::draw_list_item>& drawer) {
        items.set_drawer(drawer);
      }

      inline void set_drawer (std::function<win::list::draw_list_item>&& drawer) {
        items.set_drawer(std::move(drawer));
      }

      inline void set_count (std::size_t n) {
        items.set_count(n);
      }

      ~drop_down_list () {
        auto* root = super::get_root();
        if (root) {
          root->unregister_event_handler(me);
        }
      }

    private:
      void create_children(window*, const core::rectangle&) {
        button.create(*this);
        button.set_visible();
        super::layout();
      }

      void create_popup (const core::rectangle& place) {
        popup.register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size& sz) {
          items.place(core::rectangle(sz));
        }));
        popup.register_event_handler(REGISTER_FUNCTION, create_event([&](window*, const core::rectangle& r) {
          items.create(popup, core::rectangle(r.size()));
          items.set_visible();
        }));
        popup.register_event_handler(REGISTER_FUNCTION, show_event([&]() {
          filter_id = global::register_message_filter([&](const core::event& e) -> bool {
            if (is_button_event_outside(popup, e)) {
              hide_popup();
              return true;
            }
            return false;
          });
        }));
        popup.register_event_handler(REGISTER_FUNCTION, hide_event([&]() {
          if (filter_id) {
            global::unregister_message_filter(filter_id);
          }
        }));
        popup.create(*this, place);

        auto* root = super::get_root();
        if (root) {
          root->register_event_handler(REGISTER_FUNCTION, me);
        }

      }

      data_provider data;
      push_button button;
      popup_window popup;
      list_type items;
      int selection;
      int visible_items;
      int filter_id;
      const move_event me;

    };

  } // win

} // gui
