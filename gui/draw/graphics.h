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
#include <util/bind_method.h>
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

    enum class copy_mode : uint32_t {
      bit_copy = IF_WIN32_ELSE(SRCCOPY, GXcopy),
      bit_and = IF_WIN32_ELSE(SRCAND, GXand),
      bit_or = IF_WIN32_ELSE(SRCPAINT, GXor),
      bit_xor = IF_WIN32_ELSE(SRCINVERT, GXxor),
      bit_dest_clear = IF_WIN32_ELSE(BLACKNESS, GXclear),
      bit_dest_set = IF_WIN32_ELSE(WHITENESS, GXset),
      bit_dest_invert = IF_WIN32_ELSE(DSTINVERT, GXinvert),
      bit_not_src_or_dst = IF_WIN32_ELSE(MERGEPAINT, GXorInverted),
      bit_not_src = IF_WIN32_ELSE(NOTSRCCOPY, GXcopyInverted),
      bit_not_src_and_not_dst = IF_WIN32_ELSE(NOTSRCERASE, GXnor),
      bit_and_not = IF_WIN32_ELSE(SRCERASE, GXandReverse),
    };

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT graphics {
    public:
      typedef std::vector<core::rectangle> clipping_stack_t;

      graphics (os::drawable target, os::graphics gc);
      graphics (draw::basic_map& target);
      graphics (const graphics&);
      ~graphics ();

      void operator= (const graphics&);

      const graphics& clear (os::color color) const;
      const graphics& draw_pixel (const core::native_point& pt,
                                  os::color color) const;

      os::color get_pixel (const core::native_point&) const;

      const graphics& draw_lines (std::vector<core::point> points,
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

#ifdef USE_XFT
      operator XftDraw* () const;
#endif // USE_XFT

    protected:
#ifdef USE_XFT
      XftDraw* get_xft () const;
#endif // USE_XFT


      friend struct clip;
      void push_clipping (const core::rectangle&) const;
      void pop_clipping () const;

      void set_clip_rect (const core::rectangle&) const;
      void clear_clip_rect () const;

    private:
      void destroy ();

      os::graphics gc;
      os::drawable target;
      bool own_gc;
      bool ref_gc;
      mutable clipping_stack_t clipping_stack;
    };

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT paint {
      typedef std::function<void (const draw::graphics&)> painter;

      paint (const painter& f);
      paint (painter&& f);

      template<typename T, typename F>
      paint (T* t, F f);

      void operator() (os::window, os::graphics g);

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

      void operator() (os::window, os::graphics g);

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
