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
 * @brief     C++ API: basic controls
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/item_state.h>


namespace gui {

  namespace paint {

    // --------------------------------------------------------------------------
    void text_item (const draw::graphics& g,
                    const core::rectangle& place,
                    const draw::brush& background,
                    const std::string& text,
                    const ctrl::item_state& state,
                    text_origin_t origin) {
      using namespace draw;
      g.fill(rectangle(place), ctrl::item_state::selected == state ? color::highLightColor() : background);
      os::color col = color::windowTextColor();
      switch (state) {
        case ctrl::item_state::selected: col = color::highLightTextColor(); break;
        case ctrl::item_state::disabled: col = color::disabledTextColor(); break;
      }
      g.text(text_box(text, place, origin), font::system(), col);
    }

  } // namespace paint

} // gui
