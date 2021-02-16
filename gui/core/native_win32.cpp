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
 * @brief     C++ API: basic window types
 *
 * @file
 */

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/native.h>
#include <gui/core/context.h>


namespace gui {

  namespace core {

    namespace native {

      // --------------------------------------------------------------------------
      void set_clip_rect (core::context& ctx, const gui::os::rectangle& r) {
        SelectClipRgn(ctx, NULL);
        IntersectClipRect(ctx, r.left, r.top, r.right, r.bottom);
      }

      // --------------------------------------------------------------------------
      void clear_clip_rect (core::context& ctx) {
        SelectClipRgn(ctx, NULL);
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        auto dc = GetDC(NULL);
        auto ndc = CreateCompatibleDC(dc);
        SelectObject(ndc, id);
        ReleaseDC(NULL, dc);
        return ndc;
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
        DeleteDC(id);
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_WIN
