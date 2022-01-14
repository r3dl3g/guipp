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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <set>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/table.h>


namespace gui {

  namespace ctrl {

    namespace table {

      namespace data {

        // --------------------------------------------------------------------------
        void spawns::set (const position& cell, const spawn& sp) {
          if (data.size() <= (cell.x() + sp.x)) {
            data.resize(cell.x() + sp.x + 1);
          }
          for(int16_t x = 0; x < sp.x + 1; ++x) {
            column& c = data[cell.x() + x];
            if (c.size() <= (cell.y() + sp.y)) {
              c.resize(cell.y() + sp.y + 1);
            }
            for(int16_t y = 0; y < sp.y + 1; ++y) {
              c[cell.y() + y] = {int16_t(-x), int16_t(-y)};
            }
          }
          data[cell.x()][cell.y()] = sp;
        }

        spawn spawns::get (const position& cell) const {
          if (cell.x() < data.size()) {
            const std::vector<spawn>& c = data[cell.x()];
            if (cell.y() < c.size()) {
              return c[cell.y()];
            }
          }
          return {};
        }

        void spawns::clear (const position& cell) {
          spawn sp = get(cell);
          if (!sp.is_empty()) {
            for(int x = 0; x < sp.x + 1; ++x) {
              column& c = data[cell.x() + x];
              for(int y = 0; y < sp.y + 1; ++y) {
                c[cell.y() + y] = {};
              }
            }
          }
        }

      } // namespace data

      table::position invalid(-1, -1);

      inline bool is_valid (const table::position& p) {
        return p != invalid;
      }

      inline void clear (table::position& p) {
        p = invalid;
      }

      // --------------------------------------------------------------------------
      core::size metric::get_size (const position& cell) const {
        core::size sz(widths.get_size(cell.x()), heights.get_size(cell.y()));
        const auto spwn = spawns.get(cell);
        if (spwn.is_spawn()) {
          for(int dx = 0; dx < spwn.x; ++dx) {
            sz.width(sz.width() + widths.get_size(cell.x() + dx));
          }
          for(int dy = 0; dy < spwn.y; ++dy) {
            sz.height(sz.height() + heights.get_size(cell.y() + dy));
          }
        }
        return sz;
      }

      // --------------------------------------------------------------------------
      void layout::set_size (std::size_t idx, core::size::type size) {
        if (sizes[idx] != size) {
          sizes[idx] = size;
          if (idx > 0) {
            if (idx < positions.size()) {
              positions.resize(idx - 1);
            }
          } else {
            positions.clear();
          }
          calc();
        }
      }

      void layout::set_offset (core::point::type offset) {
        if (offset < 0) {
          offset = 0;
        }
        if (this->offset != offset) {
          this->offset = offset;
          calc();
        }
      }

      int layout::index_at (core::point::type pt) const {
        core::point::type pos = -get_offset();
        int idx = 0;
        while (pos <= pt) {
          pos += get_size(idx);
          ++idx;
        }
        return idx - 1;
      }

      int layout::split_idx_at (core::point::type pt, core::size::type delta) const {
        core::point::type pos = -get_offset();
        const core::point::type lower = pt - delta;
        const core::point::type upper = pt + delta;
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

      core::point::type layout::position_of (int idx) const {
        if (idx < 1) {
          return -get_offset();
        }
        if (idx <= positions.size()) {
          return positions[idx - 1] - get_offset();
        }
        const int startidx = std::max(0, static_cast<int>(positions.size()));
        core::point::type pos = startidx == 0 ? 0 : positions[startidx - 1];
        positions.resize(idx);
        logging::trace() << "Calc position for idx " << startidx << " to " << idx;
        for (int i = startidx; i < idx; ++i) {
          pos += get_size(i);
          positions[i] = pos;
        }
        return pos - get_offset();
      }

      void layout::calc () {
        core::point::type pos = -get_offset();
        first_idx = 0;
        first_offset = 0;
        while (pos <= 0) {
          first_offset = pos;
          pos += get_size(static_cast<int>(first_idx));
          ++first_idx;
        }
        --first_idx;
      }

      // --------------------------------------------------------------------------
      namespace paint {

        item_state get_cell_item_state (const std::function<filter::selection_and_hilite>& selection_filter,
                                        const std::function<filter::selection_and_hilite>& hilite_filter,
                                        const position& cell, const metric& geometrie,
                                        bool disabled = false) {
          return item_state(hilite_filter(cell, geometrie), selection_filter(cell, geometrie), disabled);
        }

        void draw_table_data (draw::graphics& graph,
                              const core::rectangle& place,
                              const metric& geometrie,
                              const data::matrix<text_origin_t>& aligns,
                              const data::matrix<os::color>& foregrounds,
                              const data::matrix<os::color>& backgrounds,
                              const std::function<cell_drawer>& drawer,
                              const std::function<filter::selection_and_hilite>& selection_filter,
                              const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const core::point max_sz = place.x2y2();
            position cell(0, static_cast<position::type>(geometrie.heights.get_first_idx()));
            core::point::type y = place.y() + geometrie.heights.get_first_offset();

            std::set<position> painted_spawns;

            while (y < max_sz.y()) {
              const core::size::type height = geometrie.heights.get_size(cell.y());

              cell.x(static_cast<int>(geometrie.widths.get_first_idx()));
              core::point::type x = place.x() + geometrie.widths.get_first_offset();

              while (x < max_sz.x()) {
                const core::size::type width = geometrie.widths.get_size(cell.x());
                core::rectangle area(x, y, width, height);
                const auto spwn = geometrie.spawns.get(cell);

                position paint_cell = cell;
                if (spwn.is_hidden()) {
                  paint_cell = cell + offset(spwn.x, spwn.y);
                  if (painted_spawns.find(paint_cell) == painted_spawns.end()) {
                    core::point::type mx = x;
                    core::point::type my = y;
                    for (int i = 0; i > spwn.x; --i) {
                      mx -= geometrie.widths.get_size(cell.x() + i);
                    }
                    for (int i = 0; i > spwn.y; --i) {
                      my -= geometrie.heights.get_size(cell.y() + i);
                    }
                    area.set_position({mx, my});
                    area.set_size(geometrie.get_size(paint_cell));
                    painted_spawns.insert(paint_cell);
                  } else {
                    area.set_size(core::size::zero);
                  }
                } else if (spwn.is_spawn()) {
                  painted_spawns.insert(paint_cell);
                  area.set_size(geometrie.get_size(paint_cell));
                }
                if (!area.size().empty()) {
                  drawer(paint_cell, graph, area,
                         aligns.get_cell(paint_cell),
                         foregrounds.get_cell(paint_cell),
                         backgrounds.get_cell(paint_cell),
                         get_cell_item_state(selection_filter, hilite_filter, cell, geometrie));
                }

                x += width;
                cell.move_x(1);
              }
              y += height;
              cell.move_y(1);
            }
          }
        }

        void draw_table_column (draw::graphics& graph,
                                const core::rectangle& place,
                                const metric& geometrie,
                                const data::vector<text_origin_t>& aligns,
                                const data::vector<os::color>& foregrounds,
                                const data::vector<os::color>& backgrounds,
                                const std::function<cell_drawer>& drawer,
                                const std::function<filter::selection_and_hilite>& selection_filter,
                                const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const auto max_x = place.x2();
            position cell(static_cast<position::type>(geometrie.widths.get_first_idx()), 0);
            core::point::type x = place.x() + geometrie.widths.get_first_offset();

            while (x < max_x) {
              const core::size::type width = geometrie.widths.get_size(cell.x());
              drawer(cell, graph,
                     core::rectangle(x, place.y(), width, place.height()),
                     aligns.get(cell.x()),
                     foregrounds.get(cell.x()),
                     backgrounds.get(cell.x()),
                     get_cell_item_state(selection_filter, hilite_filter, cell, geometrie));

              x += width;
              cell.x(1 + cell.x());
            }
          }
        }

        void draw_table_row (draw::graphics& graph,
                             const core::rectangle& place,
                             const metric& geometrie,
                             const data::vector<text_origin_t>& aligns,
                             const data::vector<os::color>& foregrounds,
                             const data::vector<os::color>& backgrounds,
                             const std::function<cell_drawer>& drawer,
                             const std::function<filter::selection_and_hilite>& selection_filter,
                             const std::function<filter::selection_and_hilite>& hilite_filter) {
          if (drawer) {
            const core::point::type max_y = place.y2();
            position cell(0, static_cast<position::type>(geometrie.heights.get_first_idx()));
            core::point::type y = place.y() + geometrie.heights.get_first_offset();

            while (y < max_y) {
              const core::size::type height = geometrie.heights.get_size(cell.y());

              drawer(cell, graph,
                     core::rectangle(place.x(), y, place.width(), height),
                     aligns.get(cell.y()),
                     foregrounds.get(cell.y()),
                     backgrounds.get(cell.y()),
                     get_cell_item_state(selection_filter, hilite_filter, cell, geometrie));

              y += height;
              cell.y(1 + cell.y());
            }
          }
        }

      }

      // --------------------------------------------------------------------------
      std::function<cell_drawer> default_data_drawer (const std::function<data_source>& src) {
        return [src] (const position &cell, draw::graphics& graph, const core::rectangle& place,
                      const text_origin_t align, const os::color& foreground, const os::color& background,
                      item_state state) {
          gui::look::text_cell<std::string, draw::frame::lines>(src(cell), graph, place, align,
                                                                foreground, background, state);
        };
      }

      // --------------------------------------------------------------------------
      std::function<cell_drawer> default_header_drawer (const std::function<data_source>& src) {
        return [src] (const position &cell, draw::graphics& graph, const core::rectangle& place,
                      const text_origin_t align, const os::color& foreground, const os::color& background,
                      item_state state) {
          gui::look::text_cell<std::string, draw::frame::raised_relief>(src(cell), graph, place, align,
                                                                        foreground, background, state);
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
                            text_origin_t align,
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
        super::on_paint(draw::paint([&](draw::graphics&  graph){
          paint::draw_table_data(graph, client_geometry(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

      // --------------------------------------------------------------------------
      column_view::column_view (metric& geometrie,
                                text_origin_t align,
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
        super::on_paint(draw::paint([&](draw::graphics&  graph) {
          paint::draw_table_column(graph, client_geometry(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

      // --------------------------------------------------------------------------
      row_view::row_view (metric& geometrie,
                          text_origin_t align,
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
        super::on_paint(draw::paint([&](draw::graphics&  graph){
          paint::draw_table_row(graph, client_geometry(), geometrie, aligns, foregrounds, backgrounds, drawer, selection_filter, hilite_filter);
        }));
      }

    } // namespace table

    // --------------------------------------------------------------------------
    core::point default_scroll_maximum (const core::size& size,
                                        const core::point& current_pos,
                                        const core::point&) {
      return current_pos + size * 2.0;
    }

    // --------------------------------------------------------------------------
    table_view::table_view (core::size::type default_width,
                            core::size::type default_height,
                            core::size::type row_width,
                            core::size::type column_height,
                            text_origin_t align,
                            os::color foreground,
                            os::color background,
                            os::color header_background)
      : super(column_height, static_cast<core::size::type>(scroll_bar::get_scroll_bar_width()),
              row_width, static_cast<core::size::type>(scroll_bar::get_scroll_bar_width()))
      , geometrie(default_width, default_height)
      , data(geometrie, align, foreground, background)
      , columns(geometrie, align, foreground, header_background)
      , rows(geometrie, align, foreground, header_background)
      , enable_v_size(false)
      , enable_h_size(false)
      , enable_hilite_(false)
      , enable_selection_(false)
      , moved(false)
      , last_mouse_point(core::native_point::undefined)
    {
      init();
    }

    table_view::table_view (const table_view& rhs)
      : super(rhs)
      , geometrie(rhs.geometrie)
      , data(geometrie, rhs.data)
      , columns(geometrie, rhs.columns)
      , rows(geometrie, rhs.rows)
      , enable_v_size(false)
      , enable_h_size(false)
      , enable_hilite_(false)
      , enable_selection_(false)
      , moved(false)
      , last_mouse_point(core::native_point::undefined)
    {
      init();
    }

    table_view::table_view (table_view&& rhs) noexcept
      : super(std::move(rhs))
      , geometrie(std::move(rhs.geometrie))
      , data(geometrie, std::move(rhs.data))
      , columns(geometrie, std::move(rhs.columns))
      , rows(geometrie, std::move(rhs.rows))
      , enable_v_size(false)
      , enable_h_size(false)
      , enable_hilite_(false)
      , enable_selection_(false)
      , moved(false)
      , last_mouse_point(core::native_point::undefined)
    {
      init();
    }

    void table_view::enable_size (bool h_size, bool v_size) {
      enable_h_size = h_size;
      enable_v_size = v_size;
    }

    void table_view::enable_hilite (bool hilite) {
      enable_hilite_ = hilite;
    }

    void table_view::enable_selection (bool selection) {
      enable_selection_ = selection;
    }

    bool table_view::enable_hilite () const {
      return enable_hilite_;
    }

    bool table_view::enable_selection () const {
      return enable_selection_;
    }

    void table_view::handle_layout (const core::rectangle& r) {
      set_scroll_maximum(scroll_maximum(data.client_size(), geometrie.get_offset(), core::point(hscroll.get_max(), vscroll.get_max())));
      edge.position(r.top_left());
    }

    void table_view::handle_scroll (const core::point& pos) {
      geometrie.set_offset(pos);
      if ((hscroll.get_selection() == scrollbar_item::nothing) && (vscroll.get_selection() == scrollbar_item::nothing)) {
        set_scroll_maximum(scroll_maximum(data.client_size(), pos, core::point(hscroll.get_max(), vscroll.get_max())));
      }
      redraw_all();
    }

    void table_view::init () {
      if (!enable_selection_) {
        table::clear(geometrie.selection);
      }
      if (!enable_hilite_) {
        geometrie.hilite.clear();
      }
      set_scroll_maximum_calcer(default_scroll_maximum);
      get_layout().set_center_top_bottom_left_right(layout::lay(data), layout::lay(columns), layout::lay(hscroll), layout::lay(rows), layout::lay(vscroll));
      on_create(util::bind_method(this, &table_view::handle_created));
      on_layout(util::bind_method(this, &table_view::handle_layout));
      vscroll.on_scroll([&] (core::point::type pos) {
        handle_scroll(core::point(geometrie.widths.get_offset(), pos));
      });
      hscroll.on_scroll([&] (core::point::type pos) {
        handle_scroll(core::point(pos, geometrie.heights.get_offset()));
      });

      data.on_any_key_down(util::bind_method(this, &table_view::handle_key));
#ifndef GUIPP_BUILD_FOR_MOBILE
      data.on_left_btn_dblclk([&](os::key_state, const core::native_point&) {
        notify_event(detail::SELECTION_COMMIT_MESSAGE);
      });
#endif // GUIPP_BUILD_FOR_MOBILE

      data.on_wheel<orientation_t::horizontal>([&](const core::native_point::type delta, const core::native_point& pt){
        hscroll.handle_wheel(delta, pt);
      });
      data.on_wheel<orientation_t::vertical>([&](const core::native_point::type delta, const core::native_point& pt){
        vscroll.handle_wheel(delta, pt);
      });
      data.on_mouse_move(util::bind_method(this, &table_view::handle_mouse_move));
      data.on_left_btn_down(util::bind_method(this, &table_view::handle_left_btn_down));
      data.on_left_btn_up(util::bind_method(this, &table_view::handle_left_btn_up));

      columns.on_wheel<orientation_t::horizontal>([&](const core::native_point::type delta, const core::native_point & pt){
        hscroll.handle_wheel(delta, pt);
      });
      columns.on_mouse_move(util::bind_method(this, &table_view::handle_column_mouse_move));
      columns.on_left_btn_down(util::bind_method(this, &table_view::handle_column_left_btn_down));
      columns.on_left_btn_up(util::bind_method(this, &table_view::handle_column_left_btn_up));

      rows.on_wheel<orientation_t::vertical>([&](const core::native_point::type delta, const core::native_point& pt){
        vscroll.handle_wheel(delta, pt);
      });
      rows.on_mouse_move(util::bind_method(this, &table_view::handle_row_mouse_move));
      rows.on_left_btn_down(util::bind_method(this, &table_view::handle_row_left_btn_down));
      rows.on_left_btn_up(util::bind_method(this, &table_view::handle_row_left_btn_up));

      edge.set_background(columns.get_default_background());
    }

    core::point table_view::get_scroll_pos () const {
      return core::point(hscroll.get_value(), vscroll.get_value());
    }

    void table_view::set_scroll_pos (const core::point& pos) {
      if (hscroll.is_enabled()) {
        hscroll.set_value(pos.x(), false);
        geometrie.widths.set_offset(hscroll.get_value());
      }
      if (vscroll.is_enabled()) {
        vscroll.set_value(pos.y(), false);
        geometrie.heights.set_offset(vscroll.get_value());
      }
      redraw_all();
    }

    void table_view::clear_selection (event_source notify) {
      if (table::is_valid(geometrie.selection)) {
        table::clear(geometrie.selection);
        if (notify != event_source::logic) {
          notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
          redraw_all();
        }
      }
    }

    void table_view::set_selection (table::position selection, event_source notify) {
      if (selection.x() < -1) {
        selection.x(-1);
      }
      if (selection.y() < -1) {
        selection.y(-1);
      }
      if (geometrie.selection != selection) {
        geometrie.selection = selection;
        if (table::is_valid(geometrie.selection)) {
          make_selection_visible();
          redraw_all();
        }
        notify_event(detail::SELECTION_CHANGE_MESSAGE, static_cast<int>(notify));
      }
    }

    const table::position& table_view::get_selection () const {
      return geometrie.selection;
    }

    bool table_view::has_selection () const {
      return table::is_valid(geometrie.selection);
    }

    table::position table_view::get_valid_selection (const table::offset& offs) const {
      const auto x = geometrie.selection.x() + offs.width();
      const auto y = geometrie.selection.y() + offs.height();
      return table::position(x < 0 ? 0 : x, y < 0 ? 0 : y);
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
      data.invalidate();
      rows.invalidate();
      columns.invalidate();
    }

    core::size::type table_view::row_width () const {
      return get_layout().get_left_width();
    }

    core::size::type table_view::column_height () const {
      return get_layout().get_top_height();
    }

    void table_view::set_scroll_maximum_calcer (std::function<scroll_maximum_calcer> caller) {
      scroll_maximum = caller;
    }

    void table_view::set_scroll_maximum (const core::point& pos) {
      vscroll.set_max(pos.y());
      hscroll.set_max(pos.x());
    }

    void table_view::handle_created () {
      edge.create(*this, core::rectangle(0, 0, row_width(), column_height()));
      data.create(*this, core::rectangle(row_width(), column_height(), 160, 80));
      columns.create(*this, core::rectangle(row_width(), 0, 160, column_height()));
      rows.create(*this, core::rectangle(0, row_width(), column_height(), 80));
      vscroll.create(*this, core::rectangle(240, 0, static_cast<core::size::type>(scroll_bar::scroll_bar_width), 80));
      hscroll.create(*this, core::rectangle(0, 100, 240, static_cast<core::size::type>(scroll_bar::scroll_bar_width)));

      core::rectangle r = geometry();
      vscroll.set_max(r.height() * 2);
      vscroll.set_step(column_height());
      hscroll.set_max(r.width() * 2);
      hscroll.set_step(row_width());

      edge.set_visible();
      data.set_visible();
      columns.set_visible();
      rows.set_visible();
      hscroll.set_visible();
      vscroll.set_visible();
    }

    void table_view::handle_left_btn_down (os::key_state, const core::native_point& pt) {
      last_mouse_point = pt;
      moved = false;
      data.take_focus();
      down_idx.clear();
      set_cursor(win::cursor::move());
    }

    void table_view::handle_left_btn_up (os::key_state keys, const core::native_point& pt) {
      if (enable_selection_ && !moved && (last_mouse_point != core::native_point::undefined)) {
        auto new_selection = data.get_index_at_point(data.surface_to_client(pt));
        const auto spawn = geometrie.spawns.get(new_selection);
        if (spawn.is_hidden()) {
          new_selection.move({spawn.x, spawn.y});
        }
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (core::control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
#ifdef GUIPP_BUILD_FOR_MOBILE
        } else {
          notify_event(detail::SELECTION_COMMIT_MESSAGE);
#endif // GUIPP_BUILD_FOR_MOBILE
        }
      }
      last_mouse_point = core::native_point::undefined;
      down_idx.clear();
      set_cursor(win::cursor::arrow());
    }

    void table_view::handle_mouse_move (os::key_state keys, const core::native_point& pt) {
      const auto r = data.surface_geometry();
      if (core::left_button_bit_mask::is_set(keys) && r.is_inside(pt)) {
        if (last_mouse_point != core::native_point::undefined) {
          auto delta = last_mouse_point - pt;
          set_scroll_pos(get_scroll_pos() + core::global::scale_from_native(delta));
          moved = true;
        }
        last_mouse_point = pt;
      } else if (enable_hilite_) {
        const auto new_hilite = data.get_index_at_point(data.surface_to_client(pt));
        if (geometrie.hilite != new_hilite) {
          geometrie.hilite = new_hilite;
          redraw_all();
          notify_event(detail::HILITE_CHANGE_MESSAGE, true);
        }
      }
    }

    void table_view::handle_column_left_btn_down (os::key_state, const core::native_point& npt) {
      last_mouse_point = npt;
      moved = false;
      if (enable_h_size) {
        down_idx.x(geometrie.widths.split_idx_at(columns.surface_to_client(npt).x(), 2.0F));
      }
      columns.set_cursor(down_idx.x() > -1 ? win::cursor::size_h() : win::cursor::move());
      columns.capture_pointer();
    }

    void table_view::handle_column_left_btn_up (os::key_state keys, const core::native_point& npt) {
      if (enable_selection_ && !moved && (last_mouse_point != core::native_point::undefined)) {
        const auto new_selection = columns.get_index_at_point(columns.surface_to_client(npt));
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (core::control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      last_mouse_point = core::native_point::undefined;
      down_idx.clear();
      columns.set_cursor(win::cursor::arrow());
      columns.uncapture_pointer();
    }

    void table_view::handle_column_mouse_move (os::key_state keys, const core::native_point& npt) {
      if (core::left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::native_point::undefined) {
          auto delta = core::global::scale_from_native<core::size::type>((npt.x() - last_mouse_point.x()));
          if (down_idx.x() > -1) {
            geometrie.widths.set_size(down_idx.x(), geometrie.widths.get_size(down_idx.x()) + delta);
            redraw_all();
          } else {
            if (hscroll.is_enabled()) {
              hscroll.set_value(hscroll.get_value() - delta, true);
            }
          }
          moved = true;
        }
        last_mouse_point = npt;
      } else {
        core::point cpt = columns.surface_to_client(npt);
        const int idx = enable_h_size ? geometrie.widths.split_idx_at(cpt.x(), 2.0F) : -1;
        columns.set_cursor(idx > -1 ? win::cursor::size_h() : win::cursor::arrow());
        if (enable_hilite_ && (idx < 0)) {
          const auto new_hilite = columns.get_index_at_point(cpt);
          if (geometrie.hilite != new_hilite) {
            geometrie.hilite = new_hilite;
            redraw_all();
            notify_event(detail::HILITE_CHANGE_MESSAGE, true);
          }
        }
      }
    }

    void table_view::handle_row_left_btn_down (os::key_state, const core::native_point& npt) {
      last_mouse_point = npt;
      moved = false;
      if (enable_v_size) {
        down_idx.y(geometrie.heights.split_idx_at(rows.surface_to_client(npt).y(), 2.0F));
      }
      rows.set_cursor(down_idx.y() > -1 ? win::cursor::size_v() : win::cursor::move());
      rows.capture_pointer();
    }

    void table_view::handle_row_left_btn_up (os::key_state keys, const core::native_point& npt) {
      if (enable_selection_ && !moved && (last_mouse_point != core::native_point::undefined)) {
        const auto new_selection = rows.get_index_at_point(rows.surface_to_client(npt));
        if (get_selection() != new_selection) {
          set_selection(new_selection, event_source::mouse);
        } else if (core::control_key_bit_mask::is_set(keys)) {
          clear_selection(event_source::mouse);
        }
      }
      last_mouse_point = core::native_point::undefined;
      down_idx.clear();
      rows.set_cursor(win::cursor::arrow());
      rows.uncapture_pointer();
    }

    void table_view::handle_row_mouse_move (os::key_state keys, const core::native_point& npt) {
      if (core::left_button_bit_mask::is_set(keys)) {
        if (last_mouse_point != core::native_point::undefined) {
          auto delta = core::global::scale_from_native<core::size::type>(npt.y() - last_mouse_point.y());
          if (down_idx.y() > -1) {
            geometrie.heights.set_size(down_idx.y(), geometrie.heights.get_size(down_idx.y()) + delta);
            redraw_all();
          } else {
            if (vscroll.is_enabled()) {
              vscroll.set_value(vscroll.get_value() - delta, true);
            }
          }
          moved = true;
        }
        last_mouse_point = npt;
      } else {
        core::point rpt = rows.surface_to_client(npt);
        const int idx = enable_v_size ? geometrie.heights.split_idx_at(rpt.y(), 2.0F) : -1;
        rows.set_cursor(idx > -1 ? win::cursor::size_v() : win::cursor::arrow());
        if (enable_hilite_ && (idx < 0)) {
          const auto new_hilite = rows.get_index_at_point(rpt);
          if (geometrie.hilite != new_hilite) {
            geometrie.hilite = new_hilite;
            redraw_all();
            notify_event(detail::HILITE_CHANGE_MESSAGE, true);
          }
        }
      }
    }

    void table_view::handle_key (os::key_state state,
                                 os::key_symbol key,
                                 const std::string&) {
      if ((state != core::state::none) && (state != core::state::alt)) {
        return;
      }
      switch (key) {
      case core::keys::up:
      case core::keys::numpad::up:
        if (enable_selection_) {
          set_selection(get_valid_selection(table::offset(0, -1)), event_source::keyboard);
        }
        break;
      case core::keys::down:
      case core::keys::numpad::down:
        if (enable_selection_) {
          set_selection(get_valid_selection(table::offset(0, 1)), event_source::keyboard);
        }
        break;
      case core::keys::left:
      case core::keys::numpad::left:
        if (enable_selection_) {
          set_selection(get_valid_selection(table::offset(-1, 0)), event_source::keyboard);
        }
        break;
      case core::keys::right:
      case core::keys::numpad::right:
        if (enable_selection_) {
          set_selection(get_valid_selection(table::offset(1, 0)), event_source::keyboard);
        }
        break;
      case core::keys::page_up:
      case core::keys::numpad::page_up: {
        core::size sz = data.client_size();
        if (enable_selection_) {
          const core::point pt = geometrie.position_of(geometrie.selection);
          if (core::alt_key_bit_mask::is_set(state)) {
            sz.height(0);
          } else {
            sz.width(0);
          }
          set_selection(geometrie.index_at(pt), event_source::logic);
        }
        set_scroll_pos(get_scroll_pos() - sz);
        break;
      }
      case core::keys::page_down:
      case core::keys::numpad::page_down: {
        core::size sz = data.client_size();
        if (enable_selection_) {
          const core::point pt = geometrie.position_of(geometrie.selection);
          if (core::alt_key_bit_mask::is_set(state)) {
            sz.height(0);
          } else {
            sz.width(0);
          }
          set_selection(geometrie.index_at(pt), event_source::logic);
        }
        set_scroll_pos(get_scroll_pos() + sz);
        break;
      }
      break;
      case core::keys::home:
      case core::keys::numpad::home:
        if (enable_selection_) {
          set_selection(table::position(0, 0), event_source::keyboard);
        } else {
          set_scroll_pos(core::point::zero);
        }
        break;
      case core::keys::f2:
      case core::keys::enter:
        notify_event(detail::SELECTION_COMMIT_MESSAGE);
        break;
      case core::keys::escape:
        clear_selection(event_source::keyboard);
        break;
      }
    }

    void table_view::on_selection_changed (selection_changed_event::function&& f) {
      on<selection_changed_event>(std::move(f));
    }

    void table_view::on_selection_commit (selection_commit_event::function&& f) {
      on<selection_commit_event>(std::move(f));
    }

    void table_view::on_selection_cancel (selection_cancel_event::function&& f) {
      on<selection_cancel_event>(std::move(f));
    }

    void table_view::on_hilite_changed (hilite_changed_event::function&& f) {
      on<hilite_changed_event>(std::move(f));
    }

    void table_view::on_content_changed (content_changed_event::function&& f) {
      on<content_changed_event>(std::move(f));
    }

    // --------------------------------------------------------------------------
    table_edit::table_edit (core::size::type default_width,
                            core::size::type default_height,
                            core::size::type row_width,
                            core::size::type column_height,
                            text_origin_t align,
                            os::color foreground,
                            os::color background,
                            os::color header_background)
      : table_view(default_width, default_height, row_width, column_height,
                   align, foreground, background, header_background)
      , enable_edit(true)
    {
      enable_hilite(true);
      enable_selection(true);
      on_selection_commit(util::bind_method(this, &table_edit::enter_edit));

      editor.on_btn_down([&](os::key_state, const core::native_point& pt) {
        if (!editor.surface_geometry().is_inside(pt)) {
          commit_edit();
        }
      });

      editor.on_selection_cancel(util::bind_method(this, &table_edit::cancel_edit));
      editor.on_selection_commit(util::bind_method(this, &table_edit::commit_edit));

      vscroll.on_scroll(util::bind_method(this, &table_edit::move_editor));
      hscroll.on_scroll(util::bind_method(this, &table_edit::move_editor));

      on_selection_changed([&] (event_source) {
        commit_edit();
      });

    }

    void table_edit::move_editor (core::point::type) {
      if (editor.is_visible()) {
        const auto pos = geometrie.position_of(get_selection());
        if (data.client_geometry().is_inside(pos)) {
          editor.position(pos);
        } else {
          editor.set_visible(false);
        }
      }
    }

    void table_edit::enter_edit () {
      if (enable_edit && data_source) {
        auto cell = get_selection();
        auto spwn = geometrie.spawns.get(cell);
        if (spwn.is_hidden()) {
          cell.move({spwn.x, spwn.y});
        }
        auto pt = geometrie.position_of(cell);
        auto sz = geometrie.get_size(cell);
        if (!editor.is_valid()) {
          editor.create(data, core::rectangle(0, 0, 10, 10));
        }
        editor.geometry(core::rectangle(pt, sz));
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

    // --------------------------------------------------------------------------

  } // ctrl

} // gui
