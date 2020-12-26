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
#include <gui/draw/datamap.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT basic_map {
    public:
      explicit basic_map (os::bitmap id);

      ~basic_map ();

      operator const os::drawable() const;

      void clear ();

      bool is_valid () const;
      operator bool () const;

      os::bitmap& get_id ();
      const os::bitmap& get_id () const;

      bitmap_info get_info () const;

      core::native_size native_size () const;

      core::size scaled_size () const;

      byte depth () const;
      pixel_format_t pixel_format () const;

      basic_map& operator= (const basic_map& rhs);
      basic_map& operator= (basic_map&& rhs);

      static uint32_t calc_bytes_per_line (uint32_t w, pixel_format_t px_fmt);

    protected:
      basic_map ();
      basic_map (const basic_map&);
      basic_map (basic_map&& rhs) noexcept ;

      void create (const bitmap_info& bmi);

      void set_id (os::bitmap);

    private:
      os::bitmap id;
    };

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT bitmap : public basic_map {
    public:
      typedef basic_map super;

      bitmap () = default;

      bitmap (const bwmap& rhs);
      bitmap (uint32_t w, uint32_t h);
      explicit bitmap (const core::native_size& sz);
      explicit bitmap (const core::size& sz);

      void create (uint32_t w, uint32_t h);
      void create (const core::native_size& sz);
      void create (const core::size& sz);

      void copy_from (const bitmap& src_img,
                      const core::native_rect& src_rect,
                      const core::native_point& dest_pt);

      void invert ();

      void operator= (const bwmap& rhs);

      bwmap get () const;
      operator bwmap () const;

    };

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT pixmap : public basic_map {
    public:
      typedef basic_map super;

      pixmap () = default;

      template<pixel_format_t T>
      pixmap (const datamap<T>& rhs);

      template<pixel_format_t T>
      pixmap (const const_image_data<T>& rhs);

      pixmap (uint32_t w, uint32_t h);
      pixmap (const core::native_size& sz);
      pixmap (const core::size& sz);

      void create (uint32_t w, uint32_t h);
      void create (const core::native_size& sz);
      void create (const core::size& sz);

      void copy_from (const pixmap& src_img,
                      const core::native_rect& src_rect,
                      const core::native_point& dest_pt);

      void invert ();

      template<pixel_format_t T>
      pixmap& operator= (const datamap<T>& rhs);

      template<pixel_format_t T>
      datamap<T> get () const;

      template<pixel_format_t T>
      operator datamap<T> () const;

      bwmap get_mask (pixel::gray limit = {0x7F}) const;

      static byte default_depth ();
      static pixel_format_t default_pixel_format ();

      template<pixel_format_t T>
      void put (const datamap<T>& rhs);

    private:
      template<pixel_format_t T>
      void invert ();

      void put_raw (cbyteptr data, const draw::bitmap_info& bmi);
      void get_raw (blob& data, draw::bitmap_info& bmi) const;

    };

    // --------------------------------------------------------------------------
    class GUIPP_DRAW_EXPORT masked_bitmap {
    public:
      typedef pixmap super;

      masked_bitmap () = default;

      /// Copy/Move
      masked_bitmap (const masked_bitmap&) = default;
      masked_bitmap (masked_bitmap&&) = default;

      /**
       * Create from B/W Bitmap.
       * White will be transparent, black will be black.
       */
      explicit masked_bitmap (const bitmap& rhs);
      explicit masked_bitmap (bitmap&& mask);

      /**
       * Create from colored Pixmap.
       * Black will be transparent, all other will be painted.
       */
      explicit masked_bitmap (const pixmap& rhs);
      explicit masked_bitmap (pixmap&& rhs);

      /**
       * Create from colored Pixmap with bitmap mask.
       * Bitmap black will be transparent, Bitmap white will be painted from pixmap.
       */
      masked_bitmap (const pixmap& bmp, const bitmap& mask);
      masked_bitmap (pixmap&& bmp, bitmap&& mask);

      masked_bitmap& operator= (const pixmap& bmp);
      masked_bitmap& operator= (pixmap&& bmp);

      masked_bitmap& operator= (const masked_bitmap& rhs);
      masked_bitmap& operator= (masked_bitmap&&);

      masked_bitmap& operator= (const bitmap& bmp);
      masked_bitmap& operator= (bitmap&& bmp);

      core::native_size native_size () const;

      core::size scaled_size () const;

      bool is_valid () const;

      operator bool () const;

      pixmap image;
      bitmap mask;
    };

  } // namespace draw

} // namespace gui

#include <gui/draw/bitmap.inl>
