/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    gui lib
*
* Customer   -
*
* @brief     C++ API: color definition
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <vector>
#include <functional>
#include <string>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"
#include "color.h"
#include "pen.h"
#include "font.h"


namespace gui {

  namespace draw {

    typedef std::function<void(core::graphics_id)> drawable;

    template<BOOL(WINAPI Fnct)(HDC, int, int, int, int)>
    struct rect_function {
      inline rect_function(const core::rectangle& rect)
        : rect(rect)
      {}

      void operator() (core::graphics_id id) {
        Fnct(id, rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y);
      }

    private:
      const core::rectangle rect;
    };

    typedef rect_function<Rectangle> rectangle;
    typedef rect_function<Ellipse> ellipse;

    struct round_rectangle {
      round_rectangle(const core::rectangle& rect, const core::size& size);
      void operator() (core::graphics_id id);

    private:
      const core::rectangle rect;
      const core::size size;
    };

    struct arc {
      arc(const core::position& pos, unsigned int radius, float startrad, float endrad);
      void operator() (core::graphics_id id);

    private:
      const core::position pos;
      unsigned int radius;
      float startrad;
      float endrad;
    };

    template<BOOL(WINAPI Fnct)(HDC, int, int, int, int, int, int, int, int)>
    struct arc_function {
      inline arc_function(const core::rectangle& rect, const core::position& limit1, const core::position& limit2)
        : rect(rect)
        , limit1(limit1)
        , limit2(limit2)
      {}

      void operator() (core::graphics_id id) {
        Fnct(id,
             rect.topleft.x, rect.topleft.y, rect.bottomright.x, rect.bottomright.y,
             limit1.x, limit1.y, limit2.x, limit2.y);
      }

    private:
      const core::rectangle rect;
      const core::position limit1;
      const core::position limit2;
    };

    typedef arc_function<ArcTo> elliptic_arc;
    typedef arc_function<Pie> pie;
    typedef arc_function<Chord> chord;

    POINT *buildPoints(const std::vector<core::position>&, int&);

    template<typename T, BOOL(WINAPI Fnct)(HDC, CONST POINT *, T)>
    struct poly_function {
      poly_function(std::vector<core::position>& points)
        : points(buildPoints(points), count)
      {}

      ~poly_function() {
        delete [] points;
      }
      
      void operator() (core::graphics_id id);

    private:
      POINT *points;
      int count;
    };

    typedef poly_function<int, Polyline> polyline;
    typedef poly_function<int, Polygon> polygone;
    typedef poly_function<DWORD, PolyBezier> polybezier;

    enum text_origin {
      top_left = DT_TOP | DT_LEFT | DT_WORDBREAK,
      top_hcenter = DT_TOP | DT_CENTER | DT_WORDBREAK,
      top_right = DT_TOP | DT_RIGHT | DT_WORDBREAK,
      bottom_left = DT_BOTTOM | DT_SINGLELINE | DT_LEFT,
      bottom_hcenter = DT_BOTTOM | DT_SINGLELINE | DT_CENTER,
      bottom_right = DT_BOTTOM | DT_SINGLELINE | DT_RIGHT,
      vcenter_left = DT_SINGLELINE | DT_VCENTER | DT_LEFT,
      vcenter_right = DT_SINGLELINE | DT_VCENTER | DT_RIGHT,
      center = DT_SINGLELINE | DT_VCENTER | DT_CENTER,
      end_ellipsis = DT_END_ELLIPSIS,
      path_ellipsis = DT_PATH_ELLIPSIS,
      word_ellipsis = DT_WORD_ELLIPSIS,
      expand_tabs = DT_EXPANDTABS,
      undefined = -1
    };

    struct text_box {
      text_box(const std::string& text, const core::rectangle& rect, text_origin origin = top_left, bool clear_background = false)
        : text(text)
        , rect(rect)
        , origin(origin)
        , clear_background(clear_background)
      {}

      void operator() (core::graphics_id id);

    private:
      const std::string text;
      const core::rectangle rect;
      const text_origin origin;
      bool clear_background;
    };

    struct text {
      text(const std::string& str, const core::position& pos, text_origin origin = top_left, bool clear_background = false)
        : str(str)
        , pos(pos)
        , origin(origin)
        , clear_background(clear_background) {}

      void operator() (core::graphics_id id);

    private:
      const std::string str;
      const core::position pos;
      const text_origin origin;
      bool clear_background;
    };

    class graphics {
    public:
      graphics(core::graphics_id id)
        :id(id)
      {}

      void drawPixel(const core::position& pt, const draw::color& color);
      draw::color getPixel(const core::position&) const;

      void drawLine(const core::position& from, const core::position& to, const draw::pen& pen);
      void drawLines(std::vector<core::position>& points, const draw::pen& pen);

      void frame(drawable drawer, const draw::pen& pen) const;
      void fill(drawable drawer, const draw::color& color) const;
      void draw(drawable drawer, const draw::color& color, const draw::pen& pen) const;
      void draw(drawable drawer, const draw::font& font, const draw::color& color) const;

      void invert(const core::rectangle&) const;

    private:
      core::graphics_id id;

    };

  }

}
