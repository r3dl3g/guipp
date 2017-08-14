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

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    typedef void (table_cell_drawer)(std::size_t,           // column
                                     std::size_t,           // row
                                     const draw::graphics&, // graph
                                     const core::rectangle&,// place
                                     const text_origin,     // align
                                     const os::color&,      // foreground
                                     const os::color&,      // background
                                     bool,                  // selected
                                     bool);                 // hilited

    // --------------------------------------------------------------------------
    namespace paint {

      template<typename T,
               void(F)(const draw::graphics&, const core::rectangle&) = draw::frame::sunken_relief>
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

      // --------------------------------------------------------------------------
      namespace data {

        // --------------------------------------------------------------------------
        template<typename T>
        class vector {
        public:
          vector (const T& default_data)
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
        class matrix {
        public:
          matrix (const T& default_data)
            : column_data(default_data)
            , row_data(default_data)
          {}

          const T& get_cell (std::size_t column, std::size_t row) const {
            if (column < data.size()) {
              const std::vector<T>& c = data[column];
              if (row < c.size()) {
                return c[row];
              }
            }
            return get_column_row_cell(column, row);
          }

          void set_cell (std::size_t column, std::size_t row, const T& t) {
            const std::size_t data_size = data.size();
            if (data_size <= column) {
              data.resize(column + 1);
            }
            std::vector<T>& c = data[column];
            const std::size_t rows = c.size();
            if (rows <= row) {
              c.resize(row + 1, column_data.get(column));
              for (std::size_t r = rows; r < row; ++r) {
                c[r] = get_column_row_cell(column, r);
              }
            }
            c[row] = t;
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
              set_cell(c, row, t);
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
          inline const T& get_column_row_cell (std::size_t column, std::size_t row) const {
            if (column < column_data.size()) {
              return column_data[column];
            }
            return row_data[row];
          }

        private:
          typedef data::vector<T> vector;

          std::vector<std::vector<T>> data;
          vector column_data;
          vector row_data;

        };

      } // data

      // --------------------------------------------------------------------------
      struct cell_position {
        inline cell_position (int c = -1, int r = -1)
          : column(c)
          , row(r)
        {}

        inline bool is_cell (std::size_t c, std::size_t r) const {
          return (column == static_cast<int>(c)) && (row == static_cast<int>(r));
        }

        inline bool is_column (std::size_t c) const {
          return (column == static_cast<int>(c)) && (row < 0);
        }

        inline bool is_row (std::size_t r) const {
          return (row == static_cast<int>(r)) && (column < 0);
        }

        inline bool operator == (const cell_position& rhs) const {
          return (column == rhs.column) && (row == rhs.row);
        }

        inline bool operator != (const cell_position& rhs) const {
          return !operator==(rhs);
        }

        inline bool is_empty () const {
          return (column < 0) && (row < 0);
        }

        inline void clear () {
          column = -1;
          row = -1;
        }

        int column;
        int row;
      };

      // --------------------------------------------------------------------------
      class cell_layout {
      public:
        cell_layout (core::size_type default_size)
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

        void calc ();

      private:
        std::size_t first_idx;
        core::point_type offset;
        core::point_type first_offset;

        data::vector<core::size_type> sizes;
      };

      // --------------------------------------------------------------------------
      struct cell_geometrie {
        cell_geometrie (core::size_type default_width,
                        core::size_type default_height)
          : widths(default_width)
          , heights(default_height)
        {}

        cell_layout widths;
        cell_layout heights;
        cell_position selection;
        cell_position hilite;
      };

      // --------------------------------------------------------------------------
      namespace filter {

        typedef bool (selection_and_hilite)(const cell_position&, const cell_geometrie&);

        // --------------------------------------------------------------------------
        inline bool data_selection (const cell_position& cell, const cell_geometrie& geometrie) {
          return geometrie.selection == cell;
        }

        inline bool data_hilite (const cell_position& cell, const cell_geometrie& geometrie) {
          return (geometrie.hilite == cell) ||
                  geometrie.selection.is_column(cell.column) ||
                  geometrie.selection.is_row(cell.row);
        }

        // --------------------------------------------------------------------------
        inline bool column_selection (const cell_position& cell, const cell_geometrie& geometrie) {
          return geometrie.selection.is_column(cell.column);
        }

        inline bool column_hilite (const cell_position& cell, const cell_geometrie& geometrie) {
          return geometrie.hilite.is_column(cell.column) || (geometrie.selection.column == cell.column);
        }

        // --------------------------------------------------------------------------
        inline bool row_selection (const cell_position& cell, const cell_geometrie& geometrie) {
          return geometrie.selection.is_row(cell.row);
        }

        inline bool row_hilite (const cell_position& cell, const cell_geometrie& geometrie) {
          return geometrie.hilite.is_row(cell.row) || (geometrie.selection.row == cell.row);
        }

      } // filter

      // --------------------------------------------------------------------------
      class cell_view : public window {
      public:
        typedef window super;

        cell_view (cell_geometrie& geometrie,
                   text_origin align = text_origin::center,
                   os::color foreground = color::black,
                   os::color background = color::very_very_light_gray,
                   const std::function<filter::selection_and_hilite>& selection_filter = filter::data_selection,
                   const std::function<filter::selection_and_hilite>& hilite_filter = filter::data_hilite);

        void create (const container& parent,
                     const core::rectangle& r = core::rectangle::def);

        void set_drawer (const std::function<table_cell_drawer>& drawer);

        void set_drawer (std::function<table_cell_drawer>&& drawer);

        inline const std::function<table_cell_drawer>& get_drawer () const {
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

        void set_selection_filter (const std::function<filter::selection_and_hilite>& f);
        void set_hilite_filter (const std::function<filter::selection_and_hilite>& f);

        cell_geometrie& geometrie;
        data::matrix<text_origin> aligns;
        data::matrix<os::color> foregrounds;
        data::matrix<os::color> backgrounds;

      protected:
        std::function<table_cell_drawer> drawer;
        std::function<filter::selection_and_hilite> selection_filter;
        std::function<filter::selection_and_hilite> hilite_filter;

      private:
        static no_erase_window_class clazz;

      };

      // --------------------------------------------------------------------------
      typedef std::string (table_data_source)(std::size_t,  // column
                                              std::size_t); // row)

      std::function<table_cell_drawer> default_data_drawer (const std::function<table_data_source>& src);
      std::function<table_cell_drawer> default_header_drawer (const std::function<table_data_source>& src);

      // --------------------------------------------------------------------------
      namespace paint {

        void draw_table_data (const draw::graphics& graph,
                              const table::cell_view& data,
                              const std::function<filter::selection_and_hilite>& selection_filter,
                              const std::function<filter::selection_and_hilite>& hilite_filter);

        void draw_table_column (const draw::graphics& graph,
                                const table::cell_view& data,
                                const std::function<filter::selection_and_hilite>& selection_filter,
                                const std::function<filter::selection_and_hilite>& hilite_filter);

        void draw_table_row (const draw::graphics& graph,
                             const table::cell_view& data,
                             const std::function<filter::selection_and_hilite>& selection_filter,
                             const std::function<filter::selection_and_hilite>& hilite_filter);
      }

      // --------------------------------------------------------------------------
      class data_view : public cell_view {
      public:
        typedef cell_view super;

        data_view (cell_geometrie& geometrie,
                   text_origin align = text_origin::center,
                   os::color foreground = color::black,
                   os::color background = color::very_very_light_gray)
          : super(geometrie, align, foreground, background, filter::data_selection, filter::data_hilite) {
          super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph){
            paint::draw_table_data(graph, *this, selection_filter, hilite_filter);
          }));
        }

        cell_position get_index_at_point (const core::point& pt) const {
          return cell_position(geometrie.widths.index_at(pt.x()),
                               geometrie.heights.index_at(pt.y()));
        }
      };

      // --------------------------------------------------------------------------
      class column_view : public cell_view {
      public:
        typedef cell_view super;

        column_view (cell_geometrie& geometrie,
                     text_origin align = text_origin::center,
                     os::color foreground = color::black,
                     os::color background = color::very_very_light_gray)
          : super(geometrie, align, foreground, background, filter::column_selection, filter::column_hilite) {
          super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph){
            paint::draw_table_column(graph, *this, selection_filter, hilite_filter);
          }));
        }

        cell_position get_index_at_point (const core::point& pt) const {
          return cell_position(geometrie.widths.index_at(pt.x()), -1);
        }
      };

      // --------------------------------------------------------------------------
      class row_view : public cell_view {
      public:
        typedef cell_view super;

        row_view (cell_geometrie& geometrie,
                  text_origin align = text_origin::center,
                  os::color foreground = color::black,
                  os::color background = color::very_very_light_gray)
          : super(geometrie, align, foreground, background, filter::row_selection, filter::row_hilite) {
          super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph){
            paint::draw_table_row(graph, *this, selection_filter, hilite_filter);
          }));
        }

        cell_position get_index_at_point (const core::point& pt) const {
          return cell_position(-1, geometrie.heights.index_at(pt.y()));
        }
      };

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

      core::point get_scroll_pos () const;
      void set_scroll_pos (const core::point& pos);

      void clear_selection (event_source notify);

      void handle_created (win::window* win, const core::rectangle& place);
      void handle_size (const core::size& sz);

      void handle_wheel_x (const core::point_type delta, const core::point&);
      void handle_wheel_y (const core::point_type delta, const core::point&);

      void handle_left_btn_down (os::key_state, const core::point& pt);

      void handle_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_mouse_move (os::key_state keys, const core::point& pt);

      void handle_column_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_column_mouse_move (os::key_state keys, const core::point& pt);

      void handle_row_left_btn_up (os::key_state keys, const core::point& pt);
      void handle_row_mouse_move (os::key_state keys, const core::point& pt);

      core::size_type row_width () const;
      core::size_type column_height () const;

      table::cell_geometrie geometrie;
      table::data_view      data;
      table::column_view    columns;
      table::row_view       rows;
      vscroll_bar           vscroll;
      hscroll_bar           hscroll;

      typedef label_t<text_origin::center,
      draw::frame::raised_relief,
      color::black,
      color::very_very_light_gray> edge_view;

      edge_view edge;

    private:
      void redraw_all ();

      bool moved;
      core::point last_mouse_point;

    };

  } // win

} // gui
