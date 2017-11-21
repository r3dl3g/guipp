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
// Library includes
//
#include <gui/win/window_event_handler.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/list_state.h>


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      class list_base : public window {
      public:
        typedef window super;
        typedef core::size::type pos_t;

        list_base (os::color background = color::white,
                   bool grab_focus = true);
        list_base (const list_base&);
        list_base (list_base&&);

        const list_state get_state() const;
        list_state get_state();

        std::size_t get_count () const;
        int get_selection () const;
        int get_hilite () const;

        os::color get_background () const;
        core::point get_last_mouse_point () const;

        bool is_valid_idx (int idx) const;
        bool is_scroll_bar_enabled () const;
        bool is_moved () const;

        void set_background (os::color background);

        typedef void (draw_list_item)(std::size_t idx,
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
          data (os::color background = color::white);

          std::size_t item_count;
          int selection;
          int hilite;
          core::point last_mouse_point;
          os::color background;
        } data;

      private:
        void init ();

        std::function<draw_list_item> drawer;

      };

    } // namespace detail

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
    using list_item_drawer = void (*)(const T&,
                                      const draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      bool,
                                      bool);

    // static data for list.
    // --------------------------------------------------------------------------
    template<typename T,
             list_item_drawer<T> F = default_list_item_drawer<T> >
    struct simple_list_data : public std::vector<T> {
      typedef std::vector<T> super;

      typedef typename super::iterator iterator;

      simple_list_data ();
      simple_list_data (std::initializer_list<T> args);
      simple_list_data (iterator b, iterator e);

      template<size_t N>
      simple_list_data (const T(&t)[N]);

      template<typename L>
      void update_list (L& l);

      void operator() (std::size_t idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       bool selected,
                       bool hilited);

    };

    // --------------------------------------------------------------------------
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
    class basic_list : public detail::list_base {
    public:
      typedef detail::list_base super;
      typedef typename super::pos_t pos_t;
      typedef list_traits<V> traits;
      typedef basic_scroll_bar<V> scroll_bar_type;
      typedef no_erase_window_class<basic_list> clazz;

      const pos_t zero = pos_t(0);

      basic_list (os::color background = color::white,
                  bool grab_focus = true);
      basic_list (const basic_list& rhs);
      basic_list (basic_list&& rhs);

      core::size client_size () const;

      void create_scroll_bar ();
      void enable_scroll_bar (bool enable);
      bool is_scroll_bar_visible () const;
      pos_t get_scroll_pos () const;

      void clear_selection (event_source notify);

      void set_hilite (int sel, bool notify = true);
      void clear_hilite (bool notify = true);

    protected:
      pos_t get_dimension (const core::point&) const;
      pos_t get_other_dimension (const core::point&) const;

      void set_dimension (core::rectangle &, pos_t, pos_t) const;
      void set_other_dimension (core::rectangle &, pos_t, pos_t) const;

      pos_t get_list_size () const;
      core::rectangle get_scroll_bar_area () const;

      scroll_bar_type scrollbar;

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    template<orientation V>
    class lines_list : public basic_list<V> {
    public:
      typedef basic_list<V> super;
      typedef typename super::pos_t pos_t;

      lines_list (core::size::type item_size = 20,
                  os::color background = color::white,
                  bool grab_focus = true);

      lines_list (const lines_list& rhs);

      lines_list (lines_list&& rhs);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T,
               list_item_drawer<T> F = default_list_item_drawer<T> >
      void create (const container& parent,
                   const core::rectangle& place,
                   const simple_list_data<T, F>& data);

      core::size::type get_item_size () const;

      void set_item_size (core::size::type item_size);
      void set_item_size_and_background (core::size::type item_size, os::color background);

      void adjust_scroll_bar ();
      void set_scroll_pos (pos_t pos);

      void set_count (std::size_t count);

      template<typename F>
      void set_data (const F& data);

      int get_index_at_point (const core::point& pt);
      core::rectangle get_place_of_index (int idx);

      void set_selection (int sel, event_source notify);
      void make_selection_visible ();

      void paint (const draw::graphics& graph);

      void handle_wheel (const pos_t delta, const core::point&);
      void handle_mouse_move (os::key_state keys, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void handle_direction_key (os::key_symbol key);

      void init ();

      core::size::type item_size;

    };

    // --------------------------------------------------------------------------
    typedef lines_list<orientation::horizontal> horizontal_list;
    typedef lines_list<orientation::vertical> vertical_list;
    typedef vertical_list list;

    // --------------------------------------------------------------------------
    class edit_list : public vertical_list {
    public:
      typedef vertical_list super;

      typedef std::string (source)(int);
      typedef void (target)(int, const std::string&);

      edit_list (core::size::type item_size = 20,
                 os::color background = color::white,
                 bool grab_focus = true);

      edit_list (const edit_list& rhs);
      edit_list (edit_list&& rhs);

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
        data ();

        win::edit editor;
        std::function<source> data_source;
        std::function<target> data_target;
        bool enable_edit;
      } data;

    };

  } // win

} // gui

#include <gui/ctrl/list.inl>
