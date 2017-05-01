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
#include "bitmap.h"
#include "graphics.h"
#include "converter.h"


namespace gui {

  namespace draw {

    bitmap::bitmap (const bitmap& rhs)
      : id(0)
    {
      operator=(rhs);
    }

    void bitmap::operator= (const bitmap& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs) {
          create(rhs.size(), rhs.depth());
          put(rhs);
        }
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
        return core::size(static_cast<core::size_type>(bmp.bmWidth), static_cast<core::size_type>(bmp.bmHeight));
      }
      return core::size::zero;
#endif
#ifdef X11
      unsigned int w = 0, h = 0;
      if (id) {
        Window root;
        int x, y;
        unsigned int b, d;
        auto display = core::global::get_instance();
        Status st = XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
      }
      return core::size(w, h);
#endif
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

    void bitmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    void bitmap::create (const core::size& sz, int bpp) {
      create(sz.os_width(), sz.os_height(), bpp);
    }

    void bitmap::create (int w, int h) {
      clear();
#if WIN32
      HDC dc = GetDC(NULL);
      id = CreateCompatibleBitmap(dc, w, h);
      ReleaseDC(NULL, dc);
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto visual = DefaultRootWindow(display);
      id = XCreatePixmap(display, visual, w, h, DefaultDepth(display, screen));
#endif
      if (!id) {
        throw std::runtime_error("create image failed");
      }
    }

    void bitmap::create (int w, int h, int bpp) {
      clear();
#if WIN32
      HDC dc = GetDC(NULL);
      id = CreateBitmap(w, h, 1, bpp, NULL);
      ReleaseDC(NULL, dc);
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

    void bitmap::create (const std::vector<char>& data, int w, int h, int bpl, int bpp) {
#if WIN32
      id = CreateBitmap(w, h, 1, bpp, data.data());
      if (!id) {
        throw std::runtime_error("create image failed");
      }
#endif
#ifdef X11
      create(w, h, bpp);
      put(data, w, h, bpl, bpp);
#endif
    }

    void bitmap::create_compatible (const bitmap& rhs) {
      int w, h, bpl, bpp;
      std::vector<char> data;
      rhs.get_data(data, w, h, bpl, bpp);
      create(w, h);
      put(data, w, h, bpl, bpp);
    }

    void bitmap::make_compatible () {
      if (depth() != core::global::get_device_bits_per_pixel()) {
        create_compatible(*this);
      }
    }

    template<int M>
    int up_modulo (int v) {
      return v + v % M;
    }

    int bitmap::calc_bytes_per_line (int w, int bpp) {
      switch (bpp) {
        case 1:
          return up_modulo<8>(w / 8);
        case 8:
          return up_modulo<4>(w);
        case 24:
          return up_modulo<4>(w * 3);
        case 32:
          return up_modulo<4>(w * 4);
      }
      return -1;
    }

    void bitmap::put (const std::vector<char>& src, int w, int h, int bpl, int bpp) {
#if WIN32
      HDC gdc = GetDC(NULL);
      HDC gc = CreateCompatibleDC(gdc);
      ReleaseDC(NULL, gdc);

      SelectObject(gc, id);

      int usage = bpp > 1 ? DIB_RGB_COLORS : DIB_PAL_COLORS;
      BITMAPINFOHEADER bi = {
        sizeof(BITMAPINFOHEADER),
        w, -h, 1, (WORD)bpp, BI_RGB, (DWORD)(h * bpl), 0, 0, (bpp == 8) ? 256u : 0, 0
      };

      int ret = StretchDIBits(gc, 0, 0, w, h, 0, 0, w, h, src.data(), reinterpret_cast<BITMAPINFO*>(&bi), usage, SRCCOPY);
      if (ret != h) {
        throw std::runtime_error("put image data failed");
      }
      DeleteDC(gc);
#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto gc = XCreateGC(display, id, 0, nullptr);

      int my_bpp = depth();
      if (my_bpp == bpp) {

        XImage im = {
            w, h,                             /* size of image */
            0,                                /* number of pixels offset in X direction */
            ZPixmap,                          /* XYBitmap, XYPixmap, ZPixmap */
            const_cast<char*>(src.data()),   /* pointer to image data */
            ImageByteOrder(display),          /* data byte order, LSBFirst, MSBFirst */
            BitmapUnit(display),              /* quant. of scanline 8, 16, 32 */
            BitmapBitOrder(display),          /* LSBFirst, MSBFirst */
            BitmapPad(display),               /* 8, 16, 32 either XY or ZPixmap */
            bpp,                              /* depth of image */
            bpl,                              /* accelarator to next line */
            bpp,                              /* bits per pixel (ZPixmap) */
            0                                 /* bits in z arrangment */
        };

        XInitImage(&im);
        XPutImage(display, id, gc, &im, 0, 0, 0, 0, w, h);

      } else {
        std::vector<char> dst;
        int dst_bpl = calc_bytes_per_line(w, my_bpp);
        dst.resize(dst_bpl * h);

        using namespace convert;

        switch (bpp) {
          case 1:
            switch (my_bpp) {
              case 8:  converter<1, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<1, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<1, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 8:
            switch (my_bpp) {
              case 1:  converter<8, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<8, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<8, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 24:
            switch (my_bpp) {
              case 1:  converter<24, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 8:  converter<24, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<24, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 32:
            switch (my_bpp) {
              case 1:  converter<32, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 8:  converter<32, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<32, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              }
          break;
        }

        XImage im = {
            w, h,                           /* size of image */
            0,                              /* number of pixels offset in X direction */
            ZPixmap,                        /* XYBitmap, XYPixmap, ZPixmap */
            const_cast<char*>(dst.data()),  /* pointer to image data */
            ImageByteOrder(display),        /* data byte order, LSBFirst, MSBFirst */
            BitmapUnit(display),            /* quant. of scanline 8, 16, 32 */
            BitmapBitOrder(display),        /* LSBFirst, MSBFirst */
            BitmapPad(display),             /* 8, 16, 32 either XY or ZPixmap */
            my_bpp,                         /* depth of image */
            dst_bpl,                        /* accelarator to next line */
            my_bpp,                         /* bits per pixel (ZPixmap) */
            0                               /* bits in z arrangment */
        };

        XInitImage(&im);
        XPutImage(display, id, gc, &im, 0, 0, 0, 0, w, h);
      }
      XFreeGC(display, gc);
#endif
    }

    void bitmap::put (const bitmap& rhs) {
      int w, h, bpl, bpp;
      std::vector<char> data;
      rhs.get_data(data, w, h, bpl, bpp);
      put(data, w, h, bpl, bpp);
    }

    struct save_bitmapinfo : public BITMAPINFO {
      RGBQUAD moreColors[255];
    };

    void bitmap::get_data (std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) const {
#if WIN32
      save_bitmapinfo bi;
      memset(&bi, 0, sizeof(save_bitmapinfo));
      bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

      HDC gdc = GetDC(NULL);
      HDC gc = CreateCompatibleDC(gdc);
      ReleaseDC(NULL, gdc);

      HGDIOBJ old = SelectObject(gc, id);

      int ret = GetDIBits(gc, id, 0, 0, nullptr, &bi, DIB_RGB_COLORS);
      if (!ret) {
        throw std::runtime_error("get image info failed");
      }

      const int n = bi.bmiHeader.biSizeImage;
      w = bi.bmiHeader.biWidth;
      h = bi.bmiHeader.biHeight;
      bpl = n / h;
      bpp = bi.bmiHeader.biBitCount;
      bi.bmiHeader.biClrUsed = (bpp == 8) ? 256 : 0;
      int usage = bpp > 1 ? DIB_RGB_COLORS : DIB_PAL_COLORS;

      data.resize(n);

      // positiv numbers give a bottom up bitmap.
      bi.bmiHeader.biHeight = -std::abs(bi.bmiHeader.biHeight);
      bi.bmiHeader.biCompression = BI_RGB;

      ret = GetDIBits(gc, id, 0, h, data.data(), &bi, usage);
      if (ret != h) {
        throw std::runtime_error("get image data failed");
      }

      SelectObject(gc, old);
      DeleteDC(gc);

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

    void bitmap::operator() (const graphics& g, const core::point& pt) const {
      g.copy_from(*this, core::rectangle(size()), pt);
    }

    transparent_bitmap::transparent_bitmap (const bitmap& b)
      : bitmap(b)
    {
      if (is_valid()) {
        mask.create(size(), 1);
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator= (const bitmap& bmp) {
      bitmap::operator=(bmp);
      if (is_valid()) {
        mask.create(size(), 1);
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator() (const graphics& g, const core::point& pt) const {
#ifdef WIN32
      g.copy_from(*this, core::rectangle(size()), pt);
#endif // WIN32
#ifdef X11
      auto display = core::global::get_instance();
      XSetClipMask(display, g, mask);
      XSetClipOrigin(display, g, pt.os_x(), pt.os_y());
      g.copy_from(*this, core::rectangle(size()), pt);
      XSetClipMask(display, g, None);
#endif
    }
  }

}
