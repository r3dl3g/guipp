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
      bitmap_info ();
      bitmap_info (uint32_t w, uint32_t h, BPP bpp);
      bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, BPP bpp);

      uint32_t width;
      uint32_t height;
      uint32_t bytes_per_line;
      BPP bits_per_pixel;

      core::size size () const;
      byte depth () const;
      std::size_t mem_size () const;

    };

    namespace copy {

      // --------------------------------------------------------------------------
      template<BPP bpp>
      void row (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<>
      void row<BPP::BW> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<>
      void row<BPP::GRAY> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<>
      void row<BPP::RGB> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<>
      void row<BPP::RGBA> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w);

      template<BPP bpp>
      void sub (const blob& src_data,
                const bitmap_info& src_bmi,
                blob& dest_data,
                const bitmap_info& dest_bmi,
                uint32_t src_x0, uint32_t src_y0,
                uint32_t dest_x0, uint32_t dest_y0,
                uint32_t dest_w, uint32_t dest_h);


    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void row (const blob& src_data, blob& dest_data,
                uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w);

      template<>
      void row<BPP::GRAY> (const blob& src_data, blob& dest_data,
                           uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w);

      template<>
      void row<BPP::RGB> (const blob& src_data, blob& dest_data,
                          uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w);

      template<>
      void row<BPP::RGBA> (const blob& src_data, blob& dest_data,
                           uint32_t src_offs, uint32_t dest_offs, uint32_t src_x0, uint32_t dest_x0, uint32_t src_w, uint32_t dest_w);

      template<BPP bpp>
      void sub (const blob& src_data, const bitmap_info& src_bmi,
                blob& dest_data, const bitmap_info& dest_bmi,
                uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h);

      template<>
      void sub<BPP::BW> (const blob& src_data, const bitmap_info& src_bmi,
                         blob& dest_data, const bitmap_info& dest_bmi,
                         uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                         uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h);

    } // namespace stretch

    namespace adjust {

      template<BPP bpp>
      void brightness_row (byteptr data, uint32_t offset, uint32_t w, float f);

      template<>
      void brightness_row<BPP::GRAY> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<>
      void brightness_row<BPP::RGB> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<>
      void brightness_row<BPP::RGBA> (byteptr data, uint32_t offset, uint32_t w, float f);

      template<BPP bpp>
      void brightness (blob& data, const bitmap_info& bmi, float f);

    }

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

      void create (const blob& data, const bitmap_info& bmi);
      void clear ();
      void invert ();

      void put_data (const blob& data, const bitmap_info& bmi);
      void get_data (blob& data, bitmap_info& bmi) const;

      void put (const bitmap& rhs);

      core::size size () const;
      byte depth () const;
      BPP bits_per_pixel () const;

      static uint32_t calc_bytes_per_line (uint32_t w, BPP bpp);

    protected:
      bitmap (bitmap&& rhs);

      void operator= (bitmap&&);

      void create_compatible (uint32_t w, uint32_t h);
      void create_compatible (const core::size& sz);

      void create (uint32_t w, uint32_t h, BPP bpp);
      void create (const core::size& sz, BPP bpp);

      void copy_from (const bitmap& src_img);
      void copy_from (const bitmap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void stretch_from (const bitmap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

      static bitmap_info convert (const blob& src,
                                  const bitmap_info& bmi,
                                  blob& dst,
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
      datamap (uint32_t w, uint32_t h);
      datamap (uint32_t w, uint32_t h, uint32_t bpl, const blob& data);
      datamap (const core::size& sz);

      void operator= (const datamap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (datamap&& rhs);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);

      void copy_from (const datamap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void stretch_from (const datamap& src);
      void stretch_from (const datamap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

      void adjust_brightness (float f);

      datamap sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      datamap brightness (float f) const;

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
      memmap (uint32_t w, uint32_t h);
      memmap (const core::size& sz);

      void operator= (const memmap& rhs);
      void operator= (const bitmap& rhs);
      void operator= (memmap&& rhs);

      operator os::drawable() const;

      void create (uint32_t w, uint32_t h);
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
    struct frame_image {
      frame_image (const core::rectangle& r, const bitmap& img, uint32_t edge);
      frame_image (const core::rectangle& r, const bitmap& img, uint32_t horizontal, uint32_t vertical);
      frame_image (const core::rectangle& r, const bitmap& img, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);

      void operator() (const graphics&, const core::point&) const;

    private:
      const core::rectangle rect;
      const bitmap& img;

      uint32_t left;
      uint32_t top;
      uint32_t right;
      uint32_t bottom;

      void operator= (frame_image&) = delete;
    };

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    namespace copy {

      template<>
      inline void row<BPP::BW> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          byte b = convert::bpp::get<BPP::BW>(src, src_x0 + x);
          convert::bpp::set<BPP::BW>(dst, dest_x0 + x, b);
        }
      }

      template<>
      inline void row<BPP::GRAY> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0, src + src_x0, dest_w);
      }

      template<>
      inline void row<BPP::RGB> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 3, src + src_x0 * 3, dest_w * 3);
      }

      template<>
      inline void row<BPP::RGBA> (cbyteptr src, byteptr dst, uint32_t src_x0, uint32_t dest_x0, uint32_t dest_w) {
        memcpy(dst + dest_x0 * 4, src + src_x0 * 4, dest_w * 4);
      }

      template<BPP bpp>
      void sub (const blob& src_data,
                const bitmap_info& src_bmi,
                blob& dest_data,
                const bitmap_info& dest_bmi,
                uint32_t src_x0, uint32_t src_y0,
                uint32_t dest_x0, uint32_t dest_y0,
                uint32_t dest_w, uint32_t dest_h) {
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          cbyteptr src = src_data.data() + (src_y0 + y) * src_bmi.bytes_per_line;
          byteptr dst = dest_data.data() + (dest_y0 + y) * dest_bmi.bytes_per_line;
          row<bpp>(src, dst, src_x0, dest_x0, dest_w);
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    namespace stretch {

      template<BPP bpp>
      void sub (const blob& src_data, const bitmap_info& src_bmi,
                blob& dest_data, const bitmap_info& dest_bmi,
                uint32_t src_x0, uint32_t src_y0, uint32_t src_w, uint32_t src_h,
                uint32_t dest_x0, uint32_t dest_y0, uint32_t dest_w, uint32_t dest_h) {
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          const uint32_t src_y = src_y0 + y * src_h / dest_h;
          const uint32_t src_offs = src_y * src_bmi.bytes_per_line;
          const uint32_t dest_offs = (dest_y0 + y) * dest_bmi.bytes_per_line;
          row<bpp>(src_data, dest_data, src_offs, dest_offs, src_x0, dest_x0, src_w, dest_w);
        }
      }

    } // namespace stretch

    namespace adjust {

      template<BPP bpp>
      void brightness_row (byteptr data, uint32_t offset, uint32_t w, float f);

      template<BPP bpp>
      void brightness (blob& data, const bitmap_info& bmi, float f) {
        for (uint_fast32_t y = 0; y < bmi.height; ++y) {
          const uint32_t offs = y * bmi.bytes_per_line;
          brightness_row<bpp>(data.data(), offs, bmi.width, f);
        }
      }

    }

    // --------------------------------------------------------------------------
    inline bitmap_info::bitmap_info ()
      : width(0)
      , height(0)
      , bytes_per_line(0)
      , bits_per_pixel(BPP::Undefined)
    {}

    inline bitmap_info::bitmap_info (uint32_t w, uint32_t h, uint32_t bpl, BPP bpp)
      : width(w)
      , height(h)
      , bytes_per_line(bpl)
      , bits_per_pixel(bpp)
    {}

    // --------------------------------------------------------------------------
    inline core::size bitmap_info::size () const {
      return {static_cast<core::size_type>(width), static_cast<core::size_type>(height)};
    }

    inline byte bitmap_info::depth () const {
      return static_cast<byte>(bits_per_pixel);
    }

    inline std::size_t bitmap_info::mem_size () const {
      return static_cast<size_t>(bytes_per_line) * static_cast<size_t>(height);
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
    inline datamap<T>::datamap (uint32_t w, uint32_t h) {
      create(w, h);
    }

    template<BPP T>
    inline datamap<T>::datamap (uint32_t w, uint32_t h, uint32_t bpl, const blob& data) {
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
    inline void datamap<T>::create (uint32_t w, uint32_t h) {
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

    template<BPP T>
    inline void datamap<T>::adjust_brightness (float f) {
      blob data;
      bitmap_info bmi;

      super::get_data(data, bmi);
      adjust::brightness<T>(data, bmi, f);
      super::put_data(data, bmi);
    }

    template<BPP T>
    inline auto datamap<T>::sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const -> datamap {
      datamap bmp(w, h);
      bmp.copy_from(*this, 
                    core::rectangle(core::point_type(x), core::point_type(y), core::size_type(w), core::size_type(h)),
                    core::point::zero);
      return bmp;
    }

    template<BPP T>
    inline auto datamap<T>::brightness (float f) const -> datamap {
      blob data;
      bitmap_info bmi;
      super::get_data(data, bmi);
      adjust::brightness<T>(data, bmi, f);
      datamap bmp;
      bmp.super::create(data, bmi);
      return bmp;
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

    inline memmap::memmap (uint32_t w, uint32_t h) {
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

    inline void memmap::create (uint32_t w, uint32_t h) {
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
