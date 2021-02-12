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
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui/draw/graphics.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/look/textbox.h>
#include <gui/ctrl/look/edit.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace look {

    namespace detail {

      // --------------------------------------------------------------------------
      size_t get_line_cursor (const core::basic_point<int>& cursor_pos, size_t idx) {
        return (cursor_pos.y() == idx) ? cursor_pos.x() : -1;
      }

      core::range<size_t> get_line_selection (const core::range<core::basic_point<int> >& selection, size_t idx) {
        core::range<size_t>::type first = 0;
        if (selection.begin().y() < idx) {
          first = 0;
        } else if (selection.begin().y() == idx) {
          first = selection.begin().x();
        }
        core::range<size_t>::type last = 0;
        if (selection.begin().y() <= idx) {
          if (selection.end().y() > idx) {
            last = std::numeric_limits<core::range<size_t>::type>::max();
          } else if (selection.end().y() == idx) {
            last = selection.end().x();
          }
        }
        return {first, last};
      }

    } // namespace detail

    void text_box (draw::graphics& graph,
                   const core::rectangle& area,
                   const std::vector<std::string>& lines,
                   const draw::font& fnt,
                   os::color foreground,
                   os::color background,
                   const text_origin_t& origin,
                   const core::range<core::basic_point<int> >& selection,
                   const core::basic_point<int>& cursor_pos,
                   const core::point& offset,
                   const bool is_focused,
                   const bool is_enabled) {
      draw::clip clp(graph, area);

      const auto height = area.y2();
      const auto row_sz = static_cast<core::size::type>(fnt.line_height());
      const auto last = lines.size();
      const auto first = static_cast<int>(offset.y() / row_sz);
      core::rectangle r(area.x() - offset.x(), area.y() + row_sz * first - offset.y(), area.width() + offset.x(), row_sz);
      const auto needed_height = last * row_sz;
      const auto diff = area.height() - needed_height;
      if (diff > 0) {
        if (origin_is_v_center(origin)) {
          r.move_y(diff / 2);
        } else if (origin_is_bottom(origin)) {
          r.move_y(diff);
        }
      }

      if (r.y() > area.y()) {
        graph.fill(draw::rectangle(core::rectangle(area.top_left(), r.top_right())), background);
      }

      for (auto idx = first; (idx < last) && (r.y() < height); ++idx) {
        look::edit_line(graph, r, lines[idx], fnt, foreground, background, origin,
                        detail::get_line_selection(selection, idx),
                        detail::get_line_cursor(cursor_pos, idx),
                        0, is_focused, is_enabled);
        r.move_y(row_sz);
      }

      if (r.y() < area.y2()) {
        graph.fill(draw::rectangle(core::rectangle(r.top_left(), area.x2y2())), background);
      }

      if (is_focused) {
        draw::frame::dots(graph, area);
      }
    }

  } // namespace look

} // gui
