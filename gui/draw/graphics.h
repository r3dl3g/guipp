/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/bind_method.h>
#include <gui/core/gui_types.h>
#include <gui/core/color.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>


namespace gui {

  namespace draw {

    class graphics;
    class pixmap;
    class basic_map;
    class masked_bitmap;

    typedef void (drawable) (const graphics&, const brush&, const pen&);
    typedef void (frameable) (const graphics&, const pen&);
    typedef void (fillable) (const graphics&, const brush&);
    typedef void (textable) (const graphics&, const font& font, os::color color);
    typedef void (copyable) (const graphics&, const core::point&);

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT graphics {
    public:
      graphics (os::drawable target, os::graphics gc);
      graphics (draw::pixmap& target);
      graphics (const graphics&);
      ~graphics ();

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

      const graphics& copy_from (const graphics&, const core::point& dest = core::point::zero) const;
      const graphics& copy_from (const graphics&, const core::rectangle& src, const core::point& dest = core::point::zero) const;

      const graphics& copy_from (const draw::pixmap&, const core::point& dest = core::point::zero) const;
      const graphics& copy_from (const draw::pixmap&, const core::rectangle& src, const core::point& dest) const;
      const graphics& copy_from (const draw::masked_bitmap&, const core::point& dest = core::point::zero) const;

      const graphics& copy_from (os::drawable, const core::rectangle& src,
                                 const core::point& dest = core::point::zero) const;
      const graphics& stretch_from (os::drawable, const core::rectangle& src,
                                    const core::rectangle& dest) const;

      void invert (const core::rectangle&) const;
      void flush () const;

      int depth () const;
      core::rectangle area () const;

      os::graphics os () const;
      operator os::graphics () const;
      operator os::drawable() const;

#ifdef X11
      operator XftDraw* () const;
#endif // X11

    protected:
#ifdef X11
      XftDraw* get_xft () const;
#endif // X11

      friend struct clip;
      void push_clipping (const core::rectangle&) const;
      void pop_clipping () const;
      void restore_clipping () const;

    private:
      void destroy ();

      os::graphics gc;
      os::drawable target;
      bool own_gc;
      bool ref_gc;
      mutable std::vector<IF_WIN32_ELSE(HRGN, os::rectangle)> clipping_stack;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT paint {
      typedef std::function<void (const draw::graphics&)> painter;

      paint (const painter& f);
      paint (painter&& f);

      template<typename T, typename F>
      paint (T* t, F f);

      void operator() (os::window, const os::graphics& g);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT buffered_paint {
      typedef std::function<void (const draw::graphics&)> painter;

      buffered_paint (const painter& f);
      buffered_paint (painter&& f);

      template<typename T, typename F>
      buffered_paint (T* t, F f);

      void operator() (os::window, const os::graphics& g);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clip {
      clip (const graphics& g, const core::rectangle& r);
      ~clip ();

    private:
      const graphics& g;

      void operator= (clip&) = delete;
    };

  } // draw

} // gui

#include <gui/draw/graphics.inl>
