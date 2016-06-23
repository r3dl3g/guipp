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
#include <boost/serialization/strong_typedef.hpp>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"
#include "color.h"
#include "pen.h"
#include "brush.h"
#include "font.h"


namespace gui {

  namespace draw {

    typedef std::function<void(core::drawable_id,
                               core::graphics_id,
                               const pen&)> frameable;
    typedef std::function<void(core::drawable_id,
                               core::graphics_id,
                               const brush&)> fillable;
    typedef std::function<void(core::drawable_id,
                               core::graphics_id,
                               const brush&,
                               const pen&)> drawable;

    struct rectangle {
      inline rectangle(const core::rectangle& rect)
        : rect(rect) {
      }

      inline rectangle(const core::point& pos,
                       const core::size& sz)
        : rect(pos, sz) {
      }

      inline rectangle(const core::point& pos1,
                       const core::point& pos2)
        : rect(pos1, pos2) {
      }

      operator drawable() const;
      operator frameable() const;
      operator fillable() const;

    private:
      const core::rectangle rect;
    };

    struct ellipse {
      inline ellipse(const core::rectangle& rect)
        : rect(rect) {
      }

      inline ellipse(const core::point& pos,
                     const core::size& sz)
                     : rect(pos, sz) {
      }

      inline ellipse(const core::point& pos1,
                     const core::point& pos2)
                     : rect(pos1, pos2) {
      }

      operator drawable() const;
      operator frameable() const;
      operator fillable() const;

    private:
      const core::rectangle rect;
    };

    struct round_rectangle {
      inline round_rectangle(const core::rectangle& rect, const core::size& size)
        : rect(rect)
        , size(size) {
      }

      operator drawable() const;
      operator frameable() const;
      operator fillable() const;

    private:
      const core::rectangle rect;
      const core::size size;
    };

    struct arc {
      arc(const core::point& pos, unsigned int radius, float startrad, float endrad);

      operator drawable() const;
      operator frameable() const;
      operator fillable() const;

    private:
      const core::point pos;
      unsigned int radius;
      float startrad;
      float endrad;
    };

    core::point_type *buildPoints(const std::vector<core::point>&, int&);

    struct polygone {
      polygone(const std::vector<core::point>& points)
        : points(buildPoints(points, count)) {
      }

      polygone(const polygone&);

      ~polygone();

      operator drawable() const;
      operator frameable() const;
      operator fillable() const;

    private:
      core::point_type *points;
      int count;
    };

#ifdef X11
#     define DT_TOP           0x0000
#     define DT_LEFT          0x0000
#     define DT_CENTER        0x0001
#     define DT_RIGHT         0x0002
#     define DT_VCENTER       0x0004
#     define DT_BOTTOM        0x0008
#     define DT_WORDBREAK     0x0010
#     define DT_SINGLELINE    0x0020
#     define DT_EXPANDTABS    0x0040
#     define DT_END_ELLIPSIS  0x0100
#     define DT_PATH_ELLIPSIS 0x0200
#     define DT_WORD_ELLIPSIS 0x0400
#endif // X11

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

      operator drawable() const;

    private:
      const std::string text;
      const core::rectangle rect;
      const text_origin origin;
      bool clear_background;
    };

    struct text {
      text(const std::string& str, const core::point& pos, text_origin origin = top_left, bool clear_background = false)
        : str(str)
        , pos(pos)
        , origin(origin)
        , clear_background(clear_background)
      {}

      operator drawable() const;

    private:
      const std::string str;
      const core::point pos;
      const text_origin origin;
      bool clear_background;
    };

    class graphics {
    public:
      graphics(core::window_id win, core::graphics_id gc)
        : gc(gc)
        , win(win) {
      }

      void drawPixel(const core::point& pt, const color& color);
      color getPixel(const core::point&) const;

      void drawLine(const core::point& from, const core::point& to, const pen& pen);
      void drawLines(std::vector<core::point>& points, const pen& pen);

      void frame(const frameable& drawer, const pen& pen) const;
      void fill(const fillable& drawer, const brush& brush) const;
      void draw(const drawable& drawer, const brush& brush, const pen& pen) const;
      void draw(const drawable& drawer, const font& font, const color& color) const;

      void invert(const core::rectangle&) const;

    private:
      core::graphics_id gc;
      core::window_id win;

    };

  }

}
