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

    void line::operator() (graphics& g, const pen& p) const {
      const auto x1 = from.os_x(g.context());
      const auto y1 = from.os_y(g.context());
      const auto x2 = to.os_x(g.context());
      const auto y2 = to.os_y(g.context());

      if (p.size() > 1) {
        thickLineColor(g, x1, y1, x2, y2, p.size(), p.color());
      } else {
        lineColor(g, x1, y1, x2, y2, p.color());
      }
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (graphics& g,
                                const brush& b,
                                const pen& p) const {
      const auto r = rect.os(g.context());

      boxColor(g, r.x, r.y, r.x + r.w, r.y + r.h, b.color());
      rectangleColor(g, r.x, r.y, r.x + r.w, r.y + r.h, p.color());
    }

    void rectangle::operator() (graphics& g,
                                const pen& p) const {
      const auto r = rect.os(g.context());

      rectangleColor(g, r.x, r.y, r.x + r.w, r.y + r.h, p.color());
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
      const auto r = rect.os(g.context());

      filledEllipseColor(g, r.x, r.y, r.x + r.w, r.y + r.h, b.color());
      ellipseColor(g, r.x, r.y, r.x + r.w, r.y + r.h, p.color());
    }

    void ellipse::operator() (graphics& g,
                              const pen& p) const {
      const auto r = rect.os(g.context());

      ellipseColor(g, r.x, r.y, r.x + r.w, r.y + r.h, p.color());
    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      const auto r = rect.os(g.context());

      filledEllipseColor(g, r.x, r.y, r.x + r.w, r.y + r.h, b.color());
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
    void draw_arc<arc_type::pie> (graphics& g, const arc_coords& c, const pen& p) {
      int rx = c.w / 2;
      int ry = c.h / 2;
      int cx = c.x + rx;
      int cy = c.y + ry;

      int rad = (rx + ry) / 2;

      pieColor(g, cx, cy, rad, c.end.deg(), c.start.deg(), p.color());
    }

    template<>
    void draw_arc<arc_type::arc> (graphics& g, const arc_coords& c, const pen& p) {
      int rx = c.w / 2;
      int ry = c.h / 2;
      int cx = c.x + rx;
      int cy = c.y + ry;

      int rad = (rx + ry) / 2;

      arcColor(g, cx, cy, rad, c.end.deg(), c.start.deg(), p.color());
    }

    template<>
    void fill_arc<arc_type::pie> (graphics& g, const arc_coords& c, const brush& b) {
      int rx = c.w / 2;
      int ry = c.h / 2;
      int cx = c.x + rx;
      int cy = c.y + ry;

      int rad = (rx + ry) / 2;

      filledPieColor(g, cx, cy, rad, c.end.deg(), c.start.deg(), b.color());
    }

    template<>
    void fill_arc<arc_type::arc> (graphics& g, const arc_coords& c, const brush& b) {
      int rx = c.w / 2;
      int ry = c.h / 2;
      int cx = c.x + rx;
      int cy = c.y + ry;

      int rad = (rx + ry) / 2;

      arcColor(g, cx, cy, rad, c.end.deg(), c.start.deg(), b.color());
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
      auto pts = convert(g);
      
      bool first = true;
      os::point p0;
      for (const auto& pt : pts) {
        if (first) {
          first = false;
        } else {
          if (p.size() > 1) {
            thickLineColor(g, p0.x, p0.y, pt.x, pt.y, p.size(), p.color());
          } else {
            lineColor(g, p0.x, p0.y, pt.x, pt.y, p.color());
          }
        }
        p0 = pt;
      }

    }

    void polyline::operator() (graphics& g,
                               const brush& b) const {
      auto pts = convert(g);
      const int cnt = pts.size();
      std::vector<Sint16> vx(cnt);
      std::vector<Sint16> vy(cnt);

      for (int i = 0; i < cnt; ++i) {
        vx[i] = pts[i].x;
        vy[i] = pts[i].y;
      }

      filledPolygonColor(g, vx.data(), vy.data(), cnt, b.color());
    }

    // --------------------------------------------------------------------------
    void polygon::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      auto pts = convert(g);
      const int cnt = pts.size() + 1;
      std::vector<Sint16> vx(cnt);
      std::vector<Sint16> vy(cnt);

      for (int i = 0; i < cnt - 1; ++i) {
        vx[i] = pts[i].x;
        vy[i] = pts[i].y;
      }
      vx[cnt - 1] = pts[0].x;
      vy[cnt - 1] = pts[0].y;

      filledPolygonColor(g, vx.data(), vy.data(), cnt, b.color());
      polygonColor(g, vx.data(), vy.data(), cnt, p.color());
    }

    void polygon::operator() (graphics& g,
                              const pen& p) const {
      auto pts = convert(g);
      const int cnt = pts.size() + 1;
      std::vector<Sint16> vx(cnt);
      std::vector<Sint16> vy(cnt);

      for (int i = 0; i < cnt - 1; ++i) {
        vx[i] = pts[i].x;
        vy[i] = pts[i].y;
      }
      vx[cnt - 1] = pts[0].x;
      vy[cnt - 1] = pts[0].y;

      polygonColor(g, vx.data(), vy.data(), cnt, p.color());
    }

    void polygon::operator() (graphics& g,
                              const brush& b) const {
      auto pts = convert(g);
      const int cnt = pts.size() + 1;
      std::vector<Sint16> vx(cnt);
      std::vector<Sint16> vy(cnt);

      for (int i = 0; i < cnt - 1; ++i) {
        vx[i] = pts[i].x;
        vy[i] = pts[i].y;
      }
      vx[cnt - 1] = pts[0].x;
      vy[cnt - 1] = pts[0].y;

      filledPolygonColor(g, vx.data(), vy.data(), cnt, b.color());
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
