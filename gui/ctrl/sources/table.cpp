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

    namespace table {

      // --------------------------------------------------------------------------
      void cell_layout::set_size (std::size_t idx, core::size_type size) {
        if (sizes[idx] != size) {
          sizes[idx] = size;
          calc();
        }
      }

      void cell_layout::set_offset (core::point_type offset) {
        if (offset < 0) {
          offset = 0;
        }
        if (this->offset != offset) {
          this->offset = offset;
          calc();
        }
      }

      std::size_t cell_layout::index_at (core::point_type pt) const {
        core::point_type pos = -get_offset();
        std::size_t idx = 0;
        while (pos <= pt) {
          pos += get_size(idx);
          ++idx;
        }
        return idx - 1;
      }

      void cell_layout::calc () {
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
      cell_view::cell_view (cell_geometrie& geometrie,
                            text_origin align,
                            os::color foreground,
                            os::color background)
        : geometrie(geometrie)
        , aligns(align)
        , foregrounds(foreground)
        , backgrounds(background)
      {}

      void cell_view::create (const container& parent,
                              const core::rectangle& place) {
        window::create(clazz, parent, place);
      }

      void cell_view::set_drawer (const std::function<table_cell_drawer>& drawer) {
        this->drawer = drawer;
      }

      void cell_view::set_drawer (std::function<table_cell_drawer>&& drawer) {
        this->drawer = std::move(drawer);
      }

      // --------------------------------------------------------------------------
      no_erase_window_class cell_view::clazz = create_group_window_clazz(color::very_very_light_gray);

      // --------------------------------------------------------------------------
      data_view::data_view (cell_geometrie& geometrie,
                            text_origin align,
                            os::color foreground,
                            os::color background)
        : super(geometrie, align, foreground, background)
      {
        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &data_view::paint));
      }

      void data_view::paint (const draw::graphics& graph) {
        core::rectangle place = client_area();

        if (drawer) {
          core::point_type xMax = place.x2();
          core::point_type yMax = place.y2();

          std::size_t row = geometrie.heights.get_first_idx();
          core::point_type y = geometrie.heights.get_first_offset();

          while (y < yMax) {
            const core::size_type height = geometrie.heights.get_size(row);

            std::size_t column = geometrie.widths.get_first_idx();
            core::point_type x = geometrie.widths.get_first_offset();

            while (x < xMax) {
              const core::size_type width = geometrie.widths.get_size(column);
              core::rectangle cell(x, y, width, height);

              drawer(column, row, graph, cell,
                     aligns.get_cell(column, row),
                     foregrounds.get_cell(column, row),
                     backgrounds.get_cell(column, row),
                     geometrie.selection.is_cell(column, row),
                     geometrie.hilite.is_cell(column, row));

              x += width;
              ++column;
            }
            y += height;
            ++row;
          }
        }
      }

      cell_position data_view::get_index_at_point (const core::point& pt) const {
        return cell_position(geometrie.widths.index_at(pt.x()),
                             geometrie.heights.index_at(pt.y()));
      }

      // --------------------------------------------------------------------------
      column_view::column_view (cell_geometrie& geometrie,
                                text_origin align,
                                os::color foreground,
                                os::color background)
        : super(geometrie, align, foreground, background)
      {
        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &column_view::paint));
      }

      void column_view::paint (const draw::graphics& graph) {
        core::rectangle place = client_area();

        if (drawer) {
          core::point_type xMax = place.x2();
          const core::size_type height = place.height();

          std::size_t column = geometrie.widths.get_first_idx();
          core::point_type x = geometrie.widths.get_first_offset();

          while (x < xMax) {
            const core::size_type width = geometrie.widths.get_size(column);
            core::rectangle cell(x, 0, width, height);

            drawer(column, 0, graph, cell,
                   aligns.get_cell(column, 0),
                   foregrounds.get_cell(column, 0),
                   backgrounds.get_cell(column, 0),
                   geometrie.selection.is_column(column),
                   geometrie.hilite.is_column(column));

            x += width;
            ++column;
          }
        }
      }

      cell_position column_view::get_index_at_point (const core::point& pt) const {
        return cell_position(geometrie.widths.index_at(pt.x()), -1);
      }

      // --------------------------------------------------------------------------
      row_view::row_view (cell_geometrie& geometrie,
                          text_origin align,
                          os::color foreground,
                          os::color background)
        : super(geometrie, align, foreground, background)
      {
        super::register_event_handler(REGISTER_FUNCTION, paint_event(this, &row_view::paint));
      }

      void row_view::paint (const draw::graphics& graph) {
        core::rectangle place = client_area();

        if (drawer) {
          core::point_type yMax = place.y2();
          const core::size_type width = place.width();

          std::size_t row = geometrie.heights.get_first_idx();
          core::point_type y = geometrie.heights.get_first_offset();

          while (y < yMax) {
            const core::size_type height = geometrie.heights.get_size(row);

            core::rectangle cell(0, y, width, height);

            drawer(0, row, graph, cell,
                   aligns.get_cell(0, row),
                   foregrounds.get_cell(0, row),
                   backgrounds.get_cell(0, row),
                   geometrie.selection.is_row(row),
                   geometrie.hilite.is_row(row));

            y += height;
            ++row;
          }
        }
      }

      cell_position row_view::get_index_at_point (const core::point& pt) const {
        return cell_position(-1, geometrie.heights.index_at(pt.y()));
      }

      // --------------------------------------------------------------------------
      std::function<table_cell_drawer> default_data_drawer (const std::function<table_data_source>& src) {
        return [src] (std::size_t column,
                      std::size_t row,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const text_origin align,
                      const os::color& foreground,
                      const os::color& background,
                      bool selected,
                      bool hilited) {
          paint::text_cell<std::string, draw::frame::lines>(src(column, row), graph, place, align,
                                                      foreground, background, selected, hilited);
        };
      }

      // --------------------------------------------------------------------------
      std::function<table_cell_drawer> default_header_drawer (const std::function<table_data_source>& src) {
        return [src] (std::size_t column,
                      std::size_t row,
                      const draw::graphics& graph,
                      const core::rectangle& place,
                      const text_origin align,
                      const os::color& foreground,
                      const os::color& background,
                      bool selected,
                      bool hilited) {
          paint::text_cell<std::string, draw::frame::raised_relief>(src(column, row), graph, place, align,
                                                              foreground, background, selected, hilited);
        };
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
      : super(column_height, scroll_bar::get_scroll_bar_width(), row_width, scroll_bar::get_scroll_bar_width())
      , geometrie(default_width, default_height)
      , data(geometrie, align, foreground, background)
      , columns(geometrie, align, foreground, header_background)
      , rows(geometrie, align, foreground, header_background)
      , last_mouse_point(core::point::undefined)
      , moved(false)
    {
      get_layout().set_center_top_bottom_left_right(&data, &columns, &hscroll, &rows, &vscroll);
      register_event_handler(REGISTER_FUNCTION, create_event(this, &table_view::handle_created));
      register_event_handler(REGISTER_FUNCTION, win::size_event(this, &table_view::handle_size));
      vscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&](core::point::type pos) {
        geometrie.heights.set_offset(pos);
        data.redraw_later();
        rows.redraw_later();
      }));
      hscroll.register_event_handler(REGISTER_FUNCTION, scroll_event([&](core::point::type pos) {
        geometrie.widths.set_offset(pos);
        data.redraw_later();
        columns.redraw_later();
      }));

      data.register_event_handler(REGISTER_FUNCTION, wheel_x_event(this, &table_view::handle_wheel_x));
      data.register_event_handler(REGISTER_FUNCTION, wheel_y_event(this, &table_view::handle_wheel_y));
      columns.register_event_handler(REGISTER_FUNCTION, wheel_x_event(this, &table_view::handle_wheel_x));
      rows.register_event_handler(REGISTER_FUNCTION, wheel_y_event(this, &table_view::handle_wheel_y));

      data.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_mouse_move));
      data.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_left_btn_down));
      data.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_left_btn_up));

      columns.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_column_mouse_move));
      columns.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_left_btn_down));
      columns.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_column_left_btn_up));

      rows.register_event_handler(REGISTER_FUNCTION, mouse_move_event(this, &table_view::handle_row_mouse_move));
      rows.register_event_handler(REGISTER_FUNCTION, left_btn_down_event(this, &table_view::handle_left_btn_down));
      rows.register_event_handler(REGISTER_FUNCTION, left_btn_up_event(this, &table_view::handle_row_left_btn_up));

    }

    core::point table_view::get_scroll_pos () const {
      return core::point(hscroll.get_value(), vscroll.get_value());
    }

    void table_view::set_scroll_pos (const core::point& pos) {
      if (hscroll.is_enabled()) {
        hscroll.set_value(pos.x());
        geometrie.widths.set_offset(pos.x());
      }
      if (vscroll.is_enabled()) {
        vscroll.set_value(pos.y());
        geometrie.heights.set_offset(pos.y());
      }
      data.redraw_later();
      rows.redraw_later();
      columns.redraw_later();
    }

    int table_view::row_width () const {
      return get_layout().get_left_width();
    }

    int table_view::column_height () const {
      return get_layout().get_top_height();
    }

    void table_view::handle_created (win::window* win, const core::rectangle& place) {
      edge.create(*this, core::rectangle(0, 0, row_width(), column_height()));
      data.create(*this, core::rectangle(row_width(), column_height(), 160, 80));
      columns.create(*this, core::rectangle(row_width(), 0, 160, column_height()));
      rows.create(*this, core::rectangle(0, row_width(), column_height(), 80));
      vscroll.create(*this, core::rectangle(240, 0, win::scroll_bar::scroll_bar_width, 80));
      hscroll.create(*this, core::rectangle(0, 100, 240, win::scroll_bar::scroll_bar_width));

      vscroll.set_max(place.height() * 2);
      vscroll.set_step(column_height());
      hscroll.set_max(place.width() * 2);
      hscroll.set_step(row_width());

      edge.set_visible();
      data.set_visible();
      columns.set_visible();
      rows.set_visible();
      hscroll.set_visible();
      vscroll.set_visible();

      get_layout().layout(place.size());
    }

    void table_view::handle_size (const core::size& sz) {
      vscroll.set_max(sz.height());
      hscroll.set_max(sz.width());
    }

    void table_view::handle_wheel_x (const core::point_type delta, const core::point&) {
      if (hscroll.is_enabled()) {
        hscroll.set_value(hscroll.get_value() - delta, true);
      }
    }

    void table_view::handle_wheel_y (const core::point_type delta, const core::point&) {
      if (vscroll.is_enabled()) {
        vscroll.set_value(vscroll.get_value() - delta, true);
      }
    }

    void table_view::handle_left_btn_down (os::key_state, const core::point& pt) {
      last_mouse_point = pt;
      moved = false;
      data.take_focus();
    }

    void table_view::handle_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        geometrie.selection = data.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
      last_mouse_point = core::point::undefined;
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
        geometrie.hilite = data.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
    }

    void table_view::handle_column_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        geometrie.selection = columns.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
      last_mouse_point = core::point::undefined;
    }

    void table_view::handle_column_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = columns.client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.x() - last_mouse_point.x();
          handle_wheel_x(delta, pt);
          moved = true;
        }
        last_mouse_point = pt;
      } else {
        geometrie.hilite = columns.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
    }

    void table_view::handle_row_left_btn_up (os::key_state keys, const core::point& pt) {
      if (!moved && (last_mouse_point != core::point::undefined)) {
        geometrie.selection = rows.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
      last_mouse_point = core::point::undefined;
    }

    void table_view::handle_row_mouse_move (os::key_state keys, const core::point& pt) {
      const core::rectangle r = rows.client_area();
      if (left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (last_mouse_point != core::point::undefined) {
          auto delta = pt.y() - last_mouse_point.y();
          handle_wheel_y(delta, pt);
          moved = true;
        }
        last_mouse_point = pt;
      } else {
        geometrie.hilite = rows.get_index_at_point(pt);
        data.redraw_later();
        columns.redraw_later();
        rows.redraw_later();
      }
    }

  } // win

} // gui
