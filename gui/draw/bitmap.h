/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: bitmap definition
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>
#include <gui/draw/converter.h>


namespace gui {

  namespace draw {

    struct bitmap_info {
      unsigned int width;
      unsigned int height;
      int bytes_per_line;
      BPP bits_per_pixel;

      core::size size () const;
      int depth () const;
      std::size_t mem_size () const;

    };

    namespace copy {

      // --------------------------------------------------------------------------
      template<BPP bpp>
      void row (cbyteptr src, byteptr dst, int src_x0, int dest_x0, int dest_w);

      template<>
      inline void row<BPP::BW> (cbyteptr src, byteptr dst, int src_x0, int dest_x0, int dest_w) {
        for (int x = 0; x < dest_w; ++x) {
          byte b = convert::bpp::get<BPP::BW>(src, src_x0 + x);
          convert::bpp::set<BPP::BW>(dst, dest_x0 + x, b);
        }
      }

      template<>
      inline void row<BPP::GRAY> (cbyteptr src, byteptr dst, int src_x0, int dest_x0, int dest_w) {
        memcpy(dst + dest_x0, src + src_x0, dest_w);
      }

      template<>
      inline void row<BPP::RGB> (cbyteptr src, byteptr dst, int src_x0, int dest_x0, int dest_w) {
        memcpy(dst + dest_x0 * 3, src + src_x0 * 3, dest_w * 3);
      }

      template<>
      inline void row<BPP::RGBA> (cbyteptr src, byteptr dst, int src_x0, int dest_x0, int dest_w) {
        memcpy(dst + dest_x0 * 4, src + src_x0 * 4, dest_w * 4);
      }

      template<BPP bpp>
      void sub (const std::vector<char>& src_data,
                const bitmap_info& src_bmi,
                std::vector<char>& dest_data,
                const bitmap_info& dest_bmi,
                int src_x0, int src_y0,
                int dest_x0, int dest_y0,
                int dest_w, int dest_h) {
        for (int y = 0; y < dest_h; ++y) {
          cbyteptr src = reinterpret_cast<cbyteptr>(src_data.data() + (src_y0 + y) * src_bmi.bytes_per_line);
          byteptr dst = reinterpret_cast<byteptr>(dest_data.data() + (dest_y0 + y) * dest_bmi.bytes_per_line);
          row<bpp>(src, dst, src_x0, dest_x0, dest_w);
        }
      }
    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (const std::vector<char>& src_data, std::vector<char>& dest_data,
                int src_offs, int dest_offs, int src_x0, int dest_x0, int src_w, int dest_w);

      template<>
      void row<BPP::GRAY> (const std::vector<char>& src_data, std::vector<char>& dest_data,
                           int src_offs, int dest_offs, int src_x0, int dest_x0, int src_w, int dest_w);

      template<>
      void row<BPP::RGB> (const std::vector<char>& src_data, std::vector<char>& dest_data,
                          int src_offs, int dest_offs, int src_x0, int dest_x0, int src_w, int dest_w);

      template<>
      void row<BPP::RGBA> (const std::vector<char>& src_data, std::vector<char>& dest_data,
                           int src_offs, int dest_offs, int src_x0, int dest_x0, int src_w, int dest_w);

      template<BPP bpp>
      void sub (const std::vector<char>& src_data, const bitmap_info& src_bmi,
                std::vector<char>& dest_data, const bitmap_info& dest_bmi,
                int src_x0, int src_y0, int src_w, int src_h,
                int dest_x0, int dest_y0, int dest_w, int dest_h) {
        for (int y = 0; y < dest_h; ++y) {
          const int src_y = src_y0 + y * src_h / dest_h;
          const int src_offs = src_y * src_bmi.bytes_per_line;
          const int dest_offs = (dest_y0 + y) * dest_bmi.bytes_per_line;
          row<bpp>(src_data, dest_data, src_offs, dest_offs, src_x0, dest_x0, src_w, dest_w);
        }
      }

      template<>
      void sub<BPP::BW> (const std::vector<char>& src_data, const bitmap_info& src_bmi,
                         std::vector<char>& dest_data, const bitmap_info& dest_bmi,
                         int src_x0, int src_y0, int src_w, int src_h,
                         int dest_x0, int dest_y0, int dest_w, int dest_h);

    } // namespace stretch

    // --------------------------------------------------------------------------
    class bitmap {
    public:
      bitmap ();
      explicit bitmap (os::bitmap id);

      ~bitmap ();

      bool is_valid () const;

      operator bool () const;

      os::bitmap get_id () const;
      bitmap_info get_info () const;

      void create (const std::vector<char>& data, const bitmap_info& bmi);
      void clear ();
      void invert ();

      void put_data (const std::vector<char>& data, const bitmap_info& bmi);
      void get_data (std::vector<char>& data, bitmap_info& bmi) const;

      void put (const bitmap& rhs);

      core::size size () const;
      int depth () const;
      BPP bits_per_pixel () const;

      static int calc_bytes_per_line (int w, BPP bpp);

    protected:
      bitmap (bitmap&& rhs);
      void operator= (bitmap&&);

      void create_compatible (int w, int h);
      void create_compatible (const core::size& sz);

      void create (int w, int h, BPP bpp);
      void create (const core::size& sz, BPP bpp);

      void copy_from (const bitmap& src_img);
      void copy_from (const bitmap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void stretch_from (const bitmap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

      static bitmap_info convert (const std::vector<char>& src,
                                  const bitmap_info& bmi,
                                  std::vector<char>& dst,
                                  BPP dst_bpp);

    private:
      void set_id (os::bitmap);

      os::bitmap id;
      bitmap (const bitmap&) = delete;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    class datamap : public bitmap {
    public:
      typedef bitmap super;
      static constexpr BPP bpp = T;

      datamap ();

      datamap (const datamap& rhs);
      datamap (datamap&& rhs);

      datamap (const bitmap& rhs);
      datamap (int w, int h);
      datamap (int w, int h, int bpl, const std::vector<char>& data);
      datamap (const core::size& sz);

      void operator= (const datamap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (datamap&& rhs);

      void create (int w, int h);
      void create (const core::size& sz);

      void copy_from (const datamap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void stretch_from (const datamap& src);
      void stretch_from (const datamap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

    };

    // --------------------------------------------------------------------------
    typedef datamap<BPP::BW> maskmap;
    typedef datamap<BPP::GRAY> graymap;
    typedef datamap<BPP::RGB> rgbmap;
    typedef datamap<BPP::RGBA> rgbamap;

    // --------------------------------------------------------------------------
    class memmap : public bitmap {
    public:
      typedef bitmap super;

      memmap ();

      memmap (const memmap& rhs);
      memmap (memmap&& rhs);

      memmap (const bitmap& rhs);
      memmap (int w, int h);
      memmap (const core::size& sz);

      void operator= (const memmap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (memmap&& rhs);

      operator os::drawable() const;

      void create (int w, int h);
      void create (const core::size& sz);

      void copy_from (const memmap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void stretch_from (const memmap& src);
      void stretch_from (const memmap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

    };

    // --------------------------------------------------------------------------
    class masked_bitmap {
    public:
      typedef memmap super;

      masked_bitmap ();

      masked_bitmap (const masked_bitmap&);
      masked_bitmap (masked_bitmap&&);

      masked_bitmap (const memmap& bmp);
      masked_bitmap (memmap&& bmp);

      masked_bitmap (const memmap& bmp, const maskmap& mask);
      masked_bitmap (memmap&& bmp, maskmap&& mask);

      void operator= (const masked_bitmap& rhs);
      void operator= (masked_bitmap&&);

      void operator= (const memmap& bmp);
      void operator= (memmap&& bmp);

      core::size size () const;

      bool is_valid () const;

      operator bool () const;

      memmap image;
      maskmap mask;
    };

    // --------------------------------------------------------------------------
    // inlines
    inline core::size bitmap_info::size () const {
      return {static_cast<core::size_type>(width), static_cast<core::size_type>(height)};
    }

    inline int bitmap_info::depth () const {
      return static_cast<int>(bits_per_pixel);
    }

    inline std::size_t bitmap_info::mem_size () const {
      return bytes_per_line * height;
    }

    // --------------------------------------------------------------------------
    inline bitmap::bitmap ()
      : id(0)
    {}

    inline bitmap::bitmap (os::bitmap id)
      : id(id)
    {}

    inline bitmap::~bitmap () {
      clear();
    }

    inline bitmap::bitmap (bitmap&& rhs)
      : id(0) {
      operator= (std::move(rhs));
    }

    inline void bitmap::set_id (os::bitmap rhs) {
      id = rhs;
    }

    inline bool bitmap::is_valid () const {
      return id != 0;
    }

    inline bitmap::operator bool () const {
      return is_valid();
    }

    inline os::bitmap bitmap::get_id () const {
      return id;
    }

    // --------------------------------------------------------------------------
    template<BPP T>
    inline datamap<T>::datamap ()
    {}

    template<BPP T>
    inline datamap<T>::datamap (const datamap& rhs) {
      operator= (rhs);
    }

    template<BPP T>
    inline datamap<T>::datamap (const bitmap& rhs) {
      operator= (rhs);
    }

    template<BPP T>
    inline datamap<T>::datamap (datamap&& rhs)
      : super(std::move(rhs))
    {}

    template<BPP T>
    inline datamap<T>::datamap (int w, int h) {
      create(w, h);
    }

    template<BPP T>
    inline datamap<T>::datamap (int w, int h, int bpl, const std::vector<char>& data) {
      super::create(data, {w, h, bpl, T});
    }

    template<BPP T>
    inline datamap<T>::datamap (const core::size& sz) {
      create(sz);
    }

    template<BPP T>
    inline void datamap<T>::operator= (const datamap& rhs) {
      operator= (static_cast<const bitmap&>(rhs));
    }

    template<BPP T>
    inline void datamap<T>::operator= (const bitmap& rhs) {
      if (this != &rhs) {
        if (rhs) {
          create(rhs.size());
          put(rhs);
        } else {
          clear();
        }
      }
    }

    template<BPP T>
    inline void datamap<T>::operator= (datamap&& rhs) {
      super::operator= (std::move(rhs));
    }

    template<BPP T>
    inline void datamap<T>::create (int w, int h) {
      super::create(w, h, T);
    }

    template<BPP T>
    inline void datamap<T>::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    template<BPP T>
    inline void datamap<T>::copy_from (const datamap& src_img,
                                       const core::rectangle& src_rect,
                                       const core::point& dest_pt) {
      super::copy_from(src_img, src_rect, dest_pt);
    }

    template<BPP T>
    inline void datamap<T>::stretch_from (const datamap& src) {
      super::stretch_from(src, core::rectangle(src.size()), core::rectangle(size()));
    }

    template<BPP T>
    inline void datamap<T>::stretch_from (const datamap& src_img,
                                          const core::rectangle& src_rect,
                                          const core::rectangle& dest_rect) {
      super::stretch_from(src_img, src_rect, dest_rect);
    }

    // --------------------------------------------------------------------------
    inline memmap::memmap ()
    {}

    inline memmap::memmap (const memmap& rhs) {
      operator= (rhs);
    }

    inline memmap::memmap (const bitmap& rhs) {
      operator= (rhs);
    }

    inline memmap::memmap (memmap&& rhs)
      : super(std::move(rhs))
    {}

    inline memmap::memmap (int w, int h) {
      create(w, h);
    }

    inline memmap::memmap (const core::size& sz) {
      create(sz);
    }

    inline void memmap::operator= (const memmap& rhs) {
      operator= (static_cast<const bitmap&>(rhs));
    }

    inline void memmap::operator= (const bitmap& rhs) {
      if (this != &rhs) {
        if (rhs) {
          create(rhs.size());
          put(rhs);
        } else {
          clear();
        }
      }
    }

    inline void memmap::operator= (memmap&& rhs) {
      super::operator= (std::move(rhs));
    }

    inline memmap::operator os::drawable() const {
      return get_id();
    }

    inline void memmap::create (int w, int h) {
      create_compatible(w, h);
    }

    inline void memmap::create (const core::size& sz) {
      create(sz.os_width(), sz.os_height());
    }

    inline void memmap::copy_from (const memmap& src_img,
                                   const core::rectangle& src_rect,
                                   const core::point& dest_pt) {
      super::copy_from(src_img, src_rect, dest_pt);
    }

    inline void memmap::stretch_from (const memmap& src) {
      super::stretch_from(src, core::rectangle(src.size()), core::rectangle(size()));
    }

    inline void memmap::stretch_from (const memmap& src_img,
                                      const core::rectangle& src_rect,
                                      const core::rectangle& dest_rect) {
      super::stretch_from(src_img, src_rect, dest_rect);
    }

    // --------------------------------------------------------------------------
    inline masked_bitmap::masked_bitmap ()
    {}

    inline masked_bitmap::masked_bitmap (const masked_bitmap& rhs)
      : image(rhs.image)
      , mask(rhs.mask)
    {}

    inline masked_bitmap::masked_bitmap (masked_bitmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const memmap& rhs) {
      operator= (rhs);
    }

    inline masked_bitmap::masked_bitmap (memmap&& rhs) {
      operator= (std::move(rhs));
    }

    inline masked_bitmap::masked_bitmap (const memmap& image, const maskmap& mask)
      : image(image)
      , mask(mask)
    {}

    inline masked_bitmap::masked_bitmap (memmap&& img, maskmap&& msk) {
      std::swap(image, img);
      std::swap(mask, msk);
    }

    inline core::size masked_bitmap::size () const {
      return image.size();
    }

    inline bool masked_bitmap::is_valid () const {
      return image.is_valid();
    }

    inline masked_bitmap::operator bool () const {
      return image.is_valid();
    }

    // --------------------------------------------------------------------------
  }

}
