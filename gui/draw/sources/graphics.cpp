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

  }

}
