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
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#include <QtWidgets/QDesktopWidget>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/native.h>


namespace gui {

  namespace core {

    namespace native {

      // --------------------------------------------------------------------------
      void set_clip_rect (core::context& ctx, const core::rectangle& r) {
        ctx.g->setClipRect(r.os());
      }

      // --------------------------------------------------------------------------
      void clear_clip_rect (core::context& ctx) {
        ctx.g->setClipping(false);
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_QT
