/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: bitmap
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <limits>
#include <algorithm>
#include <map>
#include <math.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/bitmap.h>
#include <gui/draw/graphics.h>
#include <gui/core/logger.h>


#ifdef X11

//# define USE_XSHM

# ifdef USE_XSHM
#  include <X11/Xlib.h>
#  include <sys/ipc.h>
#  include <sys/shm.h>
#  include <X11/extensions/XShm.h>
# endif // USE_XSHM

#endif // X11


namespace gui {

  namespace {

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data = nullptr);
    void free_bitmap (os::bitmap id);
    void bitmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi);
    void bitmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi);
    draw::bitmap_info bitmap_get_info (os::bitmap id);

#ifdef X11

# ifdef USE_XSHM
    std::map<os::bitmap, std::pair<XShmSegmentInfo, draw::bitmap_info>> pixmaps;

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto dpy = core::global::get_instance();
      auto drawable = DefaultRootWindow(dpy);

      XShmSegmentInfo shminfo = {0};

      size_t sz = bmi.mem_size();
      shminfo.shmid = shmget(IPC_PRIVATE, sz, IPC_CREAT | 0644);
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

    void free_bitmap (os::bitmap id) {
      auto display = core::global::get_instance();

      XFreePixmap(display, id);

      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        int res = XShmDetach(display, &shminfo);
        pixmaps.erase(i);
      }
    }

    draw::bitmap_info bitmap_get_info (os::bitmap id) {
      if (id) {
        auto i = pixmaps.find(id);
        if (i != pixmaps.end()) {
          return i->second.second;
        }
      }
      return {0, 0, 0, BPP::Undefined};
    }

    void bitmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi) {
      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        memcpy(shminfo.shmaddr, data, bmi.mem_size());
        i->second.second = bmi;
      }
    }

    void bitmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi) {
      auto i = pixmaps.find(id);
      if (i != pixmaps.end()) {
        XShmSegmentInfo& shminfo = i->second.first;
        bmi = i->second.second;
        byteptr d = reinterpret_cast<byteptr>(shminfo.shmaddr);
        data.assign(d, d + bmi.mem_size());
      }
    }


# else // !USE_XSHM

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      os::bitmap id = XCreatePixmap(display, visual, bmi.width, bmi.height, bmi.depth());
      if (data) {
        bitmap_put_data(id, data, bmi);
      }
      return id;
    }

    void free_bitmap (os::bitmap id) {
      auto display = core::global::get_instance();
      XFreePixmap(display, id);
    }

    draw::bitmap_info bitmap_get_info (os::bitmap id) {
      if (id) {
        Window root;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        return {
          static_cast<uint32_t>(w),
          static_cast<uint32_t>(h),
          BPP(d)
        };
      }
      return {0, 0, 0, BPP::Undefined};
    }

    void bitmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi) {
      auto display = core::global::get_instance();
      //auto screen = core::global::get_screen();
      auto gc = XCreateGC(display, id, 0, nullptr);

      XImage im {
        static_cast<int>(bmi.width),
        static_cast<int>(bmi.height),                           /* size of image */
        0,                                                      /* number of pixels offset in X direction */
        ZPixmap,                                                /* XYBitmap, XYPixmap, ZPixmap */
        const_cast<char*>(reinterpret_cast<const char*>(data)), /* pointer to image data */
        ImageByteOrder(display),                                /* data byte order, LSBFirst, MSBFirst */
        BitmapUnit(display),                                    /* quant. of scanline 8, 16, 32 */
        BitmapBitOrder(display),                                /* LSBFirst, MSBFirst */
        BitmapPad(display),                                     /* 8, 16, 32 either XY or ZPixmap */
        bmi.depth(),                                            /* depth of image */
        static_cast<int>(bmi.bytes_per_line),                   /* accelarator to next line */
        bmi.depth(),                                            /* bits per pixel (ZPixmap) */
        color::mask<color::part::red>::value,                   /* bits in z arrangment */
        color::mask<color::part::green>::value,
        color::mask<color::part::blue>::value
      };

      Status st = XInitImage(&im);
      int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, bmi.width, bmi.height);
      res = XFreeGC(display, gc);
    }

    void bitmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi) {
      bmi = bitmap_get_info(id);
      auto display = core::global::get_instance();
      XImage* im = XGetImage(display, id, 0, 0, bmi.width, bmi.height, AllPlanes, ZPixmap);
      if (im) {
        draw::bitmap_info src_bmi = {
          static_cast<uint32_t>(im->width),
          static_cast<uint32_t>(im->height),
          static_cast<uint32_t>(im->bytes_per_line),
          BPP(im->bits_per_pixel)
        };
        bmi = {
          static_cast<uint32_t>(im->width),
          static_cast<uint32_t>(im->height),
          BPP(im->depth)
        };
        const size_t n = src_bmi.mem_size();
        if (src_bmi.bits_per_pixel != bmi.bits_per_pixel) {
          data.resize(bmi.mem_size());

          byte* src = reinterpret_cast<byte*>(im->data);
          switch (bmi.bits_per_pixel) {
            case BPP::RGB: {
              typedef draw::const_image_data<BPP::RGBA> src_data;
              typedef draw::image_data<BPP::RGB> dst_data;
              //<BPP::RGBA, BPP::RGB>
              convert::bpp::convert(src_data(src_data::raw_type(src, n), src_bmi),
                                    dst_data(dst_data::raw_type(data), bmi),
                                    im->width, im->height);
            }
            break;
            case BPP::RGBA:
              typedef draw::const_image_data<BPP::RGB> src_data;
              typedef draw::image_data<BPP::RGBA> dst_data;
              //<BPP::RGB, BPP::RGBA>
              convert::bpp::convert(src_data(src_data::raw_type(src, n), src_bmi),
                                    dst_data(dst_data::raw_type(data), bmi),
                                    im->width, im->height);
            break;
          }
        } else {
          data.assign(im->data, im->data + n);
        }
        XDestroyImage(im);
      } else {
        throw std::runtime_error("get image failed");
      }
    }

# endif // !USE_XSHM

#endif // !X11

#if WIN32

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      return CreateBitmap(bmi.width, bmi.height, 1, bmi.depth(), data);
    }

    void free_bitmap (os::bitmap id) {
      DeleteObject(id);
    }

    draw::bitmap_info bitmap_get_info (os::bitmap id) {
      if (id) {
        BITMAP bmp;
        GetObject(id, sizeof (BITMAP), &bmp);
        return {
          static_cast<uint32_t>(bmp.bmWidth),
          static_cast<uint32_t>(bmp.bmHeight),
          static_cast<uint32_t>(bmp.bmWidthBytes),
          BPP(bmp.bmBitsPixel)
        };
      }
      return {};
    }

    void bitmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi) {
      SetBitmapBits(id, (DWORD)bmi.mem_size(), data);
    }

    void bitmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi) {
      bmi = bitmap_get_info(id);
      data.resize(bmi.height * bmi.bytes_per_line);
      int ret = GetBitmapBits(id, (LONG)data.size(), data.data());
      if (ret != data.size()) {
        throw std::runtime_error("get image data failed");
      }
    }

#endif // WIN32

  }

  namespace draw {

    // --------------------------------------------------------------------------
    void basic_map::operator= (basic_map&& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs.is_valid()) {
          std::swap(id, rhs.id);
        }
      }
    }

    void basic_map::operator= (const basic_map& rhs) {
      if (&rhs != this) {
        if (rhs) {
          blob bin;
          bitmap_info bmi;
          bitmap_get_data(rhs.get_id(), bin, bmi);
          create(bmi);
          bitmap_put_data(get_id(), bin.data(), bmi);
        } else {
          clear();
        }
      }
    }

    void basic_map::clear () {
      if (is_valid()) {
        free_bitmap(get_id());
        set_id(0);
      }
    }

    bitmap_info basic_map::get_info () const {
      return bitmap_get_info(get_id());
    }

    core::size basic_map::size () const {
      return get_info().size<core::size::type>();
    }

    byte basic_map::depth () const {
      return get_info().depth();
    }

    BPP basic_map::bits_per_pixel () const {
      return get_info().bits_per_pixel;
    }

    void basic_map::create (const bitmap_info& rhs) {
      bitmap_info bmi = get_info();
      if (bmi == rhs) {
        return;
      }
      clear();
      set_id(create_bitmap(rhs));
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    uint32_t basic_map::calc_bytes_per_line (uint32_t w, BPP bpp) {
      return draw::bitmap_info(w, 0, bpp).bytes_per_line;
    }

    // --------------------------------------------------------------------------
    void bitmap::create (uint32_t w, uint32_t h) {
      super::create({w, h, BPP::BW});
      if (!is_valid()) {
        throw std::runtime_error("create bitmap failed");
      }
    }

    void bitmap::copy_from (const bitmap& src_img,
                            const core::rectangle& src_rect,
                            const core::point& dest_pt) {
      bwmap dest;
      dest.copy_from(src_img.get(), src_rect, dest_pt);
      operator=(dest);
    }

    void bitmap::invert () {
      bwmap data = get();
      data.invert();
      operator=(data);
    }

    void bitmap::operator= (const bwmap& rhs) {
      if (rhs) {
        const auto raw = rhs.get_raw();
        const auto& bmi = raw.get_info();
        create(bmi.size());
        bitmap_put_data(get_id(), raw.raw_data().data(0, bmi.mem_size()), bmi);
      } else {
        clear();
      }
    }

    bwmap bitmap::get () const {
      blob data;
      bitmap_info bmi;
      bitmap_get_data(get_id(), data, bmi);
      return bwmap(const_image_data<BPP::BW>(core::array_wrapper<const byte>(data), bmi));
    }

    bitmap::operator bwmap () const {
      return get();
    }

    // --------------------------------------------------------------------------
    void pixmap::create (uint32_t w, uint32_t h) {
#if WIN32
      clear();
      HDC dc = GetDC(NULL);
      set_id(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
//      int count = 0;
//      XPixmapFormatValues* fmts = XListPixmapFormats(core::global::get_instance(), &count);
//      for (int i = 0; i < count; ++i) {
//        LogDebug << "PixmapFormat " << i << ": bits:" << fmts[i].bits_per_pixel << " depth:" << fmts[i].depth << " pad:" << fmts[i].scanline_pad;
//      }
//      BPP bpp = BPP::RGBA;
      BPP bpp = core::global::get_device_bits_per_pixel();
      super::create({w, h, bpp});
#endif
      if (!is_valid()) {
        throw std::runtime_error("create pixmap failed");
      }
    }

    void pixmap::copy_from (const pixmap& src_img,
                            const core::rectangle& src_rect,
                            const core::point& dest_pt) {
      graphics g(*this);
      g.copy_from(src_img, src_rect, dest_pt);
    }

    void pixmap::invert () {
      switch (bits_per_pixel()) {
        case BPP::BW:   invert<BPP::BW>();   break;
        case BPP::GRAY: invert<BPP::GRAY>(); break;
        case BPP::RGB:  invert<BPP::RGB>();  break;
        case BPP::RGBA: invert<BPP::RGBA>(); break;
        default:  break;
      }
    }

    void pixmap::put (cbyteptr data, const draw::bitmap_info& bmi) {
      if (is_valid()) {
        bitmap_put_data(get_id(), data, bmi);
      }
    }

    void pixmap::get (blob& data, draw::bitmap_info& bmi) const {
      if (is_valid()) {
        bitmap_get_data(get_id(), data, bmi);
      }
    }


    // --------------------------------------------------------------------------
    void masked_bitmap::operator= (const masked_bitmap& rhs) {
      if (&rhs != this) {
        image = rhs.image;
        mask = rhs.mask;
      }
    }

    void masked_bitmap::operator= (masked_bitmap&& rhs) {
      if (&rhs != this) {
        image = std::move(rhs.image);
        mask = std::move(rhs.mask);
      }
    }

    void masked_bitmap::operator= (const pixmap& rhs) {
      image = rhs;
      if (image.is_valid()) {
        mask = image.get<BPP::BW>();
      }
    }

    void masked_bitmap::operator= (pixmap&& rhs) {
      image = std::move(rhs);
      if (image.is_valid()) {
        mask = image.get<BPP::BW>();
      }
    }

    // --------------------------------------------------------------------------
  }

}
