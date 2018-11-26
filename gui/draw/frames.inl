/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: color definition
 *
 * @file
 */

#pragma once


namespace gui {

  namespace draw {

    namespace frame {

      inline core::rectangle no_frame (const draw::graphics&, const core::rectangle& r) {
        return r;
      }

      inline core::rectangle relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          return sunken_relief(g, r);
        } else {
          return raised_relief(g, r);
        }
      }

      inline core::rectangle deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          return sunken_deep_relief(g, r);
        } else {
          return raised_deep_relief(g, r);
        }
      }

    } // frame

  } // draw

} // gui
