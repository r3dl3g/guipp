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
// Library includes
//
#include <gui/layout/layout_container.h>
#include <gui/ctrl/list.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    class GUIPP_CTRL_EXPORT drop_down {
    public:
      void layout (const core::rectangle&) const;
      void init (win::container*, win::widget*);

      core::rectangle label_place (const core::rectangle&) const;
      core::rectangle button_place (const core::rectangle&) const;

    private:
      struct data {
        data ();

        win::container* main;
        win::widget* button;

      } data;
    };

  } // layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace paint {

      GUIPP_CTRL_EXPORT void drop_down_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           item_state state);

      GUIPP_CTRL_EXPORT void drop_down_button (const draw::graphics& graph,
                             const core::rectangle& r,
                             const button_state& state,
                             bool is_open);

    } // paint

    // --------------------------------------------------------------------------
    template<typename T>
    using drop_down_drawer = void (*)(const T&,
                                      const draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      item_state);

    template<typename T>
    void default_drop_down_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& r,
                                   const draw::brush& b,
                                   item_state state) {
      paint::drop_down_item(g, r, b, util::string::convert::from(t), state);
    }

    template<typename T, list_item_drawer<T> D = default_drop_down_drawer<T>>
    using const_dropdown_data = const_list_data<T, D>;

    template<typename T, list_item_drawer<T> D = default_drop_down_drawer<T>>
    using indirect_dropdown_data = indirect_list_data<T, D>;

    // --------------------------------------------------------------------------
    class GUIPP_LAYOUT_EXPORT drop_down_list : public win::group_window<layout::drop_down> {
    public:
      typedef win::group_window<layout::drop_down> super;
      typedef vertical_list list_type;

      drop_down_list (core::size::type item_size = 20,
                      os::color background = color::white);
      drop_down_list (drop_down_list&& rhs);

      void init ();

      void paint (const draw::graphics& graph);

      void handle_key (os::key_state state,
                       os::key_symbol key,
                       const std::string& t);

      void handle_selection_changed (event_source src);

      template<typename F>
      void set_data (const std::vector<F>& data);

      template<typename F>
      void set_data (std::initializer_list<F> args);

      void set_data (std::function<list_data_provider> data);

      core::rectangle get_popup_place () const;

      void show_popup ();
      void hide_popup ();

      void set_selection (int idx, event_source src);
      void set_visible_items (int n);

      int get_selection () const;
      int get_visible_items () const;

      bool is_popup_visible () const;

      void toggle_popup ();

      list_type& items ();
      const list_type& items () const;

      ~drop_down_list ();

    private:
      void create_children (window*, const core::rectangle& r);
      void create_popup (const core::rectangle& place);

      void handle_move (const core::point&);
      void handle_wheel (const core::point::type delta, const core::point&);

      struct data {
        data (core::size::type item_size = 20,
              os::color background = color::white);

        push_button button;
        win::popup_window popup;
        list_type items;
        int selection;
        int visible_items;
        int filter_id;

      } data;

      win::move_event::function me;

    };

  } // ctrl

} // gui

#include <gui/ctrl/drop_down.inl>
