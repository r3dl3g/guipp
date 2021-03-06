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

#include <gui/core/rectangle.h>
#include <gui/draw/arc_coords.h>


namespace gui {

  namespace draw {

    arc_coords::arc_coords (const core::context& ctx,
                            const core::rectangle& rect,
                            const core::angle& start_angle,
                            const core::angle& end_angle)
      : x(rect.os_x(ctx))
      , y(rect.os_y(ctx))
      , w(rect.os_width())
      , h(rect.os_height())
      , start(start_angle)
      , end(end_angle)
    {}

    inline os::point calc_arc_point (const os::point& pt,
                                     const os::size& sz,
                                     double a) {
      const auto ca = cos(a);
      const auto sa = sin(a);
      return os::point{(short)round(os::get_x(pt) + os::get_width(sz) * ca),
                       (short)round(os::get_y(pt) - os::get_height(sz) * sa)};
    }

    os::size arc_coords::radius () const {
      return {static_cast<os::size_type>(w / 2.0),
              static_cast<os::size_type>(h / 2.0)};
    }

    os::point arc_coords::center (const os::size& radius) const {
      return {static_cast<os::point_type>(x + os::get_width(radius)),
              static_cast<os::point_type>(y + os::get_height(radius))};
    }

    std::array<os::point, 2> arc_coords::calc_points0 () const {
      const auto r = radius();
      const auto c = center(r);
      return {
        c,
        calc_arc_point(c, r, start.rad())
      };
    }

    std::array<os::point, 3> arc_coords::calc_points () const {
      const auto r = radius();
      const auto c = center(r);
      return {
        calc_arc_point(c, r, start.rad()),
        c,
        calc_arc_point(c, r, end.rad())
      };
    }

    bool arc_coords::full () const {
      return start.deg() + 360.0F == end.deg();
    }

    bool arc_coords::empty () const {
      return start.deg() == end.deg();
    }

  }

}
