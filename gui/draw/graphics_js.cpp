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

#ifdef GUIPP_JS

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <emscripten/bind.h>

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

    using namespace emscripten;

    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      auto self = emscripten::val::global("self");
      self.set("ctx", os());
      EM_ASM_({
        const imgData = self.ctx.createImageData(1, 1);
        imgData.data.set(HEAPU8.subarray($0, $0 + 4));
        self.ctx.putImageData(imgData, $1, $2);
      }, &c, pt.x(), pt.y());
      self.set("ctx", emscripten::val::undefined());

      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      auto self = emscripten::val::global("self");
      self.set("ctx", os());
      os::color c = 0;
      EM_ASM_({
        let imageData = self.ctx.getImageData($1, $2, 1, 1);
        HEAPU8.subarray($0, $0 + 4).set(imageData.data);
      }, &c, pt.x(), pt.y());
      self.set("ctx", emscripten::val::undefined());

      return c;
    }

    void graphics::flush () {
    }

    int graphics::depth () const {
      return 32;
    }

    core::native_rect graphics::native_area () const {
        auto self = emscripten::val::global("self");
        auto rect = self["rect"];
        // int x = rect["x"].as<int>();
        // int y = rect["y"].as<int>();
        core::native_rect::size_type width = rect["width"].as<int>();
        core::native_rect::size_type height = rect["height"].as<int>();

        return {0, 0, width, height};
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      return copy_from(src.os(), r, pt, copy_mode::bit_copy);
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      core::native_size sz = bmp.image.native_size();
      os().call<void>("save");
      copy_from(bmp.mask.get_os_bitmap(), core::native_rect(sz), pt, copy_mode::bit_copy);
      os().call<void>("globalCompositeOperation", std::string("source-in"));
      copy_from(bmp.image.get_os_bitmap(), core::native_rect(sz), pt, copy_mode::bit_copy);
      os().call<void>("restore");

      // EM_ASM_({
      //   let src = $4;
      //   let tgt = $5;
      //   let imageData = src.getImageData(0, 0, $2, $3);
      //   tgt.putImageData(imageData, $0, $1);
      // }, pt.x(), pt.y(), sz.width(), sz.height(), os().as_handle(), bmp.image.get_os_bitmap().as_handle());

      // auto imageData = bmp.image.get_os_bitmap().call<val>("getImageData", 0, 0, sz.width(), sz.height());
      // os().call<void>("putImageData", imageData, pt.x(), pt.y());
      return *this;
    }

    graphics& graphics::copy_from (os::drawable src,
                                   const core::native_rect& r,
                                   const core::native_point& pt,
                                   const copy_mode) {
      logging::trace() << "copy_from(drawable)";
      os().call<void>("drawImage", src,
        r.x(), r.y(), r.width(), r.height(),
        pt.x(), pt.y(), r.width(), r.height());

      // auto imageData = src.call<val>("getImageData", r.x(), r.y(), r.width(), r.height());
      // os().call<void>("putImageData", imageData, pt.x(), pt.y());
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap, const core::rectangle& src, const core::point& pt) {
      return copy_from(pixmap, core::global::scale_to_native(src), core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap,
                                   const core::native_rect& src,
                                   const core::native_point& dst) {
      if (pixmap) {
        logging::trace() << "copy_from(pixmap))";
        copy_from(pixmap.get_os_bitmap(), src, dst, copy_mode::bit_copy);
        // os().call<void>("drawImage", pixmap.get_os_bitmap(),
        //   src.x(), src.y(), src.width(), src.height(),
        //   dst.x(), dst.y(), src.width(), src.height());
      }
      return *this;
    }

    graphics& graphics::draw_streched (const draw::pixmap& pixmap,
                                       const core::native_rect& dst,
                                       const core::native_point& src,
                                       const std::string& filter) {
      if (pixmap) {
        logging::trace() << "draw_streched(pixmap))";
        auto sz = pixmap.native_size();
        os().call<void>("drawImage", pixmap.get_os_bitmap(),
          src.x(), src.y(), sz.width(), sz.height(),
          dst.x(), dst.y(), dst.width(), dst.height());
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      // auto rect = core::global::scale_to_native(r);
    }

    std::vector<std::string> graphics::get_filter_list(os::drawable d) {
      static std::vector<std::string> list = {"STRETCH"};
      return list;
    }

    // --------------------------------------------------------------------------

  }

}

#endif // GUIPP_JS
