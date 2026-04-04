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
 * @brief     monochrome and coloured fast bitmaps
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <util/ostreamfmt.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui/core/native.h"
#include "gui/draw/bitmap.h"
#include "gui/draw/graphics.h"
#include "gui/draw/pen.h"
#include "gui/draw/use.h"


#if GUIPP_X11

# ifdef USE_XSHM
#  include <X11/Xlib.h>
#  include <sys/ipc.h>
#  include <sys/shm.h>
#  include <X11/extensions/XShm.h>
# endif // USE_XSHM

namespace gui {

    namespace native {

# ifdef USE_XSHM
    std::map<os::bitmap, std::pair<XShmSegmentInfo, draw::bitmap_info>> pixmaps;

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto dpy = core::global::get_instance();
      auto drawable = DefaultRootWindow(dpy);

      XShmSegmentInfo shminfo = {0};

      size_t sz = bmi.mem_size();
      shminfo.shmid = shmget(IPC_PRIVATE, sz, IPC_CREAT | 0777);
      shminfo.readOnly = False;
      shminfo.shmaddr = reinterpret_cast<char*>(shmat(shminfo.shmid, 0, 0));

      if (data) {
        memcpy(shminfo.shmaddr, data, sz);
      }

      Pixmap id = XShmCreatePixmap(dpy, drawable, shminfo.shmaddr, &shminfo, bmi.width, bmi.height, bmi.depth());

      int res = XShmAttach(dpy, &shminfo);

      pixmaps[id] = std::make_pair(shminfo, bmi);

      return id;
    }

    void free_bitmap (os::bitmap& id) {
      auto display = core::global::get_instance();

      XFreePixmap(display, id);

      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        int res = XShmDetach(display, &shminfo);
        pixmaps.erase(i);
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        auto i = pixmaps.find(id);
        if (i != pixmaps.end()) {
          return i->second.second;
        }
      }
      return {0, 0, 0, pixel_format_t::Undefined};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        memcpy(shminfo.shmaddr, data, bmi.mem_size());
        i->second.second = bmi;
      }
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        bmi = i->second.second;
        byteptr d = reinterpret_cast<byteptr>(shminfo.shmaddr);
        data.assign(d, d + bmi.mem_size());
      }
    }


# else // !USE_XSHM

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi);

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      const auto depth = bmi.bits_per_pixel();
      os::bitmap id = XCreatePixmap(display, visual, bmi.width, bmi.height, depth);
      switch (id) {
        case BadValue:
        case BadDrawable:
        case BadAlloc:
          throw std::runtime_error(ostreamfmt("XCreatePixmap failed with " << id));
        return 0;
      
        default:
          if (data) {
            bitmap_put_data(id, data, bmi);
          }
        return id;
      }
    }

    void free_bitmap (os::bitmap& id) {
      auto display = core::global::get_instance();
      int result = XFreePixmap(display, id);
      if (result == BadPixmap) {
          throw std::runtime_error(ostreamfmt("XCreatePixmap XFreePixmap with " << result));
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      draw::bitmap_info bmi;
      if (id) {
        Window root = 0;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        switch (st) {
          case BadDrawable:
          case BadWindow:
            throw std::runtime_error(ostreamfmt("XGetGeometry failed with " << st));
          break;
          
          default:
            bmi = {
              static_cast<uint32_t>(w),
              static_cast<uint32_t>(h),
              get_pixel_format(d, core::byte_order_t(ImageByteOrder(display)))
            };
          break;
        }
      }
      return bmi;
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      if (id) {
        Window root = 0;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        switch (st) {
          case BadDrawable:
          case BadWindow:
            throw std::runtime_error(ostreamfmt("XGetGeometry failed with " << st));
          break;
          
          default:
          break;
        }
        XImage* im = XGetImage(display, id, 0, 0, w, h, AllPlanes, ZPixmap);
        if (im) {
          bmi = {
            static_cast<uint32_t>(im->width),
            static_cast<uint32_t>(im->height),
            static_cast<uint32_t>(im->bytes_per_line),
            get_pixel_format(im->bits_per_pixel, core::byte_order_t(im->byte_order))
          };
          data.assign((const byte*)im->data, (const byte*)(im->data + bmi.mem_size()));
          XDestroyImage(im);
        } else {
          throw std::runtime_error("get image failed");
        }
      }
    }

    int calc_padding (int bytes_per_line, int width, int bits_per_pixel) {
      int rest = bytes_per_line - width * (bits_per_pixel / 8);
      switch (rest) {
        case 0: return 8;
        case 1: return 16;
        default: return 32;
      }
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      auto display = core::global::get_instance();
      auto gc = core::native::create_graphics_context(id);

      core::byte_order_t byte_order = get_pixel_format_byte_order(bmi.pixel_format);
      const int bpl = static_cast<int>(bmi.bytes_per_line);
      const int width = static_cast<int>(bmi.width);
      const int height = static_cast<int>(bmi.height);
      const int bpp = bmi.bits_per_pixel();
      const int pad = calc_padding(bpl, width, bpp);
      char *idata = const_cast<char*>(reinterpret_cast<const char*>(data));

      XImage im {
        width, height,                  /* size of image */
        0,                              /* number of pixels offset in X direction */
        ZPixmap,                        /* XYBitmap, XYPixmap, ZPixmap */
        idata,                          /* pointer to image data */
        static_cast<bool>(byte_order),  /* data byte order, LSBFirst, MSBFirst */
        BitmapUnit(display),            /* quant. of scanline 8, 16, 32 */
        BitmapBitOrder(display),        /* LSBFirst, MSBFirst */
        pad, //BitmapPad(display),      /* 8, 16, 32 either XY or ZPixmap */
        bpp,                            /* depth of image */
        bpl,                            /* accelarator to next line */
        bpp                             /* bits per pixel (ZPixmap) */
      };

      Status st = XInitImage(&im);
      if (st) {
        int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, bmi.width, bmi.height);
        core::native::delete_graphics_context(gc);
      } else {
        throw std::runtime_error("XInitImage failed");
      }
    }

    void copy_bitmap (draw::basic_map& lhs, const draw::basic_map& rhs) {
      draw::bitmap_info bmi = rhs.get_info();
      lhs.create(bmi);
      draw::graphics(lhs).copy_from((os::drawable)rhs, core::native_rect(bmi.size()));
    }

# endif // !USE_XSHM

  } // namespace native

}

#endif // !GUIPP_X11
