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

        column_list_layout (win::window* main);
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

      simple_column_list_layout (win::window* main);
      simple_column_list_layout (win::window* main, const simple_column_list_layout& rhs);
      simple_column_list_layout (win::window* main, simple_column_list_layout&& rhs);

      void set_column_width (std::size_t i, column_size_type w, bool update = true);
      void set_column_count (std::size_t i);

      void set_columns (std::initializer_list<simple_column_info> infos, bool update = true);
      void set_column_min_width (std::size_t i, const column_size_type w);
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

      weight_column_list_layout (win::window* main);
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
      class base_column_list_layout {
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
      typedef Layout layout_type;
      typedef control super;
      typedef no_erase_window_class<column_list_header> clazz;
      typedef void (cell_draw)(std::size_t,            // idx
                               const draw::graphics&,  // gc
                               const core::rectangle&, // place
                               const draw::brush&);    // background

      column_list_header ();
      column_list_header (const column_list_header& rhs);
      column_list_header (column_list_header&& rhs);

      void paint (const draw::graphics& g);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void set_cell_drawer (std::function<cell_draw> cd);
      void set_labels (std::initializer_list<std::string> labels);

      void handle_left_btn_down (os::key_state, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_mouse_move (os::key_state keys, const core::point& pt);

      layout_type& get_column_layout ();
      const layout_type& get_column_layout () const;

      void layout (const core::rectangle&);

    private:
      std::function<cell_draw> cell_drawer;
      core::point last_mouse_point;
      int down_idx;
      layout_type layouter;

      void init ();

      void operator= (column_list_header&) = delete;
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

        base_column_list (core::size::type item_size = 20,
                          os::color background = color::white,
                          bool grab_focus = true);
        base_column_list (const base_column_list& rhs);
        base_column_list (base_column_list&& rhs);

        layout_type& get_column_layout ();
        const layout_type& get_column_layout () const;

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

        header_type header;
        list_type list;

      private:
        void init ();

      };

    }

    // --------------------------------------------------------------------------
    // static data for simple_column_list.
    // --------------------------------------------------------------------------
    template<typename T,
             draw::frame::drawer F = draw::frame::no_frame>
    struct simple_column_list_data : public std::vector<std::vector<T> > {
      typedef std::vector<T> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      simple_column_list_data ();
      simple_column_list_data (std::initializer_list<row> args);
      simple_column_list_data (iterator b, iterator e);

      template<size_t N>
      simple_column_list_data (const row(&t)[N]);

      template<typename L>
      void update_list (L& l);

      void operator() (std::size_t row_id,
                       std::size_t col_id,
                       const draw::graphics& graph,
                       const core::rectangle& place,
                       const draw::brush& background,
                       item_state state,
                       text_origin_t align);

    };

    // --------------------------------------------------------------------------
    template<typename Layout>
    class simple_column_list : public detail::base_column_list<Layout> {
    public:
      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;

      typedef void (cell_draw)(std::size_t,            // row_id
                               std::size_t,            // col_id
                               const draw::graphics&,  // g
                               const core::rectangle&, // place
                               const draw::brush&,     // background
                               item_state,             // state
                               text_origin_t);           // align

      simple_column_list (core::size::type item_size = 20,
                          os::color background = color::white,
                          bool grab_focus = true);

      void set_drawer (std::function<cell_draw> drawer);

      void create (const win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

      template<typename T, draw::frame::drawer F = draw::frame::no_frame>
      void set_data (simple_column_list_data<T, F> data);

    protected:
      void draw_cells (std::size_t idx,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       item_state state);

      std::function<cell_draw> drawer;
    };

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    using column_list_row_t = std::tuple<Arguments ...>;

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct static_column_list_data_t : public std::vector<column_list_row_t<Arguments ...> > {
      typedef column_list_row_t<Arguments ...> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      static_column_list_data_t ();
      static_column_list_data_t (std::initializer_list<row> args);
      static_column_list_data_t (iterator b, iterator e);

      template<size_t N>
      static_column_list_data_t (const row(&t)[N]);

      template<typename L>
      void update_list (L& l);

      row operator() (std::size_t row_id) const;

    };

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
#if _MSC_VER < 1910

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
    template<typename Layout, typename ... Arguments>
    struct column_list_row_drawer_t : public row_cell_drawer_t<Arguments ...> {
      typedef Layout layout_type;
      typedef row_cell_drawer_t<Arguments ...> super;
      typedef column_list_row_t<Arguments ...> row;

      column_list_row_drawer_t ();

      column_list_row_drawer_t (cell_drawer_t<Arguments>... args);

      void operator= (const column_list_row_drawer_t& rhs);

      template<std::size_t I>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      const core::rectangle& place,
                      core::point::type x,
                      const draw::brush& background,
                      item_state state);

      template<std::size_t I, typename T, typename ... Args>
      void draw_cell (const row& data,
                      const layout_type& l,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      core::point::type x,
                      const draw::brush& background,
                      item_state state);

      void operator() (const row& data,
                       const layout_type& l,
                       const draw::graphics& g,
                       const core::rectangle& place,
                       const draw::brush& background,
                       item_state state);
    };

    // --------------------------------------------------------------------------
    template<typename Layout, typename ... Arguments>
    class column_list_t : public detail::base_column_list<Layout> {
    public:
      static const std::size_t size = sizeof ... (Arguments);

      typedef Layout layout_type;
      typedef detail::base_column_list<layout_type> super;
      typedef column_list_row_t<Arguments ...> row;
      typedef static_column_list_data_t<Arguments ...> standard_data;
      typedef column_list_row_drawer_t<layout_type, Arguments ...> row_drawer;
      typedef row (get_row_data_t)(std::size_t idy);
      typedef void (draw_row_data_t)(const row&,             // r
                                     const layout_type&,     // l
                                     const draw::graphics&,  // g
                                     const core::rectangle&, // place
                                     const draw::brush&,     // background
                                     item_state);            // state

      typedef std::function<get_row_data_t> data_provider;
      typedef std::function<draw_row_data_t> data_drawer;

      column_list_t (core::size::type item_size = 20,
                     os::color background = color::white,
                     bool grab_focus = true);
      column_list_t (const column_list_t& rhs);
      column_list_t (column_list_t&& rhs);

      void set_drawer (data_drawer drawer);
      void set_data (data_provider data, std::size_t count);

    protected:
      void draw_cells_t (std::size_t row_id,
                         const draw::graphics& g,
                         const core::rectangle& place,
                         const draw::brush& back,
                         item_state state);

      data_drawer drawer;
      data_provider data;
    };
    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/column_list.inl>
