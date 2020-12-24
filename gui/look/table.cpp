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
#include <gui/look/table.h>


namespace gui {

  namespace paint {

    os::color get_back_color (const ctrl::item_state& state, const os::color& background) {
      switch (state) {
        case ctrl::item_state::selected:  return color::highLightColor();
        case ctrl::item_state::hilited:   return color::darker(background, 0.05F);
        default: return background;
      }
    }

    os::color get_fore_color (const ctrl::item_state& state, const os::color& foreground) {
      switch (state) {
        case ctrl::item_state::selected:  return color::highLightTextColor();
        case ctrl::item_state::hilited:   return color::darker(foreground, 0.25F);
        default: return foreground;
      }
    }

    template<>
    void text_cell<std::string, draw::frame::no_frame>(const std::string& t,
                                                       const draw::graphics& graph,
                                                       const core::rectangle& place,
                                                       const text_origin_t& align,
                                                       const os::color& foreground,
                                                       const os::color& background,
                                                       const ctrl::item_state& state) {
      using namespace draw;
      graph.fill(draw::rectangle(place), get_back_color(state, background));
      graph.text(draw::text_box(t, place, align), font::system(), get_fore_color(state, foreground));
    }

  } // namespace paint

} // gui
