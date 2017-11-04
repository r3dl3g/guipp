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


namespace gui {

  namespace draw {

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
#if WIN32
        DeleteObject(get_id());
#endif
#ifdef X11
        auto display = core::global::get_instance();
        XFreePixmap(display, get_id());
#endif
        set_id(0);
      }
    }

    bitmap_info bitmap::get_info () const {
#if WIN32
      if (is_valid()) {
        BITMAP bmp;
        GetObject(get_id(), sizeof (BITMAP), &bmp);
        return { 
          static_cast<uint32_t>(bmp.bmWidth),
          static_cast<uint32_t>(bmp.bmHeight),
          static_cast<uint32_t>(bmp.bmWidthBytes),
          BPP(bmp.bmBitsPixel)
        };
      }
#endif
#ifdef X11
      if (is_valid()) {
        Window root;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, get_id(), &root, &x, &y, &w, &h, &b, &d);
        return {
          static_cast<uint32_t>(w),
          static_cast<uint32_t>(h),
          calc_bytes_per_line(w, BPP(d)),
          BPP(d)
        };
      }
#endif
      return {0, 0, 0, BPP::Undefined};
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
      HDC dc = GetDC(NULL);
      set_id(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
      create(w, h, BPP(core::global::get_device_bits_per_pixel()));
//      auto display = core::global::get_instance();
//      auto screen = core::global::get_screen();
//      int d = DefaultDepth(display, screen);
//      clear();
//      auto visual = DefaultRootWindow(display);
//      set_id(XCreatePixmap(display, visual, w, h, d));
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
#if WIN32
      set_id(CreateBitmap(w, h, 1, static_cast<byte>(bpp), NULL));
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      set_id(XCreatePixmap(display, visual, w, h, static_cast<byte>(bpp)));
#endif
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (const blob& data, const bitmap_info& bmi) {
#if WIN32
      const auto bpl = calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      if (bmi.bytes_per_line == bpl) {
        set_id(CreateBitmap(bmi.width, bmi.height, 1, bmi.depth(), data.data()));
      } else {
        // realign bits
        bitmap_info bmi2 = bmi;
        bmi2.bytes_per_line = bpl;
        blob data2(bmi2.mem_size());
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          memcpy(data2.data() + y * bpl, data.data() + y * bmi.bytes_per_line, bmi.bytes_per_line);
        }
        set_id(CreateBitmap(bmi2.width, bmi2.height, 1, bmi2.depth(), data2.data()));
      }
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
#endif
#ifdef X11
      create(bmi.size(), bmi.bits_per_pixel);
      put_data(data, bmi);
#endif
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

    template<uint32_t D, uint32_t M>
    uint32_t up_modulo (uint32_t v) {
      uint32_t r = (v + D - 1) / D;
      return r + (r % M ? M - r % M : 0);
    }

    uint32_t bitmap::calc_bytes_per_line (uint32_t w, BPP bpp) {
#ifdef WIN32
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
#endif // WIN32
#ifdef X11
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
#endif // X11
    }

    void put_bmp_data (os::bitmap id, const blob& src, const bitmap_info& bmi) {
#if WIN32
      /*int ret = */ SetBitmapBits(id, (DWORD)src.size(), src.data());
#endif
#ifdef X11
      auto display = core::global::get_instance();
      //auto screen = core::global::get_screen();
      auto gc = XCreateGC(display, id, 0, nullptr);

      XImage im;
      im.width = static_cast<int>(bmi.width);
      im.height = static_cast<int>(bmi.height);               /* size of image */
      im.xoffset = 0;                                         /* number of pixels offset in X direction */
      im.format = ZPixmap;                                    /* XYBitmap, XYPixmap, ZPixmap */
      im.data = const_cast<char*>(reinterpret_cast<const char*>(src.data()));                /* pointer to image data */
      im.byte_order = ImageByteOrder(display);                /* data byte order, LSBFirst, MSBFirst */
      im.bitmap_unit = BitmapUnit(display);                   /* quant. of scanline 8, 16, 32 */
      im.bitmap_bit_order = BitmapBitOrder(display);          /* LSBFirst, MSBFirst */
      im.bitmap_pad = BitmapPad(display);                     /* 8, 16, 32 either XY or ZPixmap */
      im.depth = bmi.depth();                                 /* depth of image */
      im.bytes_per_line = bmi.bytes_per_line;                 /* accelarator to next line */
      im.bits_per_pixel = bmi.depth();                        /* bits per pixel (ZPixmap) */
      im.red_mask = color::mask<color::part::red>::value;     /* bits in z arrangment */
      im.green_mask = color::mask<color::part::green>::value;
      im.blue_mask = color::mask<color::part::blue>::value;

      Status st = XInitImage(&im);
      int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, bmi.width, bmi.height);
      res = XFreeGC(display, gc);
#endif
    }

    bitmap_info bitmap::convert (const blob& src, const bitmap_info& bmi, blob& dst, BPP dst_bpp) {
      const auto w = bmi.width;
      const auto h = bmi.height;
      const uint32_t src_bpl = bmi.bytes_per_line;
      const uint32_t dst_bpl = calc_bytes_per_line(w, dst_bpp);

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

    void bitmap::put_data (const blob& src_data, const bitmap_info& src_bmi) {
      const bitmap_info bmi = get_info();
      const uint32_t dst_bpl = bmi.bytes_per_line;
      const uint32_t src_bpl = src_bmi.bytes_per_line;

      if ((bmi.bits_per_pixel == src_bmi.bits_per_pixel) && (dst_bpl == src_bpl)) {
        put_bmp_data(get_id(), src_data, src_bmi);
      } else {
        blob dst;
        bitmap_info dst_bmi = convert(src_data, src_bmi, dst, bmi.bits_per_pixel);
        put_bmp_data(get_id(), dst, dst_bmi);

      }
    }

#ifdef X11
    unsigned long get_planes (BPP bpp) {
      switch (bpp) {
        case BPP::BW: return 0x01;
        case BPP::GRAY: return 0xFF;
        case BPP::RGB: return 0xFFFFFF;
        case BPP::RGBA: return 0xFFFFFFFF;
      }
      return AllPlanes;
    }
#endif

    void bitmap::get_data (blob& data, bitmap_info& bmi) const {
#if WIN32
      BITMAP bmp;
      GetObject(get_id(), sizeof (BITMAP), &bmp);
      bmi = { 
        static_cast<uint32_t>(bmp.bmWidth),
        static_cast<uint32_t>(bmp.bmHeight),
        static_cast<uint32_t>(bmp.bmWidthBytes),
        BPP(bmp.bmBitsPixel)
      };
      data.resize(bmp.bmHeight * bmp.bmWidthBytes);
      //uint32_t dst_bpl = calc_bytes_per_line(bmi.width, bmi.bits_per_pixel);
      int ret = GetBitmapBits(get_id(), (LONG)data.size(), data.data());
      if (ret != data.size()) {
        throw std::runtime_error("get image data failed");
      }
#endif
#ifdef X11
      core::size sz = size();
      auto display = core::global::get_instance();
      XImage* im = XGetImage(display, get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
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
          bmi = convert(src, bmi, data, BPP(im->depth));
        } else {
          data.assign(im->data, im->data + n);
        }
        XDestroyImage(im);
      } else {
        throw std::runtime_error("get image failed");
      }
#endif
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
      copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                     0, 0, 0, 0, left, top);

      // top right
      copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                     source_right, 0, target_right, 0, right, top);

      // bottom left
      copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                     0, source_bottom, 0, target_bottom, left, bottom);

      // bottom right
      copy::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                     source_right, source_bottom, target_right, target_bottom, right, bottom);

      if ((target_right > right) && (target_bottom > bottom)) {
        const uint32_t target_inner_width = target_right - right;
        const uint32_t target_inner_height = target_bottom - bottom;
        const uint32_t source_inner_width = source_right - right;
        const uint32_t source_inner_height = source_bottom - bottom;

        // top center
        stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                          left, 0, right - left, top,
                          left, 0, target_inner_width, top);

        // bottom center
        stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                          left, source_bottom, right - left, bottom,
                          left, target_bottom, target_inner_width, bottom);

        // left center
        stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                          0, top, left, source_inner_height,
                          0, top, left, target_inner_height);

        // right center
        stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                          source_right, top, right, source_inner_height,
                          target_right, top, right, target_inner_height);

        // center
        stretch::sub<bpp>(src_data, src_bmi, dest_data, dest_bmi,
                          left, top, source_inner_width, source_inner_height,
                          left, top, target_inner_width, target_inner_height);
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
