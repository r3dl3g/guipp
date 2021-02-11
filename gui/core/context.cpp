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

// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <logging/logger.h>
#include <gui/core/context.h>
#include <gui/core/native.h>
#ifdef GUIPP_QT
# include <QtGui/QPainter>
#endif // GUIPP_QT


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    // --------------------------------------------------------------------------
    void clipping_stack::push (core::context& ctx, const core::rectangle& r) {
      if (stack.empty()) {
        stack.push_back(r);
      } else {
        stack.push_back(stack.back() & r);
      }
      set(ctx, stack.back());
    }

    // --------------------------------------------------------------------------
    void clipping_stack::pop (core::context& ctx) {
      if (!stack.empty()) {
        stack.pop_back();
      }
      restore(ctx);
    }

    // --------------------------------------------------------------------------
    void clipping_stack::restore (core::context& ctx) {
      if (stack.empty()) {
        clear(ctx);
      } else {
        set(ctx, stack.back());
      }
    }

    // --------------------------------------------------------------------------
    void clipping_stack::set (core::context& ctx, const core::rectangle& r) {
      native::set_clip_rect(ctx, r);
    }

    // --------------------------------------------------------------------------
    void clipping_stack::clear (core::context& ctx) {
      native::clear_clip_rect(ctx);
    }

    // --------------------------------------------------------------------------
    context::context (gui::os::drawable id, gui::os::graphics g)
      : id(id)
      , g(g)
      , own_gc(false)
    {
      clippings.clear(*this);
    }

    // --------------------------------------------------------------------------
    context::context (gui::os::drawable id)
      : id(id)
      , g(0)
      , own_gc(true)
    {
#ifdef GUIPP_WIN
      HDC gdc = GetDC(NULL);
      g = CreateCompatibleDC(gdc);
      ReleaseDC(NULL, gdc);
#endif // GUIPP_WIN
#ifdef GUIPP_X11
      g = XCreateGC(global::get_instance(), id, 0, 0);
#endif // GUIPP_X11
#ifdef GUIPP_QT
      g = new QPainter(id);
#endif // GUIPP_QT
      clippings.clear(*this);
    }

    // --------------------------------------------------------------------------
    context::~context () {
      clippings.clear(*this);
      if (own_gc) {
#ifdef GUIPP_WIN
        DeleteDC(g);
#endif // GUIPP_WIN
#ifdef GUIPP_X11
        XFreeGC(global::get_instance(), g);
#endif // GUIPP_X11
#ifdef GUIPP_QT
        delete g;
#endif // GUIPP_QT
        own_gc = false;
        g = 0;
      }
    }

  } // core

} // gui
