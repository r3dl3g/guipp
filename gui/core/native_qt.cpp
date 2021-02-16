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

#ifdef GUIPP_QT

// --------------------------------------------------------------------------
//
// Common includes
//
#include <QtGui/QPainter>


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
        ctx.graphics()->setClipRect(r);
      }

      // --------------------------------------------------------------------------
      void clear_clip_rect (core::context& ctx) {
        if (ctx.graphics()->isActive()) {
          ctx.graphics()->setClipping(false);
        }
      }

      // --------------------------------------------------------------------------
      gui::os::graphics create_graphics_context (gui::os::drawable id) {
        if (id) {
          auto g = new QPainter(id);
          if (g->device()->depth() == 1) {
            g->setCompositionMode(QPainter::RasterOp_NotSource);
          }
          return g;
        } else {
          return new QPainter();
        }
      }

      // --------------------------------------------------------------------------
      void delete_graphics_context (gui::os::graphics id) {
        delete id;
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_QT
