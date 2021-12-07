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
 * @brief     frames drawer
 *
 * @license   MIT license. See accompanying file LICENSE.
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

      struct static_pen : public pen {
        typedef pen super;

        static_pen (const os::color& color,
                     pen::size_type width = 1,
                     pen::Style style = Style::solid,
                     pen::Cap cap = Cap::square,
                     pen::Join join = Join::miter)
          : super(color, width, style, cap, join)
#ifdef GUIPP_WIN
          , scale_factor(core::global::get_scale_factor())
          , width(width)
#endif // GUIPP_WIN
        {}

        const pen& operator() () {
#ifdef GUIPP_WIN
          if (scale_factor != core::global::get_scale_factor()) {
            scale_factor = core::global::get_scale_factor();
            super::recreate(width);
          }
#endif // GUIPP_WIN
          return *this;
        }

#ifdef GUIPP_WIN
      private:
        double scale_factor;
        pen::size_type width;
#endif // GUIPP_WIN
      };


      static static_pen line_pen(gui::color::very_light_gray, 1, pen::Style::solid, pen::Cap::square, pen::Join::miter);
      static static_pen black_pen(color::black);
      static static_pen black_dot_pen(color::black, 1, pen::Style::dot, pen::Cap::flat, pen::Join::bevel);
      static static_pen white_pen(color::white);
      static static_pen medium_gray_pen(color::medium_gray);
      static static_pen very_very_light_gray_pen(color::very_very_light_gray);
      static static_pen light_gray_pen(color::light_gray);

      // --------------------------------------------------------------------------
      core::rectangle black (draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), black_pen());
        return place.shrinked(core::size::one);
      }

      core::rectangle white (draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), white_pen());
        return place.shrinked(core::size::one);
      }

      core::rectangle dots (draw::graphics& g, const core::rectangle& place) {
        g.frame(draw::rectangle(place), black_dot_pen());
        return place.shrinked(core::size::one);
      }

      core::rectangle lines (draw::graphics& g, const core::rectangle& r) {
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, line_pen());
        return r - core::size::one;
      }

      core::rectangle vline (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_right(), r.bottom_right()), line_pen());
        return r - x1y0;
      }

      core::rectangle hline (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.bottom_left(), r.bottom_right()), line_pen());
        return r - x0y1;
      }

      core::rectangle vraise (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), white_pen());
        g.frame(line(r.top_right(), r.bottom_right()), medium_gray_pen());
        return r.shrinked(x1y0);
      }

      core::rectangle hraise (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), white_pen());
        g.frame(line(r.bottom_right(), r.bottom_left()), medium_gray_pen());
        return r.shrinked(x0y1);
      }

      core::rectangle vgroove (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.bottom_left()), medium_gray_pen());
        g.frame(line(r.top_right(), r.bottom_right()), white_pen());
        return r.shrinked(x1y0);
      }

      core::rectangle hgroove (draw::graphics& g, const core::rectangle& r) {
        g.frame(line(r.top_left(), r.top_right()), medium_gray_pen());
        g.frame(line(r.bottom_right(), r.bottom_left()), white_pen());
        return r.shrinked(x0y1);
      }

      core::rectangle raised_relief (draw::graphics& g, const core::rectangle& r, const pen& high, const pen& low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, high);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, low);
        return r.shrinked(core::size::one);
      }

      core::rectangle sunken_relief (draw::graphics& g, const core::rectangle& r, const pen& high, const pen& low) {
        g.draw_lines({r.bottom_left(), r.top_left(), r.top_right()}, low);
        g.draw_lines({r.bottom_left(), r.bottom_right(), r.top_right()}, high);
        return r.shrinked(core::size::one);
      }

      core::rectangle raised_relief (draw::graphics& g, const core::rectangle& place) {
        return raised_relief(g, place, white_pen(), medium_gray_pen());
      }

      core::rectangle sunken_relief (draw::graphics& g, const core::rectangle& place) {
        return sunken_relief(g, place, white_pen(), medium_gray_pen());
      }

      core::rectangle raised_deep_relief (draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = raised_relief(g, place, white_pen(), medium_gray_pen());
        return raised_relief(g, r, very_very_light_gray_pen(), light_gray_pen());
      }

      core::rectangle sunken_deep_relief (draw::graphics& g, const core::rectangle& place) {
        core::rectangle r = sunken_relief(g, place, white_pen(), medium_gray_pen());
        return sunken_relief(g, r, very_very_light_gray_pen(), light_gray_pen());
      }

    } // frame

  }

}
