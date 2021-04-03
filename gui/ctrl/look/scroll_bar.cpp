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
#include <gui/draw/icons.h>
#include <gui/ctrl/look/scroll_bar.h>
#include <gui/ctrl/look/button.h>


namespace gui {

  namespace look {

    namespace {

      void draw_up_left_arrow (draw::graphics& g, bool horizontal, const core::rectangle& up, os::color col) {
        if (horizontal) {
          g.frame(draw::icon<draw::icon_t::left>(up.center(), up.max_radius() / 2), col);
        } else {
          g.frame(draw::icon<draw::icon_t::up>(up.center(), up.max_radius() / 2), col);
        }
      }

      void draw_down_right_arrow (draw::graphics& g, bool horizontal, const core::rectangle& up, os::color col) {
        if (horizontal) {
          g.frame(draw::icon<draw::icon_t::right>(up.center(), up.max_radius() / 2), col);
        } else {
          g.frame(draw::icon<draw::icon_t::down>(up.center(), up.max_radius() / 2), col);
        }
      }

    }

    // --------------------------------------------------------------------------
    template<>
    void scrollbar<look_and_feel_t::metal> (draw::graphics& g,
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
        draw_up_left_arrow(g, horizontal, up, col);
      }
      if (!down.empty()) {
        look::simple_frame(g, down, ctrl::scrollbar_item::down_button == hilite);
        if (ctrl::scrollbar_item::down_button == select) {
          draw::frame::sunken_relief(g, down.shrinked(core::size::two));
        }
        draw_down_right_arrow(g, horizontal, down, col);
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
    void scrollbar<look_and_feel_t::w95> (draw::graphics& g,
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
          draw::frame::sunken_relief(g, up.shrinked(2, 3, 2, 3));
        }
        draw_up_left_arrow(g, horizontal, up, col);
      }
      if (!down.empty()) {
        look::button_frame<look_and_feel_t::w95>(g, down, true, false, ctrl::scrollbar_item::down_button == hilite, false);
        if (ctrl::scrollbar_item::down_button == select) {
          draw::frame::sunken_relief(g, down.shrinked(2, 3, 2, 3));
        }
        draw_down_right_arrow(g, horizontal, down, col);
      }
      if (!thumb.empty()) {
        look::button_frame<look_and_feel_t::w95>(g, thumb, true, false, ctrl::scrollbar_item::thumb_button == hilite, false);
        if (ctrl::scrollbar_item::thumb_button == select) {
          draw::frame::sunken_relief(g, thumb.shrinked(2, 3, 2, 3));
        }
      }
    }

    static os::color gray128 = color::rgb_gray<128>::value;
    static os::color gray166 = color::rgb_gray<166>::value;
    static os::color gray195 = color::rgb_gray<195>::value;
    static os::color gray205 = color::rgb_gray<205>::value;
    static os::color gray232 = color::rgb_gray<232>::value;
    static os::color gray250 = color::rgb_gray<250>::value;

    template<>
    void scrollbar<look_and_feel_t::w10> (draw::graphics& g,
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
        g.fill(draw::rectangle(page_up), color::very_very_light_gray);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one), color::very_very_light_gray);
      }
      os::color col = is_enabled ? color::black : color::gray;
      if (!up.empty()) {
        g.fill(draw::rectangle(up), color::very_very_light_gray);
        draw_up_left_arrow(g, horizontal, up, col);
      }
      if (!down.empty()) {
        g.fill(draw::rectangle(down), color::very_very_light_gray);
        draw_down_right_arrow(g, horizontal, down, col);
      }
      if (!thumb.empty()) {
        g.fill(draw::rectangle(thumb), ctrl::scrollbar_item::thumb_button == select ?
               gray166 : gray205);
      }
    }

    template<>
    void scrollbar<look_and_feel_t::osx> (draw::graphics& g,
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
        g.fill(draw::rectangle(page_up), gray250);
      }
      if (!page_down.empty()) {
        g.fill(draw::rectangle(page_down + core::size::one), gray250);
      }
      if (!thumb.empty()) {
        g.fill(draw::rectangle(thumb), gray250);
        core::rectangle r = thumb.shrinked({4, 4});
        if (ctrl::scrollbar_item::thumb_button == select) {
          g.fill(draw::round_rectangle(r, 4), gray128);
        } else {
         g.fill(draw::round_rectangle(r, 4), gray195);
        }
      }
      core::rectangle full = thumb | page_up | page_down;
      g.frame(draw::rectangle(full), gray232);
    }

  } // namespace look

} // gui
