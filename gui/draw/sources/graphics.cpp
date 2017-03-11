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
#ifdef X11
# include <X11/Xlib.h>
#endif // X11

namespace gui {

  namespace draw {

    namespace detail {
      core::point_type *buildPoints(const std::vector<core::point>& pts, int& size) {
        size = (int)pts.size();
        core::point_type *points = new core::point_type[size];
        for (int i = 0; i < size; ++i) {
          points[i] = pts[i];
        }
        return points;
      }
    }

#ifdef WIN32
    brush null_brush((core::brush_id)GetStockObject(NULL_BRUSH));
    pen null_pen((core::pen_id)GetStockObject(NULL_PEN));

    template<typename T>
    struct Use {
      Use(core::graphics_id gc, HGDIOBJ t)
        : gc(gc)
        , obj(t)
        , old(set(t)) {
        set(t);
      }

      HGDIOBJ set(HGDIOBJ t) {
        return SelectObject(gc, t);
      }

      void unset(HGDIOBJ t) {
        SelectObject(gc, t);
      }

      inline ~Use() {
        unset(old);
      }

      core::graphics_id gc;
      HGDIOBJ obj;
      HGDIOBJ old;
    };

    rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        Rectangle(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
  }

    rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        Use<brush> br(gc, null_brush);
        Rectangle(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
    }

    rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        pen p(b.color());
        Use<pen> pn(gc, p);
        Rectangle(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
    }

    ellipse::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        Ellipse(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
    }

    ellipse::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        Use<brush> br(gc, null_brush);
        Ellipse(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
    }

    ellipse::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        pen p(b.color());
        Use<pen> pn(gc, p);
        Ellipse(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y);
      };
    }

    round_rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        Use<brush> br(gc, null_brush);
        RoundRect(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y, size.width * 2, size.height * 2);
      };
    }

    round_rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        pen p(b.color());
        Use<pen> pn(gc, p);
        RoundRect(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y, size.width * 2, size.height * 2);
      };
    }

    round_rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        RoundRect(gc, rect.top_left.x, rect.top_left.y, rect.bottom_right.x, rect.bottom_right.y, size.width * 2, size.height * 2);
      };
    }

    arc::arc(const core::point& pos, unsigned int radius, float startrad, float endrad)
      : pos(pos)
      , radius(radius)
      , startrad(startrad)
      , endrad(endrad) {
    }

    arc::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        BeginPath(gc);
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        MoveToEx(gc, pos.x, pos.y, nullptr);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
        EndPath(gc);
        StrokeAndFillPath(gc);
      };
    }

    arc::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        Use<brush> br(gc, null_brush);
        MoveToEx(gc, pos.x, pos.y, nullptr);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
      };
    }

    arc::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        BeginPath(gc);
        Use<brush> br(gc, b);
        pen p(b.color());
        Use<pen> pn(gc, p);
        MoveToEx(gc, pos.x, pos.y, nullptr);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
        EndPath(gc);
        StrokeAndFillPath(gc);
      };
    }

    polygone::polygone(const polygone& rhs)
      : count(rhs.count)
    {
      points = new core::point_type[count];
      memcpy(points, rhs.points, count * sizeof(core::point_type));
    }

    polygone::~polygone() {
      delete[] points;
    }

    polygone::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        Polygon(gc, points, count);
      };
    }

    polygone::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        Use<brush> br(gc, null_brush);
        Polygon(gc, points, count);
      };
    }

    polygone::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        pen p(b.color());
        Use<pen> pn(gc, p);
        Polygon(gc, points, count);
      };
    }

    text_box::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        core::color_type old_color = SetTextColor(gc, c);
        int old_mode = SetBkMode(gc, clear_background ? OPAQUE : TRANSPARENT);
        RECT Rect = rect;
        if ((origin & (DT_WORDBREAK | DT_VCENTER)) == (DT_WORDBREAK | DT_VCENTER)) {
          int h = DrawText(gc, str.c_str(), (int)str.size(), &Rect, (UINT)origin | DT_CALCRECT);
          Rect.left = rect.top_left.x;
          Rect.right = rect.bottom_right.x;
          Rect.top = (rect.size().height - h) / 2;
          Rect.bottom = Rect.top + h;
        }
        DrawText(gc, str.c_str(), (int)str.size(), &Rect, (UINT)origin);
        SetBkMode(gc, old_mode);
        SetTextColor(gc, old_color);
      };
    }

    bounding_box::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        RECT Rect = rect;
        DrawText(gc, str.c_str(), (int)str.size(), &Rect, (UINT)origin | DT_CALCRECT);
        rect = core::rectangle(Rect);
      };
    }

    text::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        core::color_type old_color = SetTextColor(gc, c);
        int old_mode = SetBkMode(gc, clear_background ? OPAQUE : TRANSPARENT);
        int px = pos.x;
        int py = pos.y;
        UINT old_align = top_left;

        switch (origin) {
          case top_left:
            old_align = SetTextAlign(gc, TA_LEFT | TA_TOP | TA_NOUPDATECP);
            break;
          case top_right:
            old_align = SetTextAlign(gc, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
            break;
          case top_hcenter:
            old_align = SetTextAlign(gc, TA_CENTER | TA_TOP | TA_NOUPDATECP);
            break;
          case bottom_left:
            old_align = SetTextAlign(gc, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
            break;
          case bottom_right:
            old_align = SetTextAlign(gc, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP);
            break;
          case bottom_hcenter:
            old_align = SetTextAlign(gc, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP);
            break;
          case vcenter_right: {
            SIZE sz;
            GetTextExtentPoint32(gc, str.c_str(), (int)str.size(), &sz);
            py += sz.cy / 2;
            old_align = SetTextAlign(gc, TA_RIGHT | TA_BASELINE | TA_NOUPDATECP);
            break;
          }
          case vcenter_left: {
            SIZE sz;
            GetTextExtentPoint32(gc, str.c_str(), (int)str.size(), &sz);
            py += sz.cy / 2;
            old_align = SetTextAlign(gc, TA_LEFT | TA_BASELINE | TA_NOUPDATECP);
            break;
          }
          case center: {
            SIZE sz;
            GetTextExtentPoint32(gc, str.c_str(), (int)str.size(), &sz);
            py += sz.cy / 2;
            old_align = SetTextAlign(gc, TA_CENTER | TA_BASELINE | TA_NOUPDATECP);
            break;
          }
        }
        TextOut(gc, px, py, str.c_str(), (int)str.size());
        SetTextAlign(gc, old_align);
        SetBkMode(gc, old_mode);
        SetTextColor(gc, old_color);
      };
    }

    void graphics::drawPixel(const core::point &pt, const color &c) {
      SetPixel(gc, pt.x, pt.y, c);
    }

    color graphics::getPixel(const core::point &pt) const {
      return GetPixel(gc, pt.x, pt.y);
    }

    void graphics::drawLine(const core::point &from, const core::point &to, const pen& p) {
      Use<pen> pn(gc, p);
      MoveToEx(gc, from.x, from.y, nullptr);
      LineTo(gc, to.x, to.y);
    }

    void graphics::drawLines(const std::vector<core::point> &points, const pen& p) {
      Use<pen> pn(gc, p);
      bool first = true;
      for (core::point pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc, pt.x, pt.y, nullptr);
        } else {
          LineTo(gc, pt.x, pt.y);
        }
      }
    }

    void graphics::invert(const core::rectangle& r) const {
      RECT rect = r;
      InvertRect(gc, &rect);
    }

#endif // WIN32

#ifdef X11
    template<typename T>
    struct Use {
      Use(core::graphics_id gc, const T& t)
        : gc(gc) {
        set(t);
      }

      void set(const T& t);

      core::graphics_id gc;
  };

    template<>
    void Use<pen>::set(const pen& p) {
      core::instance_id display = core::global::get_instance();
      XSetForeground(display, gc, p.color());
      XSetLineAttributes(display, gc, p.size(), p.style() & 0x0F, CapButt, JoinMiter);
      if (p.style() & 0x0F0) {
        switch (p.style()) {
          case pen::dot:
            static const char dots[] = { 1, 1 };
            XSetDashes(display, gc, 0, dots, 2);
          break;
          case pen::dashDot:
            static const char dash_dots[] = { 4, 4, 1, 4 };
            XSetDashes(display, gc, 0, dash_dots, 4);
            break;
          case pen::dashDotDot:
            static const char dash_dot_dots[] = { 4, 4, 1, 2, 1, 4 };
            XSetDashes(display, gc, 0, dash_dot_dots, 6);
            break;
        }
      }
    }

    template<>
    void Use<brush>::set(const brush& b) {
      core::instance_id display = core::global::get_instance();
      XSetForeground(display, gc, b.color());
      XSetFillStyle(display, gc, b.style());
    }

    template<>
    void Use<font>::set(const font& f) {
      XSetFont(core::global::get_instance(), gc, f);
    }

    rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        core::instance_id display = core::global::get_instance();
        XFillRectangle(display,
                       id,
                       gc,
                       rect.top_left.x,
                       rect.top_left.y,
                       (unsigned int)rect.size().width,
                       (unsigned int)rect.size().height);
        Use<pen> pn(gc, p);
        XDrawRectangle(display,
                       id,
                       gc,
                       rect.top_left.x,
                       rect.top_left.y,
                       (unsigned int)rect.size().width,
                       (unsigned int)rect.size().height);
      };
    }

    rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        XDrawRectangle(core::global::get_instance(),
                       id,
                       gc,
                       rect.top_left.x,
                       rect.top_left.y,
                       (unsigned int)rect.size().width,
                       (unsigned int)rect.size().height);
      };
    }

    rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        XFillRectangle(core::global::get_instance(),
                       id,
                       gc,
                       rect.top_left.x,
                       rect.top_left.y,
                       (unsigned int)rect.size().width,
                       (unsigned int)rect.size().height);
      };
    }

    ellipse::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.top_left;

        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display,
                 id,
                 gc,
                 pt.x,
                 pt.y,
                 (unsigned int)rect.size().width,
                 (unsigned int)rect.size().height,
                 0,
                 360 * 64);
        Use<pen> pn(gc, p);
        XSetArcMode(display, gc, ArcChord);
        XDrawArc(display,
                 id,
                 gc,
                 pt.x,
                 pt.y,
                 (unsigned int)rect.size().width,
                 (unsigned int)rect.size().height,
                 0,
                 360 * 64);
      };
    }

    ellipse::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.top_left;

        XSetArcMode(display, gc, ArcChord);
        XDrawArc(display,
                 id,
                 gc,
                 pt.x,
                 pt.y,
                 (unsigned int)rect.size().width,
                 (unsigned int)rect.size().height,
                 0,
                 360 * 64);
      };
    }

    ellipse::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.top_left;

        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display,
                 id,
                 gc,
                 pt.x,
                 pt.y,
                 (unsigned int)rect.size().width,
                 (unsigned int)rect.size().height,
                 0,
                 360 * 64);
      };
    }

    static const int degree_0 = 0;
    static const int degree_90 = 90 * 64;
    static const int degree_180 = 180 * 64;
    static const int degree_270 = 270 * 64;

    void calc_arcs (const core::rectangle& rect,
                    const core::size& size,
                    std::array<XArc, 4>* arcs,
                    std::array<XSegment, 4>* segments,
                    std::array<XRectangle, 3>* rects) {

      const unsigned short w = (unsigned short)size.width;
      const unsigned short h = (unsigned short)size.height;

      const short x0 = (short)rect.top_left.x;
      const short x1 = x0 + w;
      const short x3 = (short)rect.bottom_right.x;
      const short x2 = x3 - w;

      const short y0 = (short)rect.top_left.y;
      const short y1 = y0 + h;
      const short y3 = (short)rect.bottom_right.y;
      const short y2 = y3 - h;

      const unsigned short w2 = w * (unsigned short)(2);
      const unsigned short h2 = h * (unsigned short)(2);

      if (arcs) {
        (*arcs)[0] = {x0, y0, w2, h2, degree_90, degree_90};
        (*arcs)[1] = {short(x3 - w2), y0, w2, h2, degree_0, degree_90};
        (*arcs)[2] = {x0, short(y3 - h2), w2, h2, degree_180, degree_90};
        (*arcs)[3] = {short(x3 - w2), short(y3 - h2), w2, h2, degree_270, degree_90};
      }

      if (segments) {
        (*segments)[0] = {x1, y0, x2, y0};
        (*segments)[1] = {x3, y1, x3, y2};
        (*segments)[2] = {x1, y3, x2, y3};
        (*segments)[3] = {x0, y1, x0, y2};
      }

      if (rects) {
        (*rects)[0] = {x0, y1, w, (unsigned short)(y2 - y1)};
        (*rects)[1] = {x2, y1, w, (unsigned short)(y2 - y1)};
        (*rects)[2] = {x1, y0, (unsigned short)(x2 - x1),  (unsigned short)(y3 - y0)};
      }
    }


    round_rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcChord);

        std::array<XArc, 4> arcs;
        std::array<XSegment, 4> segments;

        calc_arcs(rect, size, &arcs, &segments, nullptr);

        XDrawArcs(display, id, gc, arcs.data(), (int)arcs.size());
        XDrawSegments(display, id, gc, segments.data(), (int)segments.size());
      };
    }

    round_rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcChord);

        std::array<XArc, 4> arcs;
        std::array<XRectangle, 3> rects;
        calc_arcs(rect, size, &arcs, nullptr, &rects);

        XFillArcs(display, id, gc, arcs.data(), (int)arcs.size());
        XFillRectangles(display, id, gc, rects.data(), (int)rects.size());
      };
    }

    round_rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcPieSlice);

        std::array<XArc, 4> arcs;
        std::array<XSegment, 4> segments;
        std::array<XRectangle, 3> rects;
        calc_arcs(rect, size, &arcs, &segments, &rects);

        XFillArcs(display, id, gc, arcs.data(), (int)arcs.size());
        XFillRectangles(display, id, gc, rects.data(), (int)rects.size());

        Use<pen> pn(gc, p);
        XSetArcMode(display, gc, ArcChord);

        XDrawArcs(display, id, gc, arcs.data(), (int)arcs.size());
        XDrawSegments(display, id, gc, segments.data(), (int)segments.size());
      };
    }

    arc::arc(const core::point& pos, unsigned int radius, float startrad, float endrad)
      : pos(pos)
      , radius(radius)
      , startrad(startrad)
      , endrad(endrad) {
    }

    arc::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        int x = pos.x - radius;
        int y = pos.y - radius;
        unsigned int sz = radius * 2;
        XFillArc(core::global::get_instance(), id, gc, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
        Use<pen> pn(gc, p);
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
      };
    }

    arc::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        int x = pos.x - radius;
        int y = pos.y - radius;
        unsigned int sz = radius * 2;
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
      };
    }

    arc::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        int x = pos.x - radius;
        int y = pos.y - radius;
        unsigned int sz = radius * 2;

        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display, id, gc, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
      };
    }

    polygone::polygone(const polygone& rhs)
      : count(rhs.count)
    {
      points = new core::point_type[count];
      memcpy(points, rhs.points, count * sizeof(core::point_type));
    }

    polygone::~polygone() {
      delete[] points;
    }

    polygone::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
        XFillPolygon(core::global::get_instance(), id, gc, points, count, 0, CoordModeOrigin);
        Use<pen> pn(gc, p);
        XDrawLines(core::global::get_instance(), id, gc, points, count, CoordModeOrigin);
      };
    }

    polygone::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
        XDrawLines(core::global::get_instance(), id, gc, points, count, CoordModeOrigin);
      };
    }

    polygone::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
        XFillPolygon(core::global::get_instance(), id, gc, points, count, 0, CoordModeOrigin);
      };
    }

    text_box::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        Use<pen> pn(gc, c);

        int direction = 0, ascent = 0, descent = 0;
        XCharStruct overall;
        memset(&overall, 0, sizeof(XCharStruct));

        if (f.font_type()) {
          XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                       &direction, &ascent, &descent, &overall);
        } else {
          LogError << "font_type is zero!";
        }

        int px = rect.position().x;
        int py = rect.position().y;
        int width = overall.width;
        int height = (ascent - descent);

//        LogDebug << "r.x:" << px << " r.y:" << py
//                 << " r.size:" << rect.size()
//                 << " o.w:" << width << " o.h:" << height
//                 << " asc:" << ascent << " des:" << descent;

        if ((origin & DT_CENTER) == DT_CENTER) {
          px += (rect.size().width - width) / 2;
        } else if ((origin & DT_RIGHT) == DT_RIGHT) {
          px += rect.size().width - width;
        }
        if ((origin & DT_VCENTER) == DT_VCENTER) {
          py += (rect.size().height + height) / 2;
        } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
          py += rect.size().height;
        } else {
          py += height;
        }

//        LogDebug << "x:" << px << " y:" << py;

        XDrawString(core::global::get_instance(), id, gc, px, py, str.c_str(), int(str.size()));
      };
    }

    bounding_box::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        Use<pen> pn(gc, c);

        int direction = 0, ascent = 0, descent = 0;
        XCharStruct overall;
        memset(&overall, 0, sizeof(XCharStruct));

        if (f.font_type()) {
          XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                       &direction, &ascent, &descent, &overall);
        } else {
          LogError << "font_type is zero!";
        }

        int px = rect.position().x;
        int py = rect.position().y;
        int width = overall.width;
        int height = (ascent - descent);

        if ((origin & DT_CENTER) == DT_CENTER) {
          px += (rect.size().width - width) / 2;
        } else if ((origin & DT_RIGHT) == DT_RIGHT) {
          px += rect.size().width - width;
        }
        if ((origin & DT_VCENTER) == DT_VCENTER) {
          py += (rect.size().height + height) / 2;
        } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
          py += rect.size().height;
        } else {
          py += height;
        }

        rect.top_left = { px, py - overall.ascent };
        rect.set_size({width, overall.ascent + overall.descent});
      };
    }

    text::operator texter() const {
      return [&](core::drawable_id id, core::graphics_id gc, const font& f, const color& c) {
        Use<font> fn(gc, f);
        Use<pen> pn(gc, c);

        int direction = 0, ascent = 0, descent = 0;
        XCharStruct overall;
        memset(&overall, 0, sizeof(XCharStruct));

        if (f.font_type()) {
          XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                       &direction, &ascent, &descent, &overall);
        } else {
          LogError << "font_type is zero!";
        }

        int px = pos.x;
        int py = pos.y;

        if ((origin & DT_CENTER) == DT_CENTER) {
          px -= overall.width / 2;
        } else if ((origin & DT_RIGHT) == DT_RIGHT) {
          px -= overall.width;
        }

        int height = (overall.ascent - overall.descent);
        if ((origin & DT_VCENTER) == DT_VCENTER) {
          py += height / 2;
        } else if ((origin & DT_BOTTOM) == DT_BOTTOM) {
        } else {
          py += height;
        }

        XDrawString(core::global::get_instance(), id, gc, px, py, str.c_str(), int(str.size()));
      };
    }

    void graphics::drawPixel(const core::point &pt, const color &c) {
      Use<pen> pn(gc, pen(c));
      XDrawPoint(core::global::get_instance(), win, gc, pt.x, pt.y);
    }

    color graphics::getPixel(const core::point &pt) const {
      return color::black;
    }

    void graphics::drawLine(const core::point &from, const core::point &to, const pen& p) {
      Use<pen> pn(gc, p);
      XDrawLine(core::global::get_instance(), win, gc, from.x, from.y, to.x, to.y);
    }

    void graphics::drawLines(const std::vector<core::point> &points, const pen& p) {
      polygone poly(points);
      frame(poly, p);
    }

    void graphics::invert(const core::rectangle& r) const {
    }

#endif // X11

    void graphics::frame(const frameable& drawer, const pen& p) const {
      drawer(win, gc, p);
    }

    void graphics::fill(const fillable& drawer, const brush& b) const {
      drawer(win, gc, b);
    }

    void graphics::draw(const drawable& drawer, const brush& b, const pen& p) const {
      drawer(win, gc, b, p);
    }

    void graphics::text(const texter& drawer, const font& f, const color& c) const {
      drawer(win, gc, f, c);
    }

  }

}
