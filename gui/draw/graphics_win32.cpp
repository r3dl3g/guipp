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

#ifdef GUIPP_WIN

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>

#include <util/ostreamfmt.h>
# include <util/string_util.h>

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

    // --------------------------------------------------------------------------
    graphics& graphics::draw_pixel (const core::native_point& pt,
                                          os::color c) {
      SetPixel(gc(), pt.x(), pt.y(), c);
      return *this;
    }

    os::color graphics::get_pixel (const core::native_point& pt) const {
      return GetPixel(gc(), pt.x(), pt.y());
    }

    graphics& graphics::copy_from (graphics& src,
                                   const core::native_rect& r,
                                   const core::native_point& pt) {
      if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                  src, r.x(), r.y(), SRCCOPY)) {
        throw std::runtime_error("graphics::copy_from failed");
      }
      return *this;
    }

    graphics& graphics::copy_from (os::drawable w,
                                   const core::native_rect& r,
                                   const core::native_point& pt,
                                   const copy_mode mode) {
      HDC source_gc = GetDC((HWND)w);
      if (!source_gc) {
        source_gc = CreateCompatibleDC(gc());
        HGDIOBJ old = SelectObject(source_gc, w);
        if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        SelectObject(source_gc, old);
        DeleteDC(source_gc);
      } else {
        if (!BitBlt(gc(), pt.x(), pt.y(), r.width(), r.height(),
                    source_gc, r.x(), r.y(), static_cast<uint32_t>(mode))) {
          throw std::runtime_error("graphics::copy_from failed");
        }
        ReleaseDC((HWND)w, source_gc);
      }
      return *this;
    }

    int get_stretch_blt_mode (const std::string& m) {
      if (m == "STRETCH_ANDSCANS") {
        return STRETCH_ANDSCANS;
      } else if (m == "STRETCH_ORSCANS") {
        return STRETCH_ORSCANS;
      } else if (m == "STRETCH_DELETESCANS") {
        return STRETCH_DELETESCANS;
      } else if(m == "STRETCH_HALFTONE") {
        return STRETCH_HALFTONE;
      }
      return 0;
    }

    graphics& graphics::draw_streched (const draw::pixmap& pixmap,
                                       const core::native_rect& dst,
                                       const core::native_point& src,
                                       const std::string& filter) {
      if (pixmap) {
        auto sz = pixmap.native_size();
        auto w = static_cast<const os::drawable&>(pixmap);
        HDC source_gc = GetDC((HWND)w);
        HDC dest_gc = gc();
        const auto old_mode = SetStretchBltMode(dest_gc, get_stretch_blt_mode(filter));
        if (!source_gc) {
          source_gc = CreateCompatibleDC(dest_gc);
          HGDIOBJ old = SelectObject(source_gc, w);
          if (!StretchBlt(dest_gc, dst.x(), dst.y(), dst.width(), dst.height(),
            source_gc, src.x(), src.y(), sz.width(), sz.height(), SRCCOPY)) {
            throw std::runtime_error("graphics::stretch_from failed");
          }
          SelectObject(source_gc, old);
          DeleteDC(source_gc);
        } else {
          if (!StretchBlt(dest_gc, dst.x(), dst.y(), dst.width(), dst.height(),
            source_gc, src.x(), src.y(), sz.width(), sz.height(), SRCCOPY)) {
            throw std::runtime_error("graphics::stretch_from failed");
          }
          ReleaseDC((HWND)w, source_gc);
        }
        SetStretchBltMode(dest_gc, old_mode);
      }
      return *this;
    }

    std::vector<std::string> graphics::get_filter_list(os::drawable d) {
      static std::vector<std::string> list = {"STRETCH_ANDSCANS", "STRETCH_ORSCANS", "STRETCH_DELETESCANS", "STRETCH_HALFTONE"};
      return list;
    }

    graphics& graphics::copy_from (const draw::masked_bitmap& bmp,
                                   const core::native_point& pt) {
      if (bmp.mask) {
        HDC dc = CreateCompatibleDC(gc());
        SelectObject(dc, bmp.mask.get_os_bitmap());
        core::native_size sz = bmp.mask.native_size();
        BitBlt(gc(), pt.x(), pt.y(), sz.width(), sz.height(), dc, 0, 0,
               static_cast<int>(copy_mode::bit_and)); //DSna, https://docs.microsoft.com/en-us/windows/win32/gdi/ternary-raster-operations
        if (bmp.image) {
          sz = bmp.image.native_size();
          SelectObject(dc, bmp.image.get_os_bitmap());
          BitBlt(gc(), pt.x(), pt.y(), sz.width(), sz.height(), dc, 0, 0,
                 static_cast<int>(copy_mode::bit_or));  // DSo
        }
        DeleteDC(dc);
      } else {
        core::native_size sz = bmp.image.native_size();
        copy_from(bmp.image, core::native_rect(sz), pt);
      }
      return *this;
    }

    void graphics::invert (const core::rectangle& r) {
      RECT rect = r.os(context());
      InvertRect(gc(), &rect);
    }

    void graphics::flush () {
      GdiFlush();
    }

    int graphics::depth () const {
      return GetDeviceCaps(gc(), BITSPIXEL);
    }

    core::native_rect graphics::native_area () const {
      core::native_rect r = {0, 0, 0, 0};
      os::window id = WindowFromDC(gc());
      if (id) {
        RECT s;
        GetWindowRect(id, &s);
        r.width(std::abs(s.right - s.left));
        r.height(std::abs(s.bottom - s.top));
      } else {
        HGDIOBJ hBmp = GetCurrentObject(gc(), OBJ_BITMAP);
        if (hBmp) {
          BITMAP bmp;
          memset(&bmp, 0, sizeof (BITMAP));
          GetObject(hBmp, sizeof (BITMAP), &bmp);
          r.width(bmp.bmWidth);
          r.height(bmp.bmHeight);
        } else {
          r.width(GetDeviceCaps(gc(), HORZRES));
          r.height(GetDeviceCaps(gc(), VERTRES));
        }
      }
      return r;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::rectangle& src, const core::point& pt) {
      if (bmp) {
        if (bmp.get_info().bits_per_pixel() == depth()) {
          return copy_from(static_cast<const os::drawable&>(bmp), src, pt);
        } else {
          switch (depth()) {
            case 24: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGB>())), src, pt);
              break;
            }
            case 32: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGBA>())), src, pt);
              break;
            }
          }
        }
      }
      return *this;
    }

    graphics& graphics::copy_from (const draw::pixmap& bmp, const core::native_rect& src, const core::native_point& pt) {
      if (bmp) {
        if (bmp.get_info().bits_per_pixel() == depth()) {
          return copy_from(static_cast<const os::drawable&>(bmp), src, pt);
        } else {
          switch (depth()) {
            case 24: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGB>())), src, pt);
              break;
            }
            case 32: {
              return copy_from(static_cast<const os::drawable&>(pixmap(bmp.get<pixel_format_t::RGBA>())), src, pt);
              break;
            }
          }
        }
      }
      return *this;
    }
    
  }
  
}

#endif // GUIPP_WIN