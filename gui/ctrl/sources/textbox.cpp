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
* @brief     C++ API: textbox controls
*
* @file
*/

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "textbox.h"
#include "edit.h"


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      edit_base::pos_t get_line_cursor (const core::position<int>& cursor_pos, size_t idx) {
        return (cursor_pos.row == idx) ? cursor_pos.column : -1;
      }

      edit_base::range get_line_selection (const core::range<core::position<int>>& selection, size_t idx) {
        edit_base::range r;
        if (selection.first.row < idx) {
            r.first = 0;
          } else if (selection.first.row == idx) {
            r.first = selection.first.column;
          }
        if (selection.first.row <= idx) {
            if (selection.last.row > idx) {
                r.last = std::numeric_limits<edit_base::range::type>::max();
              } else if (selection.last.row == idx) {
                r.last = selection.last.column;
              }
          }
        return r;
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    namespace paint {

      void text_box (const draw::graphics& graph,
                     const core::rectangle& area,
                     const std::vector<std::string>& lines,
                     const draw::font& fnt,
                     os::color foreground,
                     os::color background,
                     const text_origin origin,
                     const core::range<core::position<int>>& selection,
                     const core::position<int>& cursor_pos,
                     const core::point& offset,
                     const bool has_focus) {
        const auto height = area.height();
        const auto row_sz = fnt.line_height();
        const auto last = lines.size();
        const auto first = static_cast<std::size_t>(offset.y() / row_sz);
        core::rectangle r(area.x() - offset.x(), row_sz * first - offset.y(), area.width(), row_sz);

        for(auto idx = first; (idx < last) && (r.y() < height); ++idx) {
          win::paint::edit_line(graph, r, lines[idx], fnt, foreground, background, origin,
                                detail::get_line_selection(selection, idx),
                                detail::get_line_cursor(cursor_pos, idx),
                                0, true);
          r.move_y(row_sz);
        }

        if (r.y() < area.y2()) {
          graph.fill(draw::rectangle(core::rectangle(r.top_left(), area.bottom_right())), background);
        }

        if (has_focus) {
          draw::frame::dots(graph, area);
        }
      }

    } // namespace paint

    namespace detail {

      // --------------------------------------------------------------------------
      const window_class textbox::clazz = no_erase_window_class(typeid(textbox).name());

      // --------------------------------------------------------------------------
      void textbox_base::set_text (const std::string& t) {
        data.lines = ibr::string::split<'\n'>(t);
        data.cursor_pos.clear();
        data.selection.clear();
        redraw_later();
      }

      std::string textbox_base::get_text () const {
        return ibr::string::merge<'\n'>(data.lines);
      }

      void textbox_base::set_scroll_pos (const core::point& pos) {
        data.offset = pos;
        redraw_later();
      }

      void textbox_base::set_selection (const textbox_base::range& s) {
        auto row = std::min<int>(s.last.row, row_count());
        if (row > -1) {
            auto column = std::min<int>(s.last.column, data.lines[row].size());
            data.selection = {s.first, {column, row}};
          } else {
            data.selection = s;
          }
        redraw_later();
      }

      void textbox_base::set_cursor_pos (const textbox_base::position& p, bool shift) {
        if (!data.lines.empty()) {
            auto row = std::min<int>(p.row, row_count());
            if ((row > -1) && (row < row_count())) {
                auto column = std::min<int>(p.column, data.lines[row].size());
                if (column > -1) {
                    position new_pos(column, row);
                    if (shift) {
                        if (data.cursor_pos == data.selection.last) {
                            data.selection.last = new_pos;
                          } else if (data.cursor_pos == data.selection.first) {
                            data.selection.first = new_pos;
                          } else {
                            data.selection = {new_pos};
                          }
                        data.selection.sort();
                      } else {
                        data.selection = {new_pos};
                      }
                    data.cursor_pos = new_pos;
                  } else {
                    data.cursor_pos = p;
                  }
              } else {
                data.cursor_pos = p;
              }

            redraw_later();
          }
      }

      textbox_base::position textbox_base::get_position_at_point (const core::point& pt) const {
        const auto row_sz = data.font.line_height();
        const auto row = static_cast<int>(pt.y() + data.offset.y()) / row_sz;
        if ((row > -1) && (row < row_count())) {
            const std::string text = data.lines[row];
            const auto max_chars = text.size();
            const auto x = pt.x() + data.offset.x();
            for (int i = 1; i < max_chars; ++i) {
                core::size sz = data.font.get_text_size(text.substr(0, i));
                if (sz.width() >= x) {
                    return {i - 1, row};
                  }
              }
            return {max_chars, row};
          }

        return {};
      }

      void textbox_base::replace_selection (const std::string& new_text) {
        auto v = ibr::string::split<'\n'>(new_text);
        if (data.lines.empty()) {
          data.lines = v;
          set_selection({{position::max, 0}});
        } else {
          auto sel = get_selection();
          if (sel.first.row == sel.last.row) {
            data.lines[sel.first.row].replace(sel.first.column, sel.last.column - sel.first.column, v.empty() ? std::string() : v.front());
          } else {
            std::string& new_line = data.lines[sel.first.row];
            new_line.replace(sel.first.column, new_line.size() - sel.first.column, v.empty() ? std::string() : v.front());
            new_line.append(data.lines[sel.last.row].substr(sel.last.column));
            erase_lines(sel.first.row + 1, sel.last.row);
          }
          if (v.empty()) {
            set_selection({{sel.first.column, sel.first.row}});
          } else if (data.lines.empty()) {
            data.lines = v;
            set_selection({{0, 0}});
          } else {
            data.lines.insert(std::next(data.lines.begin(), sel.first.row + 1), std::next(v.begin()), v.end());
            set_selection({{sel.first.column + v.back().size(), sel.first.row + v.size() - 1}});
          }
        }
        redraw_later();
      }

      std::string textbox_base::get_text_in_range (const textbox_base::range& r) const {
        if (r.empty() || data.lines.empty() || (r.first.row >= row_count())) {
            return std::string();
          }
        if (r.first.row == r.last.row) {
            return data.lines[r.first.row].substr(r.first.column, r.last.column - r.first.column);
          }
        std::ostringstream oss;
        const std::string& first = data.lines[r.first.row];
        oss << first.substr(r.first.column, first.size() - r.first.column) << "\n";
        std::copy(std::next(data.lines.begin(), r.first.row + 1), std::next(data.lines.begin(), r.last.row), std::ostream_iterator<std::string>(oss, "\n"));
        const std::string& last = data.lines[r.last.row - 1];
        oss << last.substr(0, r.last.column);
        return oss.str();
      }

      std::string textbox_base::get_selected_text () const {
        return get_text_in_range(get_selection());
      }

      void textbox_base::erase_line (int first) {
        erase_lines(first, first);
      }

      void textbox_base::erase_lines (int first, int last) {
        data.lines.erase(std::next(data.lines.begin(), first), std::next(data.lines.begin(), last + 1));
      }

    } // namespace detail

  } // win

} // gui
