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
 * @brief     drop down list control
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/ctrl/look/drop_down.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    struct GUIPP_CTRL_EXPORT drop_down {

      static core::rectangle label_geometry (const core::rectangle&) ;
      static core::rectangle button_geometry (const core::rectangle&) ;

    };

  } // layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T>
    using drop_down_drawer = void (*)(const T&,
                                      draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      item_state);

    template<typename T>
    void default_drop_down_drawer (const T& t,
                                   draw::graphics& g,
                                   const core::rectangle& r,
                                   const draw::brush& b,
                                   item_state state) {
      look::drop_down_item(g, r, b, util::string::convert::from(t), state);
    }

    template<typename T, list_item_drawer<T> D = default_drop_down_drawer<T>>
    using const_dropdown_data = const_list_data<T, D>;

    template<typename T, list_item_drawer<T> D = default_drop_down_drawer<T>, typename V = std::vector<T>>
    using indirect_dropdown_data = indirect_list_data<T, D, V>;

    // --------------------------------------------------------------------------
    template<core::os::ui_t T = core::os::system_ui>
    struct drop_down_list_defaults {};

    template<>
    struct drop_down_list_defaults<core::os::ui_t::desktop> {
      static constexpr core::size::type item_size = 20;
    };

    template<>
    struct drop_down_list_defaults<core::os::ui_t::mobile> {
      static constexpr core::size::type item_size = 80;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT drop_down_list : public push_button {
    public:
      typedef push_button super;
      typedef vertical_scrollable_list list_type;

      explicit drop_down_list (core::size::type item_size = drop_down_list_defaults<>::item_size,
                               os::color background = color::white);
      drop_down_list (drop_down_list&& rhs) noexcept;

      void init ();

      void paint (draw::graphics& graph);

      void handle_key (os::key_state state,
                       os::key_symbol key,
                       const std::string& t);

      void handle_selection_changed (event_source src);

      template<typename F, list_item_drawer<F> D = default_drop_down_drawer<F>, typename V = std::vector<F>>
      void set_data (const V& data);

      template<typename F, list_item_drawer<F> D = default_drop_down_drawer<F>>
      void set_data (std::initializer_list<F> args);

      void set_data (std::function<list_data_provider> dta);

      core::rectangle get_popup_geometry () const;

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
      void create_popup (const core::rectangle& place);

      void handle_move (const core::point&);
      void handle_wheel (core::native_point::type delta, const core::native_point&);

      struct data {
        explicit data (core::size::type item_size = drop_down_list_defaults<>::item_size,
                       os::color background = color::white);

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
