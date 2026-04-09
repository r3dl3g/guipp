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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/draw/drawers.h"
#include "gui/draw/gui++-draw-export.h"


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT arc_coords {
      const core::point center;
      const core::size radius;
      const core::angle start;
      const core::angle end;

      arc_coords (const core::point& center,
                  const core::size& radius,
                  const core::angle& start_angle,
                  const core::angle& end_angle);

      std::array<core::point, 2> calc_points0 () const;
      std::array<core::point, 3> calc_points () const;

      std::array<os::point, 2> calc_os_points0 (const core::context& ctx) const;
      std::array<os::point, 3> calc_os_points (const core::context& ctx) const;

      core::rectangle get_area () const;

      bool full () const;
      bool empty () const;

      int count_of_arc_segments () const;

      std::vector<core::point> calc_arc_points (int count) const;
      std::vector<os::point> calc_arc_os_points (int count, const core::context& ctx) const;

      std::vector<core::point> calc_arc_points () const;
      std::vector<os::point> calc_arc_os_points (const core::context& ctx) const;

      static core::point calc_arc_point (const core::point& center,
                                         const core::size& radius,
                                         double angle);
    };

    // --------------------------------------------------------------------------
    template<arc_type T>
    void draw_arc (graphics&, const arc_coords&, const pen&);

    template<arc_type T>
    void fill_arc (graphics&, const arc_coords&, const brush&);

    // --------------------------------------------------------------------------
    template<>
    GUIPP_DRAW_EXPORT void draw_arc<arc_type::arc> (graphics& g, const arc_coords& c, const pen& p);

    template<>
    GUIPP_DRAW_EXPORT void draw_arc<arc_type::pie> (graphics& g, const arc_coords& c, const pen& p);

    template<>
    GUIPP_DRAW_EXPORT void fill_arc<arc_type::arc> (graphics& g, const arc_coords& c, const brush& b);

    template<>
    GUIPP_DRAW_EXPORT void fill_arc<arc_type::pie> (graphics& g, const arc_coords& c, const brush& b);

    // --------------------------------------------------------------------------
  } //namespace draw

} //namespace gui
