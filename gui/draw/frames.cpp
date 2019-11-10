/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/frames.h>


namespace gui {

  namespace draw {

    namespace frame {

      const core::size x0y1 = {0, 1};
      const core::size x1y0 = {1, 0};
      const core::size x1y1 = {1, 1};
      const core::size x1y_1 = {1, -1};

      // --------------------------------------------------------------------------
      core::rectangle black (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), color::black);
        return place.shrinked(core::size::one);
      }

      core::rectangle white (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), color::white);
        return place.shrinked(core::size::one);
      }

      core::rectangle dots (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), pen(color::black, 1, pen::Style::dot));
        return place.shrinked(core::size::one);
      }

      core::rectangle lines (const draw::graphics& g, const core::rectangle& r) {
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, color::very_light_gray);
        return r - core::size::one;
      }

      core::rectangle vline (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_right(), r.bottom_right()), color::very_light_gray);
        return r - x1y0;
      }

      core::rectangle hline (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.bottom_left(), r.bottom_right()), color::very_light_gray);
        return r - x0y1;
      }

      core::rectangle vraise (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), color::white);
        g.frame(line(r.top_right(), r.bottom_right()), color::gray);
        return r.shrinked(x1y0);
      }

      core::rectangle hraise (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), color::white);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::gray);
        return r.shrinked(x0y1);
      }

      core::rectangle vgroove (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), color::gray);
        g.frame(line(r.top_right(), r.bottom_right()), color::white);
        return r.shrinked(x1y0);
      }

      core::rectangle hgroove (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), color::gray);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::white);
        return r.shrinked(x0y1);
      }

      core::rectangle raised_relief (const draw::graphics& g, const core::rectangle& r, os::color high, os::color low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, high);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, low);
        return r.shrinked(core::size::one);
      }

      core::rectangle sunken_relief (const draw::graphics& g, const core::rectangle& r, os::color high, os::color low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, low);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, high);
        return r.shrinked(core::size::one);
      }

      core::rectangle raised_relief (const draw::graphics& g, const core::rectangle& place) {
        return raised_relief(g, place, color::white, color::gray);
      }

      core::rectangle sunken_relief (const draw::graphics& g, const core::rectangle& place) {
        return sunken_relief(g, place, color::white, color::gray);
      }

      core::rectangle raised_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = raised_relief(g, place, color::white, color::gray);
        return raised_relief(g, r, color::medium_gray, color::dark_gray);
      }

      core::rectangle sunken_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = sunken_relief(g, place, color::white, color::gray);
        return sunken_relief(g, r, color::medium_gray, color::dark_gray);
      }

    } // frame

  }

}
