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
 * @brief     table control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/selection_adjustment.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/look/table.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace table {

      typedef core::basic_point<int> position;
      typedef core::basic_size<int> offset;

      // --------------------------------------------------------------------------
      typedef void (cell_drawer)(const position&,        // position
                                 draw::graphics&,  // graph
                                 const core::rectangle&, // place
                                 const text_origin_t,      // align
                                 const os::color&,       // foreground
                                 const os::color&,       // background
                                 item_state);            // state

      // --------------------------------------------------------------------------
      namespace data {

        // --------------------------------------------------------------------------
        template<typename T>
        struct vector : public std::vector<T> {
          typedef std::vector<T> super;

          explicit vector (const T& default_data);

          const T& get (std::size_t idx) const;
          void set (std::size_t idx, const T& t);

          const T& operator[] (std::size_t idx) const;
          T& operator[] (std::size_t idx);

          const T& get_default_data () const;
          void set_default_data (const T&);

        private:
          T default_data;
        };

        // --------------------------------------------------------------------------
        template<typename T>
        struct matrix {
          explicit matrix (const T& default_data);

          const T& get_cell (const position& cell) const;
          void set_cell (const position& cell, const T& t);

          void set_column (std::size_t column, const T& t);
          void set_row (std::size_t row, const T& t);

          position size () const;
          void clear ();

          const T& get_default_data () const;
          void set_default_data (const T&);

        protected:
          const T& get_column_row_cell (const position& cell) const;

        private:
          typedef data::vector<T> vector;

          std::vector<std::vector<T> > data;
          vector column_data;
          vector row_data;

        };

        // --------------------------------------------------------------------------
        struct GUIPP_CTRL_EXPORT spawn {
          int16_t x;
          int16_t y;

          inline spawn (const int16_t x = 0, const int16_t y = 0) // NOLINT(google-explicit-constructor)
            : x(x)
            , y(y)
          {}

          bool is_hidden () const;
          bool is_empty () const;
          bool is_spawn () const;

          bool operator== (const spawn&) const;
        };

        // --------------------------------------------------------------------------
        struct GUIPP_CTRL_EXPORT spawns {

          void set (const position&, const spawn&);
          spawn get (const position&) const;
          void clear (const position&);

        private:
          typedef std::vector<spawn> column;
          std::vector<column> data;
        };

      } // data

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT layout {
      public:
        explicit layout (core::size::type default_size);

        core::size::type get_default_size () const;
        core::size::type get_size (int idx) const;
        core::point::type get_offset () const;

        std::size_t get_first_idx () const;
        core::point::type get_first_offset () const;

        void set_default_size (core::size::type);
        void set_size (std::size_t idx, core::size::type size);
        void set_offset (core::point::type offset);

        int index_at (core::point::type pt) const;
        core::point::type position_of (int idx) const;
        int split_idx_at (core::point::type pt, core::size::type delta) const;

        void calc ();

      private:
        std::size_t first_idx;
        core::point::type offset;
        core::point::type first_offset;

        data::vector<core::size::type> sizes;
        mutable std::vector<core::point::type> positions;
      };

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT metric {
      public:
        metric (core::size::type default_width,
                core::size::type default_height);

        core::point position_of (const position& cell) const;
        position index_at (const core::point& pt) const;

        core::size get_size (const position& cell) const;
        core::size get_default_size () const;
        core::size::type get_default_width () const;
        core::size::type get_default_height () const;
        core::point get_offset () const;

        void set_default_size (const core::size&);
        void set_default_width (core::size::type);
        void set_default_height (core::size::type);
        void set_offset (const core::point&);

        position get_first_idx () const;
        core::point get_first_offset () const;

        layout widths;
        layout heights;
        position selection;
        position hilite;
        data::spawns spawns;
      };

      // --------------------------------------------------------------------------
      namespace filter {

        typedef bool (selection_and_hilite)(const position&, const metric&);

        // --------------------------------------------------------------------------
        bool data_selection (const position& cell, const metric& geometrie);
        bool data_hilite (const position& cell, const metric& geometrie);

        // --------------------------------------------------------------------------
        bool column_selection (const position& cell, const metric& geometrie);
        bool column_hilite (const position& cell, const metric& geometrie);

        // --------------------------------------------------------------------------
        bool row_selection (const position& cell, const metric& geometrie);
        bool row_hilite (const position& cell, const metric& geometrie);

      } // namespace filter

      // --------------------------------------------------------------------------
      namespace paint {

        GUIPP_CTRL_EXPORT void draw_table_data (draw::graphics& graph,
                                                const core::rectangle& place,
                                                const metric& geometrie,
                                                const data::matrix<text_origin_t>& aligns,
                                                const data::matrix<os::color>& foregrounds,
                                                const data::matrix<os::color>& backgrounds,
                                                const std::function<cell_drawer>& drawer,
                                                const std::function<filter::selection_and_hilite>& selection_filter,
                                                const std::function<filter::selection_and_hilite>& hilite_filter);

        GUIPP_CTRL_EXPORT void draw_table_column (draw::graphics& graph,
                                                  const core::rectangle& place,
                                                  const metric& geometrie,
                                                  const data::vector<text_origin_t>& aligns,
                                                  const data::vector<os::color>& foregrounds,
                                                  const data::vector<os::color>& backgrounds,
                                                  const std::function<cell_drawer>& drawer,
                                                  const std::function<filter::selection_and_hilite>& selection_filter,
                                                  const std::function<filter::selection_and_hilite>& hilite_filter);

        GUIPP_CTRL_EXPORT void draw_table_row (draw::graphics& graph,
                                               const core::rectangle& place,
                                               const metric& geometrie,
                                               const data::vector<text_origin_t>& aligns,
                                               const data::vector<os::color>& foregrounds,
                                               const data::vector<os::color>& backgrounds,
                                               const std::function<cell_drawer>& drawer,
                                               const std::function<filter::selection_and_hilite>& selection_filter,
                                               const std::function<filter::selection_and_hilite>& hilite_filter);
      } // namespace look

      // --------------------------------------------------------------------------
      template<template<typename U> class T>
      class cell_view : public win::container {
      public:
        typedef win::container super;
        typedef win::no_erase_window_class<cell_view> clazz;

        template<typename U>
        using container_type = T<U>;

        explicit cell_view (metric& geometrie,
                            text_origin_t align = text_origin_t::center,
                            os::color foreground = color::black,
                            os::color background = color::very_very_light_gray,
                            const std::function<filter::selection_and_hilite>& selection_filter = filter::data_selection,
                            const std::function<filter::selection_and_hilite>& hilite_filter = filter::data_hilite);

        cell_view (metric& geometrie, const cell_view& rhs);
        cell_view (metric& geometrie, cell_view&& rhs);

        cell_view (const cell_view&) = delete;
        cell_view (cell_view&&) = delete;

        using win::container::create;
        void create (win::container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void set_drawer (const std::function<cell_drawer>& drawer);
        void set_drawer (std::function<cell_drawer>&& drawer);

        const std::function<cell_drawer>& get_drawer () const;

        text_origin_t get_default_align () const;
        os::color get_default_foreground () const;
        os::color get_default_background () const;

        const std::function<filter::selection_and_hilite>& get_selection_filter () const;
        const std::function<filter::selection_and_hilite>& get_hilite_filter () const;

        void set_default_align (text_origin_t);
        void set_default_foreground (os::color);
        void set_default_background (os::color);

        void set_selection_filter (const std::function<filter::selection_and_hilite>& f);
        void set_hilite_filter (const std::function<filter::selection_and_hilite>& f);

        metric& geometrie;
        container_type<text_origin_t> aligns;
        container_type<os::color> foregrounds;
        container_type<os::color> backgrounds;

      protected:
        std::function<cell_drawer> drawer;
        std::function<filter::selection_and_hilite> selection_filter;
        std::function<filter::selection_and_hilite> hilite_filter;

      };

      // --------------------------------------------------------------------------
      typedef std::string (data_source)(const position&);
      typedef void (data_target)(const position&, const std::string&);

      GUIPP_CTRL_EXPORT std::function<cell_drawer> default_data_drawer (const std::function<data_source>& src);
      GUIPP_CTRL_EXPORT std::function<cell_drawer> default_header_drawer (const std::function<data_source>& src);

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT data_view : public cell_view<data::matrix> {
      public:
        typedef cell_view<data::matrix> super;

        explicit data_view (metric& geometrie,
                            text_origin_t align = text_origin_t::center,
                            os::color foreground = color::black,
                            os::color background = color::very_very_light_gray);

        data_view (metric& geometrie, const data_view& rhs);
        data_view (metric& geometrie, data_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT column_view : public cell_view<data::vector> {
      public:
        typedef cell_view<data::vector> super;

        explicit column_view (metric& geometrie,
                              text_origin_t align = text_origin_t::center,
                              os::color foreground = color::black,
                              os::color background = color::very_very_light_gray);

        column_view (metric& geometrie, const column_view& rhs);
        column_view (metric& geometrie, column_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT row_view : public cell_view<data::vector> {
      public:
        typedef cell_view<data::vector> super;

        explicit row_view (metric& geometrie,
                           text_origin_t align = text_origin_t::center,
                           os::color foreground = color::black,
                           os::color background = color::very_very_light_gray);

        row_view (metric& geometrie, const row_view& rhs);
        row_view (metric& geometrie, row_view&& rhs);

        table::position get_index_at_point (const core::point& pt) const;

      private:
        void init ();
      };

      GUIPP_CTRL_EXPORT std::string build_std_column_name (std::size_t c);

    } // table

    // --------------------------------------------------------------------------
    typedef core::point (scroll_maximum_calcer)(const core::size&, const core::point&, const core::point&);

    GUIPP_CTRL_EXPORT core::point default_scroll_maximum (const core::size& sz,
                                                          const core::point& current_pos,
                                                          const core::point& current_max);

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT table_view : public win::group_window<gui::layout::dynamic_border_layout<layout::border_layout_type::bottom_right_maximize>,
        float, float, float, float> {
    public:
      typedef group_window<gui::layout::dynamic_border_layout<layout::border_layout_type::bottom_right_maximize>, float, float, float, float> super;

      typedef basic_label<text_origin_t::center, draw::frame::raised_relief> edge_view;

      explicit table_view (core::size::type default_width = 80,
                           core::size::type default_height = 20,
                           core::size::type row_width = 80,
                           core::size::type column_height = 20,
                           text_origin_t align = text_origin_t::center,
                           os::color foreground = color::black,
                           os::color background = color::white,
                           os::color header_background = color::very_very_light_gray);

      table_view (const table_view&);
      table_view (table_view&&) noexcept ;

      void enable_size (bool h_size, bool v_size);

      void enable_hilite (bool hilite);
      void enable_selection (bool selection);

      bool enable_hilite () const;
      bool enable_selection () const;

      core::point get_scroll_pos () const;
      void set_scroll_pos (const core::point& pos);

      void set_default_cell_size (const core::size&);
      void set_default_row_width (core::size::type);
      void set_default_column_height (core::size::type);

      void clear_selection (event_source notify);
      void set_selection (table::position selection, event_source notify);
      const table::position& get_selection () const;
      bool has_selection () const;

      void make_selection_visible ();

      void set_selection_adjustment (core::selection_adjustment);
      core::selection_adjustment get_selection_adjustment () const;

      void handle_created ();
      void handle_layout (const core::rectangle& r);

      void handle_left_btn_down (os::key_state, const core::native_point& pt);
      void handle_left_btn_up (os::key_state keys, const core::native_point& pt);
      void handle_mouse_move (os::key_state keys, const core::native_point& pt);

      void handle_column_left_btn_down (os::key_state, const core::native_point& pt);
      void handle_column_left_btn_up (os::key_state keys, const core::native_point& pt);
      void handle_column_mouse_move (os::key_state keys, const core::native_point& pt);

      void handle_row_left_btn_down (os::key_state, const core::native_point& pt);
      void handle_row_left_btn_up (os::key_state keys, const core::native_point& pt);
      void handle_row_mouse_move (os::key_state keys, const core::native_point& pt);

      void handle_key (os::key_state,
                       os::key_symbol key,
                       const std::string&);

      void handle_scroll (const core::point& pos);

      void on_selection_changed (std::function<void(event_source)>&& f);
      void on_selection_commit (std::function<void()>&& f);
      void on_selection_cancel (std::function<void()>&& f);
      void on_hilite_changed (std::function<void(bool)>&& f);
      void on_content_changed (std::function<void()>&& f);

      core::size::type row_width () const;
      core::size::type column_height () const;

      void set_scroll_maximum_calcer (std::function<scroll_maximum_calcer> scroll_maximum);
      void set_scroll_maximum (const core::point& pos);

      table::metric geometrie;
      table::data_view data;
      table::column_view columns;
      table::row_view rows;
      vertical_scroll_bar vscroll;
      horizontal_scroll_bar hscroll;
      edge_view edge;

    protected:
      table::position get_valid_selection (const table::offset&) const;
      void redraw_all ();

      bool enable_v_size;
      bool enable_h_size;
      bool enable_hilite_;
      bool enable_selection_;

      bool moved;
      core::native_point last_mouse_point;
      table::position down_idx;
      core::selection_adjustment adjustment;

      std::function<scroll_maximum_calcer> scroll_maximum;

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT table_edit : public table_view {
    public:
      explicit table_edit (core::size::type default_width = 80,
                           core::size::type default_height = 20,
                           core::size::type row_width = 80,
                           core::size::type column_height = 20,
                           text_origin_t align = text_origin_t::center,
                           os::color foreground = color::black,
                           os::color background = color::white,
                           os::color header_background = color::very_very_light_gray);

      void set_enable_edit (bool enable);
      bool is_edit_enabled () const;

      void enter_edit ();
      void commit_edit ();
      void cancel_edit ();

      void set_data_source_and_target (const std::function<table::data_source>& data_source,
                                       const std::function<table::data_target>& data_target);

    protected:
      void move_editor (core::point::type);

      edit_left editor;
      std::function<table::data_source> data_source;
      std::function<table::data_target> data_target;
      bool enable_edit;

    };

  } // ctrl

} // gui

#include <gui/ctrl/table.inl>
