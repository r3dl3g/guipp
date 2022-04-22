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
 * @brief     edit look
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    void edit_line (draw::graphics& graph,
                    const core::rectangle& area,
                    const std::string& text,
                    const draw::font& fnt,
                    os::color foreground_,
                    os::color background,
                    const text_origin_t& origin,
                    const core::range<size_t>& selection,
                    const size_t cursor_pos,
                    const size_t scroll_pos,
                    const bool is_focused,
                    const bool is_enabled) {

      using namespace gui::draw;
//      clip clp(graph, area);

      graph.fill(rectangle(area), background);

      os::color foreground = is_enabled ? foreground_ : color::disabledTextColor();

      if (is_focused && (selection.begin() < selection.end())) {
        gui::core::rectangle selection_area = area;
        auto begin = scroll_pos;
        if (selection.begin() > scroll_pos) {
          begin = selection.begin();
          std::string t1 = text.substr(scroll_pos, begin - scroll_pos);
          graph.text(bounding_box(t1, selection_area, origin), fnt, foreground);
          selection_area.x(selection_area.x2());
        }

        if (selection.end() > scroll_pos) {
          std::string t2 = text.substr(begin, selection.end() - begin);
          graph.text(bounding_box(t2, selection_area, origin), fnt, foreground);
          selection_area.y(area.y()).height(area.height());
          graph.fill(rectangle(selection_area), color::invert(is_enabled && is_focused ? color::highLightColor()
                                                                         : color::remove_color(color::highLightColor())));
        }

        graph.text(text_box(text.substr(scroll_pos), area, origin), fnt, is_enabled ? foreground : color::black);

        if (selection.end() > scroll_pos) {
          graph.invert(selection_area);
        }

      } else {
        graph.text(text_box(text.substr(scroll_pos), area, origin), fnt, foreground);
      }

      if (is_focused && (cursor_pos < std::numeric_limits<size_t>::max())) {
        gui::core::rectangle cursor_area = area;
        graph.text(bounding_box(text.substr(scroll_pos, cursor_pos - scroll_pos), cursor_area, origin),
                   fnt, foreground);
        graph.frame(line(core::point(cursor_area.x2(), area.y()),
                         core::point(cursor_area.x2(), area.y2())),
                    color::black);
      }
#ifdef SHOW_TEXT_AREA
      graph.text(bounding_box(text, area, origin), fnt, foreground);
      graph.frame(rectangle(area), pen(foreground, pen::dot));
#endif // SHOW_TEXT_AREA
    }

  } // look

} // gui
