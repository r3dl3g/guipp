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
        bmi = {
          static_cast<uint32_t>(im->width),
          static_cast<uint32_t>(im->height),
          BPP(im->depth)
        };
        const size_t n = im->height * im->bytes_per_line;
        if (im->bits_per_pixel != im->depth) {
          blob src;
          src.assign(im->data, im->data + n);
          data.resize(bmi.mem_size());
          switch (im->depth) {
            case 24:
              convert::bpp::convert<BPP::RGBA, BPP::RGB>(src, data,
                                                         im->width, im->height,
                                                         im->bytes_per_line,
                                                         bmi.bytes_per_line);
            break;
            case 32:
              convert::bpp::convert<BPP::RGB, BPP::RGBA>(src, data,
                                                         im->width, im->height,
                                                         im->bytes_per_line,
                                                         bmi.bytes_per_line);
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
        copy_from(rhs);
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
      return get_info().size();
    }

    byte basic_map::depth () const {
      return get_info().depth();
    }

    BPP basic_map::bits_per_pixel () const {
      return get_info().bits_per_pixel;
    }

    void basic_map::create (uint32_t w, uint32_t h, BPP bpp) {
      bitmap_info bmi = get_info();
      auto sz = bmi.size();
      if ((sz.os_width() == w) && (sz.os_height() == h) && (bmi.bits_per_pixel == bpp)) {
        return;
      }
      clear();
      set_id(create_bitmap({w, h, bpp}));
      if (!is_valid()) {
        throw std::runtime_error("create image failed");
      }
    }

    void basic_map::copy_from (const basic_map& rhs) {
      clear();
      if (rhs) {
        bitmap_info bmi;
        blob data;
        rhs.get_data(data, bmi);
        set_id(create_bitmap(bmi, data.data()));
      }
    }

    uint32_t basic_map::calc_bytes_per_line (uint32_t w, BPP bpp) {
      return draw::bitmap_info(w, 0, bpp).bytes_per_line;
    }

    void basic_map::put_data (const blob& src_data, const bitmap_info& src_bmi) {
      if (is_valid()) {
        bitmap_put_data(get_id(), src_data.data(), src_bmi);
      }
    }

    void basic_map::get_data (blob& data, bitmap_info& bmi) const {
      if (is_valid()) {
        bitmap_get_data(get_id(), data, bmi);
      }
    }

    void basic_map::invert () {
      bitmap_info bmi;
      blob data;
      get_data(data, bmi);
      for (auto& c : data) {
        c = ~c;
      }
      put_data(data, bmi);
    }

    // --------------------------------------------------------------------------
    void bitmap::create (uint32_t w, uint32_t h) {
      super::create(w, h, BPP::BW);
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

    bwmap bitmap::get () const {
      bwmap bmp(size());
      get_data(bmp.get_data(), bmp.get_info());
      return bmp;
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
      super::create(w, h, core::global::get_device_bits_per_pixel());
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

    void pixmap::put (const basic_datamap& rhs) {
      if (rhs) {
        put_data(rhs.get_data(), rhs.get_info());
      }
    }

    void pixmap::operator= (const basic_datamap& rhs) {
      if (rhs) {
        create(rhs.size());
        if (bits_per_pixel() == rhs.bits_per_pixel()) {
          put(rhs);
        } else {
          switch (bits_per_pixel()) {
            case BPP::RGB:  put(rgbmap(rhs));   break;
            case BPP::RGBA: put(rgbamap(rhs));  break;
            case BPP::GRAY: put(graymap(rhs));  break;
          }
        }
      } else {
        clear();
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
    frame_image::frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t edge)
      : rect(r)
      , img(img)
      , left(edge)
      , top(edge)
      , right(edge)
      , bottom(edge)
    {}

    frame_image::frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t horizontal, uint32_t vertical)
      : rect(r)
      , img(img)
      , left(horizontal)
      , top(vertical)
      , right(horizontal)
      , bottom(horizontal)
    {}

    frame_image::frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom)
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
      const uint32_t target_bpl = dest_bmi.bytes_per_line;
      const uint32_t source_right = src_bmi.width - right;
      const uint32_t source_bottom = src_bmi.height - bottom;
      const uint32_t src_bpl = src_bmi.bytes_per_line;
      const uint32_t dest_bpl = dest_bmi.bytes_per_line;

      using namespace convert;

      // top left
      if (top && left) {
        copy::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl, 0, 0, 0, 0, left, top);
      }

      // top right
      if (top && right) {
        copy::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl, source_right, 0, target_right, 0, right, top);
      }

      // bottom left
      if (bottom && left) {
        copy::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl, 0, source_bottom, 0, target_bottom, left, bottom);
      }

      if (bottom && right) {
        // bottom right
        copy::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                       source_right, source_bottom, target_right, target_bottom, right, bottom);
      }

      if ((target_right > left) && (target_bottom > top) && (source_right >= left) && (source_bottom >= top)) {
        const uint32_t target_inner_width = target_right - left;
        const uint32_t target_inner_height = target_bottom - top;
        const uint32_t source_inner_width = source_right - left;
        const uint32_t source_inner_height = source_bottom - top;

        // top center
        if (top && target_inner_width) {
          stretch::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                            left, 0, source_inner_width, top,
                            left, 0, target_inner_width, top);
        }

        // bottom center
        if (bottom && target_inner_width) {
          stretch::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                            left, source_bottom, source_inner_width, bottom,
                            left, target_bottom, target_inner_width, bottom);
        }

        // left center
        if (left && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                            0, top, left, source_inner_height,
                            0, top, left, target_inner_height);
        }

        // right center
        if (right && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                            source_right, top, right, source_inner_height,
                            target_right, top, right, target_inner_height);
        }

        // center
        if (target_inner_width && target_inner_height) {
          stretch::sub<bpp>(src_data, src_bpl, dest_data, dest_bpl,
                            left, top, source_inner_width, source_inner_height,
                            left, top, target_inner_width, target_inner_height);
        }
      }
    }

    void frame_image::operator() (const graphics& g, const core::point& pt) const {
      const blob& src_data = img.get_data();
      const bitmap_info& src_bmi = img.get_info();

      if (rect.size() <= core::size::two) {
        return;
      }

      const BPP bpp = src_bmi.bits_per_pixel;
      const uint32_t w = roundup<uint32_t>(rect.width());
      const uint32_t h = roundup<uint32_t>(rect.height());

      const uint32_t width = roundup<uint32_t>(rect.width());
      const uint32_t height = roundup<uint32_t>(rect.height());

      const uint32_t l = std::min(left, width / 2);
      const uint32_t r = std::min(right, width / 2);
      const uint32_t t = std::min(top, height / 2);
      const uint32_t b = std::min(bottom, height / 2);

      pixmap buffer;

      switch (bpp) {
        case BPP::BW: {
          bwmap dest(w, h);
          copy_frame_image<BPP::BW>(src_data, dest.get_data(), src_bmi, dest.get_info(), l, t, r, b);
          buffer = dest;
          break;
        }
        case BPP::GRAY: {
          graymap dest(w, h);
          copy_frame_image<BPP::GRAY>(src_data, dest.get_data(), src_bmi, dest.get_info(), l, t, r, b);
          buffer = dest;
          break;
        }
        case BPP::RGB: {
          rgbmap dest(w, h);
          copy_frame_image<BPP::RGB>(src_data, dest.get_data(), src_bmi, dest.get_info(), l, t, r, b);
          buffer = dest;
          break;
        }
        case BPP::RGBA: {
          rgbamap dest(w, h);
          copy_frame_image<BPP::RGBA>(src_data, dest.get_data(), src_bmi, dest.get_info(), l, t, r, b);
          buffer = dest;
          break;
        }
      }

      g.copy_from(buffer, pt);
    }
    // --------------------------------------------------------------------------
  }

}
