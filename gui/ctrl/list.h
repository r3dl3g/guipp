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
 * @brief     list control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <set>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/core/orientation_traits.h>
#include <gui/core/list_state.h>
#include <gui/core/selector.h>
#include <gui/core/selection_adjustment.h>
#include <gui/draw/brush.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/look/control.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T>
    using list_item_drawer = void (*)(const T&,
                                      draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      item_state);

    // --------------------------------------------------------------------------
    template<typename T, text_origin_t O = text_origin_t::vcenter_left>
    void default_list_item_drawer (const T& t,
                                   draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   item_state state) {
      look::text_item(g, place, background, util::string::convert::from<T>(t), state, O);
    }

    // --------------------------------------------------------------------------
    typedef void (list_index_drawer) (std::size_t,
                                      draw::graphics&,
                                      const core::rectangle&,
                                      const draw::brush&,
                                      item_state);

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT list_data {

      virtual std::size_t size () const = 0;

      virtual void draw_at (std::size_t idx,
                            draw::graphics&,
                            const core::rectangle& place,
                            const draw::brush& background,
                            item_state state) const = 0;

      virtual ~list_data () = default;

      const list_data& operator ()() const {
        return *this;
      }

    };
    // --------------------------------------------------------------------------
    typedef const list_data& (list_data_provider) ();

    // --------------------------------------------------------------------------
    template<typename T, list_item_drawer<T> D = default_list_item_drawer<T>>
    struct list_data_t : public list_data {

      virtual T at (std::size_t) const = 0;

      void draw_at (std::size_t idx,
                    draw::graphics& g,
                    const core::rectangle& place,
                    const draw::brush& background,
                    item_state state) const override {
        D(at(idx), g, place, background, state);
      }

    };

    // --------------------------------------------------------------------------
    template<typename T, list_item_drawer<T> D = default_list_item_drawer<T>>
    struct const_list_data : public list_data_t<T, D> {
      using super = list_data_t<T, D>;

      const_list_data (std::initializer_list<T> args)
        : data(args)
      {}

      std::size_t size () const override {
        return data.size();
      }

      T at (std::size_t idx) const override {
        return data[idx];
      }

    protected:
      const std::vector<T> data;
    };

    // --------------------------------------------------------------------------
    template<typename T, list_item_drawer<T> D = default_list_item_drawer<T>>
    struct indirect_list_data : public list_data_t<T, D> {
      using super = list_data_t<T, D>;

      explicit indirect_list_data (const std::vector<T>& args)
        : data(args)
      {}

      std::size_t size () const override {
        return data.size();
      }

      T at (std::size_t idx) const override {
        return data[idx];
      }

    protected:
      const std::vector<T>& data;
    };

    // --------------------------------------------------------------------------
    template<typename T, list_item_drawer<T> D = default_list_item_drawer<T>>
    struct calc_list_data : public list_data_t<T, D> {
      using super = list_data_t<T, D>;

      typedef T (data_getter_f)(std::size_t idx);
      typedef std::size_t (size_getter_f)();
      typedef std::function<data_getter_f> data_getter;
      typedef std::function<size_getter_f> size_getter;

      calc_list_data (data_getter d, size_getter s)
        : data(d)
        , sizer(s)
      {}

      std::size_t size () const override {
        return sizer ? sizer() : 0;
      }

      T at (std::size_t idx) const override {
        return data(idx);
      }

    protected:
      data_getter data;
      size_getter sizer;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT draw_list_data : public list_data {
      using super = list_data;

      typedef std::size_t (size_getter_f)();
      typedef std::function<size_getter_f> size_getter;
      typedef std::function<list_index_drawer> drawer;

      draw_list_data (drawer d, size_getter s)
        : draw(d)
        , sizer(s)
      {}

      std::size_t size () const override {
        return sizer ? sizer() : 0;
      }

      void draw_at (std::size_t idx,
                    draw::graphics& g,
                    const core::rectangle& place,
                    const draw::brush& background,
                    item_state state) const override {
        if (draw) {
          draw(idx, g, place, background, state);
        }
      }

    protected:
      drawer draw;
      size_getter sizer;
    };

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT list_base : public control {
      public:
        typedef control super;
        typedef core::size::type pos_t;

        explicit list_base (os::color background = color::white,
                            bool grab_focus = true);
        list_base (list_base&&) noexcept ;

        core::list_state::is get_state() const;
        core::list_state::set set_state();

        std::size_t get_count () const;

        int get_hilite () const;
        void set_hilite (int sel, bool notify = true);
        void clear_hilite (bool notify = true);

        core::native_point get_last_mouse_point () const;

        void set_scroll_pos (const core::point& pos);
        const core::point& get_scroll_pos () const;

        bool is_valid_idx (int idx) const;
        bool is_scroll_bar_enabled () const;
        bool is_moved () const;

        void draw_item (std::size_t idx,
                        draw::graphics&,
                        const core::rectangle& place,
                        const draw::brush& background,
                        item_state state) const;

        void on_scroll (std::function<void(core::point::type)>&& f);
        void notify_scroll (pos_t);

      protected:
        struct data {
          explicit data ();

          std::function<list_data_provider> items;
          int hilite;
          core::point offset;
          core::native_point last_mouse_point;
        } data;

      private:
        void init ();

      };

      // --------------------------------------------------------------------------
      template<typename T = core::selector::single>
      class selectable_list : public list_base {
      public:
        typedef list_base super;
        typedef T selector_type;

        explicit selectable_list (os::color background = color::white,
                                  bool grab_focus = true);

        const selector_type& get_selection () const;

        bool has_selection () const;

        void clear_selection (event_source notify);

        item_state get_item_state (int idx) const;

        bool can_select_multi () const;

      protected:
        selector_type selection;
      };

    } // namespace detail

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
    template<orientation_t V, typename T, typename S = core::selector::single>
    class uniform_list : public detail::selectable_list<S> {
    public:
      typedef detail::selectable_list<S> super;
      typedef T traits_type;
      typedef typename traits_type::size_type size_type;
      typedef typename super::pos_t pos_t;
      typedef basic_scroll_bar<V> scroll_bar_type;
      typedef win::no_erase_window_class<uniform_list> clazz;

      const pos_t zero = pos_t(0);

      explicit uniform_list (size_type item_size,
                  os::color background = color::white,
                  bool grab_focus = true);
      uniform_list (uniform_list&& rhs) noexcept ;

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename U,
               list_item_drawer<U> F = default_list_item_drawer<U> >
      void create (win::container& parent,
                   const core::rectangle& place,
                   std::function<list_data_provider> data);

      template<typename U, list_item_drawer<U> D = default_list_item_drawer<U>>
      void set_data (const std::vector<U>& data);

      template<typename U, list_item_drawer<U> D = default_list_item_drawer<U>>
      void set_data (std::initializer_list<U> args);

      void set_data (const std::function<list_data_provider>& dta);

      core::size::type get_item_dimension () const;

      size_type get_item_size () const;
      void set_item_size (size_type item_size);
      void set_item_size_and_background (size_type item_size, os::color background);

      int get_index_at_point (const core::point& pt);
      core::rectangle get_geometry_of_index (int idx);

      bool try_to_select (int sel, event_source notify);
      void set_selection (int sel, event_source notify, bool add = false);
      void select (int sel, event_source notify);
      void select_all (event_source notify);
      void unselect (int sel, event_source notify);
      void make_selection_visible ();
      void make_entry_visible (int sel);

      void set_scroll_pos_1 (pos_t pos);
      pos_t get_scroll_pos_1 () const;

      void set_selection_adjustment (core::selection_adjustment);
      core::selection_adjustment get_selection_adjustment () const;

      void handle_mouse_move (os::key_state keys, const core::native_point& pt);
      void handle_left_btn_up (os::key_state keys, const core::native_point& pt);

    protected:
      pos_t get_list_size () const;

      traits_type traits;
      core::selection_adjustment adjustment;

    private:
      void init ();

    };

    // --------------------------------------------------------------------------
    template<orientation_t V>
    struct linear_list_traits : public core::orientation_traits<V> {
      typedef core::orientation_traits<V> super;
      typedef core::size::type size_type;

      explicit linear_list_traits (size_type item_size);

      size_type get_offset_of_index (const core::size& list_size, int idx) const;

      int get_index_at_point (const core::rectangle& list_size,
                              const core::point& pt,
                              const core::point& scroll_pos,
                              size_t /*count*/) const;

      core::rectangle get_geometry_of_index (const core::rectangle& list_size,
                                             int idx,
                                             const core::point& scroll_pos) const;

      size_type get_line_size () const;
      size_type get_item_dimension () const;

      template<typename T>
      void handle_direction_key (T& list, bool shift_pressed, os::key_symbol key);

      size_type item_size;
    };

    // --------------------------------------------------------------------------
    template<core::os::ui_t T = core::os::system_ui>
    struct list_defaults {};

    template<>
    struct list_defaults<core::os::ui_t::mobile> {
      static constexpr core::size::type item_size = 50;
    };
    template<>
    struct list_defaults<core::os::ui_t::desktop> {
      static constexpr core::size::type item_size = 20;
    };

    // --------------------------------------------------------------------------
    template<orientation_t V, typename S = core::selector::single>
    class linear_list : public uniform_list<V, linear_list_traits<V>, S> {
    public:
      typedef uniform_list<V, linear_list_traits<V>, S> super;

      explicit linear_list (core::size::type item_size = list_defaults<>::item_size,
                   os::color background = color::white,
                   bool grab_focus = true);

      linear_list (linear_list&& rhs) noexcept ;

      core::rectangle get_virtual_geometry (const core::rectangle&) const;
      core::size get_scroll_steps () const;

      void paint (draw::graphics& graph);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    template<typename S>
    using horizontal_list_t = linear_list<orientation_t::horizontal, S>;

    template<typename S>
    using vertical_list_t = linear_list<orientation_t::vertical, S>;

    template<typename S>
    using horizontal_scrollable_list_t = virtual_view<horizontal_list_t<S>>;

    template<typename S>
    using vertical_scrollable_list_t = virtual_view<vertical_list_t<S>>;

    template<typename S>
    using list_t = vertical_scrollable_list_t<S>;

    // --------------------------------------------------------------------------
    using horizontal_list = horizontal_list_t<core::selector::single>;
    using vertical_list = vertical_list_t<core::selector::single>;

    using horizontal_scrollable_list = virtual_view<horizontal_list>;
    using vertical_scrollable_list = virtual_view<vertical_list>;

    using list = vertical_scrollable_list;

    // --------------------------------------------------------------------------
    using horizontal_multi_list = horizontal_list_t<core::selector::multi>;
    using vertical_multi_list = vertical_list_t<core::selector::multi>;

    using horizontal_scrollable_multi_list = virtual_view<horizontal_multi_list>;
    using vertical_scrollable_multi_list = virtual_view<vertical_multi_list>;

    using multi_list = vertical_scrollable_multi_list;

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT edit_list : public vertical_list {
    public:
      typedef vertical_list super;

      typedef std::string (source)(int);
      typedef void (target)(int, const std::string&);

      explicit edit_list (core::size::type item_size = list_defaults<>::item_size,
                 os::color background = color::white,
                 bool grab_focus = true);

      edit_list (edit_list&& rhs) noexcept ;

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

        edit_left editor;
        std::function<source> data_source;
        std::function<target> data_target;
        bool enable_edit;
      } data;

    };

    typedef virtual_view<edit_list> scrollable_edit_list;

  } // ctrl

} // gui

#include <gui/ctrl/list.inl>
