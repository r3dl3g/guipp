/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "graphics.h"
#include <logger.h>
#ifdef WIN32
#include <string_util.h>
#endif // WIN32

#ifdef X11
#include <cmath>
#include <algorithm>
#endif // X11

namespace gui {

  namespace draw {

#ifdef WIN32
    brush null_brush((os::brush)GetStockObject(NULL_BRUSH));
    pen null_pen((os::pen)GetStockObject(NULL_PEN));

    template<typename T>
    struct Use {
      Use(os::graphics g, HGDIOBJ t)
        : g(g)
        , obj(t)
        , old(set(t)) {
        set(t);
      }

      HGDIOBJ set(HGDIOBJ t) {
        return SelectObject(g, t);
      }

      void unset(HGDIOBJ t) {
        SelectObject(g, t);
      }

      inline ~Use() {
        unset(old);
      }

      os::graphics g;
      HGDIOBJ obj;
      HGDIOBJ old;
    };

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
      , end_radius(end_radius) {
    }

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      BeginPath(g);
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      BeginPath(g);
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt) {
        points.push_back(pt.os());
      });
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt) {
        points.push_back(pt.os());
      });
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
                               const color& c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, clear_background ? OPAQUE : TRANSPARENT);
      RECT Rect = rect;

      std::wstring wstr = ibr::string::utf8_to_utf16(str);

      if ((origin & (DT_WORDBREAK | DT_VCENTER)) == (DT_WORDBREAK | DT_VCENTER)) {
        int h = DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, (UINT)origin | DT_CALCRECT);
        Rect.left = rect.os_x();
        Rect.right = rect.os_x2();
        Rect.top = ((rect.size().os_height() - h) / 2);
        Rect.bottom = Rect.top + h;
      }
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, (UINT)origin);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   const color& c) const {
      Use<font> fn(g, f);
      RECT Rect = rect;
      std::wstring wstr = ibr::string::utf8_to_utf16(str);
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, (UINT)origin | DT_CALCRECT);
      rect = core::rectangle(Rect);
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           const color& c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, clear_background ? OPAQUE : TRANSPARENT);
      int px = pos.os_x();
      int py = pos.os_y();
      UINT old_align = top_left;
      std::wstring wstr = ibr::string::utf8_to_utf16(str);

      switch (origin) {
        case top_left:
          old_align = SetTextAlign(g, TA_LEFT | TA_TOP | TA_NOUPDATECP);
          break;
        case top_right:
          old_align = SetTextAlign(g, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
          break;
        case top_hcenter:
          old_align = SetTextAlign(g, TA_CENTER | TA_TOP | TA_NOUPDATECP);
          break;
        case bottom_left:
          old_align = SetTextAlign(g, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP | TA_BASELINE);
          break;
        case bottom_right:
          old_align = SetTextAlign(g, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP | TA_BASELINE);
          break;
        case bottom_hcenter:
          old_align = SetTextAlign(g, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP | TA_BASELINE);
          break;
        case vcenter_right: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_RIGHT | TA_NOUPDATECP);
          break;
        }
        case vcenter_left: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_LEFT | TA_NOUPDATECP);
          break;
        }
        case center: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_CENTER | TA_NOUPDATECP);
          break;
        }
      }
      TextOutW(g, px, py, wstr.c_str(), (int)wstr.size());
      SetTextAlign(g, old_align);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    graphics::graphics (os::window win, os::graphics gc)
      : gc(gc)
      , win(win)
    {}

    void graphics::draw_pixel (const core::point& pt,
                              const color& c) const {
      SetPixel(gc, pt.os_x(), pt.os_y(), c);
    }

    color graphics::get_pixel (const core::point& pt) const {
      return GetPixel(gc, pt.os_x(), pt.os_y());
    }

    void graphics::draw_lines (std::initializer_list<core::point> points,
                              const pen& p) const {
      Use<pen> pn(gc, p);
      bool first = true;
      for (core::point pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc, pt.os_x(), pt.os_y(), nullptr);
        } else {
          LineTo(gc, pt.os_x(), pt.os_y());
        }
      }
    }

    void graphics::invert (const core::rectangle& r) const {
      RECT rect = r;
      InvertRect(gc, &rect);
    }

    void graphics::flush() const {
      GdiFlush();
    }

    void graphics::push_clip_rectangle (const core::rectangle& r) const {
      HRGN hr = CreateRectRgn(r.os_x(), r.os_y(), r.os_x2(), r.os_y2());
      SelectClipRgn(gc, hr);
      clipping_rectangles.push_back(hr);
    }

    void graphics::pop_clip_rectangle () const {
      if (clipping_rectangles.size()) {
        HRGN hr = clipping_rectangles.back();
        DeleteObject(hr);
        clipping_rectangles.pop_back();
        if (clipping_rectangles.size()) {
          SelectClipRgn(gc, clipping_rectangles.back());
        }
      }
    }
    // --------------------------------------------------------------------------

#endif // WIN32

#ifdef X11

    // --------------------------------------------------------------------------
    template<typename T>
    struct Use {
      Use (os::graphics g,
           const T& t)
        : g(g) {
        set(t);
      }

      void set (const T& t);

      os::graphics g;
    };

    template<>
    void Use<pen>::set (const pen& p) {
      os::instance display = core::global::get_instance();
      XSetForeground(display, g, p.color());
      XSetLineAttributes(display, g, p.size(), p.style() & 0x0F, CapButt, JoinMiter);
      if (p.style() & 0x0F0) {
        switch (p.style()) {
          case pen::dot:
            static const char dots[] = {1, 1};
            XSetDashes(display, g, 0, dots, 2);
            break;
          case pen::dashDot:
            static const char dash_dots[] = {4, 4, 1, 4};
            XSetDashes(display, g, 0, dash_dots, 4);
            break;
          case pen::dashDotDot:
            static const char dash_dot_dots[] = {4, 4, 1, 2, 1, 4};
            XSetDashes(display, g, 0, dash_dot_dots, 6);
            break;
        }
      }
    }

    template<>
    void Use<brush>::set (const brush& b) {
      os::instance display = core::global::get_instance();
      XSetForeground(display, g, b.color());
      XSetFillStyle(display, g, b.style());
    }

    // --------------------------------------------------------------------------
    XftDraw* graphics::s_xft = nullptr;

    // --------------------------------------------------------------------------
    graphics::graphics (os::window win, os::graphics gc)
      : gc(gc)
      , win(win)
    {
      if (!s_xft) {
        Visual* visual = DefaultVisual(core::global::get_instance(), core::global::get_screen());
        Colormap colormap = DefaultColormap(core::global::get_instance(), core::global::get_screen());
        s_xft = XftDrawCreate(core::global::get_instance(), win, visual, colormap);
      } else {
        XftDrawChange(s_xft, win);
      }
    }

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLine(core::global::get_instance(), g, g, from.os_x(), from.os_y(), to.os_x(), to.os_y());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      Use<brush> br(g, b);
      os::instance display = core::global::get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
      XFillRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
      Use<pen> pn(g, p);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = core::global::get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = core::global::get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
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
      os::instance display = core::global::get_instance();
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
      os::instance display = core::global::get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = core::global::get_instance();
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
      const size_type w = size.os_width();
      const size_type h = size.os_height();

      const point_type x0 = rect.top_left().os_x();
      const point_type x1 = x0 + w;
      const point_type x3 = rect.bottom_right().os_x();
      const point_type x2 = x3 - w;

      const point_type y0 = rect.top_left().os_y();
      const point_type y1 = y0 + h;
      const point_type y3 = rect.bottom_right().os_y();
      const point_type y2 = y3 - h;

      const size_type w2 = w * size_type(2);
      const size_type h2 = h * size_type(2);

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
      os::instance display = core::global::get_instance();
      XSetArcMode(display, g, ArcChord);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;

      calc_arcs(rect, size, &arcs, &segments, nullptr);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = core::global::get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect, size, &arcs, nullptr, &rects);

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
      os::instance display = core::global::get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect, size, &arcs, &segments, &rects);

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
      : pos(pos), radius(radius), start_radius(startrad), end_radius(endrad) {
    }

    void frame_arc (const graphics& g,
                    const pen& p,
                    const core::point& pos,
                    unsigned int radius,
                    float startrad,
                    float endrad) {
      Use<pen> pn(g, p);
      int x = pos.os_x() - radius;
      int y = pos.os_y() - radius;
      unsigned int sz = radius * 2;
      XDrawArc(core::global::get_instance(), g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));

      int istart = int(startrad * 1000.0F) % 360000;
      int iend = int(endrad * 1000.0F) % 360000;
      if (istart != iend) {
        double start = M_PI * startrad / 180.0;
        double end = M_PI * endrad / 180.0;
        os::point pt[3];
        pt[0].x = short(pos.x() + int(radius * cos(start)));
        pt[0].y = short(pos.y() - int(radius * sin(start)));
        pt[1].x = short(pos.x());
        pt[1].y = short(pos.y());
        pt[2].x = short(pos.x() + int(radius * cos(end)));
        pt[2].y = short(pos.y() - int(radius * sin(end)));
        XDrawLines(core::global::get_instance(), g, g, pt, 3, CoordModeOrigin);
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

      os::instance display = core::global::get_instance();
      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));

      XDrawArc(core::global::get_instance(), g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
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
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt){
        points.push_back(pt.os());
      });
      points.push_back(pts[0].os());
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt){
        points.push_back(pt.os());
      });
      points.push_back(pts.begin()->os());
    }

    void polygon::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      XFillPolygon(core::global::get_instance(),
                   g,
                   g,
                   (XPoint*)points.data(),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      Use<pen> pn(g, p);
      XDrawLines(core::global::get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLines(core::global::get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      XFillPolygon(core::global::get_instance(),
                   g,
                   g,
                   const_cast<XPoint*>(points.data()),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawLines(core::global::get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (const graphics& g,
                               const font& f,
                               const color& c) const {
      os::instance display = core::global::get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        width = extents.width;
        height = extents.height;
        dx = extents.x;
        dy = extents.y;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if ((origin & DT_CENTER) == DT_CENTER) {
        px += (rect.size().width() - width) / 2;
      } else if ((origin & DT_RIGHT) == DT_RIGHT) {
        px += rect.size().width() - width;
      }
      if ((origin & DT_VCENTER) == DT_VCENTER) {
        py += (rect.size().height() - height) / 2;
      } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
        py += rect.size().height() - height;
      }

      Visual* visual = XftDrawVisual(g);
      Colormap colormap = XftDrawColormap(g);

      /* Xft text color */
      XRenderColor xrcolor = {
        (unsigned short)(c.r() << 8),
        (unsigned short)(c.g() << 8),
        (unsigned short)(c.b() << 8),
        0xffff
      };
      XftColor xftcolor;
      XftColorAllocValue(display, visual, colormap, &xrcolor, &xftcolor);

      clip clp(g, rect);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px + dx, py + dy, (XftChar8*)str.c_str(), int(str.size()));

      XftColorFree(display, visual, colormap, &xftcolor);
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   const color& c) const {
      os::instance display = core::global::get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        width = extents.xOff - extents.x;
        height = extents.height;
        dx = extents.x;
        dy = extents.y;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if ((origin & DT_CENTER) == DT_CENTER) {
        px += (rect.size().width() - width) / 2;
      } else if ((origin & DT_RIGHT) == DT_RIGHT) {
        px += rect.size().width() - width;
      }
      if ((origin & DT_VCENTER) == DT_VCENTER) {
        py += (rect.size().height() - height) / 2;
      } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
        py += rect.size().height() - height;
      }

      rect.top_left({core::point::type(px + dx), core::point::type(py + dy - height)});
      rect.set_size({core::size::type(width), core::size::type(height)});
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           const color& c) const {
      os::instance display = core::global::get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        width = extents.width;
        height = extents.height;
        dx = extents.x;
        dy = extents.y;
      } else {
        LogError << "font_type is zero!";
      }

      int px = pos.os_x();
      int py = pos.os_y();

      if ((origin & DT_CENTER) == DT_CENTER) {
        px -= width / 2;
      } else if ((origin & DT_RIGHT) == DT_RIGHT) {
        px -= width;
      }

      if ((origin & DT_VCENTER) == DT_VCENTER) {
        py -= height / 2;
      } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
        py -= height;
      }

      Visual* visual = XftDrawVisual(g);
      Colormap colormap = XftDrawColormap(g);

      /* Xft text color */
      XRenderColor xrcolor = {
        (unsigned short)(c.r() << 8),
        (unsigned short)(c.g() << 8),
        (unsigned short)(c.b() << 8),
        0xffff
      };
      XftColor xftcolor;
      XftColorAllocValue(display, visual, colormap, &xrcolor, &xftcolor);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px + dx, py + dy, (XftChar8*)str.c_str(), int(str.size()));

      XftColorFree(display, visual, colormap, &xftcolor);
    }

    // --------------------------------------------------------------------------
    void graphics::draw_pixel (const core::point& pt,
                               const color& c) const {
      Use<pen> pn(gc, pen(c));
      XDrawPoint(core::global::get_instance(), win, gc, pt.os_x(), pt.os_y());
    }

    color graphics::get_pixel (const core::point& pt) const {
      return color::black();
    }

    void graphics::draw_lines (std::initializer_list<core::point> pts,
                               const pen& p) const {

      Use<pen> pn(gc, p);
      std::vector<os::point> points;
      points.reserve(pts.size());
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt){
        points.push_back(pt.os());
      });
      XDrawLines(core::global::get_instance(), win, gc,
                 points.data(), (int)points.size(),
                 CoordModeOrigin);
    }

    void graphics::invert (const core::rectangle& r) const {
    }

    void graphics::flush () const {
//      XFlushGC(core::global::get_instance(), gc);
    }

    void graphics::push_clip_rectangle (const core::rectangle& rect) const {
      os::rectangle r = rect;
      clipping_rectangles.push_back(r);
      XSetClipRectangles(core::global::get_instance(), gc, 0, 0, &r, 1, Unsorted);
      XftDrawSetClipRectangles(s_xft, 0, 0, &r, 1);
    }

    void graphics::pop_clip_rectangle () const {
      clipping_rectangles.pop_back();
      if (clipping_rectangles.size()) {
        os::rectangle& r = clipping_rectangles.back();
        XSetClipRectangles(core::global::get_instance(), gc, 0, 0, &r, 1, Unsorted);
        XftDrawSetClipRectangles(s_xft, 0, 0, &r, 1);
      } else {
        XSetClipMask(core::global::get_instance(), gc, None);
        XftDrawSetClip(s_xft, None);
      }
    }

#endif // X11
    void graphics::frame (std::function<frameable> drawer,
                          const pen& p) const {
      drawer(*this, p);
    }

    void graphics::fill (std::function<fillable> drawer,
                         const brush& b) const {
      drawer(*this, b);
    }

    void graphics::draw (std::function<drawable> drawer,
                         const brush& b,
                         const pen& p) const {
      drawer(*this, b, p);
    }

    void graphics::text (std::function<textable> drawer,
                         const font& f,
                         const color& c) const {
      drawer(*this, f, c);
    }

    namespace frame {

      // --------------------------------------------------------------------------
      void lines (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, color::veryLightGray());
      }

      void vline (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(line(place.top_right(), place.bottom_right()), color::veryLightGray());
      }

      void hline (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(line(place.bottom_left(), place.bottom_right()), color::veryLightGray());
      }

      void vraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(1, 0);
        g.frame(line(r.top_left(), r.bottom_left()), color::white());
        g.frame(line(r.top_right(), r.bottom_right()), color::gray());
      }

      void hraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(0, 1);
        g.frame(line(r.top_left(), r.top_right()), color::white());
        g.frame(line(r.bottom_right(), r.bottom_left()), color::gray());
      }

      void vgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(1, 0);
        g.frame(line(r.top_left(), r.bottom_left()), color::gray());
        g.frame(line(r.top_right(), r.bottom_right()), color::white());
      }

      void hgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(0, 1);
        g.frame(line(r.top_left(), r.top_right()), color::gray());
        g.frame(line(r.bottom_right(), r.bottom_left()), color::white());
      }

      void raised_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::white());
        g.draw_lines({ r.top_right(), r.bottom_right(), r.bottom_left() }, color::gray());
      }

      void sunken_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::gray());
        g.draw_lines({ r.top_right(), r.bottom_right(), r.bottom_left() }, color::white());
      }

      void raised_deep_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::white());
        g.draw_lines({ r.bottom_left(), r.bottom_right(), r.top_right() }, color::gray());

        const core::point pm = core::point(1, -1);
        g.draw_lines({ r.bottom_left() + pm, r.top_left() + core::point::one, r.top_right() - pm }, color::veryLightGray());
        g.draw_lines({ r.bottom_left() + pm, r.bottom_right() - core::point::one, r.top_right() - pm }, color::mediumGray());
      }

      void sunken_deep_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::gray());
        g.draw_lines({ r.bottom_left(), r.bottom_right(), r.top_right() }, color::white());

        const core::point pm = core::point(1, -1);
        g.draw_lines({ r.bottom_left() + pm, r.top_left() + core::point::one, r.top_right() - pm }, color::darkGray());
        g.draw_lines({ r.bottom_left() + pm, r.bottom_right() - core::point::one, r.top_right() - pm }, color::veryLightGray());
      }

      // --------------------------------------------------------------------------
    } // frame

  }

}
