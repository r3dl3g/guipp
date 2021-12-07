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
 * @brief     X11 specific drawing functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

//#define GUIPP_X11
#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <cmath>
#include <algorithm>
#include <cstring>

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

#define OPTIMIZE_DRAWxx

namespace gui {

  namespace draw {

    using namespace core::global;

#ifdef GUIPP_USE_XFT
    // --------------------------------------------------------------------------
    struct render_color : XRenderColor {
      explicit render_color (os::color c)
        : XRenderColor({
                         (unsigned short)(color::extract<color::part::red>(c) << 8),
                         (unsigned short)(color::extract<color::part::green>(c) << 8),
                         (unsigned short)(color::extract<color::part::blue>(c) << 8),
                         (unsigned short)((color::extract<color::part::alpha>(c) << 8) ^ 0xffff)
                       })
      {}
    };

    // --------------------------------------------------------------------------
    struct xft_color : public XftColor {
      xft_color (os::color c, XftDraw* xft)
        : xft(xft) {
        render_color xrcolor(c);
        XftColorAllocValue(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), &xrcolor, this);
      }

      ~xft_color () {
        XftColorFree(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), this);
      }

      XftDraw* xft;
    };
#endif // GUIPP_USE_XFT

    // --------------------------------------------------------------------------
    void line::operator() (graphics& g, const pen& p) const {
      gui::os::instance display = get_instance();
//      const short pw = p.os_size();
//      const short off = pw / 2;
      Use<pen> pn(g, p);

      const auto x0 = from.os_x(g.context());
      const auto y0 = from.os_y(g.context());
      const auto x1 = to.os_x(g.context());
      const auto y1 = to.os_y(g.context());

      XDrawLine(display, g, g, x0, y0, x1, y1);
//      XDrawLine(display, g, g, x0 + off, y0 + off, x1 + off, y1 + off);
//      if ((pw > 1) && (pw % 2 == 0)) {
//        XDrawPoint(display, g, g, x0, y0);
//        if ((x1 < x0) || (y1 < y0)) {
//          XDrawPoint(display, g, g, x1, y1);
//        }
//      }
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (graphics& g, const brush& b, const pen& p) const {
      gui::os::instance display = get_instance();
      const os::rectangle r = (rect - core::size::one).os(g.context());

#ifdef OPTIMIZE_DRAW
      const auto pw = p.os_size();
      const auto off = pw / 2;

      if ((r.width > pw) && (r.height > pw)) {
        Use<brush> br(g, b);
        XFillRectangle(display, g, g, r.x + pw, r.y + pw, r.width - pw * 2, r.height - pw * 2);
        Use<pen> pn(g, p);
        XDrawRectangle(display, g, g, r.x + off, r.y + off, r.width - pw, r.height - pw);
      } else if ((r.width > 1) && (r.height > 1)) {
        Use<brush> br(g, brush(p.color()));
        XFillRectangle(display, g, g, r.x, r.y, r.width, r.height);
      } else if ((1 == r.width) && (1 == r.height)) {
        Use<pen> pn(g, p);
        XDrawPoint(display, g, g, r.x + off, r.y + off);
      }
#else
      if (!is_transparent(b)) {
        Use<brush> br(g, b);
        XFillRectangle(display, g, g, r.x, r.y, r.width, r.height);
      }
      if (!is_transparent(p)) {
        Use<pen> pn(g, p);
        XDrawRectangle(display, g, g, r.x, r.y, r.width, r.height);
      }
#endif
    }

    void rectangle::operator() (graphics& g, const pen& p) const {
      const os::rectangle r = (rect - core::size::one).os(g.context());
#ifdef OPTIMIZE_DRAW
      const auto pw = p.os_size();
      if (pw == 1) {
        Use<pen> pn(g, p);
        XDrawRectangle(get_instance(), g, g, r.x, r.y, r.width - 1, r.height - 1);
      } else {
        const auto off = pw / 2;
        if ((r.width > pw) && (r.height > pw)) {
          Use<pen> pn(g, p);
          XDrawRectangle(get_instance(), g, g, r.x + off, r.y + off, r.width - pw, r.height - pw);
        } else if ((r.width > 1) && (r.height > 1)) {
          Use<brush> br(g, brush(p.color()));
          XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
        } else if ((1 == r.width) && (1 == r.height)) {
          Use<pen> pn(g, p);
          XDrawPoint(get_instance(), g, g, r.x + off, r.y + off);
        }
      }
#else
      Use<pen> pn(g, p);
      XDrawRectangle(get_instance(), g, g, r.x, r.y, r.width, r.height);
#endif
    }

    void rectangle::operator() (graphics& g, const brush& b) const {
      operator ()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    static const short int degree_90 = 90 * 64;
    static const short int degree_360 = 360 * 64;

    // --------------------------------------------------------------------------
    void ellipse::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      gui::os::instance display = get_instance();
      const auto pw = p.os_size();
      const auto off = pw/*(pw - 1)*/ / 2;
      const os::rectangle r = (rect - core::size::one).os(g.context());
      if ((r.width == 0) && (r.height == 0)) {
        if (!is_transparent(p)) {
          if (pw < 2) {
            Use<pen> pn(g, p);
            XDrawPoint(display, g, g, r.x + off, r.y + off);
          } else {
            Use<brush> br(g, brush(p.color()));
            XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
          }
        }
      } else {
        const auto soff = pw;//-(pw + 1) % 2;
        if (!is_transparent(b)) {
          Use<brush> br(g, b);
          XSetArcMode(display, g, ArcPieSlice);
          XFillArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
        }
        if (!is_transparent(p)) {
          Use<pen> pn(g, p);
          XSetArcMode(display, g, ArcChord);
          XDrawArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
        }
      }
    }

    void ellipse::operator() (graphics& g,
                              const pen& p) const {
      gui::os::instance display = get_instance();
      const auto pw = p.os_size();
      const auto off = pw/*(pw - 1)*/ / 2;
      const os::rectangle r = (rect - core::size::one).os(g.context());
      if ((r.width == 0) && (r.height == 0)) {
        if (pw < 2) {
          Use<pen> pn(g, p);
          XDrawPoint(display, g, g, r.x + off, r.y + off);
        } else {
          Use<brush> br(g, brush(p.color()));
          XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
        }
      } else {
        Use<pen> pn(g, p);
        XSetArcMode(display, g, ArcChord);
        const auto soff = pw;//-(pw + 1) % 2;
        XDrawArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
      }
    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      operator ()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    template<typename Arc, typename Line, typename Rectangle, typename Angle>
    void calc_arcs (const core::context& ctx,
                    const core::rectangle& rect,
                    const core::size& size,
                    std::array<Arc, 4>* arcs,
                    std::array<Line, 4>* segments,
                    std::array<Rectangle, 3>* rects,
                    Angle angle90,
                    os::size_type pen_size) {

      using namespace os;

      const auto sz = size.min(rect.size() / 2);
      const auto sz2 = sz * 2;
      const auto in = rect.shrinked(sz);
      const auto br = rect.x2y2() - sz2;

      const point_type x0 = rect.os_x(ctx);
      const point_type y0 = rect.os_y(ctx);
      const point_type x4 = rect.os_x2(ctx);
      const point_type y4 = rect.os_y2(ctx);
      const point_type x1 = in.os_x(ctx);
      const point_type y1 = in.os_y(ctx);
      const point_type x3 = in.os_x2(ctx);
      const point_type y3 = in.os_y2(ctx);
      const point_type x2 = br.os_x(ctx);
      const point_type y2 = br.os_y(ctx);

      const size_type w2 = sz2.os_width();
      const size_type h2 = sz2.os_height();

      if (arcs) {
        (*arcs)[0] = {x0, y0, w2, h2, angle90, angle90};
        (*arcs)[1] = {x2, y0, w2, h2, 0, angle90};
        (*arcs)[2] = {x0, y2, w2, h2, (Angle)(angle90 * 2), angle90};
        (*arcs)[3] = {x2, y2, w2, h2, (Angle)(angle90 * 3), angle90};
      }

      if (segments) {
        const int offs = 1 - (pen_size % 2);
        const point_type y0o = (point_type)(y0 + offs);
        const point_type x0o = (point_type)(x0 + offs);
        (*segments)[0] = {x1, y0o, x3, y0o};
        (*segments)[1] = {x4, y1, x4, y3};
        (*segments)[2] = {x3, y4, x1, y4};
        (*segments)[3] = {x0o, y3, x0o, y1};
      }

      if (rects) {
        (*rects)[0] = {x0, y1, rect.os_width(), in.os_height()};
        (*rects)[1] = {x1, y0, in.os_width(), sz.os_height()};
        (*rects)[2] = {x3, y1, in.os_width(), sz.os_height()};
      }
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcChord);

      std::array<XArc, 4> arcs{};
      std::array<XSegment, 4> segments{};

      calc_arcs<XArc, XSegment, XRectangle>(g.context(), rect - core::size::one, radius, &arcs, &segments, nullptr, degree_90, p.os_size());

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b) const {
      pen p(b.color());

      Use<brush> br(g, b);
      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs{};
      std::array<XRectangle, 3> rects{};
      calc_arcs<XArc, XSegment, XRectangle>(g.context(), rect - core::size::one, radius, &arcs, nullptr, &rects, degree_90, p.os_size());

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());

      XFillRectangles(display, g, g, rects.data(), (int)rects.size());
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      std::array<XArc, 4> arcs{};
      std::array<XSegment, 4> segments{};
      std::array<XRectangle, 3> rects{};
      calc_arcs<XArc, XSegment, XRectangle>(g.context(), rect - core::size::one, radius, &arcs, &segments, &rects, degree_90, p.os_size());

      gui::os::instance display = get_instance();
      if (!is_transparent(b)) {
        Use<brush> br(g, b);
        XSetArcMode(display, g, ArcPieSlice);

        XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
        XFillRectangles(display, g, g, rects.data(), (int)rects.size());
      }
      if (!is_transparent(p)) {
        Use<pen> pn(g, p);
        XSetArcMode(display, g, ArcChord);

        XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
        XDrawSegments(display, g, g, segments.data(), (int)segments.size());
      }
    }

    // --------------------------------------------------------------------------
    template<>
    void draw_arc<arc_type::arc> (graphics& g, const arc_coords& c, const pen& p) {
      Use<pen> pn(g, p);

      gui::os::instance display = get_instance();

      XSetArcMode(get_instance(), g, ArcPieSlice);
      XDrawArc(display, g, g, c.x, c.y, c.w, c.h, c.start.os(), (c.end - c.start).os());
    }

    template<>
    void draw_arc<arc_type::pie> (graphics& g, const arc_coords& c, const pen& p) {
      Use<pen> pn(g, p);

      gui::os::instance display = get_instance();

      XSetArcMode(get_instance(), g, ArcPieSlice);
      XDrawArc(display, g, g, c.x, c.y, c.w, c.h, c.start.os(), (c.end - c.start).os());

      if (!c.full()) {
        auto pt = c.calc_points();
        XDrawLines(display, g, g, pt.data(), pt.size(), CoordModeOrigin);
        XDrawPoint(display, g, g, pt[0].x, pt[0].y);
        XDrawPoint(display, g, g, pt[2].x, pt[2].y);
      }
    }

    template<>
    void fill_arc<arc_type::arc> (graphics& g, const arc_coords& c, const brush& b) {
      Use<brush> br(g, b);
      XSetArcMode(get_instance(), g, ArcChord);
      XFillArc(get_instance(), g, g, c.x, c.y, c.w, c.h, c.start.os(), (c.end - c.start).os());
    }

    template<>
    void fill_arc<arc_type::pie> (graphics& g, const arc_coords& c, const brush& b) {
      Use<brush> br(g, b);
      XSetArcMode(get_instance(), g, ArcPieSlice);
      XFillArc(get_instance(), g, g, c.x, c.y, c.w, c.h, c.start.os(), (c.end - c.start).os());
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (graphics& g,
                               const brush& b,
                               const pen& p) const {
      auto pts = convert(g);
      if (!is_transparent(b)) {
        Use<brush> br(g, b);
        XFillPolygon(get_instance(),
                     g,
                     g,
                     (XPoint*)pts.data(),
                     (int)pts.size(),
                     0,
                     CoordModeOrigin);
      }
      if (!is_transparent(p)) {
        Use<pen> pn(g, p);
        XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
      }
    }

    void polyline::operator() (graphics& g,
                               const pen& p) const {
      auto pts = convert(g);
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
    }

    void polyline::operator() (graphics& g,
                               const brush& b) const {
      auto pts = convert(g);
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   const_cast<XPoint*>(pts.data()),
                   (int)pts.size(),
                   0,
                   CoordModeOrigin);
      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
    }

    // --------------------------------------------------------------------------
    void polygon::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      auto pts = convert(g);
      if (!is_transparent(b)) {
        Use<brush> br(g, b);
        XFillPolygon(get_instance(),
                     g,
                     g,
                     (XPoint*)pts.data(),
                     (int)pts.size(),
                     0,
                     CoordModeOrigin);
      }
      if (!is_transparent(p)) {
        Use<pen> pn(g, p);
        XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
      }
    }

    void polygon::operator() (graphics& g,
                              const pen& p) const {
      auto pts = convert(g);
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
    }

    void polygon::operator() (graphics& g,
                              const brush& b) const {
      auto pts = convert(g);
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   const_cast<XPoint*>(pts.data()),
                   (int)pts.size(),
                   0,
                   CoordModeOrigin);
      pen p = pen(b.color()).with_os_size(1);
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(pts.data()), (int)pts.size(), CoordModeOrigin);
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (graphics& g,
                               const font& f,
                               os::color c) const {
      gui::os::instance display = get_instance();

      int px = rect.os_x(g.context());
      int py = rect.os_y(g.context());
#ifdef GUIPP_USE_XFT
      if (f.font_type()) {
        XGlyphInfo extents = {};
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);

        const auto height = f.font_type()->ascent;
        px += extents.x;
        py += height;

        if (origin_is_h_center(origin)) {
          px += (rect.os_width() - extents.width) / 2;
        } else if (origin_is_right(origin)) {
          px += rect.os_width() - extents.width;
        }

        if (origin_is_v_center(origin)) {
          py += (rect.os_height() - height) / 2;
        } else if (origin_is_bottom(origin)) {
          py += rect.os_height() - height;
        }

      } else {
        logging::error() << "font_type is zero!";
      }

      xft_color xftcolor(c, g);
//      clip clp(g, rect);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px, py,
                        (XftChar8*)str.c_str(), int(str.size()));
#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        logging::error() << "font_type is zero!";
      }

      int width = overall.width;
      int height = (ascent - descent);

//        LogDebug << "r.x():" << px << " r.y():" << py
//                 << " r.size:" << rect.size()
//                 << " o.w:" << width << " o.h:" << height
//                 << " asc:" << ascent << " des:" << descent;

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() + height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height();
      } else {
        py += height;
      }

      XDrawString(core::global::get_instance(), g, g, px, py, str.c_str(), int(str.size()));
#endif // GUIPP_USE_XFT
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (graphics& g,
                                   const font& f,
                                   os::color c) const {
      gui::os::instance display = get_instance();

#ifdef GUIPP_USE_XFT
      if (f.font_type()) {
        XGlyphInfo extents = {};
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        const auto height = f.font_type()->ascent;
        os::point_type px = rect.os_x(g.context()) + extents.x;
        os::point_type py = rect.os_y(g.context());

        if (origin_is_h_center(origin)) {
          px += (rect.os_width() - extents.width) / 2;
        } else if (origin_is_right(origin)) {
          px += rect.os_width() - extents.width;
        }

        if (origin_is_v_center(origin)) {
          py += (rect.os_height() - height) / 2;
        } else if (origin_is_bottom(origin)) {
          py += rect.os_height() - height;
        }

        rect.top_left(core::point(os::point{px, py}, g.context()));
        rect.set_size(core::size(os::size{extents.width, static_cast<os::size_type>(height)}));

      } else {
        logging::error() << "font_type is zero!";
      }

#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        logging::error() << "font_type is zero!";
      }

      int width = overall.width;
      int height = (ascent - descent);
      os::point_type px = rect.os_x(g.context());
      os::point_type py = rect.os_y(g.context());

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() + height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height();
      } else {
        py += height;
      }

      rect.top_left({core::point::type(px), core::point::type(py - overall.ascent)});
      rect.set_size({core::size::type(width), core::size::type(overall.ascent + overall.descent)});
#endif // GUIPP_USE_XFT
    }

    // --------------------------------------------------------------------------
    void text::operator() (graphics& g,
                           const font& f,
                           os::color c) const {
      gui::os::instance display = get_instance();

      int px = pos.os_x(g.context());
      int py = pos.os_y(g.context());
#ifdef GUIPP_USE_XFT
      if (f.font_type()) {
        XGlyphInfo extents = {};
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        const auto height = f.font_type()->ascent;
        px += extents.x;
        py += height;

        if (origin_is_h_center(origin)) {
          px -= extents.width / 2;
        } else if (origin_is_right(origin)) {
          px -= extents.width;
        }

        if (origin_is_v_center(origin)) {
          py -= height / 2;
        } else if (origin_is_bottom(origin)) {
          py -= height;
        }

      } else {
        logging::error() << "font_type is zero!";
      }

      xft_color xftcolor(c, g);
      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px, py,
                        (XftChar8*)str.c_str(), int(str.size()));
#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        logging::error() << "font_type is zero!";
      }

      if (origin_is_h_center(origin)) {
        px -= overall.width / 2;
      } else if (origin_is_right(origin)) {
        px -= overall.width;
      }
      int height = (overall.ascent - overall.descent);
      if (origin_is_v_center(origin)) {
        py += height / 2;
      } else if (origin_is_bottom(origin)) {
      } else {
        py += height;
      }

      XDrawString(display, g, g, px, py, str.c_str(), int(str.size()));
#endif // GUIPP_USE_XFT
    }
  }
}

#endif // GUIPP_X11
