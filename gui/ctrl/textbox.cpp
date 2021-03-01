/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <util/string_util.h>
#include <gui/ctrl/textbox.h>
#include <gui/win/clipboard.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      void textbox_base::set_text (const std::string& t) {
        data.lines = util::string::split<'\n'>(t);
        if (data.lines.empty()) {
          data.lines.emplace_back(std::string());
        }
        data.cursor_pos.clear();
        data.selection.clear();
        notify_content_changed();
        notify_selection_changed(event_source::logic);
        invalidate();
      }

      std::string textbox_base::get_text () const {
        return util::string::merge<'\n'>(data.lines);
      }

      void textbox_base::set_scroll_pos (const core::point& pos) {
        if (data.offset != pos) {
          data.offset = pos;
          invalidate();
        }
      }

      void textbox_base::set_selection (const textbox_base::range& s) {
        auto old_selection = data.selection;
        auto row = std::min<int>(s.end().y(), static_cast<int>(row_count()) - 1);
        if (row > -1) {
          auto column = std::min<int>(s.end().x(), static_cast<int>(data.lines[row].size()));
          data.selection = {s.begin(), {column, row}};
        } else {
          data.selection = s;
        }
        if (old_selection != data.selection) {
          invalidate();
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
                if (data.cursor_pos == data.selection.end()) {
                  data.selection = {data.selection.begin(), new_pos};
                } else if (data.cursor_pos == data.selection.begin()) {
                  data.selection = {new_pos, data.selection.end()};
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
          notify_selection_changed(event_source::mouse);
          invalidate();
        }
      }

      textbox_base::position textbox_base::get_position_at_point (const core::point& pt, const text_origin_t origin) const {
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

        const int row = static_cast<int>((pt.y() + data.offset.y() - y) / row_sz);
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
        auto v = util::string::split<'\n'>(new_text);
        if (data.lines.empty()) {
          data.lines = v;
          set_cursor_pos(position::end);
        } else {
          auto sel = get_selection();
          auto first_line = std::next(data.lines.begin(), sel.begin().y());
          if ((sel.begin().y() == sel.end().y()) && (v.size() < 2)) {
            first_line->replace(sel.begin().x(), sel.end().x() - sel.begin().x(), new_text);
            set_cursor_pos({sel.begin().x() + static_cast<decltype(sel.begin().x())>(new_text.size()), sel.begin().y()});
          } else {
            auto last_line = std::next(data.lines.begin(), sel.end().y());
            std::string head = first_line->substr(0, sel.begin().x());
            std::string tail = last_line->substr(sel.end().x());
            data.lines.erase(std::next(first_line), std::next(last_line));
            if (v.size() < 2) {
              *first_line = head + new_text + tail;
              set_cursor_pos({sel.begin().x() + static_cast<decltype(sel.begin().x())>(new_text.size()), sel.begin().y()});
            } else {
              *first_line = head + v.front();
              std::string last = v.back() + tail;
              data.lines.insert(std::next(first_line), last);
              data.lines.insert(std::next(first_line), std::next(v.begin()), std::prev(v.end()));
              set_cursor_pos({static_cast<decltype(sel.begin().x())>(v.back().size()), sel.begin().y() + static_cast<decltype(sel.begin().y())>(v.size()) - 1});
            }
          }
        }
        if (data.lines.empty()) {
          data.lines.emplace_back(std::string());
          set_cursor_pos(position::zero);
        }
        notify_content_changed();
        invalidate();
      }

      std::string textbox_base::get_text_in_range (const textbox_base::range& r) const {
        if (r.empty() || data.lines.empty() || (r.begin().y() >= row_count())) {
          return std::string();
        }
        if (r.begin().y() == r.end().y()) {
          return data.lines[r.begin().y()].substr(r.begin().x(), r.end().x() - r.begin().x());
        }
        std::ostringstream oss;
        const std::string& first = data.lines[r.begin().y()];
        oss << first.substr(r.begin().x(), first.size() - r.begin().x()) << "\n";
        std::copy(std::next(data.lines.begin(), r.begin().y() + 1), std::next(data.lines.begin(), r.end().y()), std::ostream_iterator<std::string>(oss, "\n"));
        const std::string& last = data.lines[r.end().y()];
        oss << last.substr(0, r.end().x());
        return oss.str();
      }

      std::string textbox_base::get_selected_text () const {
        return get_text_in_range(get_selection());
      }

      core::rectangle textbox_base::get_virtual_geometry (const core::rectangle&) const {
        if (data.virtual_size.empty()) {
          const auto row_sz = data.font.line_height();
          const auto row_cnt = row_count();
          core::size::type w = 0;
          for (const auto& text : data.lines) {
            w = std::max(w, data.font.get_text_size(text).width());
          }
          data.virtual_size = {w, static_cast<core::size::type>(row_sz * row_cnt)};
        }
        return core::rectangle(data.virtual_size);
      }

      core::size textbox_base::get_scroll_steps () const {
        return {data.font.get_text_size("O").width(), data.font.line_height()};
      }

      void textbox_base::make_selection_visible () {
        if (data.cursor_pos >= position::zero) {
          const auto old_pos = data.offset;
          core::rectangle area(data.offset, client_size());
          // first check row
          const auto row_sz = data.font.line_height();
          const auto y = static_cast<core::point::type>(data.cursor_pos.y() * row_sz);
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
            invalidate();
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

      void textbox_base::notify_content_changed () {
        data.virtual_size.clear();
        super::notify_content_changed();
      }

      textbox_base::position textbox_base::find_prev_word (const textbox_base::position& pos) {
        if (pos.is_valid() && (pos.x() > 0)) {
          std::string::size_type p = util::string::find_left_space(data.lines[pos.y()], pos.x());
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
        if (pos.is_valid() && (pos.y() < data.lines.size()) && (pos.x() < data.lines[pos.y()].size())) {
          std::string::size_type p = util::string::find_right_space(data.lines[pos.y()], pos.x());
          if (p != std::string::npos) {
            return {static_cast<decltype(pos.y())>(p), pos.y()};
          }
        }
        if (pos.y() < (row_count() - 1)) {
          return {0, pos.y() + 1};
        }
        return pos;
      }

      void textbox_base::enable_select_by_mouse (const text_origin_t origin) {
        on_left_btn_down([&, origin](os::key_state, const core::native_point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          set_cursor_pos(get_position_at_point(surface_to_client(pt), origin));
        });
        on_left_btn_up([&](os::key_state, const core::native_point&) {
          data.last_mouse_point = core::native_point::undefined;
        });
        on_left_btn_dblclk([&, origin](os::key_state, const core::native_point& pt) {
          take_focus();
          data.last_mouse_point = pt;
          const auto p = get_position_at_point(surface_to_client(pt), origin);
          const auto l = find_prev_word(p);
          const auto r = find_next_word(p);
          set_cursor_pos(p);
          set_selection({l, r});
        });
        on_mouse_move([&, origin](os::key_state keys, const core::native_point& pt) {
          if ((data.last_mouse_point != core::native_point::undefined) && core::left_button_bit_mask::is_set(keys)) {
            set_cursor_pos(get_position_at_point(surface_to_client(pt), origin), true);
          }
        });
        on_key_down<core::keys::c, core::state::control>([&]() {
          win::clipboard::get().set_text(*this, get_selected_text());
        });
        on_set_focus([&] () {
          invalidate();
        });
        on_lost_focus([&] () {
          invalidate();
        });
      }

    } // namespace detail

  } // ctrl

} // gui
