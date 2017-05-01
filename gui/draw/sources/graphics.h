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
#include "brush.h"
#include "font.h"


namespace gui {

  namespace draw {

    class graphics;
    class bitmap;

    typedef void (drawable) (const graphics&, const brush&, const pen&);
    typedef void (frameable) (const graphics&, const pen&);
    typedef void (fillable) (const graphics&, const brush&);
    typedef void (textable) (const graphics&, const font& font, os::color color);
    typedef void (copyable) (const graphics&, const core::point&);

    // --------------------------------------------------------------------------
    class graphics {
    public:
      graphics (os::drawable target, os::graphics gc);
      graphics (os::drawable target);
      graphics (const graphics&);
      ~graphics();

      void operator= (const graphics&);

      const graphics& clear (os::color color) const;
      const graphics& draw_pixel (const core::point& pt,
                                  os::color color) const;

      os::color get_pixel (const core::point&) const;

      const graphics& draw_lines (std::initializer_list<core::point> points,
                                  const pen& pen) const;

      const graphics& frame (const std::function<frameable>&, const pen& pen) const;
      const graphics& fill (const std::function<fillable>&, const brush& brush) const;
      const graphics& draw (const std::function<drawable>&, const brush& brush, const pen& pen) const;
      const graphics& text (const std::function<textable>&, const font& font, os::color color) const;
      const graphics& copy (const std::function<copyable>&, const core::point&) const;

      const graphics& copy_from(const draw::bitmap&, const core::point& dest = core::point::zero) const;
      const graphics& copy_from (os::drawable, const core::rectangle& src,
                                 const core::point& dest = core::point::zero) const;
      const graphics& stretch_from (os::drawable, const core::rectangle& src,
                                    const core::rectangle& dest) const;

      void invert (const core::rectangle&) const;
      void flush () const;

      int depth () const;

      inline os::graphics os () const {
        return gc;
      }

      inline operator os::graphics () const {
        return gc;
      }

      inline operator os::drawable () const {
        return target;
      }

      static int get_drawable_depth (os::drawable);

#ifdef X11
      operator XftDraw* () const;
#endif // X11

    protected:
#ifdef X11
      XftDraw* get_xft () const;
#endif // X11

      friend struct clip;
      void push_clip_rectangle (const core::rectangle&) const;
      void pop_clip_rectangle () const;

    private:
      void destroy();

      os::graphics gc;
      os::drawable target;
      bool own_gc;
      bool ref_gc;
#ifdef WIN32
      mutable std::vector<HRGN> clipping_rectangles;
#endif // WIN32
#ifdef X11
      mutable std::vector<os::rectangle> clipping_rectangles;
#endif // X11
    };

    // --------------------------------------------------------------------------
    struct clip {
      inline clip (const graphics& g, const core::rectangle& r)
        : g(g) {
        g.push_clip_rectangle(r);
      }

      inline ~clip () {
        g.pop_clip_rectangle();
      }

    private:
      const graphics& g;

      void operator=(clip&) = delete;
    };

    // --------------------------------------------------------------------------
    struct line {
      inline line (const core::point& from,
                   const core::point& to)
        : from(from)
        , to(to)
      {}

      void operator() (const graphics&, const pen&) const;

    private:
      core::point from;
      core::point to;
    };

    // --------------------------------------------------------------------------
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

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
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

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
    };

    // --------------------------------------------------------------------------
    struct round_rectangle {
      inline round_rectangle(const core::rectangle& rect, const core::size& size)
        : rect(rect)
        , size(size) {
      }

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::rectangle rect;
      const core::size size;
    };

    // --------------------------------------------------------------------------
    struct arc {
      arc(const core::point& pos, unsigned int radius, float startrad, float endrad);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      const core::point pos;
      unsigned int radius;
      float start_radius;
      float end_radius;
    };

    // --------------------------------------------------------------------------
    struct polygon {
      polygon (const std::vector<core::point>& pts);
      polygon (std::initializer_list<core::point> pts);

      void operator() (const graphics&, const brush&, const pen&) const;
      void operator() (const graphics&, const pen&) const;
      void operator() (const graphics&, const brush&) const;

    private:
      std::vector<os::point> points;
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

    // --------------------------------------------------------------------------
    enum text_origin : unsigned int {
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
      undefined = 0xFFFF
    };

    // --------------------------------------------------------------------------
    struct text_box {
      text_box (const std::string& str, const core::rectangle& rect, text_origin origin = top_left, bool clear_background = false)
        : str(str)
        , rect(rect)
        , origin(origin)
        , clear_background(clear_background)
      {}

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::rectangle rect;
      const text_origin origin;
      bool clear_background;
    };

    // --------------------------------------------------------------------------
    struct bounding_box {
      bounding_box (const std::string& str, core::rectangle& rect, text_origin origin = top_left)
              : str(str)
              , rect(rect)
              , origin(origin)
      {}

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      core::rectangle& rect;
      const text_origin origin;

      // Not implemented!
      void operator= (const bounding_box&);
    };

    // --------------------------------------------------------------------------
    struct text {
      text(const std::string& str, const core::point& pos, text_origin origin = top_left, bool clear_background = false)
        : str(str)
        , pos(pos)
        , origin(origin)
        , clear_background(clear_background)
      {}

      void operator() (const graphics&, const font& font, os::color color) const;

    private:
      const std::string str;
      const core::point pos;
      const text_origin origin;
      bool clear_background;
    };

    // --------------------------------------------------------------------------
    namespace frame {

      inline void no_frame (const draw::graphics&, const core::rectangle&)
      {}

      void lines (const draw::graphics&, const core::rectangle&);
      void vline (const draw::graphics&, const core::rectangle&);
      void hline (const draw::graphics&, const core::rectangle&);

      void vraise (const draw::graphics&, const core::rectangle&);
      void hraise (const draw::graphics&, const core::rectangle&);

      void vgroove (const draw::graphics&, const core::rectangle&);
      void hgroove (const draw::graphics&, const core::rectangle&);

      void raised_relief (const draw::graphics&, const core::rectangle&);
      void sunken_relief (const draw::graphics&, const core::rectangle&);

      inline void relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          sunken_relief(g, r);
        } else {
          raised_relief(g, r);
        }
      }

      void raised_deep_relief (const draw::graphics&, const core::rectangle&);
      void sunken_deep_relief (const draw::graphics&, const core::rectangle&);

      inline void deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          sunken_deep_relief(g, r);
        } else {
          raised_deep_relief(g, r);
        }
      }

    } // frame
    // --------------------------------------------------------------------------

  } // fraw

} // gui
