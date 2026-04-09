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
 * @brief     arc coordinates calculator
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//

#include "gui/core/rectangle.h"
#include "gui/draw/arc_coords.h"


namespace gui {

  namespace draw {

    arc_coords::arc_coords (const core::point& c,
                            const core::size& r,
                            const core::angle& start_angle,
                            const core::angle& end_angle)
      : center(c)
      , radius(r)
      , start(start_angle)
      , end(end_angle)
    {}

    core::point arc_coords::calc_arc_point (const core::point& pt,
                                            const core::size& sz,
                                            double angle) {
      return core::point{static_cast<core::point::type>(pt.x() + sz.width() * cos(angle)),
                         static_cast<core::point::type>(pt.y() - sz.height() * sin(angle))};
    }

    std::array<core::point, 2> arc_coords::calc_points0 () const {
      return {
        center,
        calc_arc_point(center, radius, start.rad())
      };
    }

    std::array<os::point, 2> arc_coords::calc_os_points0 (const core::context& ctx) const {
      return {
        center.os(ctx),
        calc_arc_point(center, radius, start.rad()).os(ctx)
      };
    }

    std::array<core::point, 3> arc_coords::calc_points () const {
      return {
        calc_arc_point(center, radius, start.rad()),
        center,
        calc_arc_point(center, radius, end.rad())
      };
    }

    std::array<os::point, 3> arc_coords::calc_os_points (const core::context& ctx) const {
      return {
        calc_arc_point(center, radius, start.rad()).os(ctx),
        center.os(ctx),
        calc_arc_point(center, radius, end.rad()).os(ctx)
      };
    }

    int arc_coords::count_of_arc_segments () const {
        return static_cast<int>(radius.width() + radius.height()) / 2.0 * (end.deg() - start.deg()) / 360;
    }

    std::vector<core::point> arc_coords::calc_arc_points (int count) const {
      std::vector<core::point> pts(count);
      const auto step = (end.rad() - start.rad()) / count;
      for (int i = 0; i < count; ++i) {
        pts[i] = calc_arc_point(center, radius, start.rad() + step * i);
      }
      return pts;
    }

    std::vector<core::point> arc_coords::calc_arc_points () const {
      return calc_arc_points(count_of_arc_segments());
    }

    std::vector<os::point> arc_coords::calc_arc_os_points (int count, const core::context& ctx) const {
      std::vector<os::point> pts(count);
      const auto step = (end.rad() - start.rad()) / count;
      for (int i = 0; i < count; ++i) {
        pts[i] = calc_arc_point(center, radius, start.rad() + step * i).os(ctx);
      }
      return pts;
    }

    std::vector<os::point> arc_coords::calc_arc_os_points (const core::context& ctx) const {
      return calc_arc_os_points(count_of_arc_segments(), ctx);
    }

    core::rectangle arc_coords::get_area () const {
      return {center - radius, radius * 2.0};
    }

    bool arc_coords::full () const {
      return end.deg() - start.deg() >= 360.0F;
    }

    bool arc_coords::empty () const {
      return start.deg() == end.deg();
    }

  }

}
