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
#include <gui/draw/frames.h>
#include <gui/draw/brush.h>
#include <gui/draw/pen.h>
#include <gui/draw/font.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/look/button.h>
#include <gui/ctrl/look/drop_down.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace image_data {

#   include <gui/ctrl/look/res/osx_dropdown_button.h>
#   include <gui/ctrl/look/res/osx_dropdown_disabled_button.h>

  } // namespace image_data

  namespace detail {

    template<typename T, size_t N>
    inline std::string make_string (T(&t)[N]) {
      return std::string((const char*)t, N);
    }

    draw::rgbmap build_rgb_image (const std::string& data);
    draw::graymap build_gray_image (const std::string& data);
    template<typename T>
    T upscale (const T& img);

    const draw::rgbmap& get_osx_dropdown_button (bool enabled) {
      static draw::rgbmap img = upscale(build_rgb_image(make_string(image_data::osx_dropdown_button)));
      static draw::rgbmap dis = upscale(build_gray_image(make_string(image_data::osx_dropdown_disabled_button)).convert<pixel_format_t::RGB>());
      return enabled ? img : dis;
    }

  }

  namespace look {

    void drop_down_item (draw::graphics& g,
                         const core::rectangle& r,
                         const draw::brush& b,
                         const std::string& t,
                         const ctrl::item_state& s) {
      text_item(g, r, b, t, s, text_origin_t::vcenter_left);
    }

    std::vector<core::point> get_up_down_polygon (const core::rectangle& area, bool up) {
      core::rectangle r = area.shrinked(area.size() / 3);
      if (!r.empty()) {
        if (up) {
          return {r.x2y2(), {r.center_x(), r.y()}, r.x1y2()};
        } else {
          return {r.top_left(), {r.center_x(), r.y2()}, r.x2y1()};
        }
      }
      return {};
    }

    template<>
    void drop_down_button_t<look_and_feel_t::metal> (draw::graphics& graph,
                                                     const core::rectangle& area,
                                                     const core::button_state::is& state,
                                                     bool is_open) {
      graph.fill(draw::polygon(get_up_down_polygon(area, is_open)), state.enabled() ? color::black : color::light_gray);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::w95> (draw::graphics& graph,
                                                   const core::rectangle& area,
                                                   const core::button_state::is& state,
                                                   bool is_open) {
      look::button_frame<look_and_feel_t::w95>(graph, area, state);
      graph.fill(draw::polygon(get_up_down_polygon(area, is_open)), state.enabled() ? color::black : color::light_gray);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::w10> (draw::graphics& graph,
                                                   const core::rectangle& r,
                                                   const core::button_state::is& state,
                                                   bool is_open) {
      graph.frame(draw::polyline(get_up_down_polygon(r, is_open)), state.enabled() ? color::black : color::light_gray);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::osx> (draw::graphics& graph,
                                                   const core::rectangle& area,
                                                   const core::button_state::is& state,
                                                   bool) {
      const auto& img = detail::get_osx_dropdown_button(state.enabled());
      graph.fill(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_right), color::buttonColor());
    }

    void drop_down_button (draw::graphics& graph,
                           const core::rectangle& area,
                           const core::button_state::is& state,
                           bool is_open) {
      drop_down_button_t<>(graph, area, state, is_open);
    }

    template<>
    void drop_down_t<look_and_feel_t::metal> (draw::graphics& graph,
                                              const core::rectangle& r,
                                              const core::button_state::is& state) {
      button_frame<look_and_feel_t::metal>(graph, r, state);
    }

    template<>
    void drop_down_t<look_and_feel_t::w95> (draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& state) {
      draw::frame::sunken_deep_relief(graph, area);
      if (state.focused()) {
        draw::frame::dots(graph, area);
      }
    }

    template<>
    void drop_down_t<look_and_feel_t::w10> (draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& state) {
      button_frame<look_and_feel_t::w10>(graph, area, state);
    }

    void drop_down_frame (draw::graphics& graph,
                          const core::rectangle& r,
                          bool pushed, bool hilite, bool focused) {
      if (pushed && hilite) {
        graph.copy(draw::frame_image(r, osx::get_pressed_frame(), 4), r.top_left());
      } else if (focused) {
        graph.copy(draw::frame_image(r, osx::get_focused_frame(), 4), r.top_left());
      } else {
        graph.copy(draw::frame_image(r, osx::get_frame(), 4), r.top_left());
      }
    }

    template<>
    void drop_down_t<look_and_feel_t::osx> (draw::graphics& graph,
                                            const core::rectangle& r,
                                            const core::button_state::is& state) {
      drop_down_frame(graph, r, state.pushed(), state.hilited(), state.focused());
    }

    void drop_down (draw::graphics& graph,
                    const core::rectangle& area,
                    const core::button_state::is& state) {
      drop_down_t<>(graph, area, state);
    }

  } // look

} // gui
