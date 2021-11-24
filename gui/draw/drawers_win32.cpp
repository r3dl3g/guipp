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

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <util/string_util.h>

#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/arc_coords.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/use.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace draw {

    // --------------------------------------------------------------------------
    void line::operator() (graphics& g, const pen& p) const {
      const auto pw = p.os_size();
      const auto off = p.os_size() / 2;
      const auto x = from.os_x(g.context()) + off;
      const auto y = from.os_y(g.context()) + off;
      const auto x2 = to.os_x(g.context()) + off;
      const auto y2 = to.os_y(g.context()) + off;
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
    void rectangle::operator() (graphics& g,
                                const brush& b,
                                const pen& p) const {
      const os::rectangle r = rect.os(g.context());
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

    void rectangle::operator() (graphics& g,
                                const pen& p) const {
      const os::rectangle r = rect.os(g.context());
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
        Use<brush> ubr(g, brush::invisible);
        const auto tl = pen_offsets_tl(pw);
        const auto br = pen_offsets_br(pw);
        Rectangle(g, r.left + tl, r.top + tl, r.right + br, r.bottom + br);
      }
    }

    void rectangle::operator() (graphics& g,
                                const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void ellipse::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      const os::rectangle r = rect.os(g.context());
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

    void ellipse::operator() (graphics& g,
                              const pen& p) const {
      const os::rectangle r = rect.os(g.context());
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
        Use<brush> ubr(g, brush::invisible);
        if ((r.right - r.left < 2) || (r.bottom - r.top < 2)) {
          const auto br = pen_offsets_br(pw + 2);
          Rectangle(g, r.left, r.top, r.right + br + 2, r.bottom + br + 2);
        } else {
          Ellipse(g, r.left, r.top, r.right + pw, r.bottom + pw);
        }
      }

    }

    void ellipse::operator() (graphics& g,
                              const brush& b) const {
      operator()(g, b, b.color());
    }

    // --------------------------------------------------------------------------
    void round_rectangle::operator() (graphics& g,
                                      const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, brush::invisible);
      RoundRect(g,
                rect.os_x(g.context()),
                rect.os_y(g.context()),
                rect.os_x2(g.context()),
                rect.os_y2(g.context()),
                (radius.os_width() * 2),
                (radius.os_height() * 2));
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(g.context()),
                rect.os_y(g.context()),
                rect.os_x2(g.context()),
                rect.os_y2(g.context()),
                (radius.os_width() * 2),
                (radius.os_height() * 2));
    }

    void round_rectangle::operator() (graphics& g,
                                      const brush& b,
                                      const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      RoundRect(g,
                rect.os_x(g.context()),
                rect.os_y(g.context()),
                rect.os_x2(g.context()),
                rect.os_y2(g.context()),
                (radius.os_width() * 2),
                (radius.os_height() * 2));
    }

    // --------------------------------------------------------------------------
    void draw_full_arc (graphics& g, const arc_coords& c) {
      if ((c.w < 3) || (c.h < 3)) {
        Rectangle(g, c.x, c.y, c.x + c.w + 1, c.y + c.h + 1);
      } else {
        Ellipse(g, c.x, c.y, c.x + c.w + 1, c.y + c.h + 1);
      }
    }

    template<arc_type T>
    void draw_empty_arc (graphics& g, const arc_coords&, os::color);

    template<>
    void draw_empty_arc<arc_type::pie> (graphics& g, const arc_coords& c, os::color) {
      auto pt = c.calc_points0();
      MoveToEx(g, os::get_x(pt[1]), os::get_y(pt[1]), nullptr);
      LineTo(g, os::get_x(pt[0]), os::get_y(pt[0]));
    }

    template<>
    void draw_empty_arc<arc_type::arc> (graphics& g, const arc_coords& c, os::color col) {
      auto pt = c.calc_points0();
      SetPixel(g, os::get_x(pt[1]), os::get_y(pt[1]), col);
    }

    template<arc_type T>
    void draw_angle_arc (graphics& g, const arc_coords&);

    template<>
    void draw_angle_arc<arc_type::pie> (graphics& g, const arc_coords& c) {
      auto pt = c.calc_points();
      Pie(g, c.x, c.y, c.x + c.w + 1, c.y + c.h + 1, pt[0].x, pt[0].y, pt[2].x, pt[2].y);
    }

    template<>
    void draw_angle_arc<arc_type::arc> (graphics& g, const arc_coords& c) {
      auto pt = c.calc_points();
      Arc(g, c.x, c.y, c.x + c.w + 1, c.y + c.h + 1, pt[0].x, pt[0].y, pt[2].x, pt[2].y);
    }

    template<arc_type T>
    void draw_arc_ (graphics& g, const arc_coords& c, const pen& p) {
      Use<pen> pn(g, p);
      if (c.full()) {
        draw_full_arc(g, c);
      } else if (c.empty()) {
        draw_empty_arc<T>(g, c, p.color());
      } else {
        draw_angle_arc<T>(g, c);
      }
    }

    template<arc_type T>
    void fill_arc_ (graphics& g, const arc_coords& c, const brush& b) {
      Use<brush> br(g, b);
      if (c.full()) {
        draw_full_arc(g, c);
      } else if (c.empty()) {
        draw_empty_arc<T>(g, c, b.color());
      } else {
        draw_angle_arc<T>(g, c);
      }
    }

    template<> GUIPP_DRAW_EXPORT void draw_arc<arc_type::arc> (graphics& g, const arc_coords& c, const pen& p) {
      draw_arc_<arc_type::arc>(g, c, p);
    }

    template<> GUIPP_DRAW_EXPORT void draw_arc<arc_type::pie> (graphics& g, const arc_coords& c, const pen& p) {
      draw_arc_<arc_type::pie>(g, c, p);
    }

    template<> GUIPP_DRAW_EXPORT void fill_arc<arc_type::arc> (graphics& g, const arc_coords& c, const brush& b) {
      fill_arc_<arc_type::arc>(g, c, b);
    }

    template<> GUIPP_DRAW_EXPORT void fill_arc<arc_type::pie> (graphics& g, const arc_coords& c, const brush& b) {
      fill_arc_<arc_type::pie>(g, c, b);
    }

    // --------------------------------------------------------------------------
    void polyline::operator() (graphics& g,
                              const brush& b,
                              const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      auto pts = convert(g);
      Polyline(g, (const POINT*)pts.data(), (int)pts.size());
    }

    void polyline::operator() (graphics& g,
                              const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, brush::invisible);
      auto pts = convert(g);
      Polyline(g, (const POINT*)pts.data(), (int)pts.size());
    }

    void polyline::operator() (graphics& g,
                              const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      auto pts = convert(g);
      Polyline(g, (const POINT*)pts.data(), (int)pts.size());
    }

    // --------------------------------------------------------------------------
    void polygon::operator() (graphics& g,
                               const brush& b,
                               const pen& p) const {
      Use<brush> br(g, b);
      Use<pen> pn(g, p);
      auto pts = convert(g);
      Polygon(g, (const POINT*)pts.data(), (int)pts.size());
    }

    void polygon::operator() (graphics& g,
                               const pen& p) const {
      Use<pen> pn(g, p);
      Use<brush> br(g, brush::invisible);
      auto pts = convert(g);
      Polygon(g, (const POINT*)pts.data(), (int)pts.size());
    }

    void polygon::operator() (graphics& g,
                               const brush& b) const {
      Use<brush> br(g, b);
      pen p(b.color());
      Use<pen> pn(g, p);
      auto pts = convert(g);
      Polygon(g, (const POINT*)pts.data(), (int)pts.size());
    }

    // --------------------------------------------------------------------------
    void text_box::operator() (graphics& g,
                               const font& f,
                               os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, TRANSPARENT);
      RECT Rect = rect.os(g.context());

      std::wstring wstr = util::string::utf8_to_utf16(str);

      unsigned int o = static_cast<unsigned int>(origin);
      if (core::bit_mask<unsigned int, DT_WORDBREAK | DT_VCENTER>::is_set(o)) {
        int h = DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o | DT_CALCRECT);
        Rect.left = rect.os_x(g.context());
        Rect.right = rect.os_x2(g.context());
        Rect.top = ((rect.size().os_height() - h) / 2);
        Rect.bottom = Rect.top + h;
      }
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &Rect, o | DT_NOPREFIX);
      SetBkMode(g, old_mode);
      SetTextColor(g, old_color);
    }

    // --------------------------------------------------------------------------
    void bounding_box::operator() (graphics& g,
                                   const font& f,
                                   os::color) const {
      Use<font> fn(g, f);
      RECT r = rect.os(g.context());
      std::wstring wstr = util::string::utf8_to_utf16(str);
      DrawTextW(g, wstr.c_str(), (int)wstr.size(), &r, static_cast<unsigned int>(text_origin_t::top_left) | DT_CALCRECT);
      core::rectangle rr(r, g.context());
      if (origin_is_right(origin)) {
        rect.set_horizontal(rect.x2() - rr.width(), rr.width());
      } else if (origin_is_h_center(origin)) {
        rect.set_horizontal(rect.center_x() - rr.width() / 2, rr.width());
      } else {
        rect.width(rr.width());
      }
      if (origin_is_bottom(origin)) {
        rect.set_vertical(rect.y2() - rr.height(), rr.height());
      } else if (origin_is_v_center(origin)) {
        rect.set_vertical(rect.center_y() - rr.height() / 2, rr.height());
      } else {
        rect.height(rr.height());
      }
    }

    // --------------------------------------------------------------------------
    void text::operator() (graphics& g,
                           const font& f,
                           os::color c) const {
      Use<font> fn(g, f);
      os::color old_color = SetTextColor(g, c);
      int old_mode = SetBkMode(g, TRANSPARENT);
      int px = pos.os_x(g.context());
      int py = pos.os_y(g.context());
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

  } // namespace draw

} // namespace gui

#endif // GUIPP_WIN
