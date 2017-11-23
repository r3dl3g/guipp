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

      inline void no_frame (const draw::graphics&, const core::rectangle&)
      {}

      inline void relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          sunken_relief(g, r);
        } else {
          raised_relief(g, r);
        }
      }

      inline void deep_relief (const draw::graphics& g, const core::rectangle& r, bool sunken) {
        if (sunken) {
          sunken_deep_relief(g, r);
        } else {
          raised_deep_relief(g, r);
        }
      }

    } // frame

  } // draw

} // gui
