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
#ifdef X11
# include <cmath>
# include <algorithm>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/logger.h>
#include <base/ostreamfmt.h>
#ifdef WIN32
# include <base/string_util.h>
#endif // WIN32

#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/drawers.h>
#include <gui/draw/use.h>


namespace gui {

  namespace draw {

#ifdef WIN32
    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {}

    graphics::graphics (draw::pixmap& target)
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

    const graphics& graphics::draw_pixel (const core::point& pt,
                                          os::color c) const {
      SetPixel(gc, pt.os_x(), pt.os_y(), c);
      return *this;
    }

    os::color graphics::get_pixel (const core::point& pt) const {
      return GetPixel(gc, pt.os_x(), pt.os_y());
    }

    const graphics& graphics::draw_lines (std::initializer_list<core::point> points,
                                          const pen& p) const {
      Use<pen> pn(gc, p);
      bool first = true;
      for (const core::point& pt : points) {
        if (first) {
          first = false;
          MoveToEx(gc, pt.os_x(), pt.os_y(), nullptr);
        } else {
          LineTo(gc, pt.os_x(), pt.os_y());
        }
      }
      return *this;
    }

    const graphics& graphics::copy_from (const graphics& src, const core::point& pt) const {
      return copy_from(src, src.area(), pt);
    }

    const graphics& graphics::copy_from (const graphics& src, const core::rectangle& r, const core::point& pt) const {
      if (!BitBlt(gc, pt.os_x(), pt.os_y(), r.os_width(), r.os_height(),
                  src, r.os_x(), r.os_y(), SRCCOPY)) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }

    const graphics& graphics::copy_from (os::drawable w,
                                         const core::rectangle& r,
                                         const core::point& pt) const {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc);
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!BitBlt(gc, pt.os_x(), pt.os_y(), r.os_width(), r.os_height(),
                    source_gc, r.os_x(), r.os_y(), SRCCOPY)) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!BitBlt(gc, pt.os_x(), pt.os_y(), r.os_width(), r.os_height(),
                    source_gc, r.os_x(), r.os_y(), SRCCOPY)) {
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

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) const {
      if (bmp.image) {
        core::size sz = bmp.image.size();
        if (bmp.mask) {
          HDC mask_dc = CreateCompatibleDC(gc);
          SelectObject(mask_dc, bmp.mask.get_id());
          HDC img_dc = CreateCompatibleDC(gc);
          SelectObject(img_dc, bmp.image.get_id());
          BitBlt(gc, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), img_dc, 0, 0, SRCINVERT);
          BitBlt(gc, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), mask_dc, 0, 0, SRCAND);
          BitBlt(gc, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), img_dc, 0, 0, SRCINVERT);
          DeleteDC(img_dc);
          DeleteDC(mask_dc);
        } else {
          copy_from(bmp.image, core::rectangle(sz), pt);
        }
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

    core::rectangle graphics::area () const {
      RECT r = {0, 0, 0, 0};
      os::window id = WindowFromDC(gc);
      if (id) {
        GetWindowRect(id, &r);
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
      return core::rectangle(r);
    }

    void graphics::push_clipping (const core::rectangle& r) const {
      HRGN hr = CreateRectRgn(r.os_x(), r.os_y(), r.os_x2(), r.os_y2());
      SelectClipRgn(gc, hr);
      clipping_stack.push_back(hr);
    }

    void graphics::pop_clipping () const {
      if (!clipping_stack.empty()) {
        HRGN hr = clipping_stack.back();
        DeleteObject(hr);
        clipping_stack.pop_back();
      }
      restore_clipping();
    }

    void graphics::restore_clipping () const {
      if (clipping_stack.size()) {
        SelectClipRgn(gc, clipping_stack.back());
      }
      SelectClipRgn(gc, NULL);
    }

#endif // WIN32

#ifdef X11
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

    core::rectangle get_drawable_area (os::drawable t) {
      Window root;
      int x, y;
      unsigned int w, h, b, d;
      XGetGeometry(get_instance(), t, &root, &x, &y, &w, &h, &b, &d);
      return {
          (core::point::type)x,
          (core::point::type)y,
          (core::size::type)w,
          (core::size::type)h
//        core::global::unscale((core::point::type)x),
//        core::global::unscale((core::point::type)y),
//        core::global::unscale((core::size::type)w),
//        core::global::unscale((core::size::type)h)
      };
    }


    XftDraw* get_xft_draw (os::drawable target) {
      static XftDraw* s_xft = nullptr;

      if (!s_xft) {
        auto display = get_instance();
        auto screen = get_screen();
        auto visual = get_visual();
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

    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
      get_xft();
    }

    graphics::graphics (draw::basic_map& target)
      : gc(0)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
      gc = XCreateGC(get_instance(), target, 0, 0);
      own_gc = true;
      get_xft();
    }

    graphics::graphics (const graphics& rhs)
      : gc(0)
      , target(0)
      , own_gc(false)
      , ref_gc(false)
    {
      get_xft();
      operator= (rhs);
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

    const graphics& graphics::draw_pixel (const core::uint32_point& pt,
                                          os::color c) const {
      Use<pen> pn(gc, pen(c));
      XDrawPoint(get_instance(), target, gc, pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::uint32_point& pt) const {
      XImage* im = XGetImage(get_instance(), target, pt.x(), pt.y(), 1, 1, AllPlanes, ZPixmap);
      os::color c = color::black;
      if (im) {
        c = XGetPixel(im, 0, 0);
        XDestroyImage(im);
      }
      return c;
    }

    const graphics& graphics::draw_lines (std::initializer_list<core::point> pts,
                                          const pen& p) const {

      Use<pen> pn(gc, p);
      std::vector<os::point> points;
      points.reserve(pts.size());
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
      XDrawLines(get_instance(), target, gc,
                 points.data(), (int)points.size(),
                 CoordModeOrigin);
      return *this;
    }

    const graphics& graphics::copy_from (const graphics& src, const core::point& pt) const {
      return copy_from(src, src.area(), pt);
    }

    const graphics& graphics::copy_from (const graphics& src, const core::rectangle& r, const core::point& pt) const {
      int res = XCopyArea(get_instance(), src, target, gc, r.x(), r.y(), r.width(), r.height(), pt.os_x(), pt.os_y());
      if (!res) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }

    const graphics& graphics::copy_from (os::drawable w,
                                         const core::rectangle& r,
                                         const core::point& pt) const {
      const int dd = get_drawable_depth(w);
      const int md = depth();
      int res = 0;
      if (dd == md) {
        res = XCopyArea(get_instance(), w, target, gc, r.x(), r.y(), r.width(), r.height(), pt.os_x(), pt.os_y());
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) const {
      auto display = core::global::get_instance();
      int res = 0;
      if (bmp.mask) {
        res = XSetClipMask(display, gc, bmp.mask.get_id());
        res = XSetClipOrigin(display, gc, pt.os_x(), pt.os_y());
      }
      if (bmp.image) {
        copy_from(bmp.image, core::rectangle(bmp.image.size()), pt);
      }
      res = XSetClipMask(display, gc, None);
      return *this;
    }

    void graphics::invert (const core::rectangle& r) const {
      pixmap img(r.size());
      graphics rhs(img);
      rhs.copy_from(*this, r);
      img.invert();
      copy_from(img, core::rectangle(r.size()), r.position());
    }

    void graphics::flush () const {
      XFlushGC(get_instance(), gc);
    }

    int graphics::depth () const {
      return get_drawable_depth(target);
    }

    core::rectangle graphics::area () const {
      return get_drawable_area(target);
    }

    void graphics::push_clipping (const core::rectangle& rect) const {
      os::rectangle r = rect.os();
      clipping_stack.push_back(r);
      XSetClipRectangles(get_instance(), gc, 0, 0, &r, 1, Unsorted);
      XftDrawSetClipRectangles(get_xft(), 0, 0, &r, 1);
    }

    void graphics::pop_clipping () const {
      if (!clipping_stack.empty()) {
        clipping_stack.pop_back();
      }
      restore_clipping();
    }

    void graphics::restore_clipping () const {
      if (clipping_stack.empty()) {
        XSetClipMask(get_instance(), gc, None);
        XftDrawSetClip(get_xft(), None);
      } else {
        os::rectangle& r = clipping_stack.back();
        XSetClipRectangles(get_instance(), gc, 0, 0, &r, 1, Unsorted);
        XftDrawSetClipRectangles(get_xft(), 0, 0, &r, 1);
      }
    }

    XftDraw* graphics::get_xft () const {
      return get_xft_draw(target);
    }

    graphics::operator XftDraw* () const {
      return get_xft();
    }

#endif // X11
    // --------------------------------------------------------------------------

    const graphics& graphics::clear (os::color color) const {
      return draw(rectangle(unscale(area())), color, color);
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) const {
      if (bmp) {
        if (bmp.depth() == depth()) {
          return copy_from(bmp.get_id(), src, pt);
        } else {
          switch (depth()) {
            case 24: {
              return copy_from(pixmap(bmp.get<PixelFormat::RGB>()).get_id(), src, pt);
              break;
            }
            case 32: {
              return copy_from(pixmap(bmp.get<PixelFormat::RGBA>()).get_id(), src, pt);
              break;
            }
          }
        }
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::point& pt) const {
      return copy_from(bmp, core::rectangle(bmp.size()), pt);
    }

    const graphics& graphics::frame (const std::function<frameable>& drawer,
                                     const pen& p) const {
      drawer(*this, p);
      return *this;
    }

    const graphics& graphics::fill (const std::function<fillable>& drawer,
                                    const brush& b) const {
      drawer(*this, b);
      return *this;
    }

    const graphics& graphics::draw (const std::function<drawable>& drawer,
                                    const brush& b,
                                    const pen& p) const {
      drawer(*this, b, p);
      return *this;
    }

    const graphics& graphics::text (const std::function<textable>& drawer,
                                    const font& f,
                                    os::color c) const {
      drawer(*this, f, c);
      return *this;
    }

    const graphics& graphics::copy (const std::function<copyable>& drawer,
                                    const core::point& pt) const {
      drawer(*this, pt);
      return *this;
    }

    // --------------------------------------------------------------------------
    paint::paint (const painter& f)
      : p(f)
    {}

    paint::paint (painter&& f)
      : p(std::move(f))
    {}

    void paint::operator() (os::window id, const os::graphics& g) {
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

    void buffered_paint::operator() (os::window id, const os::graphics& g) {
      if (p) {
        draw::graphics graph(id, g);
#ifndef BUILD_FOR_ARM
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
