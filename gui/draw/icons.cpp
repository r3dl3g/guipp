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
    point calc_clock_point (const point& center, double srad, double crad, size::type radius) {
      return {
        static_cast<point::type>(center.x() + radius * srad),
        static_cast<point::type>(center.y() - radius * crad)
      };
    }
    // --------------------------------------------------------------------------
    point calc_clock_point (const point& center, const core::angle& angle, size::type radius) {
      const auto rad = angle.rad();
      return calc_clock_point(center, sin(rad), cos(rad), radius);
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
        calc_clock_point(center, sa, ca, radius0), calc_clock_point(center, sa, ca, radius1)
      };
    }

    // --------------------------------------------------------------------------
    constexpr double radian (double degrees) {
      return static_cast<float>(M_PI * degrees / 180.0);
    }
    // --------------------------------------------------------------------------
    template<int angle>
    struct sinus {
      static constexpr double value = sin(radian(angle));
    };
    // --------------------------------------------------------------------------
    template<int angle>
    struct cosinus {
      static constexpr double value = cos(radian(angle));
    };
    // --------------------------------------------------------------------------
    template<int angle>
    point calc_clock_point (const point& center, size::type radius) {
      return calc_clock_point(center, sinus<angle>::value, cosinus<angle>::value, radius);
    }
    // --------------------------------------------------------------------------
    template<int angle>
    draw::line calc_centerline (const point& center, size::type radius) {
      return { center, calc_clock_point<angle>(center, radius) };
    }
    // --------------------------------------------------------------------------
    template<int angle>
    draw::line calc_centerline (const point& center, size::type radius0, size::type radius1) {
      const auto ca = cosinus<angle>::value;
      const auto sa = sinus<angle>::value;
      return {
        calc_clock_point(center, sa, ca, radius0), calc_clock_point(center, sa, ca, radius1)
      };
    }

    // --------------------------------------------------------------------------
    left_icon::left_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void left_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point<30>(center, radius);
      auto p1 = calc_clock_point<150>(center, radius);
      auto p2 = calc_clock_point<270>(center, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    right_icon::right_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void right_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point<90>(center, radius);
      auto p1 = calc_clock_point<210>(center, radius);
      auto p2 = calc_clock_point<330>(center, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    up_icon::up_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void up_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point<120>(center, radius);
      auto p1 = calc_clock_point<240>(center, radius);
      auto p2 = calc_clock_point<0>(center, radius);
      g.frame(draw::polygon({p0, p1, p2}), pn);
    }

    // --------------------------------------------------------------------------
    down_icon::down_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void down_icon::operator() (graphics& g, const pen& pn) const {
      auto p0 = calc_clock_point<60>(center, radius);
      auto p1 = calc_clock_point<180>(center, radius);
      auto p2 = calc_clock_point<300>(center, radius);
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
      g.frame(calc_centerline<120>(center, r1), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
    }

    // --------------------------------------------------------------------------
    stopwatch_icon::stopwatch_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void stopwatch_icon::operator() (graphics& g, const pen& pn) const {
      const auto r0 = radius * 1.2;
      const auto r2 = radius * 0.8;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
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
      g.frame(calc_centerline<90>(center, r1), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
      g.frame(draw::arc(center, r0, 145, 125), pn);
      g.frame(draw::arc(center, r0, 55, 35), pn);
    }

    // --------------------------------------------------------------------------
    pause_icon::pause_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void pause_icon::operator() (graphics& g, const pen& pn) const {
      const auto tll = calc_clock_point<315>(center, radius);
      const auto trr = calc_clock_point<45>(center, radius);
      const auto tl = calc_point_between(tll, trr, 0.333);
      const auto tr = calc_point_between(tll, trr, 0.667);

      const auto bll = calc_clock_point<225>(center, radius);
      const auto brr = calc_clock_point<135>(center, radius);
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
      const auto tll = calc_clock_point<315>(center, radius);
      const auto trr = calc_clock_point<45>(center, radius);
      const auto brr = calc_clock_point<135>(center, radius);
      const auto bll = calc_clock_point<225>(center, radius);
      g.frame(draw::polygon({tll, trr, brr, bll}), pn);
    }

    // --------------------------------------------------------------------------
    reset_icon::reset_icon (const point& center, size::type radius)
      : icon_base(center, radius)
    {}

    void reset_icon::operator() (graphics& g, const pen& pn) const {
      const auto p = calc_clock_point<-55>(center, radius);
      const auto l = calc_centerline<-35>(center, radius * 0.8, radius * 1.2);
      g.frame(draw::arc(center, radius, -145, 145), pn);
      g.frame(draw::polyline({l.p0(), p, l.p1()}), pn);
      g.fill(draw::pie(center, radius * 0.2, 0, 360), pn.color());
    }

    // --------------------------------------------------------------------------
    add_icon::add_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void draw_minus (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline({calc_clock_point<90>(center, radius), calc_clock_point<270>(center, radius)}), pn);
    }

    void draw_plus (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline({calc_clock_point<0>(center, radius), calc_clock_point<180>(center, radius)}), pn);
      draw_minus(g, pn, center, radius);
    }

    void add_icon::operator() (graphics& g, const pen& pn) const {
      draw_plus(g, pn, center, radius);
    }

    // --------------------------------------------------------------------------
    cancel_icon::cancel_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void cancel_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(draw::line(calc_clock_point<45>(center, radius), calc_clock_point<225>(center, radius)), pn);
    }

    // --------------------------------------------------------------------------
    okay_icon::okay_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void okay_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::polyline({calc_clock_point<-45>(center, radius), center,
                              calc_clock_point<45>(center, radius * 1.4142)}), pn);
    }

    // --------------------------------------------------------------------------
    close_icon::close_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void close_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::line(calc_clock_point<45>(center, radius), calc_clock_point<225>(center, radius)), pn);
      g.frame(draw::line(calc_clock_point<-45>(center, radius), calc_clock_point<135>(center, radius)), pn);
    }

    // --------------------------------------------------------------------------
    menu_icon::menu_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void menu_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::line(center - core::size(radius, radius),  center + core::size(radius, -radius)), pn);
      g.frame(draw::line(center - core::size(radius, 0),       center + core::size(radius, 0)), pn);
      g.frame(draw::line(center - core::size(radius, -radius), center + core::size(radius, radius)), pn);
    }

    // --------------------------------------------------------------------------
    clear_icon::clear_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void clear_icon::operator() (graphics& g, const pen& pn) const {
      const auto r = radius * 0.7;
      g.frame(draw::line(calc_clock_point<45>(center, r), calc_clock_point<225>(center, r)), pn);
      g.frame(draw::line(calc_clock_point<-45>(center, r), calc_clock_point<135>(center, r)), pn);
      g.frame(draw::arc(center, radius, 0, 360), pn);
    }

    // --------------------------------------------------------------------------
    find_icon::find_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void draw_find (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(draw::line(calc_clock_point<135>(center, radius * 1.1), calc_clock_point<135>(center, radius * 2)), pn);
    }

    void find_icon::operator() (graphics& g, const pen& pn) const {
      draw_find(g, pn, center - core::size(radius / 4, radius / 4), radius * 3 / 4);
    }

    // --------------------------------------------------------------------------
    undo_icon::undo_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void undo_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 0, 170), pn);
      const auto p = calc_clock_point<-90>(center, radius);
      const auto l = calc_centerline<-60>(center, radius * 0.8, radius * 1.2);
      g.draw(draw::polygon({l.p0(), p, l.p1()}), pn.color(), pn);
    }

    // --------------------------------------------------------------------------
    redo_icon::redo_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void redo_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 180, 330), pn);
      const auto p = calc_clock_point<100>(center, radius);
      const auto l = calc_centerline<120>(center, radius * 0.8, radius * 1.2);
      g.draw(draw::polygon({l.p0(), p, l.p1()}), pn.color(), pn);
    }

    // --------------------------------------------------------------------------
    info_icon::info_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void info_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.draw(draw::pie(center - core::size(0, radius / 2), radius / 20, 0, 360), pn.color(), pn);
      g.frame(draw::line(center - core::size(0, radius / 4),  center + core::size(0, radius / 2)), pn);
    }

    // --------------------------------------------------------------------------
    important_icon::important_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void important_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.draw(draw::pie(center + core::size(0, radius / 2), radius / 20, 0, 360), pn.color(), pn);
      g.frame(draw::line(center + core::size(0, radius / 4),  center - core::size(0, radius / 2)), pn);
    }

    // --------------------------------------------------------------------------
    sync_icon::sync_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void sync_icon::operator() (graphics& g, const pen& pn) const {
      g.frame(draw::arc(center, radius, 10, 160), pn);
      const auto p0 = calc_clock_point<90>(center, radius);
      const auto l0 = calc_centerline<70>(center, radius * 0.8, radius * 1.2);
      g.draw(draw::polygon({l0.p0(), p0, l0.p1()}), pn.color(), pn);

      g.frame(draw::arc(center, radius, 190, 340), pn);
      const auto p1 = calc_clock_point<270>(center, radius);
      const auto l1 = calc_centerline<250>(center, radius * 0.8, radius * 1.2);
      g.draw(draw::polygon({l1.p0(), p1, l1.p1()}), pn.color(), pn);
    }

    // --------------------------------------------------------------------------
    restart_icon::restart_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void restart_icon::operator() (graphics& g, const pen& pn) const {
      const auto p = calc_clock_point<55>(center, radius);
      const auto l = calc_centerline<35>(center, radius * 0.8, radius * 1.2);
      g.frame(draw::arc(center, radius, 35, 330), pn);
      g.draw(draw::polygon({l.p0(), p, l.p1()}), pn.color(), pn);
    }

    // --------------------------------------------------------------------------
    zoom_in_icon::zoom_in_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void zoom_in_icon::operator() (graphics& g, const pen& pn) const {
      core::point c = center - core::size(radius / 4, radius / 4);
      core::size::type r = radius * 3 / 4;
      draw_find(g, pn, c, r);
      draw_plus(g, pn, c, r * 0.6);
    }

    // --------------------------------------------------------------------------
    zoom_out_icon::zoom_out_icon (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void zoom_out_icon::operator() (graphics& g, const pen& pn) const {
      core::point c = center - core::size(radius / 4, radius / 4);
      core::size::type r = radius * 3 / 4;
      draw_find(g, pn, c, r);
      draw_minus(g, pn, c, r * 0.6);
    }

    // --------------------------------------------------------------------------
  }

}
