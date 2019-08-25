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

#ifdef X11
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
#endif // X11


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

    const graphics& graphics::draw_pixel (const core::uint32_point& pt,
                                          os::color c) const {
      SetPixel(gc, pt.x(), pt.y(), c);
      return *this;
    }

    os::color graphics::get_pixel (const core::uint32_point& pt) const {
      return GetPixel(gc, pt.x(), pt.y());
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
                                         const core::point& pt,
                                         const copy_mode mode) const {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc);
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!BitBlt(gc, pt.os_x(), pt.os_y(), r.os_width(), r.os_height(),
                    source_gc, r.os_x(), r.os_y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!BitBlt(gc, pt.os_x(), pt.os_y(), r.os_width(), r.os_height(),
                    source_gc, r.os_x(), r.os_y(), static_cast<uint32_t>(mode))) {
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
      if (bmp.mask) {
        core::size sz = bmp.mask.size();
        HDC mask_dc = CreateCompatibleDC(gc);
        SelectObject(mask_dc, bmp.mask.get_id());
        BitBlt(gc, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), mask_dc, 0, 0, bmp.image ? (DWORD)(0x00220326) : SRCAND); //DSna, https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations
        DeleteDC(mask_dc);
        if (bmp.image) {
          core::size sz = bmp.image.size();
          HDC img_dc = CreateCompatibleDC(gc);
          SelectObject(img_dc, bmp.image.get_id());
          BitBlt(gc, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), img_dc, 0, 0, SRCPAINT);  // DSo
          DeleteDC(img_dc);
        }

      } else {
        core::size sz = bmp.image.size();
        copy_from(bmp.image, core::rectangle(sz), pt);
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
      };
    }


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
        if (im->bits_per_pixel == 1) {
          c = XGetPixel(im, 0, 0) ? color::white : color::black;
        } else {
          c = XGetPixel(im, 0, 0);
        }
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
                                         const core::point& pt,
                                         const copy_mode mode) const {
      return copy_from(w,
                       core::uint32_rect(r.os_x(), r.os_y(), r.os_width(), r.os_height()),
                       core::uint32_point(pt.os_x(), pt.os_y()),
                       mode);
    }

    const graphics& graphics::copy_from (os::drawable w,
                                         const core::uint32_rect& r,
                                         const core::uint32_point& pt,
                                         const copy_mode mode) const {
      const int dd = get_drawable_depth(w);
      const int md = depth();
      if (dd == md) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
        int res = XCopyArea(get_instance(), w, target, gc, r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
        values = { GXcopy }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
      } else if (1 == dd) {
        auto display = core::global::get_instance();
        XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
        int res = XCopyPlane(get_instance(), w, target, gc, r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y(), 1);
        values = { GXcopy }; // .function =
        XChangeGC(display, gc, GCFunction, &values);
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      return *this;
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) const {
      return copy_from(bmp, core::uint32_point(pt.os_x(), pt.os_y()));
    }

    const graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::uint32_point& pt) const {
      auto display = core::global::get_instance();
      int res = 0;
      XGCValues org_values = {0};
      res = XGetGCValues(display, gc, GCFunction|GCForeground|GCBackground, &org_values);
//      LogDebug << "XGCValues:" << org_values;
      if (bmp.mask) {
        auto screen = core::global::x11::get_screen();
        XGCValues values = {
          //function
            static_cast<int>(copy_mode::bit_and)
          //plane_mask
          , 0
          //foreground
          , bmp.image ? XBlackPixel(display, screen)
                      : XWhitePixel(display, screen)
          //background
          , bmp.image ? XWhitePixel(display, screen)
                      : XBlackPixel(display, screen)
        };
        res = XChangeGC(display, gc, GCFunction|GCForeground|GCBackground, &values);
        auto sz = bmp.mask.size();
        res = XCopyPlane(get_instance(), bmp.mask, target, gc, 0, 0, sz.width(), sz.height(), pt.x(), pt.y(), 1);
      }
      if (bmp.image) {
        XGCValues values = {
          static_cast<int>(copy_mode::bit_or) //function
        };
        res = XChangeGC(display, gc, GCFunction, &values);
        auto sz = bmp.image.size();
        res = XCopyArea(get_instance(), bmp.image, target, gc, 0, 0, sz.width(), sz.height(), pt.x(), pt.y());
      }
      res = XChangeGC(display, gc, GCFunction|GCForeground|GCBackground, &org_values);
      return *this;
    }

    void graphics::invert (const core::rectangle& r) const {
      auto display = core::global::get_instance();
      XGCValues values = { GXinvert };  // .function =
      XChangeGC(display, gc, GCFunction, &values);
      int res = XCopyArea(get_instance(), 0, target, gc, r.os_x(), r.os_y(), r.os_width(), r.os_height(), r.os_x(), r.os_y());
      values = { GXcopy }; // .function =
      XChangeGC(display, gc, GCFunction, &values);

//      pixmap img(r.size());
//      graphics rhs(img);
//      rhs.copy_from(*this, r);
//      img.invert();
//      copy_from(img, core::rectangle(r.size()), r.position());
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
      return draw(rectangle(core::global::unscale(area())), color, color);
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

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::uint32_rect& src, const core::uint32_point& pt) const {
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

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::uint32_point& pt) const {
      return copy_from(bmp, core::uint32_rect(bmp.size()), pt);
    }

    const graphics& graphics::copy_from (const draw::pixmap& bmp, const core::point& pt) const {
      return copy_from(bmp, core::rectangle(bmp.size<float>()), pt);
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
