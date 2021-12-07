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
 * @brief     OS native core functions
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/core/gui++-core-export.h>


namespace gui {

  namespace core {

    struct context;

    namespace native {

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT void set_clip_rect (context&, const gui::os::rectangle&);

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT void clear_clip_rect (context&);

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT gui::os::graphics create_graphics_context (gui::os::drawable);

      // --------------------------------------------------------------------------
      GUIPP_CORE_EXPORT void delete_graphics_context (gui::os::graphics);

      // --------------------------------------------------------------------------
#ifdef GUIPP_USE_XFT
      namespace x11 {
        GUIPP_CORE_EXPORT XftDraw* get_xft_draw (context&);
      } // namespace x11
#endif // GUIPP_USE_XFT

      // --------------------------------------------------------------------------
    } // namespace native

  } // namespace core

} // namespace gui

