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

#include <gui/draw/clock.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <util/string_util.h>


namespace gui {

  namespace draw {

    using namespace gui::core;

    // --------------------------------------------------------------------------
    centerline::centerline (const core::point& center,
                            core::size::type radius,
                            const core::angle& angle)
      : icon_base(center, radius)
      , angle(angle)
    {}

    void centerline::operator() (graphics& g, const pen& pn) const {
      g.frame(calc_centerline(center, angle, radius), pn);
    }

    // --------------------------------------------------------------------------
    centerline_section::centerline_section (const core::point& center,
                                            core::size::type radius,
                                            core::size::type radius2,
                                            const core::angle& angle)
      : centerline(center, radius, angle)
      , radius2(radius2)
    {}

    void centerline_section::operator() (graphics& g, const pen& pn) const {
      g.frame(calc_centerline(center, angle, radius, radius2), pn);
    }

    // --------------------------------------------------------------------------
    centerlines::centerlines (const core::point& center,
                              core::size::type radius,
                              core::size::type radius2,
                              const core::angle& step,
                              const core::angle& start,
                              const core::angle& end)
      : center(center)
      , radius(radius)
      , radius2(radius2)
      , step(step)
      , start(start)
      , end(end)
    {}

    void centerlines::operator() (graphics& g, const pen& pn) const {
      for (angle i(start); i < end; i += step) {
        g.frame(calc_centerline(center, i, radius, radius2), pn);
      }
    }

    // --------------------------------------------------------------------------
    clock_minutes::clock_minutes (const core::point& center, core::size::type radius)
      : centerlines(center, radius * 0.95F, radius, 360 / 60)
    {}

    // --------------------------------------------------------------------------
    clock_hours::clock_hours (const core::point& center, core::size::type radius)
      : centerlines(center, radius * 0.95F, radius, 360 / 12)
    {}

    // --------------------------------------------------------------------------
    clock_numbers::clock_numbers (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void clock_numbers::operator() (graphics& g, const font& font, gui::os::color color) const {
      for (int i = 1; i < 13; ++i) {
        g.text(draw::text(util::string::convert::from(i),
                          calc_clock_point(center, radius, i * 360.0F / 12.0F),
                          text_origin_t::center),
               font, color);
      }
    }
    // --------------------------------------------------------------------------
    clock_face::clock_face (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void clock_face::operator() (graphics& g, const font& font, gui::os::color col) const {
      const auto rt = radius * 0.8F;

      g.frame(clock_minutes(center, radius), draw::pen(color::darker(col, 0.5), 3));
      g.frame(clock_hours(center, radius), draw::pen(col, 5));

      draw::font fnt = font.with_size(radius / 9.0F);
      for (int i = 1; i < 13; ++i) {
        g.text(draw::text(util::string::convert::from(i),
                          calc_clock_point(center, rt, i * 360.0F / 12.0F),
                          text_origin_t::center),
               fnt, col);
      }

    }

    // --------------------------------------------------------------------------
    clock_hands::clock_hands (const core::point& center, core::size::type radius,
                              int hour, int min, int sec,
                              gui::os::color second_color)
      : icon_base(center, radius)
      , second_color(second_color)
      , hour(hour)
      , min(min)
      , sec(sec)
    {}

    void clock_hands::operator() (graphics& g, const pen& p) const {
      const auto w = radius * 0.03F;

      draw::pen pn(p.color(), w, draw::pen::Style::solid, draw::pen::Cap::round);

      const auto hangle = ((hour % 12) + min / 60.0F) * 360 / 12;
      g.frame(calc_centerline(center, hangle, radius * 0.1F, radius * 0.6F), pn);
      g.frame(calc_centerline(center, hangle, radius * 0.1F), pn.with_size(w / 3.0F));
      const auto mangle = min * 360 / 60;
      g.frame(calc_centerline(center, mangle, radius * 0.1F, radius * 0.9F), pn);
      g.frame(calc_centerline(center, mangle, radius * 0.1F), pn.with_size(w / 3.0F));
      draw::pen pn2(second_color, w / 2.0F, draw::pen::Style::solid, draw::pen::Cap::round);
      g.frame(calc_centerline(center, sec * 360.0F / 60.0F, radius), pn2);

      g.fill(draw::pie(center, w, 0, 360), p.color());
    }

    // --------------------------------------------------------------------------
    clock_simple_hands::clock_simple_hands (const core::point& center,
                                            core::size::type radius,
                                            int hour, int min)
      : icon_base(center, radius)
      , hour(hour)
      , min(min)
    {}

    void clock_simple_hands::operator() (graphics& g, const pen& p) const {
      const auto w = radius * 0.03F;

      draw::pen pn(p.color(), w, draw::pen::Style::solid, draw::pen::Cap::round);

      const auto hangle = ((hour % 12) + min / 60.0F) * 360.0F / 12.0F;
      g.frame(calc_centerline(center, hangle, radius * 0.6F), pn);
      const auto mangle = min * 360.0F / 60.0F;
      g.frame(calc_centerline(center, mangle, radius), pn);

      g.fill(draw::pie(center, w, 0, 360), p.color());
    }

    // --------------------------------------------------------------------------
    stopwatch_seconds_face::stopwatch_seconds_face (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void stopwatch_seconds_face::operator() (graphics& g, const font& font, gui::os::color col) const {
      const auto r0 = radius * 0.975F;
      const auto r1 = radius * 0.95F;
      const auto rt = radius * 0.825F;
      g.frame(draw::centerlines(center, r0, radius, 360.0F / 60.0F / 5.0F), draw::pen(color::darker(col, 0.5F), 2));
      g.frame(draw::centerlines(center, r1, radius, 360.0F / 60.0F), draw::pen(color::darker(col, 0.5F), 2));
      g.frame(draw::centerlines(center, r1, radius, 360.0F / 12.0F), draw::pen(col, 3));
      draw::font fnt = font.with_size(radius / 9.0F);
      for (int i = 5; i < 61; i += 5) {
        g.text(draw::text(util::string::convert::from(i),
                          draw::calc_clock_point(center, rt, i * 360.0F / 60.0F),
                          text_origin_t::center),
               fnt, col);
      }
    }

    // --------------------------------------------------------------------------
    stopwatch_minutes_face::stopwatch_minutes_face (const core::point& center, core::size::type radius)
      : icon_base(center, radius)
    {}

    void stopwatch_minutes_face::operator() (graphics& g, const font& font, gui::os::color col) const {

      const auto rt = radius * 0.5F;
      const auto r0 = radius * 0.9F;
      const auto r1 = radius * 0.8F;

      g.frame(draw::centerlines(center, r0, radius, 360.0F / 30.0F / 2.0F), draw::pen(color::darker(col, 0.5F), 1));
      g.frame(draw::centerlines(center, r1, radius, 360.0F / 30.0F), draw::pen(color::darker(col, 0.5F), 1));
      g.frame(draw::centerlines(center, r1, radius, 360.0F / 6.0F), draw::pen(col, 3));

      draw::font fnt = font.with_size(radius / 5.0F);
      for (int i = 5; i < 31; i += 5) {
        g.text(draw::text(util::string::convert::from(i),
                          draw::calc_clock_point(center, rt, i * 360.0F / 30.0F),
                          text_origin_t::center),
               fnt, col);
      }
    }

    // --------------------------------------------------------------------------
    stopwatch_digits::stopwatch_digits (const core::point& center, core::size::type radius,
                                        std::chrono::milliseconds duration)
      : icon_base(center, radius)
      , duration(duration)
    {}

    void stopwatch_digits::operator() (graphics& g, const font& font, gui::os::color col) const {

      const auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration);
      const auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);

      std::ostringstream out;
      out << std::setfill('0') << std::setw(2) << mins.count() << ":";
      out << std::setfill('0') << std::setw(2) << (secs.count() % 60) << ",";
      out << std::setfill('0') << std::setw(2) << (duration.count() % 1000) / 10;

      draw::font fnt = font.with_size(radius / 10.0F);
      g.text(draw::text(out.str(), center, text_origin_t::center), fnt, col);
    }
    // --------------------------------------------------------------------------
    stopwatch_seconds_hands::stopwatch_seconds_hands (const core::point& center,
                                                      core::size::type radius,
                                                      std::chrono::milliseconds duration)
      : icon_base(center, radius)
      , duration(duration)
    {}

    void stopwatch_seconds_hands::operator() (graphics& g, const pen& pn) const {
      const auto secs = duration.count() / 1000.0F;
      g.frame(draw::calc_centerline(center, secs * 360.0F / 60.0F, -radius / 5.0F, radius), pn);
      g.draw(draw::pie(center, radius * 0.02F, 0, 360), color::black, pn);
    }

    // --------------------------------------------------------------------------
    stopwatch_minutes_hands::stopwatch_minutes_hands (const core::point& center,
                                                      core::size::type radius,
                                                      std::chrono::milliseconds duration)
      : icon_base(center, radius)
      , duration(duration)
    {}

    void stopwatch_minutes_hands::operator() (graphics& g, const pen& pn) const {

      const auto minutes = duration.count() / 60000.0F;
      g.frame(draw::calc_centerline(center, minutes * 360.0F / 30.0F, radius), pn);
      g.draw(draw::pie(center, radius * 0.05F, 0, 360), color::black, pn);
    }

    // --------------------------------------------------------------------------
  }

}
