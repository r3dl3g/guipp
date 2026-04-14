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

#ifdef GUIPP_JS


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

using namespace emscripten;

namespace gui {

  // --------------------------------------------------------------------------
  namespace draw {

    void line::operator() (graphics& g, const pen& p) const {
      Use<pen> pn(g, p);

      const auto x1 = from.os_x(g.context());
      const auto y1 = from.os_y(g.context());
      const auto x2 = to.os_x(g.context());
      const auto y2 = to.os_y(g.context());

      g.os().call<void>("beginPath");
      g.os().call<void>("moveTo", x1, y1);
      g.os().call<void>("lineTo", x2, y2);
      g.os().call<void>("stroke");
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (graphics& g,
                                const brush& b,
                                const pen& p) const {
      Use<brush> ubr(g, b);
      Use<pen> pn(g, p);

      const os::rectangle r = rect.os(g.context());
      g.os().call<void>("fillRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r));
      g.os().call<void>("strokeRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r));
    }

    void rectangle::operator() (graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);

      const os::rectangle r = rect.os(g.context());
      g.os().call<void>("strokeRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r));
    }

    void rectangle::operator() (graphics& g,
                                const brush& b) const {
      Use<brush> ubr(g, b);

      const os::rectangle r = rect.os(g.context());
      g.os().call<void>("fillRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r));
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
      Use<pen> pn(g, p);

      const auto c = rect.center().os(g.context());
      const auto r = (rect.size() / 2).os();

      g.os().call<void>("beginPath");
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 0, 2 * M_PI);
      g.os().call<void>("stroke");
    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      Use<brush> ubr(g, b);
      const auto c = rect.center().os(g.context());
      const auto r = (rect.size() / 2).os();

      g.os().call<void>("beginPath");
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 0, 2 * M_PI);
      g.os().call<void>("fill");
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      const os::rectangle r = rect.os(g.context());
      int rii = (radius.os_width() + radius.os_height()) / 2;

      g.os().call<void>("beginPath");
      g.os().call<void>("roundRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r), rii);
      g.os().call<void>("stroke");
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b) const {
      Use<brush> ubr(g, b);
      const os::rectangle r = rect.os(g.context());
      int rii = (radius.os_width() + radius.os_height()) / 2;

      g.os().call<void>("beginPath");
      g.os().call<void>("roundRect", os::get_x(r), os::get_y(r), os::get_width(r), os::get_height(r), rii);
      g.os().call<void>("fill");
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      operator()(g, b);
      operator()(g, p);
    }

    // --------------------------------------------------------------------------
    template<>
    void draw_arc<arc_type::pie> (graphics& g, const arc_coords& coord, const pen& p) {
      Use<pen> pn(g, p);

      auto c = coord.center.os(g.context());
      auto r = coord.radius.os();

      g.os().call<void>("beginPath");
      g.os().call<void>("moveTo", c.x, c.y);
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 2 * M_PI - coord.end.rad(), 2 * M_PI - coord.start.rad());
      g.os().call<void>("lineTo", c.x, c.y);
      g.os().call<void>("stroke");
    }

    template<>
    void draw_arc<arc_type::arc> (graphics& g, const arc_coords& coord, const pen& p) {
      Use<pen> pn(g, p);

      auto c = coord.center.os(g.context());
      auto r = coord.radius.os();

      g.os().call<void>("beginPath");
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 2 * M_PI - coord.end.rad(), 2 * M_PI - coord.start.rad());
      g.os().call<void>("stroke");
    }

    template<>
    void fill_arc<arc_type::pie> (graphics& g, const arc_coords& coord, const brush& b) {
      Use<brush> br(g, b);

      auto c = coord.center.os(g.context());
      auto r = coord.radius.os();

      g.os().call<void>("beginPath");
      g.os().call<void>("moveTo", c.x, c.y);
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 2 * M_PI - coord.end.rad(), 2 * M_PI - coord.start.rad());
      g.os().call<void>("lineTo", c.x, c.y);
      g.os().call<void>("fill");
    }

    template<>
    void fill_arc<arc_type::arc> (graphics& g, const arc_coords& coord, const brush& b) {
      Use<brush> br(g, b);

      auto c = coord.center.os(g.context());
      auto r = coord.radius.os();

      g.os().call<void>("beginPath");
      g.os().call<void>("ellipse", c.x, c.y, r.cx, r.cy, 0, 2 * M_PI - coord.end.rad(), 2 * M_PI - coord.start.rad());
      g.os().call<void>("fill");
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
      Use<pen> pn(g, p);
      auto pts = convert(g);
      bool first = true;

      g.os().call<void>("beginPath");

      for (const auto& pt : pts) {
        if (first) {
          first = false;
          g.os().call<void>("moveTo", pt.x, pt.y);
        } else {
          g.os().call<void>("lineTo", pt.x, pt.y);
        }
      }
    
      g.os().call<void>("stroke");
    }

    void polyline::operator() (graphics& g,
                               const brush& b) const {
      Use<brush> br(g, b);
      auto pts = convert(g);
      bool first = true;

      g.os().call<void>("beginPath");

      for (const auto& pt : pts) {
        if (first) {
          first = false;
          g.os().call<void>("moveTo", pt.x, pt.y);
        } else {
          g.os().call<void>("lineTo", pt.x, pt.y);
        }
      }
    
      g.os().call<void>("fill");
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
      Use<pen> pn(g, p);
      auto pts = convert(g, true);
      bool first = true;

      g.os().call<void>("beginPath");

      for (const auto& pt : pts) {
        if (first) {
          first = false;
          g.os().call<void>("moveTo", pt.x, pt.y);
        } else {
          g.os().call<void>("lineTo", pt.x, pt.y);
        }
      }
      g.os().call<void>("lineTo", pts[0].x, pts[0].y);
    
      g.os().call<void>("stroke");
    }

    void polygon::operator() (graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      auto pts = convert(g, false);
      bool first = true;

      g.os().call<void>("beginPath");

      for (const auto& pt : pts) {
        if (first) {
          first = false;
          g.os().call<void>("moveTo", pt.x, pt.y);
        } else {
          g.os().call<void>("lineTo", pt.x, pt.y);
        }
      }
      g.os().call<void>("lineTo", pts[0].x, pts[0].y);
    
      g.os().call<void>("fill");
    }

    void set_h_alignment (graphics& g, const text_origin_t origin) {
      if (origin_is_left(origin)) {
        g.os().set("textAlign", "left");
      } else if (origin_is_right(origin)) {
        g.os().set("textAlign", "right");
      } else if (origin_is_h_center(origin)) {
        g.os().set("textAlign", "center");
      }
    }

    void set_v_alignment (graphics& g, const text_origin_t origin) {
      if (origin_is_top(origin)) {
        g.os().set("textBaseline", "top");
      } else if (origin_is_bottom(origin)) {
        g.os().set("textBaseline", "alphabetic");
      } else if (origin_is_v_center(origin)) {
        g.os().set("textBaseline", "middle");
      }
    }

    void set_alignment (graphics& g, const text_origin_t origin) {
      set_h_alignment(g, origin);
      set_v_alignment(g, origin);
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (graphics& g,
                               const font& f,
                               os::color c) const {
      if (str.empty()) {
        return;
      }

      Use<font> fn(g, f);
      Use<brush> pn(g, c);

      core::rectangle r = rect;
      bounding_box(str, r, origin)(g, f, c);
      
      std::vector<std::string> lines = util::string::split<'\n'>(str);
      if (lines.empty()) {
        return;
      }

      int x = r.os_x(g.context());
      int y = r.os_y(g.context());
      auto line_height = r.os_height() / lines.size();

      if (origin_is_right(origin)) {
        x = r.os_x2(g.context());
      } else if (origin_is_h_center(origin)) {
        x += r.os_width() / 2;
      }

      set_h_alignment(g, origin);
      g.os().set("textBaseline", "top");
      ++y;

      for (const auto& l : lines) {
        g.os().call<void>("fillText", l, x, y);
        y += line_height;
      }

    }

    std::pair<double, double> measure_text (emscripten::val id, const std::string& t) {
        auto metric = id.call<val>("measureText", t);
        double width = metric["width"].as<double>();
        double height = metric["fontBoundingBoxAscent"].as<double>()
                      + metric["fontBoundingBoxDescent"].as<double>();
        return {width, height};
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (graphics& g,
                                   const font& f,
                                   os::color) const {
      if (str.empty()) {
        rect.set_size(core::size::zero);
        return;
      }

      Use<font> fn(g, f);

      double h = 0;
      double w = 0;
      // double line_height = 0;
      if (line_handling_is_singleline(origin)) {
        std::tie(w, h) = measure_text(g.os(), str);
        // line_height = h;
      } else {
        std::vector<std::string> lines = util::string::split<'\n'>(str);
        for (const std::string& l : lines) {
          auto metric = measure_text(g.os(), l);
          w = std::max(w, metric.first);
          h += metric.second;
        }
      }

      if (origin_is_right(origin)) {
        rect.set_horizontal(rect.x2() - w, w);
      } else if (origin_is_h_center(origin)) {
        rect.set_horizontal(rect.center_x() - w / 2, w);
      } else {
        rect.width(w);
      }
      if (origin_is_bottom(origin)) {
        rect.set_vertical(rect.y2() - h, h);
      } else if (origin_is_v_center(origin)) {
        rect.set_vertical(rect.center_y() - h / 2, h);
      } else {
        rect.height(h);
      }
      
    }

    // --------------------------------------------------------------------------
    void text::operator() (graphics& g,
                           const font& f,
                           os::color c) const {
      if (str.empty()) {
        return;
      }

      Use<font> fn(g, f);
      Use<brush> pn(g, c);

      set_alignment(g, origin);

      int x = pos.os_x(g.context());
      int y = pos.os_y(g.context());

      if (line_handling_is_singleline(origin)) {
        g.os().call<void>("fillText", str, x, y);
      } else {
        core::rectangle r = {pos, core::size::zero};
        bounding_box(str, r, origin)(g, f, c);

        std::vector<std::string> lines = util::string::split<'\n'>(str);
        if (lines.empty()) {
          return;
        }
        auto line_height = r.os_height() / lines.size();

        if (origin_is_bottom(origin)) {
          y -= r.os_height() - line_height;
        } else if (origin_is_v_center(origin)) {
          y -= (r.os_height() - line_height) / 2;
        }

        for (const std::string& l : lines) {
          g.os().call<void>("fillText", l, x, y);
          y += line_height;
        }

      }


    }

  }

}
#endif // GUIPP_JS
