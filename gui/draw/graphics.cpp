/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <memory.h>
#include <array>
#ifdef GUIPP_X11
# include <cmath>
# include <algorithm>
#endif // GUIPP_X11
#ifdef GUIPP_QT
# include <QtGui/QBitmap>
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <util/ostreamfmt.h>
#ifdef GUIPP_WIN
# include <util/string_util.h>
#endif // GUIPP_WIN

#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/drawers.h>
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
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {}

    graphics::graphics (draw::basic_map& target)
      : gc(0)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
      HDC gdc = GetDC(NULL);
      gc = CreateCompatibleDC(gdc);
      own_gc = true;
      ReleaseDC(NULL, gdc);

      SelectObject(gc, target.get_id());
    }

    graphics::graphics (const graphics& rhs)
      : gc(0)
      , target(0)
      , own_gc(false)
      , ref_gc(false)
    {
      operator= (rhs);
    }

    graphics::~graphics () {
      destroy();
    }

    void graphics::destroy () {
      if (gc) {
        if (own_gc) {
          DeleteDC(gc);
        } else if (ref_gc) {
          ReleaseDC((HWND)target, gc);
        }
      }
      gc = 0;
      own_gc = false;
      ref_gc = false;
    }

    void graphics::operator= (const graphics& rhs) {
      if (&rhs != this) {
        destroy();
        target = rhs.target;
        own_gc = rhs.own_gc;
        ref_gc = rhs.ref_gc;
        if (own_gc) {
          gc = CreateCompatibleDC(rhs.gc);
        } else if (ref_gc) {
          gc = GetDC((HWND)target);
        } else {
          gc = rhs.gc;
        }
      }
    }

    const graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) const {
      SetPixel(gc, pt.x(), pt.y(), c);
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      return GetPixel(gc, pt.x(), pt.y());
    }

    const graphics& graphics::draw_lines (std::vector<core::point> points,
                                          const pen& p) const {
      Use<pen> pn(gc, p);
      const auto off = p.os_size() / 2;
      bool first = true;
      for (const core::point& pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc, pt.os_x() + off, pt.os_y() + off, nullptr);
        } else {
          LineTo(gc, pt.os_x() + off, pt.os_y() + off);
        }
      }
      return *this;
    }

    const graphics& graphics::copy_from (const graphics& src, const core::native_rect& r, const core::native_point& pt) const {
      if (!BitBlt(gc, pt.x(), pt.y(), r.width(), r.height(),
                  src, r.x(), r.y(), SRCCOPY)) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }

    const graphics& graphics::copy_from (os::drawable w,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) const {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc);
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!BitBlt(gc, pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!BitBlt(gc, pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }

    /*
    const graphics& graphics::stretch_from (os::drawable w,
                                            const core::rectangle& src,
                                            const core::rectangle& dst) const {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc);
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!StretchBlt(gc, dst.os_x(), dst.os_y(), dst.os_width(), dst.os_height(),
                        source_gc, src.os_x(), src.os_y(), src.os_width(), src.os_height(), SRCCOPY)) {
          throw std::runtime_error("graphics::stretch_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!StretchBlt(gc, dst.os_x(), dst.os_y(), dst.os_width(), dst.os_height(),
                        source_gc, src.os_x(), src.os_y(), src.os_width(), src.os_height(), SRCCOPY)) {
          throw std::runtime_error("graphics::stretch_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }
    */

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) const {
      if (bmp.mask) {
        core::native_size sz = bmp.mask.native_size();
        HDC mask_dc = CreateCompatibleDC(gc);
        SelectObject(mask_dc, bmp.mask.get_id());
        BitBlt(gc, pt.x(), pt.y(), sz.width(), sz.height(), mask_dc, 0, 0, bmp.image ? (DWORD)(0x00220326) : SRCAND); //DSna, https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations
        DeleteDC(mask_dc);
        if (bmp.image) {
          core::native_size sz = bmp.image.native_size();
          HDC img_dc = CreateCompatibleDC(gc);
          SelectObject(img_dc, bmp.image.get_id());
          BitBlt(gc, pt.x(), pt.y(), sz.width(), sz.height(), img_dc, 0, 0, SRCPAINT);  // DSo
          DeleteDC(img_dc);
        }

      } else {
        core::native_size sz = bmp.image.native_size();
        copy_from(bmp.image, core::native_rect(sz), pt);
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) const {
      RECT rect = r;
      InvertRect(gc, &rect);
    }

    void graphics::flush () const {
      GdiFlush();
    }

    int graphics::depth () const {
      return GetDeviceCaps(gc, BITSPIXEL);
    }

    core::native_rect graphics::native_area () const {
      RECT r = {0, 0, 0, 0};
      os::window id = WindowFromDC(gc);
      if (id) {
        GetWindowRect(id, &r);
        r.left = 0;
        r.top = 0;
      } else {
        HGDIOBJ hBmp = GetCurrentObject(gc, OBJ_BITMAP);
        if (hBmp) {
          BITMAP bmp;
          memset(&bmp, 0, sizeof (BITMAP));
          GetObject(hBmp, sizeof (BITMAP), &bmp);
          r.right = bmp.bmWidth;
          r.bottom = bmp.bmHeight;
        } else {
          r.right = GetDeviceCaps(gc, HORZRES);
          r.bottom = GetDeviceCaps(gc, VERTRES);
        }
      }
      return core::native_rect(r);
    }

    void graphics::set_clip_rect (const core::rectangle& rect) const {
      SelectClipRgn(gc, NULL);
      os::rectangle r = rect.os();
      IntersectClipRect(gc, r.left, r.top, r.right, r.bottom);
    }

    void graphics::clear_clip_rect () const {
      SelectClipRgn(gc, NULL);
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

#ifdef GUIPP_USE_XFT
    XftDraw* get_xft_draw (os::drawable target) {
      static XftDraw* s_xft = nullptr;

      if (!s_xft) {
        auto display = get_instance();
        auto screen = x11::get_screen();
        auto visual = x11::get_visual();
        auto colormap = DefaultColormap(display, screen);

//        XVisualInfo vinfo;
//        XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo);

//        auto visual = vinfo.visual;
//        auto colormap = XCreateColormap(display, target, visual, AllocNone);

        s_xft = XftDrawCreate(display, target, visual, colormap);
      } else {
        XftDrawChange(s_xft, target);
      }
      return s_xft;
    }
#endif // GUIPP_USE_XFT

    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
#ifdef GUIPP_USE_XFT
      get_xft();
#endif // GUIPP_USE_XFT
      clear_clip_rect();
    }

    graphics::graphics (draw::basic_map& target)
      : gc(0)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
      gc = XCreateGC(get_instance(), target, 0, 0);
      own_gc = true;
#ifdef GUIPP_USE_XFT
      get_xft();
#endif // GUIPP_USE_XFT
      clear_clip_rect();
    }

    graphics::graphics (const graphics& rhs)
      : gc(0)
      , target(0)
      , own_gc(false)
      , ref_gc(false)
    {
#ifdef GUIPP_USE_XFT
      get_xft();
#endif // GUIPP_USE_XFT
      operator= (rhs);
      clear_clip_rect();
    }

    graphics::~graphics () {
      flush();
      destroy();
    }

    void graphics::destroy () {
      if (gc) {
        if (own_gc) {
          XFreeGC(get_instance(), gc);
        }
      }
      gc = 0;
      own_gc = false;
      ref_gc = false;
    }

    void graphics::operator= (const graphics& rhs) {
      if (&rhs != this) {
        destroy();
        target = rhs.target;
        own_gc = rhs.own_gc;
        ref_gc = rhs.ref_gc;
        if (own_gc) {
          gc = XCreateGC(get_instance(), target, 0, 0);
        } else {
          gc = rhs.gc;
        }
      }
    }

    const graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) const {
      Use<pen> pn(gc, pen(c));
      XDrawPoint(get_instance(), target, gc, pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      XImage* im = XGetImage(get_instance(), target, pt.x(), pt.y(), 1, 1, AllPlanes, ZPixmap);
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

    const graphics& graphics::draw_lines (std::vector<core::point> pts,
                                          const pen& p) const {

      Use<pen> pn(gc, p);
      const short off = p.os_size() / 2;

      std::vector<os::point> points;
      points.reserve(pts.size());
      for (const core::point& pt : pts) {
        points.push_back(pt.os() + off);
      }
      XDrawLines(get_instance(), target, gc,
                 points.data(), (int)points.size(),
                 CoordModeOrigin);
      return *this;
    }

    const graphics& graphics::copy_from (const graphics& src, const core::native_rect& r, const core::native_point& pt) const {
      int res = XCopyArea(get_instance(), src, target, gc, r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
      if (!res) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }


    const graphics& graphics::copy_from (os::drawable w,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) const {
      const int dd = get_drawable_depth(w);
      const int md = depth();
      if (dd == md) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        values.graphics_exposures = False;
        XChangeGC(display, gc, GCFunction|GCGraphicsExposures, &values);
        /*int res =*/ XCopyArea(get_instance(), w, target, gc, r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
        values = { GXcopy }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
      } else if (1 == dd) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        values.graphics_exposures = False;
        XChangeGC(display, gc, GCFunction|GCGraphicsExposures, &values);
        /*int res =*/ XCopyPlane(get_instance(), w, target, gc, r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y(), 1);
        values = { GXcopy }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) const {
      auto display = core::global::get_instance();
      int res = 0;
      if (bmp.mask) {
        XSetClipMask(display, gc, bmp.mask.get_id());
        XSetClipOrigin(display, gc, pt.x(), pt.y());
      }
      if (bmp.image) {
        auto sz = bmp.image.native_size();
        res = XCopyArea(get_instance(), bmp.image, target, gc, 0, 0, sz.width(), sz.height(), pt.x(), pt.y());
      } else {
        auto sz = bmp.mask.native_size();
        Use<brush> br(gc, color::black);
        XFillRectangle(display, target, gc, pt.x(), pt.y(), sz.width(), sz.height());
      }

      if (bmp.mask) {
        restore_clipping();
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) const {
      auto display = core::global::get_instance();

      XGCValues values = { GXinvert, 0 };  // .function =
      values.graphics_exposures = False;
      XChangeGC(display, gc, GCFunction, &values);
      XFillRectangle(display, target, gc, r.os_x(), r.os_y(), r.os_width(), r.os_height());
      values = { GXcopy, 0 }; // .function =
      XChangeGC(display, gc, GCFunction, &values);
    }

    void graphics::flush () const {
      XFlushGC(get_instance(), gc);
    }

    int graphics::depth () const {
      return get_drawable_depth(target);
    }

    core::native_rect graphics::native_area () const {
      return get_drawable_area(target);
    }

    void graphics::set_clip_rect (const core::rectangle& rect) const {
      os::rectangle r = rect.os();
      XSetClipRectangles(get_instance(), gc, 0, 0, &r, 1, Unsorted);
#ifdef GUIPP_USE_XFT
      XftDrawSetClipRectangles(get_xft(), 0, 0, &r, 1);
#endif // GUIPP_USE_XFT
    }

    void graphics::clear_clip_rect () const {
      XSetClipMask(get_instance(), gc, None);
#ifdef GUIPP_USE_XFT
      XftDrawSetClip(get_xft(), None);
#endif // GUIPP_USE_XFT
    }

#ifdef GUIPP_USE_XFT
    XftDraw* graphics::get_xft () const {
      return get_xft_draw(target);
    }

    graphics::operator XftDraw* () const {
      return get_xft();
    }
#endif // GUIPP_USE_XFT

#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable t, os::graphics g)
      : target(t)
      , gc(g)
      , own_gc(false)
      , ref_gc(false)
    {
      if (!gc) {
        gc = new QPainter(target);
        own_gc = true;
      }
      clear_clip_rect();
    }

    graphics::graphics (draw::basic_map& t)
      : target(t)
      , gc(nullptr)
      , own_gc(false)
      , ref_gc(false)
    {
      gc = new QPainter(target);
      own_gc = true;
      clear_clip_rect();
    }

    graphics::graphics (const graphics& rhs)
      : gc(rhs.gc)
      , target(rhs.target)
      , own_gc(false)
      , ref_gc(false)
    {
      clear_clip_rect();
    }

    graphics::~graphics () {
      if (gc) {
        gc->end();
      }
      destroy();
    }

    void graphics::destroy () {
      if (own_gc) {
        delete gc;
        gc = nullptr;
      }
      own_gc = false;
      ref_gc = false;
    }

    void graphics::operator= (const graphics& rhs) {
      if (&rhs != this) {
        destroy();
        target = rhs.target;
        own_gc = rhs.own_gc;
        ref_gc = rhs.ref_gc;
        if (own_gc) {
          gc = new QPainter(target);
        } else {
          gc = rhs.gc;
        }
      }
    }

    const graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) const {
      gc->setPen(c);
      gc->drawPoint(pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      return color::black;
    }

    const graphics& graphics::draw_lines (std::vector<core::point> points,
                                          const pen& p) const {
      Use<pen> pn(gc, p);
      QVector<QPoint> pointPairs;
      const auto off = p.os_size() / 2;
      bool first = true;
      QPoint last;
      for (const core::point& pt : points) {
        if (first) {
          first = false;
          last = pt.os();
        } else {
          pointPairs.push_back(last);
          last = pt.os();
          pointPairs.push_back(last);
        }
      }
      gc->drawLines(pointPairs);
      return *this;
    }

    const graphics& graphics::copy_from (const graphics& src, const core::native_rect& r, const core::native_point& pt) const {
      return copy_from(src.target, r, pt);
    }

    const graphics& graphics::copy_from (os::drawable d,
                                         const core::native_rect& r,
                                         const core::native_point& pt,
                                         const copy_mode mode) const {
      QWidget* w = dynamic_cast<QWidget*>(d);
      if (w) {
        QPixmap pix = w->grab(r.os());
        const QPainter::CompositionMode oldMode = gc->compositionMode();
        gc->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
        gc->drawPixmap(pt.x(), pt.y(), pix);
        gc->setCompositionMode(oldMode);
      } else {
        QPixmap* p = dynamic_cast<QPixmap*>(d);
        if (p) {
          const QPainter::CompositionMode oldMode = gc->compositionMode();
          gc->setCompositionMode(static_cast<QPainter::CompositionMode>(mode));
          gc->drawPixmap(pt.x(), pt.y(), *p);
          gc->setCompositionMode(oldMode);
        }
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) const {
      if (bmp.mask) {
        QPainter::CompositionMode old_mode = gc->compositionMode();
        gc->setCompositionMode(QPainter::RasterOp_NotSourceAndDestination);
        gc->drawPixmap(pt.x(), pt.y(), bmp.mask.get_id());
        gc->setCompositionMode(QPainter::RasterOp_SourceOrDestination);
        gc->drawPixmap(pt.x(), pt.y(), bmp.image.get_id());
        gc->setCompositionMode(old_mode);
      } else {
        gc->drawPixmap(pt.x(), pt.y(), bmp.image.get_id());
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) const {
      if (bmp) {
        gc->drawPixmap(pt.os(), bmp.get_id(), src.os());
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) const {
      if (bmp) {
        gc->drawPixmap(pt.x(), pt.y(), bmp.get_id(), src.x(), src.y(), src.width(), src.height());
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) const {
      const QPainter::CompositionMode oldMode = gc->compositionMode();
      gc->setCompositionMode(QPainter::RasterOp_NotDestination);
      gc->fillRect(r.os(), Qt::white);
      gc->setCompositionMode(oldMode);
    }

    void graphics::flush () const {
    }

    int graphics::depth () const {
      return gc->device()->depth();
    }

    core::native_rect graphics::native_area () const {
      auto vp = gc->viewport();
      return core::native_rect(vp.x(),
                               vp.y(),
                               static_cast<core::native_rect::size_type>(vp.width()),
                               static_cast<core::native_rect::size_type>(vp.height()));
    }

    void graphics::set_clip_rect (const core::rectangle& rect) const {
      gc->setClipRect(rect.os());
    }

    void graphics::clear_clip_rect () const {
      gc->setClipping(false);
    }
#endif // GUIPP_QT
    // --------------------------------------------------------------------------

    void graphics::push_clipping (const core::rectangle& rect) const {
      if (clipping_stack.empty()) {
        clipping_stack.push_back(rect);
      } else {
        clipping_stack.push_back(clipping_stack.back() & rect);
      }
      set_clip_rect(clipping_stack.back());
    }

    void graphics::pop_clipping () const {
      if (!clipping_stack.empty()) {
        clipping_stack.pop_back();
      }
      restore_clipping();
    }

    void graphics::restore_clipping () const {
      if (clipping_stack.empty()) {
        clear_clip_rect();
      } else {
        set_clip_rect(clipping_stack.back());
      }
    }

    const graphics& graphics::clear (os::color color) const {
#ifdef GUIPP_QT
      gc->fillRect(native_area(), QBrush(color));
      return *this;
#else
      return draw(rectangle(area()), color, color);
#endif // GUIPP_QT
    }

    core::rectangle graphics::area () const {
      return core::global::scale_from_native(native_area());
    }

    const graphics& graphics::copy_from (const graphics& src, const core::point& pt) const {
      return copy_from(src, src.native_area(), core::global::scale_to_native(pt));
    }

    const graphics& graphics::copy_from (const graphics& src, const core::native_point& pt) const {
      return copy_from(src, src.native_area(), pt);
    }

    const graphics& graphics::copy_from (os::drawable w,
                                         const core::rectangle& r,
                                         const core::point& pt,
                                         const copy_mode mode) const {
      return copy_from(w, core::global::scale_to_native(r), core::global::scale_to_native(pt), mode);
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) const {
      return copy_from(bmp, core::global::scale_to_native(pt));
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_point& pt) const {
      return copy_from(bmp, core::native_rect(bmp.native_size()), pt);
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::point& pt) const {
      return copy_from(bmp, core::global::scale_to_native(pt));
    }

#ifndef GUIPP_QT
    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) const {
      if (bmp) {
        if (bmp.depth() == depth()) {
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

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) const {
      if (bmp) {
        if (bmp.depth() == depth()) {
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

    void paint::operator() (os::window id, os::graphics g) {
      if (p) {
        draw::graphics graph(id, g);
        p(graph);
      }
    }

    // --------------------------------------------------------------------------
    buffered_paint::buffered_paint (const painter& f)
      : p(f)
    {}

    buffered_paint::buffered_paint (painter&& f)
      : p(std::move(f))
    {}

//#define NOT_IMAGE_CACHE

    void buffered_paint::operator() (os::window id, os::graphics g) {
      if (p) {
        draw::graphics graph(id, g);
#if !defined(BUILD_FOR_ARM) && !defined(NOT_IMAGE_CACHE) && !defined(GUIPP_QT)
        const auto area = graph.area();
        draw::pixmap buffer(area.size());
        draw::graphics buffer_graph(buffer);
        p(buffer_graph);
        graph.copy_from(buffer_graph);
#else
        p(graph);
#endif
      }
    }

  }

}
