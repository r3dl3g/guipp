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
      class list : public window {
      public:
        typedef window super;
        typedef core::size_type pos_t;

        list (core::size_type item_size = 20,
              os::color background = color::white);
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

        inline core::size_type get_item_size () const {
          return data.item_size;
        }

        inline void set_item_size (core::size_type item_size) {
          data.item_size = item_size;
        }

        inline os::color get_background () const {
          return data.background;
        }

        inline void set_background (os::color background) {
          data.background = background;
        }

        inline void set_item_size_and_background (core::size_type item_size, os::color background) {
          data.item_size = item_size;
          data.background = background;
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

        struct data {
          data (core::size_type item_size = 20,
                os::color background = color::white);

          std::size_t item_count;
          core::size_type item_size;
          int selection;
          int hilite;
          bool moved;
          bool scroll_bar_enabled;
          core::point last_mouse_point;
          os::color background;
        } data;

      private:
        void init ();

        std::function<draw_list_item> drawer;

      };

    }

    template<typename T>
    void default_list_item_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   bool selected,
                                   bool) {
      paint::text_item(g, place, background, convert_to_string<T>(t), selected, text_origin::vcenter_left);
    }

    template<typename T>
    using list_item_drawer = void(*) (const T&,
                                      const draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      bool,
                                      bool);

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             list_item_drawer<T> F = default_list_item_drawer<T>>
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

    template<orientation V>
    struct list_traits {};

    template<>
    struct list_traits<orientation::horizontal> {
      typedef wheel_x_event wheel_event_type;
    };

    template<>
    struct list_traits<orientation::vertical> {
      typedef wheel_y_event wheel_event_type;
    };

    // --------------------------------------------------------------------------
    template<orientation V>
    class list_t : public detail::list {
    public:
      typedef detail::list super;
      typedef typename super::pos_t pos_t;
      typedef list_traits<V> traits;
      typedef scroll_bar_t<V> scrollbar_type;

      const pos_t zero = pos_t(0);

      list_t (core::size_type item_size = 20,
              os::color background = color::white,
              bool grab_focus = true)
        : super(item_size, background)
        , scrollbar(grab_focus)
      {
        init();
      }

      list_t (const list_t& rhs)
        : super(rhs)
        , scrollbar(rhs.scrollbar)
      {
        init();
      }

      list_t (list_t&& rhs)
        : super(std::move(rhs))
        , scrollbar(std::move(rhs.scrollbar))
      {
        init();
      }

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(clazz, parent, place);
        adjust_scroll_bar();
      }

      template<typename T,
               list_item_drawer<T> F = default_list_item_drawer<T>>
      void create (const container& parent,
                   const core::rectangle& place,
                   const simple_list_data<T, F>& data) {
        super::create(clazz, parent, place);
        set_data(data);
      }

      core::size client_size () const;

      void create_scroll_bar () {
        if (!scrollbar.is_valid()) {
          scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
        }
      }

      void enable_scroll_bar (bool enable) {
        data.scroll_bar_enabled = enable;
        if (enable) {
          create_scroll_bar();
        }
        scrollbar.enable(enable);
        scrollbar.set_visible(enable && scrollbar.get_max());
      }

      void adjust_scroll_bar () {
        if (is_scroll_bar_enabled()) {
          scroll_bar::type visible = (data.item_size * get_count()) - get_list_size();
          const bool show_scroll = (visible > zero);
          if (show_scroll) {
            create_scroll_bar();
          }
          scrollbar.set_max(std::max(visible, zero));
          scrollbar.set_visible(show_scroll);
        }
      }

      bool is_scroll_bar_visible () const {
        return scrollbar.is_visible();
      }

      pos_t get_scroll_pos () const {
        return scrollbar.get_value();
      }

      template<typename F>
      void set_data (const F& data) {
        super::set_drawer(data);
        set_count(data.size());
      }

      void set_count (std::size_t count) {
        data.item_count = count;

        const pos_t sz = get_list_size();
        const pos_t visible = (data.item_size * static_cast<pos_t>(count)) - sz;

        scrollbar.set_min_max_step(zero, std::max(visible, zero), sz);

        if (super::is_valid()) {
          const bool show_scroll = (visible > zero) && super::is_scroll_bar_enabled();
          if (show_scroll) {
            create_scroll_bar();
          }
          scrollbar.set_visible(show_scroll);
          super::redraw_later();
        }
      }

      int get_index_at_point (const core::point& pt) {
        if (super::client_area().is_inside(pt)) {
          return static_cast<int>((get_dimension(pt) + get_scroll_pos()) / data.item_size);
        }
        return -1;
      }

      core::rectangle get_place_of_index (int idx) {
        if (super::is_valid_idx(idx)) {
          core::rectangle place(super::client_size());
          set_dimension(place, data.item_size * idx - get_scroll_pos(), data.item_size);
          return place;
        }
        return core::rectangle::zero;
      }

      void set_selection (int sel, event_source notify) {
        int new_selection = std::max(-1, sel);
        if (new_selection >= static_cast<int>(super::get_count())) {
          new_selection = -1;
        }
        if (data.selection != new_selection) {
          data.selection = new_selection;
          make_selection_visible();
          if (notify != event_source::logic) {
            send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
            super::redraw_later();
          }
        }
      }

      void clear_selection (event_source notify) {
        if (data.selection != -1) {
          data.selection = -1;
          if (notify != event_source::logic) {
            send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
            super::redraw_later();
          }
        }
      }

      void make_selection_visible () {
        if (data.selection > -1) {
          const pos_t sel_pos = static_cast<pos_t>(data.item_size * data.selection);
          const pos_t sz = get_list_size();

          if (sel_pos < get_scroll_pos()) {
            set_scroll_pos(sel_pos);
          } else if (sel_pos + data.item_size - get_scroll_pos() > sz) {
            set_scroll_pos(sel_pos + data.item_size - sz);
          }
        }
      }

      void set_hilite (int sel, bool notify = true) {
        int new_hilite = std::max(-1, sel);
        if (new_hilite >= static_cast<int>(super::get_count())) {
          new_hilite = -1;
        }
        if (super::get_hilite() != new_hilite) {
          data.hilite = new_hilite;
          if (notify) {
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, new_hilite != -1);
            super::redraw_later();
          }
        }
      }

      void clear_hilite (bool notify = true) {
        if (super::get_hilite() != -1) {
          data.hilite = -1;
          if (notify) {
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, false);
            super::redraw_later();
          }
        }
      }

      void set_scroll_pos (pos_t pos) {
        const pos_t max_delta =
          std::max(zero, (data.item_size * (pos_t)super::get_count()) - get_list_size());
        auto value = std::min(std::max(zero, pos), max_delta);
        if (value != scrollbar.get_value()) {
          scrollbar.set_value(value, true);
        }
      }

      void paint (const draw::graphics& graph) {
        const core::rectangle area(super::client_size());
        core::rectangle place = area;

        draw::brush back_brush(data.background);

        const pos_t list_sz = get_list_size();
        const auto last = super::get_count();
        const auto first = static_cast<decltype(last)>(get_scroll_pos() / data.item_size);

        set_dimension(place, data.item_size * first - get_scroll_pos(), data.item_size);

        for(auto idx = first; (idx < last) && (get_dimension(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, back_brush, super::get_selection() == idx, super::get_hilite() == idx);
          set_dimension(place, get_dimension(place.top_left()) + data.item_size, data.item_size);
        }

        if (place.y() < area.y2()) {
          graph.fill(draw::rectangle(core::rectangle(place.top_left(), area.bottom_right())), back_brush);
        }

        if (super::has_focus()) {
          draw::frame::dots(graph, area);
        }

      }

      void handle_wheel (const pos_t delta, const core::point&) {
        set_scroll_pos(get_scroll_pos() - data.item_size * delta);
        data.moved = true;
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
        data.last_mouse_point = core::point::undefined;
      }

      void handle_mouse_move (os::key_state keys, const core::point& pt) {
        const core::rectangle r = super::client_area();
        if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
          if (super::get_last_mouse_point() != core::point::undefined) {
            pos_t delta = get_dimension(super::get_last_mouse_point()) - get_dimension(pt);
            set_scroll_pos(get_scroll_pos() + delta);
            data.moved = true;
          }
          data.last_mouse_point = pt;
        } else {
          set_hilite(get_index_at_point(pt));
        }
      }

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&) {
        handle_direction_key(key);
        switch (key) {
          case keys::page_up:
          case keys::numpad::page_up:
            set_selection(super::get_selection() -
                          static_cast<int>(get_list_size() / data.item_size),
                          event_source::keyboard);
            break;
          case keys::page_down:
          case keys::numpad::page_down:
            set_selection(super::get_selection() +
                          static_cast<int>(get_list_size() / data.item_size),
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

    protected:
      pos_t get_dimension (const core::point&) const;
      void set_dimension (core::rectangle&, pos_t, pos_t) const;

      pos_t get_list_size () const;
      core::rectangle get_scroll_bar_area () const;

      void handle_direction_key (os::key_symbol key);

      scrollbar_type scrollbar;

    private:
      void init () {
        scrollbar.register_event_handler(REGISTER_FUNCTION, scroll_event([&](pos_t) {
          super::redraw_later();
        }));
        if (scrollbar.is_focus_accepting()) {
          super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point&) {
            super::take_focus();
          }));
        }
        super::register_event_handler(REGISTER_FUNCTION, paint_event(draw::buffered_paint(this, &list_t::paint)));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &list_t::handle_left_btn_up));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_dblclk_event([&](os::key_state keys, const core::point& pt) {
          send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        }));
        super::register_event_handler(REGISTER_FUNCTION, typename traits::wheel_event_type(this, &list_t::handle_wheel));
        super::register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &list_t::handle_mouse_move));
        super::register_event_handler(REGISTER_FUNCTION, any_key_down_event(this, &list_t::handle_key));
        super::register_event_handler(REGISTER_FUNCTION, size_event([&](const core::size&){
          if (scrollbar.is_valid()) {
            scrollbar.place(get_scroll_bar_area());
          }
          adjust_scroll_bar();
        }));
        super::register_event_handler(REGISTER_FUNCTION, mouse_leave_event([&]() {
          clear_hilite();
        }));
      }

      static no_erase_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<>
    core::rectangle list_t<orientation::horizontal>::get_scroll_bar_area () const;

    template<>
    auto list_t<orientation::horizontal>::get_list_size () const -> pos_t;

    template<>
    auto list_t<orientation::horizontal>::get_dimension (const core::point&) const->pos_t;

    template<>
    void list_t<orientation::horizontal>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

    template<>
    core::size list_t<orientation::horizontal>::client_size() const;

    template<>
    void list_t<orientation::horizontal>::handle_direction_key (os::key_symbol key);

    // --------------------------------------------------------------------------
    template<>
    core::rectangle list_t<orientation::vertical>::get_scroll_bar_area () const;

    template<>
    auto list_t<orientation::vertical>::get_list_size () const -> pos_t;

    template<>
    auto list_t<orientation::vertical>::get_dimension (const core::point&) const -> pos_t;

    template<>
    void list_t<orientation::vertical>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

    template<>
    core::size list_t<orientation::vertical>::client_size() const;

    template<>
    void list_t<orientation::vertical>::handle_direction_key (os::key_symbol key);

    // --------------------------------------------------------------------------
    template<orientation V>
    no_erase_window_class list_t<V>::clazz = no_erase_window_class(typeid(list_t<V>).name());

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
                 bool grab_focus = true)
        : super(item_size, background, grab_focus)
      {
        init();
      }

      edit_list (const edit_list& rhs)
        : super(rhs)
        , data(rhs.data)
      {
        init();
      }

      edit_list (edit_list&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      void set_enable_edit (bool enable);

      bool is_edit_enabled () const;

      void enter_edit ();
      void commit_edit ();
      void cancel_edit ();

      void set_data_source_and_target (const std::function<source>& data_source,
                                       const std::function<target>& data_target);

    private:
      void init ();

      struct data {
        data ()
          : enable_edit(true)
        {}

        win::edit editor;
        std::function<source> data_source;
        std::function<target> data_target;
        bool enable_edit;
      } data;

    };

  } // win

} // gui
