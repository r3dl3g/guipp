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
 * @brief     Qt specific drawing functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#ifdef GUIPP_SDL


// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <cmath>
#include <logging/logger.h>
#include <util/string_util.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/graphics.h"
#include "gui/draw/drawers.h"
#include "gui/draw/arc_coords.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/font.h"
#include "gui/draw/use.h"

#include "SDL2/SDL2_gfxPrimitives.h"


namespace gui {

  // --------------------------------------------------------------------------
  namespace draw {

    void thickEllipseColor (SDL_Renderer* renderer, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint8 width, Uint32 color) {
      for (Uint8 i = 0; i < width; ++i) {
        auto delta = (i >> 1) ^ -(i & 1);
        ellipseColor(renderer, x, y, rx + delta, ry + delta, color);
      }
    }

    void myThickLineColor(SDL_Renderer* renderer, const os::point& p0, const os::point& p1, Uint8 width, Uint32 color) {
      thickLineColor(renderer, p0.x, p0.y, p1.x, p1.y, width, color);
    }

    void thickPolylineColor (SDL_Renderer* renderer, const std::vector<os::point>& pts, Uint8 width, Uint32 color) {
      bool first = true;
      os::point p0;
      for (const auto& pt : pts) {
        if (first) {
          first = false;
        } else {
          myThickLineColor(renderer, p0, pt, width, color);
        }
        p0 = pt;
      }
    }

    void thickPolygoneColor (SDL_Renderer* renderer, const std::vector<os::point>& pts, Uint8 width, Uint32 color) {
      bool first = true;
      os::point p0, p1;
      for (const auto& pt : pts) {
        if (first) {
          p0 = pt;
          first = false;
        } else {
          myThickLineColor(renderer, p1, pt, width, color);
        }
        p1 = pt;
      }
      myThickLineColor(renderer, p1, p0, width, color);
    }

    int filledPolygonColor (SDL_Renderer* renderer, const std::vector<os::point>& pts, Uint32 color) {
        const int cnt = pts.size();
        std::vector<Sint16> vx(cnt);
        std::vector<Sint16> vy(cnt);

        for (int i = 0; i < cnt - 1; ++i) {
          vx[i] = pts[i].x;
          vy[i] = pts[i].y;
        }
        return filledPolygonColor(renderer, vx.data(), vy.data(), cnt, color);
    }

    // --------------------------------------------------------------------------
    void line::operator() (graphics& g, const pen& p) const {
      const auto p1 = from.os(g.context());
      const auto p2 = to.os(g.context());

      myThickLineColor(g, p1, p2, p.os_size(), p.color());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (graphics& g,
                                const brush& b,
                                const pen& p) const {
      operator()(g, b);
      operator()(g, p);
    }

    void rectangle::operator() (graphics& g,
                                const pen& p) const {
      const auto r = rect.os(g.context());

      thickLineColor(g, r.x, r.y, r.x + r.w, r.y, p.os_size(), p.color());
      thickLineColor(g, r.x, r.y + r.h, r.x + r.w, r.y + r.h, p.os_size(), p.color());
      thickLineColor(g, r.x, r.y, r.x, r.y + r.h, p.os_size(), p.color());
      thickLineColor(g, r.x + r.w, r.y, r.x + r.w, r.y + r.h, p.os_size(), p.color());
    }

    void rectangle::operator() (graphics& g,
                                const brush& b) const {
      const auto r = rect.os(g.context());

      boxColor(g, r.x, r.y, r.x + r.w, r.y + r.h, b.color());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      operator()(g, b);
      operator()(g, p);
    }

    void ellipse::operator() (graphics& g,
                              const pen& p) const {
      const auto c = rect.center().os(g.context());
      const auto r = (rect.size() / 2).os();

      thickEllipseColor(g, c.x, c.y, r.w, r.h, p.os_size(), p.color());
    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      const auto r = rect.os(g.context());

      auto w = r.w / 2;
      auto h = r.h / 2;
      filledEllipseColor(g, r.x + w, r.y + h, w, h, b.color());
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (graphics& g,
                                      const pen& p) const {
      const auto r = rect.os(g.context());

      int rad = (radius.os_width() + radius.os_height()) / 2;

      roundedRectangleColor(g, r.x, r.y, r.x + r.w, r.y + r.h, rad, p.color());
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b) const {
      const auto r = rect.os(g.context());

      int rad = (radius.os_width() + radius.os_height()) / 2;

      roundedBoxColor(g, r.x, r.y, r.x + r.w, r.y + r.h, rad, b.color());
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      const auto r = rect.os(g.context());

      int rad = (radius.os_width() + radius.os_height()) / 2;

      roundedBoxColor(g, r.x, r.y, r.x + r.w, r.y + r.h, rad, b.color());
      roundedRectangleColor(g, r.x, r.y, r.x + r.w, r.y + r.h, rad, p.color());
    }

    // --------------------------------------------------------------------------
    template<>
    void draw_arc<arc_type::pie> (graphics& g, const arc_coords& coord, const pen& p) {
      auto c = coord.center.os(g.context());
      
      if (coord.full()) {
        auto r = coord.radius.os();
        thickEllipseColor(g, c.x, c.y, r.w, r.h, p.os_size(), p.color());
      } else {
        auto pts = coord.calc_arc_os_points(g.context());
        pts.push_back(c);
        thickPolygoneColor(g, pts, p.os_size(), p.color());
      }
    }

    template<>
    void draw_arc<arc_type::arc> (graphics& g, const arc_coords& coord, const pen& p) {
      
      if (coord.full()) {
        auto c = coord.center.os(g.context());
        auto r = coord.radius.os();
        thickEllipseColor(g, c.x, c.y, r.w, r.h, p.os_size(), p.color());
      } else {
        thickPolylineColor(g, coord.calc_arc_os_points(g.context()), p.os_size(), p.color());
      }
    }

    template<>
    void fill_arc<arc_type::pie> (graphics& g, const arc_coords& coord, const brush& b) {

      if (coord.full()) {
        auto r = coord.get_area().os(g.context());
        filledEllipseColor(g, r.x, r.y, r.w, r.h, b.color());
      } else {
        auto pts = coord.calc_arc_os_points(g.context());
        pts.push_back(coord.center.os(g.context()));
        filledPolygonColor(g, pts, b.color());
      }
    }

    template<>
    void fill_arc<arc_type::arc> (graphics& g, const arc_coords& coord, const brush& b) {

      if (coord.full()) {
        auto r = coord.get_area().os(g.context());
        filledEllipseColor(g, r.x, r.y, r.w, r.h, b.color());
      } else {
        auto pts = coord.calc_arc_os_points(g.context());
        filledPolygonColor(g, pts, b.color());
      }
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (graphics& g,
                               const brush& b,
                               const pen& p) const {

      operator()(g, b);
      operator()(g, p);
    }

    void polyline::operator() (graphics& g,
                               const pen& p) const {
      thickPolylineColor(g, convert(g), p.os_size(), p.color());
    }

    void polyline::operator() (graphics& g,
                               const brush& b) const {
      filledPolygonColor(g, convert(g), b.color());
    }

    // --------------------------------------------------------------------------
    void polygon::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      operator()(g, b);
      operator()(g, p);
    }

    void polygon::operator() (graphics& g,
                              const pen& p) const {
      thickPolygoneColor(g, convert(g), p.os_size(), p.color());
    }

    void polygon::operator() (graphics& g,
                              const brush& b) const {
      filledPolygonColor(g, convert(g), b.color());
    }

    FC_AlignEnum get_h_alignment (const text_origin_t origin) {
      if (origin_is_left(origin)) {
        return FC_ALIGN_LEFT;
      } else if (origin_is_right(origin)) {
        return FC_ALIGN_RIGHT;
      } else if (origin_is_h_center(origin)) {
        return FC_ALIGN_CENTER;
      }
      return FC_ALIGN_LEFT;
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (graphics& g,
                               const font& f,
                               os::color c) const {
      if (str.empty()) {
        return;
      }

      core::rectangle r = rect;
      bounding_box(str, r, origin)(g, f, c);

      FC_AlignEnum align = get_h_alignment(origin);
      FC_Rect box = r.os(g.context());
      FC_DrawBoxAlign(f.font_type().get(), g.gc(), box, align, str.c_str());

    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (graphics& g,
                                   const font& f,
                                   os::color) const {
      if (str.empty()) {
        rect.set_size(core::size::zero);
        return;
      }

      FC_AlignEnum align = get_h_alignment(origin);
      FC_Rect box = rect.os(g.context());
      float scale = static_cast<float>(1.0 / core::global::get_scale_factor());
      box = FC_GetBounds(f.font_type().get(), box.x, box.y, align, {scale, scale}, str.c_str());

      if (origin_is_right(origin)) {
        rect.set_horizontal(rect.x2() - box.w, box.w);
      } else if (origin_is_h_center(origin)) {
        rect.set_horizontal(rect.center_x() - box.w / 2, box.w);
      } else {
        rect.width(box.w);
      }
      if (origin_is_bottom(origin)) {
        rect.set_vertical(rect.y2() - box.h, box.h);
      } else if (origin_is_v_center(origin)) {
        rect.set_vertical(rect.center_y() - box.h / 2, box.h);
      } else {
        rect.height(box.h);
      }
      
    }

    // --------------------------------------------------------------------------
    void text::operator() (graphics& g,
                           const font& f,
                           os::color c) const {
      if (str.empty()) {
        return;
      }

      core::rectangle r = {pos, core::size::zero};
      bounding_box(str, r, origin)(g, f, c);

      float px = pos.os_x(g.context());
      float py = pos.os_y(g.context());
      if (origin_is_bottom(origin)) {
        py = r.os_y(g.context());
      } else if (origin_is_v_center(origin)) {
        py = r.os_y(g.context());
      }
      FC_AlignEnum align = get_h_alignment(origin);
      FC_DrawAlign(f.font_type().get(), g.gc(), px, py, align, str.c_str());
    }

  } // namespace draw

} // namespace gui

#endif // GUIPP_SDL
