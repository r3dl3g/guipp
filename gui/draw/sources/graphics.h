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
#include "font.h"


namespace gui {

  namespace draw {

    namespace detail {
      enum DrawType {
        Draw
      };
      enum FillType {
        Fill
      };
      enum FrameType {
        Frame
      };
    }

    typedef std::function<void(core::drawable_id, core::graphics_id, detail::DrawType)> drawable;
    typedef std::function<void(core::drawable_id, core::graphics_id, detail::FrameType)> frameable;
    typedef std::function<void(core::drawable_id, core::graphics_id, detail::FillType)> fillable;

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

#ifdef WIN32
      void operator() (core::drawable_id, core::graphics_id id, int);
#elif X11
      operator drawable() const;
      operator frameable() const;
      operator fillable() const;
#endif

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

#ifdef WIN32
      void operator() (core::drawable_id, core::graphics_id id, int);
#elif X11
      operator drawable() const;
      operator frameable() const;
      operator fillable() const;
#endif

    private:
      const core::rectangle rect;
    };

    struct round_rectangle {
      inline round_rectangle(const core::rectangle& rect, const core::size& size)
        : rect(rect)
        , size(size) {
      }

#ifdef WIN32
      void operator() (core::drawable_id, core::graphics_id id, int);
#elif X11
      operator drawable() const;
      operator frameable() const;
      operator fillable() const;
#endif

    private:
      const core::rectangle rect;
      const core::size size;
    };

    struct arc {
      arc(const core::point& pos, unsigned int radius, float startrad, float endrad);

#ifdef WIN32
      void operator() (core::drawable_id, core::graphics_id id, int);
#elif X11
      operator drawable() const;
      operator frameable() const;
      operator fillable() const;
#endif

    private:
      const core::point pos;
      unsigned int radius;
      float startrad;
      float endrad;
    };

    core::point_type *buildPoints(const std::vector<core::point>&, int&);

    struct polygone {
      polygone(std::vector<core::point>& points)
        : points(buildPoints(points, count)) {
      }

      polygone(const polygone&);

      ~polygone();

#ifdef WIN32
      void operator() (core::drawable_id, core::graphics_id id, int);
#elif X11
      operator drawable() const;
      operator frameable() const;
      operator fillable() const;
#endif

    private:
      core::point_type *points;
      int count;
    };

#ifdef X11
#define DT_TOP        0x00000000
#define DT_LEFT       0x00000000
#define DT_CENTER     0x00000001
#define DT_RIGHT      0x00000002
#define DT_VCENTER    0x00000004
#define DT_BOTTOM     0x00000008
#define DT_WORDBREAK  0x00000010
#define DT_SINGLELINE 0x00000020
#define DT_EXPANDTABS 0x00000040
#define DT_END_ELLIPSIS  0x00000100
#define DT_PATH_ELLIPSIS 0x00000200
#define DT_WORD_ELLIPSIS 0x00000400
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
      graphics(core::window_id win, core::graphics_id id)
        : id(id)
        , win(win) {
      }

      void drawPixel(const core::point& pt, const draw::color& color);
      draw::color getPixel(const core::point&) const;

      void drawLine(const core::point& from, const core::point& to, const draw::pen& pen);
      void drawLines(std::vector<core::point>& points, const draw::pen& pen);

      void frame(const frameable& drawer, const draw::pen& pen) const;
      void fill(const fillable& drawer, const draw::color& color) const;
      void draw(const drawable& drawer, const draw::color& color, const draw::pen& pen) const;
      void draw(const drawable& drawer, const draw::font& font, const draw::color& color) const;

      void invert(const core::rectangle&) const;

    private:
      core::graphics_id id;
      core::window_id win;

    };

  }

}
