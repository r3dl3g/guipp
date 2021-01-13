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
#include <gui/ctrl/look/drop_down.h>
#include <gui/ctrl/look/button.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace image_data {

#   include <gui/ctrl/look/res/osx_dropdown_button.h>

  } // namespace image_data

  namespace detail {

    template<typename T, size_t N>
    inline std::string make_string (T(&t)[N]) {
      return std::string((const char*)t, N);
    }

    draw::rgbmap build_rgb_image (const std::string& data);
    template<typename T>
    T upscale (const T& img);

    const draw::rgbmap& get_osx_dropdown_button () {
      static draw::rgbmap img = upscale(build_rgb_image(make_string(image_data::osx_dropdown_button)));
      return img;
    }

  }

  namespace look {

    template<>
    void drop_down_item_t<look_and_feel_t::metal> (const draw::graphics& g,
                                                   const core::rectangle& r,
                                                   const draw::brush&,
                                                   const std::string& t,
                                                   const ctrl::item_state& s) {
      os::color col = color::black;
      if (ctrl::item_state::selected == s) {
        g.fill(draw::rectangle(r), color::highLightColor());
        col = color::highLightTextColor();
      } else if (ctrl::item_state::hilited == s) {
        g.fill(draw::rectangle(r), color::menuColorHighlight());
      }

      g.text(draw::text_box(t, r, text_origin_t::vcenter_left), draw::font::system(), col);
    }

    template<>
    void drop_down_item_t<look_and_feel_t::w95> (const draw::graphics& g,
                                                 const core::rectangle& r,
                                                 const draw::brush& b,
                                                 const std::string& t,
                                                 const ctrl::item_state& s) {
      os::color col = color::black;
      if (ctrl::item_state::selected == s) {
        g.fill(draw::rectangle(r), color::highLightColor());
        col = color::highLightTextColor();
      } else if (ctrl::item_state::hilited == s) {
        g.fill(draw::rectangle(r), color::menuColorHighlight());
      } else {
        g.fill(draw::rectangle(r), b);
      }

      g.text(draw::text_box(t, r, text_origin_t::vcenter_left), draw::font::system(), col);
    }

    template<>
    void drop_down_item_t<look_and_feel_t::w10> (const draw::graphics& g,
                                                 const core::rectangle& r,
                                                 const draw::brush& b,
                                                 const std::string& t,
                                                 const ctrl::item_state& s) {
      drop_down_item_t<look_and_feel_t::w95>(g, r, b, t, s);
    }

    template<>
    void drop_down_item_t<look_and_feel_t::osx> (const draw::graphics& g,
                                                 const core::rectangle& r,
                                                 const draw::brush& b,
                                                 const std::string& t,
                                                 const ctrl::item_state& s) {
      drop_down_item_t<look_and_feel_t::metal>(g, r, b, t, s);
    }

    void drop_down_item (const draw::graphics& g,
                         const core::rectangle& r,
                         const draw::brush& b,
                         const std::string& t,
                         const ctrl::item_state& s) {
      drop_down_item_t<>(g, r, b, t, s);
    }

    std::vector<core::point> get_button_poly (const core::rectangle& area, bool is_open) {
      core::rectangle r = area.shrinked(area.size() / 3);
      if (!r.empty()) {
        if (is_open) {
          return {r.bottom_right(), {r.center_x(), r.y()}, r.bottom_left()};
        } else {
          return {r.top_left(), {r.center_x(), r.y2()}, r.top_right()};
        }
      }
      return {};
    }

    template<>
    void drop_down_button_t<look_and_feel_t::metal> (const draw::graphics& graph,
                                                     const core::rectangle& area,
                                                     const core::button_state::is& state,
                                                     bool is_open) {
      graph.fill(draw::polygon(get_button_poly(area, is_open)), color::black);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::w95> (const draw::graphics& graph,
                                                   const core::rectangle& area,
                                                   const core::button_state::is& state,
                                                   bool is_open) {
      look::button_frame<look_and_feel_t::w95>(graph, area, state);
      graph.fill(draw::polygon(get_button_poly(area, is_open)), color::black);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::w10> (const draw::graphics& graph,
                                                   const core::rectangle& r,
                                                   const core::button_state::is& state,
                                                   bool is_open) {
      graph.frame(draw::polyline(get_button_poly(r, is_open)), color::black);
    }

    template<>
    void drop_down_button_t<look_and_feel_t::osx> (const draw::graphics& graph,
                                                   const core::rectangle& area,
                                                   const core::button_state::is&,
                                                   bool) {
      const auto& img = detail::get_osx_dropdown_button();
      graph.fill(draw::image<decltype(img)>(img, area, text_origin_t::vcenter_right), color::buttonColor());
    }

    void drop_down_button (const draw::graphics& graph,
                           const core::rectangle& area,
                           const core::button_state::is& state,
                           bool is_open) {
      drop_down_button_t<>(graph, area, state, is_open);
    }

    template<>
    void drop_down_t<look_and_feel_t::metal> (const draw::graphics& graph,
                                              const core::rectangle& r,
                                              const core::button_state::is& state) {
      button_frame<look_and_feel_t::metal>(graph, r, state);
    }

    template<>
    void drop_down_t<look_and_feel_t::w95> (const draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& state) {
      draw::frame::sunken_deep_relief(graph, area);
      if (state.focused()) {
        draw::frame::dots(graph, area);
      }
    }

    template<>
    void drop_down_t<look_and_feel_t::w10> (const draw::graphics& graph,
                                            const core::rectangle& area,
                                            const core::button_state::is& state) {
      button_frame<look_and_feel_t::w10>(graph, area, state);
    }

    template<>
    void drop_down_t<look_and_feel_t::osx> (const draw::graphics& graph,
                                            const core::rectangle& r,
                                            const core::button_state::is& state) {
      button_frame<look_and_feel_t::osx>(graph, r, state);
    }

    void drop_down (const draw::graphics& graph,
                    const core::rectangle& area,
                    const core::button_state::is& state) {
      drop_down_t<>(graph, area, state);
    }

  } // look

} // gui
