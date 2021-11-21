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
 * @brief     C++ API: tree control
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
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/draw/icons.h>
#include <gui/io/pnm.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/tree.h>


namespace gui {

  namespace tree {

    icon_drawer* standard_icon_drawer (bool has_children, bool is_open, bool selected) {
      if (has_children) {
        if (is_open) { }
          return &draw::draw_icon<draw::icon_type::folder_open>;
      } else {
        return &draw::draw_icon<draw::icon_type::folder>;
      }
      return &draw::draw_icon<draw::icon_type::file>;
    }

  } // tree

  namespace look {

    void tree_button (draw::graphics& graph,
                      const core::rectangle& area,
                      bool is_open,
                      const draw::pen& pn) {
      const auto radius = area.max_radius()/ 2;
      if (is_open) {
        graph.frame(draw::icon_t<draw::icon_type::down>(area.center(), radius), pn);
      } else {
        graph.frame(draw::icon_t<draw::icon_type::right>(area.center(), radius), pn);
      }
    }

    void tree_node (draw::graphics& graph,
                    const core::rectangle& area,
                    const draw::brush& background,
                    std::size_t depth,
                    const std::string& label,
                    tree::icon_drawer* icon,
                    bool has_children,
                    bool is_open,
                    ctrl::item_state state) {
      if (!color::is_transparent(background.color())) {
        graph.fill(draw::rectangle(area), get_background_color(state, background.color()));
      }

      const auto radius = area.max_radius();
      core::rectangle r = area + core::point(core::point::type(depth * radius * 2), 0);
      const os::color col = get_text_color(state);

      draw::pen pn(col, 1, draw::pen::Style::solid, draw::pen::Cap::round, draw::pen::Join::round);

      if (has_children) {
        r.width(radius * 2);
        tree_button(graph, r, is_open, pn);
      }

      r += core::point(radius*2, 0);

      if (icon) {
        (*icon)(graph, pn, {r.x() + radius, r.center_y()}, radius);
        r += core::point(radius * 2 + 5, 0);
      }

      r.x2(area.x2());
      graph.text(draw::text_box(label, r, text_origin_t::vcenter_left),
                 draw::font::system(), col);
    }

  } // look

} // gui
