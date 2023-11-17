/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     various button looks
 *
 * @license   MIT license. See accompanying file LICENSE.
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

#   include <gui/ctrl/look/res/metal_check_off.h>
#   include <gui/ctrl/look/res/metal_check_on.h>
#   include <gui/ctrl/look/res/metal_check_disabled.h>
#   include <gui/ctrl/look/res/metal_check_disabled_on.h>

#   include <gui/ctrl/look/res/metal_radio_off.h>
#   include <gui/ctrl/look/res/metal_radio_on.h>
#   include <gui/ctrl/look/res/metal_radio_disabled.h>
#   include <gui/ctrl/look/res/metal_radio_disabled_on.h>

#   include <gui/ctrl/look/res/osx_checkbox_off.h>
#   include <gui/ctrl/look/res/osx_checkbox_on.h>
#   include <gui/ctrl/look/res/osx_checkbox_disabled_off.h>
#   include <gui/ctrl/look/res/osx_checkbox_disabled_on.h>

#   include <gui/ctrl/look/res/osx_radio_off.h>
#   include <gui/ctrl/look/res/osx_radio_on.h>
#   include <gui/ctrl/look/res/osx_radio_disabled_off.h>
#   include <gui/ctrl/look/res/osx_radio_disabled_on.h>

#   include <gui/ctrl/look/res/osx_frame.h>
#   include <gui/ctrl/look/res/osx_frame_disabled.h>
#   include <gui/ctrl/look/res/osx_frame_default.h>
#   include <gui/ctrl/look/res/osx_frame_pressed.h>

  } // namespace image_data

  namespace detail {

    template<typename T, size_t N>
    inline std::string make_string (T(&t)[N]) {
      return std::string((const char*)t, N);
    }

    draw::graymap build_gray_image (const std::string& data) {
      draw::graymap bmp;
      std::istringstream in(data);
      io::load_pnm(in, bmp);
      return bmp;
    }

    draw::rgbmap build_rgb_image (const std::string& data) {
      draw::rgbmap bmp;
      std::istringstream in(data);
      io::load_pnm(in, bmp);
      return bmp;
    }

    draw::graymap build_button_frame_image (bool pressed, bool rot) {
      return build_gray_image(pressed ? (rot ? make_string(image_data::button_pressed_rot_frame_bytes)
                                             : make_string(image_data::button_pressed_frame_bytes))
                                      : (rot ? make_string(image_data::button_rot_frame_bytes)
                                             : make_string(image_data::button_frame_bytes)));
    }

    draw::graymap build_simple_frame_image () {
      return build_gray_image(make_string(image_data::simple_frame_bytes));
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
      return pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::bottom> (bool pressed) {
      static draw::graymap image(get_button_frame<false, true>().sub(1, 4, 7, 24));
      static draw::graymap image_pressed(get_button_frame<false, false>().sub(1, 4, 7, 24));
      return pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::left> (bool pressed) {
      static draw::graymap image(get_button_frame<true, false>().sub(0, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, true>().sub(0, 1, 24, 7));
      return pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::right> (bool pressed) {
      static draw::graymap image(get_button_frame<true, true>().sub(4, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, false>().sub(4, 1, 24, 7));
      return pressed ? image_pressed : image;
    }

    template<typename T>
    T upscale (const T& img) {
      if (core::global::get_scale_factor() != 1.0) {
        T up(img.native_size() * core::global::get_scale_factor());
        up.stretch_from(img);
        return up;
      }
      return img;
    }

    template<typename T>
    T downscale (const T& img) {
      if (core::global::get_scale_factor() != 2.0) {
        T up(img.native_size() * (core::global::get_scale_factor() / 2.0));
        up.stretch_from(img);
        return up;
      }
      return img;
    }

    template<typename T>
    draw::masked_bitmap make_masked (const T& img) {
      auto mask = img.get_mask({0x01});
      return draw::masked_bitmap(img, std::move(mask));
    }


    const draw::masked_bitmap& get_osx_checkbox (bool active, bool disabled) {
      static draw::masked_bitmap off      = make_masked(upscale(build_gray_image(make_string(image_data::osx_checkbox_off)).convert<pixel_format_t::RGB>()));
      static draw::masked_bitmap on       = make_masked(upscale(build_rgb_image(make_string(image_data::osx_checkbox_on))));
      static draw::masked_bitmap dis_off  = make_masked(upscale(build_gray_image(make_string(image_data::osx_checkbox_disabled_off)).convert<pixel_format_t::RGB>()));
      static draw::masked_bitmap dis_on   = make_masked(upscale(build_gray_image(make_string(image_data::osx_checkbox_disabled_on)).convert<pixel_format_t::RGB>()));
      return disabled ? (active ? dis_on : dis_off) : (active ? on : off);
    }

    const draw::masked_bitmap& get_osx_radio (bool active, bool disabled) {
      static draw::masked_bitmap off      = make_masked(upscale(build_gray_image(make_string(image_data::osx_radio_off)).convert<pixel_format_t::RGB>()));
      static draw::masked_bitmap on       = make_masked(upscale(build_rgb_image(make_string(image_data::osx_radio_on))));
      static draw::masked_bitmap dis_off  = make_masked(upscale(build_gray_image(make_string(image_data::osx_radio_disabled_off)).convert<pixel_format_t::RGB>()));
      static draw::masked_bitmap dis_on   = make_masked(upscale(build_gray_image(make_string(image_data::osx_radio_disabled_on)).convert<pixel_format_t::RGB>()));
      return disabled ? (active ? dis_on : dis_off) : (active ? on : off);
    }

    const draw::masked_bitmap& get_metal_checkbox (bool active, bool disabled) {
      static draw::masked_bitmap off      = make_masked(downscale(build_gray_image(make_string(image_data::metal_check_off))));
      static draw::masked_bitmap on       = make_masked(downscale(build_gray_image(make_string(image_data::metal_check_on))));
      static draw::masked_bitmap dis_off  = make_masked(downscale(build_gray_image(make_string(image_data::metal_check_disabled))));
      static draw::masked_bitmap dis_on   = make_masked(downscale(build_gray_image(make_string(image_data::metal_check_disabled_on))));
      return disabled ? (active ? dis_on : dis_off) : (active ? on : off);
    }

    const draw::masked_bitmap& get_metal_radio (bool active, bool disabled) {
      static draw::masked_bitmap off = make_masked(downscale(build_gray_image(make_string(image_data::metal_radio_off))));
      static draw::masked_bitmap on = make_masked(downscale(build_gray_image(make_string(image_data::metal_radio_on))));
      static draw::masked_bitmap dis_off = make_masked(downscale(build_gray_image(make_string(image_data::metal_radio_disabled))));
      static draw::masked_bitmap dis_on = make_masked(downscale(build_gray_image(make_string(image_data::metal_radio_disabled_on))));
      return disabled ? (active ? dis_on : dis_off) : (active ? on : off);
    }

  } // namespace detail

  // --------------------------------------------------------------------------
  namespace look {

    namespace osx {

      const draw::rgbmap& get_frame () {
        static draw::rgbmap img = detail::build_gray_image(detail::make_string(image_data::osx_frame)).convert<pixel_format_t::RGB>();
        return img;
      }

      const draw::rgbmap& get_disabled_frame () {
        static draw::rgbmap img = detail::build_gray_image(detail::make_string(image_data::osx_frame_disabled)).convert<pixel_format_t::RGB>();
        return img;
      }

      const draw::rgbmap& get_focused_frame () {
        static draw::rgbmap img = detail::build_rgb_image(detail::make_string(image_data::osx_frame_default));
        return img;
      }

      const draw::rgbmap& get_pressed_frame () {
        static draw::rgbmap img = detail::build_rgb_image(detail::make_string(image_data::osx_frame_pressed));
        return img;
      }

    } // namespace osx

#ifndef GUIPP_BUILD_FOR_MOBILE
    const gui::draw::pen::size_type dot_line_width = 1;
    const draw::pen::Style dot_line_style = draw::pen::Style::dot;
#endif // GUIPP_BUILD_FOR_MOBILE

    // --------------------------------------------------------------------------
    template<>
    void button_frame_t<look_and_feel_t::w95> (draw::graphics& graph,
                                             const core::rectangle& r,
                                             const core::button_state::is& st,
                                             os::color, os::color) {
      core::rectangle area = r;
      graph.fill(draw::rectangle(area), st.enabled() && st.hilited() ? color::buttonHighLightColor() : color::buttonColor());
      if (st.enabled() && st.focused()) {
        graph.frame(draw::rectangle(area), color::black);
        area.shrink({1, 1});
      }

      draw::frame::deep_relief(graph, area, st.pushed());

#ifndef GUIPP_BUILD_FOR_MOBILE
      if (st.enabled() && st.focused() && !st.pushed()) {
        area = r.shrinked({6, 6});
        graph.frame(draw::rectangle(area), draw::pen(color::light_gray, dot_line_width, dot_line_style));
      }
#endif // GUIPP_BUILD_FOR_MOBILE
    }

    namespace win10 {

      os::color get_button_color (const core::button_state::is& st) {
        if (!st.enabled()) {
          return color::rgb_gray<204>::value;
        } else if (st.pushed()) {
          return color::rgb<204, 228, 247>::value;
        } else if (st.hilited() || st.focused()) {
          return color::rgb<229, 241, 251>::value;
        } else {
          return color::rgb_gray<225>::value;
        }
      }

      os::color get_button_frame_color (const core::button_state::is& st) {
        if (!st.enabled()) {
          return color::rgb_gray<191>::value;
        } else if (st.pushed()) {
          return color::rgb<0, 84, 153>::value;
        } else if (st.hilited() || st.focused()) {
          return color::rgb<0, 120, 215>::value;
        } else {
          return color::rgb_gray<173>::value;
        }
      }

    }

    // --------------------------------------------------------------------------
    os::color get_flat_button_background (const core::button_state::is& st, os::color bg) {
      if (st.enabled()) {
        if (st.pushed()) {
          return color::darker(bg, 0.25);
        } else if (st.hilited()) {
          return color::lighter(bg, 0.25);
        }
      }
      return bg;
    }

    os::color get_flat_button_foreground (const core::button_state::is& st, os::color fg, os::color bg) {
      if (st.enabled()) {
        if (st.pushed()) {
          const os::color b2 = color::invert(bg);
          const os::color f2 = color::invert(fg);
          const int i1 = color::compare_weight(bg, b2);
          const int i2 = color::compare_weight(bg, f2);
          if (std::abs(i1) > std::abs(i2)) {
            return b2;
          } else {
            return f2;
          }
        }
        return fg;
      } else {
        return color::darker(fg);
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void button_frame_t<look_and_feel_t::w10> (draw::graphics& graph,
                                             const core::rectangle& r,
                                             const core::button_state::is& st,
                                             os::color, os::color) {
      graph.draw(draw::rectangle(r), win10::get_button_color(st), win10::get_button_frame_color(st));
    }

    // --------------------------------------------------------------------------
    template<>
    void button_frame_t<look_and_feel_t::metal> (draw::graphics& graph,
                                               const core::rectangle& r,
                                               const core::button_state::is& st,
                                               os::color, os::color) {
      if (st.enabled() && st.hilited()) {
        graph.copy(draw::frame_image(r, detail::get_button_frame<false, false>().brightness(1.025F), 4), r.top_left());
      } else {
        graph.copy(draw::frame_image(r, detail::get_button_frame<false, false>(), 4), r.top_left());
      }
      if (st.pushed()) {
        draw::frame::sunken_relief(graph, r.shrinked(core::size::two));
      }

      if (st.enabled() && st.focused() && !st.pushed()) {
        core::rectangle area = r;
        area.shrink({2, 2});
        graph.frame(draw::rectangle(area), draw::pen(color::very_light_blue, 2, draw::pen::Style::solid));
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void button_frame_t<look_and_feel_t::osx> (draw::graphics& graph,
                                             const core::rectangle& r,
                                             const core::button_state::is& st,
                                             os::color, os::color) {
      if (!st.enabled()) {
        graph.copy(draw::frame_image(r, osx::get_disabled_frame(), 4), r.top_left());
      } else if (st.pushed() && st.hilited()) {
        graph.copy(draw::frame_image(r, osx::get_pressed_frame(), 4), r.top_left());
      } else if (st.focused()) {
        graph.copy(draw::frame_image(r, osx::get_focused_frame(), 4), r.top_left());
      } else {
        graph.copy(draw::frame_image(r, osx::get_frame(), 4), r.top_left());
      }
    }

    void button_frame (draw::graphics& graph,
                       const core::rectangle& r,
                       const core::button_state::is& st,
                       os::color fg,
                       os::color bg) {
      button_frame_t<system_look_and_feel>(graph, r, st, fg, bg);
    }

    // --------------------------------------------------------------------------
    void button_text (draw::graphics& g,
                      const core::rectangle& r,
                      const std::string& text,
                      const core::button_state::is& st,
                      os::color fg,
                      os::color) {
      os::color f = st.enabled() ? fg : color::disabledTextColor();
      g.text(draw::text_box(text, r, text_origin_t::center), draw::font::system(), f);
    }

    // --------------------------------------------------------------------------
    void simple_frame (draw::graphics& graph,
                       const core::rectangle& r,
                       bool hilite,
                       uint32_t horizontal,
                       uint32_t vertical) {
      graph.copy(draw::frame_image(r, hilite ? detail::get_simple_frame().brightness(1.025F)
                                             : detail::get_simple_frame(),
                                   horizontal, vertical), r.top_left());
    }

    void simple_frame (draw::graphics& g,
                       const core::rectangle& r,
                       const core::button_state::is& st,
                       os::color,
                       os::color) {
      simple_frame(g, r, st.hilited());
    }

    // --------------------------------------------------------------------------
    void flat_button_frame (draw::graphics& g,
                            const core::rectangle& r,
                            const core::button_state::is& st,
                            os::color fg,
                            os::color bg) {
      os::color b = get_flat_button_background(st, bg);
      g.fill(draw::rectangle(r), b);
#ifndef GUIPP_BUILD_FOR_MOBILE
      if (st.enabled() && st.focused()) {
        g.frame(draw::rectangle(r), draw::pen(get_flat_button_foreground(st, fg, b),
                                              dot_line_width, dot_line_style));
      }
#endif // GUIPP_BUILD_FOR_MOBILE
    }

    void flat_button_text (draw::graphics& g,
                           const core::rectangle& r,
                           const std::string& text,
                           const core::button_state::is& st,
                           os::color fg,
                           os::color bg) {
      os::color f = get_flat_button_foreground(st, fg, get_flat_button_background(st, bg));
      g.text(draw::text_box(text, r, text_origin_t::center), draw::font::system(), f);
    }

    // --------------------------------------------------------------------------
    void flat_button (draw::graphics& g,
                      const core::rectangle& r,
                      const std::string& text,
                      const core::button_state::is& st,
                      os::color fg,
                      os::color bg) {
      flat_button_frame(g, r, st, fg, bg);
      flat_button_text(g, r, text, st, fg, bg);
    }

    // --------------------------------------------------------------------------
    void push_button (draw::graphics& g,
                      const core::rectangle& r,
                      const std::string& text,
                      const core::button_state::is& st,
                      os::color fg,
                      os::color bg) {
      button_frame(g, r, st, fg, bg);
      button_text(g, r, text, st, fg, bg);
    }

    // --------------------------------------------------------------------------
    template<>
    void tab_button<look_and_feel_t::metal> (draw::graphics& g,
                                             const core::rectangle& r,
                                             const core::button_state::is& st,
                                             os::color fg,
                                             alignment_t a) {
      if (st.checked()) {
        switch (a) {
          case alignment_t::top:    g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::top>(true), 3, 3, 3, 0), r.top_left());    break;
          case alignment_t::bottom: g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::bottom>(true), 3, 0, 3, 3), r.top_left()); break;
          case alignment_t::left:   g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::left>(true), 3, 3, 0, 3), r.top_left());   break;
          case alignment_t::right:  g.copy(draw::frame_image(r, detail::get_tab_frame<alignment_t::right>(true), 0, 3, 3, 3), r.top_left());  break;
          default: break;
        }
      } else if (st.enabled()) {
        if (st.hilited()) {
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
    }

    void tab_button_text (draw::graphics& g,
                          const core::rectangle& r,
                          const std::string& text,
                          const core::button_state::is& st,
                          os::color fg,
                          os::color bg) {
      g.text(draw::text_box(text, r.shrinked({4, 4}), text_origin_t::center), draw::font::system(), fg);
    }
    // --------------------------------------------------------------------------
    void switch_button (draw::graphics& graph,
                        const core::rectangle& rect,
                        const core::button_state::is& st,
                        os::color fg,
                        os::color bg) {
      animated_switch_button(graph, rect, st, fg, bg);
    }

    void switch_button_text (draw::graphics& graph,
                             const core::rectangle& rect,
                             const std::string& text,
                             const core::button_state::is& st,
                             os::color fg,
                             os::color bg) {
      os::color text_col = st.enabled() ? color::windowTextColor() : color::disabledTextColor();
      graph.text(draw::text_box(text, rect + core::point(rect.height() * 2 + 10, 0),
                                text_origin_t::vcenter_left), draw::font::system(), text_col);
    }

    // --------------------------------------------------------------------------
    void animated_switch_button (draw::graphics& graph,
                                 const core::rectangle& rect,
                                 const core::button_state::is& st,
                                 os::color fg,
                                 os::color bg,
                                 float animation_step) {
      bool enabled = st.enabled();
      core::size::type height = rect.height();
      core::size::type width = height * 2;
      core::size::type edge = height / 2;
      graph.fill(draw::rectangle(rect), bg);

      float step = st.checked() ? animation_step : 1.0F - animation_step;

      core::rectangle switch_rect{rect.top_left(), core::size(width, height)};
      os::color thumb_col = enabled && st.hilited() ? color::lighter(bg) : bg;
      os::color fill_col = color::merge(fg, color::very_light_gray, 1.0F - step);
      core::rectangle thumb = {core::point{rect.x() + height * step, rect.y()}, core::size(height)};

      graph.draw(draw::round_rectangle(switch_rect, core::size(edge)), fill_col, color::medium_gray);
      auto center = thumb.center();
      graph.draw(draw::pie(center, edge, 0, 360), thumb_col, color::medium_gray);
      graph.frame(draw::pie(center, edge - 1, 40, 250), color::white);
      graph.frame(draw::pie(center, edge - 1, 250, 40), color::medium_gray);
      graph.fill(draw::pie(center, edge - 2, 0, 360), thumb_col);
    }

    // --------------------------------------------------------------------------
    template<>
    os::color get_button_foreground<look_and_feel_t::metal> (const core::button_state::is& st, os::color fg, os::color bg) {
      return st.enabled() ? fg : color::disabledTextColor();
    }

    template<>
    os::color get_button_foreground<look_and_feel_t::w95> (const core::button_state::is& st, os::color fg, os::color bg) {
      return st.enabled() ? fg : color::disabledTextColor();
    }

    template<>
    os::color get_button_foreground<look_and_feel_t::w10> (const core::button_state::is& st, os::color fg, os::color bg) {
      if (!st.enabled()) {
        return color::rgb_gray<204>::value;
      } else if (st.focused() || st.pushed()) {
        return color::rgb<0, 120, 215>::value;
      } else {
        return color::black;
      }
    }

    template<>
    os::color get_button_foreground<look_and_feel_t::osx> (const core::button_state::is& st, os::color fg, os::color bg) {
      return st.enabled() ? fg : color::disabledTextColor();
    }

    // --------------------------------------------------------------------------
    void check_button_text (draw::graphics& graph,
                            const core::rectangle& rec,
                            const std::string& text,
                            const core::button_state::is& st,
                            os::color fg,
                            os::color) {
      using namespace draw;

      core::rectangle area = rec.dx(20);
      graph.text(text_box(text, area, text_origin_t::vcenter_left), font::system(), fg);
#ifndef GUIPP_BUILD_FOR_MOBILE
      if (st.focused()) {
        graph.text(bounding_box(text, area, text_origin_t::vcenter_left), font::system(), color::black);
        area.grow({3, 3});
        graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
      }
#endif // GUIPP_BUILD_FOR_MOBILE
    }

    // --------------------------------------------------------------------------
    template<>
    void radio_button_frame<look_and_feel_t::w95> (draw::graphics& graph,
                                               const core::rectangle& area,
                                               const core::button_state::is& st,
                                               os::color fg,
                                               os::color bg) {
      using namespace draw;

      graph.fill(draw::rectangle(area), bg);

      core::point::type y = area.y() + area.size().height() / 2;
      core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
      graph.draw(ellipse(r), st.pushed() ? color::very_light_gray : bg, fg);
      if (st.checked()) {
        r.shrink(core::size(2, 2));
        graph.fill(ellipse(r), st.pushed() ? color::dark_gray : fg);
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void radio_button_frame<look_and_feel_t::w10> (draw::graphics& graph,
                                               const core::rectangle& area,
                                               const core::button_state::is& st,
                                               os::color fg,
                                               os::color bg) {
      using namespace draw;

      graph.fill(rectangle(area), bg);

      core::point::type y = area.center_y();
      core::rectangle r(core::point(area.x() + 1, y - 6), core::size(12, 12));
      graph.frame(ellipse(r), fg);
      if (st.checked()) {
        r.shrink(core::size(3, 3));
        graph.fill(ellipse(r), fg);
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void radio_button_frame<look_and_feel_t::metal> (draw::graphics& graph,
                                                 const core::rectangle& area,
                                                 const core::button_state::is& st,
                                                 os::color fg,
                                                 os::color bg) {
      using namespace draw;

      const auto& img = detail::get_metal_radio(st.checked(), !st.enabled());
      graph.fill(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_left), bg);
    }

    // --------------------------------------------------------------------------
    template<>
    void radio_button_frame<look_and_feel_t::osx> (draw::graphics& graph,
                                               const core::rectangle& area,
                                               const core::button_state::is& st,
                                               os::color fg,
                                               os::color bg) {
      using namespace draw;

      const auto& img = detail::get_osx_radio(st.checked(), !st.enabled());
      graph.fill(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_left), bg);
    }

    // --------------------------------------------------------------------------
    void radio_button (draw::graphics& graph,
                       const core::rectangle& rec,
                       const std::string& text,
                       const core::button_state::is& st,
                       os::color fg,
                       os::color bg) {
      fg = get_button_foreground<>(st, fg, bg);
      radio_button_frame<>(graph, rec, st, fg, bg);
      check_button_text(graph, rec, text, st, fg, bg);
    }

    // --------------------------------------------------------------------------
    template<>
    void check_box_frame<look_and_feel_t::w95> (draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& st,
                                            os::color fg,
                                            os::color bg) {
      using namespace draw;

      graph.fill(draw::rectangle(area), bg);

      core::point::type y = area.y() + area.height() / 2;

      core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
      graph.draw(rectangle(r), st.pushed() ? color::very_light_gray : bg, fg);

      if (st.checked()) {
        r.shrink(core::size(2, 2));
        graph.fill(rectangle(r), st.pushed() ? color::dark_gray : fg);
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void check_box_frame<look_and_feel_t::w10> (draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& st,
                                            os::color fg,
                                            os::color bg) {
      using namespace draw;

      graph.fill(rectangle(area), bg);

      core::point::type y = area.center_y();
      core::rectangle r(core::point(area.x() + 1, y - 6), core::size(12, 12));
      graph.frame(rectangle(r), fg);
      if (st.checked()) {
        graph.frame(polyline({{r.x() + 1, y}, {r.x() + 4, y + 3}, {r.x() + 10, y - 3}}), pen(fg, 2));
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void check_box_frame<look_and_feel_t::metal> (draw::graphics& graph,
                                              const core::rectangle& area,
                                              const core::button_state::is& st,
                                              os::color fg,
                                              os::color bg) {
      const auto& img = detail::get_metal_checkbox(st.checked(), !st.enabled());
      graph.copy(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_left), area.top_left());
    }

    // --------------------------------------------------------------------------
    template<>
    void check_box_frame<look_and_feel_t::osx> (draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& st,
                                            os::color fg,
                                            os::color bg) {
      const auto& img = detail::get_osx_checkbox(st.checked(), !st.enabled());
      graph.copy(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_left), area.top_left());
    }

    // --------------------------------------------------------------------------
    void check_box (draw::graphics& graph,
                    const core::rectangle& rec,
                    const std::string& text,
                    const core::button_state::is& st,
                    os::color fg,
                    os::color bg) {
      fg = get_button_foreground<>(st, fg, bg);
      check_box_frame<>(graph, rec, st, fg, bg);
      check_button_text(graph, rec, text, st, fg, bg);
    }

  } // look

} // gui
