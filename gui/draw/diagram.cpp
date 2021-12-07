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
 * @brief     diagram drawing
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <math.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/diagram.h>


namespace gui {

  namespace draw {

    namespace diagram {

      // --------------------------------------------------------------------------
      circle::circle (float radius)
        : radius(radius)
      {}

      void circle::operator() (graphics& g, const brush& b, const core::point& pt) const {
        g.fill(draw::pie(pt, radius, 0, 360), b);
      }

      // --------------------------------------------------------------------------
      diamond::diamond (float radius)
        : radius(radius)
      {}

      void diamond::operator() (graphics& g, const brush& b, const core::point& pt) const {
        g.fill(draw::polygon({ pt.dx(-radius), pt.dy(-radius), pt.dx(radius), pt.dy(radius) }), b);
      }

      // --------------------------------------------------------------------------
      cross::cross (float radius)
        : radius(radius)
      {}

      void cross::operator() (graphics& g, const brush& b, const core::point& pt) const {
        g.frame(draw::line(pt.dxy(-radius, -radius), pt.dxy(radius, radius)), b.color());
        g.frame(draw::line(pt.dxy(-radius, radius), pt.dxy(radius, -radius)), b.color());
      }

      // --------------------------------------------------------------------------
      square::square (float radius)
        : radius(radius)
      {}

      void square::operator() (graphics& g, const brush& b, const core::point& pt) const {
        g.fill(draw::rectangle(pt.dxy(-radius, -radius), pt.dxy(radius, radius)), b);
      }

    } // namespace diagram

    // --------------------------------------------------------------------------
  } // namespace draw

} // namespace gui
