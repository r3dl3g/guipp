/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: brush definition
 *
 * @file
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
#include <gui/draw/drawers.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    struct GUIPP_DRAW_EXPORT arc_coords {
      const os::point_type x;
      const os::point_type y;
      const os::size_type w;
      const os::size_type h;
      const core::angle start;
      const core::angle end;

      arc_coords (const core::context& ctx,
                  const core::rectangle& rect,
                  const core::angle& start_angle,
                  const core::angle& end_angle);

      std::array<os::point, 2> calc_points0 () const;
      std::array<os::point, 3> calc_points () const;

      os::point center (const os::size& radius) const;
      os::size radius () const;

      bool full () const;
      bool empty () const;

      static os::point calc_arc_point (const os::point& center,
                                       const os::size& radius,
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
