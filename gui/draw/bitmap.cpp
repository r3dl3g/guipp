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
    uint32_t bitmap_calc_bytes_per_line (uint32_t w, BPP bpp);
    draw::bitmap_info bitmap_convert (const blob& src, const draw::bitmap_info& bmi, blob& dst, BPP dst_bpp);

    template<uint32_t D, uint32_t M>
    uint32_t up_modulo (uint32_t v) {
      uint32_t r = (v + D - 1) / D;
      return r + (r % M ? M - r % M : 0);
    }

#ifdef X11

    uint32_t bitmap_calc_bytes_per_line (uint32_t w, BPP bpp) {
      switch (bpp) {
      case BPP::BW:
        return up_modulo<8, 4>(w);
      case BPP::GRAY:
        return up_modulo<1, 4>(w);
      case BPP::RGB:
        return up_modulo<1, 4>(w * 3);
      case BPP::RGBA:
        return up_modulo<1, 4>(w * 4);
      case BPP::Undefined:
        break;
      }
      return -1;
    }


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
        bmi = {
          static_cast<uint32_t>(im->width),
          static_cast<uint32_t>(im->height),
          static_cast<uint32_t>(im->bytes_per_line),
          BPP(im->bits_per_pixel)
        };
        const size_t n = im->height * im->bytes_per_line;
        if (im->bits_per_pixel != im->depth) {
          blob src;
          src.assign(im->data, im->data + n);
          bmi = bitmap_convert(src, bmi, data, BPP(im->depth));
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

    uint32_t bitmap_calc_bytes_per_line (uint32_t w, BPP bpp) {
      switch (bpp) {
      case BPP::BW:
        return up_modulo<8, 2>(w);
      case BPP::GRAY:
        return up_modulo<1, 2>(w);
      case BPP::RGB:
        return up_modulo<1, 2>(w * 3);
      case BPP::RGBA:
        return up_modulo<1, 2>(w * 4);
      case BPP::Undefined:
        break;
      }
      return ((((w * static_cast<byte>(bpp)) + 31) & ~31) >> 3);
    }

#endif // WIN32

    draw::bitmap_info bitmap_convert (const blob& src, const draw::bitmap_info& bmi, blob& dst, BPP dst_bpp) {
      const auto w = bmi.width;
      const auto h = bmi.height;
      const uint32_t src_bpl = bmi.bytes_per_line;
      const uint32_t dst_bpl = bitmap_calc_bytes_per_line(w, dst_bpp);

      dst.resize(dst_bpl * h);

      using namespace convert;

      switch (bmi.bits_per_pixel) {
        case BPP::BW:
          switch (dst_bpp) {
            case BPP::BW:   bpp_converter<BPP::BW, BPP::BW>::convert(src, dst, w, h, src_bpl, dst_bpl);   break;
            case BPP::GRAY: bpp_converter<BPP::BW, BPP::GRAY>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::RGB:  bpp_converter<BPP::BW, BPP::RGB>::convert(src, dst, w, h, src_bpl, dst_bpl);  break;
            case BPP::RGBA: bpp_converter<BPP::BW, BPP::RGBA>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::Undefined: break;
          }
        break;
        case BPP::GRAY:
          switch (dst_bpp) {
            case BPP::BW:   bpp_converter<BPP::GRAY, BPP::BW>::convert(src, dst, w, h, src_bpl, dst_bpl);   break;
            case BPP::GRAY: bpp_converter<BPP::GRAY, BPP::GRAY>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::RGB:  bpp_converter<BPP::GRAY, BPP::RGB>::convert(src, dst, w, h, src_bpl, dst_bpl);  break;
            case BPP::RGBA: bpp_converter<BPP::GRAY, BPP::RGBA>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::Undefined: break;
          }
        break;
        case BPP::RGB:
          switch (dst_bpp) {
            case BPP::BW:   bpp_converter<BPP::RGB, BPP::BW>::convert(src, dst, w, h, src_bpl, dst_bpl);    break;
            case BPP::GRAY: bpp_converter<BPP::RGB, BPP::GRAY>::convert(src, dst, w, h, src_bpl, dst_bpl);  break;
            case BPP::RGB:  bpp_converter<BPP::RGB, BPP::RGB>::convert(src, dst, w, h, src_bpl, dst_bpl);   break;
            case BPP::RGBA: bpp_converter<BPP::RGB, BPP::RGBA>::convert(src, dst, w, h, src_bpl, dst_bpl);  break;
            case BPP::Undefined: break;
          }
        break;
        case BPP::RGBA:
          switch (dst_bpp) {
            case BPP::BW:   bpp_converter<BPP::RGBA, BPP::BW>::convert(src, dst, w, h, src_bpl, dst_bpl);   break;
            case BPP::GRAY: bpp_converter<BPP::RGBA, BPP::GRAY>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::RGB:  bpp_converter<BPP::RGBA, BPP::RGB>::convert(src, dst, w, h, src_bpl, dst_bpl);  break;
            case BPP::RGBA: bpp_converter<BPP::RGBA, BPP::RGBA>::convert(src, dst, w, h, src_bpl, dst_bpl); break;
            case BPP::Undefined: break;
          }
        break;
        case BPP::Undefined: break;
      }
      return {w, h, dst_bpl, dst_bpp};
    }

  }

  namespace draw {

    // --------------------------------------------------------------------------
    namespace stretch {

      template<>
      void row<BPP::GRAY> (const blob& src_data, blob& dest_data,
                           uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w) {
        src_offs += src_x0;
        dest_offs += dest_x0;
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = x * src_w / dest_w;
          dest_data[dest_offs + x] = src_data[src_offs + src_x];
        }
      }

      template<>
      void row<BPP::RGB> (const blob& src_data, blob& dest_data,
                          uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w) {
        src_offs += src_x0 * 3;
        dest_offs += dest_x0 * 3;
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = src_offs + (x * src_w / dest_w) * 3;
          const uint32_t dest_x = dest_offs + x * 3;
          dest_data[dest_x] = src_data[src_x];
          dest_data[dest_x + 1] = src_data[src_x + 1];
          dest_data[dest_x + 2] = src_data[src_x + 2];
        }
      }

      template<>
      void row<BPP::RGBA> (const blob& src_data, blob& dest_data,
                           uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w) {
        src_offs += src_x0 * 4;
        dest_offs += dest_x0 * 4;
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = src_offs + (x * src_w / dest_w) * 4;
          const uint32_t dest_x = dest_offs + x * 4;
          dest_data[dest_x] = src_data[src_x];
          dest_data[dest_x + 1] = src_data[src_x + 1];
          dest_data[dest_x + 2] = src_data[src_x + 2];
          dest_data[dest_x + 3] = src_data[src_x + 3];
        }
      }

      template<>
      void sub<BPP::BW> (const blob& src_data,
                         const bitmap_info& src_bmi,
                         blob& dest_data,
                         const bitmap_info& dest_bmi,
                         uint32_t src_x0, uint32_t src_y0,
                         uint32_t src_w, uint32_t src_h,
                         uint32_t dest_x0, uint32_t dest_y0,
                         uint32_t dest_w, uint32_t dest_h) {
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          const uint32_t src_y = src_y0 + y * src_h / dest_h;
          cbyteptr src = src_data.data() + (src_y * src_bmi.bytes_per_line);
          byteptr dst = dest_data.data() + dest_y0 + (y * dest_bmi.bytes_per_line);
          for (uint_fast32_t x = 0; x < dest_w; ++x) {
            const uint32_t src_x = x * src_w / dest_w;
            byte b = convert::bpp::get<BPP::BW>(src, src_x0 + src_x);
            convert::bpp::set<BPP::BW>(dst, dest_x0 + x, b);
          }
        }
      }

    } // namespace stretch

    namespace adjust {

      template<>
      void brightness_row<BPP::GRAY> (byteptr data, uint32_t offset, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[offset + x] = static_cast<byte>(std::min<int>(0xff, static_cast<int>(data[offset + x] * f)));
        }
      }

      template<>
      void brightness_row<BPP::RGB> (byteptr data, uint32_t offset, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          const uint32_t offs = offset + x * 3;
          data[offs] = static_cast<byte>(data[offs] * f);
          data[offs + 1] = static_cast<byte>(data[offs + 1] * f);
          data[offs + 2] = static_cast<byte>(data[offs + 2] * f);
        }
      }

      template<>
      void brightness_row<BPP::RGBA> (byteptr data, uint32_t offset, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          const uint32_t offs = offset + x * 4;
          data[offs] = static_cast<byte>(data[offs] * f);
          data[offs + 1] = static_cast<byte>(data[offs + 1] * f);
          data[offs + 2] = static_cast<byte>(data[offs + 2] * f);
        }
      }

    }

    // --------------------------------------------------------------------------
    bitmap_info::bitmap_info (uint32_t w, uint32_t h, BPP bpp)
      : width(w)
      , height(h)
      , bytes_per_line(bitmap_calc_bytes_per_line(w, bpp))
      , bits_per_pixel(bpp)
    {}

    // --------------------------------------------------------------------------
    void bitmap::operator= (bitmap&& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs.is_valid()) {
          std::swap(id, rhs.id);
        }
      }
    }

    void bitmap::clear () {
      if (is_valid()) {
        free_bitmap(get_id());
        set_id(0);
      }
    }

    bitmap_info bitmap::get_info () const {
      return bitmap_get_info(get_id());
    }

    core::size bitmap::size () const {
      return get_info().size();
    }

    byte bitmap::depth () const {
      return get_info().depth();
    }

    BPP bitmap::bits_per_pixel () const {
      return get_info().bits_per_pixel;
    }

    void bitmap::create_compatible (const core::size& sz) {
      create_compatible(sz.os_width(), sz.os_height());
    }

    void bitmap::create_compatible (uint32_t w, uint32_t h) {
#if WIN32
      clear();
      HDC dc = GetDC(NULL);
      set_id(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
      create(w, h, core::global::get_device_bits_per_pixel());
#endif
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (const core::size& sz, BPP bpp) {
      create(sz.os_width(), sz.os_height(), bpp);
    }

    void bitmap::create (uint32_t w, uint32_t h, BPP bpp) {
      auto sz = size();
      if ((sz.os_width() == w) && (sz.os_height() == h) && (bits_per_pixel() == bpp)) {
        return;
      }
      clear();
      set_id(create_bitmap({w, h, bpp}));
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (const blob& data, const bitmap_info& bmi) {
      const auto bpl = calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      if (bmi.bytes_per_line == bpl) {
        set_id(create_bitmap(bmi, data.data()));
      } else {
        // realign bits
        bitmap_info bmi2 = bmi;
        bmi2.bytes_per_line = bpl;
        blob data2(bmi2.mem_size());
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          memcpy(data2.data() + y * bpl, data.data() + y * bmi.bytes_per_line, bmi.bytes_per_line);
        }
        set_id(create_bitmap(bmi2, data2.data()));
      }
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::copy_from (const bitmap& rhs) {
      if (rhs) {
        bitmap_info bmi;
        blob data;
        rhs.get_data(data, bmi);
        create(bmi.size(), bmi.bits_per_pixel);
        put_data(data, bmi);
      } else {
        clear();
      }
    }

    void bitmap::put (const bitmap& rhs) {
      if (rhs) {
        bitmap_info bmi;
        blob data;
        rhs.get_data(data, bmi);
        put_data(data, bmi);
      } else {
        clear();
      }
    }

    uint32_t bitmap::calc_bytes_per_line (uint32_t w, BPP bpp) {
      return bitmap_calc_bytes_per_line(w, bpp);
    }

    bitmap_info bitmap::convert (const blob& src, const bitmap_info& bmi, blob& dst, BPP dst_bpp) {
      return bitmap_convert(src, bmi, dst, dst_bpp);
    }

    void bitmap::put_data (const blob& src_data, const bitmap_info& src_bmi) {
      const bitmap_info bmi = get_info();
      const uint32_t dst_bpl = bmi.bytes_per_line;
      const uint32_t src_bpl = src_bmi.bytes_per_line;

      if ((bmi.bits_per_pixel == src_bmi.bits_per_pixel) && (dst_bpl == src_bpl)) {
        bitmap_put_data(get_id(), src_data.data(), src_bmi);
      } else {
        blob dst;
        bitmap_info dst_bmi = convert(src_data, src_bmi, dst, bmi.bits_per_pixel);
        bitmap_put_data(get_id(), dst.data(), dst_bmi);

      }
    }

    void bitmap::get_data (blob& data, bitmap_info& bmi) const {
      bitmap_get_data(get_id(), data, bmi);
    }

    void bitmap::invert () {
      bitmap_info bmi;
      blob data;
      get_data(data, bmi);
      for (auto& c : data) {
        c = ~c;
      }
      put_data(data, bmi);
    }

    // --------------------------------------------------------------------------
    void bitmap::copy_from (const bitmap& src_img,
                            const core::rectangle& src_rect,
                            const core::point& dest_pt) {
      blob src_data,  dest_data;
      bitmap_info src_bmi = src_img.get_info();
      bitmap_info dest_bmi;

      if (id) {
        get_data(dest_data, dest_bmi);

        if (src_bmi.bits_per_pixel == dest_bmi.bits_per_pixel) {
          src_img.get_data(src_data, src_bmi);
        } else {
          bitmap tmp;
          tmp.create(src_img.size(), dest_bmi.bits_per_pixel);
          tmp.put(src_img);
          tmp.get_data(src_data, src_bmi);
        }
      } else {
        src_img.get_data(src_data, src_bmi);
        const uint32_t w = src_rect.os_width();
        const uint32_t h = src_rect.os_height();
        dest_bmi = {w, h, calc_bytes_per_line(w, src_bmi.bits_per_pixel), src_bmi.bits_per_pixel};
        dest_data.resize(dest_bmi.mem_size());
      }

      const uint32_t src_x0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width, roundup<uint32_t>(src_rect.x())));
      const uint32_t src_y0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height, roundup<uint32_t>(src_rect.y())));
      const uint32_t src_w = std::min<uint32_t>(src_bmi.width - src_x0, roundup<uint32_t>(src_rect.width()));
      const uint32_t src_h = std::min<uint32_t>(src_bmi.height - src_y0, roundup<uint32_t>(src_rect.height()));

      const uint32_t dest_x0 = std::max<uint32_t>(0, std::min<uint32_t>(dest_bmi.width, roundup<uint32_t>(dest_pt.x())));
      const uint32_t dest_y0 = std::max<uint32_t>(0, std::min<uint32_t>(dest_bmi.height, roundup<uint32_t>(dest_pt.y())));
      const uint32_t dest_w = std::min<uint32_t>(dest_bmi.width - dest_x0, src_w);
      const uint32_t dest_h = std::min<uint32_t>(dest_bmi.height - dest_y0, src_h);

      if ((dest_w < 1) ||(dest_h < 1)) {
        return;
      }

      switch (dest_bmi.bits_per_pixel) {
        case BPP::BW:
          copy::sub<BPP::BW>(src_data, src_bmi, dest_data, dest_bmi,
                             src_x0, src_y0, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::GRAY:
          copy::sub<BPP::GRAY>(src_data, src_bmi, dest_data, dest_bmi,
                               src_x0, src_y0, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::RGB:
          copy::sub<BPP::RGB>(src_data, src_bmi, dest_data, dest_bmi,
                              src_x0, src_y0, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::RGBA:
          copy::sub<BPP::RGBA>(src_data, src_bmi, dest_data, dest_bmi,
                               src_x0, src_y0, dest_x0, dest_y0, dest_w, dest_h);
        break;
      }

      put_data(dest_data, dest_bmi);
    }

    // --------------------------------------------------------------------------
    void bitmap::stretch_from (const bitmap& src_img,
                               const core::rectangle& src_rect,
                               const core::rectangle& dest_rect) {
      blob src_data,  dest_data;
      bitmap_info src_bmi = src_img.get_info();
      bitmap_info dest_bmi;

      get_data(dest_data, dest_bmi);

      if (src_bmi.bits_per_pixel == dest_bmi.bits_per_pixel) {
        src_img.get_data(src_data, src_bmi);
      } else {
        bitmap tmp;
        tmp.create(src_img.size(), dest_bmi.bits_per_pixel);
        tmp.put(src_img);
        tmp.get_data(src_data, src_bmi);
      }

      const uint32_t src_x0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width, roundup<uint32_t>(src_rect.x())));
      const uint32_t src_y0 = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height, roundup<uint32_t>(src_rect.y())));
      const uint32_t src_w = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.width - src_x0, roundup<uint32_t>(src_rect.width())));
      const uint32_t src_h = std::max<uint32_t>(0, std::min<uint32_t>(src_bmi.height - src_y0, roundup<uint32_t>(src_rect.height())));

      const uint32_t dest_x0 = std::max<uint32_t>(0, std::min<uint32_t>(dest_bmi.width, roundup<uint32_t>(dest_rect.x())));
      const uint32_t dest_y0 = std::max<uint32_t>(0, std::min<uint32_t>(dest_bmi.height, roundup<uint32_t>(dest_rect.y())));
      const uint32_t dest_w = std::min<uint32_t>(dest_bmi.width - dest_x0, roundup<uint32_t>(dest_rect.width()));
      const uint32_t dest_h = std::min<uint32_t>(dest_bmi.height - dest_y0, roundup<uint32_t>(dest_rect.height()));

      if ((dest_w < 1) ||(dest_h < 1)) {
        return;
      }

      switch (dest_bmi.bits_per_pixel) {
        case BPP::BW:
          stretch::sub<BPP::BW>(src_data, src_bmi, dest_data, dest_bmi, src_x0, src_y0, src_w, src_h, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::GRAY:
          stretch::sub<BPP::GRAY>(src_data, src_bmi, dest_data, dest_bmi, src_x0, src_y0, src_w, src_h, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::RGB:
          stretch::sub<BPP::RGB>(src_data, src_bmi, dest_data, dest_bmi, src_x0, src_y0, src_w, src_h, dest_x0, dest_y0, dest_w, dest_h);
        break;
        case BPP::RGBA:
          stretch::sub<BPP::RGBA>(src_data, src_bmi, dest_data, dest_bmi, src_x0, src_y0, src_w, src_h, dest_x0, dest_y0, dest_w, dest_h);
        break;
      }

      put_data(dest_data, dest_bmi);
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

    void masked_bitmap::operator= (const memmap& rhs) {
      image = rhs;
      if (image.is_valid()) {
        mask.create(image.size());
        mask.put(image);
      }
    }

    void masked_bitmap::operator= (memmap&& rhs) {
      image = std::move(rhs);
      if (image.is_valid()) {
        mask.create(image.size());
        mask.put(image);
      }
    }

    // --------------------------------------------------------------------------
    frame_image::frame_image (const core::rectangle& r, const bitmap& img, uint32_t edge)
      : rect(r)
      , img(img)
      , left(edge)
      , top(edge)
      , right(edge)
      , bottom(edge)
    {}

    frame_image::frame_image (const core::rectangle& r, const bitmap& img, uint32_t horizontal, uint32_t vertical)
      : rect(r)
      , img(img)
      , left(horizontal)
      , top(vertical)
      , right(horizontal)
      , bottom(horizontal)
    {}

    frame_image::frame_image (const core::rectangle& r, const bitmap& img, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom)
      : rect(r)
      , img(img)
      , left(left)
      , top(top)
      , right(right)
      , bottom(bottom)
    {}

    template<BPP bpp>
    void copy_frame_image (const blob& src_data, blob& dest_data,
                           const bitmap_info& src_bmi, const bitmap_info& dest_bmi,
                           uint32_t left, uint32_t top, uint32_t right, uint32_t bottom) {

      const uint32_t target_right = dest_bmi.width - right;
      const uint32_t target_bottom = dest_bmi.height - bottom;
      const uint32_t source_right = src_bmi.width - right;
      const uint32_t source_bottom = src_bmi.height - bottom;

      // top left
      if (top && left) {
        copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                       0, 0, 0, 0, left, top);
      }

      // top right
      if (top && right) {
        copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                       source_right, 0, target_right, 0, right, top);
      }

      // bottom left
      if (bottom && left) {
        copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                       0, source_bottom, 0, target_bottom, left, bottom);
      }

      if (bottom && right) {
        // bottom right
        copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                       source_right, source_bottom, target_right, target_bottom, right, bottom);
      }

      if ((target_right > left) && (target_bottom > top) && (source_right >= left) && (source_bottom >= top)) {
        const uint32_t target_inner_width = target_right - left;
        const uint32_t target_inner_height = target_bottom - top;
        const uint32_t source_inner_width = source_right - left;
        const uint32_t source_inner_height = source_bottom - top;

        // top center
        if (top && target_inner_width) {
          stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                            left, 0, source_inner_width, top,
                            left, 0, target_inner_width, top);
        }

        // bottom center
        if (bottom && target_inner_width) {
          stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                            left, source_bottom, source_inner_width, bottom,
                            left, target_bottom, target_inner_width, bottom);
        }

        // left center
        if (left && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                            0, top, left, source_inner_height,
                            0, top, left, target_inner_height);
        }

        // right center
        if (right && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                            source_right, top, right, source_inner_height,
                            target_right, top, right, target_inner_height);
        }

        // center
        if (target_inner_width && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                            left, top, source_inner_width, source_inner_height,
                            left, top, target_inner_width, target_inner_height);
        }
      }
    }

    void frame_image::operator() (const graphics& g, const core::point& pt) const {
      blob src_data;
      bitmap_info src_bmi;
      img.get_data(src_data, src_bmi);

      if (rect.size() <= core::size::two) {
        return;
      }

      const BPP bpp = src_bmi.bits_per_pixel;
      const uint32_t w = roundup<uint32_t>(rect.width());
      const uint32_t h = roundup<uint32_t>(rect.height());
      const uint32_t bpl = bitmap::calc_bytes_per_line(w, bpp);
      bitmap_info dest_bmi = {w, h, bpl, bpp};
      blob dest_data(dest_bmi.mem_size());

      const uint32_t width = roundup<uint32_t>(rect.width());
      const uint32_t height = roundup<uint32_t>(rect.height());

      const uint32_t l = std::min(left, width / 2);
      const uint32_t r = std::min(right, width / 2);
      const uint32_t t = std::min(top, height / 2);
      const uint32_t b = std::min(bottom, height / 2);

      switch (bpp) {
        case BPP::BW: copy_frame_image<BPP::BW>(src_data, dest_data, src_bmi, dest_bmi, l, t, r, b); break;
        case BPP::GRAY: copy_frame_image<BPP::GRAY>(src_data, dest_data, src_bmi, dest_bmi, l, t, r, b); break;
        case BPP::RGB: copy_frame_image<BPP::RGB>(src_data, dest_data, src_bmi, dest_bmi, l, t, r, b); break;
        case BPP::RGBA: copy_frame_image<BPP::RGBA>(src_data, dest_data, src_bmi, dest_bmi, l, t, r, b); break;
      }

      bitmap buffer;
      buffer.create(dest_data, dest_bmi);

      g.copy_from(buffer, pt);
    }
    // --------------------------------------------------------------------------
  }

}
