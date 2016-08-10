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


namespace gui {

  namespace draw {

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

    template<> void Use<pen>::set(const pen& p) {
      core::instance_id display = core::global::get_instance();
      XSetForeground(display, gc, p.color());
      XSetLineAttributes(display, gc, p.size(), p.style(), CapButt, JoinMiter);
    }

    template<> void Use<brush>::set(const brush& b) {
      core::instance_id display = core::global::get_instance();
      XSetForeground(display, gc, b.color());
      XSetFillStyle(display, gc, b.style());
    }

    template<> void Use<font>::set(const font& f) {
      XSetFont(core::global::get_instance(), gc, f);
    }

#endif // X11

    rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, p);
        Rectangle(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        core::instance_id display = core::global::get_instance();
        XFillRectangle(display, id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
        Use<pen> pn(gc, p);
        XDrawRectangle(display, id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
#endif //X11
      };
  }

    rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
#ifdef WIN32
        Use<brush> br(gc, null_brush);
        Rectangle(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        XDrawRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
#endif //X11
      };
    }

    rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, null_pen);
        Rectangle(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        XFillRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
#endif //X11
      };
    }

    ellipse::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, p);
        Ellipse(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.topleft;

        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display, id, gc, pt.x, pt.y, rect.size().width, rect.size().height, 0, 360 * 64);
        Use<pen> pn(gc, p);
        XSetArcMode(display, gc, ArcChord);
        XDrawArc(display, id, gc, pt.x, pt.y, rect.size().width, rect.size().height, 0, 360 * 64);
#endif //X11
      };
    }

    ellipse::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
#ifdef WIN32
        Use<brush> br(gc, null_brush);
        Ellipse(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.topleft;

        XSetArcMode(display, gc, ArcChord);
        XDrawArc(display, id, gc, pt.x, pt.y, rect.size().width, rect.size().height, 0, 360 * 64);
#endif //X11
      };
    }

    ellipse::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, null_pen);
        Ellipse(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
#elif X11
        core::instance_id display = core::global::get_instance();
        const core::point& pt = rect.topleft;

        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display, id, gc, pt.x, pt.y, rect.size().width, rect.size().height, 0, 360 * 64);
#endif //X11
      };
    }

    round_rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
#ifdef WIN32
        Use<brush> br(gc, null_brush);
        RoundRect(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
#elif X11
        const int w = size.width;
        const int h = size.height;

        const int x0 = rect.topleft.x;
        const int x1 = x0 + w;
        const int x3 = rect.bottomright.x;
        const int x2 = x3 - w;

        const int y0 = rect.topleft.y;
        const int y1 = y0 + h;
        const int y3 = rect.bottomright.y;
        const int y2 = y3 - h;

        const int w2 = w * 2;
        const int h2 = h * 2;

        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcChord);
        XDrawArc(display, id, gc, x0, y0, w2, h2, 90 * 64, 90 * 64);
        XDrawArc(display, id, gc, x3 - w2, y0, w2, h2, 0, 90 * 64);
        XDrawArc(display, id, gc, x0, y3 - h2, w2, h2, 180 * 64, 90 * 64);
        XDrawArc(display, id, gc, x3 - w2, y3 - h2, w2, h2, 270 * 64, 90 * 64);

        XDrawLine(display, id, gc, x1, y0, x2, y0);
        XDrawLine(display, id, gc, x1, y3, x2, y3);
        XDrawLine(display, id, gc, x0, y1, x0, y2);
        XDrawLine(display, id, gc, x3, y1, x3, y2);
#endif //X11
      };
    }

    round_rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, null_pen);
        RoundRect(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
#elif X11
        const int w = size.width;
        const int h = size.height;

        const int x0 = rect.topleft.x;
        const int x1 = x0 + w;
        const int x3 = rect.bottomright.x;
        const int x2 = x3 - w;

        const int y0 = rect.topleft.y;
        const int y1 = y0 + h;
        const int y3 = rect.bottomright.y;
        const int y2 = y3 - h;

        const int w2 = w * 2;
        const int h2 = h * 2;

        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display, id, gc, x0, y0, w2, h2, 90 * 64, 90 * 64);
        XFillArc(display, id, gc, x3 - w2, y0, w2, h2, 0, 90 * 64);
        XFillArc(display, id, gc, x0, y3 - h2, w2, h2, 180 * 64, 90 * 64);
        XFillArc(display, id, gc, x3 - w2, y3 - h2, w2, h2, 270 * 64, 90 * 64);

        XFillRectangle(display, id, gc, x0, y1, w, y2 - y1);
        XFillRectangle(display, id, gc, x2, y1, w, y2 - y1);
        XFillRectangle(display, id, gc, x1, y0, x2 - x1,  y3 - y0);
#endif //X11
      };
    }

    round_rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
#ifdef WIN32
        Use<brush> br(gc, b);
        Use<pen> pn(gc, p);
        RoundRect(gc, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
#elif X11
        operator fillable()(id, gc, b);
        operator frameable()(id, gc, p);
#endif //X11
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
#ifdef WIN32
        Use<pen> pn(gc, p);
        MoveToEx(gc, pos.x, pos.y, NULL);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
#elif X11
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;
        XFillArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
        Use<pen> pn(gc, p);
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
#endif // X11
      };
    }

    arc::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
#ifdef WIN32
        Use<brush> br(gc, null_brush);
        MoveToEx(gc, pos.x, pos.y, NULL);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
#elif X11
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
#endif // X11
      };
    }

    arc::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, null_pen);
        MoveToEx(gc, pos.x, pos.y, NULL);
        AngleArc(gc, pos.x, pos.y, radius, startrad, endrad);
        LineTo(gc, pos.x, pos.y);
#elif X11
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;

        core::instance_id display = core::global::get_instance();
        XSetArcMode(display, gc, ArcPieSlice);
        XFillArc(display, id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
#endif // X11
      };
    }

    core::point_type *buildPoints(const std::vector<core::point>& pts, int& size) {
      size = (int)pts.size();
      core::point_type *points = new core::point_type[size];
      for (int i = 0; i < size; ++i) {
        points[i] = pts[i];
      }
      return points;
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
#ifdef WIN32
        Use<pen> pn(gc, p);
        Polygon(gc, points, count);
#elif X11
        XFillPolygon(core::global::get_instance(), id, gc, points, count, 0, CoordModeOrigin);
        Use<pen> pn(gc, p);
        XDrawLines(core::global::get_instance(), id, gc, points, count, CoordModeOrigin);
#endif // X11
      };
    }

    polygone::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const pen& p) {
        Use<pen> pn(gc, p);
#ifdef WIN32
        Use<brush> br(gc, null_brush);
        Polygon(gc, points, count);
#elif X11
        XDrawLines(core::global::get_instance(), id, gc, points, count, CoordModeOrigin);
#endif // X11
      };
    }

    polygone::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b) {
        Use<brush> br(gc, b);
#ifdef WIN32
        Use<pen> pn(gc, null_pen);
        Polygon(gc, points, count);
#elif X11
        XFillPolygon(core::global::get_instance(), id, gc, points, count, 0, CoordModeOrigin);
#endif // X11
      };
    }

    text_box::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
#ifdef WIN32
        core::color_type old_color = SetTextColor(gc, p.color());
        int old_mode = SetBkMode(gc, clear_background ? OPAQUE : TRANSPARENT);
        RECT Rect = rect;
        if ((origin & (DT_WORDBREAK | DT_VCENTER)) == (DT_WORDBREAK | DT_VCENTER)) {
          int h = DrawText(gc, text.c_str(), (int)text.size(), &Rect, (UINT)origin | DT_CALCRECT);
          Rect.left = rect.topleft.x;
          Rect.right = rect.bottomright.x;
          Rect.top = (rect.size().height - h) / 2;
          Rect.bottom = Rect.top + h;
        }
        DrawText(gc, text.c_str(), (int)text.size(), &Rect, (UINT)origin);
        SetBkMode(gc, old_mode);
        SetTextColor(gc, old_color);
#elif X11
        Use<pen> pn(gc, p);
        // TBD!
        XDrawString(core::global::get_instance(), id, gc, rect.position().x, rect.position().y, text.c_str(), text.size());
#endif // X11
      };
    }

    text::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, const brush& b, const pen& p) {
#ifdef WIN32
        core::color_type old_color = SetTextColor(gc, p.color());
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
#elif X11
        Use<pen> pn(gc, p);
        // TBD!
        XDrawString(core::global::get_instance(), id, gc, pos.x, pos.y, str.c_str(), str.size());
#endif // X11
      };
    }

    void graphics::drawPixel(const core::point &pt, const color &c) {
#ifdef WIN32
      SetPixel(gc, pt.x, pt.y, c);
#endif // WIN32
#ifdef X11
      Use<pen> pn(gc, pen(c));
      XDrawPoint(core::global::get_instance(), win, gc, pt.x, pt.y);
#endif // X11
    }

    color graphics::getPixel(const core::point &pt) const {
#ifdef WIN32
      return GetPixel(gc, pt.x, pt.y);
#endif // WIN32
#ifdef X11
      return color::black;
#endif // X11
    }

    void graphics::drawLine(const core::point &from, const core::point &to, const pen& p) {
      Use<pen> pn(gc, p);
#ifdef WIN32
      MoveToEx(gc, from.x, from.y, NULL);
      LineTo(gc, to.x, to.y);
#endif // WIN32
#ifdef X11
      XDrawLine(core::global::get_instance(), win, gc, from.x, from.y, to.x, to.y);
#endif // X11
    }

    void graphics::drawLines(std::vector<core::point> &points, const pen& p) {
#ifdef WIN32
      Use<pen> pn(gc, p);
      bool first = true;
      for (core::point pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc, pt.x, pt.y, NULL);
        } else {
          LineTo(gc, pt.x, pt.y);
        }
      }
#elif X11
      polygone poly(points);
      frame(poly, p);
#endif // X11
    }

    void graphics::frame(const frameable& drawer, const pen& p) const {
      drawer(win, gc, p);
    }

    void graphics::fill(const fillable& drawer, const brush& b) const {
      drawer(win, gc, b);
    }

    void graphics::draw(const drawable& drawer, const brush& b, const pen& p) const {
      drawer(win, gc, b, p);
    }

    void graphics::draw(const drawable& drawer, const font& f, const color& c) const {
      Use<font> fn(gc, f);
      drawer(win, gc, brush(c), pen(c));
    }

    void graphics::invert(const core::rectangle& r) const {
#ifdef WIN32
      RECT rect = r;
      InvertRect(gc, &rect);
#endif // WIN32
    }

  }

}
