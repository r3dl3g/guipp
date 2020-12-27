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
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui/draw/graphics.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/ctrl/look/scroll_bar.h>
#include <gui/ctrl/look/button.h>


namespace gui {

  namespace look {

    namespace {
      std::string up_left_arrows[] = {
        "\xe2\x96\xb4", // up
        "\xe2\x97\x82", // left
      };
      std::string down_right_arrows[] = {
        "\xe2\x96\xbe",  // down
        "\xe2\x96\xb8", // right
      };
    }

    // --------------------------------------------------------------------------
    void scrollbar (const draw::graphics &g,
                    ctrl::scrollbar_item select,
                    ctrl::scrollbar_item hilite,
                    bool is_enabled,
                    bool horizontal,
                    bool is_focused,
                    const core::rectangle& up,
                    const core::rectangle& down,
                    const core::rectangle& thumb,
                    const core::rectangle& page_up,
                    const core::rectangle& page_down) {
      if (!page_up.empty()) {
        g.fill(draw::rectangle(page_up),
               select == ctrl::scrollbar_item::page_up ? color::light_gray
                                                 : color::very_light_gray);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one),
               select == ctrl::scrollbar_item::page_down ? color::light_gray
                                                   : color::very_light_gray);
      }
      os::color col = is_enabled ? color::black : color::gray;
      if (!up.empty()) {
        look::simple_frame(g, up, ctrl::scrollbar_item::up_button == hilite);
        if (ctrl::scrollbar_item::up_button == select) {
          draw::frame::sunken_relief(g, up.shrinked(core::size::two));
        }
        auto s = up_left_arrows[horizontal];
        g.text(draw::text_box(s, up, text_origin_t::center), draw::font::system(), col);
      }
      if (!down.empty()) {
        look::simple_frame(g, down, ctrl::scrollbar_item::down_button == hilite);
        if (ctrl::scrollbar_item::down_button == select) {
          draw::frame::sunken_relief(g, down.shrinked(core::size::two));
        }
        auto s = down_right_arrows[horizontal];
        g.text(draw::text_box(s, down, text_origin_t::center), draw::font::system(), col);
      }
      if (!thumb.empty()) {
        look::simple_frame(g, thumb, ctrl::scrollbar_item::thumb_button == hilite, 3, horizontal ? 3 : 13);
        if (ctrl::scrollbar_item::thumb_button == select) {
          draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
        }
        if (is_focused) {
          core::rectangle area = thumb;
          area.shrink({2, 2});
          g.frame(draw::rectangle(area), draw::pen(color::very_light_blue, 1, draw::pen::Style::solid));

        }
      }
    }

    void scrollbar_w95 (const draw::graphics &g,
                        ctrl::scrollbar_item select,
                        ctrl::scrollbar_item hilite,
                        bool is_enabled,
                        bool horizontal,
                        bool is_focused,
                        const core::rectangle& up,
                        const core::rectangle& down,
                        const core::rectangle& thumb,
                        const core::rectangle& page_up,
                        const core::rectangle& page_down) {
      if (!page_up.empty()) {
        g.fill(draw::rectangle(page_up),
               select == ctrl::scrollbar_item::page_up ? color::light_gray
                                                 : color::very_light_gray);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one),
               select == ctrl::scrollbar_item::page_down ? color::light_gray
                                                   : color::very_light_gray);
      }
      os::color col = is_enabled ? color::black : color::gray;
      if (!up.empty()) {
        look::button_frame_w95(g, up, true, false, ctrl::scrollbar_item::up_button == hilite, false);
        if (ctrl::scrollbar_item::up_button == select) {
          draw::frame::sunken_relief(g, up.shrinked(core::size::two));
        }
        auto s = up_left_arrows[horizontal];
        g.text(draw::text_box(s, up, text_origin_t::center), draw::font::system(), col);
      }
      if (!down.empty()) {
        look::button_frame_w95(g, down, true, false, ctrl::scrollbar_item::down_button == hilite, false);
        if (ctrl::scrollbar_item::down_button == select) {
          draw::frame::sunken_relief(g, down.shrinked(core::size::two));
        }
        auto s = down_right_arrows[horizontal];
        g.text(draw::text_box(s, down, text_origin_t::center), draw::font::system(), col);
      }
      if (!thumb.empty()) {
        look::button_frame_w95(g, thumb, true, false, ctrl::scrollbar_item::thumb_button == hilite, false);
        if (ctrl::scrollbar_item::thumb_button == select) {
          draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
        }
      }
    }

  } // namespace look

} // gui
