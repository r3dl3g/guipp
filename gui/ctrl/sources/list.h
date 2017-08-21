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
* @brief     C++ API: list
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
#include "window_event_handler.h"
#include "scroll_bar.h"
#include "edit.h"


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      struct list_data {
        list_data ();

        std::size_t item_count;
        int selection;
        int hilite;
        bool moved;
        bool scroll_bar_enabled;
        core::point last_mouse_point;
      };

      // --------------------------------------------------------------------------
      class list : public window {
      public:
        typedef window super;
        typedef core::size_type pos_t;

        // --------------------------------------------------------------------------
        list ();
        list (const list&);
        list (list&&);

        inline std::size_t get_count () const {
          return data.item_count;
        }

        inline int get_selection () const {
          return data.selection;
        }

        inline int get_hilite () const {
          return data.hilite;
        }

        inline bool is_scroll_bar_enabled () const {
          return data.scroll_bar_enabled;
        }

        inline bool is_moved () const {
          return data.moved;
        }

        inline core::point get_last_mouse_point () const {
          return data.last_mouse_point;
        }

        inline bool is_valid_idx (int idx) const {
          return (idx > -1) && (idx < static_cast<int>(get_count()));
        }


        typedef void(draw_list_item) (std::size_t idx,
                                      const draw::graphics&,
                                      const core::rectangle& place,
                                      const draw::brush& background,
                                      bool selected,
                                      bool hilited);

        void set_drawer (const std::function<draw_list_item>& drawer);
        void set_drawer (std::function<draw_list_item>&& drawer);

      protected:
        void draw_item (std::size_t idx,
                        const draw::graphics&,
                        const core::rectangle& place,
                        const draw::brush& background,
                        bool selected,
                        bool hilited) const;

        list_data data;

      private:
        void init ();

        std::function<draw_list_item> drawer;

      };

      template<orientation V>
      class list_t : public list {
      public:
        typedef list super;
        typedef super::pos_t pos_t;

        list_t (bool grab_focus = true)
          : scrollbar(grab_focus)
        {
          scrollbar.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (pos_t) {
            super::redraw_later();
          }));
          if (grab_focus) {
            super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point&) {
              super::take_focus();
            }));
          }
        }

        void create (const window_class& type,
                     const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(type, parent, place);
        }

        core::size client_size () const;

        void create_scroll_bar () {
          if (!scrollbar.is_valid()) {
            scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
          }
        }

        void enable_scroll_bar (bool enable) {
          super::data.scroll_bar_enabled = enable;
          if (enable) {
            create_scroll_bar();
          }
          scrollbar.enable(enable);
          scrollbar.set_visible(enable && scrollbar.get_max());
        }

        bool is_scroll_bar_visible () const {
          return scrollbar.is_visible();
        }

        pos_t get_scroll_pos () const {
          return scrollbar.get_value();
        }

        void handle_key (os::key_symbol key);

      protected:
        pos_t get_dimension (const core::point&) const;
        void set_dimension (core::rectangle&, pos_t, pos_t) const;

        pos_t get_list_size () const;
        core::rectangle get_scroll_bar_area () const;

        detail::scroll_barT<V> scrollbar;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      template<>
      core::rectangle list_t<orientation::horizontal>::get_scroll_bar_area () const;

      template<>
      list::pos_t list_t<orientation::horizontal>::get_list_size () const;

      template<>
      list::pos_t list_t<orientation::horizontal>::get_dimension (const core::point&) const;

      template<>
      void list_t<orientation::horizontal>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

      template<>
      core::size list_t<orientation::horizontal>::client_size() const;

      template<>
      void list_t<orientation::horizontal>::handle_key (os::key_symbol key);

      // --------------------------------------------------------------------------
      template<>
      core::rectangle list_t<orientation::vertical>::get_scroll_bar_area () const;

      template<>
      list::pos_t list_t<orientation::vertical>::get_list_size () const;

      template<>
      list::pos_t list_t<orientation::vertical>::get_dimension (const core::point&) const;

      template<>
      void list_t<orientation::vertical>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

      template<>
      core::size list_t<orientation::vertical>::client_size() const;

      template<>
      void list_t<orientation::vertical>::handle_key (os::key_symbol key);

    }

    template<typename T>
    void list_item_drawer (const T& t,
                           const draw::graphics& g,
                           const core::rectangle& place,
                           const draw::brush& background,
                           bool selected,
                           bool) {
      paint::text_item(g, place, background, convert_to_string<T>(t), selected, text_origin::vcenter_left);
    }

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(const T&,
                     const draw::graphics&,
                     const core::rectangle&,
                     const draw::brush&,
                     bool,
                     bool) = list_item_drawer<T>>
    struct simple_list_data : public std::vector<T> {
      typedef std::vector<T> super;

      typedef typename super::iterator iterator;

      simple_list_data ()
      {}

      simple_list_data (std::initializer_list<T> args)
        : super(args)
      {}

      simple_list_data (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      simple_list_data (const T (& t)[N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      void operator() (std::size_t idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool hilited) {
        F(super::at(idx), g, place, background, selected, hilited);
      }

    };

    // --------------------------------------------------------------------------
    template<orientation V>
    class list_t : public detail::list_t<V> {
    public:
      typedef detail::list_t<V> super;
      typedef typename super::pos_t pos_t;

      const pos_t zero = pos_t(0);

      list_t (core::size_type item_size = 20,
              os::color background = color::white,
              bool grab_focus = true)
        : super(grab_focus)
        , item_size(item_size)
        , background(background)
      {
        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &list_t::paint));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &list_t::handle_left_btn_up));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_dblclk_event([&](os::key_state keys, const core::point& pt) {
          send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        }));
        if (V == orientation::vertical) {
          super::register_event_handler(REGISTER_FUNCTION, wheel_y_event(this, &list_t::handle_wheel));
        } else {
          super::register_event_handler(REGISTER_FUNCTION, wheel_x_event(this, &list_t::handle_wheel));
        }
        super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &list_t::handle_mouse_move));
        super::register_event_handler(REGISTER_FUNCTION, key_down_event(this, &list_t::handle_key));
        super::register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size&){
          if (super::scrollbar.is_valid()) {
            super::scrollbar.place(super::get_scroll_bar_area());
          }
          adjust_scroll_bar();
        }));
        super::register_event_handler(REGISTER_FUNCTION, mouse_leave_event([&]() {
          clear_hilite();
        }));
      }

      void handle_wheel(const pos_t delta, const core::point&) {
        set_scroll_pos(super::get_scroll_pos() - item_size * delta);
        super::data.moved = true;
      }

      void handle_left_btn_up (os::key_state keys, const core::point& pt) {
        if (!super::is_moved() && (super::get_last_mouse_point() != core::point::undefined)) {
          const int new_selection = get_index_at_point(pt);
          if (new_selection != super::get_selection()) {
            set_selection(new_selection, event_source::mouse);
          } else if (control_key_bit_mask::is_set(keys)) {
            clear_selection(event_source::mouse);
          }
          super::redraw_later();
        }
        super::data.last_mouse_point = core::point::undefined;
      }

      void handle_mouse_move(os::key_state keys, const core::point& pt) {
        const core::rectangle r = super::client_area();
        if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
          if (super::get_last_mouse_point() != core::point::undefined) {
            pos_t delta = super::get_dimension(super::get_last_mouse_point()) - super::get_dimension(pt);
            set_scroll_pos(super::get_scroll_pos() + delta);
            super::data.moved = true;
          }
          super::data.last_mouse_point = pt;
        } else {
          set_hilite(get_index_at_point(pt));
        }
      }

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&) {
        if (V == orientation::vertical) {
          switch (key) {
            case keys::up:
            case keys::numpad::up:
              set_selection(super::get_selection() - 1, event_source::keyboard);
              break;
            case keys::down:
            case keys::numpad::down:
              set_selection(super::get_selection() + 1, event_source::keyboard);
              break;
          }
        } else {
          switch (key) {
            case keys::left:
            case keys::numpad::left:
              set_selection(super::get_selection() - 1, event_source::keyboard);
              break;
            case keys::right:
            case keys::numpad::right:
              set_selection(super::get_selection() + 1, event_source::keyboard);
              break;
          }
        }
        switch (key) {
          case keys::page_up:
          case keys::numpad::page_up:
            set_selection(super::get_selection() -
                          static_cast<int>(super::get_list_size() / item_size),
                          event_source::keyboard);
            break;
          case keys::page_down:
          case keys::numpad::page_down:
            set_selection(super::get_selection() +
                          static_cast<int>(super::get_list_size() / item_size),
                          event_source::keyboard);
            break;
          case keys::home:
          case keys::numpad::home:
            set_selection(0, event_source::keyboard);
            break;
          case keys::end:
          case keys::numpad::end:
            set_selection(static_cast<int>(super::get_count()) - 1,
                          event_source::keyboard);
            break;
          case keys::enter:
            send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
            break;
        }
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(clazz, parent, place);
        adjust_scroll_bar();
      }

      template<typename T,
               void(F)(const T&,
                       const draw::graphics&,
                       const core::rectangle&,
                       const draw::brush&,
                       bool,
                       bool) = list_item_drawer<T>>
      void create (const container& parent,
                   const core::rectangle& place,
                   const simple_list_data<T, F>& data) {
        super::create(clazz, parent, place);
        set_data(data);
      }

      template<typename T,
               void(F)(const T&,
                       const draw::graphics&,
                       const core::rectangle&,
                       const draw::brush&,
                       bool,
                       bool) = list_item_drawer<T>>
      void set_data (const simple_list_data<T, F>& data) {
        super::set_drawer(data);
        set_count(data.size());
      }

      void set_count (std::size_t count) {
        super::data.item_count = count;

        const pos_t sz = super::get_list_size();
        const pos_t visible = (item_size * static_cast<pos_t>(count)) - sz;

        super::scrollbar.set_min_max_step(zero, std::max(visible, zero), sz);

        if (super::is_valid()) {
          const bool show_scroll = (visible > zero) && super::is_scroll_bar_enabled();
          if (show_scroll) {
            super::create_scroll_bar();
          }
          super::scrollbar.set_visible(show_scroll);
          super::redraw_later();
        }
      }

      int get_index_at_point (const core::point& pt) {
        if (super::client_area().is_inside(pt)) {
          return static_cast<int>((super::get_dimension(pt) + super::get_scroll_pos()) / item_size);
        }
        return -1;
      }

      core::rectangle get_place_of_index (int idx) {
        if (super::is_valid_idx(idx)) {
          core::rectangle place(super::client_size());
          super::set_dimension(place, item_size * idx - super::get_scroll_pos(), item_size);
          return place;
        }
        return core::rectangle::zero;
      }

      void set_selection (int sel, event_source notify) {
        int new_selection = std::max(-1, sel);
        if (new_selection >= static_cast<int>(super::get_count())) {
          new_selection = -1;
        }
        if (super::data.selection != new_selection) {
          super::data.selection = new_selection;
          make_selection_visible();
          if (notify != event_source::logic) {
            send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
            super::redraw_later();
          }
        }
      }

      void clear_selection (event_source notify) {
        if (super::data.selection != -1) {
          super::data.selection = -1;
          if (notify != event_source::logic) {
            send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
            super::redraw_later();
          }
        }
      }

      void make_selection_visible () {
        if (super::data.selection > -1) {
          const pos_t sel_pos = static_cast<pos_t>(item_size * super::data.selection);
          const pos_t sz = super::get_list_size();

          if (sel_pos < super::get_scroll_pos()) {
            set_scroll_pos(sel_pos);
          } else if (sel_pos + item_size - super::get_scroll_pos() > sz) {
            set_scroll_pos(sel_pos + item_size - sz);
          }
        }
      }

      void set_hilite (int sel, bool notify = true) {
        int new_hilite = std::max(-1, sel);
        if (new_hilite >= static_cast<int>(super::get_count())) {
          new_hilite = -1;
        }
        if (super::get_hilite() != new_hilite) {
          super::data.hilite = new_hilite;
          if (notify) {
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, new_hilite != -1);
            super::redraw_later();
          }
        }
      }

      void clear_hilite (bool notify = true) {
        if (super::get_hilite() != -1) {
          super::data.hilite = -1;
          if (notify) {
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, false);
            super::redraw_later();
          }
        }
      }

      void set_scroll_pos (pos_t pos) {
        const pos_t max_delta =
          std::max(zero, (item_size * (pos_t)super::get_count()) - super::get_list_size());
        auto value = std::min(std::max(zero, pos), max_delta);
        if (value != super::scrollbar.get_value()) {
          super::scrollbar.set_value(value, true);
        }
      }

      void paint (const draw::graphics& graph) {
        const core::rectangle area(super::client_size());
        core::rectangle place = area;

        draw::brush back_brush(background);

        const pos_t list_sz = super::get_list_size();
        const auto last = super::get_count();
        const auto first = static_cast<decltype(last)>(super::get_scroll_pos() / item_size);

        super::set_dimension(place, item_size * first - super::get_scroll_pos(), item_size);

        for(auto idx = first; (idx < last) && (super::get_dimension(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
          super::set_dimension(place, super::get_dimension(place.top_left()) + item_size, item_size);
        }

        if (place.y() < area.y2()) {
          graph.fill(draw::rectangle(core::rectangle(place.top_left(), area.bottom_right())), back_brush);
        }

        if (super::has_focus()) {
          draw::frame::dots(graph, area);
        }

      }

      void adjust_scroll_bar () {
        if (super::is_scroll_bar_enabled()) {
          scroll_bar::type visible = (item_size * super::get_count()) - super::get_list_size();
          const bool show_scroll = (visible > zero);
          if (show_scroll) {
            super::create_scroll_bar();
          }
          super::scrollbar.set_max(std::max(visible, zero));
          super::scrollbar.set_visible(show_scroll);
        }
      }

      core::size_type get_item_size () const {
        return item_size;
      }

      void set_item_size (core::size_type item_size) {
        this->item_size = item_size;
      }

      os::color get_background () const {
        return background;
      }

      void set_background (os::color background) {
        this->background = background;
      }

      void set_item_size_and_background (core::size_type item_size, os::color background) {
        this->item_size = item_size;
        this->background = background;
      }

    protected:
      core::size_type item_size;
      os::color background;

    private:
      static no_erase_window_class clazz;
    };

    template<orientation V>
    no_erase_window_class list_t<V>::clazz = no_erase_window_class("list++");

    // --------------------------------------------------------------------------
    typedef list_t<orientation::horizontal> hlist;
    typedef list_t<orientation::vertical> vlist;
    typedef vlist list;

    // --------------------------------------------------------------------------
    class edit_list : public list_t<orientation::vertical> {
    public:
      typedef list_t<orientation::vertical> super;

      typedef std::string (source) (int);
      typedef void (target) (int, const std::string&);

      edit_list (core::size_type item_size = 20,
                 os::color background = color::white,
                 bool grab_focus = true);

      void set_enable_edit (bool enable);

      bool is_edit_enabled () const;

      void enter_edit ();
      void commit_edit ();
      void cancel_edit ();

      void set_data_source_and_target (const std::function<source>& data_source,
                                       const std::function<target>& data_target);

    protected:
      win::edit editor;
      std::function<source> data_source;
      std::function<target> data_target;
      bool enable_edit;

    };

  } // win

} // gui
