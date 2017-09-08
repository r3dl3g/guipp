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

#include "table.h"

namespace gui {

  namespace win {

    namespace paint {

      template<>
      void text_cell<std::string, draw::frame::no_frame> (const std::string& t,
                                                          const draw::graphics& graph,
                                                          const core::rectangle& place,
                                                          const text_origin align,
                                                          const os::color& foreground,
                                                          const os::color& background,
                                                          bool selected,
                                                          bool hilited) {
        using namespace draw;
        const os::color back = (selected ? color::highLightColor()
                                         : (hilited ? color::darker(background, 0.05F)
                                                    : background));
        const os::color fore = (selected ? color::highLightTextColor()
                                         : (hilited ? color::darker(foreground, 0.25F)
                                                    : foreground));
        graph.fill(rectangle(place), back);
        graph.text(text_box(t, place, align), font::system(), fore);
      }

    }

    namespace table {

      // --------------------------------------------------------------------------
      void layout::set_size (std::size_t idx, core::size_type size) {
        if (sizes[idx] != size) {
          sizes[idx] = size;
          calc();
        }
      }

      void layout::set_offset (core::point_type offset) {
        if (offset < 0) {
          offset = 0;
        }
        if (this->offset != offset) {
          this->offset = offset;
          calc();
        }
      }

      int layout::index_at (core::point_type pt) const {
        core::point_type pos = -get_offset();
        int idx = 0;
        while (pos <= pt) {
          pos += get_size(idx);
          ++idx;
        }
        return idx - 1;
      }

      int layout::split_idx_at (core::point_type pt, core::size_type sz) const {
        core::point_type pos = -get_offset();
        const core::point_type lower = pt - sz;
        const core::point_type upper = pt + sz;
        int idx = 0;
        while (pos < lower) {
          pos += get_size(idx);
          ++idx;
        }
        if ((lower <= pos) && (pos < upper)) {
          return idx - 1;
        }
        return -1;
      }

      core::point_type layout::position_of (int idx) const {
        core::point_type pos = -get_offset();
        for (int i = 0; i < idx; ++i) {
          pos += get_size(i);
        }
        return pos;
      }

      void layout::calc () {
        core::point_type pos = -get_offset();
        first_idx = 0;
        first_offset = 0;
        while (pos <= 0) {
          first_offset = pos;
          pos += get_size(first_idx);
          ++first_idx;
        }
        --first_idx;
      }

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
                              const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const core::size max_sz = place.size();
            position cell(0, geometrie.heights.get_first_idx());
            core::point_type y = geometrie.heights.get_first_offset();

            while (y < max_sz.height()) {
              const core::size_type height = geometrie.heights.get_size(cell.row);

              cell.column = static_cast<int>(geometrie.widths.get_first_idx());
              core::point_type x = geometrie.widths.get_first_offset();

              while (x < max_sz.width()) {
                const core::size_type width = geometrie.widths.get_size(cell.column);
                drawer(cell, graph,
                       core::rectangle(x, y, width, height),
                       aligns.get_cell(cell),
                       foregrounds.get_cell(cell),
                       backgrounds.get_cell(cell),
                       selection_filter(cell, geometrie),
                       hilite_filter(cell, geometrie));

                x += width;
                ++(cell.column);
              }
              y += height;
              ++(cell.row);
            }
          }
        }

        void draw_table_column (const draw::graphics& graph,
                                const core::rectangle& place,
                                const metric& geometrie,
                                const data::vector<text_origin>& aligns,
                                const data::vector<os::color>& foregrounds,
                                const data::vector<os::color>& backgrounds,
                                const std::function<cell_drawer>& drawer,
                                const std::function<filter::selection_and_hilite>& selection_filter,
                                const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const core::size max_sz = place.size();
            position cell(geometrie.widths.get_first_idx(), 0);
            core::point_type x = geometrie.widths.get_first_offset();

            while (x < max_sz.width()) {
              const core::size_type width = geometrie.widths.get_size(cell.column);
              drawer(cell, graph,
                     core::rectangle(x, 0, width, max_sz.height()),
                     aligns.get(cell.column),
                     foregrounds.get(cell.column),
                     backgrounds.get(cell.column),
                     selection_filter(cell, geometrie),
                     hilite_filter(cell, geometrie));

              x += width;
              ++(cell.column);
            }
          }
        }

        void draw_table_row (const draw::graphics& graph,
                             const core::rectangle& place,
                             const metric& geometrie,
                             const data::vector<text_origin>& aligns,
                             const data::vector<os::color>& foregrounds,
                             const data::vector<os::color>& backgrounds,
                             const std::function<cell_drawer>& drawer,
                             const std::function<filter::selection_and_hilite>& selection_filter,
                             const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const core::size max_sz = place.size();
            position cell(0, geometrie.heights.get_first_idx());
            core::point_type y = geometrie.heights.get_first_offset();

            while (y < max_sz.height()) {
              const core::size_type height = geometrie.heights.get_size(cell.row);

              drawer(cell, graph,
                     core::rectangle(0, y, max_sz.width(), height),
                     aligns.get(cell.row),
                     foregrounds.get(cell.row),
                     backgrounds.get(cell.row),
                     selection_filter(cell, geometrie),
                     hilite_filter(cell, geometrie));

              y += height;
              ++(cell.row);
            }
          }
        }
      }

      // --------------------------------------------------------------------------
      std::function<cell_drawer> default_data_drawer (const std::function<data_source>& src) {
        return[src] (const position& cell,
                     const draw::graphics& graph,
                     const core::rectangle& place,
                     const text_origin align,
                     const os::color& foreground,
                     const os::color& background,
                     bool selected,
                     bool hilited) {
          win::paint::text_cell<std::string, draw::frame::lines>(src(cell), graph, place,
                                                                 align, foreground, background,
                                                                 selected, hilited);
        };
      }

      // --------------------------------------------------------------------------
      std::function<cell_drawer> default_header_drawer (const std::function<data_source>& src) {
        return[src] (const position& cell,
                     const draw::graphics& graph,
                     const core::rectangle& place,
                     const text_origin align,
                     const os::color& foreground,
                     const os::color& background,
                     bool selected,
                     bool hilited) {
          win::paint::text_cell<std::string, draw::frame::raised_relief>(src(cell), graph, place,
                                                                         align, foreground, background,
                                                                         selected, hilited);
        };
      }

      // --------------------------------------------------------------------------
      std::string build_std_column_name (std::size_t c) {
        std::string buffer;
        int column = static_cast<int>(c);

        do {
          int col = column % 26;
          buffer.insert(0, 1, (char)('A' + col));
          column = (column - col) / 26 - 1;
        } while (column > -1);

        return buffer;
      }

      // --------------------------------------------------------------------------
      data_view::data_view (metric& geometrie,
                            text_origin align,
                            os::color foreground,
                            os::color background)
        : super(geometrie, align, foreground, background, filter::data_selection, filter::data_hilite) {
        init();
      }

      data_view::data_view (metric& geometrie,
                            const data_view& rhs)
        : super(geometrie, rhs) {
        init();
      }

      data_view::data_view (metric& geometrie,
                            data_view&& rhs)
        : super(geometrie, std::move(rhs)) {
        init();
      }

      table::position data_view::get_index_at_point (const core::point& pt) const {
        return table::position(geometrie.widths.index_at(pt.x()),
                               geometrie.heights.index_at(pt.y()));
      }

      void data_view::init () {
        super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph){
          paint::draw_table_data(graph, client_area(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

      // --------------------------------------------------------------------------
      column_view::column_view (metric& geometrie,
                                text_origin align,
                                os::color foreground,
                                os::color background)
        : super(geometrie, align, foreground, background, filter::column_selection, filter::column_hilite) {
        init();
      }

      column_view::column_view (metric& geometrie,
                                const column_view& rhs)
        : super(geometrie, rhs) {
        init();
      }

      column_view::column_view (metric& geometrie,
                                column_view&& rhs)
        : super(geometrie, std::move(rhs)) {
        init();
      }

      table::position column_view::get_index_at_point (const core::point& pt) const {
        return table::position(geometrie.widths.index_at(pt.x()), -1);
      }

      void column_view::init () {
        super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph) {
          paint::draw_table_column(graph, client_area(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

      // --------------------------------------------------------------------------
      row_view::row_view (metric& geometrie,
                          text_origin align,
                          os::color foreground,
                          os::color background)
        : super(geometrie, align, foreground, background, filter::row_selection, filter::row_hilite) {
        init();
      }

      row_view::row_view (metric& geometrie,
                          const row_view& rhs)
        : super(geometrie, rhs) {
        init();
      }

      row_view::row_view (metric& geometrie,
                          row_view&& rhs)
        : super(geometrie, std::move(rhs)) {
        init();
      }

      table::position row_view::get_index_at_point (const core::point& pt) const {
        return table::position(-1, geometrie.heights.index_at(pt.y()));
      }

      void row_view::init () {
        super::register_event_handler(REGISTER_FUNCTION, paint_event([&](const draw::graphics& graph){
          paint::draw_table_row(graph, client_area(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

    } // table

    // --------------------------------------------------------------------------
    table_view::table_view (core::size_type default_width,
                            core::size_type default_height,
                            core::size_type row_width,
                            core::size_type column_height,
                            text_origin align,
                            os::color foreground,
                            os::color background,
                            os::color header_background)
      : super(column_height, static_cast<core::size_type>(scroll_bar::get_scroll_bar_width()),
              row_width, static_cast<core::size_type>(scroll_bar::get_scroll_bar_width()))
      , geometrie(default_width, default_height)
      , data(geometrie, align, foreground, background)
      , columns(geometrie, align, foreground, header_background)
      , rows(geometrie, align, foreground, header_background)
      , last_mouse_point(core::point::undefined)
      , enable_v_size(false)
      , enable_h_size(false)
      , moved(false)
    {
      init();
    }

    table_view::table_view (const table_view& rhs)
      : super(rhs)
      , geometrie(rhs.geometrie)
      , data(geometrie, rhs.data)
      , columns(geometrie, rhs.columns)
      , rows(geometrie, rhs.rows)
      , last_mouse_point(core::point::undefined)
      , enable_v_size(false)
      , enable_h_size(false)
      , moved(false)
    {
      init();
    }

    table_view::table_view (table_view&& rhs)
      : super(rhs)
      , geometrie(std::move(rhs.geometrie))
      , data(geometrie, std::move(rhs.data))
      , columns(geometrie, std::move(rhs.columns))
      , rows(geometrie, std::move(rhs.rows))
      , last_mouse_point(core::point::undefined)
      , enable_v_size(false)
      , enable_h_size(false)
      , moved(false)
    {
      init();
    }

    void table_view::enable_size (bool h_size, bool v_size) {
      enable_h_size = h_size;
      enable_v_size = v_size;
    }

    void table_view::init () {
      get_layout().set_center_top_bottom_left_right(&data, &columns, &hscroll, &rows, &vscroll);
      register_event_handler(REGISTER_FUNCTION, create_event(this, &table_view::handle_created));
      register_event_handler(REGISTER_FUNCTION, win::size_event(this, &table_view::handle_size));
      vscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type pos) {
        geometrie.heights.set_offset(pos);
        data.redraw_later();
        rows.redraw_later();
      }));
      hscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type pos) {
        geometrie.widths.set_offset(pos);
        data.redraw_later();
        columns.redraw_later();
      }));

      data.register_event_handler(REGISTER_FUNCTION, key_down_event(this, &table_view::handle_key));
      data.set_accept_focus(true);

      data.register_event_handler(REGISTER_FUNCTION,
                                  win::left_btn_dblclk_event([&](os::key_state, const core::point&) {
        send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
      }));

      data.register_event_handler(REGISTER_FUNCTION, wheel_x_event(this, &table_view::handle_wheel_x));
      data.register_event_handler(REGISTER_FUNCTION, wheel_y_event(this, &table_view::handle_wheel_y));
      data.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_mouse_move));
      data.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_left_btn_down));
      data.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_left_btn_up));

      columns.register_event_handler(REGISTER_FUNCTION, wheel_x_event(this, &table_view::handle_wheel_x));
      columns.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_column_mouse_move));
      columns.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_column_left_btn_down));
      columns.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_column_left_btn_up));

      rows.register_event_handler(REGISTER_FUNCTION, wheel_y_event(this, &table_view::handle_wheel_y));
      rows.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_row_mouse_move));
      rows.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_row_left_btn_down));
      rows.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_row_left_btn_up));

    }

    core::point table_view::get_scroll_pos () const {
      return core::point(hscroll.get_value(), vscroll.get_value());
    }

    void table_view::set_scroll_pos (const core::point& pos) {
      if (hscroll.is_enabled()) {
        hscroll.set_value(pos.x());
        geometrie.widths.set_offset(hscroll.get_value());
      }
      if (vscroll.is_enabled()) {
        vscroll.set_value(pos.y());
        geometrie.heights.set_offset(vscroll.get_value());
      }
      redraw_all();
    }

    void table_view::clear_selection (event_source notify) {
      if (!geometrie.selection.is_empty()) {
        geometrie.selection.clear();
        if (notify != event_source::logic) {
          send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          redraw_all();
        }
      }
    }

    void table_view::set_selection (table::position selection, event_source notify) {
      if (selection.column < -1) {
        selection.column = -1;
      }
      if (selection.row < -1) {
        selection.row = -1;
      }
      if (geometrie.selection != selection) {
        geometrie.selection = selection;
        make_selection_visible();
        redraw_all();
        send_client_message(this, detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
      }
    }

    const table::position& table_view::get_selection () const {
      return geometrie.selection;
    }

    void table_view::make_selection_visible () {
      const core::point pt = geometrie.position_of(geometrie.selection);
      const core::size sz = geometrie.get_size(geometrie.selection);
      const core::size space = data.client_size();

      core::point delta;
      if (pt.x() < 0) {
        delta.x(pt.x());
      } else if ((pt.x() + sz.width()) > space.width()) {
        delta.x((pt.x() + sz.width()) - space.width());
      }
      if (pt.y() < 0) {
        delta.y(pt.y());
      } else if ((pt.y() + sz.height()) > space.height()) {
        delta.y((pt.y() + sz.height()) - space.height());
      }

      if (delta != core::point::zero) {
        set_scroll_pos(get_scroll_pos() + delta);
      }

    }

    void table_view::redraw_all () {
      data.redraw_later();
      rows.redraw_later();
      columns.redraw_later();
    }

    core::size_type table_view::row_width () const {
      return get_layout().get_left_width();
    }

    core::size_type table_view::column_height () const {
      return get_layout().get_top_height();
    }

    void table_view::handle_created (win::window* win, const core::rectangle& place) {
      edge.create(*this, core::rectangle(0, 0, row_width(), column_height()));
      data.create(*this, core::rectangle(row_width(), column_height(), 160, 80));
      columns.create(*this, core::rectangle(row_width(), 0, 160, column_height()));
      rows.create(*this, core::rectangle(0, row_width(), column_height(), 80));
      vscroll.create(*this, core::rectangle(240, 0, static_cast<core::size_type>(win::scroll_bar::scroll_bar_width), 80));
      hscroll.create(*this, core::rectangle(0, 100, 240, static_cast<core::size_type>(win::scroll_bar::scroll_bar_width)));

      vscroll.set_max(place.height() * 10);
      vscroll.set_step(column_height());
      hscroll.set_max(place.width() * 10);
      hscroll.set_step(row_width());

      edge.set_visible();
      data.set_visible();
      columns.set_visible();
      rows.set_visible();
      hscroll.set_visible();
      vscroll.set_visible();
    }

    void table_view::handle_size (const core::size& sz) {
      vscroll.set_max(sz.height() * 10);
      hscroll.set_max(sz.width() * 10);
    }

    void table_view::handle_wheel_x (const core::point_type delta, const core::point&) {
      if (hscroll.is_enabled()) {
        hscroll.set_value(hscroll.get_value() - delta * hscroll.get_step(), true);
      }
    }

    void table_view::handle_wheel_y (const core::point_type delta, const core::point&) {
      if (vscroll.is_enabled()) {
        vscroll.set_value(vscroll.get_value() - delta * vscroll.get_step(), true);
      }
    }

    void table_view::handle_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      moved = false;
      data.take_focus();
      down_idx.clear();
      set_cursor(cursor::move());
    }

    void table_view::handle_left_btn_up(os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        const auto new_selection = data.get_index_at_point(pt);
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      last_mouse_point = core::point::undefined;
      down_idx.clear();
      set_cursor(cursor::arrow());
    }

    void table_view::handle_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = data.client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = last_mouse_point - pt;
          set_scroll_pos(get_scroll_pos() + delta);
          moved = true;
        }
        last_mouse_point = pt;
      } else {
        const auto new_hilite = data.get_index_at_point(pt);
        if (geometrie.hilite != new_hilite) {
          geometrie.hilite = new_hilite;
          redraw_all();
          send_client_message(this, detail::HILITE_CHANGE_MESSAGE, static_cast<int>(event_source::mouse));
        }
      }
    }

    void table_view::handle_column_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      moved = false;
      if (enable_h_size) {
        down_idx.column = geometrie.widths.split_idx_at(pt.x(), 2.0F);
      }
      columns.set_cursor(down_idx.column > -1 ? cursor::size_h() : cursor::move());
      columns.capture_pointer();
    }

    void table_view::handle_column_left_btn_up(os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        const auto new_selection = columns.get_index_at_point(pt);
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      last_mouse_point = core::point::undefined;
      down_idx.clear();
      columns.set_cursor(cursor::arrow());
      columns.uncapture_pointer();
    }

    void table_view::handle_column_mouse_move (os::key_state keys, const core::point& pt) {
      if (left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.x() - last_mouse_point.x();
          if (down_idx.column > -1) {
            geometrie.widths.set_size(down_idx.column, geometrie.widths.get_size(down_idx.column) + delta);
            redraw_all();
          } else {
            if (hscroll.is_enabled()) {
              hscroll.set_value(hscroll.get_value() - delta, true);
            }
          }
          moved = true;
        }
        last_mouse_point = pt;
      } else {
        const int idx = enable_h_size ? geometrie.widths.split_idx_at(pt.x(), 2.0F) : -1;
        columns.set_cursor(idx > -1 ? cursor::size_h() : cursor::arrow());
        if (idx < 0) {
          const auto new_hilite = columns.get_index_at_point(pt);
          if (geometrie.hilite != new_hilite) {
            geometrie.hilite = new_hilite;
            redraw_all();
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, static_cast<int>(event_source::mouse));
          }
        }
      }
    }

    void table_view::handle_row_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      moved = false;
      if (enable_v_size) {
        down_idx.row = geometrie.heights.split_idx_at(pt.y(), 2.0F);
      }
      rows.set_cursor(down_idx.row > -1 ? cursor::size_v() : cursor::move());
      rows.capture_pointer();
    }

    void table_view::handle_row_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        const auto new_selection = rows.get_index_at_point(pt);
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      last_mouse_point = core::point::undefined;
      down_idx.clear();
      rows.set_cursor(cursor::arrow());
      rows.uncapture_pointer();
    }

    void table_view::handle_row_mouse_move (os::key_state keys, const core::point& pt) {
      if (left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.y() - last_mouse_point.y();
          if (down_idx.row > -1) {
            geometrie.heights.set_size(down_idx.row, geometrie.heights.get_size(down_idx.row) + delta);
            redraw_all();
          } else {
            if (vscroll.is_enabled()) {
              vscroll.set_value(vscroll.get_value() - delta, true);
            }
          }
          moved = true;
        }
        last_mouse_point = pt;
      } else {
        const int idx = enable_v_size ? geometrie.heights.split_idx_at(pt.y(), 2.0F) : -1;
        rows.set_cursor(idx > -1 ? cursor::size_v() : cursor::arrow());
        if (idx < 0) {
          const auto new_hilite = rows.get_index_at_point(pt);
          if (geometrie.hilite != new_hilite) {
            geometrie.hilite = new_hilite;
            redraw_all();
            send_client_message(this, detail::HILITE_CHANGE_MESSAGE, static_cast<int>(event_source::mouse));
          }
        }
      }
    }

    void table_view::handle_key (os::key_state state,
                                 os::key_symbol key,
                                 const std::string&) {
      switch (key) {
        case keys::up:
        case keys::numpad::up:
          set_selection(get_selection() - table::position(0, 1), event_source::keyboard);
        break;
        case keys::down:
        case keys::numpad::down:
          set_selection(get_selection() + table::position(0, 1), event_source::keyboard);
        break;
        case keys::left:
        case keys::numpad::left:
          set_selection(get_selection() - table::position(1, 0), event_source::keyboard);
        break;
        case keys::right:
        case keys::numpad::right:
          set_selection(get_selection() + table::position(1, 0), event_source::keyboard);
        break;
        case keys::page_up:
        case keys::numpad::page_up: {
            const core::point pt = geometrie.position_of(geometrie.selection);
            core::size sz = data.client_size();
            if (alt_key_bit_mask::is_set(state)) {
              sz.height(0);
            } else {
              sz.width(0);
            }
            set_scroll_pos(get_scroll_pos() - sz);
            set_selection(geometrie.index_at(pt), event_source::logic);
            break;
          }
        case keys::page_down:
        case keys::numpad::page_down: {
            const core::point pt = geometrie.position_of(geometrie.selection);
            core::size sz = data.client_size();
            if (alt_key_bit_mask::is_set(state)) {
              sz.height(0);
            } else {
              sz.width(0);
            }
            set_scroll_pos(get_scroll_pos() + sz);
            set_selection(geometrie.index_at(pt), event_source::logic);
            break;
          }
        break;
        case keys::home:
        case keys::numpad::home:
          set_selection(table::position(0, 0), event_source::keyboard);
        break;
        case keys::f2:
        case keys::enter:
          send_client_message(this, detail::SELECTION_COMMIT_MESSAGE);
        break;
        case keys::escape:
          clear_selection(event_source::keyboard);
        break;
      }
    }

    // --------------------------------------------------------------------------
    table_edit::table_edit (core::size_type default_width,
                            core::size_type default_height,
                            core::size_type row_width,
                            core::size_type column_height,
                            text_origin align,
                            os::color foreground,
                            os::color background,
                            os::color header_background)
      : table_view(default_width, default_height, row_width, column_height,
                   align, foreground, background, header_background)
      , enable_edit(true)
    {
      register_event_handler(REGISTER_FUNCTION, win::selection_commit_event(this, &table_edit::enter_edit));

      editor.register_event_handler(REGISTER_FUNCTION,
                                       win::btn_down_event([&](os::key_state, const core::point& pt) {
        if(!editor.client_area().is_inside(pt)) {
          commit_edit();
        }
      }));

      editor.register_event_handler(REGISTER_FUNCTION, win::selection_cancel_event(this, &table_edit::cancel_edit));
      editor.register_event_handler(REGISTER_FUNCTION, win::selection_commit_event(this, &table_edit::commit_edit));

      vscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type) {
        if (editor.is_visible()) {
          editor.move(geometrie.position_of(get_selection()));
        }
      }));
      hscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&] (core::point::type) {
        if (editor.is_visible()) {
          editor.move(geometrie.position_of(get_selection()));
        }
      }));

      register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&] (event_source) {
        commit_edit();
      }));

    }

    void table_edit::enter_edit () {
      if (enable_edit && data_source) {
        auto cell = get_selection();
        auto pt = geometrie.position_of(cell);
        auto sz = geometrie.get_size(cell);
        if (!editor.is_valid()) {
          editor.create(*reinterpret_cast<container*>(&data), core::rectangle(0, 0, 10, 10));
        }
        editor.place(core::rectangle(pt, sz));
        editor.set_text(data_source(cell));
        editor.set_cursor_pos(editor.get_text_length());
        editor.set_visible();
        editor.take_focus();
      }
    }

    void table_edit::commit_edit () {
      if (data_target && editor.is_visible()) {
        auto pos = editor.position();
        auto cell = geometrie.index_at(pos);
        data_target(cell, editor.get_text());
      }
      cancel_edit();
    }

    void table_edit::cancel_edit () {
      editor.set_visible(false);
      data.take_focus();
    }

    void table_edit::set_data_source_and_target (const std::function<table::data_source>& data_source,
                                                 const std::function<table::data_target>& data_target) {
      this->data_source = data_source;
      this->data_target = data_target;
    }


  } // win

} // gui
