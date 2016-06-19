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

#ifdef X11
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

      round_rectangle::round_rectangle(const core::rectangle& rect, const core::size& size)
        : rect(rect)
        , size(size)
      {}

#ifdef WIN32
      void round_rectangle::operator() (core::drawable_id, core::graphics_id id) {
        RoundRect(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
      }
#endif // WIN32

#ifdef X11
      round_rectangle::operator drawable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XFillRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
            XDrawRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
        };
      }

      round_rectangle::operator frameable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XDrawRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
        };
      }

      round_rectangle::operator fillable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XFillRectangle(core::global::get_instance(), id, gc, rect.topleft.x, rect.topleft.y, rect.size().width, rect.size().height);
        };
      }

#endif // X11

    arc::arc(const core::point& pos, unsigned int radius, float startrad, float endrad)
      : pos(pos)
      , radius(radius)
      , startrad(startrad)
      , endrad(endrad)
    {}

#ifdef WIN32
    void arc::operator() (core::drawable_id, core::graphics_id id) {
      AngleArc(id, pos.x, pos.y, radius, startrad, endrad);
    }
#endif // WIN32
#ifdef X11
      arc::operator drawable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
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

#ifdef WIN32
    template<>
    void poly_function<int, Polyline>::operator() (core::drawable_id, core::graphics_id id) {
      Polyline(id, points, count);
    }

    template<>
    void poly_function<int, Polygon>::operator() (core::drawable_id, core::graphics_id id) {
      Polygon(id, points, count);
    }

    template<>
    void poly_function<DWORD, PolyBezier>::operator() (core::drawable_id, core::graphics_id id) {
      PolyBezier(id, points, (DWORD)count);
    }
#endif // WIN32
#ifdef X11
      polygone::operator drawable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XFillPolygon(core::global::get_instance(), id, gc, points, count. 0, CoordModeOrigin);
            XDrawLines(core::global::get_instance(), id, gc, points, count. CoordModeOrigin);
        };
      }

      polygone::operator frameable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XDrawLines(core::global::get_instance(), id, gc, points, count. CoordModeOrigin);
        };
      }

      polygone::operator fillable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XFillPolygon(core::global::get_instance(), id, gc, points, count. 0, CoordModeOrigin);
        };
      }
#endif // X11

#ifdef WIN32
    void text_box::operator() (core::drawable_id, core::graphics_id id) {
      int old_mode = SetBkMode(id, clear_background ? OPAQUE : TRANSPARENT);
      RECT Rect = rect;
      if ((origin & (DT_WORDBREAK | DT_VCENTER)) == (DT_WORDBREAK | DT_VCENTER)) {
        int h = DrawText(id, text.c_str(), (int)text.size(), &Rect, (UINT)origin | DT_CALCRECT);
        Rect.left = rect.topleft.x;
        Rect.right = rect.bottomright.x;
        Rect.top = (rect.size().height - h) / 2;
        Rect.bottom = Rect.top + h;
      }
      DrawText(id, text.c_str(), (int)text.size(), &Rect, (UINT)origin);
      SetBkMode(id, old_mode);
    }

    void text::operator() (core::drawable_id, core::graphics_id id) {
      int old_mode = SetBkMode(id, clear_background ? OPAQUE : TRANSPARENT);
      int px = pos.x;
      int py = pos.y;
      UINT old_align = top_left;

      switch (origin) {
        case top_left:
          old_align = SetTextAlign(id, TA_LEFT | TA_TOP | TA_NOUPDATECP);
          break;
        case top_right:
          old_align = SetTextAlign(id, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
          break;
        case top_hcenter:
          old_align = SetTextAlign(id, TA_CENTER | TA_TOP | TA_NOUPDATECP);
          break;
        case bottom_left:
          old_align = SetTextAlign(id, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case bottom_right:
          old_align = SetTextAlign(id, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case bottom_hcenter:
          old_align = SetTextAlign(id, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case vcenter_right: {
          SIZE sz;
          GetTextExtentPoint32(id, str.c_str(), (int)str.size(), &sz);
          py += sz.cy / 2;
          old_align = SetTextAlign(id, TA_RIGHT | TA_BASELINE | TA_NOUPDATECP);
          break;
        }
        case vcenter_left: {
          SIZE sz;
          GetTextExtentPoint32(id, str.c_str(), (int)str.size(), &sz);
          py += sz.cy / 2;
          old_align = SetTextAlign(id, TA_LEFT | TA_BASELINE | TA_NOUPDATECP);
          break;
        }
        case center: {
          SIZE sz;
          GetTextExtentPoint32(id, str.c_str(), (int)str.size(), &sz);
          py += sz.cy / 2;
          old_align = SetTextAlign(id, TA_CENTER | TA_BASELINE | TA_NOUPDATECP);
          break;
        }
      }
      TextOut(id, px, py, str.c_str(), (int)str.size());
      SetTextAlign(id, old_align);
      SetBkMode(id, old_mode);
    }
#endif // WIN32
#ifdef X11
      text_box::operator drawable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XDrawString(core::global::get_instance(), id, gc, rect.position().x, pos.position().y. str.c_str(), str.size());
        };
      }

      text::operator drawable() const {
        return [&](core::drawable_id id, core::graphics_id gc) {
            XDrawString(core::global::get_instance(), id, gc, pos.x, pos.y. str.c_str(), str.size());
        };
      }
#endif // X11

#ifdef WIN32
    template<bool freeObj = false>
    struct Use {
      inline Use(core::graphics_id id, HGDIOBJ obj)
        : id(id)
        , obj(obj) {
        old = SelectObject(id, obj);
      }

      inline ~Use() {
        SelectObject(id, old);
        if (freeObj) {
          DeleteObject(obj);
        }
      }

      core::graphics_id id;
      HGDIOBJ obj;
      HGDIOBJ old;
    };
#endif // WIN32
#ifdef X11
    template<bool freeObj, typename T>
    struct Use {
      inline Use(core::graphics_id id, const T& obj)
        : gc(id)
        , obj(obj) {
        old = set(obj);
      }

      T set(const T&);

      inline ~Use() {
        set(old);
      }

      core::graphics_id gc;
      T obj;
      T old;
    };

    template<bool freeObj> draw::pen Use<freeObj, draw::pen>::set(const draw::pen&) {
        draw::pen p(gc.foreground, gc.line_style, gc.line_width)
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

    void graphics::frame(frameable drawer, const draw::pen& pen) const {
      Use<false> p(id, pen);
#ifdef WIN32
      Use<false> b(id, GetStockObject(NULL_BRUSH));
#endif // WIN32
      drawer(win, id);
    }

    void graphics::fill(fillable drawer, const draw::color& color) const {
#ifdef WIN32
      Use<true> b(id, CreateSolidBrush(color));
      Use<false> p(id, GetStockObject(NULL_PEN));
#endif // WIN32
#ifdef X11
      Use<false> p(id, color);
#endif // X11
      drawer(win, id);
    }

    void graphics::draw(drawable drawer, const draw::color& color, const draw::pen& pen) const {
#ifdef WIN32
      Use<true> b(id, CreateSolidBrush(color));
#endif // WIN32
#ifdef X11
      Use<false> p(id, color);
#endif // X11
      Use<false> p(id, pen);
      drawer(win, id);
    }

    void graphics::draw(drawable drawer, const draw::font& font, const draw::color& color) const {
      Use<false> f(id, font);
#ifdef WIN32
      core::color_type old_color = SetTextColor(id, color);
#endif // WIN32
#ifdef X11
      XSetForeground(core::global::get_instance(), gc, color);
#endif // X11
      drawer(win, id);
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
