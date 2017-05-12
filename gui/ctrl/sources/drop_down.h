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
             int S = 20,
             os::color B = color::white,
             void (D)(const T&,
                      const draw::graphics&,
                      const core::rectangle&,
                      const draw::brush&,
                      bool, bool) = drop_down_item_drawer<T>>
    class drop_down_list : public group_window<layout::drop_down, B> {
    public:
      typedef group_window<layout::drop_down, B> super;
      typedef vlist<S, B> list_type;

      typedef T(get_data_t)(int);
      typedef std::function<get_data_t> data_provider;

      drop_down_list ()
        : items(false)
        , selection(-1)
        , visible_items(5)
      {
        super::get_layout().init(&button);

        super::register_event_handler(win::paint_event([&](const draw::graphics& graph) {
          draw::frame::sunken_deep_relief(graph, super::client_area());
          if (selection > -1) {
            D(data(selection), graph, super::get_layout().label_place(super::client_size()), B, false, false);
          }
        }));
        super::register_event_handler(win::create_event(core::bind_method(this, &drop_down_list::create_children)));

        button.register_event_handler(win::paint_event([&](const draw::graphics& graph) {
          paint::drop_down_button(graph, button, popup.is_visible());
        }));
        button.register_event_handler(win::button_clicked_event(
          core::bind_method(this, &drop_down_list::toggle_popup)));

        items.register_event_handler(selection_changed_event([&]() {
          int idx = items.get_selection();
          if (idx > -1) {
            selection = idx;
            super::redraw_later();
            show_popup(false);
          }
        }));

        items.set_drawer([&](int idx,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             bool selected,
                             bool hilited) {
          D(data(idx), g, r, b, selected, hilited);
        });
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

      void show_popup (bool show) {
        if (show) {
          core::rectangle place = super::absolute_place();
          place.move_y(place.height());
          place.height(core::size_type(visible_items * list_type::item_size));
          if (!popup.is_valid()) {
            create_popup(place);
          } else {
            popup.place(place);
          }
          items.set_selection(selection, false);
          items.make_selection_visible();
          popup.set_visible();
          button.redraw_later();
        } else {
          popup.set_visible(false);
          button.redraw_later();
        }
      }

      void set_selection (int idx) {
        selection = idx;
        if (popup.is_visible()) {
          items.set_selection(idx);
        }
      }

      void set_visible_items (int n) {
        visible_items = n;
        if (popup.is_visible()) {
          core::rectangle place = super::absolute_place();
          place.move_y(place.height());
          place.height(core::size_type(visible_items * list_type::item_size));
          popup.place(place);
        }
      }

      int get_selection () const {
        return selection;
      }

      inline int get_visible_items () const {
        return visible_items;
      }

      void toggle_popup () {
        show_popup(!popup.is_visible());
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

    private:
      void create_children(window*, const core::rectangle&) {
        button.create(*this);
        button.set_visible();
        layout();
      }

      void create_popup (const core::rectangle& place) {
        popup.register_event_handler(size_event([&](const core::size& sz) {
          items.place(core::rectangle(sz));
        }));
        popup.register_event_handler(create_event([&](window*, const core::rectangle& r) {
          items.create(popup, core::rectangle(r.size()));
          items.set_visible();
        }));
#ifdef X11
        popup.register_event_handler(mouse_enter_event([&]() {
          popup.uncapture_pointer();
        }));
        popup.register_event_handler(mouse_leave_event([&]() {
          if (popup.is_visible()) {
            popup.capture_pointer();
          }
        }));
        popup.register_event_handler(show_event([&]() {
          popup.capture_pointer();
        }));
        popup.register_event_handler(hide_event([&]() {
          popup.uncapture_pointer();
        }));
        popup.register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          if (!popup.client_area().is_inside(pt)) {
            show_popup(false);
          }
        }));
#endif // X11
#ifdef WIN32
        //items.register_event_handler(mouse_enter_event([&]() {
        //  LogInfo << "items.mouse_enter_event";
        //  items.uncapture_pointer();
        //}));
        //items.register_event_handler(mouse_leave_event([&]() {
        //  LogInfo << "items.mouse_leave_event";
        //  if (popup.is_visible()) {
        //    items.capture_pointer();
        //  }
        //}));
        popup.register_event_handler(show_event([&]() {
          items.capture_pointer();
        }));
        popup.register_event_handler(hide_event([&]() {
          items.uncapture_pointer();
        }));
        items.register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
          if (!popup.client_area().is_inside(pt)) {
            show_popup(false);
          }
        }));
#endif // WIN32
        popup.create(*this, place);
      }

      data_provider data;
      push_button button;
      popup_window popup;
      list_type items;
      int selection;
      int visible_items;

    };

  } // win

} // gui
