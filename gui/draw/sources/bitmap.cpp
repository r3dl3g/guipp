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


namespace gui {

  namespace draw {

    std::ostream& operator<< (std::ostream& out, const bitmap& bmp) {
      bmp.save_ppm(out);
      return out;
    }

    std::istream& operator>> (std::istream& in, bitmap& bmp) {
      bmp.load_ppm(in);
      return in;
    }

    void bitmap::clear () {
      auto display = core::global::get_instance();
      if (gc) {
#if WIN32
        ReleaseDC(id, gc);
#endif
#ifdef X11
        XFreeGC(display, gc);
#endif
        gc = 0;
      }
      if (id) {
#if WIN32
#endif
#ifdef X11
        XFreePixmap(display, id);
#endif
        id = 0;
      }
    }

    void bitmap::create (const core::size& sz) {
      clear();
#ifdef X11
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto visual = DefaultRootWindow(display);
      id = XCreatePixmap(display, visual, sz.width(), sz.height(), DisplayPlanes(display, screen));
#endif
    }

    os::graphics bitmap::get_gc () {
      if (!gc && id) {
#if WIN32
        gc = GetDC(id);
#endif
#ifdef X11
        auto display = core::global::get_instance();
        gc = XCreateGC(display, id, 0, nullptr);
#endif
      }
      return gc;
    }

    core::size bitmap::size () const {
      unsigned int w = 0, h = 0;
#ifdef X11
      if (id) {
        Window root;
        int x, y;
        unsigned int b, d;
        auto display = core::global::get_instance();
        XGetGeometry(display, id, &root, &x, &y, &w, &h, &b, &d);
      }
#endif
      return core::size(w, h);
    }

    void bitmap::load_ppm (std::istream& in) {
      clear();

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
      int w, h, max;
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
      int bpl = w * 4;
      std::size_t sz = bpl * h;
      char* data = new char[sz];

      for (int i = 0; i < h; ++i) {
        char* d = (data + (i * bpl));
        for (int j = 0; j < w; ++j) {
          in.read(d, 3);
          d += 4;
        }
      }

      create(core::size(w, h));

#ifdef X11
      auto display = core::global::get_instance();
      auto screen = core::global::get_screen();
      auto gc = DefaultGC(display, screen);

      XImage* im = XCreateImage(display, DefaultVisual(display, screen), 24, ZPixmap, 0, data, w, h, 32, bpl);
      if (im) {
        XPutImage(display, id, gc, im, 0, 0, 0, 0, w, h);
        XDestroyImage(im);
      } else {
        throw std::runtime_error("create image failed");
      }
#endif
    }

    void save_ppm_p6 (std::ostream& out, char* data, int width, int height, int bpl, int bpp) {
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
      } else if (24 == bpp) {
        std::size_t n = bpl * height;
        out.write(data, n);
      }
    }

    void bitmap::save_ppm (std::ostream& out) const {
      core::size sz = size();
#ifdef X11
      auto display = core::global::get_instance();
      XImage* im = XGetImage(display, get_id(), 0, 0, sz.width(), sz.height(), AllPlanes, ZPixmap);
      if (im) {
        save_ppm_p6(out, im->data, im->width, im->height, im->bytes_per_line, im->bits_per_pixel);
        XDestroyImage(im);
      } else {
        throw std::runtime_error("get image failed");
      }
#endif
    }

    void bitmap::copy_from (os::drawable w, const core::rectangle& r) {
      if (id) {
#ifdef X11
        auto display = core::global::get_instance();
        auto screen = core::global::get_screen();
        auto gc = DefaultGC(display, screen);
        XCopyArea(display, w, id, gc, r.os_x(), r.os_y(), r.os_width(), r.os_height(), 0, 0);
#endif
      }
    }

    void bitmap::copy_to (os::drawable w, const core::rectangle& r) const {
      if (id) {
#ifdef X11
        auto display = core::global::get_instance();
        auto screen = core::global::get_screen();
        auto gc = DefaultGC(display, screen);
        XCopyArea(display, id, w, gc, 0, 0, r.os_width(), r.os_height(), r.os_x(), r.os_y());
#endif
      }
    }

    void bitmap::copy_to (os::drawable w, const core::point& pt) const {
      if (id) {
      core::size sz = size();
#ifdef X11
        auto display = core::global::get_instance();
        auto screen = core::global::get_screen();
        auto gc = DefaultGC(display, screen);
        XCopyArea(display, id, w, gc, 0, 0, sz.os_width(), sz.os_height(), pt.os_x(), pt.os_y());
#endif
      }
    }

  }

}
