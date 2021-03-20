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
// Library includes
//

#include <gui/draw/icons.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <util/string_util.h>


namespace gui {

  namespace draw {

    using namespace gui::core;
    namespace detail {

      // --------------------------------------------------------------------------
      icon_base::icon_base (const point& center, size::type radius)
        : center(center)
        , radius(radius)
      {}

    } // namespace detail

    // --------------------------------------------------------------------------
    point calc_clock_point (const point& center, const core::angle& angle, size::type radius) {
      const auto rad = angle.rad();
      return {
        static_cast<point::type>(center.x() + radius * sin(rad)),
        static_cast<point::type>(center.y() - radius * cos(rad))
      };
    }
    // --------------------------------------------------------------------------
    point calc_point_between (const point& p0, const point& p1, double f) {
      const auto d = (p1 - p0) * f;
      return p0 + d;
    }
    // --------------------------------------------------------------------------
    draw::line calc_centerline (const point& center, const core::angle& angle, size::type radius) {
      return { center, calc_clock_point(center, angle, radius) };
    }
    // --------------------------------------------------------------------------
    draw::line calc_centerline (const point& center, const core::angle& angle, size::type radius0, size::type radius1) {
      const auto rad = angle.rad();
      const auto ca = cos(rad);
      const auto sa = sin(rad);
      return {
        {static_cast<point::type>(center.x() + radius0 * sa), static_cast<point::type>(center.y() - radius0 * ca)},
        {static_cast<point::type>(center.x() + radius1 * sa), static_cast<point::type>(center.y() - radius1 * ca)}
      };
    }

    // --------------------------------------------------------------------------
    left_icon::left_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void left_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point(center, 30, radius);
      auto p1 = calc_clock_point(center, 150, radius);
      auto p2 = calc_clock_point(center, 270, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    right_icon::right_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void right_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point(center, 90, radius);
      auto p1 = calc_clock_point(center, 210, radius);
      auto p2 = calc_clock_point(center, 330, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    up_icon::up_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void up_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point(center, 120, radius);
      auto p1 = calc_clock_point(center, 240, radius);
      auto p2 = calc_clock_point(center, 0, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    down_icon::down_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void down_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point(center, 60, radius);
      auto p1 = calc_clock_point(center, 180, radius);
      auto p2 = calc_clock_point(center, 300, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    clock_icon::clock_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void clock_icon::operator() (graphics& g, const pen& pn) const {
      const auto r1 = radius * 0.5;
      const auto r2 = radius * 0.8;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline(center, 120.0F, r1), pn);
      g.frame(calc_centerline(center, 0, r2), pn);
    }

    // --------------------------------------------------------------------------
    stopwatch_icon::stopwatch_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void stopwatch_icon::operator() (graphics& g, const pen& pn) const {
      const auto r0 = radius * 1.2;
      const auto r2 = radius * 0.8;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline(center, 0, r2), pn);
      g.frame(draw::arc(center, r0, 80, 100), pn);
      g.frame(draw::arc(center, r0, 138, 132), pn);
      g.frame(draw::arc(center, r0, 48, 42), pn);
    }

    // --------------------------------------------------------------------------
    timer_icon::timer_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void timer_icon::operator() (graphics& g, const pen& pn) const {
      const auto r0 = radius * 1.2;
      const auto r1 = radius * 0.5;
      const auto r2 = radius * 0.8;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline(center, 90.0F, r1), pn);
      g.frame(calc_centerline(center, 0, r2), pn);
      g.frame(draw::arc(center, r0, 145, 125), pn);
      g.frame(draw::arc(center, r0, 55, 35), pn);
    }

    // --------------------------------------------------------------------------
    pause_icon::pause_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void pause_icon::operator() (graphics& g, const pen& pn) const {
      const auto tll = calc_clock_point(center, 315, radius);
      const auto trr = calc_clock_point(center, 45, radius);
      const auto tl = calc_point_between(tll, trr, 0.333);
      const auto tr = calc_point_between(tll, trr, 0.667);

      const auto bll = calc_clock_point(center, 225, radius);
      const auto brr = calc_clock_point(center, 135, radius);
      const auto bl = calc_point_between(bll, brr, 0.333);
      const auto br = calc_point_between(bll, brr, 0.667);
      g.frame(draw::polygon({tll, tl, bl, bll}), pn);
      g.frame(draw::polygon({tr, trr, brr, br}), pn);
    }

    // --------------------------------------------------------------------------
    stop_icon::stop_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void stop_icon::operator() (graphics& g, const pen& pn) const {
      const auto tll = calc_clock_point(center, 315, radius);
      const auto trr = calc_clock_point(center, 45, radius);
      const auto brr = calc_clock_point(center, 135, radius);
      const auto bll = calc_clock_point(center, 225, radius);
      g.frame(draw::polygon({tll, trr, brr, bll}), pn);
    }

    // --------------------------------------------------------------------------
    reset_icon::reset_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void reset_icon::operator() (graphics& g, const pen& pn) const {
      const auto p = calc_clock_point(center, -55, radius);
      const auto l = calc_centerline(center, -35, radius * 0.8, radius * 1.2);
      g.frame(draw::arc(center, radius, -145, 145), pn);
      g.frame(draw::polyline({l.p0(), p, l.p1()}), pn);
      g.fill(draw::pie(center, radius * 0.2, 0, 360), pn.color());
    }

    // --------------------------------------------------------------------------
  }

}
