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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "graphics.h"


namespace gui {

  namespace draw {

#ifdef WIN32
    void rectangle::operator() (core::drawable_id, core::graphics_id id, int) {
      Rectangle(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
    }
#elif X11
    rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XDrawRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
        XFillRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
      };
  }

    rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XDrawRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
      };
    }

    rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XFillRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
      };
    }
#endif

#ifdef WIN32
    void ellipse::operator() (core::drawable_id, core::graphics_id id, int) {
      Ellipse(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
    }
#elif X11
    ellipse::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XDrawArc(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height, 0, 360 * 64);
        XFillArc(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height, 0, 360 * 64);
      };
    }

    ellipse::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XDrawArc(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height, 0, 360 * 64);
      };
    }

    ellipse::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        XFillArc(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height, 0, 360 * 64);
      };
    }
#endif // X11

#ifdef WIN32
    void round_rectangle::operator() (core::drawable_id, core::graphics_id id, int) {
      RoundRect(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
    }
#elif X11
    round_rectangle::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        int x0 = rect.topleft.x;
        int x1 = x1 + size.width;
        int x3 = rect.bottomright.x;
        int x2 = x3 - size.width;

        int y0 = rect.topleft.y;
        int y1 = y0 + size.height;
        int y3 = rect.bottomright.y;
        int y2 = y3 - size.height;

        int w = size.width * 2;
        int h = size.height * 2;

        XDrawArc(core::global::get_instance(), id, gc, x0, y0, w, h, 90 * 64, 180 * 64);
        XDrawArc(core::global::get_instance(), id, gc, x3 - w, y0, w, h, 0, 90 * 64);
        XDrawArc(core::global::get_instance(), id, gc, x0, y3 - h, w, h, 180 * 64, 270 * 64);
        XDrawArc(core::global::get_instance(), id, gc, x3 - w, y3 - h, w, h, 270 * 64, 360 * 64);

        XDrawLine(core::global::get_instance(), win, gc, x1, y0, x2, y0);
        XDrawLine(core::global::get_instance(), win, gc, x1, y3, x2, y3);
        XDrawLine(core::global::get_instance(), win, gc, x0, y1, x0, y2);
        XDrawLine(core::global::get_instance(), win, gc, x3, y1, x3, y2);
      };
    }

    round_rectangle::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        int x0 = rect.topleft.x;
        int x1 = x1 + size.width;
        int x3 = rect.bottomright.x;
        int x2 = x3 - size.width;

        int y0 = rect.topleft.y;
        int y1 = y0 + size.height;
        int y3 = rect.bottomright.y;
        int y2 = y3 - size.height;

        int w = size.width * 2;
        int h = size.height * 2;

        XFillArc(core::global::get_instance(), id, gc, x0, y0, w, h, 90 * 64, 180 * 64);
        XFillArc(core::global::get_instance(), id, gc, x3 - w, y0, w, h, 0, 90 * 64);
        XFillArc(core::global::get_instance(), id, gc, x0, y3 - h, w, h, 180 * 64, 270 * 64);
        XFillArc(core::global::get_instance(), id, gc, x3 - w, y3 - h, w, h, 270 * 64, 360 * 64);

        XFillRectangle(core::global::get_instance(), id, gc, x0, y1, size.width, y3 - h);
        XFillRectangle(core::global::get_instance(), id, gc, x2, y1, size.width, y3 - h);
        XFillRectangle(core::global::get_instance(), id, gc, x1, y0, x2 - x1,  y3 - y0);
      };
    }

    round_rectangle::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int i) {
        operator fillable()(id, gc, i);
        operator frameable()(id, gc, i);
      };
    }
#endif // X11

    arc::arc(const core::point& pos, unsigned int radius, float startrad, float endrad)
      : pos(pos)
      , radius(radius)
      , startrad(startrad)
      , endrad(endrad) {
    }

#ifdef WIN32
    void arc::operator() (core::drawable_id, core::graphics_id id, int) {
      AngleArc(id, pos.x, pos.y, radius, startrad, endrad);
    }
#elif X11
    arc::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
        XFillArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
      };
    }

    arc::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;
        XDrawArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
      };
    }

    arc::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc) {
        int x = pos.x - radius;
        int y = pos.y - radius;
        int sz = radius * 2;
        XFillArc(core::global::get_instance(), id, gc, x, y, sz, sz, startrad * 64, endrad * 64);
      };
    }
#endif // X11

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

#ifdef WIN32
    void polygone::operator() (core::drawable_id, core::graphics_id gc, int) {
      Polygon(gc, points, count);
    }
#elif X11
    polygone::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        XFillPolygon(core::global::get_instance(), id, gc, points, count. 0, CoordModeOrigin);
        XDrawLines(core::global::get_instance(), id, gc, points, count.CoordModeOrigin);
      };
    }

    polygone::operator frameable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        XDrawLines(core::global::get_instance(), id, gc, points, count.CoordModeOrigin);
      };
    }

    polygone::operator fillable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
        XFillPolygon(core::global::get_instance(), id, gc, points, count. 0, CoordModeOrigin);
      };
    }
#endif // X11

    text_box::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
#ifdef WIN32
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
#elif X11
        // TBD!
        XDrawString(core::global::get_instance(), id, gc, rect.position().x, pos.position().y.str.c_str(), str.size());
#endif // X11
      };
    }

    text::operator drawable() const {
      return [&](core::drawable_id id, core::graphics_id gc, int) {
#ifdef WIN32
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
#elif X11
        // TBD!
        XDrawString(core::global::get_instance(), id, gc, pos.x, pos.y.str.c_str(), str.size());
#endif // X11
      };
    }

    template<bool freeObj, typename T
#ifdef WIN32
     = HGDIOBJ
#endif // WIN32
    >
    struct Use {
      inline Use(core::graphics_id id, const T& obj)
        : gc(id)
        , obj(obj) {
        old = set(obj);
      }

      T set(const T&);

      inline ~Use() {
        set(old);
#ifdef WIN32
        if (freeObj) {
          DeleteObject(obj);
        }
#endif // WIN32
      }

      core::graphics_id gc;
      T obj;
      T old;
    };

#ifdef WIN32

    template<> HGDIOBJ Use<false, HGDIOBJ>::set(const HGDIOBJ& hobj) {
      return SelectObject(gc, hobj);
    }

    template<> HGDIOBJ Use<true, HGDIOBJ>::set(const HGDIOBJ& hobj) {
      return SelectObject(gc, hobj);
    }

#elif X11

    template<bool freeObj> draw::pen Use<freeObj, draw::pen>::set(const draw::pen&) {
      draw::pen p(gc.foreground, gc.line_style, gc.line_width);
      XSetForeground(core::global::get_instance(), gc, pen.color());
      XSetLineAttributes(core::global::get_instance(), gc, pen.size(), pen.style());
      return old;
    }

    template<bool freeObj> draw::color Use<freeObj, draw::color>::set(const draw::color& color) {
      draw::color c(gc.background);
      XSetBackground(core::global::get_instance(), gc, color);
      XSetFillStyle(core::global::get_instance(), gc, FillSolid);
      return c;
    }

    template<bool freeObj> draw::font Use<freeObj, draw::font>::set(const draw::font& font) {
      draw::font f(gc.font);
      XSetFont(core::global::get_instance(), gc, font);
      return f;
    }

#endif // X11

    void graphics::drawPixel(const core::point &pt, const draw::color &color) {
#ifdef WIN32
      SetPixel(id, pt.x, pt.y, color);
#endif // WIN32
#ifdef X11
      XSetForeground(core::global::get_instance(), gc, color);
      XDrawPoint(core::global::get_instance(), win, gc, pt.x, pt.y)
#endif // X11
    }

    draw::color graphics::getPixel(const core::point &pt) const {
#ifdef WIN32
      return GetPixel(id, pt.x, pt.y);
#endif // WIN32
#ifdef X11
      return draw::color::black();
#endif // X11
    }

    void graphics::drawLine(const core::point &from, const core::point &to, const draw::pen& pen) {
      Use<false> p(id, pen);
#ifdef WIN32
      MoveToEx(id, from.x, from.y, NULL);
      LineTo(id, to.x, to.y);
#endif // WIN32
#ifdef X11
      XDrawLine(core::global::get_instance(), win, gc, from.x, from.y, to.x, to.y);
#endif // X11
    }

    void graphics::drawLines(std::vector<core::point> &points, const draw::pen& pen) {
      Use<false> p(id, pen);
#ifdef WIN32
      bool first = true;
      for (core::point pt : points) {
        if (first) {
          first = false;
          MoveToEx(id, pt.x, pt.y, NULL);
        } else {
          LineTo(id, pt.x, pt.y);
        }
      }
#endif // WIN32
#ifdef X11
      polygone poly(points);
      frame(poly, pen);
#endif // X11
    }

    void graphics::frame(const frameable& drawer, const draw::pen& pen) const {
      Use<false> p(id, pen);
#ifdef WIN32
      Use<false> b(id, GetStockObject(NULL_BRUSH));
#endif // WIN32
      drawer(win, id, detail::Frame);
    }

    void graphics::fill(const fillable& drawer, const draw::color& color) const {
#ifdef WIN32
      Use<true> b(id, CreateSolidBrush(color));
      Use<false> p(id, GetStockObject(NULL_PEN));
#endif // WIN32
#ifdef X11
      Use<false> p(id, color);
#endif // X11
      drawer(win, id, detail::Fill);
    }

    void graphics::draw(const drawable& drawer, const draw::color& color, const draw::pen& pen) const {
#ifdef WIN32
      Use<true> b(id, CreateSolidBrush(color));
#endif // WIN32
#ifdef X11
      Use<false> p(id, color);
#endif // X11
      Use<false> p(id, pen);
      drawer(win, id, detail::Draw);
    }

    void graphics::draw(const drawable& drawer, const draw::font& font, const draw::color& color) const {
      Use<false> f(id, font);
#ifdef WIN32
      core::color_type old_color = SetTextColor(id, color);
#endif // WIN32
#ifdef X11
      XSetForeground(core::global::get_instance(), gc, color);
#endif // X11
      drawer(win, id, detail::Draw);
#ifdef WIN32
      SetTextColor(id, old_color);
#endif // WIN32
    }

    void graphics::invert(const core::rectangle& r) const {
#ifdef WIN32
      RECT rect = r;
      InvertRect(id, &rect);
#endif // WIN32
    }

  }

}
