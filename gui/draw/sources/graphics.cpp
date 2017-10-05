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
#include <logger.h>
#include <ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "graphics.h"
#include "bitmap.h"
#ifdef WIN32
#include <string_util.h>
#endif // WIN32

#ifdef X11
#include <cmath>
#include <algorithm>
#endif // X11

namespace gui {

  namespace draw {

#ifdef WIN32
    brush null_brush((os::brush)GetStockObject(NULL_BRUSH));
    pen null_pen((os::pen)GetStockObject(NULL_PEN));

    template<typename T>
    struct Use {
      Use(os::graphics g, const T& t)
        : g(g)
        , obj(t)
        , old(set(t)) 
      {}

      HGDIOBJ set(HGDIOBJ t) {
        return SelectObject(g, t);
      }

      void unset(HGDIOBJ t) {
        SelectObject(g, t);
      }

      inline ~Use() {
        unset(old);
      }

      os::graphics g;
      HGDIOBJ obj;
      HGDIOBJ old;
    };

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      Use<pen> pn(g, p);
      MoveToEx(g, from.os_x(), from.os_y(), nullptr);
      LineTo(g, to.os_x(), to.os_y());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Rectangle(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Ellipse(g, rect.os_x(), rect.os_y(), rect.os_x2(), rect.os_y2());
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (const graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(),
                rect.os_y(),
                rect.os_x2(),
                rect.os_y2(),
                (size.os_width() * 2),
                (size.os_height() * 2));
    }

    // --------------------------------------------------------------------------
    arc::arc (const core::point& pos,
              unsigned int radius,
              float start_radius,
              float end_radius)
      : pos(pos)
      , radius(radius)
      , start_radius(start_radius)
      , end_radius(end_radius) {
    }

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      BeginPath(g);
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      BeginPath(g);
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      MoveToEx(g, pos.os_x(), pos.os_y(), nullptr);
      AngleArc(g, pos.os_x(), pos.os_y(), radius, start_radius, end_radius);
      LineTo(g, pos.os_x(), pos.os_y());
      EndPath(g);
      StrokeAndFillPath(g);
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      for(const core::point& pt : pts){
        points.push_back(pt.os());
      }
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      for(const core::point& pt : pts){
        points.push_back(pt.os());
      }
    }

    void polygon::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    void polygon::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    void polygon::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Polygon(g, (const POINT*)points.data(), (int)points.size());
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (const graphics& g,
                               const font& f,
                               os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, clear_background ? OPAQUE : TRANSPARENT);
      RECT Rect = rect;

      std::wstring wstr = ibr::string::utf8_to_utf16(str);

      unsigned int o = static_cast<unsigned int>(origin);
      if (bit_mask<unsigned int, DT_WORDBREAK | DT_VCENTER>::is_set(o)) {
        int h = DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o | DT_CALCRECT);
        Rect.left = rect.os_x();
        Rect.right = rect.os_x2();
        Rect.top = ((rect.size().os_height() - h) / 2);
        Rect.bottom = Rect.top + h;
      }
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   os::color c) const {
      Use<font> fn(g, f);
      RECT r = rect;
      std::wstring wstr = ibr::string::utf8_to_utf16(str);
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &r, static_cast<unsigned int>(origin) | DT_CALCRECT);
      rect = core::rectangle(r);
      //auto sz = f.get_text_size(str);
      //if (rect.size() != sz) {
      //  LogDebug << "Text size differs: DT_CALCRECT:" << rect.size() << ", GetTextExtentPoint32:" << sz;
      //}
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, clear_background ? OPAQUE : TRANSPARENT);
      int px = pos.os_x();
      int py = pos.os_y();
      unsigned int old_align = static_cast<unsigned int>(text_origin::top_left);
      std::wstring wstr = ibr::string::utf8_to_utf16(str);

      switch (origin) {
        case text_origin::top_left:
          old_align = SetTextAlign(g, TA_LEFT | TA_TOP | TA_NOUPDATECP);
          break;
        case text_origin::top_right:
          old_align = SetTextAlign(g, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
          break;
        case text_origin::top_hcenter:
          old_align = SetTextAlign(g, TA_CENTER | TA_TOP | TA_NOUPDATECP);
          break;
        case text_origin::bottom_left:
          old_align = SetTextAlign(g, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case text_origin::bottom_right:
          old_align = SetTextAlign(g, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case text_origin::bottom_hcenter:
          old_align = SetTextAlign(g, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP);
          break;
        case text_origin::vcenter_right: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_RIGHT | TA_NOUPDATECP);
          break;
        }
        case text_origin::vcenter_left: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_LEFT | TA_NOUPDATECP);
          break;
        }
        case text_origin::center: {
          SIZE sz;
          GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
          py -= sz.cy / 2;
          old_align = SetTextAlign(g, TA_CENTER | TA_NOUPDATECP);
          break;
        }
        case text_origin::undefined:
          break;
      }
      TextOutW(g, px, py, wstr.c_str(), (int)wstr.size());
      SetTextAlign(g, old_align);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {}

    graphics::graphics (draw::memmap& target)
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
      operator=(rhs);
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
      }
      else {
        if (!StretchBlt(gc, dst.os_x(), dst.os_y(), dst.os_width(), dst.os_height(),
                        source_gc, src.os_x(), src.os_y(), src.os_width(), src.os_height(), SRCCOPY)) {
          throw std::runtime_error("graphics::stretch_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }

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
      RECT r = {0};
      os::window id = WindowFromDC(gc);
      if (id) {
        GetWindowRect(id, &r);
      } else {
        HGDIOBJ hBmp = GetCurrentObject(gc, OBJ_BITMAP);
        if (hBmp) {
          BITMAP bmp;
          memset(&bmp, 0, sizeof(BITMAP));
          GetObject(hBmp, sizeof(BITMAP), &bmp);
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

    // --------------------------------------------------------------------------

#endif // WIN32

#ifdef X11

    using namespace core::global;

    // --------------------------------------------------------------------------
    template<typename T>
    struct Use {
      Use (os::graphics g,
           const T& t)
        : g(g) {
        set(t);
      }

      void set (const T& t);

      os::graphics g;
    };

    template<>
    void Use<pen>::set (const pen& p) {
      os::instance display = get_instance();
      XSetForeground(display, g, p.color());
      XSetLineAttributes(display, g, p.size(), static_cast<int>(p.style()) & 0x0F, CapButt, JoinMiter);
      if (static_cast<int>(p.style()) & 0x0F0) {
        switch (p.style()) {
          case pen::Style::dot:
            static const char dots[] = {1, 1};
            XSetDashes(display, g, 0, dots, 2);
            break;
          case pen::Style::dashDot:
            static const char dash_dots[] = {4, 4, 1, 4};
            XSetDashes(display, g, 0, dash_dots, 4);
            break;
          case pen::Style::dashDotDot:
            static const char dash_dot_dots[] = {4, 4, 1, 2, 1, 4};
            XSetDashes(display, g, 0, dash_dot_dots, 6);
            break;
        }
      }
    }

    template<>
    void Use<brush>::set (const brush& b) {
      os::instance display = get_instance();
      XSetForeground(display, g, b.color());
      XSetFillStyle(display, g, b.style());
    }

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
      return {(core::point_type)x, (core::point_type)y, (core::size_type)w, (core::size_type)h};
    }
    // --------------------------------------------------------------------------

    XftDraw* get_xft_draw (os::drawable target) {
      static XftDraw* s_xft = nullptr;

      if (!s_xft) {
        auto display = get_instance();
        auto screen = get_screen();
        auto visual = get_visual();
        auto colormap = DefaultColormap(display, screen);
        s_xft = XftDrawCreate(display, target, visual, colormap);
      } else {
        XftDrawChange(s_xft, target);
      }
      return s_xft;
    }

    XftDraw* graphics::get_xft () const {
      return get_xft_draw(target);
    }

    graphics::operator XftDraw* () const {
      return get_xft();
    }

    // --------------------------------------------------------------------------
    struct render_color : XRenderColor {
      render_color (os::color c)
        : XRenderColor({
          (unsigned short)(color::extract<color::part::red>(c) << 8),
          (unsigned short)(color::extract<color::part::green>(c) << 8),
          (unsigned short)(color::extract<color::part::blue>(c) << 8),
          (unsigned short)((color::extract<color::part::alpha>(c) << 8) ^ 0xffff)
        })
      {}
    };

    // --------------------------------------------------------------------------
    struct xft_color : public XftColor {
      xft_color (os::color c, XftDraw* xft)
        :xft(xft) {
        render_color xrcolor(c);
        XftColorAllocValue(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), &xrcolor, this);
      }

      ~xft_color () {
        XftColorFree(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), this);
      }

      XftDraw* xft;
    };

    // --------------------------------------------------------------------------
    graphics::graphics (os::drawable target, os::graphics gc)
      : gc(gc)
      , target(target)
      , own_gc(false)
      , ref_gc(false)
    {
      get_xft();
    }

    graphics::graphics (draw::memmap& target)
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
      operator=(rhs);
    }

    graphics::~graphics () {
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

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLine(get_instance(), g, g, from.os_x(), from.os_y(), to.os_x(), to.os_y());
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      os::instance display = get_instance();

      Use<brush> br(g, b);
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
      XFillRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
      Use<pen> pn(g, p);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);
      XFillRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());

      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawRectangle(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height());
    }

    // --------------------------------------------------------------------------
    static const int degree_0 = 0;
    static const int degree_90 = 90 * 64;
    static const int degree_180 = 180 * 64;
    static const int degree_270 = 270 * 64;
    static const int degree_360 = 360 * 64;

    // --------------------------------------------------------------------------
    void ellipse::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      const core::point& pt = rect.top_left();
      core::size sz(rect.size() - core::size::one);

      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);

      pen p(b.color());
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArc(display, g, g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), 0, degree_360);
    }

    // --------------------------------------------------------------------------
    void calc_arcs (const core::rectangle& rect,
                    const core::size& size,
                    std::array<XArc, 4>* arcs,
                    std::array<XSegment, 4>* segments,
                    std::array<XRectangle, 3>* rects) {

      using namespace os;
      const size_type w = size.os_width();
      const size_type h = size.os_height();

      const point_type x0 = rect.top_left().os_x();
      const point_type x1 = x0 + w;
      const point_type x3 = rect.bottom_right().os_x();
      const point_type x2 = x3 - w;

      const point_type y0 = rect.top_left().os_y();
      const point_type y1 = y0 + h;
      const point_type y3 = rect.bottom_right().os_y();
      const point_type y2 = y3 - h;

      const size_type w2 = w * size_type(2);
      const size_type h2 = h * size_type(2);

      if (arcs) {
        (*arcs)[0] = {x0, y0, w2, h2, degree_90, degree_90};
        (*arcs)[1] = {point_type(x3 - w2), y0, w2, h2, degree_0, degree_90};
        (*arcs)[2] = {x0, point_type(y3 - h2), w2, h2, degree_180, degree_90};
        (*arcs)[3] = {point_type(x3 - w2), point_type(y3 - h2), w2, h2, degree_270, degree_90};
      }

      if (segments) {
        (*segments)[0] = {x1, y0, x2, y0};
        (*segments)[1] = {x3, y1, x3, y2};
        (*segments)[2] = {x1, y3, x2, y3};
        (*segments)[3] = {x0, y1, x0, y2};
      }

      if (rects) {
        (*rects)[0] = {x0, y1, w, size_type(y2 - y1)};
        (*rects)[1] = {x2, y1, size_type(w + 1), size_type(y2 - y1)};
        (*rects)[2] = {x1, y0, size_type(x2 - x1), size_type(y3 - y0 + 1)};
      }
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (const graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcChord);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;

      calc_arcs(rect, size, &arcs, &segments, nullptr);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect, size, &arcs, nullptr, &rects);

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      pen p(b.color());
      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);
      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());

      XFillRectangles(display, g, g, rects.data(), (int)rects.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      Use<brush> br(g, b);
      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect, size, &arcs, &segments, &rects);

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      XFillRectangles(display, g, g, rects.data(), (int)rects.size());

      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    // --------------------------------------------------------------------------
    arc::arc (const core::point& pos,
              unsigned int radius,
              float startrad,
              float endrad)
      : pos(pos), radius(radius), start_radius(startrad), end_radius(endrad) {
    }

    void frame_arc (const graphics& g,
                    const pen& p,
                    const core::point& pos,
                    unsigned int radius,
                    float startrad,
                    float endrad) {
      Use<pen> pn(g, p);
      int x = pos.os_x() - radius;
      int y = pos.os_y() - radius;
      unsigned int sz = radius * 2;
      XDrawArc(get_instance(), g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));

      int istart = int(startrad * 1000.0F) % 360000;
      int iend = int(endrad * 1000.0F) % 360000;
      if (istart != iend) {
        double start = M_PI * startrad / 180.0;
        double end = M_PI * endrad / 180.0;
        os::point pt[3];
        pt[0].x = short(pos.x() + int(radius * cos(start)));
        pt[0].y = short(pos.y() - int(radius * sin(start)));
        pt[1].x = short(pos.x());
        pt[1].y = short(pos.y());
        pt[2].x = short(pos.x() + int(radius * cos(end)));
        pt[2].y = short(pos.y() - int(radius * sin(end)));
        XDrawLines(get_instance(), g, g, pt, 3, CoordModeOrigin);
      }
    }

    void fill_arc (const graphics& g,
                   const brush& b,
                   const core::point& pos,
                   unsigned int radius,
                   float startrad,
                   float endrad) {
      Use<brush> br(g, b);
      int x = pos.os_x() - radius;
      int y = pos.os_y() - radius;
      unsigned int sz = radius * 2;

      os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));

      XDrawArc(get_instance(), g, g, x, y, sz, sz, int(startrad * 64), int(endrad * 64));
    }

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      fill_arc(g, b, pos, radius, start_radius, end_radius);
      frame_arc(g, p, pos, radius, start_radius, end_radius);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      frame_arc(g, p, pos, radius, start_radius, end_radius);
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      fill_arc(g, b, pos, radius, start_radius, end_radius);
      frame_arc(g, pen(b.color()), pos, radius, start_radius, end_radius);
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      std::for_each(pts.begin(), pts.end(), [&](const core::point& pt){
        points.push_back(pt.os());
      });
      points.push_back(pts[0].os());
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      for(const core::point& pt : pts) {
        points.push_back(pt.os());
      }
      points.push_back(pts.begin()->os());
    }

    void polygon::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   (XPoint*)points.data(),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polygon::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      XFillPolygon(get_instance(),
                   g,
                   g,
                   const_cast<XPoint*>(points.data()),
                   (int)points.size(),
                   0,
                   CoordModeOrigin);
      pen p(b.color());
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (const graphics& g,
                               const font& f,
                               os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.width;
        dx = 0;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if ((static_cast<unsigned int>(origin) & DT_CENTER) == DT_CENTER) {
        px += (rect.width() - width) / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_RIGHT) == DT_RIGHT) {
        px += rect.width() - width;
      }
      if ((static_cast<unsigned int>(origin) & DT_VCENTER) == DT_VCENTER) {
        py += (rect.height() - dy) / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_BOTTOM) == DT_BOTTOM) {
        py += rect.height() - height;
      }

      xft_color xftcolor(c, g);
      clip clp(g, rect);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px + dx, py + dy, (XftChar8*)str.c_str(), int(str.size()));
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.xOff;
        dx = 0;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = rect.os_x();
      int py = rect.os_y();

      if ((static_cast<unsigned int>(origin) & DT_CENTER) == DT_CENTER) {
        px += (rect.width() - width) / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_RIGHT) == DT_RIGHT) {
        px += rect.width() - width;
      }
      if ((static_cast<unsigned int>(origin) & DT_VCENTER) == DT_VCENTER) {
        py += (rect.height() - dy) / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_BOTTOM) == DT_BOTTOM) {
        py += rect.height() - height;
      }

      rect.top_left({core::point::type(px + dx), core::point::type(py)});
      rect.set_size({core::size::type(width), core::size::type(height)});
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      os::instance display = get_instance();

      int height = 0, width = 0;
      int dx = 0, dy = 0;
      if (f.font_type()) {
        XGlyphInfo extents;
        XftTextExtentsUtf8(display,
                           f.font_type(),
                           (XftChar8*)str.c_str(),
                           int(str.size()),
                           &extents);
        auto fi = f.font_type();
        height = fi->ascent;
        width = extents.xOff;
        dx = extents.x;
        dy = fi->ascent - fi->descent;
      } else {
        LogError << "font_type is zero!";
      }

      int px = pos.os_x();
      int py = pos.os_y();

      if ((static_cast<unsigned int>(origin) & DT_CENTER) == DT_CENTER) {
        px -= width / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_RIGHT) == DT_RIGHT) {
        px -= width;
      }

      if ((static_cast<unsigned int>(origin) & DT_VCENTER) == DT_VCENTER) {
        py -= dy / 2;
      } else if ((static_cast<unsigned int>(origin) & DT_BOTTOM) == DT_BOTTOM) {
        py -= height;
      }

      xft_color xftcolor(c, g);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(), px + dx, py + dy, (XftChar8*)str.c_str(), int(str.size()));
    }

    // --------------------------------------------------------------------------
    const graphics& graphics::draw_pixel (const core::point& pt,
                                          os::color c) const {
      Use<pen> pn(gc, pen(c));
      XDrawPoint(get_instance(), target, gc, pt.os_x(), pt.os_y());
      return *this;
    }

    os::color graphics::get_pixel (const core::point& pt) const {
      return color::black;
    }

    const graphics& graphics::draw_lines (std::initializer_list<core::point> pts,
                                          const pen& p) const {

      Use<pen> pn(gc, p);
      std::vector<os::point> points;
      points.reserve(pts.size());
      for(const core::point& pt : pts) {
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
      int res = XCopyArea(get_instance(), src, target, gc, r.os_x(), r.os_y(), r.os_width(), r.os_height(), pt.os_x(), pt.os_y());
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
        res = XCopyArea(get_instance(), w, target, gc, r.os_x(), r.os_y(), r.os_width(), r.os_height(), pt.os_x(), pt.os_y());
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      return *this;
    }

    const graphics& graphics::stretch_from (os::drawable w,
                                            const core::rectangle& r,
                                            const core::rectangle& pt) const {
      return copy_from(w, r, pt.position());
    }

    const graphics& graphics::copy_from(const draw::masked_bitmap& bmp, const core::point& pt) const {
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
      os::rectangle r = rect;
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

#endif // X11
    const graphics& graphics::clear(os::color color) const {
      rectangle r(core::rectangle(0, 0, 0xffff, 0xffff));
      r.operator()(*this, brush(color));
      return *this;
    }

//#if WIN32
//    struct log_palette : public LOGPALETTE {
//      log_palette (int bpp);

//    private:
//      PALETTEENTRY moreEntries[255];
//    };

//    struct palette {
//      palette (const log_palette&);
//      ~palette ();

//      HPALETTE get_id() const {
//        return id;
//      }

//      //    private:
//      HPALETTE id;
//    };

//    struct bitmap_info : public BITMAPINFO {

//      bitmap_info();
//      bitmap_info(int w, int h, int bpl, int bpp);

//      void init_colors();
//      void init_gray_colors();
//      void init_bw_colors();

//      void set_gray_colors(HBITMAP id);
//      void set_gray_colors(HDC id);
//      void set_bw_colors(HBITMAP id);
//      void set_bw_colors(HDC id);

//    private:
//      RGBQUAD moreColors[255];
//    };

//    log_palette::log_palette(int bpp) {
//      palVersion = 0x300;
//      palNumEntries = (DWORD)(bpp < 24 ? (0x01 << bpp) : 0);
//      if (bpp == 8) {
//        for (int i = 0; i < 256; ++i) {
//          palPalEntry[i] = {(BYTE)i, (BYTE)i, (BYTE)i, PC_NOCOLLAPSE};
//        }
//      } else if (bpp == 1) {
//        palPalEntry[0] = {0, 0, 0, PC_NOCOLLAPSE};
//        palPalEntry[1] = {0xff, 0xff, 0xff, PC_NOCOLLAPSE};
//      }
//    }

//    palette::palette(const log_palette& pal) {
//      id = CreatePalette(&pal);
//    }

//    palette::~palette() {
//      if (id) {
//        DeleteObject(id);
//        id = NULL;
//      }
//    }

//    bitmap_info::bitmap_info () {
//      memset(this, 0, sizeof(bitmap_info));
//      bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//      init_gray_colors();
//    }

//    bitmap_info::bitmap_info (int w, int h, int bpl, int bpp) {
//      DWORD cols = (DWORD)(bpp < 24 ? (0x01 << bpp) : 0);
//      bmiHeader = {
//        sizeof(BITMAPINFOHEADER),
//        w, -h, 1, (WORD)bpp,
//        BI_RGB,
//        (DWORD)(h * bpl),
//        0, 0,
//        cols,
//        0
//      };
//      if (bpp == 8) {
//        init_gray_colors();
//      } else if (bpp == 1) {
//        init_bw_colors();
//      }
//    }

//    void bitmap_info::init_colors () {
//      HDC dc = GetDC(NULL);
//      GetDIBColorTable(dc, 0, 256, bmiColors);
//      ReleaseDC(NULL, dc);
//    }

//    void bitmap_info::init_gray_colors () {
//      for (int i = 0; i < 256; ++i) {
//        bmiColors[i] = {(BYTE)i, (BYTE)i, (BYTE)i, 0};
//      }
//    }

//    void bitmap_info::init_bw_colors() {
//      bmiColors[1] = {0, 0, 0, 0};
//      bmiColors[0] = {0xff, 0xff, 0xff, 0};
//    }

//    void bitmap_info::set_gray_colors(HBITMAP id) {
//      HDC gdc = GetDC(NULL);
//      HDC gc = CreateCompatibleDC(gdc);
//      ReleaseDC(NULL, gdc);
//      SelectObject(gc, id);
//      set_gray_colors(gc);
//      DeleteDC(gc);
//    }

//    void bitmap_info::set_gray_colors(HDC gc) {
//      init_gray_colors();
//      UINT ret = SetDIBColorTable(gc, 0, 256, bmiColors);
//      if (ret != 256) {
//        LogError << "SetDIBColorTable returned " << ret << " (expected 256)";
//      }
//    }

//    void bitmap_info::set_bw_colors(HBITMAP id) {
//      HDC gdc = GetDC(NULL);
//      HDC gc = CreateCompatibleDC(gdc);
//      ReleaseDC(NULL, gdc);
//      SelectObject(gc, id);
//      set_bw_colors(gc);
//      DeleteDC(gc);
//    }

//    void bitmap_info::set_bw_colors(HDC gc) {
//      init_bw_colors();
//      UINT ret = SetDIBColorTable(gc, 0, 2, bmiColors);
//      if (ret != 2) {
//        LogError << "SetDIBColorTable returned " << ret << " (expected 2)";
//      }
//    }

//#endif


    const graphics& graphics::copy_from(const draw::bitmap& bmp, const core::point& pt) const {
      if (bmp) {
        int dep = bmp.depth();
        if (dep == depth()) {
          return copy_from(bmp.get_id(), core::rectangle(bmp.size()), pt);
        } else {
          return copy_from(memmap(bmp).get_id(), core::rectangle(bmp.size()), pt);
        }
      }
      return *this;
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

    buffered_paint::buffered_paint (painter f)
      : p(f)
    {}

    void buffered_paint::operator () (const draw::graphics& g) {
      if (p) {
        const auto area = g.area();
        draw::memmap buffer(area.size());
        //{
          draw::graphics graph(buffer);
          p(graph);
        //}
        //g.copy_from(buffer);
          g.copy_from(graph);
      }
    }

    namespace frame {

      // --------------------------------------------------------------------------
      void black (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(r), color::black);
      }

      void white (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(r), color::white);
      }

      void dots (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(r), pen(color::black, 1, pen::Style::dot));
      }

      void lines (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, color::very_light_gray);
      }

      void vline (const draw::graphics& g, const core::rectangle& place) {
        g.frame(line(place.top_right(), place.bottom_right()), color::very_light_gray);
      }

      void hline (const draw::graphics& g, const core::rectangle& place) {
        g.frame(line(place.bottom_left(), place.bottom_right()), color::very_light_gray);
      }

      void vraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(1, 0);
        g.frame(line(r.top_left(), r.bottom_left()), color::white);
        g.frame(line(r.top_right(), r.bottom_right()), color::gray);
      }

      void hraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(0, 1);
        g.frame(line(r.top_left(), r.top_right()), color::white);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::gray);
      }

      void vgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(1, 0);
        g.frame(line(r.top_left(), r.bottom_left()), color::gray);
        g.frame(line(r.top_right(), r.bottom_right()), color::white);
      }

      void hgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size(0, 1);
        g.frame(line(r.top_left(), r.top_right()), color::gray);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::white);
      }

      void raised_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::white);
        g.draw_lines({ r.top_right(), r.bottom_right(), r.bottom_left() }, color::gray);
      }

      void sunken_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::gray);
        g.draw_lines({ r.top_right(), r.bottom_right(), r.bottom_left() }, color::white);
      }

      void raised_deep_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::white);
        g.draw_lines({ r.bottom_left(), r.bottom_right(), r.top_right() }, color::gray);

        const core::point pm = core::point(1, -1);
        g.draw_lines({ r.bottom_left() + pm, r.top_left() + core::point::one, r.top_right() - pm }, color::very_light_gray);
        g.draw_lines({ r.bottom_left() + pm, r.bottom_right() - core::point::one, r.top_right() - pm }, color::medium_gray);
      }

      void sunken_deep_relief(const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({ r.bottom_left(), r.top_left(), r.top_right() }, color::gray);
        g.draw_lines({ r.bottom_left(), r.bottom_right(), r.top_right() }, color::white);

        const core::point pm = core::point(1, -1);
        g.draw_lines({ r.bottom_left() + pm, r.top_left() + core::point::one, r.top_right() - pm }, color::dark_gray);
        g.draw_lines({ r.bottom_left() + pm, r.bottom_right() - core::point::one, r.top_right() - pm }, color::very_light_gray);
      }

      // --------------------------------------------------------------------------
    } // frame

  }

}
