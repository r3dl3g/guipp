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
 * @brief     C++ API: drop down list
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/color.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/look/drop_down.h>
#include <gui/ctrl/look/button.h>


namespace gui {

  namespace look {

    void drop_down_item (const draw::graphics& g,
                         const core::rectangle& r,
                         const draw::brush& background,
                         const std::string& label,
                         const ctrl::item_state& state) {
      if (ctrl::item_state::selected == state) {
        g.fill(draw::rectangle(r), color::highLightColor());
      } else if (ctrl::item_state::hilited == state) {
        g.fill(draw::rectangle(r), color::menuColorHighlight());
      } else {
        g.fill(draw::rectangle(r), background);
      }

      os::color col = ctrl::item_state::selected == state ? color::highLightTextColor()
                                                    : color::black;

      g.text(draw::text_box(label, r, text_origin_t::vcenter_left), draw::font::system(), col);

    }

    void drop_down_button (const draw::graphics& graph,
                           const core::rectangle& area,
                           const core::button_state::is& state,
                           bool is_open) {
#ifdef BUILD_FOR_ARM
      look::button_frame_w95(graph, area, state);
#else
      look::button_frame(graph, area, state);
#endif
      core::rectangle r = area.shrinked(core::size(4, 5));
      if (!r.empty()) {
        std::vector<core::point> p;
        if (is_open) {
          p = {r.bottom_right(), {r.center_x(), r.y()}, r.bottom_left()};
        } else {
          p = {r.top_left(), {r.center_x(), r.y2()}, r.top_right()};
        }
        graph.fill(draw::polygon(p), color::black);
      }
    }

  } // look

} // gui
