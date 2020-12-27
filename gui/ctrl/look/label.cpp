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
#include <gui/draw/graphics.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/drawers.h>
#include <gui/ctrl/look/label.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace look {

    void label (const draw::graphics& graph,
                const core::rectangle& area,
                const std::string& text,
                os::color foreground,
                os::color background,
                const text_origin_t& origin) {
      using namespace gui::draw;
      graph.fill(draw::rectangle(area), background);
      graph.text(draw::text_box(text, area, origin),
                 font::system(),
                 foreground);
#ifdef SHOW_TEXT_AREA
      graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
      graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
    }

  } //look

} // gui
