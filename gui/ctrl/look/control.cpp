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
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/item_state.h>
#include <gui/ctrl/look/control.h>


namespace gui {

  namespace look {

    os::color get_background_color (ctrl::item_state state, os::color def_color) {
      using i = ctrl::item_state;
      switch (state) {
        case i::normal:
        case i::disabled:
        case i::disabled_hilited:
          return def_color;
        case i::selected:
        case i::hilited_selected:
          return color::highLightColor();
        case i::disabled_selected:
        case i::disabled_hilited_selected:
          return color::remove_color(color::highLightColor());
        case i::hilited:
          return color::darker(def_color, 0.05F);
      }
      return def_color;
    }

    os::color get_text_color (ctrl::item_state state, os::color def_color) {
      using i = ctrl::item_state;
      switch (state) {
        case i::normal:
          return def_color;
        case i::disabled:
        case i::disabled_hilited:
          return color::disabledTextColor();
        case i::selected:
        case i::hilited_selected:
          return color::highLightTextColor();
        case i::disabled_selected:
        case i::disabled_hilited_selected:
          return color::remove_color(color::highLightTextColor());
        case i::hilited:
          return color::darker(def_color, 0.25F);
      }
      return def_color;
    }

    // --------------------------------------------------------------------------
    void text_item (const draw::graphics& g,
                    const core::rectangle& place,
                    const draw::brush& background,
                    const std::string& text,
                    const ctrl::item_state& state,
                    const text_origin_t& origin) {
      using namespace draw;
      g.fill(rectangle(place), get_background_color(state, background.color()));
      g.text(text_box(text, place, origin), font::system(), get_text_color(state));
    }

  } // namespace look

} // gui
