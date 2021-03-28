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
#include <gui/core/rectangle.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    gui::os::rectangle intersection (const gui::os::rectangle& lhs, const gui::os::rectangle& rhs) {
      gui::os::point_type x0 = std::max(gui::os::get_x(lhs),  gui::os::get_x(rhs));
      gui::os::point_type y0 = std::max(gui::os::get_y(lhs),  gui::os::get_y(rhs));
      gui::os::point_type x1 = std::min(gui::os::get_x2(lhs), gui::os::get_x2(rhs));
      gui::os::point_type y1 = std::min(gui::os::get_y2(lhs), gui::os::get_y2(rhs));
      return (x1 > x0) && (y1 > y0)? gui::os::mk_rectangle(x0, y0, x1, y1) : gui::os::rectangle();
    }

    // --------------------------------------------------------------------------
    void clipping_stack::push (core::context& ctx, const gui::os::rectangle& r) {
      if (stack.empty()) {
        stack.push_back(r);
      } else {
        stack.push_back(intersection(stack.back(), r));
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
    void clipping_stack::set (core::context& ctx, const gui::os::rectangle& r) {
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
      , offs_x(0)
      , offs_y(0)
      , own_gc(false)
    {
      clippings.clear(*this);
    }

    // --------------------------------------------------------------------------
    context::context (gui::os::drawable id)
      : id(id)
      , g(0)
      , offs_x(0)
      , offs_y(0)
      , own_gc(true)
    {
      g = core::native::create_graphics_context(id);
      clippings.clear(*this);
    }

    // --------------------------------------------------------------------------
    context::~context () {
      if (g) {
        clippings.clear(*this);
      }
      detach();
    }

    // --------------------------------------------------------------------------
    void context::detach () {
      if (own_gc) {
        core::native::delete_graphics_context(g);
        own_gc = false;
      }
      g = 0;
    }

//    core::native_point context::offset () const {
//      POINT pt;
//      GetViewportOrgEx(gc(), &pt);
//      return {pt.x, pt.y};
//    }

//    void context::set_offset (const core::native_point& o) {
//      SetViewportOrgEx(gc(), o.x(), o.y(), NULL);
//    }

    // --------------------------------------------------------------------------
    clip::clip (context& ctx, const core::native_rect& r)
      : ctx(ctx)
      , is_popped(false) {
      ctx.push_clipping(r.os(ctx));
    }
    // --------------------------------------------------------------------------
    clip::~clip () {
      unclip();
    }
    // --------------------------------------------------------------------------
    void clip::unclip () {
      if (!is_popped) {
        ctx.pop_clipping();
        is_popped = true;
      }
    }

  } // core

} // gui
