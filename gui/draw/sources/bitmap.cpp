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
#include <map>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "logger.h"
#include "bitmap.h"
#include "converter.h"

namespace gui {

  namespace draw {

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

    core::size bitmap::size () const {
#if WIN32
      if (is_valid()) {
        BITMAP bmp;
        GetObject(get_id(), sizeof (BITMAP), &bmp);
        return {(core::size_type)bmp.bmWidth, (core::size_type)bmp.bmHeight};
      }
#endif
#ifdef X11
      if (is_valid()) {
        Window root;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, get_id(), &root, &x, &y, &w, &h, &b, &d);
        return {(core::size_type)w, (core::size_type)h};
      }
#endif
      return core::size::zero;
    }

    int bitmap::depth () const {
#if WIN32
      if (is_valid()) {
        BITMAP bmp;
        GetObject(get_id(), sizeof (BITMAP), &bmp);
        return bmp.bmBitsPixel;
      }
#endif
#ifdef X11
      if (is_valid()) {
        Window root;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, get_id(), &root, &x, &y, &w, &h, &b, &d);
        return d;
      }
#endif
      return 0;
    }

    BPP bitmap::bits_per_pixel () const {
      return BPP(depth());
    }

    void bitmap::create_compatible (int w, int h) {
#if WIN32
      HDC dc = GetDC(NULL);
      set_id(CreateCompatibleBitmap(dc, w, h));
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
      clear();
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto visual = DefaultRootWindow(display);
      int d = DefaultDepth(display, screen);
      set_id(XCreatePixmap(display, visual, w, h, d));
#endif
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (int w, int h, BPP bpp) {
      auto sz = size();
      if ((sz.os_width() == w) && (sz.os_height() == h) && (bits_per_pixel() == bpp)) {
        return;
      }
      clear();
#if WIN32
      set_id(CreateBitmap(w, h, 1, static_cast<int>(bpp), NULL));
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      set_id(XCreatePixmap(display, visual, w, h, static_cast<int>(bpp)));
#endif
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (const std::vector<char>& data, int w, int h, int bpl, BPP bpp) {
#if WIN32
      set_id(CreateBitmap(w, h, 1, static_cast<int>(bpp), data.data()));
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
#endif
#ifdef X11
      create(w, h, bpp);
      put_data(data, w, h, bpl, bpp);
#endif
    }

    void bitmap::copy_from (const bitmap& rhs) {
      if (rhs) {
        int w, h, bpl;
        BPP bpp;
        std::vector<char> data;
        rhs.get_data(data, w, h, bpl, bpp);
        create(w, h, bpp);
        put_data(data, w, h, bpl, bpp);
      } else {
        clear();
      }
    }

    void bitmap::put (const bitmap& rhs) {
      if (rhs) {
        int w, h, bpl;
        BPP bpp;
        std::vector<char> data;
        rhs.get_data(data, w, h, bpl, bpp);
        put_data(data, w, h, bpl, bpp);
      } else {
        clear();
      }
    }

    template<int D, int M>
    int up_modulo (int v) {
      int r = (v + D - 1) / D;
      return r + (r % M ? M - r % M : 0);
    }

    int bitmap::calc_bytes_per_line (int w, BPP bpp) {
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
      return ((((w * static_cast<int>(bpp)) + 31) & ~31) >> 3);
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
      }
      return -1;
#endif // X11
    }

    void put_bmp_data (os::bitmap id, const std::vector<char>& src, int w, int h, int bpl, BPP bpp) {
#if WIN32
      /*int ret = */ SetBitmapBits(id, (DWORD)src.size(), src.data());
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
//      auto gc = DefaultGC(display, screen);
      auto gc = XCreateGC(display, id, 0, nullptr);

      XImage im = {
        w, h,                             /* size of image */
        0,                                /* number of pixels offset in X direction */
        ZPixmap,                          /* XYBitmap, XYPixmap, ZPixmap */
        const_cast<char*>(src.data()),    /* pointer to image data */
        ImageByteOrder(display),          /* data byte order, LSBFirst, MSBFirst */
        BitmapUnit(display),              /* quant. of scanline 8, 16, 32 */
        BitmapBitOrder(display),          /* LSBFirst, MSBFirst */
        BitmapPad(display),               /* 8, 16, 32 either XY or ZPixmap */
        static_cast<int>(bpp),            /* depth of image */
        bpl,                              /* accelarator to next line */
        static_cast<int>(bpp),            /* bits per pixel (ZPixmap) */
        0                                 /* bits in z arrangment */
      };

      Status st = XInitImage(&im);
      int res = XPutImage(display, id, gc, &im, 0, 0, 0, 0, w, h);
      res = XFreeGC(display, gc);
#endif
    }

    void bitmap::put_data (const std::vector<char>& src, int w, int h, int bpl, BPP bpp) {
      BPP dst_bpp = bits_per_pixel();
      int dst_bpl = calc_bytes_per_line(w, dst_bpp);
      if ((dst_bpp == bpp) && (dst_bpl == bpl)) {
        put_bmp_data(get_id(), src, w, h, bpl, bpp);
      } else {
        std::vector<char> dst;
        dst.resize(dst_bpl * h);

        using namespace convert;

        switch (bpp) {
        case BPP::BW:
          switch (dst_bpp) {
          case BPP::BW:   bpp_converter<BPP::BW, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::GRAY: bpp_converter<BPP::BW, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGB:  bpp_converter<BPP::BW, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGBA: bpp_converter<BPP::BW, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::Undefined: break;
          }
          break;
        case BPP::GRAY:
          switch (dst_bpp) {
          case BPP::BW:   bpp_converter<BPP::GRAY, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::GRAY: bpp_converter<BPP::GRAY, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGB:  bpp_converter<BPP::GRAY, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGBA: bpp_converter<BPP::GRAY, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::Undefined: break;
          }
          break;
        case BPP::RGB:
          switch (dst_bpp) {
          case BPP::BW:   bpp_converter<BPP::RGB, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::GRAY: bpp_converter<BPP::RGB, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGB:  bpp_converter<BPP::RGB, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGBA: bpp_converter<BPP::RGB, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::Undefined: break;
          }
          break;
        case BPP::RGBA:
          switch (dst_bpp) {
          case BPP::BW:   bpp_converter<BPP::RGBA, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::GRAY: bpp_converter<BPP::RGBA, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGB:  bpp_converter<BPP::RGBA, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::RGBA: bpp_converter<BPP::RGBA, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl);
            break;
          case BPP::Undefined: break;
          }
          break;
        case BPP::Undefined: break;
        }

        put_bmp_data(get_id(), dst, w, h, dst_bpl, dst_bpp);

      }
    }

    void bitmap::get_data (std::vector<char>& data, int& w, int& h, int& bpl, BPP& bpp) const {
#if WIN32
      BITMAP bmp;
      GetObject(get_id(), sizeof (BITMAP), &bmp);
      w = bmp.bmWidth;
      h = bmp.bmHeight;
      bpl = bmp.bmWidthBytes;
      bpp = BPP(bmp.bmBitsPixel);
      data.resize(bpl * h);
      /*int dst_bpl = */ calc_bytes_per_line(w, bpp);
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
        int n = im->height * im->bytes_per_line;
        data.assign(im->data, im->data + n);
        w = im->width;
        h = im->height;
        bpl = im->bytes_per_line;
        bpp = BPP(im->bits_per_pixel);
        XDestroyImage(im);
      } else {
        throw std::runtime_error("get image failed");
      }
#endif
    }

    void bitmap::invert () {
      int w, h, bpl;
      BPP bpp;
      std::vector<char> data;
      get_data(data, w, h, bpl, bpp);
      for (char& c : data) {
        c = ~c;
      }
      put_data(data, w, h, bpl, bpp);
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
  }

}
