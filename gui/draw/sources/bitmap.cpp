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

#if WIN32
    struct bitmap_info : public BITMAPINFO {

      bitmap_info () {
        memset(this, 0, sizeof(bitmap_info));
        bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        init_gray_colors();
      }

      bitmap_info (int w, int h, int bpl, int bpp) {
        bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        DWORD cols = (DWORD)(bpp < 24 ? (0x01 << bpp) : 0);
        bmiHeader = {
          sizeof(BITMAPINFOHEADER),
          w, h, 1, (WORD)bpp,
          BI_RGB,
          (DWORD)(h * bpl), 
          0, 0, 
          cols, 
          cols
        };
        init_gray_colors();
      }

      void init_colors () {
        HDC dc = GetDC(NULL);
        GetDIBColorTable(dc, 0, 256, bmiColors);
        ReleaseDC(NULL, dc);
      }

      void init_gray_colors () {
        for (int i = 0; i < 256; ++i) {
          bmiColors[i] = { (BYTE)i, (BYTE)i, (BYTE)i, 0 };
        }
      }

      void set_gray_colors(HBITMAP id) {
        HDC gdc = GetDC(NULL);
        HDC gc = CreateCompatibleDC(gdc);
        ReleaseDC(NULL, gdc);
        SelectObject(gc, id);

        SetDIBColorTable(gc, 0, 256, bmiColors);
        DeleteDC(gc);
      }

    private:
      RGBQUAD moreColors[255];
    };
#endif

    bitmap::bitmap (const bitmap& rhs)
      : id(0)
    {
      operator=(rhs);
    }

    bitmap::bitmap (bitmap&& rhs)
      : id(std::move(rhs.id))
    {}

    void bitmap::operator= (const bitmap& rhs) {
      if (&rhs != this) {
        clear();
        if (rhs) {
          create(rhs.size(), rhs.depth());
          put(rhs);
        }
      }
    }

    void bitmap::operator= (bitmap&& rhs) {
      if (&rhs != this) {
        clear();
        id = std::move(rhs.id);
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

    void bitmap::create (int w, int h, int bpp) {
      clear();
#if WIN32
      id = CreateBitmap(w, h, 1, bpp, NULL);
      if (id && (8 == bpp)) {
        bitmap_info bi;
        bi.set_gray_colors(id);
      }
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
      if (8 == bpp) {
        bitmap_info bi;
        bi.set_gray_colors(id);
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

    template<int D, int M>
    int up_modulo (int v) {
      int r = (v + D - 1) / D;
      return r + r % M;
    }

    int bitmap::calc_bytes_per_line (int w, int bpp) {
      switch (bpp) {
#ifdef WIN32
        case 1:
          return up_modulo<8, 2>(w);
        case 8:
          return up_modulo<1, 2>(w);
        case 24:
          return up_modulo<1, 2>(w * 3);
        case 32:
          return up_modulo<1, 2>(w * 4);
#endif // WIN32
#ifdef X11
        case 1:
          return up_modulo<8, 16>(w);
        case 8:
          return up_modulo<1, 4>(w);
        case 24:
          return up_modulo<1, 4>(w * 3);
        case 32:
          return up_modulo<1, 4>(w * 4);
#endif // X11
      }
      return -1;
    }

    void put_data (os::bitmap id, const std::vector<char>& src, int w, int h, int bpl, int bpp) {
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
      XFreeGC(display, gc);
#endif
    }

    void bitmap::put (const std::vector<char>& src, int w, int h, int bpl, int bpp) {
//#if WIN32
      //if (depth() == core::global::get_device_bits_per_pixel()) {
      //  HDC gdc = GetDC(NULL);
      //  HDC gc = CreateCompatibleDC(gdc);
      //  ReleaseDC(NULL, gdc);

      //  SelectObject(gc, id);

      //  bitmap_info bi(w, h, bpl, bpp);

      //  int ret = StretchDIBits(gc, 0, 0, w, h, 0, 0, w, h, src.data(), &bi, DIB_RGB_COLORS, SRCCOPY);
      //  DeleteDC(gc);
      //  if (ret == h) {
      //    return;
      //  }
      //}
      //// fall back to amnual mode.
//#endif

      int dst_bpp = depth();
      if (dst_bpp == bpp) {
        put_data(id, src, w, h, bpl, bpp);
      } else {
        std::vector<char> dst;
        int dst_bpl = calc_bytes_per_line(w, dst_bpp);
        dst.resize(dst_bpl * h);

        using namespace convert;

        switch (bpp) {
          case 1:
            switch (dst_bpp) {
              case 8:  converter<1, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<1, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<1, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 8:
            switch (dst_bpp) {
              case 1:  converter<8, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<8, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<8, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 24:
            switch (dst_bpp) {
              case 1:  converter<24, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 8:  converter<24, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 32: converter<24, 32>::convert(src, dst, w, h, bpl, dst_bpl); break;
            }
          break;
          case 32:
            switch (dst_bpp) {
              case 1:  converter<32, 1>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 8:  converter<32, 8>::convert(src, dst, w, h, bpl, dst_bpl); break;
              case 24: converter<32, 24>::convert(src, dst, w, h, bpl, dst_bpl); break;
              }
          break;
        }

        put_data(id, dst, w, h, dst_bpl, dst_bpp);

      }
    }

    void bitmap::put (const bitmap& rhs) {
      int w, h, bpl, bpp;
      std::vector<char> data;
      rhs.get_data(data, w, h, bpl, bpp);
      put(data, w, h, bpl, bpp);
    }

    void bitmap::get_data (std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) const {
#if WIN32
      BITMAP bmp;
      GetObject(id, sizeof(BITMAP), &bmp);
      //if (bmp.bmBitsPixel == 8) {
        w = bmp.bmWidth;
        h = bmp.bmHeight;
        bpl = bmp.bmWidthBytes;
        bpp = bmp.bmBitsPixel;
        data.resize(bpl * h);
        int dst_bpl = calc_bytes_per_line(w, bpp);
        int ret = GetBitmapBits(id, (LONG)data.size(), data.data());
        if (ret != data.size()) {
          throw std::runtime_error("get image data failed");
        }
      //} else {
      //  bitmap_info bi;

      //  HDC gdc = GetDC(NULL);
      //  HDC gc = CreateCompatibleDC(gdc);
      //  ReleaseDC(NULL, gdc);

      //  HGDIOBJ old = SelectObject(gc, id);

      //  int ret = GetDIBits(gc, id, 0, 0, nullptr, &bi, DIB_RGB_COLORS);
      //  if (!ret) {
      //    throw std::runtime_error("get image info failed");
      //  }

      //  const int n = bi.bmiHeader.biSizeImage;
      //  w = bi.bmiHeader.biWidth;
      //  h = bi.bmiHeader.biHeight;
      //  bpl = n / h;
      //  bpp = bi.bmiHeader.biBitCount;

      //  data.resize(n);

      //  // positiv numbers give a bottom up bitmap.
      //  bi.bmiHeader.biHeight = -std::abs(bi.bmiHeader.biHeight);
      //  bi.bmiHeader.biCompression = BI_RGB;

      //  ret = GetDIBits(gc, id, 0, h, data.data(), &bi, DIB_RGB_COLORS);
      //  if (ret != h) {
      //    throw std::runtime_error("get image data failed");
      //  }

      //  SelectObject(gc, old);
      //  DeleteDC(gc);
      //}

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
      draw(g, pt);
    }

    void bitmap::draw (const graphics& g, const core::point& pt) const {
      g.copy_from(*this, core::rectangle(size()), pt);
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

    transparent_bitmap::transparent_bitmap (bitmap&& b)
      : bitmap(std::move(b))
    {
      if (is_valid()) {
        mask.create(size(), 1);
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator= (bitmap&& bmp) {
      bitmap::operator=(std::move(bmp));
      if (is_valid()) {
        mask.create(size(), 1);
        mask.put(*this);
      }
    }

    void transparent_bitmap::operator() (const graphics& g, const core::point& pt) const {
      draw(g, pt);
    }

    void transparent_bitmap::draw (const graphics& g, const core::point& pt) const {
#ifdef WIN32
      core::size sz = size();
      HDC mem_dc = CreateCompatibleDC(g);
      HGDIOBJ old = SelectObject(mem_dc, mask.get_id());
      BitBlt(g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), mem_dc, 0, 0, SRCAND);
      SelectObject(mem_dc, get_id());
      BitBlt(g, pt.os_x(), pt.os_y(), sz.os_width(), sz.os_height(), mem_dc, 0, 0, SRCPAINT);
      SelectObject(mem_dc, old);
      DeleteDC(mem_dc);
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
