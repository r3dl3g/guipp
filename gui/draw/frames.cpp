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
#include <gui/draw/pen.h>


namespace gui {

  namespace draw {

    namespace frame {

      const core::size x0y1 = {0, 1};
      const core::size x1y0 = {1, 0};
      const core::size x1y1 = {1, 1};
      const core::size x1y_1 = {1, -1};

      static pen line_pen(color::very_light_gray, 1, pen::Style::solid, pen::Cap::square, pen::Join::miter);
      static pen black_pen(color::black);
      static pen black_dot_pen(color::black, 1, pen::Style::dot, pen::Cap::flat, pen::Join::bevel);
      static pen white_pen = pen(color::white);
      static pen medium_gray_pen = pen(color::medium_gray);
      static pen very_very_light_gray_pen = pen(color::very_very_light_gray);
      static pen light_gray_pen = pen(color::light_gray);

      // --------------------------------------------------------------------------
      core::rectangle black (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), black_pen);
        return place.shrinked(core::size::one);
      }

      core::rectangle white (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), white_pen);
        return place.shrinked(core::size::one);
      }

      core::rectangle dots (const draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), black_dot_pen);
        return place.shrinked(core::size::one);
      }

      core::rectangle lines (const draw::graphics& g, const core::rectangle& r) {
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, line_pen);
        return r - core::size::one;
      }

      core::rectangle vline (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_right(), r.bottom_right()), line_pen);
        return r - x1y0;
      }

      core::rectangle hline (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.bottom_left(), r.bottom_right()), line_pen);
        return r - x0y1;
      }

      core::rectangle vraise (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), white_pen);
        g.frame(line(r.top_right(), r.bottom_right()), medium_gray_pen);
        return r.shrinked(x1y0);
      }

      core::rectangle hraise (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), white_pen);
        g.frame(line(r.bottom_right(), r.bottom_left()), medium_gray_pen);
        return r.shrinked(x0y1);
      }

      core::rectangle vgroove (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), medium_gray_pen);
        g.frame(line(r.top_right(), r.bottom_right()), white_pen);
        return r.shrinked(x1y0);
      }

      core::rectangle hgroove (const draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), medium_gray_pen);
        g.frame(line(r.bottom_right(), r.bottom_left()), white_pen);
        return r.shrinked(x0y1);
      }

      core::rectangle raised_relief (const draw::graphics& g, const core::rectangle& r, const pen& high, const pen& low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, high);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, low);
        return r.shrinked(core::size::one);
      }

      core::rectangle sunken_relief (const draw::graphics& g, const core::rectangle& r, const pen& high, const pen& low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, low);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, high);
        return r.shrinked(core::size::one);
      }

      core::rectangle raised_relief (const draw::graphics& g, const core::rectangle& place) {
        return raised_relief(g, place, white_pen, medium_gray_pen);
      }

      core::rectangle sunken_relief (const draw::graphics& g, const core::rectangle& place) {
        return sunken_relief(g, place, white_pen, medium_gray_pen);
      }

      core::rectangle raised_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = raised_relief(g, place, white_pen, medium_gray_pen);
        return raised_relief(g, r, very_very_light_gray_pen, light_gray_pen);
      }

      core::rectangle sunken_deep_relief (const draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = sunken_relief(g, place, white_pen, medium_gray_pen);
        return sunken_relief(g, r, very_very_light_gray_pen, light_gray_pen);
      }

    } // frame

  }

}
