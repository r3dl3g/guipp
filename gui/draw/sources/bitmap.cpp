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
#include <ostream>
#include <logger.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "bitmap.h"
#include "graphics.h"


namespace gui {

  namespace core {

    void load_ppm_p6 (std::istream& in, std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) {
      std::string p6;
      std::getline(in, p6);
      if (p6 != "P6") {
        throw std::runtime_error("stream contains no P6 ppm");
      }
      std::skipws(in);
      while (in.peek() == '#') {
        std::string tmp;
        std::getline(in, tmp);
      }
      std::skipws(in);
      int max;
      in >> w >> h >> max;

      if (w < 1) {
        throw std::runtime_error("invalid ppm image width");
      }
      if (h < 1) {
        throw std::runtime_error("invalid ppm image height");
      }
      if (max > 255) {
        throw std::runtime_error("invalid ppm max value");
      }

      if (bpp == 24) {
        bpl = w * 3;
        std::size_t n = bpl * h;
        data.resize(n);
        in.read(data.data(), n);
      } else {
        bpp = 32;
        bpl = w * 4;
        std::size_t n = bpl * h;
        data.resize(n);
#ifndef NDEBUG
        char* end = data.data() + n;
#endif // NDEBUG
        for (int i = 0; i < h; ++i) {
          char* d = (data.data() + (i * bpl));
          for (int j = 0; j < w; ++j) {
            in.read(d, 3);
#ifndef NDEBUG
            if (d > end) {
              throw std::out_of_range("access beyond boundary!");
            }
#endif // NDEBUG
            d += 4;
          }
        }
      }

    }

    void save_ppm_p6(std::ostream& out, char* data, int width, int height, int bpl, int bpp) {
      out << "P6\n"
        << "# XImage created RGB ppm file\n"
        << width << ' ' << height << " 255\n";
      if (32 == bpp) {
        for (int h = 0; h < height; ++h) {
          char* d = (data + (h * bpl));
          for (int w = 0; w < width; ++w) {
            out.write(d, 3);
            d += 4;
          }
        }
      }
      else if (24 == bpp) {
        std::size_t n = bpl * height;
        out.write(data, n);
      }
    }

  }

  namespace draw {

    std::ostream& operator<< (std::ostream& out, const bitmap& bmp) {
      bmp.save_ppm(out);
      return out;
    }

    std::istream& operator>> (std::istream& in, bitmap& bmp) {
      bmp.load_ppm(in);
      return in;
    }

    bitmap::bitmap (const bitmap& rhs)
      : id(0)
    {
      operator=(rhs);
    }

    void bitmap::operator= (const bitmap& rhs) {
      if (&rhs != this) {
        core::size sz = rhs.size();
        create(sz);
        draw::graphics(*this).copy_from(rhs, core::rectangle(sz));
      }
    }

    void bitmap::clear () {
      if (id) {
#if WIN32
        DeleteObject(id);
#endif
#ifdef X11
        auto display = core::global::get_instance();
        XFreePixmap(display, id);
#endif
        id = 0;
      }
    }

    void bitmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
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
      id = XCreatePixmap(display, visual, w, h, DisplayPlanes(display, screen));
#endif
      if (!id) {
        throw std::runtime_error("create image failed");
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
        XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
      }
      return core::size(w, h);
#endif
    }

    void bitmap::create (const std::vector<char>& data, int w, int h, int bpl, int bpp) {
#if WIN32
      if (core::global::get_device_bits_per_pixel() == bpp) {
        id = CreateBitmap(w, h, 1, bpp, data.data());
      } else {
        create(w, h);

        HDC gdc = GetDC(NULL);
        HDC gc = CreateCompatibleDC(gdc);
        ReleaseDC(NULL, gdc);

        SelectObject(gc, id);

        BITMAPINFOHEADER bi = {
          sizeof(BITMAPINFOHEADER),
          w, -h, 1, (WORD)bpp, BI_RGB, (DWORD)(h * bpl), 0, 0, 0, 0
        };

        StretchDIBits(gc, 0, 0, w, h, 0, 0, w, h, data.data(), reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, SRCCOPY);
        DeleteDC(gc);
      }
      if (!id) {
        throw std::runtime_error("create image failed");
      }

#endif
#ifdef X11
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto visual = DefaultRootWindow(display);
      id = XCreatePixmap(display, visual, w, h, DisplayPlanes(display, screen));
      if (!id) {
        throw std::runtime_error("create image failed");
      }
      auto gc = DefaultGC(display, screen);

      XImage im = {
          w, h,                             /* size of image */
          0,                                /* number of pixels offset in X direction */
          ZPixmap,                          /* XYBitmap, XYPixmap, ZPixmap */
          const_cast<char*>(data.data()),   /* pointer to image data */
          LSBFirst,                         /* data byte order, LSBFirst, MSBFirst */
          bpp,                              /* quant. of scanline 8, 16, 32 */
          LSBFirst,                         /* LSBFirst, MSBFirst */
          bpp,                              /* 8, 16, 32 either XY or ZPixmap */
          24,                               /* depth of image */
          bpl,                              /* accelarator to next line */
          bpp,                              /* bits per pixel (ZPixmap) */
          0                                 /* bits in z arrangment */
      };

      XInitImage(&im);
      XPutImage(display, id, gc, &im, 0, 0, 0, 0, w, h);

//      XImage* im = XCreateImage(display, DefaultVisual(display, screen), bpp, ZPixmap, 0, const_cast<char*>(data.data()), w, h, 32, bpl);
//      if (im) {
//        XPutImage(display, id, gc, im, 0, 0, 0, 0, w, h);
//        XDestroyImage(im);
//      }
//      else {
//        throw std::runtime_error("create image failed");
//      }
#endif
    }

    void bitmap::get_data (std::vector<char>& data, int& w, int& h, int& bpl, int& bpp) const {
#if WIN32
      BITMAPINFOHEADER bi = { sizeof(BITMAPINFOHEADER), 0 };

      HDC gdc = GetDC(NULL);
      HDC gc = CreateCompatibleDC(gdc);
      ReleaseDC(NULL, gdc);

      HGDIOBJ old = SelectObject(gc, id);

      int ret = GetDIBits(gc, id, 0, 0, nullptr, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
      if (!ret) {
        throw std::runtime_error("get image info failed");
      }

      const int n = bi.biSizeImage;
      w = bi.biWidth;
      h = bi.biHeight;
      bpl = n / h;
      bpp = bi.biBitCount;

      data.resize(n);

      // positiv numbers give a bottom up bitmap.
      bi.biHeight = -std::abs(bi.biHeight);
      bi.biCompression = BI_RGB;

      ret = GetDIBits(gc, id, 0, h, data.data(), reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
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

    void bitmap::load_ppm (std::istream& in) {
      int w, h, bpl, bpp = 32;
      std::vector<char> data;
      core::load_ppm_p6(in, data, w, h, bpl, bpp);
      create(data, w, h, bpl, bpp);
    }

    void bitmap::save_ppm(std::ostream& out) const {
#if WIN32
      int w, h, bpl, bpp;
      std::vector<char> data;
      get_data(data, w, h, bpl, bpp);
      core::save_ppm_p6(out, data.data(), w, h, bpl, bpp);
#endif
#ifdef X11
      core::size sz = size();
      auto display = core::global::get_instance();
      XImage* im = XGetImage(display, get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
      if (im) {
        core::save_ppm_p6(out, im->data, im->width, im->height, im->bytes_per_line, im->bits_per_pixel);
        XDestroyImage(im);
      } else {
        throw std::runtime_error("get image failed");
      }
#endif
    }

  }

}
