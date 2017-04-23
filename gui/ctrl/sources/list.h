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


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
#ifdef WIN32
    typedef event_handler<detail::SELECTION_CHANGE_MESSAGE, 0,
                          Params<>::caller<>>
            selection_changed_event;
    typedef event_handler<detail::SELECTION_COMMIT_MESSAGE, 0,
                          Params<>::caller<>>
            selection_commit_event;
    typedef event_handler<detail::HILITE_CHANGE_MESSAGE, 0,
                          Params<bool>::caller<get_param<0, bool>>>
            hilite_changed_event;
    // --------------------------------------------------------------------------
#endif //WIN32

#ifdef X11
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::SELECTION_CHANGE_MESSAGE>>
            selection_changed_event;
    typedef event_handler<ClientMessage, 0,
                          Params<>::caller<>, 0,
                          client_message_matcher<detail::SELECTION_COMMIT_MESSAGE>>
            selection_commit_event;
    typedef event_handler<ClientMessage, 0,
                          Params<bool>::caller<get_client_data<0, bool>>, 0,
                          client_message_matcher<detail::HILITE_CHANGE_MESSAGE>>
            hilite_changed_event;
#endif // X11

    namespace detail {

      // --------------------------------------------------------------------------
      class list : public window {
      public:
        typedef window super;
        typedef core::size_type pos_t;

        // --------------------------------------------------------------------------
        list ();

        size_t get_count () const {
          return item_count;
        }

        int get_selection () const {
          return selection;
        }

        int get_hilite () const {
          return hilite;
        }

        typedef void(draw_list_item) (int idx,
                                      const draw::graphics&,
                                      const core::rectangle& place,
                                      const draw::brush& background,
                                      bool selected,
                                      bool hilited);

        void set_drawer (std::function<draw_list_item> drawer);

      protected:
        void draw_item (int idx,
                        const draw::graphics&,
                        const core::rectangle& place,
                        const draw::brush& background,
                        bool selected,
                        bool hilited) const;

        size_t item_count;
        int selection;
        int hilite;
        bool moved;
        core::point last_mouse_point;

      private:
        std::function<draw_list_item> drawer;

      };

      template<bool V>
      class list_t : public list {
      public:
        typedef list super;
        typedef super::pos_t pos_t;

        list_t () {
          scrollbar.register_event_handler(win::scroll_event([&] (pos_t) {
            super::redraw_later();
          }));
          super::register_event_handler(left_btn_down_event([&](os::key_state, const core::point&) {
            super::take_focus();
          }));
        }

        void create (const window_class& type,
                     const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          super::create(type, parent, place);
          scrollbar.create(*reinterpret_cast<container*>(this), get_scroll_bar_area());
        }

        core::size client_size () const;

        void enable_scroll_bar (bool enable) {
          scrollbar.enable(enable);
          scrollbar.set_visible(enable && scrollbar.get_max());
        }

        bool is_scroll_bar_enabled () const {
          return scrollbar.is_enabled();
        }

        bool is_scroll_bar_visible () const {
          return scrollbar.is_visible();
        }

        pos_t get_scroll_pos () const {
          return scrollbar.get_value();
        }

      protected:
        pos_t get_dimension (const core::point&) const;
        void set_dimension (core::rectangle&, pos_t, pos_t) const;

        pos_t get_list_size () const;
        core::rectangle get_scroll_bar_area () const;

        detail::scroll_barT<!V> scrollbar;
      };

      // --------------------------------------------------------------------------
      template<>
      core::rectangle list_t<false>::get_scroll_bar_area () const;

      template<>
      list::pos_t list_t<false>::get_list_size () const;

      template<>
      list::pos_t list_t<false>::get_dimension (const core::point&) const;

      template<>
      void list_t<false>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

      template<>
      core::size list_t<false>::client_size() const;

      // --------------------------------------------------------------------------
      template<>
      core::rectangle list_t<true>::get_scroll_bar_area () const;

      template<>
      list::pos_t list_t<true>::get_list_size () const;

      template<>
      list::pos_t list_t<true>::get_dimension (const core::point&) const;

      template<>
      void list_t<true>::set_dimension (core::rectangle&, list::pos_t, list::pos_t) const;

      template<>
      core::size list_t<true>::client_size() const;

    }

    template<typename T,
             draw::text_origin O = draw::vcenter_left,
             void(F)(const draw::graphics&,
                     const core::rectangle&) = draw::frame::no_frame>
    void list_item_drawer (const T& t,
                           const draw::graphics& g,
                           const core::rectangle& place,
                           const draw::brush& background,
                           bool selected,
                           bool hilited) {
      paint::text_item(convert_to_string<T>(t), g, place, background, selected, O);
      if (!selected) {
        F(g, place);
      }
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

      void operator() (int idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool hilited) {
        F(super::at(idx), g, place, background, selected, hilited);
      }

    };

    // --------------------------------------------------------------------------
    template<bool V, int S, os::color B>
    class list_t : public detail::list_t<V> {
    public:
      typedef detail::list_t<V> super;
      typedef typename super::pos_t pos_t;
      static const int item_size = S;

      list_t () {
        super::register_event_handler(paint_event([&](const draw::graphics& g) {
          paint(g);
        }));
        super::register_event_handler(left_btn_up_event([&](os::key_state keys, const core::point& pt) {
          if (!super::moved) {
            const int new_selection = get_index_at_point(pt);
            if (new_selection != super::get_selection()) {
              set_selection(new_selection);
            } else if (control_key_bit_mask::is_set(keys)) {
              clear_selection();
            }
            super::redraw_later();
          }
          super::last_mouse_point = core::point::undefined;
        }));
        super::register_event_handler(left_btn_dblclk_event([&](os::key_state keys, const core::point& pt) {
          send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        }));
        if (V) {
          super::register_event_handler(wheel_y_event([&](const pos_t delta, const core::point&){
            set_scroll_pos(super::get_scroll_pos() - S * delta);
            super::moved = true;
          }));
        } else {
          super::register_event_handler(wheel_x_event([&](const pos_t delta, const core::point&){
            set_scroll_pos(super::get_scroll_pos() - S * delta);
            super::moved = true;
          }));
        }
        super::register_event_handler(mouse_move_event([&](os::key_state keys,
                                                    const core::point& pt) {
          const core::rectangle r = super::place();
          if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
            if (super::last_mouse_point != core::point::undefined) {
              pos_t delta = super::get_dimension(super::last_mouse_point) - super::get_dimension(pt);
              set_scroll_pos(super::get_scroll_pos() + delta);
              super::moved = true;
            }
            super::last_mouse_point = pt;
          } else {
            set_hilite(get_index_at_point(pt));
          }
        }));
        super::register_event_handler(size_event([&](const core::size&){
          super::scrollbar.place(super::get_scroll_bar_area());
          adjust_scroll_bar();
        }));
        super::register_event_handler(key_down_event([&](os::key_state,
                                                         os::key_symbol key,
                                                         const std::string&){
          if (V) {
            switch (key) {
              case keys::up:
              case keys::numpad::up:
                set_selection(super::get_selection() - 1);
                break;
              case keys::down:
              case keys::numpad::down:
                set_selection(super::get_selection() + 1);
                break;
            }
          } else {
            switch (key) {
              case keys::left:
              case keys::numpad::left:
                set_selection(super::get_selection() - 1);
                break;
              case keys::right:
              case keys::numpad::right:
                set_selection(super::get_selection() + 1);
                break;
            }
          }
          switch (key) {
            case keys::page_up:
            case keys::numpad::page_up:
              set_selection(super::get_selection() -
                            static_cast<int>(super::get_list_size() / S));
              break;
            case keys::page_down:
            case keys::numpad::page_down:
              set_selection(super::get_selection() +
                            static_cast<int>(super::get_list_size() / S));
              break;
            case keys::home:
            case keys::numpad::home:
              set_selection(0);
              break;
            case keys::end:
            case keys::numpad::end:
              set_selection((int)super::get_count() - 1);
              break;
            case keys::enter:
              send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
              break;
          }
        }));
          super::register_event_handler(mouse_leave_event([&]() {
            set_hilite(-1);
          }));
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
                   simple_list_data<T, F> data) {
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
      void set_data (simple_list_data<T, F> data) {
        super::set_drawer(data);
        set_count(data.size());
      }

      void set_count (size_t count) {
        super::item_count = count;

        const pos_t sz = super::get_list_size();
        const pos_t visible = (S * (int)super::item_count) - sz;

        const pos_t zero = pos_t(0);
        super::scrollbar.set_min_max_step(zero, std::max(visible, zero), sz);
        super::scrollbar.set_visible((visible > zero) && super::is_scroll_bar_enabled());

        super::redraw_later();
      }

      int get_index_at_point(const core::point& pt) {
        if (super::place().is_inside(pt)) {
          return static_cast<int>((super::get_dimension(pt) + super::get_scroll_pos()) / S);
        }
        return -1;
      }

      void set_selection (int sel) {
        int new_selection = std::max(-1, sel);
        if (new_selection >= super::get_count()) {
          new_selection = -1;
        }
        if (super::selection != new_selection) {
          super::selection = new_selection;
          make_selection_visible();
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
          super::redraw_later();
        }
      }

      void clear_selection () {
        if (super::selection != -1) {
          super::selection = -1;
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
          super::redraw_later();
        }
      }

      void make_selection_visible () {
        if (super::selection > -1) {
          const pos_t sel_pos = static_cast<pos_t>(S * super::selection);
          const pos_t sz = super::get_list_size();

          if (sel_pos < super::get_scroll_pos()) {
            set_scroll_pos(sel_pos);
          } else if (sel_pos + S - super::get_scroll_pos() > sz) {
            set_scroll_pos(sel_pos + S - sz);
          }
        }
      }

      void set_hilite (int sel) {
        int new_hilite = std::max(-1, sel);
        if (new_hilite >= super::get_count()) {
          new_hilite = -1;
        }
        if (super::hilite != new_hilite) {
          super::hilite = new_hilite;
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, new_hilite != -1);
          super::redraw_later();
        }
      }

      void set_scroll_pos (pos_t pos) {
        const pos_t max_delta =
          std::max(zero, (S * (pos_t)super::get_count()) - super::get_list_size());
        super::scrollbar.set_value(std::min(std::max(zero, pos), max_delta));
        super::redraw_later();
      }

      void paint (const draw::graphics& graph) {
        core::rectangle area(super::client_size());
        core::rectangle place = area;

        draw::brush background(B);

        const pos_t list_sz = super::get_list_size();
        const int last = (int)super::get_count();
        const int first = int(super::get_scroll_pos() / S);

        super::set_dimension(place, S * first - super::get_scroll_pos(), S);

        for(int idx = first; (idx < last) && (super::get_dimension(place.top_left()) < list_sz); ++idx) {
          super::draw_item(idx, graph, place, background, super::get_selection() == idx, super::get_hilite() == idx);
          super::set_dimension(place, super::get_dimension(place.top_left()) + S, S);
        }

        if (place.y() < area.y2()) {
          graph.fill(draw::rectangle(core::rectangle(place.top_left(), area.bottom_right())), background);
        }

      }

    private:

      void adjust_scroll_bar () {
        scroll_bar::type visible = (S * super::item_count) - super::get_list_size();

        super::scrollbar.set_max(std::max(visible, zero));
        super::scrollbar.set_visible((visible > zero) && super::is_scroll_bar_enabled());
      }

      const pos_t zero = pos_t(0);

      static no_erase_window_class clazz;
    };

    template<bool V, int S, os::color B>
    no_erase_window_class list_t<V, S, B>::clazz = create_group_window_clazz(B);

    // --------------------------------------------------------------------------
    template<int S = 20, os::color B = color::white>
    using hlist = list_t<false, S, B>;

    template<int S = 20, os::color B = color::white>
    using vlist = list_t<true, S, B>;

    using list = vlist<20, color::white>;

  } // win

} // gui
