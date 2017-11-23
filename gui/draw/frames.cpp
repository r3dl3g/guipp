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
      void black (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(r), color::black);
      }

      void white (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(r), color::white);
      }

      void dots (const draw::graphics& g, const core::rectangle& place) {
//        core::rectangle r = place - core::size::one;
        g.frame(draw::rectangle(place), pen(color::black, 1, pen::Style::dot));
      }

      void lines (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, color::very_light_gray);
      }

      void vline (const draw::graphics& g, const core::rectangle& place) {
        g.frame(line(place.top_right(), place.bottom_right()), color::very_light_gray);
      }

      void hline (const draw::graphics& g, const core::rectangle& place) {
        g.frame(line(place.bottom_left(), place.bottom_right()), color::very_light_gray);
      }

      void vraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - x1y0;
        g.frame(line(r.top_left(), r.bottom_left()), color::white);
        g.frame(line(r.top_right(), r.bottom_right()), color::gray);
      }

      void hraise (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - x0y1;
        g.frame(line(r.top_left(), r.top_right()), color::white);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::gray);
      }

      void vgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - x1y0;
        g.frame(line(r.top_left(), r.bottom_left()), color::gray);
        g.frame(line(r.top_right(), r.bottom_right()), color::white);
      }

      void hgroove (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - x0y1;
        g.frame(line(r.top_left(), r.top_right()), color::gray);
        g.frame(line(r.bottom_right(), r.bottom_left()), color::white);
      }

      void raised_relief (const draw::graphics& g, const core::rectangle& r, os::color high, os::color low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, high);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, low);
//        g.draw_lines({r.bottom_left() + x1y_1, r.bottom_left() - x0y1,
//                      r.top_left() + x0y1, r.top_left() + x1y1, r.top_left() + x1y0,
//                      r.top_right() - x1y0}, high);
//        g.draw_lines({r.bottom_left() + x1y0,
//                      r.bottom_right() - x1y0, r.bottom_right() - x1y1, r.bottom_right() - x0y1,
//                      r.top_right() + x0y1, r.top_right() - x1y_1}, low);
      }

      void sunken_relief (const draw::graphics& g, const core::rectangle& r, os::color high, os::color low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, low);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, high);
//        g.draw_lines({r.bottom_left() - x0y1,
//                      r.top_left() + x0y1, r.top_left() + x1y1, r.top_left() + x1y0,
//                      r.top_right() - x1y0, r.top_right() - x1y_1}, low);
//        g.draw_lines({r.bottom_left() + x1y_1, r.bottom_left() + x1y0,
//                      r.bottom_right() - x1y0, r.bottom_right() - x1y1, r.bottom_right() - x0y1,
//                      r.top_right() + x0y1}, high);
      }

      void raised_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        raised_relief(g, r, color::white, color::grey(80));
      }

      void sunken_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        sunken_relief(g, r, color::white, color::grey(80));
      }

      void raised_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        raised_relief(g, r, color::white, color::grey(80));
        r.shrink(core::size::one);
        raised_relief(g, r, color::grey(95), color::grey(90));
      }

      void sunken_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = place - core::size::one;
        sunken_relief(g, r, color::white, color::grey(80));
        r.shrink(core::size::one);
        sunken_relief(g, r, color::grey(95), color::grey(70));
      }

    } // frame

  }

}
