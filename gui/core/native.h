/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic structures
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include <gui++-core-export.h>


namespace gui {

  namespace core {

    struct context;

    namespace native {

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT void set_clip_rect (context&, const gui::os::rectangle&);

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT void clear_clip_rect (context&);

#ifdef GUIPP_USE_XFT
      namespace x11 {
        GUIPP_CORE_EXPORT XftDraw* get_xft_draw (context&);
      } // namespace x11
#endif // GUIPP_USE_XFT

      // --------------------------------------------------------------------------
    } // namespace native

  } // namespace core

} // namespace gui

