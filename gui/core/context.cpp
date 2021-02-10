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
      g = XCreateGC(global::get_instance(), id, 0, 0);
      clippings.clear(*this);
    }

    // --------------------------------------------------------------------------
    context::~context () {
      clippings.clear(*this);
      if (own_gc) {
        XFreeGC(global::get_instance(), g);
        own_gc = false;
        g = 0;
      }
    }

  } // core

} // gui
