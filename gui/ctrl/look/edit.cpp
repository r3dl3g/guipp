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
 * @brief     C++ API: edit control
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <gui/draw/drawers.h>
#include <gui/draw/graphics.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/ctrl/look/edit.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace look {

    void edit_line (const draw::graphics& graph,
                    const core::rectangle& area,
                    const std::string& text,
                    const draw::font& fnt,
                    os::color foreground,
                    os::color background,
                    const text_origin_t& origin,
                    const core::range<size_t>& selection,
                    const size_t cursor_pos,
                    const size_t scroll_pos,
                    const bool is_focused) {

      using namespace gui::draw;
      clip clp(graph, area);

      graph.fill(rectangle(area), background);

      core::point::type y1 = area.y();
      core::point::type y2 = area.y2();

      if (is_focused && (selection.begin() < selection.end())) {
        auto begin = scroll_pos;
        core::point::type left = area.x();
        if (selection.begin() > scroll_pos) {
          begin = selection.begin();
          std::string t1 = text.substr(scroll_pos, begin - scroll_pos);
          gui::core::rectangle a1 = area;
          graph.text(bounding_box(t1, a1, origin), fnt, foreground);
          left = a1.x2();
        }

        gui::core::rectangle a2 = area.with_x(left);
        if (selection.end() > scroll_pos) {
          std::string t2 = text.substr(begin, selection.end() - begin);
          graph.text(bounding_box(t2, a2, origin), fnt, foreground);
          graph.fill(rectangle(a2), color::invert(color::highLightColor()));
        }
        graph.text(text_box(text.substr(scroll_pos), area, origin), fnt, foreground);

        if (selection.end() > scroll_pos) {
          graph.invert(a2);
        }

      } else {
        graph.text(text_box(text.substr(scroll_pos), area, origin), fnt, foreground);
      }

      if (is_focused && (cursor_pos < std::numeric_limits<size_t>::max())) {
        gui::core::rectangle cursor_area = area;
        graph.text(bounding_box(text.substr(scroll_pos, cursor_pos - scroll_pos), cursor_area, origin),
                   fnt, foreground);
        graph.frame(line(core::point(cursor_area.x2(), y1),
                         core::point(cursor_area.x2(), y2)),
                    foreground);
      }
#ifdef SHOW_TEXT_AREA
      graph.text(bounding_box(text, area, origin), fnt, foreground);
      graph.frame(rectangle(area), pen(foreground, pen::dot));
#endif // SHOW_TEXT_AREA
    }

  } // look

} // gui
