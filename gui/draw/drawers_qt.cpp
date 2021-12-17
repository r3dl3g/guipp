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

#ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <QtGui/qpainterpath.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/arc_coords.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/use.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace draw {

    void line::operator() (graphics& g, const pen& p) const {
      const auto pw = p.os_size();

      if (from == to) {
        if (pw < 2) {
          Use<pen> pn(g, p);
          g.os()->drawPoint(from.os(g.context()));
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);

          const auto off = pw / 2;
          const auto x = from.os_x(g.context()) + off;
          const auto y = from.os_y(g.context()) + off;
          g.os()->drawRect(x - off, y - off, pw - off, pw - off);
        }
      } else {
        Use<pen> pn(g, p);

        const auto x0 = from.os_x(g.context());
        const auto y0 = from.os_y(g.context());
        const auto x1 = to.os_x(g.context());
        const auto y1 = to.os_y(g.context());

        g.os()->drawLine(x0, y0, x1, y1);
//#if defined(GUIPP_BUILD_FOR_MOBILE)
//#else

//        //                  0, 1, 2, 3, 4, 5, 6, 7, 8
//        // const offs[] =  {0, 0, 1, 1, 2, 2, 3, 3, 4};
//        // const offs2[] = {0, 0, 1, 2, 2, 3, 3, 4, 4};

//        const auto x0off = (x1 < x0) && (y1 != y0) && (pw > 2) ? (pw + 1) / 2 : pw / 2;
//        const auto y0off = (y1 < y0) && (x1 != x0) && (pw > 2) ? (pw + 1) / 2 : pw / 2;
//        const auto x1off = (x1 > x0) && (y1 != y0) && (pw > 2) ? (pw + 1) / 2 : pw / 2;
//        const auto y1off = (y1 > y0) && (x1 != x0) && (pw > 2) ? (pw + 1) / 2 : pw / 2;

//        g.os()->drawLine(x0 + x0off, y0 + y0off, x1 + x1off, y1 + y1off);
//#endif // GUIPP_BUILD_FOR_MOBILE
      }
    }

    // --------------------------------------------------------------------------
    constexpr int pen_offsets_tl (int i) {
      return i/2;
    }

    constexpr int pen_offsets_br (int i) {
      return -pen_offsets_tl(i - 1);
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (graphics& g,
                                const brush& b,
                                const pen& p) const {
      const os::rectangle r = rect.os(g.context());
      const auto pw = p.os_size();
      const auto off = pw / 2;
      Use<brush> ubr(g, b);
      Use<pen> pn(g, p);
      if ((r.width() > pw) && (r.height() > pw)) {
        g.os()->fillRect(r.x() + off, r.y() + off, r.width() - pw, r.height() - pw, b.color());
        g.os()->drawRect(r.x() + off, r.y() + off, r.width() - pw, r.height() - pw);
      } else if ((r.width() > 1) && (r.height() > 1)) {
        g.os()->fillRect(r.x(), r.y(), r.width(), r.height(), p.color());
      } else if ((1 == r.width()) && (1 == r.height())) {
        g.os()->drawPoint(r.x() + off, r.y() + off);
      }
    }

    void rectangle::operator() (graphics& g,
                                const pen& p) const {
      os::rectangle r = rect.os(g.context());
      const auto pw = p.os_size();
      const auto off = pw / 2;
      Use<pen> pn(g, p);
      Use<brush> ubr(g, brush::invisible);
      if ((r.width() > pw) && (r.height() > pw)) {
        g.os()->drawRect(r.x() + off, r.y() + off, r.width() - pw, r.height() - pw);
      } else if ((r.width() > 1) || (r.height() > 1)) {
        g.os()->fillRect(r.x(), r.y(), r.width(), r.height(), p.color());
      } else if ((1 == r.width()) && (1 == r.height())) {
        g.os()->drawPoint(r.x() + off, r.y() + off);
      }
    }

    void rectangle::operator() (graphics& g,
                                const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      os::rectangle r = rect.os(g.context());
      Use<pen> upn(g, p);
      Use<brush> ubr(g, b);
      const auto pw = p.os_size();
      const auto off = pw / 2;
      r.adjust(off, off, -1, -1);
      if ((0 == r.width()) && (0 == r.height())) {
        g.os()->drawPoint(r.x(), r.y());
      } else {
        g.os()->drawEllipse(r);
      }
    }

    void ellipse::operator() (graphics& g,
                              const pen& p) const {
      operator()(g, brush::invisible, p);
    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (graphics& g,
                                      const pen& p) const {
      operator()(g, brush::invisible, p);
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b) const {
      operator()(g, b, b.color());
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      os::rectangle r = rect.os(g.context());
      Use<brush> br(g, b);
      Use<pen> pn(g, p);

      const auto pw = p.os_size();
      const auto off = pw / 2;
      r.adjust(off, off, -1, -1);
      if ((0 == r.width()) && (0 == r.height())) {
        g.os()->drawPoint(r.x(), r.y());
      } else {
        g.os()->drawRoundedRect(r, radius.os_width(), radius.os_height());
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void draw_arc<arc_type::pie> (graphics& g, const arc_coords& c, const pen& p) {
      Use<pen> pn(g, p);

      QRectF r(c.x, c.y, c.w, c.h);
      if (c.full()) {
        g.os()->drawEllipse(r);
      } else {
        g.os()->drawPie(r, c.start.os(), (c.end - c.start).os());
      }
    }

    template<>
    void draw_arc<arc_type::arc> (graphics& g, const arc_coords& c, const pen& p) {
      Use<pen> pn(g, p);

      QRectF r(c.x, c.y, c.w, c.h);
      if (c.full()) {
        g.os()->drawEllipse(r);
      } else {
        g.os()->drawArc(r, c.start.os(), (c.end - c.start).os());
      }
    }

    template<>
    void fill_arc<arc_type::pie> (graphics& g, const arc_coords& c, const brush& b) {
      Use<brush> br(g, b);

      QRectF r(c.x, c.y, c.w, c.h);
      if (c.full()) {
        g.os()->drawEllipse(r);
      } else {
        g.os()->drawPie(r, c.start.os(), (c.end - c.start).os());
      }
    }

    template<>
    void fill_arc<arc_type::arc> (graphics& g, const arc_coords& c, const brush& b) {
      Use<brush> br(g, b);

      QRectF r(c.x, c.y, c.w, c.h);
      if (c.full()) {
        g.os()->drawEllipse(r);
      } else {
        g.os()->drawPie(r, c.start.os(), (c.end - c.start).os());
      }
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      auto pts = convert(g);
      g.os()->drawPolyline(pts.data(), (int)pts.size());
    }

    void polyline::operator() (graphics& g,
                              const pen& p) const {
      operator()(g, brush::invisible, p);
    }

    void polyline::operator() (graphics& g,
                              const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void polygon::operator() (graphics& g,
                               const brush& b,
                               const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      auto pts = convert(g);
      g.os()->drawPolygon(pts.data(), (int)pts.size());
    }

    void polygon::operator() (graphics& g,
                               const pen& p) const {
      operator()(g, brush::invisible, p);
    }

    void polygon::operator() (graphics& g,
                               const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (graphics& g,
                               const font& f,
                               os::color c) const {
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      g.os()->drawText(rect.os(g.context()), static_cast<int>(origin), QString::fromStdString(str));
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (graphics& g,
                                   const font& f,
                                   os::color) const {
      rect = core::rectangle(QFontMetrics(f).boundingRect(rect.os(g.context()), static_cast<int>(origin), QString::fromStdString(str)), g.context());
    }

    // --------------------------------------------------------------------------
    void text::operator() (graphics& g,
                           const font& f,
                           os::color c) const {
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      const QString t = QString::fromStdString(str);

      os::rectangle area = g.os()->viewport();
      os::rectangle r(pos.os(g.context()), area.bottomRight());

      if (!origin_is_left(origin) || !origin_is_top(origin)) {

        if (origin_is_h_center(origin)) {
          r.setLeft(pos.os_x(g.context()) - area.width());
          r.setRight(pos.os_x(g.context()) + area.width());
        } else if (origin_is_right(origin)) {
          r.setLeft(area.left());
          r.setRight(pos.os_x(g.context()));
        }

        if (origin_is_v_center(origin)) {
          r.setTop(pos.os_y(g.context()) - area.height());
          r.setBottom(pos.os_y(g.context()) + area.height());
        } else if (origin_is_bottom(origin)) {
          r.setTop(area.top());
          r.setBottom(pos.os_y(g.context()));
        }
      }

      g.os()->drawText(r, static_cast<int>(origin), t);
    }

  }

}
#endif // GUIPP_QT
