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
#include <gui/draw/use.h>
#include <logging/logger.h>


#ifdef GUIPP_X11

//# define USE_XSHM

# ifdef USE_XSHM
#  include <X11/Xlib.h>
#  include <sys/ipc.h>
#  include <sys/shm.h>
#  include <X11/extensions/XShm.h>
# endif // USE_XSHM

#endif // GUIPP_X11

#ifdef GUIPP_QT

#include <QtGui/QBitmap>

#endif // GUIPP_QT


namespace gui {

  namespace {

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data = nullptr);
    void free_bitmap (os::bitmap& id);
    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi);
    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi);
    draw::bitmap_info bitmap_get_info (const os::bitmap& id);

#ifdef GUIPP_X11
//    void pixmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi);
//    void pixmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi);

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
        data.assing(d, d + bmi.mem_size());
      }
    }


# else // !USE_XSHM

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      const auto depth = bmi.depth();
      os::bitmap id = XCreatePixmap(display, visual, bmi.width, bmi.height, depth);
      if (data) {
        bitmap_put_data(id, data, bmi);
      }
      return id;
    }

    void free_bitmap (os::bitmap& id) {
      auto display = core::global::get_instance();
      XFreePixmap(display, id);
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      draw::bitmap_info bmi;
      if (id) {
        Window root = 0;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        (void)st;
        bmi = {
          static_cast<uint32_t>(w),
          static_cast<uint32_t>(h),
          get_pixel_format(d, core::byte_order_t(ImageByteOrder(display)))
        };
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
        (void)st;
        XImage* im = XGetImage(display, id, 0, 0, w, h, AllPlanes, ZPixmap);
        if (im) {
          bmi = {
            static_cast<uint32_t>(im->width),
            static_cast<uint32_t>(im->height),
            static_cast<uint32_t>(im->bytes_per_line),
            get_pixel_format(im->bits_per_pixel, core::byte_order_t(im->byte_order))
          };
          data.assign(im->data, im->data + bmi.mem_size());
          XDestroyImage(im);
        } else {
          throw std::runtime_error("get image failed");
        }
      }
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      auto display = core::global::get_instance();
      auto gc = XCreateGC(display, id, 0, nullptr);

      core::byte_order_t byte_order_t = get_pixel_format_byte_order(bmi.pixel_format);

      XImage im {
        static_cast<int>(bmi.width),
        static_cast<int>(bmi.height),                           /* size of image */
        0,                                                      /* number of pixels offset in X direction */
        ZPixmap,                                                /* XYBitmap, XYPixmap, ZPixmap */
        const_cast<char*>(reinterpret_cast<const char*>(data)), /* pointer to image data */
        static_cast<bool>(byte_order_t),                          /* data byte order, LSBFirst, MSBFirst */
        BitmapUnit(display),                                    /* quant. of scanline 8, 16, 32 */
        BitmapBitOrder(display),                                /* LSBFirst, MSBFirst */
        BitmapPad(display),                                     /* 8, 16, 32 either XY or ZPixmap */
        bmi.depth(),                                            /* depth of image */
        static_cast<int>(bmi.bytes_per_line),                   /* accelarator to next line */
        bmi.bits_per_pixel()                                    /* bits per pixel (ZPixmap) */
      };

      Status st = XInitImage(&im);
      (void)st;
      int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, bmi.width, bmi.height);
      res = XFreeGC(display, gc);
    }

//    void pixmap_put_data (os::bitmap id, cbyteptr data, const draw::bitmap_info& bmi) {
//      auto display = core::global::get_instance();
//      //auto screen = core::global::x11::get_screen();
//      auto gc = XCreateGC(display, id, 0, nullptr);

//      int byte_order_t = get_pixel_format_byte_order(bmi.pixel_format);

//      XImage im {
//        static_cast<int>(bmi.width),
//        static_cast<int>(bmi.height),                           /* size of image */
//        0,                                                      /* number of pixels offset in X direction */
//        ZPixmap,                                                /* XYBitmap, XYPixmap, ZPixmap */
//        const_cast<char*>(reinterpret_cast<const char*>(data)), /* pointer to image data */
//        byte_order_t,                                             /* data byte order, LSBFirst, MSBFirst */
//        BitmapUnit(display),                                    /* quant. of scanline 8, 16, 32 */
//        BitmapBitOrder(display),                                /* LSBFirst, MSBFirst */
//        BitmapPad(display),                                     /* 8, 16, 32 either XY or ZPixmap */
//        bmi.depth(),                                            /* depth of image */
//        static_cast<int>(bmi.bytes_per_line),                   /* accelarator to next line */
//        bmi.depth()                                            /* bits per pixel (ZPixmap) */
//      };

//      Status st = XInitImage(&im);
//      (void)st;
//      int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, bmi.width, bmi.height);
//      res = XFreeGC(display, gc);
//    }

//    void pixmap_get_data (os::bitmap id, blob& data, draw::bitmap_info& bmi) {
//      bmi = bitmap_get_info(id);
//      auto display = core::global::get_instance();
//      XImage* im = XGetImage(display, id, 0, 0, bmi.width, bmi.height, AllPlanes, ZPixmap);
//      if (im) {
//        draw::bitmap_info src_bmi = {
//          static_cast<uint32_t>(im->width),
//          static_cast<uint32_t>(im->height),
//          static_cast<uint32_t>(im->bytes_per_line),
//          get_pixel_format(im->bits_per_pixel, im->byte_order_t)
//        };
//        bmi = {
//          static_cast<uint32_t>(im->width),
//          static_cast<uint32_t>(im->height),
//          src_bmi.pixel_format
//        };
//        const size_t n = src_bmi.mem_size();
//        if (src_bmi.pixel_format != bmi.pixel_format) {
//          data.resize(n);

//          byte* src = reinterpret_cast<byte*>(im->data);
//          switch (bmi.pixel_format) {
//            case pixel_format_t::RGB: {
//              typedef draw::const_image_data<pixel_format_t::RGBA> src_data;
//              typedef draw::image_data<pixel_format_t::RGB> dst_data;
//              //<pixel_format_t::RGBA, pixel_format_t::RGB>
//              convert::format::convert(src_data(src_data::raw_type(src, n), src_bmi),
//                                    dst_data(dst_data::raw_type(data), bmi),
//                                    im->width, im->height);
//            }
//            break;
//            case pixel_format_t::RGBA: {
//                typedef draw::const_image_data<pixel_format_t::RGB> src_data;
//                typedef draw::image_data<pixel_format_t::RGBA> dst_data;
//                //<pixel_format_t::RGB, pixel_format_t::RGBA>
//                convert::format::convert(src_data(src_data::raw_type(src, n), src_bmi),
//                                      dst_data(dst_data::raw_type(data), bmi),
//                                      im->width, im->height);
//              }
//            break;
//            case pixel_format_t::BGR: {
//                typedef draw::const_image_data<pixel_format_t::ABGR> src_data;
//                typedef draw::image_data<pixel_format_t::BGR> dst_data;
//                //<pixel_format_t::BGRA, pixel_format_t::BGR>
//                convert::format::convert(src_data(src_data::raw_type(src, n), src_bmi),
//                                      dst_data(dst_data::raw_type(data), bmi),
//                                      im->width, im->height);
//              }
//            break;
//            case pixel_format_t::BGRA: {
//                typedef draw::const_image_data<pixel_format_t::BGR> src_data;
//                typedef draw::image_data<pixel_format_t::BGRA> dst_data;
//                //<pixel_format_t::BGR, pixel_format_t::BGRA>
//                convert::format::convert(src_data(src_data::raw_type(src, n), src_bmi),
//                                      dst_data(dst_data::raw_type(data), bmi),
//                                      im->width, im->height);
//              }
//            break;
//          }
//        } else {
//          data.resize(n);
//          memcpy(data.data(), im->data, n);
////          data.assign(im->data, im->data + n);
//        }
//        XDestroyImage(im);
//      } else {
//        throw std::runtime_error("get image failed");
//      }
//    }

# endif // !USE_XSHM

#endif // !GUIPP_X11

#if GUIPP_WIN

    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr data) {
      return CreateBitmap(bmi.width, bmi.height, 1, bmi.bits_per_pixel(), data);
    }

    void free_bitmap (os::bitmap& id) {
      DeleteObject(id);
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        BITMAP bmp;
        GetObject(id, sizeof (BITMAP), &bmp);
        return {
          static_cast<uint32_t>(bmp.bmWidth),
          static_cast<uint32_t>(bmp.bmHeight),
          static_cast<uint32_t>(bmp.bmWidthBytes),
          get_pixel_format(bmp.bmBitsPixel, core::os::bitmap_byte_order)
        };
      }
      return {};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      SetBitmapBits(id, (DWORD)bmi.mem_size(), data);
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      bmi = bitmap_get_info(id);
      data.resize(bmi.height * bmi.bytes_per_line);
      int ret = GetBitmapBits(id, (LONG)data.size(), data.data());
      if (ret != data.size()) {
        throw std::runtime_error("get image data failed");
      }
    }

#endif // GUIPP_WIN

#ifdef GUIPP_QT
    os::bitmap create_bitmap (const draw::bitmap_info& bmi, cbyteptr) {
      if (bmi.bits_per_pixel() == 1) {
        return new QBitmap(bmi.width, bmi.height);
      } else {
        return new QPixmap(bmi.width, bmi.height);
      }
    }

    void free_bitmap (os::bitmap& id) {
      if (id) {
        delete id;
        id = nullptr;
      }
    }

    draw::bitmap_info bitmap_get_info (const os::bitmap& id) {
      if (id) {
        pixel_format_t fmt = pixel_format_t::RGBA;
        const auto depth = id->depth();
        switch (depth) {
          case 1: fmt = pixel_format_t::BW;   break;
          case 8: fmt = pixel_format_t::GRAY; break;
          case 24: fmt = pixel_format_t::RGB; break;
//          case 32: fmt = pixel_format_t::RGBA;
        }
        return draw::bitmap_info(id->width(), id->height(), fmt);
      }
      return {};
    }

    void bitmap_put_data (os::bitmap& id, cbyteptr data, const draw::bitmap_info& bmi) {
      if (id) {
        const auto fmt = draw::bitmap_info::convert(bmi.pixel_format);
        blob tmp;
        tmp.assign(data, data + bmi.mem_size());
        if (bmi.pixel_format == pixel_format_t::BW) {
          QImage img((const uchar *)tmp.data(), bmi.width, bmi.height, bmi.bytes_per_line, fmt);
          img.setColorTable({ QColor(Qt::white).rgb(), QColor(Qt::black).rgb() });
//          img.setColorTable({ QColor(Qt::black).rgb(), QColor(Qt::white).rgb() });
          *id = QBitmap::fromImage(img);
        } else {
          *id = QPixmap::fromImage(QImage((const uchar *)tmp.data(), bmi.width, bmi.height, bmi.bytes_per_line, fmt));
        }
      }
    }

    void bitmap_get_data (const os::bitmap& id, blob& data, draw::bitmap_info& bmi) {
      if (id) {
        QImage img = id->toImage();
        const auto sz = img.size();
        auto fmt = draw::bitmap_info::convert(img.format());
        bmi = draw::bitmap_info(sz.width(), sz.height(), img.bytesPerLine(), fmt);

        const uchar* bits = img.constBits();
        data.assign(bits, bits + img.byteCount());
//        if (fmt == pixel_format_t::BW) {
//          for (byte& b : data) {
//            b = ~b;
//          }
//        }
      } else {
        bmi = {};
        data.clear();
      }
    }
#endif // GUIPP_QT

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
          bitmap_info bmi = rhs.get_info();
          create(bmi);
#ifdef GUIPP_QT
          *get_id() = *rhs.get_id();
#else
          graphics(*this).copy_from((os::drawable)rhs, core::native_rect(bmi.size()));
#endif
        } else {
          clear();
        }
      }
    }

    bool basic_map::is_valid () const {
#ifdef GUIPP_QT
      return id && !id->isNull();
#else
      return id != 0;
#endif
    }

    basic_map::operator const os::drawable () const {
#ifdef GUIPP_QT
      return id;
#else
      return get_id();
#endif
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

    core::native_size basic_map::native_size () const {
      return get_info().size();
    }

    byte basic_map::depth () const {
      return get_info().depth();
    }

    pixel_format_t basic_map::pixel_format () const {
      return get_info().pixel_format;
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

    uint32_t basic_map::calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt) {
      return draw::bitmap_info(w, 0, px_fmt).bytes_per_line;
    }

    // --------------------------------------------------------------------------
    void bitmap::create (uint32_t w, uint32_t h) {
      super::create({w, h, pixel_format_t::BW});
      if (!is_valid()) {
        throw std::runtime_error("create bitmap failed");
      }
    }

    void bitmap::copy_from (const bitmap& src_img,
                            const core::native_rect& src_rect,
                            const core::native_point& dest_pt) {
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
        const auto raw = rhs.get_data();
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
      bwmap bmp(std::move(data), std::move(bmi));
      return std::move(bmp);
    }

    bitmap::operator bwmap () const {
      return get();
    }

    // --------------------------------------------------------------------------

    void pixmap::create (uint32_t w, uint32_t h) {
#if GUIPP_WIN
      clear();
      HDC dc = GetDC(NULL);
      set_id(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#elif GUIPP_X11
      pixel_format_t px_fmt = core::global::get_device_pixel_format();
      super::create({w, h, px_fmt});
#elif GUIPP_QT
      set_id(new  QPixmap(w, h));
#endif //GUIPP_QT
      if (!is_valid()) {
        throw std::runtime_error("create pixmap failed");
      }
    }

    void pixmap::copy_from (const pixmap& src_img,
                            const core::native_rect& src_rect,
                            const core::native_point& dest_pt) {
      graphics g(*this);
      g.copy_from(src_img, src_rect, dest_pt);
    }

    void pixmap::invert () {
#ifdef GUIPP_QT
      QImage tmp = get_id()->toImage();
      tmp.invertPixels();
      *(get_id()) = QPixmap::fromImage(std::move(tmp));
#else
      switch (pixel_format()) {
        case pixel_format_t::BW:   invert<pixel_format_t::BW>();   break;
        case pixel_format_t::GRAY: invert<pixel_format_t::GRAY>(); break;
        case pixel_format_t::RGB:  invert<pixel_format_t::RGB>();  break;
        case pixel_format_t::RGBA: invert<pixel_format_t::RGBA>(); break;
        case pixel_format_t::ARGB: invert<pixel_format_t::ARGB>();  break;
        case pixel_format_t::BGR:  invert<pixel_format_t::BGR>();  break;
        case pixel_format_t::BGRA: invert<pixel_format_t::BGRA>();  break;
        case pixel_format_t::ABGR: invert<pixel_format_t::ABGR>();  break;
        default:  break;
      }
#endif //GUIPP_QT
    }

    void pixmap::put_raw (cbyteptr data, const draw::bitmap_info& bmi) {
      if (is_valid()) {
        bitmap_put_data(get_id(), data, bmi);
      }
    }

    void pixmap::get_raw (blob& data, draw::bitmap_info& bmi) const {
      if (is_valid()) {
        bitmap_get_data(get_id(), data, bmi);
      }
    }

    bwmap pixmap::get_mask (pixel::gray limit) const {
      return get<pixel_format_t::GRAY>().get_mask(limit);
    }

    byte pixmap::default_depth () {
      static byte depth =
#ifdef GUIPP_X11
          core::global::get_device_depth();
#elif GUIPP_QT
          static_cast<byte>(QPixmap(1, 1).depth());
#else
          core::global::get_device_depth();
#endif
      return depth;
    }

    pixel_format_t pixmap::default_pixel_format () {
      return core::global::get_device_pixel_format();
    }

    // --------------------------------------------------------------------------
    masked_bitmap::masked_bitmap (const pixmap& img, const bitmap& msk)
      : image(img)
      , mask(msk)
    {
      if (mask) {
        graphics g(image);
        Use<pen> use(g, color::white);
        g.copy_from(mask, core::native_rect(image.native_size()), core::native_point::zero, copy_mode::bit_and);
      }
    }

    masked_bitmap::masked_bitmap (pixmap&& img, bitmap&& msk)
      : image(std::move(img))
      , mask(std::move(msk))
    {
      if (mask) {
        graphics g(image);
        Use<pen> use(g, color::white);
        g.copy_from(mask, core::native_rect(image.native_size()), core::native_point::zero, copy_mode::bit_and);
      }
    }

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
        mask = image.get_mask({0x3f});
      }
    }

    void masked_bitmap::operator= (pixmap&& rhs) {
      image = std::move(rhs);
      if (image.is_valid()) {
        mask = image.get_mask({0x3f});
      }
    }

    void masked_bitmap::operator= (const bitmap& rhs) {
      mask = rhs;
      image.clear();
    }

    void masked_bitmap::operator= (bitmap&& rhs) {
      mask = std::move(rhs);
      image.clear();
    }

    // --------------------------------------------------------------------------
  }

}
