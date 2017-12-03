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
#include <gui/win/layout_container.h>
#include <gui/ctrl/list.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    class GUIPP_EXPORT drop_down {
    public:
      drop_down (win::container* m);
      drop_down (win::container* m, const drop_down&);
      drop_down (win::container* m, drop_down&&);
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
        data (win::container* m);

        win::container* main;
        win::window* button;

      } data;
    };

  } // layout

  namespace win {

    // --------------------------------------------------------------------------
    namespace paint {

      GUIPP_EXPORT void drop_down_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           bool selected,
                           bool hilited);

      GUIPP_EXPORT void drop_down_button (const draw::graphics& graph,
                             const core::rectangle& r,
                             const button_state& state,
                             bool is_open);

    } // paint

    // --------------------------------------------------------------------------
    template<typename T>
    void default_drop_down_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& r,
                                   const draw::brush& b,
                                   bool selected,
                                   bool hilited) {
      paint::drop_down_item(g, r, b, convert_to_string(t),
                            selected, hilited);
    }

    template<typename T>
    using drop_down_drawer = void (*)(const T&,
                                      const draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      bool,
                                      bool);

    // --------------------------------------------------------------------------
    template<typename T,
             drop_down_drawer<T> D = default_drop_down_drawer<T> >
    class drop_down_list : public group_window<layout::drop_down> {
    public:
      typedef group_window<layout::drop_down> super;
      typedef vertical_list list_type;

      typedef T (get_data_t)(std::size_t);
      typedef std::function<get_data_t> data_provider;

      drop_down_list (core::size::type item_size = 20,
                      os::color background = color::white);
      drop_down_list (const drop_down_list& rhs);
      drop_down_list (drop_down_list&& rhs);

      void init ();

      void paint (const draw::graphics& graph);

      void handle_key (os::key_state state,
                       os::key_symbol key,
                       const std::string& t);

      void handle_selection_changed (event_source src);

      T get_selected_item () const;

      void set_data (const data_provider& d, std::size_t count);
      void set_data (data_provider && d, std::size_t count);

      core::rectangle get_popup_place () const;

      void show_popup ();
      void hide_popup ();

      void set_selection (int idx, event_source src);
      void set_visible_items (int n);

      int get_selection () const;
      int get_visible_items () const;

      bool is_popup_visible () const;

      void toggle_popup ();

      void set_drawer (const std::function<win::list::draw_list_item>& drawer);
      void set_drawer (std::function<win::list::draw_list_item>&& drawer);

      void set_count (std::size_t n);

      ~drop_down_list ();

    private:
      void create_children (window*, const core::rectangle& r);
      void create_popup (const core::rectangle& place);

      void handle_move (const core::point&);
      void handle_wheel (const core::point::type delta, const core::point&);

      struct data {
        data (core::size::type item_size = 20,
              os::color background = color::white);

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

#include <gui/ctrl/drop_down.inl>
