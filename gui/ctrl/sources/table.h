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
* @brief     C++ API: table
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
#include "scroll_bar.h"
#include "label.h"
#include "edit.h"
#include "container.h"


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    namespace paint {

      template<typename T,
               draw::frame::drawer F = draw::frame::sunken_relief>
      void text_cell (const T& t,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const text_origin align,
                      const os::color& foreground,
                      const os::color& background,
                      bool selected,
                      bool hilited) {
        text_cell<std::string, draw::frame::no_frame>(convert_to_string(t), graph, place, align,
                                                      foreground, background, selected, hilited);
        F(graph, place);
      }

      template<>
      void text_cell<std::string, draw::frame::no_frame> (const std::string& t,
                                                          const draw::graphics& graph,
                                                          const core::rectangle& place,
                                                          const text_origin align,
                                                          const os::color& foreground,
                                                          const os::color& background,
                                                          bool selected,
                                                          bool hilited);

    }

    // --------------------------------------------------------------------------
    namespace table {

      typedef core::position<int> position;

      // --------------------------------------------------------------------------
      typedef void (cell_drawer) (const position&,       // position
                                  const draw::graphics&, // graph
                                  const core::rectangle&,// place
                                  const text_origin,     // align
                                  const os::color&,      // foreground
                                  const os::color&,      // background
                                  bool,                  // selected
                                  bool);                 // hilited

      // --------------------------------------------------------------------------
      namespace data {

        // --------------------------------------------------------------------------
        template<typename T>
        struct vector {
          inline vector (const T& default_data)
            : default_data(default_data)
          {}

          const T& get (std::size_t idx) const {
            if (idx < data.size()) {
              return data[idx];
            }
            return default_data;
          }

          inline const T& operator[] (std::size_t idx) const {
            return get(idx);
          }

          void set (std::size_t idx, const T& t) {
            if (data.size() <= idx) {
              data.resize(idx + 1, default_data);
            }
            data[idx] = t;
          }

          T& operator[] (std::size_t idx) {
            if (data.size() <= idx) {
              data.resize(idx + 1, default_data);
            }
            return data[idx];
          }

          inline std::size_t size () const {
            return data.size();
          }

          inline void clear () {
            data.clear();
          }

          inline const T& get_default_data () const {
            return default_data;
          }

        private:
          std::vector<T> data;
          const T default_data;
        };

        // --------------------------------------------------------------------------
        template<typename T>
        struct matrix {
          inline matrix (const T& default_data)
            : column_data(default_data)
            , row_data(default_data)
          {}

          const T& get_cell (const position& cell) const {
            if (cell.column < data.size()) {
              const std::vector<T>& c = data[cell.column];
              if (cell.row < c.size()) {
                return c[cell.row];
              }
            }
            return get_column_row_cell(cell);
          }

          void set_cell (const position& cell, const T& t) {
            const std::size_t data_size = data.size();
            if (data_size <= cell.column) {
              data.resize(cell.column + 1);
            }
            std::vector<T>& c = data[cell.column];
            const std::size_t rows = c.size();
            if (rows <= cell.row) {
              c.resize(cell.row + 1, column_data.get(cell.column));
              for (std::size_t r = rows; r < cell.row; ++r) {
                c[r] = get_column_row_cell(position(cell.column, r));
              }
            }
            c[cell.row] = t;
          }

          void set_column (std::size_t column, const T& t) {
            column_data[column] = t;
            if (column < data.size()) {
              data[column].clear();
            }
          }

          void set_row (std::size_t row, const T& t) {
            row_data[row] = t;
            for (std::vector<T>& column : data) {
              if (row < column.size()) {
                column[row] = t;
              }
            }
            const std::size_t column_data_size = column_data.size();
            for (std::size_t c = 0; c < column_data_size; ++c) {
              set_cell(position(c, row), t);
            }
          }

          inline void clear () {
            data.clear();
            column_data.clear();
            row_data.clear();
          }

          inline const T& get_default_data () const {
            return column_data.get_default_data();
          }

        protected:
          inline const T& get_column_row_cell (const position& cell) const {
            if (cell.column < column_data.size()) {
              return column_data[cell.column];
            }
            return row_data[cell.row];
          }

        private:
          typedef data::vector<T> vector;

          std::vector<std::vector<T>> data;
          vector column_data;
          vector row_data;

        };

      } // data

      // --------------------------------------------------------------------------
      class layout {
      public:
        layout (core::size_type default_size)
          : first_idx(0)
          , offset(0)
          , first_offset(0)
          , sizes(default_size)
        {}

        inline core::size_type get_default_size () const {
          return sizes.get_default_data();
        }

        inline core::size_type get_size (std::size_t idx) const {
          return sizes[idx];
        }

        inline std::size_t get_first_idx () const {
          return first_idx;
        }

        inline core::point_type get_offset () const {
          return offset;
        }

        inline core::point_type get_first_offset () const {
          return first_offset;
        }

        void set_size (std::size_t idx, core::size_type size);
        void set_offset (core::point_type offset);

        int index_at (core::point_type pt) const;
        core::point_type position_of (int idx) const;
        int split_idx_at (core::point_type pt, core::size_type delta) const;

        void calc ();

      private:
        std::size_t first_idx;
        core::point_type offset;
        core::point_type first_offset;

        data::vector<core::size_type> sizes;
      };

      // --------------------------------------------------------------------------
      class metric {
      public:
        metric (core::size_type default_width,
                core::size_type default_height)
          : widths(default_width)
          , heights(default_height)
        {}

        inline core::point position_of (const position& cell) const {
          return core::point(widths.position_of(cell.column), heights.position_of(cell.row));
        }

        inline position index_at (const core::point& pt) const {
          return position(widths.index_at(pt.x()), heights.index_at(pt.y()));
        }

        inline core::size get_size (const position& cell) const {
          return core::size(widths.get_size(cell.column), heights.get_size(cell.row));
        }

        inline core::size get_default_size () const {
          return core::size(widths.get_default_size(), heights.get_default_size());
        }

        inline position get_first_idx () const {
          return position(widths.get_first_idx(), heights.get_first_idx());
        }

        inline core::point get_offset () const {
          return core::point(widths.get_offset(), heights.get_offset());
        }

        inline core::point get_first_offset () const {
          return core::point(widths.get_first_offset(), heights.get_first_offset());
        }

        layout widths;
        layout heights;
        position selection;
        position hilite;
      };

      // --------------------------------------------------------------------------
      namespace filter {

        typedef bool (selection_and_hilite)(const position&, const metric&);

        // --------------------------------------------------------------------------
        inline bool data_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection == cell;
        }

        inline bool data_hilite (const position& cell, const metric& geometrie) {
          return (geometrie.hilite == cell) ||
                  geometrie.selection.is_column(cell.column) ||
                  geometrie.selection.is_row(cell.row);
        }

        // --------------------------------------------------------------------------
        inline bool column_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection.is_column(cell.column);
        }

        inline bool column_hilite (const position& cell, const metric& geometrie) {
          return geometrie.hilite.is_column(cell.column) || (geometrie.selection.column == cell.column);
        }

        // --------------------------------------------------------------------------
        inline bool row_selection (const position& cell, const metric& geometrie) {
          return geometrie.selection.is_row(cell.row);
        }

        inline bool row_hilite (const position& cell, const metric& geometrie) {
          return geometrie.hilite.is_row(cell.row) || (geometrie.selection.row == cell.row);
        }

      } // filter

      // --------------------------------------------------------------------------
      namespace paint {

        void draw_table_data (const draw::graphics& graph,
                              const core::rectangle& place,
                              const metric& geometrie,
                              const data::matrix<text_origin>& aligns,
                              const data::matrix<os::color>& foregrounds,
                              const data::matrix<os::color>& backgrounds,
                              const std::function<cell_drawer>& drawer,
                              const std::function<filter::selection_and_hilite>& selection_filter,
                              const std::function<filter::selection_and_hilite>& hilite_filter);

        void draw_table_column (const draw::graphics& graph,
                                const core::rectangle& place,
                                const metric& geometrie,
                                const data::vector<text_origin>& aligns,
                                const data::vector<os::color>& foregrounds,
                                const data::vector<os::color>& backgrounds,
                                const std::function<cell_drawer>& drawer,
                                const std::function<filter::selection_and_hilite>& selection_filter,
                                const std::function<filter::selection_and_hilite>& hilite_filter);

        void draw_table_row (const draw::graphics& graph,
                             const core::rectangle& place,
                             const metric& geometrie,
                             const data::vector<text_origin>& aligns,
                             const data::vector<os::color>& foregrounds,
                             const data::vector<os::color>& backgrounds,
                             const std::function<cell_drawer>& drawer,
                             const std::function<filter::selection_and_hilite>& selection_filter,
                             const std::function<filter::selection_and_hilite>& hilite_filter);
      }

      // --------------------------------------------------------------------------
      template<template<typename U> class T>
      class cell_view : public window {
      public:
        typedef window super;

        template<typename U>
        using container_type = T<U> ;

        cell_view (metric& geometrie,
                   text_origin align = text_origin::center,
                   os::color foreground = color::black,
                   os::color background = color::very_very_light_gray,
                   const std::function<filter::selection_and_hilite>& selection_filter = filter::data_selection,
                   const std::function<filter::selection_and_hilite>& hilite_filter = filter::data_hilite)
          : geometrie(geometrie)
          , aligns(align)
          , foregrounds(foreground)
          , backgrounds(background)
          , selection_filter(selection_filter)
          , hilite_filter(hilite_filter)
        {}

        cell_view (metric& geometrie,
                   const cell_view& rhs)
          : geometrie(geometrie)
          , aligns(rhs.aligns)
          , foregrounds(rhs.foregrounds)
          , backgrounds(rhs.backgrounds)
          , selection_filter(rhs.selection_filter)
          , hilite_filter(rhs.hilite_filter)
        {}

        cell_view (metric& geometrie,
                   cell_view&& rhs)
          : geometrie(geometrie)
          , aligns(std::move(rhs.aligns))
          , foregrounds(std::move(rhs.foregrounds))
          , backgrounds(std::move(rhs.backgrounds))
          , selection_filter(std::move(rhs.selection_filter))
          , hilite_filter(std::move(rhs.hilite_filter))
        {}

        cell_view (const cell_view&) = delete;
        cell_view (cell_view&&) = delete;

        void create (const container& parent,
                     const core::rectangle& place = core::rectangle::def) {
          window::create(clazz, parent, place);
        }

        void set_drawer (const std::function<cell_drawer>& drawer) {
          this->drawer = drawer;
        }

        void set_drawer (std::function<cell_drawer>&& drawer) {
          this->drawer = std::move(drawer);
        }

        inline const std::function<cell_drawer>& get_drawer () const {
          return drawer;
        }

        inline text_origin get_default_align () const {
          return aligns.get_default_data();
        }

        inline os::color get_default_foreground () const {
          return foregrounds.get_default_data();
        }

        inline os::color get_default_background () const {
          return backgrounds.get_default_data();
        }

        inline const std::function<filter::selection_and_hilite>& get_selection_filter () const {
          return selection_filter;
        }

        inline const std::function<filter::selection_and_hilite>& get_hilite_filter () const {
          return hilite_filter;
        }

        void set_selection_filter (const std::function<filter::selection_and_hilite>& f) {
          selection_filter = f;
        }

        void set_hilite_filter (const std::function<filter::selection_and_hilite>& f) {
          hilite_filter = f;
        }

        metric& geometrie;
        container_type<text_origin> aligns;
        container_type<os::color> foregrounds;
        container_type<os::color> backgrounds;

      protected:
        std::function<cell_drawer> drawer;
        std::function<filter::selection_and_hilite> selection_filter;
        std::function<filter::selection_and_hilite> hilite_filter;

      private:
        static no_erase_window_class clazz;

      };

      template<template<typename U> class T>
      no_erase_window_class cell_view<T>::clazz = create_group_window_clazz(color::very_very_light_gray);

      // --------------------------------------------------------------------------
      typedef std::string (data_source) (const position&);

      typedef void (data_target) (const position&, const std::string&);

      std::function<cell_drawer> default_data_drawer (const std::function<data_source>& src);
      std::function<cell_drawer> default_header_drawer (const std::function<data_source>& src);

      // --------------------------------------------------------------------------
      class data_view : public cell_view<data::matrix> {
      public:
        typedef cell_view<data::matrix> super;

        data_view (metric& geometrie,
                   text_origin align = text_origin::center,
                   os::color foreground = color::black,
                   os::color background = color::very_very_light_gray);

        data_view (metric& geometrie, const data_view& rhs);
        data_view (metric& geometrie, data_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      class column_view : public cell_view<data::vector> {
      public:
        typedef cell_view<data::vector> super;

        column_view (metric& geometrie,
                     text_origin align = text_origin::center,
                     os::color foreground = color::black,
                     os::color background = color::very_very_light_gray);

        column_view (metric& geometrie, const column_view& rhs);
        column_view (metric& geometrie, column_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      class row_view : public cell_view<data::vector> {
      public:
        typedef cell_view<data::vector> super;

        row_view (metric& geometrie,
                  text_origin align = text_origin::center,
                  os::color foreground = color::black,
                  os::color background = color::very_very_light_gray);

        row_view (metric& geometrie, const row_view& rhs);
        row_view (metric& geometrie, row_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      std::string build_std_column_name (std::size_t c);

    } // table

    // --------------------------------------------------------------------------
    class table_view : public group_window<gui::layout::border_layout<layout::border_layout_type::bottom_right_maximize>,
                                           color::very_very_light_gray, float, float, float, float> {
    public:
      typedef group_window<gui::layout::border_layout<layout::border_layout_type::bottom_right_maximize>,
                           color::very_very_light_gray, float, float, float, float> super;

      table_view (core::size_type default_width = 80,
                  core::size_type default_height = 20,
                  core::size_type row_width = 80,
                  core::size_type column_height = 20,
                  text_origin align = text_origin::center,
                  os::color foreground = color::black,
                  os::color background = color::white,
                  os::color header_background = color::very_very_light_gray);

      table_view (const table_view&);
      table_view (table_view&&);

      void enable_size (bool h_size, bool v_size);

      core::point get_scroll_pos () const;
      void set_scroll_pos (const core::point& pos);

      void clear_selection (event_source notify);
      void set_selection (table::position selection, event_source notify);
      const table::position& get_selection () const;

      void make_selection_visible ();

      void handle_created (win::window* win, const core::rectangle& place);
      void handle_size (const core::size& sz);

//      void handle_wheel_x (const core::point_type delta, const core::point&);
//      void handle_wheel_y (const core::point_type delta, const core::point&);

      void handle_left_btn_down (os::key_state, const core::point& pt);
      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_mouse_move (os::key_state keys, const core::point& pt);

      void handle_column_left_btn_down (os::key_state, const core::point& pt);
      void handle_column_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_column_mouse_move (os::key_state keys, const core::point& pt);

      void handle_row_left_btn_down (os::key_state, const core::point& pt);
      void handle_row_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_row_mouse_move (os::key_state keys, const core::point& pt);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

      core::size_type row_width () const;
      core::size_type column_height () const;

      table::metric         geometrie;
      table::data_view      data;
      table::column_view    columns;
      table::row_view       rows;
      vertical_scroll_bar           vscroll;
      horizontal_scroll_bar           hscroll;

      typedef basic_label<text_origin::center,
                      draw::frame::raised_relief,
                      color::black,
                      color::very_very_light_gray> edge_view;

      edge_view             edge;

    protected:
      void redraw_all ();

      bool enable_v_size;
      bool enable_h_size;

      bool moved;
      core::point last_mouse_point;
      table::position down_idx;

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    class table_edit : public table_view {
    public:    
      table_edit (core::size_type default_width = 80,
                  core::size_type default_height = 20,
                  core::size_type row_width = 80,
                  core::size_type column_height = 20,
                  text_origin align = text_origin::center,
                  os::color foreground = color::black,
                  os::color background = color::white,
                  os::color header_background = color::very_very_light_gray);

      void set_enable_edit (bool enable) {
        enable_edit = enable;
      }

      bool is_edit_enabled () const {
        return enable_edit;
      }

      void enter_edit ();
      void commit_edit ();
      void cancel_edit ();

      void set_data_source_and_target (const std::function<table::data_source>& data_source,
                                       const std::function<table::data_target>& data_target);

    protected:
      win::edit editor;
      std::function<table::data_source> data_source;
      std::function<table::data_target> data_target;
      bool enable_edit;

    };

  } // win

} // gui
