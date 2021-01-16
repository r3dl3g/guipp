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
 * @brief     C++ API: table
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
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/table.h>


namespace gui {

  namespace look {

    template<>
    void text_cell<std::string, draw::frame::no_frame>(const std::string& t,
                                                       const draw::graphics& graph,
                                                       const core::rectangle& place,
                                                       const text_origin_t& align,
                                                       const os::color& foreground,
                                                       const os::color& background,
                                                       const ctrl::item_state& state) {
      using namespace draw;
      graph.fill(draw::rectangle(place), get_background_color(state, background));
      graph.text(draw::text_box(t, place, align), font::system(), get_text_color(state, foreground));
    }

  } // namespace look

} // gui
