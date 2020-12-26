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
#ifdef GUIPP_QT
# include <QtGui/QPainter>
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/bind_method.h>
#include <gui/core/gui_types.h>
#include <gui/core/color.h>
#include <gui/draw/draw_fwd.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    typedef void (drawable) (const graphics&, const brush&, const pen&);
    typedef void (frameable) (const graphics&, const pen&);
    typedef void (fillable) (const graphics&, const brush&);
    typedef void (textable) (const graphics&, const font& font, os::color color);
    typedef void (copyable) (const graphics&, const core::point&);

    enum class copy_mode : uint32_t {
      bit_copy =                IF_WIN32_X11_QT_ELSE(SRCCOPY,      GXcopy,         QPainter::CompositionMode_Source,              0),
      bit_and =                 IF_WIN32_X11_QT_ELSE(SRCAND,       GXand,          QPainter::RasterOp_SourceAndDestination,       1),
      bit_or =                  IF_WIN32_X11_QT_ELSE(SRCPAINT,     GXor,           QPainter::RasterOp_SourceOrDestination,        2),
      bit_xor =                 IF_WIN32_X11_QT_ELSE(SRCINVERT,    GXxor,          QPainter::RasterOp_SourceXorDestination,       3),
      bit_dest_clear =          IF_WIN32_X11_QT_ELSE(BLACKNESS,    GXclear,        QPainter::RasterOp_ClearDestination,           4),
      bit_dest_set =            IF_WIN32_X11_QT_ELSE(WHITENESS,    GXset,          QPainter::RasterOp_SetDestination,             5),
      bit_dest_invert =         IF_WIN32_X11_QT_ELSE(DSTINVERT,    GXinvert,       QPainter::RasterOp_NotDestination,             6),
      bit_not_src_or_dst =      IF_WIN32_X11_QT_ELSE(MERGEPAINT,   GXorInverted,   QPainter::RasterOp_NotSourceOrDestination,     7),
      bit_not_src =             IF_WIN32_X11_QT_ELSE(NOTSRCCOPY,   GXcopyInverted, QPainter::RasterOp_NotSource,                  8),
      bit_not_src_and_not_dst = IF_WIN32_X11_QT_ELSE(NOTSRCERASE,  GXnor,          QPainter::RasterOp_NotSourceAndNotDestination, 9),
      bit_not_src_and_dst =     IF_WIN32_X11_QT_ELSE(0x007700E6,   GXandInverted,  QPainter::RasterOp_NotSourceAndDestination,    10),
      bit_and_not =             IF_WIN32_X11_QT_ELSE(SRCERASE,     GXandReverse,   QPainter::RasterOp_SourceAndNotDestination,    11),
    };

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT graphics {
    public:
      typedef std::vector<core::rectangle> clipping_stack_t;

      graphics (os::drawable target, os::graphics gc);
      explicit graphics (draw::basic_map& target);
      graphics (const graphics&);
      ~graphics ();

      graphics& operator= (const graphics&);

      const graphics& clear (os::color color) const;
      const graphics& draw_pixel (const core::native_point& pt,
                                  os::color color) const;

      os::color get_pixel (const core::native_point&) const;

      const graphics& draw_lines (const std::vector<core::point>& points,
                                  const pen& pen) const;

      template<typename F> //frameable
      const graphics& frame (F, const pen& pen) const;

      template<typename F> //fillable
      const graphics& fill (F, const brush& brush) const;

      template<typename F> //drawable
      const graphics& draw (F, const brush& brush, const pen& pen) const;

      template<typename F> //textable
      const graphics& text (F, const font& font, os::color color) const;

      template<typename F> //copyable
      const graphics& copy (F, const core::point&) const;

      const graphics& copy_from (const graphics&, const core::point& dest) const;

      const graphics& copy_from (const draw::pixmap&, const core::point& dest) const;
      const graphics& copy_from (const draw::pixmap&, const core::rectangle& src, const core::point& dest) const;
      const graphics& copy_from (const draw::masked_bitmap&, const core::point& dest) const;

      const graphics& copy_from (const graphics&, const core::native_point& dest = core::native_point::zero) const;
      const graphics& copy_from (const graphics&, const core::native_rect& src, const core::native_point& dest = core::native_point::zero) const;

      const graphics& copy_from (const draw::pixmap&, const core::native_point& dest = core::native_point::zero) const;
      const graphics& copy_from (const draw::pixmap&, const core::native_rect& src, const core::native_point& dest) const;
      const graphics& copy_from (const draw::masked_bitmap&, const core::native_point& dest = core::native_point::zero) const;

      const graphics& copy_from (os::drawable, const core::rectangle& src,
                                 const core::point& dest = core::point::zero,
                                 const copy_mode = copy_mode::bit_copy) const;

      const graphics& copy_from (os::drawable, const core::native_rect& src,
                                 const core::native_point& dest = core::native_point::zero,
                                 const copy_mode = copy_mode::bit_copy) const;

      void invert (const core::rectangle&) const;
      void flush () const;

      int depth () const;
      core::rectangle area () const;
      core::native_rect native_area () const;

      os::graphics os () const;
      operator os::graphics () const;
      operator os::drawable() const;

#ifdef GUIPP_USE_XFT
      operator XftDraw* () const;
#endif // GUIPP_USE_XFT

    protected:
#ifdef GUIPP_USE_XFT
      XftDraw* get_xft () const;
#endif // GUIPP_USE_XFT


      friend struct clip;
      void push_clipping (const core::rectangle&) const;
      void pop_clipping () const;

      void set_clip_rect (const core::rectangle&) const;
      void restore_clipping () const;
      void clear_clip_rect () const;

    private:
      void destroy ();

      os::drawable target;
      os::graphics gc;
      bool own_gc;
      bool ref_gc;
      mutable clipping_stack_t clipping_stack;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT paint {
      typedef std::function<void (const draw::graphics&)> painter;

      explicit paint (const painter& f);
      explicit paint (painter&& f);

      template<typename T, typename F>
      paint (T* t, F f);

      void operator() (os::window, os::graphics g);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT buffered_paint {
      typedef std::function<void (const draw::graphics&)> painter;

      explicit buffered_paint (const painter& f);
      explicit buffered_paint (painter&& f);

      template<typename T, typename F>
      buffered_paint (T* t, F f);

      void operator() (os::window, os::graphics g);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clip {
      clip (const graphics& g, const core::rectangle& r);
      ~clip ();

      void operator= (clip&) = delete;

    private:
      const graphics& g;
    };

  } // draw

} // gui

#include <gui/draw/graphics.inl>
