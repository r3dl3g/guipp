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
 * @brief     C++ API: label
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/look/progress_bar.h>
#include <gui/draw/drawers.h>
#include <gui/draw/graphics.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace look {

    void progress_bar (draw::graphics& graph,
                       const core::rectangle& area,
                       const std::string& txt,
                       os::color foreground,
                       os::color background,
                       const text_origin_t& origin,
                       os::color bar_color,
                       core::size::type bar_pos) {
      using namespace gui::draw;
      const core::rectangle left = area.with_width(bar_pos);
      const core::rectangle right = area.with_horizontal(left.x2(), area.x2() - left.x2());
      graph.fill(draw::rectangle(left), bar_color);
      graph.fill(draw::rectangle(right), background);
      if (!txt.empty()) {
        graph.text(draw::text_box(txt, area, origin), font::system(), foreground);
      }
#ifdef SHOW_TEXT_AREA
      graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
      graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
    }

  } // namespace look

} // namespace gui
