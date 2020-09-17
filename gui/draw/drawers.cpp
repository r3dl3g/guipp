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
#include <functional>
#ifdef X11
# include <cmath>
# include <algorithm>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <util/ostreamfmt.h>
#ifdef WIN32
# include <util/string_util.h>
#endif // WIN32

#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/bitmap.h>
#include <gui/draw/use.h>


namespace gui {

  namespace draw {

#ifdef WIN32
    brush null_brush((os::brush)GetStockObject(NULL_BRUSH));
    pen null_pen((os::pen)GetStockObject(NULL_PEN));

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      const auto pw = p.os_size();
      const auto off = p.os_size() / 2;
      const auto x = from.os_x() + off;
      const auto y = from.os_y() + off;
      const auto x2 = to.os_x() + off;
      const auto y2 = to.os_y() + off;
      if ((x == x2) && (y == y2)) {
        if (pw < 2) {
          SetPixel(g, x, y, p.color());
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);
          Rectangle(g, x - off, y - off, x + pw - 1, y + pw - 1);
        }
      } else {
        Use<pen> pn(g, p);
        MoveToEx(g, x, y, nullptr);
        LineTo(g, x2, y2);
      }
    }

    // --------------------------------------------------------------------------
    constexpr int pen_offsets_tl (int i) {
      return i/2;
    }

    constexpr int pen_offsets_br (int i) {
      return -pen_offsets_tl(i - 1);
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      const os::rectangle r = rect.os();
      const auto pw = p.os_size();
      if ((r.left + pw == r.right) && (r.top + pw == r.bottom)) {
        if (pw < 2) {
          SetPixel(g, r.left, r.top, p.color());
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);
          Rectangle(g, r.left, r.top, r.right, r.bottom);
        }
      } else if ((r.right > r.left) && (r.bottom > r.top)) {
        Use<brush> ubr(g, b);
        Use<pen> upn(g, p);
        const auto tl = pen_offsets_tl(pw);
        const auto br = pen_offsets_br(pw);
        Rectangle(g, r.left + tl, r.top + tl, r.right + br, r.bottom + br);
      }
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      const os::rectangle r = rect.os();
      const auto pw = p.os_size();
      if ((r.left + pw == r.right) && (r.top + pw == r.bottom)) {
        if (pw < 2) {
          SetPixel(g, r.left, r.top, p.color());
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);
          Rectangle(g, r.left, r.top, r.right, r.bottom);
        }
      } else if ((r.right > r.left) && (r.bottom > r.top)) {
        Use<pen> upn(g, p);
        Use<brush> ubr(g, null_brush);
        const auto tl = pen_offsets_tl(pw);
        const auto br = pen_offsets_br(pw);
        Rectangle(g, r.left + tl, r.top + tl, r.right + br, r.bottom + br);
      }
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      const os::rectangle r = rect.os();
      const auto pw = p.os_size();
      if ((r.left == r.right) && (r.top == r.bottom)) {
        if (pw < 2) {
          SetPixel(g, r.left, r.top, p.color());
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);
          Rectangle(g, r.left, r.top, r.right + pw, r.bottom + pw);
        }
      } else if ((r.right > r.left) && (r.bottom > r.top)) {
        const auto tl = pen_offsets_tl(pw);
        if ((r.right - r.left < 3) || (r.bottom - r.top < 3)) {
          Use<brush> ubr(g, b);
          Use<pen> upn(g, p);
          Rectangle(g, r.left + tl, r.top + tl, r.right + pw, r.bottom + pw);
        } else {
          const auto br = pen_offsets_tl(pw + 1);
          Use<brush> ubr(g, b);
          Use<pen> upn(g, p);
          Ellipse(g, r.left + tl, r.top + tl, r.right + br, r.bottom + br);
        }
      }
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      const os::rectangle r = rect.os();
      const auto pw = p.os_size();
      if ((r.left == r.right) && (r.top == r.bottom)) {
        if (pw < 2) {
          SetPixel(g, r.left, r.top, p.color());
        } else {
          pen p1 = p.with_os_size(1);
          brush b1(p.color());
          Use<pen> upn(g, p1);
          Use<brush> ubr(g, b1);
          Rectangle(g, r.left, r.top, r.right + pw, r.bottom + pw);
        }
      } else if ((r.right > r.left) && (r.bottom > r.top)) {
        Use<pen> upn(g, p);
        Use<brush> ubr(g, null_brush);
        if ((r.right - r.left < 2) || (r.bottom - r.top < 2)) {
          const auto br = pen_offsets_br(pw + 2);
          Rectangle(g, r.left, r.top, r.right + br + 2, r.bottom + br + 2);
        } else {
          Ellipse(g, r.left, r.top, r.right + pw, r.bottom + pw);
        }
      }

    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      operator()(g, b, b.color());
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
              float radius,
              float start_angle,
              float end_angle)
      : pos(pos)
      , radius(radius)
      , start_angle(start_angle)
      , end_angle(end_angle) 
    {}

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      const uint32_t sz = static_cast<unsigned int>(core::global::scale<uint32_t>(radius * 2));

      auto end_a = end_angle;
      while (end_a < start_angle) {
        end_a += 360;
      }

      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      if ((end_a - start_angle == 360)) {
        const auto tl = pos - core::size(radius, radius);
        const auto x = tl.os_x();
        const auto y = tl.os_y();
        if (sz < 3) {
          Rectangle(g, x, y, x + sz + 1, y + sz + 1);
        } else {
          Ellipse(g, x, y, x + sz + 1, y + sz + 1);
        }
      } else {
        const auto x = pos.os_x();
        const auto y = pos.os_y();
        const auto r = core::global::scale<DWORD>(radius);
        BeginPath(g);
        MoveToEx(g, x, x, nullptr);
        AngleArc(g, x, x, r, start_angle, end_a - start_angle);
        LineTo(g, x, x);
        EndPath(g);
        StrokeAndFillPath(g);
      }
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      const uint32_t sz = core::global::scale<uint32_t>(radius * 2);

      auto end_a = end_angle;
      while (end_a < start_angle) {
        end_a += 360;
      }
      Use<pen> pn(g, p);
      if ((end_a - start_angle == 360)) {
        const auto tl = pos - core::size(radius, radius);
        const auto x = tl.os_x();
        const auto y = tl.os_y();
        Use<brush> br(g, null_brush);
        if (sz < 3) {
          Rectangle(g, x, y, x + sz + 1, y + sz + 1);
        } else {
          Ellipse(g, x, y, x + sz + 1, y + sz + 1);
        }
      } else {
        const auto x = pos.os_x();
        const auto y = pos.os_y();
        const auto r = core::global::scale<DWORD>(radius);
        BeginPath(g);
        MoveToEx(g, x, y, nullptr);
        AngleArc(g, x, y, r, start_angle, end_a - start_angle);
        LineTo(g, x, y);
        EndPath(g);
        StrokePath(g);
      }
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (const graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      Polyline(g, (const POINT*)points.data(), (int)points.size());
    }

    void polyline::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, null_brush);
      Polyline(g, (const POINT*)points.data(), (int)points.size());
    }

    void polyline::operator() (const graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      Polyline(g, (const POINT*)points.data(), (int)points.size());
    }

    // --------------------------------------------------------------------------
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
      int old_mode = SetBkMode(g, TRANSPARENT);
      RECT Rect = rect;

      std::wstring wstr = util::string::utf8_to_utf16(str);

      unsigned int o = static_cast<unsigned int>(origin);
      if (core::bit_mask<unsigned int, DT_WORDBREAK | DT_VCENTER>::is_set(o)) {
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
                                   os::color) const {
      Use<font> fn(g, f);
      RECT r = rect;
      std::wstring wstr = util::string::utf8_to_utf16(str);
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &r, static_cast<unsigned int>(origin) | DT_CALCRECT);
      rect = core::rectangle(r);
      //auto sz = f.get_text_size(str);
      //if (rect.size() != sz) {
      //  clog::debug() << "Text size differs: DT_CALCRECT:" << rect.size() << ", GetTextExtentPoint32:" << sz;
      //}
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, TRANSPARENT);
      int px = pos.os_x();
      int py = pos.os_y();
      unsigned int old_align = static_cast<unsigned int>(text_origin_t::top_left);
      std::wstring wstr = util::string::utf8_to_utf16(str);

      switch (origin) {
      case text_origin_t::top_left:
        old_align = SetTextAlign(g, TA_LEFT | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin_t::top_right:
        old_align = SetTextAlign(g, TA_RIGHT | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin_t::top_hcenter:
        old_align = SetTextAlign(g, TA_CENTER | TA_TOP | TA_NOUPDATECP);
        break;
      case text_origin_t::bottom_left:
        old_align = SetTextAlign(g, TA_LEFT | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin_t::bottom_right:
        old_align = SetTextAlign(g, TA_RIGHT | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin_t::bottom_hcenter:
        old_align = SetTextAlign(g, TA_CENTER | TA_BOTTOM | TA_NOUPDATECP);
        break;
      case text_origin_t::vcenter_right: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_RIGHT | TA_NOUPDATECP);
        break;
      }
      case text_origin_t::vcenter_left: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_LEFT | TA_NOUPDATECP);
        break;
      }
      case text_origin_t::center: {
        SIZE sz;
        GetTextExtentPoint32W(g, wstr.c_str(), (int)wstr.size(), &sz);
        py -= sz.cy / 2;
        old_align = SetTextAlign(g, TA_CENTER | TA_NOUPDATECP);
        break;
      }
      case text_origin_t::undefined:
        break;
      }
      TextOutW(g, px, py, wstr.c_str(), (int)wstr.size());
      SetTextAlign(g, old_align);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

#endif // WIN32

#ifdef X11
    using namespace core::global;

#ifdef USE_XFT
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
        : xft(xft) {
        render_color xrcolor(c);
        XftColorAllocValue(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), &xrcolor, this);
      }

      ~xft_color () {
        XftColorFree(get_instance(), XftDrawVisual(xft), XftDrawColormap(xft), this);
      }

      XftDraw* xft;
    };
#endif // USE_XFT

    // --------------------------------------------------------------------------
    void line::operator() (const graphics& g, const pen& p) const {
      gui::os::instance display = get_instance();
      const short off = p.os_size() / 2;
      Use<pen> pn(g, p);

      const auto x0 = from.os_x() + off;
      const auto y0 = from.os_y() + off;
      const auto x1 = to.os_x() + off;
      const auto y1 = to.os_y() + off;

      XDrawLine(display, g, g, x0, y0, x1, y1);
    }

    // --------------------------------------------------------------------------
    void rectangle::operator() (const graphics& g,
                                const brush& b,
                                const pen& p) const {
      gui::os::instance display = get_instance();

      const auto pw = p.os_size();
      const auto off = pw / 2;

      const os::rectangle r = rect.os();
      if ((r.width > pw) && (r.height > pw)) {
        Use<brush> br(g, b);
        XFillRectangle(display, g, g, r.x + pw, r.y + pw, r.width - pw * 2, r.height - pw * 2);
        Use<pen> pn(g, p);
        XDrawRectangle(display, g, g, r.x + off, r.y + off, r.width - pw, r.height - pw);
      } else if ((r.width > 1) && (r.height > 1)) {
        Use<brush> br(g, brush(p.color()));
        XFillRectangle(display, g, g, r.x, r.y, pw, pw);
      } else if ((1 == r.width) && (1 == r.height)) {
        Use<pen> pn(g, p);
        XDrawPoint(display, g, g, r.x + off, r.y + off);
      }
    }

    void rectangle::operator() (const graphics& g,
                                const pen& p) const {
      const auto pw = p.os_size();
      const auto off = pw / 2;
      const os::rectangle r = rect.os();
      if ((r.width > pw) && (r.height > pw)) {
        Use<pen> pn(g, p);
        XDrawRectangle(get_instance(), g, g, r.x + off, r.y + off, r.width - pw, r.height - pw);
      } else if ((r.width > 1) && (r.height > 1)) {
        Use<brush> br(g, brush(p.color()));
        XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
      } else if ((1 == r.width) && (1 == r.height)) {
        Use<pen> pn(g, p);
        XDrawPoint(get_instance(), g, g, r.x + off, r.y + off);
      }
    }

    void rectangle::operator() (const graphics& g,
                                const brush& b) const {
      operator ()(g, b, b.color());
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
      gui::os::instance display = get_instance();
      const auto pw = p.os_size();
      const auto off = (pw - 1) / 2;
      const os::rectangle r = rect.os();

      if ((r.width == 0) && (r.height == 0)) {
        if (pw < 2) {
          Use<pen> pn(g, p);
          XDrawPoint(display, g, g, r.x + off, r.y + off);
        } else {
          Use<brush> br(g, brush(p.color()));
          XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
        }
      } else {
        const auto soff = -(pw + 1) % 2;
        Use<brush> br(g, b);
        XSetArcMode(display, g, ArcPieSlice);
        XFillArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
        Use<pen> pn(g, p);
        XSetArcMode(display, g, ArcChord);
        XDrawArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
      }
    }

    void ellipse::operator() (const graphics& g,
                              const pen& p) const {
      gui::os::instance display = get_instance();
      const auto pw = p.os_size();
      const auto off = (pw - 1) / 2;
      const os::rectangle r = rect.os();
      if ((r.width == 0) && (r.height == 0)) {
        if (pw < 2) {
          Use<pen> pn(g, p);
          XDrawPoint(display, g, g, r.x + off, r.y + off);
        } else {
          Use<brush> br(g, brush(p.color()));
          XFillRectangle(get_instance(), g, g, r.x, r.y, pw, pw);
        }
      } else {
        Use<pen> pn(g, p);
        XSetArcMode(display, g, ArcChord);
        const auto soff = -(pw + 1) % 2;
        XDrawArc(display, g, g, r.x + off, r.y + off, r.width - soff, r.height - soff, 0, degree_360);
      }
    }

    void ellipse::operator() (const graphics& g,
                              const brush& b) const {
      operator ()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void calc_arcs (const core::rectangle& rect,
                    const core::size& size,
                    std::array<XArc, 4>* arcs,
                    std::array<XSegment, 4>* segments,
                    std::array<XRectangle, 3>* rects) {

      using namespace os;

      constexpr size_type two = size_type(2);

      const os::rectangle r = rect.os();
      const os::size s = size.os();

      const size_type w = std::min(s.cx, static_cast<size_type>(r.width / 2));
      const size_type h = std::min(s.cy, static_cast<size_type>(r.height / 2));

      const point_type x0 = r.x;
      const point_type x3 = r.x + r.width;
      const point_type x1 = x0 + w;
      const point_type x2 = x3 - w;

      const point_type y0 = r.y;
      const point_type y3 = r.y + r.height;
      const point_type y1 = y0 + h;
      const point_type y2 = y3 - h;

      const size_type w2 = w * two;
      const size_type h2 = h * two;

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
      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcChord);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;

      calc_arcs(rect - core::size::one, size, &arcs, &segments, nullptr);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    void round_rectangle::operator() (const graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect - core::size::one, size, &arcs, nullptr, &rects);

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
      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);

      std::array<XArc, 4> arcs;
      std::array<XSegment, 4> segments;
      std::array<XRectangle, 3> rects;
      calc_arcs(rect - core::size::one, size, &arcs, &segments, &rects);

      XFillArcs(display, g, g, arcs.data(), (int)arcs.size());
      XFillRectangles(display, g, g, rects.data(), (int)rects.size());

      Use<pen> pn(g, p);
      XSetArcMode(display, g, ArcChord);

      XDrawArcs(display, g, g, arcs.data(), (int)arcs.size());
      XDrawSegments(display, g, g, segments.data(), (int)segments.size());
    }

    // --------------------------------------------------------------------------
    arc::arc (const core::point& pos,
              float radius,
              float start_angle,
              float end_angle)
      : pos(pos), radius(radius), start_angle(start_angle), end_angle(end_angle) {}

    void frame_arc (const graphics& g,
                    const pen& p,
                    const core::point& pos,
                    float radius,
                    float start_angle,
                    float end_angle) {

      gui::os::instance display = get_instance();

      const auto tl = pos - core::size(radius, radius);
      const unsigned int sz = scale<unsigned int>(radius * 2);

      while (end_angle < start_angle) {
        end_angle += 360;
      }

      Use<pen> pn(g, p);
      XDrawArc(display, g, g, tl.os_x(), tl.os_y(), sz, sz, int(start_angle * 64), int((end_angle - start_angle) * 64));

      int istart = int(start_angle * 1000.0F) % 360000;
      int iend = int(end_angle * 1000.0F) % 360000;
      if (istart != iend) {
        double start = M_PI * start_angle / 180.0;
        double end = M_PI * end_angle / 180.0;
        os::point pt[3];
        pt[0].x = short(scale<os::point_type>(pos.x() + radius * cos(start)));
        pt[0].y = short(scale<os::point_type>(pos.y() - radius * sin(start)));
        pt[1].x = short(scale<os::point_type>(pos.x()));
        pt[1].y = short(scale<os::point_type>(pos.y()));
        pt[2].x = short(scale<os::point_type>(pos.x() + radius * cos(end)));
        pt[2].y = short(scale<os::point_type>(pos.y() - radius * sin(end)));
        XDrawLines(display, g, g, pt, 3, CoordModeOrigin);
        XDrawPoint(display, g, g, pt[0].x, pt[0].y);
        XDrawPoint(display, g, g, pt[2].x, pt[2].y);
      }
    }

    void fill_arc (const graphics& g,
                   const brush& b,
                   const core::point& pos,
                   float radius,
                   float start_angle,
                   float end_angle) {
      Use<brush> br(g, b);

      const auto tl = pos - core::size(radius, radius);
      const unsigned int sz = scale<unsigned int>(radius * 2);

      int x = tl.os_x();
      int y = tl.os_y();

      while (end_angle < start_angle) {
        end_angle += 360;
      }

      gui::os::instance display = get_instance();
      XSetArcMode(display, g, ArcPieSlice);
      XFillArc(display, g, g, x, y, sz, sz, int(start_angle * 64), int((end_angle - start_angle) * 64));

      XDrawArc(get_instance(), g, g, x, y, sz, sz, int(start_angle * 64), int((end_angle - start_angle) * 64));
    }

    void arc::operator() (const graphics& g,
                          const brush& b,
                          const pen& p) const {
      fill_arc(g, b, pos, radius, start_angle, end_angle);
      frame_arc(g, p, pos, radius, start_angle, end_angle);
    }

    void arc::operator() (const graphics& g,
                          const pen& p) const {
      frame_arc(g, p, pos, radius, start_angle, end_angle);
    }

    void arc::operator() (const graphics& g,
                          const brush& b) const {
      fill_arc(g, b, pos, radius, start_angle, end_angle);
      frame_arc(g, pen(b.color()), pos, radius, start_angle, end_angle);
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (const graphics& g,
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

    void polyline::operator() (const graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      XDrawLines(get_instance(), g, g, const_cast<XPoint*>(points.data()), (int)points.size(), CoordModeOrigin);
    }

    void polyline::operator() (const graphics& g,
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
      gui::os::instance display = get_instance();

      int px = rect.os_x();
      int py = rect.os_y();
#ifdef USE_XFT
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
        clog::error() << "font_type is zero!";
      }

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() - height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height() - height;
      }

      xft_color xftcolor(c, g);
      clip clp(g, rect);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(),
                        px + dx, py + dy,
                        (XftChar8*)str.c_str(), int(str.size()));
#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        clog::error() << "font_type is zero!";
      }

      int width = overall.width;
      int height = (ascent - descent);

//        LogDebug << "r.x():" << px << " r.y():" << py
//                 << " r.size:" << rect.size()
//                 << " o.w:" << width << " o.h:" << height
//                 << " asc:" << ascent << " des:" << descent;

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() + height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height();
      } else {
        py += height;
      }

      XDrawString(core::global::get_instance(), g, g, px, py, str.c_str(), int(str.size()));
#endif // USE_XFT
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (const graphics& g,
                                   const font& f,
                                   os::color c) const {
      gui::os::instance display = get_instance();

      int px = rect.os_x();
      int py = rect.os_y();

#ifdef USE_XFT
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
        clog::error() << "font_type is zero!";
      }

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() - height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height() - height;
      }

      rect.top_left({core::global::scale<core::point::type>(px + dx), core::global::scale<core::point::type>(py)});
      rect.set_size({core::global::scale<core::size::type>(width), core::global::scale<core::size::type>(height)});
#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        clog::error() << "font_type is zero!";
      }

      int width = overall.width;
      int height = (ascent - descent);

      if (origin_is_h_center(origin)) {
        px += (rect.os_width() - width) / 2;
      } else if (origin_is_right(origin)) {
        px += rect.os_width() - width;
      }
      if (origin_is_v_center(origin)) {
        py += (rect.os_height() + height) / 2;
      } else if (origin_is_bottom(origin)) {
        py += rect.os_height();
      } else {
        py += height;
      }

      rect.top_left({core::point::type(px), core::point::type(py - overall.ascent)});
      rect.set_size({core::size::type(width), core::size::type(overall.ascent + overall.descent)});
#endif // USE_XFT
    }

    // --------------------------------------------------------------------------
    void text::operator() (const graphics& g,
                           const font& f,
                           os::color c) const {
      gui::os::instance display = get_instance();

      int px = pos.os_x();
      int py = pos.os_y();
#ifdef USE_XFT
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
        clog::error() << "font_type is zero!";
      }


      if (origin_is_h_center(origin)) {
        px -= width / 2;
      } else if (origin_is_right(origin)) {
        px -= width;
      }

      if (origin_is_v_center(origin)) {
        py -= dy / 2;
      } else if (origin_is_bottom(origin)) {
        py -= height;
      }

      xft_color xftcolor(c, g);

      XftDrawStringUtf8(g, &xftcolor, f.font_type(),
                        (px + dx), (py + dy),
                        (XftChar8*)str.c_str(), int(str.size()));
#else
      Use<font> fn(g, f);
      Use<pen> pn(g, c);

      int direction = 0, ascent = 0, descent = 0;
      XCharStruct overall;
      memset(&overall, 0, sizeof(XCharStruct));

      if (f.font_type()) {
        XTextExtents(f.font_type(), str.c_str(), int(str.size()),
                     &direction, &ascent, &descent, &overall);
      } else {
        clog::error() << "font_type is zero!";
      }

      if (origin_is_h_center(origin)) {
        px -= overall.width / 2;
      } else if (origin_is_right(origin)) {
        px -= overall.width;
      }
      int height = (overall.ascent - overall.descent);
      if (origin_is_v_center(origin)) {
        py += height / 2;
      } else if (origin_is_bottom(origin)) {
      } else {
        py += height;
      }

      XDrawString(display, g, g, px, py, str.c_str(), int(str.size()));
#endif // USE_XFT
    }

#endif // X11

    // --------------------------------------------------------------------------
    polyline::polyline (const std::vector<core::point>& pts) {
      points.reserve(pts.size());
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
    }

    polyline::polyline (std::initializer_list<core::point> pts) {
      points.reserve(pts.size());
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
    }

    // --------------------------------------------------------------------------
    polygon::polygon (const std::vector<core::point>& pts) {
      points.reserve(pts.size() + 1);
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
      points.push_back(pts[0].os());
    }

    polygon::polygon (std::initializer_list<core::point> pts) {
      points.reserve(pts.size() + 1);
      for (const core::point& pt : pts) {
        points.push_back(pt.os());
      }
      points.push_back(pts.begin()->os());
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t px_fmt>
    void copy_frame_image (draw::const_image_data<px_fmt> src_img,
                           draw::image_data<px_fmt> dest_img,
                           const bitmap_info& src_bmi, const bitmap_info& dest_bmi,
                           const core::native_rect& frame) {

      const int32_t width = dest_bmi.width;
      const int32_t height = dest_bmi.height;

      const int32_t left = std::min(frame.x(), width);
      const int32_t right = std::min<int32_t>(frame.width(), width - left);
      const int32_t top = std::min(frame.y(), height);
      const int32_t bottom = std::min<int32_t>(frame.height(), height - top);

      const int32_t target_right = dest_bmi.width - right;
      const int32_t target_bottom = dest_bmi.height - bottom;
      const int32_t source_right = src_bmi.width - right;
      const int32_t source_bottom = src_bmi.height - bottom;

      using namespace convert;

      // top left
      if (top && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, 0}, {0, 0, static_cast<uint32_t>(left), static_cast<uint32_t>(top)});
      }

      // top right
      if (top && right) {
        copy::sub<px_fmt>(src_img, dest_img, {source_right, 0}, {target_right, 0, static_cast<uint32_t>(right), static_cast<uint32_t>(top)});
      }

      // bottom left
      if (bottom && left) {
        copy::sub<px_fmt>(src_img, dest_img, {0, source_bottom}, {0, target_bottom, static_cast<uint32_t>(left), static_cast<uint32_t>(bottom)});
      }

      if (bottom && right) {
        // bottom right
        copy::sub<px_fmt>(src_img, dest_img,
                       {source_right, source_bottom}, {target_right, target_bottom, static_cast<uint32_t>(right), static_cast<uint32_t>(bottom)});
      }

      if ((target_right >= left) && (target_bottom >= top) && (source_right >= left) && (source_bottom >= top)) {
        const uint32_t target_inner_width = static_cast<uint32_t>(std::max(0, target_right - left));
        const uint32_t target_inner_height = static_cast<uint32_t>(std::max(0, target_bottom - top));
        const uint32_t source_inner_width = static_cast<uint32_t>(std::max(0, source_right - left));
        const uint32_t source_inner_height = static_cast<uint32_t>(std::max(0, source_bottom - top));

        // top center
        if (top && target_inner_width) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, 0, source_inner_width, static_cast<uint32_t>(top)},
                               {left, 0, target_inner_width, static_cast<uint32_t>(top)});
        }

        // bottom center
        if (bottom && target_inner_width) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, source_bottom, source_inner_width, static_cast<uint32_t>(bottom)},
                               {left, target_bottom, target_inner_width, static_cast<uint32_t>(bottom)});
        }

        // left center
        if (left && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {0, top, static_cast<uint32_t>(left), source_inner_height},
                               {0, top, static_cast<uint32_t>(left), target_inner_height});
        }

        // right center
        if (right && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {source_right, top, static_cast<uint32_t>(right), source_inner_height},
                               {target_right, top, static_cast<uint32_t>(right), target_inner_height});
        }

        // center
        if (target_inner_width && target_inner_height) {
          stretch<px_fmt>::sub(src_img, dest_img,
                               {left, top, source_inner_width, source_inner_height},
                               {left, top, target_inner_width, target_inner_height});
        }
      }
    }

    template<pixel_format_t T>
    void draw_image_frame (const graphics& g,
                           const core::point& pt,
                           const core::rectangle rect,
                           const datamap<T>& img,
                           const core::native_rect& frame) {
      if (rect.size() <= core::size::two) {
        return;
      }

      const uint32_t width = core::roundup<uint32_t>(rect.os_width());
      const uint32_t height = core::roundup<uint32_t>(rect.os_height());

      datamap<T> dest(width, height);
      copy_frame_image<T>(img.get_data(), dest.get_data(),
                          img.get_info(), dest.get_info(), frame);

      pixmap buffer;
      buffer = dest;
      g.copy_from(buffer, pt);
    }

    template<>
    void image_frame<pixel_format_t::BW>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::GRAY>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::RGB>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

    template<>
    void image_frame<pixel_format_t::RGBA>::operator() (const graphics& g, const core::point& pt) const {
      draw_image_frame(g, pt, rect, img, frame);
    }

  }

}
