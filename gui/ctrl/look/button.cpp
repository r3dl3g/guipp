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
 * @brief     C++ API: button
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cmath>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/graphics.h>
#include <gui/ctrl/look/button.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace image_data {

#   include <gui/ctrl/look/res/button_frame.h>
#   include <gui/ctrl/look/res/button_pressed_frame.h>
#   include <gui/ctrl/look/res/button_rot_frame.h>
#   include <gui/ctrl/look/res/button_pressed_rot_frame.h>
#   include <gui/ctrl/look/res/simple_frame.h>

  } // namespace image_data

  namespace detail {

    template<typename T, size_t N>
    inline std::string make_string (T(&t)[N]) {
      return std::string((const char*)t, N);
    }

    draw::graymap build_image (const std::string& name) {
      draw::graymap bmp;
      std::istringstream in(name);
      io::load_pnm(in, bmp);
      return bmp;
    }

    draw::graymap build_button_frame_image (bool pressed, bool rot) {
      return build_image(pressed ? (rot ? make_string(image_data::button_pressed_rot_frame_bytes)
                                        : make_string(image_data::button_pressed_frame_bytes))
                                 : (rot ? make_string(image_data::button_rot_frame_bytes)
                                        : make_string(image_data::button_frame_bytes)));
    }

    draw::graymap build_simple_frame_image () {
      return build_image(make_string(image_data::simple_frame_bytes));
    }

    template<bool rot, bool pressed>
    const draw::graymap& get_button_frame () {
      static draw::graymap image(detail::build_button_frame_image(pressed, rot));
      return image;
    }

    const draw::graymap& get_simple_frame () {
      static draw::graymap image(detail::build_simple_frame_image());
      return image;
    }

    template<alignment_t A>
    const draw::graymap& get_tab_frame (bool pressed);

    template<>
    const draw::graymap& get_tab_frame<alignment_t::top> (bool pressed) {
      static draw::graymap image(get_button_frame<false, false>().sub(1, 0, 7, 24));
      static draw::graymap image_pressed(get_button_frame<false, true>().sub(1, 0, 7, 24));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::bottom> (bool pressed) {
      static draw::graymap image(get_button_frame<false, true>().sub(1, 4, 7, 24));
      static draw::graymap image_pressed(get_button_frame<false, false>().sub(1, 4, 7, 24));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::left> (bool pressed) {
      static draw::graymap image(get_button_frame<true, false>().sub(0, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, true>().sub(0, 1, 24, 7));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::right> (bool pressed) {
      static draw::graymap image(get_button_frame<true, true>().sub(4, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, false>().sub(4, 1, 24, 7));
      return !pressed ? image_pressed : image;
    }

  } // namespace detail

  // --------------------------------------------------------------------------
  namespace look {

    const int dot_line_width = 1;
    const draw::pen::Style dot_line_style = draw::pen::Style::dot;

    // --------------------------------------------------------------------------
    template<>
    void button_frame<look_and_feel_t::w95> (const draw::graphics& graph,
                                             const core::rectangle& r,
                                             bool enabled, bool pushed, bool hilited, bool focused) {
      core::rectangle area = r;
      graph.fill(draw::rectangle(area), enabled && hilited ? color::buttonHighLightColor() : color::buttonColor());
      if (enabled && focused) {
        graph.frame(draw::rectangle(area), color::black);
        area.shrink({1, 1});
      }

      draw::frame::deep_relief(graph, area, pushed);

      if (enabled && focused && !pushed) {
        area = r.shrinked({6, 6});
        graph.frame(draw::rectangle(area), draw::pen(color::light_gray, dot_line_width, dot_line_style));
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void button_frame<look_and_feel_t::metal> (const draw::graphics& graph,
                                               const core::rectangle& r,
                                               bool enabled, bool pushed, bool hilited, bool focused) {
      if (enabled && hilited) {
        graph.copy(draw::frame_image(r, detail::get_button_frame<false, false>().brightness(1.025F), 4), r.top_left());
      } else {
        graph.copy(draw::frame_image(r, detail::get_button_frame<false, false>(), 4), r.top_left());
      }
      if (pushed) {
        draw::frame::sunken_relief(graph, r.shrinked(core::size::two));
      }

      if (enabled && focused && !pushed) {
        core::rectangle area = r;
        area.shrink({2, 2});
        graph.frame(draw::rectangle(area), draw::pen(color::very_light_blue, 2, draw::pen::Style::solid));
      }
    }

    // --------------------------------------------------------------------------
    void simple_frame (const draw::graphics& graph,
                       const core::rectangle& r,
                       bool hilite,
                       uint32_t horizontal,
                       uint32_t vertical) {
      graph.copy(draw::frame_image(r, hilite ? detail::get_simple_frame().brightness(1.025F)
                                             : detail::get_simple_frame(),
                                   horizontal, vertical), r.top_left());
    }

    // --------------------------------------------------------------------------
    void flat_button (const draw::graphics& g,
                      const core::rectangle& r,
                      const std::string& text,
                      const core::button_state::is& state,
                      os::color foreground,
                      os::color background) {
      bool enabled = state.enabled();
      os::color b = background;
      if (state.pushed() && enabled) {
        b = color::darker(background, 0.25);
      } else if (state.hilited() && enabled) {
        b = color::lighter(background, 0.25);
      }
      g.fill(draw::rectangle(r), b);

      os::color f = foreground;
      if (enabled) {
        if (state.pushed()) {
          os::color b2 = color::invert(b);
          os::color f2 = color::invert(foreground);
          int i1 = color::compare(b, b2);
          int i2 = color::compare(b, f2);
          if (std::abs(i1) > std::abs(i2)) {
            f = b2;
          } else {
            f = f2;
          }
        }
      } else {
        f = color::darker(foreground);
      }
      g.text(draw::text_box(text, r, text_origin_t::center), draw::font::system(), f);
      if (enabled && state.focused()) {
        g.frame(draw::rectangle(r), draw::pen(f, dot_line_width, dot_line_style));
      }
    }

    // --------------------------------------------------------------------------
    void push_button (const draw::graphics& graph,
                      const core::rectangle& r,
                      const std::string& text,
                      const core::button_state::is& state) {
      button_frame<>(graph, r, state);
      using namespace draw;
      graph.text(text_box(text, r, text_origin_t::center), font::system(),
                 state.enabled() ? color::windowTextColor() : color::disabledTextColor());
    }

    // --------------------------------------------------------------------------
    void tab_button (const draw::graphics& g,
                     const core::rectangle& r,
                     const std::string& text,
                     const core::button_state::is& state,
                     os::color foreground,
                     alignment_t a) {
      if (state.checked()) {
        switch (a) {
          case alignment_t::top:    g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::top>(true), 3, 3, 3, 0), r.top_left());    break;
          case alignment_t::bottom: g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::bottom>(true), 3, 0, 3, 3), r.top_left()); break;
          case alignment_t::left:   g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::left>(true), 3, 3, 0, 3), r.top_left());   break;
          case alignment_t::right:  g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::right>(true), 0, 3, 3, 3), r.top_left());  break;
          default: break;
        }
      } else if (state.enabled()) {
        if (state.hilited()) {
          switch (a) {
            case alignment_t::top:    g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::top>(false).brightness(1.025F), 3, 3, 3, 0), r.top_left());    break;
            case alignment_t::bottom: g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::bottom>(false).brightness(1.025F), 3, 0, 3, 3), r.top_left()); break;
            case alignment_t::left:   g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::left>(false).brightness(1.025F), 3, 3, 0, 3), r.top_left());   break;
            case alignment_t::right:  g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::right>(false).brightness(1.025F), 0, 3, 3, 3), r.top_left());  break;
            default: break;
          }
        } else {
          switch (a) {
            case alignment_t::top:    g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::top>(false), 3, 3, 3, 0), r.top_left());    break;
            case alignment_t::bottom: g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::bottom>(false), 3, 0, 3, 3), r.top_left()); break;
            case alignment_t::left:   g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::left>(false), 3, 3, 0, 3), r.top_left());   break;
            case alignment_t::right:  g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::right>(false), 0, 3, 3, 3), r.top_left());  break;
            default: break;
          }
        }
      } else {
        switch (a) {
          case alignment_t::top:    g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::top>(false).brightness(0.75F), 3, 3, 3, 0), r.top_left());    break;
          case alignment_t::bottom: g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::bottom>(false).brightness(0.75F), 3, 0, 3, 3), r.top_left()); break;
          case alignment_t::left:   g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::left>(false).brightness(0.75F), 3, 3, 0, 3), r.top_left());   break;
          case alignment_t::right:  g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::right>(false).brightness(0.75F), 0, 3, 3, 3), r.top_left());  break;
          default: break;
        }
      }
      g.text(draw::text_box(text, r.shrinked({4, 4}), text_origin_t::center), draw::font::system(), foreground);
    }

    // --------------------------------------------------------------------------
    void switch_button (const draw::graphics& graph,
                        const core::rectangle& rect,
                        const std::string& text,
                        const core::button_state::is& state) {
      animated_switch_button(graph, rect, text, state);
    }

    // --------------------------------------------------------------------------
    void animated_switch_button (const draw::graphics& graph,
                                 const core::rectangle& rect,
                                 const std::string& text,
                                 const core::button_state::is& state,
                                 float animation_step) {
      bool enabled = state.enabled();
      core::size::type height = rect.height();
      core::size::type width = height * 2;
      core::size::type edge = height / 2;
      graph.fill(draw::rectangle(rect), color::buttonColor());

      float step = state.checked() ? animation_step : 1.0F - animation_step;

      core::rectangle switch_rect{rect.top_left(), core::size(width, height)};
      os::color thumb_col = enabled && state.hilited() ? color::lighter(color::buttonColor()) : color::buttonColor();
      os::color fill_col = color::merge(color::highLightColor(), color::very_light_gray, 1.0F - step);
      core::rectangle thumb = {core::point{rect.x() + height * step, rect.y()}, core::size(height)};

      graph.draw(draw::round_rectangle(switch_rect, core::size(edge)), fill_col, color::medium_gray);
      auto center = thumb.center();
      graph.draw(draw::pie(center, edge, 0, 360), thumb_col, color::medium_gray);
      graph.frame(draw::pie(center, edge - 1, 40, 250), color::white);
      graph.frame(draw::pie(center, edge - 1, 250, 40), color::medium_gray);
      graph.fill(draw::pie(center, edge - 2, 0, 360), thumb_col);

      os::color text_col = enabled ? color::windowTextColor() : color::disabledTextColor();
      graph.text(draw::text_box(text, rect + core::point(width + 10, 0), text_origin_t::vcenter_left), draw::font::system(), text_col);
    }

    // --------------------------------------------------------------------------
    void radio_button (const draw::graphics& graph,
                       const core::rectangle& rec,
                       const std::string& text,
                       const core::button_state::is& state) {
      using namespace draw;

      core::rectangle area = rec;

      os::color col = state.enabled() ? color::windowTextColor() : color::disabledTextColor();
      graph.fill(draw::rectangle(area), color::buttonColor());

      core::point::type y = area.y() + area.size().height() / 2;
      core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
      graph.draw(ellipse(r), state.pushed() ? color::very_light_gray
                                               : color::buttonColor(), col);
      if (state.checked()) {
        r.shrink(core::size(2, 2));
        graph.fill(ellipse(r), state.pushed() ? color::dark_gray : col);
      }
      area.x(20);
      graph.text(text_box(text, area, text_origin_t::vcenter_left), font::system(), col);
      if (state.focused()) {
        graph.text(bounding_box(text, area, text_origin_t::vcenter_left), font::system(), color::black);
        area.grow({3, 3});
        graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
      }
    }

    // --------------------------------------------------------------------------
    void check_box (const draw::graphics& graph,
                    const core::rectangle& rec,
                    const std::string& text,
                    const core::button_state::is& state) {
      using namespace draw;

      core::rectangle area = rec;

      graph.fill(draw::rectangle(area), color::buttonColor());

      core::point::type y = area.y() + area.height() / 2;
      os::color col = state.enabled() ? color::windowTextColor() : color::disabledTextColor();

      core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
      graph.draw(rectangle(r),
                 state.pushed() ? color::very_light_gray
                                   : color::buttonColor(),
                 col);

      if (state.checked()) {
        r.shrink(core::size(2, 2));
        graph.fill(rectangle(r), state.pushed() ? color::dark_gray : col);
      }
      area.x(20);
      graph.text(text_box(text, area, text_origin_t::vcenter_left), font::system(), col);
      if (state.focused()) {
        graph.text(bounding_box(text, area, text_origin_t::vcenter_left), font::system(), color::black);
        area.grow({3, 3});
        graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
      }
    }

  } // look

} // gui
