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
 * @brief     C++ API: menu
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/draw/bitmap.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/menu.h>


namespace gui {

  namespace look {

    void draw_menu_label (const draw::graphics& g,
                          const core::rectangle& r,
                          const std::string& l,
                          char k,
                          os::color c) {
      const draw::font& f = draw::font::menu();
      g.text(draw::text_box(l, r, text_origin_t::vcenter_left), f, c);

      if (k) {
        auto i = l.find(k);
        if (i != std::string::npos) {
          core::rectangle r1 = r;
          g.text(draw::bounding_box(l.substr(0, i + 1), r1, text_origin_t::vcenter_left), f, c);
          core::point::type x1 = r.x() + 1;
          core::point::type x2 = r1.x2() - 1;
          core::point::type y = r1.y2();
          if (i > 0) {
            core::rectangle r0 = r;
            g.text(draw::bounding_box(l.substr(0, i), r0, text_origin_t::vcenter_left), f, c);
            x1 = r0.x2();
          }
          g.draw_lines({core::point(x1, y), core::point(x2, y)}, c);
        }
      }
    }

    os::color get_background_menu_color (ctrl::item_state state, os::color def_color) {
      if (state.is_selected()) {
        return color::highLightColor();
      }
      if (state.is_hilited()) {
        return color::menuColorHighlight();
      }
      return def_color;
    }

    // --------------------------------------------------------------------------
    void main_menu_item (const draw::graphics& g,
                         const core::rectangle& r,
                         const draw::brush& background,
                         const std::string& label,
                         char menu_key,
                         ctrl::item_state state) {
      g.fill(draw::rectangle(r), get_background_menu_color(state, background.color()));
      draw_menu_label(g, r + core::point(10, 0), label, menu_key, get_text_color(state));
    }

    // --------------------------------------------------------------------------
    void menu_item (const draw::graphics& g,
                    const core::rectangle& r,
                    const draw::brush& background,
                    core::point::type text_pos,
                    core::point::type hotkey_pos,
                    const std::string& label,
                    char menu_key,
                    const draw::masked_bitmap& icon,
                    const core::hot_key& hotkey,
                    bool is_sub_menu,
                    bool separator,
                    ctrl::item_state state) {
      if (state.is_selected()) {
        if (separator) {
          g.fill(draw::rectangle(r.with_height(2)), background);
          g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::highLightColor());
        } else {
          g.fill(draw::rectangle(r), color::highLightColor());
        }
      } else if (state.is_hilited()) {
        if (separator) {
          g.fill(draw::rectangle(r.with_height(2)), background);
          g.fill(draw::rectangle(r.with_y(r.y() + 2)), color::menuColorHighlight());
        } else {
          g.fill(draw::rectangle(r), color::menuColorHighlight());
        }
      } else {
        g.fill(draw::rectangle(r), background);
      }

      core::rectangle r2 = r + core::point(text_pos, 0);

      if (separator) {
        core::rectangle r3(core::point(text_pos - 2, r.y()), core::point(r.x2(), r.y() + 2));
        draw::frame::hgroove(g, r3);
        r2 += core::point(0, 2);
      }

      os::color col = get_text_color(state);

      if (icon) {
        auto sz = icon.image.scaled_size();
        core::point::type x = (text_pos - sz.width()) / 2;
        core::point::type y = r.y() + (r.height() - sz.height()) / 2;
        g.copy_from(icon, core::point(x, y));
      }

      draw_menu_label(g, r2, label, menu_key, col);

      if (!hotkey.empty()) {
        r2.x(hotkey_pos);
        g.text(draw::text_box(hotkey.get_key_string(), r2, text_origin_t::vcenter_left),
               draw::font::menu(), col);
      }
      if (is_sub_menu) {
        core::point::type y = r.center_y();
        core::point::type x = r.x2() - 8;
        g.fill(draw::polygon({core::point(x, y - 4),
                              core::point(x, y + 4),
                              core::point(x + 4, y)}), color::black);
      }
    }

  } // namespace look

} // namespace gui
