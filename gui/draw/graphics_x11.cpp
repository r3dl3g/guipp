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

#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
# include <algorithm>
# include <X11/extensions/Xrender.h>

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

// For debugging
std::ostream& operator<< (std::ostream& out, const XGCValues& v) {
  out << "{function="    << std::hex << v.function
      << ", plane_mask=" << v.plane_mask
      << ", foreground=" << v.foreground
      << ", background=" << v.background
      << ", line_width=" << v.line_width
      << ", line_style=" << v.line_style
      << ", cap_style="  << v.cap_style
      << ", fill_style=" << v.fill_style
      << ", fill_rule="  << v.fill_rule
      << ", arc_mode="   << v.arc_mode
      << ", tile="       << v.tile
      << ", stipple="    << v.stipple
      << ", ts_x_origin="<< v.ts_x_origin
      << ", ts_y_origin="<< v.ts_y_origin
      << ", font="       << v.font
      << ", subwindow_mode=" << v.subwindow_mode
      << ", graphics_exposures="<< v.graphics_exposures
      << ", clip_x_origin=" << v.clip_x_origin
      << ", clip_y_origin=" << v.clip_y_origin
      << ", clip_mask="     << v.clip_mask
      << ", dash_offset="   << v.dash_offset
      << ", dashes="        << (int)v.dashes
      << "}";
  return out;
}

namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    using namespace core::global;

    // --------------------------------------------------------------------------
    int get_drawable_depth (os::drawable t) {
      Window root;
      int x, y;
      unsigned int w, h, b, d;
      XGetGeometry(get_instance(), t, &root, &x, &y, &w, &h, &b, &d);
      return d;
    }

    core::native_rect get_drawable_area (os::drawable t) {
      Window root;
      int x, y;
      unsigned int w, h, b, d;
      XGetGeometry(get_instance(), t, &root, &x, &y, &w, &h, &b, &d);
      return {
          (core::native_point::type)0,
          (core::native_point::type)0,
          (core::native_size::type)w,
          (core::native_size::type)h
      };
    }

    // --------------------------------------------------------------------------
    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      Use<pen> pn(gc(), pen(c));
      XDrawPoint(get_instance(), target(), gc(), pt.x(), pt.y());
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      XImage* im = XGetImage(get_instance(), target(), pt.x(), pt.y(), 1, 1, AllPlanes, ZPixmap);
      os::color c = color::black;
      if (im) {
        if (im->bits_per_pixel == 1) {
          c = XGetPixel(im, 0, 0) ? color::white : color::black;
        } else {
          c = XGetPixel(im, 0, 0);
        }
        XDestroyImage(im);
      }
      return c;
    }

    os::point operator+ (const os::point& pt, short i) {
      return { (short)(pt.x + i), (short)(pt.y + i) };
    }

    graphics& graphics::copy_from (graphics& src, const core::native_rect& r, const core::native_point& pt) {
      int res = XCopyArea(get_instance(), src.target(), target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
      if (!res) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }


    graphics& graphics::copy_from (os::drawable w,
                                   const core::native_rect& r,
                                   const core::native_point& pt,
                                   const copy_mode mode) {
      const int dd = get_drawable_depth(w);
      const int md = depth();
      auto display = core::global::get_instance();
      XGCValues values = { static_cast<int>(static_cast<uint32_t>(mode)) };
      values.graphics_exposures = False;
      XChangeGC(display, gc(), GCFunction|GCGraphicsExposures, &values);
      if (dd == md) {
        /*int res =*/ XCopyArea(get_instance(), w, target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y());
      } else if (1 == dd) {
        /*int res =*/ XCopyPlane(get_instance(), w, target(), gc(), r.x(), r.y(), r.width(), r.height(), pt.x(), pt.y(), 1);
      } else {
        throw std::runtime_error(ostreamfmt("incompatible drawable (" << dd << ") in graphics::copy_from (" << md << " expected)"));
      }
      values = { GXcopy };
      XChangeGC(display, gc(), GCFunction, &values);
      return *this;
    }

    void set_xrender_clipping (const core::context& ctx, Picture window) {
      if (!ctx.clipping().empty()) {
        auto cr = ctx.clipping().back();
        XRectangle clip = { cr.x, cr.y, cr.width, cr.height };
        XRenderSetPictureClipRectangles(core::global::get_instance(),
                                        window, 0, 0,    // Clip-Origin Offset
                                        &clip,   // Array von XRectangles
                                        1);      // Anzahl

      }
    }

    void clear_xrender_clipping (const core::context& ctx, Picture window) {
      if (!ctx.clipping().empty()) {
        XRenderPictureAttributes pa = { 0 };
        pa.clip_mask = None;
        XRenderChangePicture(core::global::get_instance(), window, CPClipMask, &pa);        
      }
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap,
                                   const core::native_rect& src,
                                   const core::native_point& dest) {
      if (pixmap.get_info().bits_per_pixel() == depth()) {
        return copy_from(pixmap.get_os_bitmap(), src, dest, copy_mode::bit_copy);
      }

      auto display = core::global::get_instance();

      int format = PictStandardARGB32;
      int op = PictOpSrc;
      switch (pixmap.pixel_format()) {
        case pixel_format_t::BW:
          format = PictStandardA1;
          break;
        case pixel_format_t::GRAY:
          format = PictStandardA8;
          break;
        case pixel_format_t::RGB:
        case pixel_format_t::BGR:
          format = PictStandardRGB24;
          break;
        case pixel_format_t::RGBA:
        case pixel_format_t::BGRA:
        case pixel_format_t::ARGB:
        case pixel_format_t::ABGR:
          op = PictOpOver;
          // format = PictStandardARGB32;
          break;
      }

      XRenderPictFormat *pixmap_format = XRenderFindStandardFormat(display, format);
      XRenderPictFormat *win_format = XRenderFindVisualFormat(display, core::global::x11::get_visual());

      XRenderPictureAttributes pa = { 0 };
      pa.subwindow_mode = IncludeInferiors;

      Picture picture = XRenderCreatePicture(display, pixmap.get_os_bitmap(), pixmap_format, 0, NULL);
      Picture window = XRenderCreatePicture(display, target(), win_format, 0, &pa);

      set_xrender_clipping(context(), window);

      XRenderComposite(display, op, picture, 0, window, src.x(), src.y(), 0, 0, dest.x(), dest.y(), src.width(), src.height());

      clear_xrender_clipping(context(), window);

      XRenderFreePicture(display, window);
      XRenderFreePicture(display, picture);
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& pixmap, const core::rectangle& src, const core::point& pt) {
      return copy_from(pixmap, core::global::scale_to_native(src), core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp, const core::native_point& pt) {
      auto display = core::global::get_instance();
      int res = 0;
      // If previous clipping intersect with this region, to much of the image is drawn.
      // -> restrict the copy or fill area to the intersection rectangle withthe previous clipping rectangle.
      core::native_rect clip;
      if (bmp.mask) {
        if (!ctx->clipping().empty()) {
          clip = core::native_rect(ctx->clipping().back(), context());
        }
        XSetClipMask(display, gc(), bmp.mask.get_os_bitmap());
        XSetClipOrigin(display, gc(), pt.x(), pt.y());
      }

      if (bmp.image) {
        core::native_rect src(bmp.image.native_size());
        res = XCopyArea(get_instance(), bmp.image, target(), gc(), src.x(), src.y(), src.width(), src.height(), pt.x(), pt.y());
//        core::native_rect src(pt, bmp.image.native_size());
//        if (!clip.empty()) {
//          src &= clip;
//        }
//        res = XCopyArea(get_instance(), bmp.image, target(), gc(), src.x() - pt.x(), src.y() - pt.x(), src.width(), src.height(), src.x(), src.y());
      } else {
        Use<brush> br(gc(), color::black);

        core::native_rect src(pt, bmp.mask.native_size());
        if (!clip.empty()) {
          src &= clip;
        }

        XFillRectangle(display, target(), gc(), src.x(), src.y(), src.width(), src.height());
      }

      if (bmp.mask) {
        XSetClipMask(display, gc(), None);
      }
      return *this;
    }

#ifdef GUIPP_USE_XSHM
    graphics& graphics::copy_from (const draw::shared_datamap& bmp, const core::native_point& pt) {
      return copy_from(bmp, core::native_rect(bmp.native_size()), pt);
    }

    graphics& graphics::copy_from (const draw::shared_datamap& bmp, const core::native_rect& src, const core::native_point& pt) {
      if (bmp.is_valid()) {
        if (core::global::x11::has_XShm()) {
          auto display = core::global::get_instance();
          logging::trace() << "Call XShmPutImage for id " << bmp.shminfo.shmid << " and adress " << std::hex << (ptrdiff_t)bmp.shminfo.shmaddr;
          Bool result = XShmPutImage(display, target(), gc(), bmp.image, src.x(), src.y(), pt.x(), pt.y(), src.width(), src.height(), False);
          if (!result) {
              logging::error() << "XShmPutImage of area " << src << " to point " << pt << " failed!";
          }
        } else {
          pixmap buffer(bmp.get_data());
          copy_from(buffer, src, pt);
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::shared_datamap& bmp, const core::point& pt) {
      return copy_from(bmp, core::native_point(pt.os(context()), context()));
    }

    graphics& graphics::copy_from (const draw::shared_datamap& bmp, const core::rectangle& r, const core::point& pt) {
      return copy_from(bmp, core::global::scale_to_native(r), core::native_point(pt.os(context()), context()));
    }
#endif // GUIPP_USE_XSHM

    static inline XFixed fx(double v) { return XDoubleToFixed(v); }

    graphics& graphics::draw_streched (const draw::pixmap& pixmap,
                                       const core::native_rect& dest,
                                       const core::native_point& src,
                                       const std::string& filter) {
      if (pixmap) {
        auto display = core::global::get_instance();

        int format = PictStandardARGB32;
        int op = PictOpSrc;
        switch (pixmap.pixel_format()) {
          case pixel_format_t::BW:
            format = PictStandardA1;
            break;
          case pixel_format_t::GRAY:
            format = PictStandardA8;
            break;
          case pixel_format_t::RGB:
          case pixel_format_t::BGR:
            format = PictStandardRGB24;
            break;
          case pixel_format_t::RGBA:
          case pixel_format_t::BGRA:
          case pixel_format_t::ARGB:
          case pixel_format_t::ABGR:
            op = PictOpOver;
            // format = PictStandardARGB32;
            break;
        }

        XRenderPictFormat *pixmap_format = XRenderFindStandardFormat(display, format);
        XRenderPictFormat *win_format = XRenderFindVisualFormat(display, core::global::x11::get_visual());

        XRenderPictureAttributes pa = { 0 };
        pa.subwindow_mode = IncludeInferiors;

        Picture picture = XRenderCreatePicture(display, pixmap.get_os_bitmap(), pixmap_format, 0, NULL);
        Picture window = XRenderCreatePicture(display, target(), win_format, 0, &pa);

        auto psz = pixmap.native_size();
        auto dsz = dest.size();

        if (psz != dsz) {
          double sx = double(psz.width())  / double(dsz.os_width());
          double sy = double(psz.height()) / double(dsz.os_height());
          XTransform xf = {{
              { fx(sx),  fx(0),  fx(0) },
              { fx(0),   fx(sy), fx(0) },
              { fx(0),   fx(0),  fx(1) }
          }};
          XRenderSetPictureTransform(display, picture, &xf);
          if (!filter.empty()) {
            XRenderSetPictureFilter(display, picture, const_cast<char*>(filter.c_str()), nullptr, 0);
          }
        }

        set_xrender_clipping(context(), window);
        XRenderComposite(display, op, picture, 0, window, src.x(), src.y(), 0, 0, dest.x(), dest.y(), dest.width(), dest.height());
        clear_xrender_clipping(context(), window);

        XRenderFreePicture(display, window);
        XRenderFreePicture(display, picture);
      }
      return *this;
    }

    std::vector<std::string> graphics::get_filter_list (os::drawable d) {
      std::vector<std::string> list;
      XFilters* filters = XRenderQueryFilters(core::global::get_instance(), d);
      if (filters) {
        list.reserve(filters->nfilter);
        if (filters) {
          for (int i = 0; i < filters->nfilter; ++i) {
            list.push_back(filters->filter[i]);
          }
        }
      }
      return list;
    }

    void graphics::invert (const core::rectangle& r) {
      auto display = core::global::get_instance();

      XGCValues values = { GXinvert, 0 };  // .function =
      values.graphics_exposures = False;
      XChangeGC(display, gc(), GCFunction, &values);
      XFillRectangle(display, target(), gc(), r.os_x(context()), r.os_y(context()), r.os_width(), r.os_height());
      values = { GXcopy, 0 }; // .function =
      XChangeGC(display, gc(), GCFunction, &values);
    }

    void graphics::flush () {
      XFlushGC(get_instance(), gc());
    }

    int graphics::depth () const {
      return get_drawable_depth(target());
    }

    core::native_rect graphics::native_area () const {
      return get_drawable_area(target());
    }

#ifdef GUIPP_USE_XFT
    XftDraw* graphics::get_xft () const {
      return core::native::x11::get_xft_draw(*ctx);
    }

    graphics::operator XftDraw* () const {
      return get_xft();
    }
#endif // GUIPP_USE_XFT
  
  } // namespace draw

} // namespace gui

#endif // GUIPP_X11
