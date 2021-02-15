/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <gui/core/context.h>
#include <gui/core/color.h>
#include <gui/draw/draw_fwd.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    typedef void (drawable) (graphics&, const brush&, const pen&);
    typedef void (frameable) (graphics&, const pen&);
    typedef void (fillable) (graphics&, const brush&);
    typedef void (textable) (graphics&, const font& font, os::color color);
    typedef void (copyable) (graphics&, const core::point&);

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
      graphics (core::context* ctx);
      explicit graphics (draw::basic_map& target);
      ~graphics ();

      graphics& clear (os::color color);
      graphics& draw_pixel (const core::native_point& pt,
                                  os::color color);

      os::color get_pixel (const core::native_point&) const;

      graphics& draw_lines (const std::vector<core::point>& points,
                                  const pen& pen);

      template<typename F> //frameable
      graphics& frame (F, const pen& pen);

      template<typename F> //fillable
      graphics& fill (F, const brush& brush);

      template<typename F> //drawable
      graphics& draw (F, const brush& brush, const pen& pen);

      template<typename F> //textable
      graphics& text (F, const font& font, os::color color);

      template<typename F> //copyable
      graphics& copy (F, const core::point&);

      graphics& copy_from (graphics&, const core::point& dest);

      graphics& copy_from (const draw::pixmap&, const core::point& dest);
      graphics& copy_from (const draw::pixmap&, const core::rectangle& src, const core::point& dest);
      graphics& copy_from (const draw::masked_bitmap&, const core::point& dest);

      graphics& copy_from (graphics&, const core::native_point& dest = core::native_point::zero);
      graphics& copy_from (graphics&, const core::native_rect& src, const core::native_point& dest = core::native_point::zero);

      graphics& copy_from (const draw::pixmap&, const core::native_point& dest = core::native_point::zero);
      graphics& copy_from (const draw::pixmap&, const core::native_rect& src, const core::native_point& dest);
      graphics& copy_from (const draw::masked_bitmap&, const core::native_point& dest = core::native_point::zero);

      graphics& copy_from (os::drawable, const core::rectangle& src,
                                 const core::point& dest = core::point::zero,
                                 const copy_mode = copy_mode::bit_copy);

      graphics& copy_from (os::drawable, const core::native_rect& src,
                                 const core::native_point& dest = core::native_point::zero,
                                 const copy_mode = copy_mode::bit_copy);

      void invert (const core::rectangle&);
      void flush ();

      int depth () const;
      core::rectangle area () const;
      core::native_rect native_area () const;

      core::native_point offset () const;
      void set_offset (const core::native_point& o);

      os::graphics os () const;
      operator os::graphics () const;
      operator os::drawable () const;

      os::graphics gc () const;
      os::drawable target () const;

      const core::context& context () const;
      core::context& context ();

#ifdef GUIPP_USE_XFT
      operator XftDraw* () const;
#endif // GUIPP_USE_XFT

    protected:
#ifdef GUIPP_USE_XFT
      XftDraw* get_xft () const;
#endif // GUIPP_USE_XFT



    private:
      void destroy ();

      core::context* ctx;
      core::native_point offs;
      bool own_gctx;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT paint {
      typedef std::function<void (draw::graphics&)> painter;

      explicit paint (const painter& f);
      explicit paint (painter&& f);

      template<typename T, typename F>
      paint (T* t, F f);

      void operator() (core::context*, core::native_rect*);

    private:
      painter p;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT clip : public core::clip {
      clip (graphics& g, const core::rectangle& r);
    };

  } // draw

} // gui

#include <gui/draw/graphics.inl>
