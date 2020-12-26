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
 * @brief     C++ API: basic controls
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/list.h>
#include <gui/layout/layout_container.h>
#include <functional>


namespace gui {

  namespace layout {

    typedef core::size::type column_size_type;

    struct column_info {
      column_size_type width;
      text_origin_t align;
    };

    namespace detail {

      class GUIPP_CTRL_EXPORT column_list_layout {
      public:
        typedef ctrl::vertical_list list_type;

        explicit column_list_layout (win::window* main);
        column_list_layout (win::window* main, const column_list_layout& rhs);
        column_list_layout (win::window* main, column_list_layout&& rhs);

        void init_auto_layout ();
        void layout (const core::rectangle& new_size);

        std::size_t get_column_count () const;
        void set_column_count (std::size_t i);

        text_origin_t get_column_align (std::size_t i) const;
        column_size_type get_column_width (std::size_t i) const;

        int split_idx_at (core::point::type pt, column_size_type delta) const;
        int index_at (core::point::type pt) const;

        column_size_type get_column_left_pos (std::size_t i) const;
        column_size_type get_column_right_pos (std::size_t i) const;
        core::size::type get_available_width () const;

        void set_column_align (std::size_t i, text_origin_t a);
        void set_column_width (std::size_t i, column_size_type w, bool update = false);
        void set_column_info (std::size_t i, const column_info& info, bool update = false);
        void set_columns (std::initializer_list<column_info> infos, bool update = false);

        void set_default_width (column_size_type);
        void set_default_align (text_origin_t);

        void set_list (list_type* l);

      protected:
        void redraw_views ();

        win::window* main;
        list_type* list;

        std::vector<column_size_type> widths;
        std::vector<text_origin_t> aligns;

      };

    }

    // --------------------------------------------------------------------------
    struct simple_column_info {
      column_size_type width;
      text_origin_t align;
      column_size_type min_width;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT simple_column_list_layout : public detail::column_list_layout {
    public:
      typedef detail::column_list_layout super;

      explicit simple_column_list_layout (win::window* main);
      simple_column_list_layout (win::window* main, const simple_column_list_layout& rhs);
      simple_column_list_layout (win::window* main, simple_column_list_layout&& rhs);

      void set_column_width (std::size_t i, column_size_type w, bool update = true);
      void set_column_count (std::size_t i);

      void set_columns (std::initializer_list<simple_column_info> infos, bool update = true);
      void set_column_min_width (std::size_t i, column_size_type w);
      column_size_type get_column_min_width (std::size_t i) const;

    protected:
      std::vector<column_size_type> min_widths;

    };

    // --------------------------------------------------------------------------
    struct weight_column_info {
      column_size_type width;
      text_origin_t align;
      column_size_type min_width;
      float weight;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT weight_column_list_layout : public simple_column_list_layout {
    public:
      typedef simple_column_list_layout super;

      explicit weight_column_list_layout (win::window* main);
      weight_column_list_layout (win::window* main, const weight_column_list_layout& rhs);
      weight_column_list_layout (win::window* main, weight_column_list_layout&& rhs);

      void layout (const core::rectangle& new_size);

      void init_auto_layout ();

      void set_column_width (std::size_t i, column_size_type w, bool update = true);
      void set_column_count (std::size_t i);
      void set_columns (std::initializer_list<weight_column_info> infos, bool update = true);

      void set_column_weight (std::size_t i, float w);
      float get_column_weight (std::size_t i) const;

    protected:
      std::vector<float> weights;

    };

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT base_column_list_layout {
      public:
        typedef ctrl::vertical_list list_type;

        void layout (const core::rectangle& sz) const;
        void set_header_and_list (win::window* header, list_type* list);

      protected:
        struct data {
          data ();

          win::window* header;
          list_type* list;
        } data;

      };

    } // detail

  } // layout

  namespace ctrl {

    GUIPP_CTRL_EXPORT void default_header_cell_drawer (std::size_t i,
                                                       const draw::graphics& g,
                                                       const core::rectangle& r,
                                                       const draw::brush& background);

    // --------------------------------------------------------------------------
    template<typename Layout, os::color background = color::very_very_light_gray>
    class column_list_header : public control {
    public:
      typedef control super;
      typedef Layout layout_type;
      typedef no_erase_window_class<column_list_header> clazz;
      typedef void (cell_draw)(std::size_t,            // idx
                               const draw::graphics&,  // gc
                               const core::rectangle&, // place
                               const draw::brush&);    // background

      column_list_header ();
      column_list_header (const column_list_header& rhs);
      column_list_header (column_list_header&& rhs) noexcept ;

      void paint (const draw::graphics& g);

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void set_cell_drawer (std::function<cell_draw> cd);
      void set_labels (std::initializer_list<std::string> args);

      void handle_left_btn_down (os::key_state, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_mouse_move (os::key_state keys, const core::point& pt);

      layout_type& get_column_layout ();
      const layout_type& get_column_layout () const;

      void layout (const core::rectangle&);

      void operator= (column_list_header&) = delete;

    private:
      std::function<cell_draw> cell_drawer;
      core::point last_mouse_point;
      int down_idx;
      layout_type layouter;

      void init ();

    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename Layout>
      class base_column_list : public win::layout_container<layout::detail::base_column_list_layout> {
      public:
        typedef Layout layout_type;
        typedef win::layout_container<layout::detail::base_column_list_layout> super;
        typedef column_list_header<layout_type> header_type;
        typedef vertical_list list_type;
        typedef no_erase_window_class<base_column_list> clazz;

        explicit base_column_list (core::size::type item_size = list_defaults<>::item_size,
                          os::color background = color::white,
                          bool grab_focus = true);
        base_column_list (base_column_list&& rhs) noexcept ;

        layout_type& get_column_layout ();
        const layout_type& get_column_layout () const;

        void create (container& parent,
                     const core::rectangle& place = core::rectangle::def);

        header_type header;
        list_type list;

      private:
        void init ();

      };

    }

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    using column_list_row_t = std::tuple<Arguments ...>;

    // --------------------------------------------------------------------------
    template<typename T,
             draw::frame::drawer F = draw::frame::no_frame>
    void cell_drawer (const T& t,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const draw::brush& background,
                      item_state state,
                      text_origin_t align);

    // --------------------------------------------------------------------------
    template<typename T>
    using cell_drawer_t = void (*)(const T& t,
                                   const draw::graphics& g,
                                   const core::rectangle& place,
                                   const draw::brush& background,
                                   item_state state,
                                   text_origin_t align);

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
#if defined(_MSC_VER) && (_MSC_VER < 1910)

    struct row_cell_drawer_t : public std::tuple<cell_drawer_t<Arguments>...> {
      typedef std::tuple<cell_drawer_t<Arguments>...> super;

      row_cell_drawer_t ()
      {}

      row_cell_drawer_t (super rhs)
        : super(rhs)
      {}

    };
#else
    using row_cell_drawer_t = std::tuple<cell_drawer_t<Arguments>...>;
#endif // _MSC_VER < 1900

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct column_list_row_drawer_t {
      typedef layout::detail::column_list_layout layout_type;
      typedef std::tuple<Arguments ...> row_type;
      typedef std::tuple<cell_drawer_t<Arguments>...> drawer_type;

      template<std::size_t I>
      static void draw_cell (const row_type& row,
                             const drawer_type& drawer,
                             const layout_type& layout,
                             const draw::graphics& g,
                             const core::rectangle& place,
                             core::point::type x,
                             const draw::brush& background,
                             item_state state);

      template<std::size_t I, typename T, typename ... Args>
      static void draw_cell (const row_type& row,
                             const drawer_type& drawer,
                             const layout_type& layout,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             core::point::type x,
                             const draw::brush& background,
                             item_state state);

      static void draw_row (const row_type& row,
                            const drawer_type& drawer,
                            const layout_type& layout,
                            const draw::graphics& g,
                            const core::rectangle& place,
                            const draw::brush& background,
                            item_state state);
    };

    // --------------------------------------------------------------------------
    struct GUIPP_CTRL_EXPORT column_list_data : public list_data {
      typedef list_data super;
      typedef layout::detail::column_list_layout layout_type;

      column_list_data ()
        : layout(nullptr)
      {}

      void set_layout (layout_type* lay) {
        layout = lay;
      }

      layout_type* get_layout () {
        return layout;
      }

      const layout_type* get_layout () const {
        return layout;
      }

      const column_list_data& operator() () const {
        return *this;
      }

      column_list_data& operator ()() {
        return *this;
      }

    protected:
      layout_type* layout;

    };

    // --------------------------------------------------------------------------
    typedef column_list_data& (column_list_data_provider) ();

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct column_list_data_t : public column_list_data {
      typedef column_list_data super;
      typedef layout::detail::column_list_layout layout_type;
      typedef std::tuple<Arguments ...> row_type;
      typedef std::tuple<cell_drawer_t<Arguments>...> drawer_type;

      explicit column_list_data_t (cell_drawer_t<Arguments>... drwr)
        : drawer(drwr...)
      {}

      explicit column_list_data_t (drawer_type&& drwr)
        : drawer(std::move(drwr))
      {}

      virtual row_type at (std::size_t) const = 0;

      void draw_at (std::size_t idx,
                    const draw::graphics& g,
                    const core::rectangle& place,
                    const draw::brush& background,
                    item_state state) const override {
        if (super::get_layout()) {
          column_list_row_drawer_t<Arguments...>::draw_row(at(idx),
                                                           drawer,
                                                           *super::get_layout(),
                                                           g,
                                                           place,
                                                           background,
                                                           state);
        }
      }

    protected:
      drawer_type drawer;
    };

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct const_column_list_data : public column_list_data_t<Arguments...> {
      typedef column_list_data_t<Arguments...> super;
      typedef typename super::row_type row_type;
      typedef typename super::drawer_type drawer_type;

      typedef std::vector<row_type> container_type;

      explicit const_column_list_data (cell_drawer_t<Arguments>... drwr)
        : super(drwr...)
      {}

      explicit const_column_list_data (drawer_type&& drwr)
        : super(std::move(drwr))
      {}

      const_column_list_data (drawer_type&& drwr, std::initializer_list<row_type> args)
        : super(std::move(drwr))
        , data(args)
      {}

      const_column_list_data (std::initializer_list<row_type> args)
        : super(std::make_tuple(ctrl::cell_drawer<Arguments>...))
        , data(args)
      {}

      std::size_t size () const override {
        return data.size();
      }

      row_type at (std::size_t idx) const override {
        return data[idx];
      }

      const_column_list_data& set_data (std::initializer_list<row_type> args) {
        data = container_type(args);
        return *this;
      }

      const_column_list_data& set_data (const container_type& dt) {
        data = dt;
        return *this;
      }

      const container_type& get_data () const {
        return data;
      }

    protected:
      container_type data;
    };

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    using indirect_column_list_data = column_list_data_t<const std::vector<std::tuple<Arguments...>>&, Arguments...>;

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    class column_list_t : public detail::base_column_list<Layout> {
    public:
      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef std::tuple<Arguments ...> row_type;
      typedef const_column_list_data<Arguments ...> standard_data;
      typedef column_list_row_drawer_t<Arguments ...> row_drawer;
      typedef row_type (get_row_data_t)(std::size_t idy);
      typedef void (draw_row_data_t)(const row_type&,        // r
                                     const layout_type&,     // l
                                     const draw::graphics&,  // g
                                     const core::rectangle&, // place
                                     const draw::brush&,     // background
                                     item_state);            // state

      static const std::size_t count = sizeof ... (Arguments);

      explicit column_list_t (core::size::type item_size = list_defaults<>::item_size,
                     os::color background = color::white,
                     bool grab_focus = true);
      column_list_t (column_list_t&& rhs) noexcept ;

      void set_data (std::function<column_list_data_provider> data);

    };
    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/column_list.inl>
