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
 * @brief     graphic functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/graphics.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/shared_datamap.h"
#include "gui/draw/drawers.h"
#include "gui/draw/pen.h"
#include "gui/draw/brush.h"
#include "gui/draw/use.h"


namespace gui {

  namespace draw {

    bool is_transparent (const pen& p) {
      return color::is_transparent(p.color()) || (p.style() == pen::Style::invisible);
    }

    bool is_transparent (const brush& p) {
      return color::is_transparent(p.color()) || (p.style() == brush::Style::invisible);
    }

    graphics::graphics (core::context* ctx, const core::native_rect& r)
      : ctx(ctx)
      , invalid_area(r)
      , own_gctx(false)
    {}

    graphics::graphics (draw::basic_map& target)
      : ctx(0)
      , invalid_area(target.native_size())
      , own_gctx(true)
    {
      ctx = new core::context(target);
#ifdef GUIPP_QT
      gc()->begin(target);
#endif // GUIPP_QT
    }

    graphics::~graphics () {
      flush();
      destroy();
    }

    void graphics::destroy () {
      if (ctx) {
        if (own_gctx) {
#ifdef GUIPP_QT
          gc()->end();
#endif // GUIPP_QT
#ifndef GUIPP_JS
          delete ctx;
#endif // GUIPP_JS
        }
        ctx = 0;
      }
      own_gctx = false;
    }

    // --------------------------------------------------------------------------

    const core::native_rect& graphics::get_invalid_area () const {
      return invalid_area;
    }

    graphics& graphics::clear (os::color color) {
      return draw(rectangle(area()), color, color);
    }

    graphics& graphics::erase (const core::rectangle& r, os::color color) {
      return draw(rectangle(r), color, color);
    }

    core::rectangle graphics::area () const {
      return core::global::scale_from_native(native_area());
    }

    graphics& graphics::copy_from (graphics& src, const core::point& pt) {
      return copy_from(src, src.native_area(), core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (graphics& src, const core::native_point& pt) {
      return copy_from(src, src.native_area(), pt);
    }

    graphics& graphics::copy_from (os::drawable w,
                                   const core::rectangle& r,
                                   const core::point& pt,
                                   const copy_mode mode) {
      return copy_from(w, core::global::scale_to_native(r), core::native_point(pt.os(context()), context()), mode);
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::point& pt) {
      return copy_from(bmp, core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_point& pt) {
      return copy_from(bmp, core::native_rect(bmp.native_size()), pt);
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::point& pt) {
      return copy_from(bmp, core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::draw_streched (const draw::pixmap& bmp,
                                       const core::rectangle& r,
                                       const core::point& pt,
                                       const std::string& filter) {
      return draw_streched(bmp, core::native_rect(r.os(context()), context()), core::global::scale_to_native(pt), filter);
    }

    // --------------------------------------------------------------------------
    paint::paint (const painter& f)
      : p(f)
    {}

    paint::paint (painter&& f)
      : p(std::move(f))
    {}

    void paint::operator() (core::context* s, const core::native_rect* r) {
      if (p) {
        draw::graphics graph(s, *r);
        p(graph);
      }
    }

    // --------------------------------------------------------------------------
    clip::clip (graphics& g, const core::rectangle& r)
      : core::clip(g.context(), core::native_rect(r.os(g.context()), g.context()))
    {}
  }

}
