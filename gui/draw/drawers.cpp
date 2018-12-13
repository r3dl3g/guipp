/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window types
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <memory.h>
#include <array>
#include <functional>
#ifdef X11
# include <cmath>
# include <algorithm>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <base/ostreamfmt.h>
#ifdef WIN32
# include <base/string_util.h>
#endif // WIN32

#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/use.h>


namespace gui {

  namespace draw {

#ifdef WIN32
    brush null_brush((os::brush)GetStockObject(NULL_BRUSH));
    pen null_pen((os::pen)GetStockObject(NULL_PEN));

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      Use<pen> pn(g, p);
      MoveToEx(g, from.os_x(), from.os_y(), nullptr);
      LineTo(g, to.os_x(), to.os_y());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (const graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    // --------------------------------------------------------------------------
    arc::arc (const core::point& pos,
              unsigned int radius,
              float start_radius,
              float end_radius)
      : pos(pos)
      , radius(radius)
      , start_radius(start_radius)
      , end_radius(end_radius) {}

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      BeginPath(g);
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius - start_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius - start_radius);
      LineTo(g, pos.os_x(), pos.os_y());
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      BeginPath(g);
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius - start_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
    }

    void polygon::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    void polygon::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    void polygon::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (const graphics& g,
                               const font& f,
                               os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, TRANSPARENT);
      RECT Rect = rect;

      std::wstring wstr = basepp::string::utf8_to_utf16(str);

      unsigned int o = static_cast<unsigned int>(origin);
      if (basepp::bit_mask<unsigned int, DT_WORDBREAK | DT_VCENTER>::is_set(o)) {
        int h = DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o | DT_CALCRECT);
        Rect.left = rect.os_x();
        Rect.right = rect.os_x2();
        Rect.top = ((rect.size().os_height() - h) / 2);
        Rect.bottom = Rect.top + h;
      }
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   os::color) const {
      Use<font> fn(g, f);
      RECT r = rect;
      std::wstring wstr = basepp::string::utf8_to_utf16(str);
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &r, static_cast<unsigned int>(origin) | DT_CALCRECT);
      rect = core::rectangle(r);
      //auto sz = f.get_text_size(str);
      //if (rect.size() != sz) {
      //  LogDebug << "Text size differs: DT_CALCRECT:" << rect.size() << ", GetTextExtentPoint32:" << sz;
      //}
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, TRANSPARENT);
      int px = pos.os_x();
      int py = pos.os_y();
      unsigned int old_align = static_cast<unsigned int>(text_origin::top_left);
      std::wstring wstr = basepp::string::utf8_to_utf16(str);

      switch (origin) {
      case text_origin::top_left:
        old_align = SetTextAlign(g, TA_LEFT | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin::top_right:
        old_align = SetTextAlign(g, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin::top_hcenter:
        old_align = SetTextAlign(g, TA_CENTER | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin::bottom_left:
        old_align = SetTextAlign(g, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin::bottom_right:
        old_align = SetTextAlign(g, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin::bottom_hcenter:
        old_align = SetTextAlign(g, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin::vcenter_right: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_RIGHT | TA_NOUPDATECP);
        break;
      }
      case text_origin::vcenter_left: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_LEFT | TA_NOUPDATECP);
        break;
      }
      case text_origin::center: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_CENTER | TA_NOUPDATECP);
        break;
      }
      case text_origin::undefined:
        break;
      }
      TextOutW(g, px, py, wstr.c_str(), (int)wstr.size());
      SetTextAlign(g, old_align);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

#endif // WIN32

#ifdef X11
    using namespace core::global;

    // --------------------------------------------------------------------------
    struct render_color : XRenderColor {
      render_color (os::color c)
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

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLine(get_instance(), g, g, from.os_x(), from.os_y(), to.os_x(), to.os_y());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      os::instance display = get_instance();

      Use<brush> br(g, b);
      const core::point& pt = rect.top_left();
      core::size sz(rect.size());// - unscale(core::size::one));
      XFillRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
      Use<pen> pn(g, p);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size());// - unscale(core::size::one));
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size());// - unscale(core::size::one));
      XFillRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());

      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    static const int degree_0 = 0;
    static const int degree_90 = 90 * 64;
    static const int degree_180 = 180 * 64;
    static const int degree_270 = 270 * 64;
    static const int degree_360 = 360 * 64;

    // --------------------------------------------------------------------------
    void ellipse::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);

      pen p(b.color());
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    // --------------------------------------------------------------------------
    void calc_arcs (const core::rectangle& rect,
                    const core::size& size,
                    std::array<XArc, 4>* arcs,
                    std::array<XSegment, 4>* segments,
                    std::array<XRectangle, 3>* rects) {

      using namespace os;

      constexpr size_type two = size_type(2);

      const size_type w = std::min(size.os_width(), static_cast<size_type>(rect.os_width() / 2));
      const size_type h = std::min(size.os_height(), static_cast<size_type>(rect.os_height() / 2));

      const point_type x0 = rect.os_x();
      const point_type x3 = rect.os_x2();
      const point_type x1 = x0 + w;
      const point_type x2 = x3 - w;

      const point_type y0 = rect.os_y();
      const point_type y3 = rect.os_y2();
      const point_type y1 = y0 + h;
      const point_type y2 = y3 - h;

      const size_type w2 = w * two;
      const size_type h2 = h * two;

      if (arcs) {
        (*arcs)[0] = {x0, y0, w2, h2, degree_90, degree_90};
        (*arcs)[1] = {point_type(x3 - w2), y0, w2, h2, degree_0, degree_90};
        (*arcs)[2] = {x0, point_type(y3 - h2), w2, h2, degree_180, degree_90};
        (*arcs)[3] = {point_type(x3 - w2), point_type(y3 - h2), w2, h2, degree_270, degree_90};
      }

      if (segments) {
        (*segments)[0] = {x1, y0, x2, y0};
        (*segments)[1] = {x3, y1, x3, y2};
        (*segments)[2] = {x1, y3, x2, y3};
        (*segments)[3] = {x0, y1, x0, y2};
      }

      if (rects) {
        (*rects)[0] = {x0, y1, w, size_type(y2 - y1)};
        (*rects)[1] = {x2, y1, size_type(w + 1), size_type(y2 - y1)};
        (*rects)[2] = {x1, y0, size_type(x2 - x1), size_type(y3 - y0 + 1)};
      }
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (const graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcChord);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;

      calc_arcs(rect - core::size::one, size, &arcs, &segments, nullptr);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect - core::size::one, size, &arcs, nullptr, &rects);

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      pen p(b.color());
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());

      XFillRectangles(display, g, g, rects.data(), (int)rects.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect - core::size::one, size, &arcs, &segments, &rects);

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      XFillRectangles(display, g, g, rects.data(), (int)rects.size());

      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    // --------------------------------------------------------------------------
    arc::arc (const core::point& pos,
              unsigned int radius,
              float startrad,
              float endrad)
      : pos(pos), radius(scale(radius)), start_radius(startrad), end_radius(endrad) {}

    void frame_arc (const graphics& g,
                    const pen& p,
                    const core::point& pos,
                    unsigned int radius,
                    float startrad,
                    float endrad) {

      os::point_type mx = pos.os_x();
      os::point_type my = pos.os_y();

      Use<pen> pn(g, p);
      int x0 = mx - radius;
      int y0 = my - radius;
      unsigned int sz = radius * 2;

      while (endrad < startrad) {
        endrad += 360;
      }

      XDrawArc(get_instance(), g, g, x0, y0, sz, sz, int(startrad * 64), int((endrad - startrad) * 64));

      int istart = int(startrad * 1000.0F) % 360000;
      int iend = int(endrad * 1000.0F) % 360000;
      if (istart != iend) {
        double start = M_PI * startrad / 180.0;
        double end = M_PI * endrad / 180.0;
        os::point pt[3];
        pt[0].x = short(mx + int(radius * cos(start)));
        pt[0].y = short(my - int(radius * sin(start)));
        pt[1].x = short(mx);
        pt[1].y = short(my);
        pt[2].x = short(mx + int(radius * cos(end)));
        pt[2].y = short(my - int(radius * sin(end)));
        XDrawLines(get_instance(), g, g, pt, 3, CoordModeOrigin);
      }
    }

    void fill_arc (const graphics& g,
                   const brush& b,
                   const core::point& pos,
                   unsigned int radius,
                   float startrad,
                   float endrad) {
      Use<brush> br(g, b);
      int x = pos.os_x() - radius;
      int y = pos.os_y() - radius;
      unsigned int sz = radius * 2;

      while (endrad < startrad) {
        endrad += 360;
      }

      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, x, y, sz, sz, int(startrad * 64), int((endrad - startrad) * 64));

      XDrawArc(get_instance(), g, g, x, y, sz, sz, int(startrad * 64), int((endrad - startrad) * 64));
    }

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      fill_arc(g, b, pos, radius, start_radius, end_radius);
      frame_arc(g, p, pos, radius, start_radius, end_radius);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      frame_arc(g, p, pos, radius, start_radius, end_radius);
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      fill_arc(g, b, pos, radius, start_radius, end_radius);
      frame_arc(g, pen(b.color()), pos, radius, start_radius, end_radius);
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      std::for_each(pts.begin(), pts.end(), [&](const core::point & pt){
                      points.push_back(pt.os());
                    });
      points.push_back(pts[0].os());
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
      points.push_back(pts.begin()->os());
    }

    void polygon::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   (XPoint*)points.data(),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   const_cast<XPoint*>(points.data()),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (const graphics& g,
                               const font& f,
                               os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.xOff;
        dx = 0;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() - height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height() - height;
      }

      xft_color xftcolor(c, g);
      clip clp(g, rect);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(),
                        px + dx, py + dy,
                        (XftChar8*)str.c_str(), int(str.size()));
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.xOff;
        dx = 0;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() - height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height() - height;
      }

      rect.top_left({core::global::unscale<core::point::type>(px + dx), core::global::unscale<core::point::type>(py)});
      rect.set_size({core::global::unscale<core::size::type>(width), core::global::unscale<core::size::type>(height)});
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.xOff;
        dx = extents.x;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = pos.os_x();
      int py = pos.os_y();

      if (origin_is_h_center(origin)) {
        px -= width / 2;
      } else if (origin_is_right(origin)) {
        px -= width;
      }

      if (origin_is_v_center(origin)) {
        py -= dy / 2;
      } else if (origin_is_bottom(origin)) {
        py -= height;
      }

      xft_color xftcolor(c, g);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(),
                        (px + dx), (py + dy),
                        (XftChar8*)str.c_str(), int(str.size()));
    }

#endif // X11

    template<PixelFormat px_fmt>
    void copy_frame_image (draw::const_image_data<px_fmt> src_img,
                           draw::image_data<px_fmt> dest_img,
                           const bitmap_info& src_bmi, const bitmap_info& dest_bmi,
                           const core::uint32_rect& frame) {

      const uint32_t width = dest_bmi.width;
      const uint32_t height = dest_bmi.height;

      const uint32_t left = std::min(frame.x(), width);
      const uint32_t right = std::min(frame.width(), width - left);
      const uint32_t top = std::min(frame.y(), height);
      const uint32_t bottom = std::min(frame.height(), height - top);

      const uint32_t target_right = dest_bmi.width - right;
      const uint32_t target_bottom = dest_bmi.height - bottom;
      const uint32_t source_right = src_bmi.width - right;
      const uint32_t source_bottom = src_bmi.height - bottom;

      using namespace convert;

      // top left
      if (top && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, 0}, {0, 0, left, top});
      }

      // top right
      if (top && right) {
        copy::sub<px_fmt>(src_img, dest_img, {source_right, 0}, {target_right, 0, right, top});
      }

      // bottom left
      if (bottom && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, source_bottom}, {0, target_bottom, left, bottom});
      }

      if (bottom && right) {
        // bottom right
        copy::sub<px_fmt>(src_img, dest_img,
                       {source_right, source_bottom}, {target_right, target_bottom, right, bottom});
      }

      if ((target_right >= left) && (target_bottom >= top) && (source_right >= left) && (source_bottom >= top)) {
        const uint32_t target_inner_width = target_right - left;
        const uint32_t target_inner_height = target_bottom - top;
        const uint32_t source_inner_width = source_right - left;
        const uint32_t source_inner_height = source_bottom - top;

        // top center
        if (top && target_inner_width) {
          stretch::sub<px_fmt>(src_img, dest_img,
                            {left, 0, source_inner_width, top},
                            {left, 0, target_inner_width, top});
        }

        // bottom center
        if (bottom && target_inner_width) {
          stretch::sub<px_fmt>(src_img, dest_img,
                            {left, source_bottom, source_inner_width, bottom},
                            {left, target_bottom, target_inner_width, bottom});
        }

        // left center
        if (left && target_inner_height) {
          stretch::sub<px_fmt>(src_img, dest_img,
                            {0, top, left, source_inner_height},
                            {0, top, left, target_inner_height});
        }

        // right center
        if (right && target_inner_height) {
          stretch::sub<px_fmt>(src_img, dest_img,
                            {source_right, top, right, source_inner_height},
                            {target_right, top, right, target_inner_height});
        }

        // center
        if (target_inner_width && target_inner_height) {
          stretch::sub<px_fmt>(src_img, dest_img,
                            {left, top, source_inner_width, source_inner_height},
                            {left, top, target_inner_width, target_inner_height});
        }
      }
    }

    template<PixelFormat T>
    void draw_image_frame (const graphics& g,
                           const core::point& pt,
                           const core::rectangle rect,
                           const datamap<T>& img,
                           const core::uint32_rect& frame) {
      if (rect.size() <= core::size::two) {
        return;
      }

      const uint32_t width = basepp::roundup<uint32_t>(rect.os_width());
      const uint32_t height = basepp::roundup<uint32_t>(rect.os_height());

      datamap<T> dest(width, height);
      copy_frame_image<T>(img.get_raw(), dest.get_raw(),
                          img.get_info(), dest.get_info(), frame);

      pixmap buffer;
      buffer = dest;
      g.copy_from(buffer, pt);
    }

    template<>
    void image_frame<PixelFormat::BW>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<PixelFormat::GRAY>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<PixelFormat::RGB>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<PixelFormat::RGBA>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

  }

}
