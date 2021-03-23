/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <array>
#ifdef GUIPP_X11
# include <algorithm>
#endif // GUIPP_X11
#ifdef GUIPP_QT
# include <QtGui/QBitmap>
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#ifdef GUIPP_WIN
# include <util/string_util.h>
#endif // GUIPP_WIN

#include <gui/core/native.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/drawers.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/use.h>

#ifdef GUIPP_X11
// For debugging
std::ostream& operator<< (std::ostream& out, const XGCValues& v) {
  out << "{function="    << std::hex << v.function
      << ", plane_mask=" << v.plane_mask
      << ", foreground=" << v.foreground
      << ", background=" << v.background
      << ", line_width=" << v.line_width
      << ", line_style=" << v.line_style
      << ", cap_style="  << v.cap_style
      << ", fill_style=" << v.fill_style
      << ", fill_rule="  << v.fill_rule
      << ", arc_mode="   << v.arc_mode
      << ", tile="       << v.tile
      << ", stipple="    << v.stipple
      << ", ts_x_origin="<< v.ts_x_origin
      << ", ts_y_origin="<< v.ts_y_origin
      << ", font="       << v.font
      << ", subwindow_mode=" << v.subwindow_mode
      << ", graphics_exposures="<< v.graphics_exposures
      << ", clip_x_origin=" << v.clip_x_origin
      << ", clip_y_origin=" << v.clip_y_origin
      << ", clip_mask="     << v.clip_mask
      << ", dash_offset="   << v.dash_offset
      << ", dashes="        << (int)v.dashes
      << "}";
  return out;
}
#endif // GUIPP_X11


namespace gui {

  namespace draw {

#ifdef GUIPP_WIN
    // --------------------------------------------------------------------------
    graphics::graphics (core::context* ctx)
      : ctx(ctx)
      , own_gctx(false)
    {}

    graphics::graphics (draw::basic_map& target)
      : ctx(0)
      , own_gctx(true)
    {
      ctx = new core::context(target);
    }

    graphics::~graphics () {
      destroy();
    }

    void graphics::destroy () {
      if (ctx) {
        if (own_gctx) {
          delete ctx;
        }
        ctx = 0;
      }
      own_gctx = false;
    }

    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      SetPixel(gc(), pt.x(), pt.y(), c);
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      return GetPixel(gc(), pt.x(), pt.y());
    }

    graphics& graphics::draw_lines (const std::vector<core::point>& points,
                                          const pen& p) {
      Use<pen> pn(gc(), p);
      const auto off = p.os_size() / 2;
      bool first = true;
      for (const core::point& pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc(), pt.os_x(context()) + off, pt.os_y(context()) + off, nullptr);
        } else {
          LineTo(gc(), pt.os_x(context()) + off, pt.os_y(context()) + off);
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                  src, r.x(), r.y(), SRCCOPY)) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }

    graphics& graphics::copy_from (os::drawable w,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc());
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }

    /*
    graphics& graphics::stretch_from (os::drawable w,
                                            const core::rectangle& src,
                                            const core::rectangle& dst) {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc());
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!StretchBlt(gc(), dst.os_x(), dst.os_y(), dst.os_width(), dst.os_height(),
                        source_gc, src.os_x(), src.os_y(), src.os_width(), src.os_height(), SRCCOPY)) {
          throw std::runtime_error("graphics::stretch_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!StretchBlt(gc(), dst.os_x(), dst.os_y(), dst.os_width(), dst.os_height(),
                        source_gc, src.os_x(), src.os_y(), src.os_width(), src.os_height(), SRCCOPY)) {
          throw std::runtime_error("graphics::stretch_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }
    */

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      if (bmp.mask) {
        HDC dc = CreateCompatibleDC(gc());
        SelectObject(dc, bmp.mask.get_os_bitmap());
        core::native_size sz = bmp.mask.native_size();
        BitBlt(gc(), pt.x(), pt.y(), sz.width(), sz.height(), dc, 0, 0, static_cast<int>(copy_mode::bit_and)); //DSna, https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations
        if (bmp.image) {
          sz = bmp.image.native_size();
          SelectObject(dc, bmp.image.get_os_bitmap());
          BitBlt(gc(), pt.x(), pt.y(), sz.width(), sz.height(), dc, 0, 0, static_cast<int>(copy_mode::bit_or));  // DSo
        }
        DeleteDC(dc);
      } else {
        core::native_size sz = bmp.image.native_size();
        copy_from(bmp.image, core::native_rect(sz), pt);
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      RECT rect = r.os(context());
      InvertRect(gc(), &rect);
    }

    void graphics::flush () {
      GdiFlush();
    }

    int graphics::depth () const {
      return GetDeviceCaps(gc(), BITSPIXEL);
    }

    core::native_rect graphics::native_area () const {
      core::native_rect r = {0, 0, 0, 0};
      os::window id = WindowFromDC(gc());
      if (id) {
        RECT s;
        GetWindowRect(id, &s);
        r.width(std::abs(s.right - s.left));
        r.height(std::abs(s.bottom - s.top));
      } else {
        HGDIOBJ hBmp = GetCurrentObject(gc(), OBJ_BITMAP);
        if (hBmp) {
          BITMAP bmp;
          memset(&bmp, 0, sizeof (BITMAP));
          GetObject(hBmp, sizeof (BITMAP), &bmp);
          r.width(bmp.bmWidth);
          r.height(bmp.bmHeight);
        } else {
          r.width(GetDeviceCaps(gc(), HORZRES));
          r.height(GetDeviceCaps(gc(), VERTRES));
        }
      }
      return r;
    }

#endif // GUIPP_WIN

#ifdef GUIPP_X11
    // --------------------------------------------------------------------------
    using namespace core::global;

    // --------------------------------------------------------------------------
    int get_drawable_depth (os::drawable t) {
      Window root;
      int x, y;
      unsigned int w, h, b, d;
      XGetGeometry(get_instance(), t, &root, &x, &y, &w, &h, &b, &d);
      return d;
    }

    core::native_rect get_drawable_area (os::drawable t) {
      Window root;
      int x, y;
      unsigned int w, h, b, d;
      XGetGeometry(get_instance(), t, &root, &x, &y, &w, &h, &b, &d);
      return {
          (core::native_point::type)0,
          (core::native_point::type)0,
          (core::native_size::type)w,
          (core::native_size::type)h
      };
    }

    // --------------------------------------------------------------------------
    graphics::graphics (core::context* ctx)
      : ctx(ctx)
      , own_gctx(false)
    {}

    graphics::graphics (draw::basic_map& target)
      : ctx(0)
      , own_gctx(true)
    {
      ctx = new core::context(target);
    }

    graphics::~graphics () {
      flush();
      destroy();
    }

    void graphics::destroy () {
      if (ctx) {
        if (own_gctx) {
          delete ctx;
        }
        ctx = 0;
      }
      own_gctx = false;
    }

    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      Use<pen> pn(gc(), pen(c));
      XDrawPoint(get_instance(), target(), gc(), pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      XImage* im = XGetImage(get_instance(), target(), pt.x(), pt.y(), 1, 1, AllPlanes, ZPixmap);
      os::color c = color::black;
      if (im) {
        if (im->bits_per_pixel == 1) {
          c = XGetPixel(im, 0, 0) ? color::white : color::black;
        } else {
          c = XGetPixel(im, 0, 0);
        }
        XDestroyImage(im);
      }
      return c;
    }

    os::point operator+ (const os::point& pt, short i) {
      return { (short)(pt.x + i), (short)(pt.y + i) };
    }

    graphics& graphics::draw_lines (const std::vector<core::point>& pts,
                                          const pen& p) {

      Use<pen> pn(gc(), p);
      const short off = p.os_size() / 2;

      std::vector<os::point> points;
      points.reserve(pts.size());
      for (const core::point& pt : pts) {
        points.push_back(pt.os(context()) + off);
      }
      XDrawLines(get_instance(), target(), gc(),
                 points.data(), (int)points.size(),
                 CoordModeOrigin);
      return *this;
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      int res = XCopyArea(get_instance(), src, target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
      if (!res) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }


    graphics& graphics::copy_from (os::drawable w,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) {
      const int dd = get_drawable_depth(w);
      const int md = depth();
      if (dd == md) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        values.graphics_exposures = False;
        XChangeGC(display, gc(), GCFunction|GCGraphicsExposures, &values);
        /*int res =*/ XCopyArea(get_instance(), w, target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
        values = { GXcopy }; // .function =
        XChangeGC(display, gc(), GCFunction, &values);
      } else if (1 == dd) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        values.graphics_exposures = False;
        XChangeGC(display, gc(), GCFunction|GCGraphicsExposures, &values);
        /*int res =*/ XCopyPlane(get_instance(), w, target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y(), 1);
        values = { GXcopy }; // .function =
        XChangeGC(display, gc(), GCFunction, &values);
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      auto display = core::global::get_instance();
      int res = 0;
      // If previous clipping intersect with this region, to much of the image is drawn.
      // -> restrict the copy or fill area to the intersection rectangle withthe previous clipping rectangle.
      core::native_rect clip;
      if (bmp.mask) {
        if (!ctx->clipping().empty()) {
          clip = core::native_rect(ctx->clipping().back(), context());
        }
        XSetClipMask(display, gc(), bmp.mask.get_os_bitmap());
        XSetClipOrigin(display, gc(), pt.x(), pt.y());
      }

      if (bmp.image) {
        core::native_rect src(bmp.image.native_size());
        res = XCopyArea(get_instance(), bmp.image, target(), gc(), src.x(), src.y(), src.width(), src.height(), pt.x(), pt.y());
//        core::native_rect src(pt, bmp.image.native_size());
//        if (!clip.empty()) {
//          src &= clip;
//        }
//        res = XCopyArea(get_instance(), bmp.image, target(), gc(), src.x() - pt.x(), src.y() - pt.x(), src.width(), src.height(), src.x(), src.y());
      } else {
        Use<brush> br(gc(), color::black);

        core::native_rect src(pt, bmp.mask.native_size());
        if (!clip.empty()) {
          src &= clip;
        }

        XFillRectangle(display, target(), gc(), src.x(), src.y(), src.width(), src.height());
      }

      if (bmp.mask) {
        ctx->restore_clipping();
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      auto display = core::global::get_instance();

      XGCValues values = { GXinvert, 0 };  // .function =
      values.graphics_exposures = False;
      XChangeGC(display, gc(), GCFunction, &values);
      XFillRectangle(display, target(), gc(), r.os_x(context()), r.os_y(context()), r.os_width(), r.os_height());
      values = { GXcopy, 0 }; // .function =
      XChangeGC(display, gc(), GCFunction, &values);
    }

    void graphics::flush () {
      XFlushGC(get_instance(), gc());
    }

    int graphics::depth () const {
      return get_drawable_depth(target());
    }

    core::native_rect graphics::native_area () const {
      return get_drawable_area(target());
    }

#ifdef GUIPP_USE_XFT
    XftDraw* graphics::get_xft () const {
      return core::native::x11::get_xft_draw(*ctx);
    }

    graphics::operator XftDraw* () const {
      return get_xft();
    }
#endif // GUIPP_USE_XFT

#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    graphics::graphics (core::context* ctx)
      : ctx(ctx)
      , own_gctx(false)
    {}

    graphics::graphics (draw::basic_map& target)
      : ctx(0)
      , own_gctx(true)
    {
      ctx = new core::context(target);
      gc()->begin(target);
    }

    graphics::~graphics () {
      destroy();
    }

    void graphics::destroy () {
      if (ctx) {
        if (own_gctx) {
          gc()->end();
          delete ctx;
        }
        ctx = 0;
      }
      own_gctx = false;
    }

    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      gc()->setPen(c);
      gc()->drawPoint(pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      QWindow* w = dynamic_cast<QWindow*>(target());
      if (w) {
        QPixmap px;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#elif QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
        px = w->grab({pt.x(), pt.y(), 1, 1});
#else
        px.grabWindow(w->winId(), pt.x(), pt.y(), 1, 1);
#endif
        return px.toImage().pixel(0, 0);
      } else {
        QPixmap* p = dynamic_cast<QPixmap*>(target());
        if (p) {
          if (p->depth() == 1) {
            return p->toImage().pixelIndex(pt.x(), pt.y()) ? color::white : color::black;
          } else {
            return p->toImage().pixel(pt.x(), pt.y());
          }
        }
      }
      return color::black;
    }

    os::point operator+ (const os::point& pt, int i) {
      return { (pt.x() + i), (pt.y() + i) };
    }

    graphics& graphics::draw_lines (const std::vector<core::point>& points,
                                          const pen& p) {
      Use<pen> pn(gc(), p);
      QVector<os::point> pointPairs;
      const auto off = p.os_size() / 2;
      bool first = true;
      os::point last;
      for (const core::point& pt : points) {
        if (first) {
          first = false;
          last = pt.os(context()) + off;
        } else {
          pointPairs.push_back(last);
          last = pt.os(context()) + off;
          pointPairs.push_back(last);
        }
      }
      gc()->drawLines(pointPairs);
      return *this;
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      return copy_from(src.target(), r, pt);
    }

    graphics& graphics::copy_from (os::drawable d,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) {
      QWindow* w = dynamic_cast<QWindow*>(d);
      if (w) {
        QPixmap pix;
        pix.grabWindow(w->winId(), r.x(), r.y(), r.width(), r.height());
        const QPainter::CompositionMode oldMode = gc()->compositionMode();
        gc()->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
        gc()->drawPixmap(pt.x(), pt.y(), pix);
        gc()->setCompositionMode(oldMode);
      } else {
        QPixmap* p = dynamic_cast<QPixmap*>(d);
        if (p) {
          const QPainter::CompositionMode oldMode = gc()->compositionMode();
          gc()->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
          gc()->drawPixmap(pt.x(), pt.y(), *p);
          gc()->setCompositionMode(oldMode);
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      if (bmp.mask && bmp.image) {
        QRegion clip(*bmp.mask.get_os_bitmap());
        clip.translate(pt.x(), pt.y());
        gc()->setClipRegion(clip);
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.image.get_os_bitmap());
        context().restore_clipping();
      } else if (bmp.image) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.image.get_os_bitmap());
      } else if (bmp.mask) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.mask.get_os_bitmap());
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) {
      if (bmp) {
        const auto r = core::global::scale_to_native(src);
        const auto p = pt.os(context());
        gc()->drawPixmap(p.x(), p.y(), *bmp.get_os_bitmap(),
                         r.x(), r.y(), r.width(), r.height());
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) {
      if (bmp) {
        gc()->drawPixmap(pt.x(), pt.y(), *bmp.get_os_bitmap(), src.x(), src.y(), src.width(), src.height());
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      const QPainter::CompositionMode oldMode = gc()->compositionMode();
      gc()->setCompositionMode(QPainter::RasterOp_NotDestination);
      gc()->fillRect(r.os(context()), Qt::white);
      gc()->setCompositionMode(oldMode);
    }

    void graphics::flush () {
    }

    int graphics::depth () const {
      return gc()->device()->depth();
    }

    core::native_rect graphics::native_area () const {
      auto vp = gc()->viewport();
      return core::native_rect(vp.x(),
                               vp.y(),
                               static_cast<core::native_rect::size_type>(vp.width()),
                               static_cast<core::native_rect::size_type>(vp.height()));
    }

#endif // GUIPP_QT
    // --------------------------------------------------------------------------

    graphics& graphics::clear (os::color color) {
#ifdef GUIPP_QT
      gc()->fillRect(native_area().os(context()), color);
      return *this;
#else
      return draw(rectangle(area()), color, color);
#endif // GUIPP_QT
    }

    graphics& graphics::erase (const core::rectangle& r, os::color color) {
      return draw(rectangle(r), color, color);
    }

    core::rectangle graphics::area () const {
      return core::global::scale_from_native(native_area());
    }

    graphics& graphics::copy_from (graphics& src, const core::point& pt) {
      return copy_from(src, src.native_area(), core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (graphics& src, const core::native_point& pt) {
      return copy_from(src, src.native_area(), pt);
    }

    graphics& graphics::copy_from (os::drawable w,
                                   const core::rectangle& r,
                                   const core::point& pt,
                                   const copy_mode mode) {
      return copy_from(w, core::global::scale_to_native(r), core::native_point(pt.os(context()), context()), mode);
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) {
      return copy_from(bmp, core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_point& pt) {
      return copy_from(bmp, core::native_rect(bmp.native_size()), pt);
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::point& pt) {
      return copy_from(bmp, core::native_point(pt.os(context()), context()));
    }

#ifndef GUIPP_QT
    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) {
      if (bmp) {
        if (bmp.get_info().bits_per_pixel() == depth()) {
          return copy_from(static_cast<const os::drawable&>(bmp), src, pt);
        } else {
          switch (depth()) {
            case 24: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGB>())), src, pt);
              break;
            }
            case 32: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGBA>())), src, pt);
              break;
            }
          }
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) {
      if (bmp) {
        if (bmp.get_info().bits_per_pixel() == depth()) {
          return copy_from(static_cast<const os::drawable&>(bmp), src, pt);
        } else {
          switch (depth()) {
            case 24: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGB>())), src, pt);
              break;
            }
            case 32: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGBA>())), src, pt);
              break;
            }
          }
        }
      }
      return *this;
    }
#endif // GUIPP_QT

    // --------------------------------------------------------------------------
    paint::paint (const painter& f)
      : p(f)
    {}

    paint::paint (painter&& f)
      : p(std::move(f))
    {}

    void paint::operator() (core::context* s, core::native_rect* r) {
      if (p) {
        draw::graphics graph(s);
        p(graph);
      }
    }

    // --------------------------------------------------------------------------
    clip::clip (graphics& g, const core::rectangle& r)
      : core::clip(g.context(), core::native_rect(r.os(g.context()), g.context()))
    {}
  }

}
