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

    round_rectangle::round_rectangle(const core::rectangle& rect, const core::size& size)
      : rect(rect)
      , size(size)
    {}

    void round_rectangle::operator() (core::graphics_id id) {
      RoundRect(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y, size.width, size.height);
    }

    arc::arc(const core::position& pos, unsigned int radius, float startrad, float endrad)
      : pos(pos)
      , radius(radius)
      , startrad(startrad)
      , endrad(endrad)
    {}

    void arc::operator() (core::graphics_id id) {
      AngleArc(id, pos.x, pos.y, radius, startrad, endrad);
    }

    POINT *buildPoints(const std::vector<core::position>& pts, int& size) {
      size = (int)pts.size();
      POINT *points = new POINT[size];
      for (int i = 0; i < size; ++i) {
        points[i] = pts[i];
      }
      return points;
    }

    template<>
    void poly_function<int, Polyline>::operator() (core::graphics_id id) {
      Polyline(id, points, count);
    }

    template<>
    void poly_function<int, Polygon>::operator() (core::graphics_id id) {
      Polygon(id, points, count);
    }

    template<>
    void poly_function<DWORD, PolyBezier>::operator() (core::graphics_id id) {
      PolyBezier(id, points, (DWORD)count);
    }

    void text_box::operator() (core::graphics_id id) {
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

    void text::operator() (core::graphics_id id) {
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

    void graphics::drawPixel(const core::position &pt, const draw::color &color) {
      SetPixel(id, pt.x, pt.y, color);
    }
    
    draw::color graphics::getPixel(const core::position &pt) const {
      return GetPixel(id, pt.x, pt.y);
    }

    void graphics::drawLine(const core::position &from, const core::position &to, const draw::pen& pen) {
      Use<false> p(id, pen);
      MoveToEx(id, from.x, from.y, NULL);
      LineTo(id, to.x, to.y);
    }
    
    void graphics::drawLines(std::vector<core::position> &points, const draw::pen& pen) {
      Use<false> p(id, pen);
      bool first = true;
      for (core::position pt : points) {
        if (first) {
          first = false;
          MoveToEx(id, pt.x, pt.y, NULL);
        } else {
          LineTo(id, pt.x, pt.y);
        }
      }
    }

    void graphics::frame(drawable drawer, const draw::pen& pen) const {
      Use<false> p(id, pen);
      Use<false> b(id, GetStockObject(NULL_BRUSH));
      drawer(id);
    }

    void graphics::fill(drawable drawer, const draw::color& color) const {
      Use<true> b(id, CreateSolidBrush(color));
      Use<false> p(id, GetStockObject(NULL_PEN));
      drawer(id);
    }

    void graphics::draw(drawable drawer, const draw::color& color, const draw::pen& pen) const {
      Use<true> b(id, CreateSolidBrush(color));
      Use<false> p(id, pen);
      drawer(id);
    }

    void graphics::draw(drawable drawer, const draw::font& font, const draw::color& color) const {
      core::color_type old_color = SetTextColor(id, color);
      Use<false> f(id, font);
      drawer(id);
      SetTextColor(id, old_color);
    }

    void graphics::invert(const core::rectangle& r) const {
      RECT rect = r;
      InvertRect(id, &rect);
    }

  }

}
