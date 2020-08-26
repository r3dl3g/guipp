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

#include <util/string_util.h>

namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT list_base : public control {
      public:
        typedef control super;
        typedef core::size::type pos_t;

        list_base (os::color background = color::white,
                   bool grab_focus = true);
        list_base (const list_base&);
        list_base (list_base&&);

        const list_state get_state() const;
        list_state get_state();

        std::size_t get_count () const;
        int get_selection () const;
        bool has_selection () const;
        int get_hilite () const;
        item_state get_item_state (int idx) const;

        os::color get_background () const;
        core::point get_last_mouse_point () const;

        bool is_valid_idx (int idx) const;
        bool is_scroll_bar_enabled () const;
        bool is_moved () const;

        void set_background (os::color background);

        typedef void (item_drawer) (std::size_t idx,
                                    const draw::graphics&,
                                    const core::rectangle& place,
                                    const draw::brush& background,
                                    item_state state);

        void set_drawer (const std::function<item_drawer>& drawer);
        void set_drawer (std::function<item_drawer>&& drawer);

      protected:
        void draw_item (std::size_t idx,
                        const draw::graphics&,
                        const core::rectangle& place,
                        const draw::brush& background,
                        item_state state) const;

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

        std::function<item_drawer> drawer;

      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T>
    void default_list_item_drawer (const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   item_state state) {
      paint::text_item(g, place, background, util::string::convert::from<T>(t), state, text_origin_t::vcenter_left);
    }

    template<typename T>
    using list_item_drawer = void (*)(const T&,
                                      const draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      item_state);

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
                       item_state state);

    };

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct orientation_traits {
      static core::point::type get (const core::point&);
      static core::point::type get_other (const core::point&);

      static core::size::type get (const core::size&);
      static core::size::type get_other (const core::size&);

      static void set (core::point&, core::point::type dim, core::point::type other);
      static void set (core::size&, core::size::type dim, core::size::type other);

      static void set (core::point&, core::point::type dim);
      static void set_other (core::point&, core::point::type other);

      static void set (core::size&, core::size::type dim);
      static void set_other (core::size&, core::size::type other);

      static void set (core::rectangle&, core::point::type, core::size::type);
      static void set_other (core::rectangle&, core::point::type, core::size::type);
    };

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct wheel_traits {};

    template<>
    struct wheel_traits<orientation_t::horizontal> {
      typedef win::wheel_x_event wheel_event_type;

    };

    template<>
    struct wheel_traits<orientation_t::vertical> {
      typedef win::wheel_y_event wheel_event_type;
    };

    // --------------------------------------------------------------------------
    template<orientation_t V, typename T>
    class basic_list : public detail::list_base {
    public:
      typedef detail::list_base super;
      typedef T traits_type;
      typedef typename traits_type::size_type size_type;
      typedef typename super::pos_t pos_t;
      typedef scroll_bar_base<V> scroll_bar_type;
      typedef no_erase_window_class<basic_list> clazz;

      const pos_t zero = pos_t(0);

      basic_list (size_type item_size,
                  os::color background = color::white,
                  bool grab_focus = true);
      basic_list (const basic_list& rhs);
      basic_list (basic_list&& rhs);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename U,
               list_item_drawer<U> F = default_list_item_drawer<U> >
      void create (const win::container& parent,
                   const core::rectangle& place,
                   const simple_list_data<U, F>& data);

      size_type get_item_size () const;
      core::size::type get_item_dimension () const;
      void set_item_size (size_type item_size);
      void set_item_size_and_background (size_type item_size, os::color background);

      void set_count (std::size_t count);

      template<typename F>
      void set_data (const simple_list_data<F>& data);

      template<typename F>
      void set_data (std::initializer_list<F> args);

      int get_index_at_point (const core::point& pt);
      core::rectangle get_place_of_index (int idx);

      bool try_to_select (int sel, event_source notify);
      void set_selection (int sel, event_source notify);
      void make_selection_visible ();

      core::size content_size (const core::size& client_sz, bool scrollbar_visible) const;
      core::size content_size (const core::size& client_sz) const;
      core::rectangle content_area (const core::size& sz) const;

      void adjust_scroll_bar ();
      void set_scroll_pos (pos_t pos);
      pos_t get_scroll_pos () const;

      void enable_scroll_bar (bool enable);
      bool is_scroll_bar_visible () const;

      void clear_selection (event_source notify);

      void set_hilite (int sel, bool notify = true);
      void clear_hilite (bool notify = true);

      void handle_wheel (const pos_t delta, const core::point&);
      void handle_mouse_move (os::key_state keys, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);

    protected:
      pos_t get_list_size () const;

      core::rectangle get_scroll_bar_area (const core::size&) const;
      void adjust_scroll_bar (const core::size&);

      scroll_bar_type scrollbar;
      traits_type traits;

    private:
      void create_scroll_bar (const core::size&);

      void init ();
    };

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct linear_list_traits : public orientation_traits<V> {
      typedef orientation_traits<V> super;
      typedef core::size::type size_type;

      linear_list_traits (size_type item_size);

      size_type get_invisible_size (const core::size& list_size, size_t count) const;

      size_type get_offset_of_index (const core::size& list_size, int idx) const;

      int get_index_at_point (const core::size& list_size,
                              const core::point& pt,
                              size_type scroll_pos,
                              size_t /*count*/) const;

      core::rectangle get_place_of_index (const core::size& list_size,
                                          int idx,
                                          size_type scroll_pos) const;

      size_type get_line_size () const;
      size_type get_item_dimension () const;

      size_type item_size;
    };

    // --------------------------------------------------------------------------
    template<orientation_t V>
    class linear_list : public basic_list<V, linear_list_traits<V>> {
    public:
      typedef basic_list<V, linear_list_traits<V>> super;

      linear_list (core::size::type item_size = 20,
                   os::color background = color::white,
                   bool grab_focus = true);

      linear_list (const linear_list& rhs);

      linear_list (linear_list&& rhs);

      void paint (const draw::graphics& graph);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void handle_direction_key (os::key_symbol key);

      void init ();
    };

    // --------------------------------------------------------------------------
    template<>
    GUIPP_CTRL_EXPORT void linear_list<orientation_t::horizontal>::handle_direction_key (os::key_symbol key);

    template<>
    GUIPP_CTRL_EXPORT void linear_list<orientation_t::vertical>::handle_direction_key (os::key_symbol key);

    // --------------------------------------------------------------------------
    typedef linear_list<orientation_t::horizontal> horizontal_list;
    typedef linear_list<orientation_t::vertical> vertical_list;
    typedef vertical_list list;

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT edit_list : public vertical_list {
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

        edit editor;
        std::function<source> data_source;
        std::function<target> data_target;
        bool enable_edit;
      } data;

    };

  } // ctrl

} // gui

#include <gui/ctrl/list.inl>
