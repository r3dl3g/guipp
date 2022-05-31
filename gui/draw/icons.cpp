/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     icon drawings
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    std::string icon_names[] = {
      "none",
      "up", "down", "left", "right",
      "up_arrow", "down_arrow", "left_arrow", "right_arrow",
      "remove", "add", "close", "open",
      "check_off", "check_on", "radio_off", "radio_on",
      "undo", "redo", "forward", "backward",
      "okay", "cancel", "cross", "restart",
      "off", "clear", "sync", "reset",
      "play", "pause", "stop", "back",
      "fast_forwad", "fast_back", "first", "last",
      "find", "zoom_in", "zoom_out", "menu",
      "clock", "stopwatch", "timer", "settings", "settings2",
      "info", "important", "person", "group",
      "trash", "file", "folder", "folder_open",
      "list", "grid", "columns",
      "lock", "unlock",
      "north", "east", "south", "west",
      "north_east","north_west", "south_east", "south_west",
      "new_blink", "new_folder", "new_file",
      "background",
      "MAX"
    };

    const std::string& icon_name (icon_type t) {
      return icon_names[static_cast<int>(t)];
    }

    // --------------------------------------------------------------------------
    point::type calc_clock_point_x (point::type center_x, size::type radius, double rad) {
        return static_cast<point::type>(center_x + radius * rad);
    }
    // --------------------------------------------------------------------------
    point::type calc_clock_point_y (point::type center_y, size::type radius, double rad) {
        return static_cast<point::type>(center_y - radius * rad);
    }
    // --------------------------------------------------------------------------
    point calc_clock_point (const point& center, size::type radius, double srad, double crad) {
      return {
        calc_clock_point_x(center.x(), radius, srad),
        calc_clock_point_y(center.y(), radius, crad)
      };
    }
    // --------------------------------------------------------------------------
    point calc_clock_point (const point& center, size::type radius, const core::angle& angle) {
      const auto rad = angle.rad();
      return calc_clock_point(center, radius, sin(rad), cos(rad));
    }
    // --------------------------------------------------------------------------
    point calc_point_between (const point& p0, const point& p1, double f) {
      const auto d = (p1 - p0) * f;
      return p0 + d;
    }
    // --------------------------------------------------------------------------
    draw::line calc_centerline (const point& center, const core::angle& angle, size::type radius) {
      return { center, calc_clock_point(center, radius, angle) };
    }
    // --------------------------------------------------------------------------
    draw::line calc_centerline (const point& center, const core::angle& angle, size::type radius0, size::type radius1) {
      const auto rad = angle.rad();
      const auto ca = cos(rad);
      const auto sa = sin(rad);
      return {
        calc_clock_point(center, radius0, sa, ca), calc_clock_point(center, radius1, sa, ca)
      };
    }

    // --------------------------------------------------------------------------
    constexpr double radian (double degrees) {
      return static_cast<float>(M_PI * degrees / 180.0);
    }
    // --------------------------------------------------------------------------
    template<int I, typename = void>
    struct const_sinus {
//      static constexpr double value = sin(radian(I));
    };
    // --------------------------------------------------------------------------
    template<> struct const_sinus<0> { static constexpr double value = 0.0; };
    template<> struct const_sinus<5> { static constexpr double value = 0.087155743; };
    template<> struct const_sinus<10> { static constexpr double value = 0.173648178; };
    template<> struct const_sinus<15> { static constexpr double value = 0.258819045; };
    template<> struct const_sinus<20> { static constexpr double value = 0.342020143; };
    template<> struct const_sinus<25> { static constexpr double value = 0.422618262; };
    template<> struct const_sinus<30> { static constexpr double value = 0.5; };
    template<> struct const_sinus<35> { static constexpr double value = 0.573576436; };
    template<> struct const_sinus<40> { static constexpr double value = 0.64278761; };
    template<> struct const_sinus<45> { static constexpr double value = 0.707106781; };
    template<> struct const_sinus<50> { static constexpr double value = 0.766044443; };
    template<> struct const_sinus<55> { static constexpr double value = 0.819152044; };
    template<> struct const_sinus<60> { static constexpr double value = 0.866025404; };
    template<> struct const_sinus<65> { static constexpr double value = 0.906307787; };
    template<> struct const_sinus<70> { static constexpr double value = 0.939692621; };
    template<> struct const_sinus<75> { static constexpr double value = 0.965925826; };
    template<> struct const_sinus<80> { static constexpr double value = 0.984807753; };
    template<> struct const_sinus<85> { static constexpr double value = 0.996194698; };
    template<> struct const_sinus<90> { static constexpr double value = 1.0; };
    // --------------------------------------------------------------------------
    template<int I>
    struct const_sinus<I, std::enable_if_t<(I > 90 && I <= 180)>> {
      static constexpr double value = const_sinus<180 - I>::value;
    };
    // --------------------------------------------------------------------------
    template<int I>
    struct const_sinus<I, std::enable_if_t<(I > 180 && I <= 360)>> {
      static constexpr double value = -(const_sinus<I - 180>::value);
    };
    // --------------------------------------------------------------------------
    template<int I>
    struct const_sinus<I, std::enable_if_t<(I < 0)>> {
      static constexpr double value = -const_sinus<-I>::value;
    };
    // --------------------------------------------------------------------------
    template<int I>
    struct const_sinus<I, std::enable_if_t<(I > 360)>> {
      static constexpr double value = const_sinus<I % 360>::value;
    };
    // --------------------------------------------------------------------------
    template<int angle>
    struct const_cosinus {
      static constexpr double value = const_sinus<(angle+90)>::value;
    };
    // --------------------------------------------------------------------------
    template<int angle>
    point calc_clock_point (const point& center, size::type radius) {
      return calc_clock_point(center, radius, const_sinus<angle>::value, const_cosinus<angle>::value);
    }
    // --------------------------------------------------------------------------
    template<int angle>
    draw::line calc_centerline (const point& center, size::type radius) {
      return { center, calc_clock_point<angle>(center, radius) };
    }
    // --------------------------------------------------------------------------
    template<int angle>
    draw::line calc_centerline (const point& center, size::type radius0, size::type radius1) {
      const auto ca = const_cosinus<angle>::value;
      const auto sa = const_sinus<angle>::value;
      return {
        calc_clock_point(center, radius0, sa, ca), calc_clock_point(center, radius1, sa, ca)
      };
    }

    // --------------------------------------------------------------------------
    std::vector<core::point> up_points (const point& center, size::type radius) {
      const auto p0 = calc_clock_point<120>(center, radius);
      const auto p1 = calc_clock_point<0>(center, radius);
      const auto p2 = calc_clock_point<240>(center, radius);
      return {p0, p1, p2};
    }

    std::vector<core::point> down_points (const point& center, size::type radius) {
      const auto p0 = calc_clock_point<60>(center, radius);
      const auto p1 = calc_clock_point<180>(center, radius);
      const auto p2 = calc_clock_point<300>(center, radius);
      return {p0, p1, p2};
    }

    std::vector<core::point> left_points (const point& center, size::type radius) {
      const auto p0 = calc_clock_point<30>(center, radius);
      const auto p1 = calc_clock_point<270>(center, radius);
      const auto p2 = calc_clock_point<150>(center, radius);
      return {p0, p1, p2};
    }

    std::vector<core::point> right_points (const point& center, size::type radius) {
      const auto p0 = calc_clock_point<210>(center, radius);
      const auto p1 = calc_clock_point<90>(center, radius);
      const auto p2 = calc_clock_point<330>(center, radius);
      return {p0, p1, p2};
    }
    // --------------------------------------------------------------------------
    template<int I, int O>
    void draw_arrow_head (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto p = calc_clock_point<I>(center, radius);
      const auto l = calc_centerline<O>(center, radius * 0.8F, radius * 1.2F);
      g.draw(draw::polygon({l.p0(), p, l.p1()}), pn.color(), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::none> (graphics&, const pen&, const core::point&, core::size::type) {
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::up> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.fill(draw::polygon(up_points(center, radius)), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::down> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.fill(draw::polygon(down_points(center, radius)), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::left> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.fill(draw::polygon(left_points(center, radius)), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::right> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.fill(draw::polygon(right_points(center, radius)), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::up_arrow> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(up_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::down_arrow> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(down_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::left_arrow> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(left_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::right_arrow> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(right_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::add> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline({calc_clock_point<0>(center, radius), calc_clock_point<180>(center, radius)}), pn);
      draw_icon<icon_type::remove>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::remove> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline({calc_clock_point<90>(center, radius), calc_clock_point<270>(center, radius)}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::open> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::check_off>(g, pn, center, radius);
      draw_icon<icon_type::add>(g, pn, center, radius * 0.6F);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::close> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::check_off>(g, pn, center, radius);
      draw_icon<icon_type::remove>(g, pn, center, radius * 0.6F);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::check_off> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::round_rectangle({center - core::size{radius, radius},
                                     core::size{radius+radius+1, radius+radius+1}}, radius/2), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::check_on> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::check_off>(g, pn, center, radius);
      const auto r = radius / core::size::type(1.5);
      g.frame(draw::polyline({calc_clock_point<270>(center, r),
                              calc_clock_point<190>(center, r),
                              calc_clock_point<50>(center, radius / 1.3F)}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::radio_off> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 360), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::radio_on> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::radio_off>(g, pn, center, radius);
      g.fill(draw::pie(center, radius / 2, 0, 360), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::undo> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 170), pn);
      draw_arrow_head<-90, -70>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::redo> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 180, 350), pn);
      draw_arrow_head<90, 110>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::forward> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 10, 180), pn);
      draw_arrow_head<90, 70>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::backward> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 190, 360), pn);
      draw_arrow_head<-90, -110>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::okay> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r = radius * 0.7F;
      g.frame(draw::polyline({calc_clock_point<270>(center, r),
                              calc_clock_point<190>(center, r),
                              calc_clock_point<50>(center, radius)}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::cancel> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(draw::line(calc_clock_point<45>(center, radius), calc_clock_point<225>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::cross> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::line(calc_clock_point<45>(center, radius), calc_clock_point<225>(center, radius)), pn);
      g.frame(draw::line(calc_clock_point<-45>(center, radius), calc_clock_point<135>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::restart> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto p = calc_clock_point<55>(center, radius);
      const auto l = calc_centerline<35>(center, radius * 0.8F, radius * 1.2F);
      g.frame(draw::arc(center, radius, 45, 330), pn);
      g.draw(draw::polygon({l.p0(), p, l.p1()}), pn.color(), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::off> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius * 0.9F, 120, 60+360), pn);
      g.frame(draw::line(calc_clock_point<0>(center, radius * 0.3F), calc_clock_point<0>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::clear> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r = radius * 0.7F;
      g.frame(draw::line(calc_clock_point<45>(center, r), calc_clock_point<225>(center, r)), pn);
      g.frame(draw::line(calc_clock_point<-45>(center, r), calc_clock_point<135>(center, r)), pn);
      g.frame(draw::arc(center, radius, 0, 360), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::sync> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 10, 160), pn);
      draw_arrow_head<90, 70>(g, pn, center, radius);

      g.frame(draw::arc(center, radius, 190, 340), pn);
      draw_arrow_head<270, 250>(g, pn, center, radius);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::reset> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, -145, 145), pn);
      draw_arrow_head<-55, -35>(g, pn, center, radius);
      g.fill(draw::pie(center, radius * 0.2F, 0, 360), pn.color());
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::play> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polygon(right_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::pause> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
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
    template<>
    void draw_icon<icon_type::stop> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tll = calc_clock_point<315>(center, radius);
      const auto brr = calc_clock_point<135>(center, radius);
      g.frame(draw::rectangle(core::rectangle{tll, brr}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::back> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polygon(left_points(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::fast_forwad> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r2 = radius * 2 / 3;
      const auto dx = calc_clock_point_x(0, r2, const_sinus<30>::value);
      g.frame(draw::polygon(right_points(center.dx(-r2), r2)), pn);
      g.frame(draw::polygon(right_points(center.dx(r2-dx), r2)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::fast_back> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r2 = radius * 2 / 3;
      const auto dx = calc_clock_point_x(0, r2, const_sinus<30>::value);
      g.frame(draw::polygon(left_points(center.dx(-r2+dx), r2)), pn);
      g.frame(draw::polygon(left_points(center.dx(r2), r2)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::first> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto dx = calc_clock_point_x(0, radius, const_sinus<30>::value);
      const auto dy = calc_clock_point_y(0, radius, const_cosinus<30>::value);
      g.frame(draw::polygon(left_points(center.dx(dx), radius)), pn);
      g.frame(draw::rectangle(core::rectangle{core::point(center.x() - radius, center.y() + dy),
                                              core::point(center.x() - radius + dx, center.y() - dy)}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::last> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto dx = calc_clock_point_x(0, radius, const_sinus<30>::value);
      const auto dy = calc_clock_point_y(0, radius, const_cosinus<30>::value);
      g.frame(draw::polygon(right_points(center.dx(-dx), radius)), pn);
      g.frame(draw::rectangle(core::rectangle{core::point(center.x() + radius - dx, center.y() + dy),
                                              core::point(center.x() + radius, center.y() - dy)}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::find> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      core::point c = center - core::size(radius / 4, radius / 4);
      core::size::type r = radius * 3 / 4;

      g.frame(draw::arc(c, r, 0, 360), pn);
      g.frame(draw::line(calc_clock_point<135>(c, r * 1.1F), calc_clock_point<135>(c, r * 2.0F)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::zoom_in> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::find>(g, pn, center, radius);

      core::point c = center - core::size(radius / 4, radius / 4);
      draw_icon<icon_type::add>(g, pn, c, radius * 0.45F);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::zoom_out> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::find>(g, pn, center, radius);
      core::point c = center - core::size(radius / 4, radius / 4);
      draw_icon<icon_type::remove>(g, pn, c, radius * 0.45F);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::menu> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<315>(center, radius);
      const auto br = calc_clock_point<135>(center, radius);
      const auto mid = (tl.y() + br.y())/2;

      g.frame(draw::line(tl, {br.x(), tl.y()}), pn);
      g.frame(draw::line({tl.x(), mid}, {br.x(), mid}), pn);
      g.frame(draw::line({tl.x(), br.y()}, br), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::clock> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r1 = radius * 0.5F;
      const auto r2 = radius * 0.8F;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline<120>(center, r1), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::stopwatch> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r0 = radius * 1.2F;
      const auto r2 = radius * 0.8F;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
      g.frame(draw::arc(center, r0, 80, 100), pn);
      g.frame(draw::arc(center, r0, 132, 138), pn);
      g.frame(draw::arc(center, r0, 42, 48), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::timer> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto r0 = radius * 1.2F;
      const auto r1 = radius * 0.5F;
      const auto r2 = radius * 0.8F;
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(calc_centerline<90>(center, r1), pn);
      g.frame(calc_centerline<0>(center, r2), pn);
      g.frame(draw::arc(center, r0, 125, 145), pn);
      g.frame(draw::arc(center, r0, 35, 55), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::info> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.draw(draw::pie(center - core::size(0, radius / 2), radius / 20, 0, 360), pn.color(), pn);
      g.frame(draw::line(center - core::size(0, radius / 4),  center + core::size(0, radius / 2)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::important> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.draw(draw::pie(center + core::size(0, radius / 2), radius / 20, 0, 360), pn.color(), pn);
      g.frame(draw::line(center + core::size(0, radius / 4),  center - core::size(0, radius / 2)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::person> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center.dy(-radius / 2), radius / 2, 0, 360), pn);
      g.frame(draw::arc(center.dy(radius), radius, 10, 170), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::group> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const core::point::type r2 = radius / 2;
      const core::point::type r4 = radius / 4;
      for (int i = -1; i < 2; ++i) {
        draw_icon<icon_type::person>(g, pn, center.dxy(r2 * i, r4 * i), r2);
      }
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::trash> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius, 45 + 22.5, 135 - 22.5), pn);

      const auto br = calc_clock_point<135>(center, radius);
      const auto tl = calc_clock_point<315>(center, radius);
      const auto dy = (br.y() - tl.y()) / 5;
      const auto dx = (br.x() - tl.x()) / 5;
      const auto steps = (br.x() - tl.x()) / 4;
      const auto top = tl.y() + dy;
      const auto bottom = br.y();
      const auto bottom2 = center.y() +  radius;
      const auto left = tl.x() + dx;
      const auto right = br.x() - dx;

      for (auto x = tl.x(); x < br.x() + steps/2; x += steps) {
        g.frame(draw::line({x, top}, {x, bottom}), pn);
      }
      g.frame(draw::line(tl, {br.x(), tl.y()}), pn);
      g.frame(draw::line({left, bottom2}, {right, bottom2}), pn);
      g.frame(draw::arc(core::point{left, bottom}, core::size{dx, dy}, 180, 270), pn);
      g.frame(draw::arc(core::point{right, bottom}, core::size{dx, dy}, 270, 360), pn);

    }
    // --------------------------------------------------------------------------
    template<int I>
    void gear_part (std::vector<core::point>& pts, const core::point& center, core::size::type radius) {
      const auto r8 = radius * 0.8F;
      const auto l0 = calc_centerline<I-15>(center, r8, radius);
      const auto l1 = calc_centerline<I+15>(center, r8, radius);
      const auto l2 = calc_centerline<I+45>(center, r8, radius);
      const auto l3 = calc_centerline<I+75>(center, r8, radius);
      pts.insert(pts.end(), {l0.p0(), l0.p1(), l1.p1(), l1.p0(), l2.p0(), l2.p1(), l3.p1(), l3.p0()});
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::settings> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius * 0.4F, 0, 360), pn);
      std::vector<core::point> points;
      points.reserve(24);
      gear_part<0>(points, center, radius);
      gear_part<120>(points, center, radius);
      gear_part<240>(points, center, radius);
      g.frame(draw::polygon(points), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::settings2> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::arc(center, radius * 0.4F, 0, 360), pn);
      std::vector<core::point> points;
      points.reserve(24);
      gear_part<30>(points, center, radius);
      gear_part<150>(points, center, radius);
      gear_part<270>(points, center, radius);
      g.frame(draw::polygon(points), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::file> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<325>(center, radius);
      const auto br = calc_clock_point<145>(center, radius);
      const auto mid = (br.x() - tl.x())/2;
      const core::point bl{tl.x(), br.y()};
      const core::point tm{br.x() - mid, tl.y()};
      const core::point rm{br.x(), tl.y() + mid};
      const core::point mm{br.x() - mid, tl.y() + mid};

      g.frame(draw::polygon({tm, tl, bl, br, rm, tm, mm, rm}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::folder> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<305>(center, radius).dx(-radius/10);
      const auto br = calc_clock_point<125>(center, radius).dx(-radius/10);
      const auto mid = (br.x() - tl.x())/2;
      const auto top = tl.y() - radius / 5;
      g.frame(draw::polyline({tl, {br.x(), tl.y()}, br, {tl.x(), br.y()}, tl, {tl.x() + mid / 4, top}, {tl.x() + mid * 3 / 4, top}, {tl.x() + mid, tl.y()}}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::folder_open> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<305>(center, radius).dx(-radius/10);
      const auto tr = calc_clock_point<80>(center, radius);
      const auto br = calc_clock_point<125>(center, radius).dx(-radius/10);
      const auto mid = (br.x() - tl.x())/2;
      const auto top = tl.y() - radius / 5;
      g.frame(draw::polyline({{tl.x(), br.y()},
                              {tl.x() + (tr.x() - br.x()), tr.y()},
                              tr,
                              br,
                              {tl.x(), br.y()},
                              tl,
                              {tl.x() + mid / 4, top},
                              {tl.x() + mid * 3 / 4, top},
                              {tl.x() + mid, tl.y()},
                              {br.x(), tl.y()},
                              {br.x(), tr.y()}}), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::list> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<315>(center, radius);
      const auto br = calc_clock_point<135>(center, radius);
      g.frame(draw::line(tl, {tl.x(), br.y()}), pn);
      g.frame(draw::line({br.x(), tl.y()}, br), pn);
      const auto dy = (br.y() - tl.y()) / 3;
      auto y = tl.y();
      for (int i = 0; i < 4; ++i) {
        g.frame(draw::line({tl.x(), y}, {br.x(), y}), pn);
        y += dy;
      }
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::grid> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<315>(center, radius);
      const auto br = calc_clock_point<135>(center, radius);
      const auto dx = (br.x() - tl.x()) / 3;
      const auto dy = (br.y() - tl.y()) / 3;
      auto y = tl.y();
      auto x = tl.x();
      for (int i = 0; i < 4; ++i) {
        g.frame(draw::line({x, tl.y()}, {x, br.y()}), pn);
        g.frame(draw::line({tl.x(), y}, {br.x(), y}), pn);
        y += dy;
        x += dx;
      }
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::columns> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto tl = calc_clock_point<315>(center, radius);
      const auto br = calc_clock_point<135>(center, radius);
      g.frame(draw::line(tl, {br.x(), tl.y()}), pn);
      g.frame(draw::line({tl.x(), br.y()}, br), pn);
      const auto dx = (br.x() - tl.x()) / 3;
      auto x = tl.x();
      for (int i = 0; i < 4; ++i) {
        g.frame(draw::line({x, tl.y()}, {x, br.y()}), pn);
        x += dx;
      }
    }
    // --------------------------------------------------------------------------
    void draw_padlock (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto bl = calc_clock_point<225>(center, radius);
      const auto br = calc_clock_point<135>(center, radius);
      const auto top = center.y() - radius / 4;
      g.frame(draw::round_rectangle{{{bl.x(), top}, br}, radius / 5}, pn);
      const auto dy = radius / 5;
      g.frame(draw::arc(center.dy(dy), radius / 20, 0, 360), pn);
      g.frame(draw::line(center.dy(dy), center.dy(dy*2)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::lock> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_padlock(g, pn, center, radius);
      const auto top = center.y() - radius / 4;
      g.frame(draw::arc({center.x(), top}, radius / 2, 0, 180), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::unlock> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_padlock(g, pn, center.dy(radius / 5), radius);
      const auto top = center.y() - radius / 4;
      g.frame(draw::arc({center.x(), top}, radius / 2, 60, 200), pn);
    }
    // --------------------------------------------------------------------------
    template<int I>
    std::vector<core::point> arrow_points (const point& center, size::type radius) {
      const auto p0 = calc_clock_point<I+180>(center, radius);
      const auto p1 = calc_clock_point<I>(center, radius);
      const auto p2 = calc_clock_point<I-45>(center, radius/2);
      const auto p3 = calc_clock_point<I>(center, radius);
      const auto p4 = calc_clock_point<I+45>(center, radius/2);
      return {p0, p1, p2, p3, p4};
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::north> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<0>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::east> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<90>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::south> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<180>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::west> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<-90>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::north_east> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<45>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::north_west> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<-45>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::south_east> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<135>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::south_west> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      g.frame(draw::polyline(arrow_points<225>(center, radius)), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::new_blink> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      const auto top = center.dy(-radius);
      const auto btm = center.dy(radius);
      const auto tl = calc_clock_point<300>(center, radius);
      const auto br = calc_clock_point<120>(center, radius);
      const auto tr = calc_clock_point<60>(center, radius);
      const auto bl = calc_clock_point<240>(center, radius);

      g.frame(draw::line(tl, br), pn);
      g.frame(draw::line(tr, bl), pn);
      g.frame(draw::line(top, btm), pn);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::new_folder> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::folder>(g, pn, center, radius);
      const auto tr = calc_clock_point<45>(center, radius);
      draw_icon<icon_type::new_blink>(g, pn, tr, radius / 3.5F);
    }
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::new_file> (graphics& g, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon<icon_type::file>(g, pn, center, radius);
      const auto tr = calc_clock_point<45>(center, radius);
      draw_icon<icon_type::new_blink>(g, pn, tr, radius / 3.5F);
    }
    // --------------------------------------------------------------------------
    // --------------------------------------------------------------------------
    template<>
    void draw_icon<icon_type::background> (graphics& g, const pen&, const core::point& center, core::size::type radius) {
      pen pn{color::gray, 0.333F};
      core::size sz{radius, radius};
//      core::size sz2{radius/2, radius/2};
      g.frame(draw::arc(center, radius, 0, 360), pn);
      g.frame(draw::arc(center, radius/2, 0, 360), pn);
//      g.frame(draw::ellipse(center - sz, center + sz), pn);
//      g.frame(draw::ellipse(center - sz2, center + sz2), pn);

      const auto tl = center.dxy(-radius, -radius);
      const auto tr = center.dxy(radius, -radius);
      const auto bl = center.dxy(-radius, radius);
      const auto br = center.dxy(radius, radius);
      g.frame(draw::line(tl, br), pn);
      g.frame(draw::line(bl, tr), pn);
//      g.frame(draw::line(tl, tr), pn);
//      g.frame(draw::line(tr, br), pn);
//      g.frame(draw::line(bl, br), pn);
//      g.frame(draw::line(tl, bl), pn);
      g.frame(draw::rectangle(center - sz, center + sz), pn);
    }
    // --------------------------------------------------------------------------
    template<icon_type I>
    void draw_icon_switch (graphics& g, icon_type type, const pen& pn, const core::point& center, core::size::type radius) {
      if (type == I) {
        draw_icon<I>(g, pn, center, radius);
      } else {
        draw_icon_switch<icon_type(static_cast<int>(I)+1)>(g, type, pn, center, radius);
      }
    }

    template<>
    void draw_icon_switch<icon_type::MAX> (graphics&, icon_type, const pen&, const core::point&, core::size::type)
    {}

    void draw_icon (graphics& g, icon_type type, const pen& pn, const core::point& center, core::size::type radius) {
      draw_icon_switch<icon_type::none>(g, type, pn, center, radius);
    }
    // --------------------------------------------------------------------------

  }

}
