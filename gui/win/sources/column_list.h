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
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "list.h"
#include "slider.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    struct column_info {
      column_info (core::size::type width = 0,
                   float weight = 1.0F,
                   draw::text_origin align = draw::vcenter_left)
        : min_width(width)
        , width(width)
        , weight(weight)
        , align(align)
      {}

      core::size::type  min_width;
      core::size::type  width;
      float             weight;
      draw::text_origin align;
    };

    namespace detail {

      class base_column_list;

      // --------------------------------------------------------------------------
      class column_list_header : public group_window<layout::standard_layout> {
      public:
        typedef framed_slider_t<false, draw::frame::sunken_relief> slider;

        column_list_header (base_column_list& m)
          : main(m)
        {}

        void init ();
        void do_layout ();

      private:
        std::vector<slider> sliders;
        base_column_list& main;

        void operator= (column_list_header&) = delete;
      };

      // --------------------------------------------------------------------------
      class base_column_list : public owner_draw {
      public:
        typedef std::vector<column_info> columns_t;

        base_column_list ();
        ~base_column_list ();

        const columns_t& get_columns () const {
          return columns;
        }

        void set_columns (const columns_t& val);
        void set_column (columns_t::size_type i, const column_info& c);

        const column_info& get_column (columns_t::size_type i) const {
          return columns[i];
        }

        column_info& get_column (columns_t::size_type i) {
          return columns[i];
        }

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void do_layout ();

        column_list_header header;
        win::list list;

      protected:
        void do_layout_intern (const core::size& sz);

      private:
        columns_t columns;

        static window_class clazz;
      };

    }

    // --------------------------------------------------------------------------
    // static data for simple_column_list.
    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    struct simple_column_list_data : public std::vector<std::vector<T>> {
      typedef std::vector<T> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      simple_column_list_data()
      {}

      simple_column_list_data (std::initializer_list<row> args)
        : super(args)
      {}

      simple_column_list_data (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      simple_column_list_data (const row(&t) [N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      void operator() (int row_id, int col_id,
                       const column_info& c,
                       draw::graphics& g,
                       const core::rectangle& place,
                       bool selected) {
        owner_draw::draw_text_item(convert_to_string(super::at(row_id).at(col_id)), g, place, selected, c.align);
        if (!selected) {
          F(g, place);
        }
      }

    };

    // --------------------------------------------------------------------------
    class simple_column_list : public detail::base_column_list {
    public:
      typedef std::vector<column_info> columns_t;
      typedef detail::base_column_list super;

      typedef void(cell_draw)(int row_id, int col_id,
                              const column_info& col,
                              draw::graphics&,
                              const core::rectangle& place,
                              bool selected);


      void set_drawer (std::function<cell_draw> drawer,
                       core::size::type item_height = 20);

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def) {
        super::create(parent, place);
      }

      template<typename T,
               void(F)(draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
      void create (const container& parent,
                   const core::rectangle& place,
                   const columns_t& val,
                   simple_column_list_data<T, F> data,
                   int item_height = 20) {
        super::create(parent, place);
        set_columns(val);
        set_data(data, item_height);
      }

      template<typename T,
               void(F)(draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
      void set_data (simple_column_list_data<T, F> data,
                     core::size::type item_height = 20) {
        set_drawer(data, item_height);
        list.set_count(data.size());
      }

    protected:
      void draw_cells (int idx,
                       draw::graphics& g,
                       const core::rectangle& place,
                       bool selected);

      std::function<cell_draw> drawer;
    };

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    using column_list_row_t = std::tuple<Arguments...>;

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    struct column_list_data_t : public std::vector<column_list_row_t<Arguments...>> {
      typedef column_list_row_t<Arguments...> row;
      typedef std::vector<row> super;

      typedef typename super::iterator iterator;

      column_list_data_t ()
      {}

      column_list_data_t (std::initializer_list<row> args)
        : super(args)
      {}

      column_list_data_t (iterator b, iterator e)
        : super(b, e)
      {}

      template<size_t N>
      column_list_data_t (const row(&t) [N])
        : super(t, t + N)
      {}

      template<typename L>
      void update_list (L& l) {
        l.set_count(super::size());
      }

      row operator() (int row_id) const {
        return super::at(row_id);
      }

    };

    // --------------------------------------------------------------------------
    template<typename T,
             void(F)(draw::graphics&, const core::rectangle&) = draw::frame::no_frame>
    void cell_drawer (const T& t,
                      const column_info& c,
                      draw::graphics& g,
                      const core::rectangle& place,
                      bool selected) {
      owner_draw::draw_text_item(convert_to_string(t), g, place, selected, c.align);
      if (!selected) {
        F(g, place);
      }
    }

    // --------------------------------------------------------------------------
    template<typename T>
    using cell_drawer_t = void (*)(const T& t,
                                   const column_info& c,
                                   draw::graphics& g,
                                   const core::rectangle& place,
                                   bool selected);

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    using row_cell_drawer_t = std::tuple<cell_drawer_t<Arguments>...>;

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    struct column_list_row_drawer_t : public row_cell_drawer_t<Arguments...> {
      typedef row_cell_drawer_t<Arguments...> super;

      typedef std::vector<column_info> columns_t;
      typedef column_list_row_t<Arguments...> row;

      column_list_row_drawer_t ()
      {}

      column_list_row_drawer_t (const super& rhs)
        : super(rhs)
      {}

      column_list_row_drawer_t (cell_drawer_t<Arguments>...args)
        : super(std::make_tuple(args...))
      {}

      void operator= (const column_list_row_drawer_t& rhs) {
        super::operator=(rhs);
      }

      template<int I>
      void draw_cell (const row&, const columns_t&, draw::graphics&, core::rectangle, bool)
      {}

      template<int I, typename T, typename... Args>
      void draw_cell (const row& data, const columns_t& cs, draw::graphics& g, core::rectangle place, bool selected) {
        const column_info& c = cs[I];
        place.width(c.width - 1);
        std::get<I>(*this)(std::get<I>(data), c, g, place, selected);
        place.move_x(c.width);
        draw_cell<I + 1, Args...>(data, cs, g, place, selected);
      }

      void operator() (const row& data,
                       const columns_t& cs,
                       draw::graphics& g,
                       const core::rectangle& place,
                       bool selected) {
        draw_cell<0, Arguments...>(data, cs, g, place, selected);
      }
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename... Arguments>
      struct column_list_drawer_t {
        typedef std::vector<column_info> columns_t;
        typedef column_list_row_t<Arguments...> row;

        template<int I>
        void draw_cell (const row& r, const columns_t& cs, draw::graphics& g, core::rectangle place, bool selected) {
        }

        template<int I, typename T, typename... Args>
        void draw_cell (const row& r, const columns_t& cs, draw::graphics& g, core::rectangle place, bool selected) {
          const column_info& c = cs[I];
          place.width(c.width - 1);
          cell_drawer(std::get<I>(r), c, g, place, selected);
          place.move_x(c.width);
          draw_cell<I + 1, Args...>(r, cs, g, place, selected);
        }

        void operator() (const row& r,
                         const columns_t& cs,
                         draw::graphics& g,
                         const core::rectangle& place,
                         bool selected) {
          draw_cell<0, Arguments...>(r, cs, g, place, selected);
        }
      };

    } // detail

    // --------------------------------------------------------------------------
    template<typename... Arguments>
    class column_list_t : public detail::base_column_list {
    public:
      static const std::size_t size = sizeof...(Arguments);

      typedef column_list_row_t<Arguments...> row;

      typedef column_list_data_t<Arguments...> standard_data;

      typedef detail::column_list_drawer_t<Arguments...> standard_drawer;
      typedef column_list_row_drawer_t<Arguments...> row_drawer;

      typedef row (get_row_data_t)(int idy);

      typedef void (draw_row_data_t)(const row&,
                                     const columns_t& cs,
                                     draw::graphics& g,
                                     const core::rectangle& place,
                                     bool selected);

      typedef std::function<get_row_data_t> data_provider;
      typedef std::function<draw_row_data_t> data_drawer;

      column_list_t () {
        set_columns(columns_t(size));
        set_drawer(standard_drawer());
      }

      void set_drawer (data_drawer drawer, core::size::type item_height = 20) {
        this->drawer = drawer;
        list.set_drawer(core::easy_bind(this, &column_list_t::draw_cells_t), { item_height, item_height });
      }

      void set_data (data_provider data, std::size_t count) {
        this->data = data;
        list.set_count(count);
      }

    protected:
      void draw_cells_t (int row_id,
                         draw::graphics& g,
                         const core::rectangle& place,
                         bool selected) {
        drawer(data(row_id), get_columns(), g, place, selected);
      }

      data_drawer drawer;
      data_provider data;
    };

  } // win

} // gui


