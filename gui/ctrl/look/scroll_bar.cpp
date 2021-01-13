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
    template<>
    void scrollbar<look_and_feel_t::metal> (const draw::graphics &g,
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

    template<>
    void scrollbar<look_and_feel_t::w95> (const draw::graphics &g,
                                          ctrl::scrollbar_item select,
                                          ctrl::scrollbar_item hilite,
                                          bool is_enabled,
                                          bool horizontal,
                                          bool /*is_focused*/,
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
        look::button_frame<look_and_feel_t::w95>(g, up, true, false, ctrl::scrollbar_item::up_button == hilite, false);
        if (ctrl::scrollbar_item::up_button == select) {
          draw::frame::sunken_relief(g, up.shrinked(core::size::two));
        }
        auto s = up_left_arrows[horizontal];
        g.text(draw::text_box(s, up, text_origin_t::center), draw::font::system(), col);
      }
      if (!down.empty()) {
        look::button_frame<look_and_feel_t::w95>(g, down, true, false, ctrl::scrollbar_item::down_button == hilite, false);
        if (ctrl::scrollbar_item::down_button == select) {
          draw::frame::sunken_relief(g, down.shrinked(core::size::two));
        }
        auto s = down_right_arrows[horizontal];
        g.text(draw::text_box(s, down, text_origin_t::center), draw::font::system(), col);
      }
      if (!thumb.empty()) {
        look::button_frame<look_and_feel_t::w95>(g, thumb, true, false, ctrl::scrollbar_item::thumb_button == hilite, false);
        if (ctrl::scrollbar_item::thumb_button == select) {
          draw::frame::sunken_relief(g, thumb.shrinked(core::size::two));
        }
      }
    }

    std::vector<core::point> up_left_arrows_poly (const core::rectangle& area, bool horizontal) {
      core::rectangle r = area.shrinked(area.size() / 3);
      if (!r.empty()) {
        if (horizontal) {
          return {r.top_right(), {r.x(), r.center_y()}, r.bottom_right()};
        } else {
          return {r.bottom_right(), {r.center_x(), r.y()}, r.bottom_left()};
        }
      }
      return {};
    }

    std::vector<core::point> down_right_arrows_poly (const core::rectangle& area, bool horizontal) {
      core::rectangle r = area.shrinked(area.size() / 3);
      if (!r.empty()) {
        if (horizontal) {
          return {r.top_left(), {r.x2(), r.center_y()}, r.bottom_left()};
        } else {
          return {r.top_left(), {r.center_x(), r.y2()}, r.top_right()};
        }
      }
      return {};
    }

    template<>
    void scrollbar<look_and_feel_t::w10> (const draw::graphics &g,
                                          ctrl::scrollbar_item select,
                                          ctrl::scrollbar_item hilite,
                                          bool is_enabled,
                                          bool horizontal,
                                          bool /*is_focused*/,
                                          const core::rectangle& up,
                                          const core::rectangle& down,
                                          const core::rectangle& thumb,
                                          const core::rectangle& page_up,
                                          const core::rectangle& page_down) {
      if (!page_up.empty()) {
        g.fill(draw::rectangle(page_up), color::rgb_gray<240>::value);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one), color::rgb_gray<240>::value);
      }
      os::color col = is_enabled ? color::black : color::gray;
      if (!up.empty()) {
        g.fill(draw::rectangle(up), color::rgb_gray<240>::value);
        g.frame(draw::polyline(up_left_arrows_poly(up, horizontal)), col);
      }
      if (!down.empty()) {
        g.fill(draw::rectangle(down), color::rgb_gray<240>::value);
        g.frame(draw::polyline(down_right_arrows_poly(down, horizontal)), col);
      }
      if (!thumb.empty()) {
        g.fill(draw::rectangle(thumb), ctrl::scrollbar_item::thumb_button == select ?
                 color::rgb_gray<166>::value : color::rgb_gray<205>::value);
      }
    }

    template<>
    void scrollbar<look_and_feel_t::osx> (const draw::graphics &g,
                                          ctrl::scrollbar_item select,
                                          ctrl::scrollbar_item /*hilite*/,
                                          bool /*is_enabled*/,
                                          bool /*horizontal*/,
                                          bool /*is_focused*/,
                                          const core::rectangle& /*up*/,
                                          const core::rectangle& /*down*/,
                                          const core::rectangle& thumb,
                                          const core::rectangle& page_up,
                                          const core::rectangle& page_down) {
      if (!page_up.empty()) {
        g.fill(draw::rectangle(page_up), color::rgb_gray<250>::value);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one), color::rgb_gray<250>::value);
      }
      if (!thumb.empty()) {
        g.fill(draw::rectangle(thumb), color::rgb_gray<250>::value);
        core::rectangle r = thumb.shrinked({4, 4});
        if (ctrl::scrollbar_item::thumb_button == select) {
          g.fill(draw::round_rectangle(r, 4), color::rgb_gray<128>::value);
        } else {
         g.fill(draw::round_rectangle(r, 4), color::rgb_gray<195>::value);
        }
      }
      core::rectangle full = thumb | page_up | page_down;
      g.frame(draw::rectangle(full), color::rgb_gray<232>::value);
    }

  } // namespace look

} // gui
