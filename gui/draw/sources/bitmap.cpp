/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "logger.h"
#include "bitmap.h"
#include "converter.h"


namespace gui {

  namespace draw {

    bitmap::bitmap (const bitmap& rhs)
      : id(0)
    {
      operator=(rhs);
    }

    bitmap::bitmap (bitmap&& rhs)
      : id(std::move(rhs.id))
    {
      rhs.id = 0;
    }

    void bitmap::operator= (const bitmap& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs) {
          core::size sz = rhs.size();
          create(sz.os_width(), sz.os_height(), rhs.bpp());
          put(rhs);
        }
      }
    }

    void bitmap::operator= (bitmap&& rhs) {
      if (&rhs != this) {
        clear();
        id = std::move(rhs.id);
        rhs.id = 0;
      }
    }

    void bitmap::clear () {
      if (id) {
#if WIN32
        DeleteObject(id);
#endif
#ifdef X11
        auto display = core::global::get_instance();
        int res = XFreePixmap(display, id);
#endif
        id = 0;
      }
    }

    core::size bitmap::size () const {
#if WIN32
      if (id) {
        BITMAP bmp;
        GetObject(id, sizeof(BITMAP), &bmp);
        return { (core::size_type)bmp.bmWidth, (core::size_type)bmp.bmHeight };
      }
#endif
#ifdef X11
      if (id) {
        Window root;
        int x, y;
        unsigned int w, h, b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        return { (core::size_type)w, (core::size_type)h };
      }
#endif
      return core::size::zero;
    }

    int bitmap::depth () const {
#if WIN32
      if (id) {
        BITMAP bmp;
        GetObject(id, sizeof(BITMAP), &bmp);
        return bmp.bmBitsPixel;
      }
#endif
#ifdef X11
      unsigned int w = 0, h = 0;
      if (id) {
        Window root;
        int x, y;
        unsigned int b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
        return d;
      }
#endif
      return 0;
    }

    BPP bitmap::bpp () const {
      return BPP(depth());
    }


    void bitmap::create (int w, int h) {
#if WIN32
      HDC dc = GetDC(NULL);
      id = CreateCompatibleBitmap(dc, w, h);
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
      clear();
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto visual = DefaultRootWindow(display);
      id = XCreatePixmap(display, visual, w, h, DefaultDepth(display, screen));
#endif
      if (!id) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (int w, int h, BPP bpp) {
      clear();
#if WIN32
      id = CreateBitmap(w, h, 1, static_cast<int>(bpp), NULL);
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto visual = DefaultRootWindow(display);
      id = XCreatePixmap(display, visual, w, h, bpp);
#endif
      if (!id) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (const std::vector<char>& data, int w, int h, int bpl, BPP bpp) {
#if WIN32
      id = CreateBitmap(w, h, 1, static_cast<int>(bpp), data.data());
      if (!id) {
        throw std::runtime_error("create image failed");
      }
#endif
#ifdef X11
      create(w, h, bpp);
      put_data(data, w, h, bpl, bpp);
#endif
    }

    void bitmap::copy_from (const bitmap& rhs) {
      int w, h, bpl;
      BPP bpp;
      std::vector<char> data;
      rhs.get_data(data, w, h, bpl, bpp);
      create(w, h);
      put_data(data, w, h, bpl, bpp);
    }

    void bitmap::put (const bitmap& rhs) {
      int w, h, bpl;
      BPP bpp;
      std::vector<char> data;
      rhs.get_data(data, w, h, bpl, bpp);
      put_data(data, w, h, bpl, bpp);
    }

    template<int D, int M>
    int up_modulo (int v) {
      int r = (v + D - 1) / D;
      return r + r % M;
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
      }
      return ((((w * static_cast<int>(bpp)) + 31) & ~31) >> 3);
#endif // WIN32
#ifdef X11
      switch (bpp) {
        case BPP::BW:
          return up_modulo<8, 16>(w);
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
      int ret = SetBitmapBits(id, (DWORD)src.size(), src.data());
#endif
#ifdef X11
      auto display = core::global::get_instance();
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

      XInitImage(&im);
      XPutImage(display, id, gc, &im, 0, 0, 0, 0, w, h);
      XFreeGC(display, gc);
#endif
    }

    void bitmap::put_data (const std::vector<char>& src, int w, int h, int bpl, BPP src_bpp) {
      BPP dst_bpp = bpp();
      if (dst_bpp == src_bpp) {
        put_bmp_data(id, src, w, h, bpl, src_bpp);
      } else {
        std::vector<char> dst;
        int dst_bpl = calc_bytes_per_line(w, dst_bpp);
        dst.resize(dst_bpl * h);

        using namespace convert;

        switch (src_bpp) {
          case BPP::BW:
            switch (dst_bpp) {
              case BPP::BW:   converter<BPP::BW, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGB:  converter<BPP::BW, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGBA: converter<BPP::BW, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case BPP::GRAY:
            switch (dst_bpp) {
              case BPP::BW:   converter<BPP::GRAY, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGB:  converter<BPP::GRAY, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGBA: converter<BPP::GRAY, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case BPP::RGB:
            switch (dst_bpp) {
              case BPP::BW:   converter<BPP::RGB, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::GRAY: converter<BPP::RGB, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGBA: converter<BPP::RGB, BPP::RGBA>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case BPP::RGBA:
            switch (dst_bpp) {
              case BPP::BW:   converter<BPP::RGBA, BPP::BW>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::GRAY: converter<BPP::RGBA, BPP::GRAY>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case BPP::RGB:  converter<BPP::RGBA, BPP::RGB>::convert(src, dst, w, h, bpl, dst_bpl); break;
              }
          break;
        }

        put_bmp_data(id, dst, w, h, dst_bpl, dst_bpp);

      }
    }

    void bitmap::get_data (std::vector<char>& data, int& w, int& h, int& bpl, BPP& bpp) const {
#if WIN32
      BITMAP bmp;
      GetObject(id, sizeof(BITMAP), &bmp);
      w = bmp.bmWidth;
      h = bmp.bmHeight;
      bpl = bmp.bmWidthBytes;
      bpp = BPP(bmp.bmBitsPixel);
      data.resize(bpl * h);
      int dst_bpl = calc_bytes_per_line(w, bpp);
      int ret = GetBitmapBits(id, (LONG)data.size(), data.data());
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
        bpp = im->bits_per_pixel;
        XDestroyImage(im);
      }
      else {
        throw std::runtime_error("get image failed");
      }
#endif
    }

    transparent_bitmap::transparent_bitmap (const transparent_bitmap& rhs)
      : super(rhs)
      , mask(rhs.mask)
    {}

    void transparent_bitmap::operator= (const transparent_bitmap& rhs) {
      if (&rhs != this) {
        super::operator=(rhs);
        mask = rhs.mask;
      }
    }

    transparent_bitmap::transparent_bitmap (transparent_bitmap&& rhs)
      : super(std::move(rhs))
      , mask(std::move(rhs.mask))
    {}

    void transparent_bitmap::operator= (transparent_bitmap&& rhs) {
      if (&rhs != this) {
        super::operator =(std::move(rhs));
        mask = std::move(rhs.mask);
      }
    }

    transparent_bitmap::transparent_bitmap (const memmap& b)
      : super(b)
    {
      if (is_valid()) {
        mask.create(size());
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator= (const memmap& bmp) {
      bitmap::operator=(bmp);
      if (is_valid()) {
        mask.create(size());
        mask.put(*this);
      }
    }

    transparent_bitmap::transparent_bitmap (memmap&& b)
      : super(std::move(b))
    {
      if (is_valid()) {
        mask.create(size());
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator= (memmap&& bmp) {
      bitmap::operator=(std::move(bmp));
      if (is_valid()) {
        mask.create(size());
        mask.put(*this);
      }
    }
  }

}
