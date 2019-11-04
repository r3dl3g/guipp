/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: layout
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window.h>
#include <gui/layout/layout.h>


namespace gui {

  namespace layout {

    layout_function lay (win::window& w) {
      return [&w] (const core::rectangle& r) {
        w.place(r);
      };
    }

    layout_function lay (win::window* w) {
      return [w] (const core::rectangle& r) {
        w->place(r);
      };
    }

  } // layout

} // gui
