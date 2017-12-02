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
// Library includes
//
#include <gui/core/string_util.h>
#include <gui/ctrl/textbox.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/clipboard.h>


namespace gui {

  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      edit_base::pos_t get_line_cursor (const core::basic_point<int>& cursor_pos, size_t idx) {
        return (cursor_pos.y() == idx) ? cursor_pos.x() : -1;
      }

      edit_base::range get_line_selection (const core::range<core::basic_point<int> >& selection, size_t idx) {
        edit_base::range r;
        if (selection.first.y() < idx) {
          r.first = 0;
        } else if (selection.first.y() == idx) {
          r.first = selection.first.x();
        }
        if (selection.first.y() <= idx) {
          if (selection.last.y() > idx) {
            r.last = std::numeric_limits<edit_base::range::type>::max();
          } else if (selection.last.y() == idx) {
            r.last = selection.last.x();
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
                     const core::range<core::basic_point<int> >& selection,
                     const core::basic_point<int>& cursor_pos,
                     const core::point& offset,
                     const bool has_focus) {
        const auto height = area.height();
        const core::size::type row_sz = static_cast<core::size::type>(fnt.line_height());
        const auto last = lines.size();
        const auto first = static_cast<int>(offset.y() / row_sz);
        core::rectangle r(area.x() - offset.x(), row_sz * first - offset.y(), area.width() + offset.x(), row_sz);
        const auto needed_height = last * row_sz;
        const auto diff = area.height() - needed_height;
        if (diff > 0) {
          if (origin_is_v_center(origin)) {
            r.y(diff / 2);
          } else if (origin_is_bottom(origin)) {
            r.y(diff);
          }
        }

        if (r.y() > area.y()) {
          graph.fill(draw::rectangle(core::rectangle(area.top_left(), r.top_right())), background);
        }

        for (auto idx = first; (idx < last) && (r.y() < height); ++idx) {
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
      void textbox_base::set_text (const std::string& t) {
        data.lines = string::split<'\n'>(t);
        if (data.lines.empty()) {
          data.lines.emplace_back(std::string());
        }
        data.cursor_pos.clear();
        data.selection.clear();
        notify_content_changed();
        notify_selection_changed();
        redraw();
      }

      std::string textbox_base::get_text () const {
        return string::merge<'\n'>(data.lines);
      }

      void textbox_base::set_scroll_pos (const core::point& pos) {
        if (data.offset != pos) {
          data.offset = pos;
          redraw();
        }
      }

      void textbox_base::set_selection (const textbox_base::range& s) {
        auto old_selection = data.selection;
        auto row = std::min<int>(s.last.y(), static_cast<int>(row_count()) - 1);
        if (row > -1) {
          auto column = std::min<int>(s.last.x(), static_cast<int>(data.lines[row].size()));
          data.selection = {s.first, {column, row}};
        } else {
          data.selection = s;
        }
        if (old_selection != data.selection) {
          redraw();
        }
      }

      void textbox_base::set_cursor_pos (const textbox_base::position& p, bool shift) {
        textbox_base::position old_pos = data.cursor_pos;
        if (!data.lines.empty()) {
          auto row = std::min<int>(p.y(), static_cast<int>(row_count()) - 1);
          if (row > -1) {
            auto column = std::min<int>(p.x(), static_cast<int>(data.lines[row].size()));
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
        } else {
          data.cursor_pos = p;
        }
        if (old_pos != data.cursor_pos) {
          notify_selection_changed();
          redraw();
        }
      }

      textbox_base::position textbox_base::get_position_at_point (const core::point& pt, const text_origin origin) const {
        const auto row_sz = data.font.line_height();
        const auto last = row_count();
        const auto diff = client_size().height() - (last * row_sz);
        core::point::type y = 0;
        if (diff > 0) {
          if (origin_is_v_center(origin)) {
            y = diff / 2;
          } else if (origin_is_bottom(origin)) {
            y = diff;
          }
        }

        const auto row = static_cast<int>(pt.y() + data.offset.y() - y) / row_sz;
        if ((row > -1) && (row < last)) {
          const std::string& text = data.lines[row];
          const int max_chars = static_cast<int>(text.size());
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
        auto v = string::split<'\n'>(new_text);
        if (data.lines.empty()) {
          data.lines = v;
          set_cursor_pos(position::end);
        } else {
          auto sel = get_selection();
          auto first_line = std::next(data.lines.begin(), sel.first.y());
          if ((sel.first.y() == sel.last.y()) && (v.size() < 2)) {
            first_line->replace(sel.first.x(), sel.last.x() - sel.first.x(), new_text);
            set_cursor_pos({sel.first.x() + static_cast<decltype(sel.first.x())>(new_text.size()), sel.first.y()});
          } else {
            auto last_line = std::next(data.lines.begin(), sel.last.y());
            std::string head = first_line->substr(0, sel.first.x());
            std::string tail = last_line->substr(sel.last.x());
            data.lines.erase(std::next(first_line), std::next(last_line));
            if (v.size() < 2) {
              *first_line = head + new_text + tail;
              set_cursor_pos({sel.first.x() + static_cast<decltype(sel.first.x())>(new_text.size()), sel.first.y()});
            } else {
              *first_line = head + v.front();
              std::string last = v.back() + tail;
              data.lines.insert(std::next(first_line), last);
              data.lines.insert(std::next(first_line), std::next(v.begin()), std::prev(v.end()));
              set_cursor_pos({static_cast<decltype(sel.first.x())>(v.back().size()), sel.first.y() + static_cast<decltype(sel.first.y())>(v.size()) - 1});
            }
          }
        }
        if (data.lines.empty()) {
          data.lines.emplace_back(std::string());
          set_cursor_pos(position::zero);
        }
        notify_content_changed();
        redraw();
      }

      std::string textbox_base::get_text_in_range (const textbox_base::range& r) const {
        if (r.empty() || data.lines.empty() || (r.first.y() >= row_count())) {
          return std::string();
        }
        if (r.first.y() == r.last.y()) {
          return data.lines[r.first.y()].substr(r.first.x(), r.last.x() - r.first.x());
        }
        std::ostringstream oss;
        const std::string& first = data.lines[r.first.y()];
        oss << first.substr(r.first.x(), first.size() - r.first.x()) << "\n";
        std::copy(std::next(data.lines.begin(), r.first.y() + 1), std::next(data.lines.begin(), r.last.y()), std::ostream_iterator<std::string>(oss, "\n"));
        const std::string& last = data.lines[r.last.y()];
        oss << last.substr(0, r.last.x());
        return oss.str();
      }

      std::string textbox_base::get_selected_text () const {
        return get_text_in_range(get_selection());
      }

      core::rectangle textbox_base::get_virtual_place () const {
        if (data.virtual_size.empty()) {
          const auto row_sz = data.font.line_height();
          const auto row_cnt = row_count();
          core::size::type w = 0;
          for (auto text : data.lines) {
            w = std::max(w, data.font.get_text_size(text).width());
          }
          data.virtual_size = {w, static_cast<core::size::type>(row_sz * row_cnt)};
        }
        return core::rectangle(-data.offset, data.virtual_size);
      }

      void textbox_base::make_selection_visible () {
        if (data.cursor_pos >= position::zero) {
          const auto old_pos = data.offset;
          core::rectangle area(data.offset, client_size());
          // first check row
          const auto row_sz = data.font.line_height();
          const core::point::type y = static_cast<core::point::type>(data.cursor_pos.y() * row_sz);
          if (y < area.y()) {
            data.offset.y(y);
          } else if (y + row_sz > area.y2()) {
            data.offset.y(y + row_sz - area.height());
          }
          // check column
          const auto& text = data.lines[data.cursor_pos.y()];
          const auto sub = text.substr(0, data.cursor_pos.x());
          const core::point::type x = data.font.get_text_size(sub).width();
          if (x < area.x()) {
            data.offset.x(x);
          } else if (x + 3 > area.x2()) {
            data.offset.x(x + 3 - area.width());
          }
          if (data.offset != old_pos) {
            redraw();
          }
        }
      }

      void textbox_base::erase_line (int first) {
        erase_lines(first, first);
      }

      void textbox_base::erase_lines (int first, int last) {
        data.lines.erase(std::next(data.lines.begin(), first), std::next(data.lines.begin(), last + 1));
        if (data.lines.empty()) {
          data.lines.emplace_back(std::string());
        }
        notify_content_changed();
      }

      void textbox_base::notify_content_changed () const {
        data.virtual_size.clear();
        send_client_message(this, detail::CONTENT_CHANGED_MESSAGE);
      }

      void textbox_base::notify_selection_changed () const {
        send_client_message(this, detail::SELECTION_CHANGE_MESSAGE);
      }

      textbox_base::position textbox_base::find_prev_word (const textbox_base::position& pos) {
        if (pos.is_valid() && (pos.x() > 0)) {
          std::string::size_type p = string::find_left_space(data.lines[pos.y()], pos.x());
          if (p != std::string::npos) {
            return {static_cast<decltype(pos.y())>(p), pos.y()};
          }
        }
        if (pos.y() > 0) {
          return {static_cast<decltype(pos.x())>(data.lines[pos.y() - 1].size()), pos.y() - 1};
        }
        return pos;
      }

      textbox_base::position textbox_base::find_next_word (const textbox_base::position& pos) {
        if (pos.is_valid() && (pos.x() < data.lines[pos.y()].size())) {
          std::string::size_type p = string::find_right_space(data.lines[pos.y()], pos.x());
          if (p != std::string::npos) {
            return {static_cast<decltype(pos.y())>(p), pos.y()};
          }
        }
        if (pos.y() < (row_count() - 1)) {
          return {0, pos.y() + 1};
        }
        return pos;
      }

      void textbox_base::enable_select_by_mouse (const text_origin origin) {
        register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&, origin](os::key_state, const core::point & pt) {
          take_focus();
          data.last_mouse_point = pt;
          set_cursor_pos(get_position_at_point(pt, origin));
        }));
        register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point & pt) {
          data.last_mouse_point = core::point::undefined;
        }));
        register_event_handler(REGISTER_FUNCTION, left_btn_dblclk_event([&, origin](os::key_state, const core::point & pt) {
          take_focus();
          data.last_mouse_point = pt;
          const auto p = get_position_at_point(pt, origin);
          const auto l = find_prev_word(p);
          const auto r = find_next_word(p);
          set_cursor_pos(p);
          set_selection({l, r});
        }));
        register_event_handler(REGISTER_FUNCTION, mouse_move_event([&, origin](os::key_state keys, const core::point & pt) {
          if ((data.last_mouse_point != core::point::undefined) && left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_position_at_point(pt, origin), true);
          }
        }));
        register_event_handler(REGISTER_FUNCTION, key_down_event<keys::c, state::control>([&]() {
          clipboard::get().set_text(*this, get_selected_text());
        }));
        register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window*){
          redraw();
        }));
        register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*){
          redraw();
        }));
      }

    } // namespace detail

  } // win

} // gui
