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
 * @brief     C++ API: label
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/brush.h>
#include <gui/look/slider.h>


namespace gui {

  namespace paint {

    void slider (const draw::graphics& graph,
                 const core::rectangle& area,
                 bool focused,
                 draw::frame::drawer frame) {
      core::rectangle place = area;
      graph.fill(draw::rectangle(place), color::buttonColor());
      frame(graph, place);
      if (focused) {
        draw::frame::dots(graph, place);
      }
    }

  } //paint

} // gui
