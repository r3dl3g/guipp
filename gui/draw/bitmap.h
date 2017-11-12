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
    class basic_map {
    public:
      explicit basic_map (os::bitmap id);

      ~basic_map ();

      operator os::drawable() const;

      void clear ();

      bool is_valid () const;
      operator bool () const;

      os::bitmap get_id () const;
      bitmap_info get_info () const;
      core::size size () const;
      byte depth () const;
      BPP bits_per_pixel () const;

      void invert ();

      void operator= (const basic_map& rhs);
      void operator= (basic_map&& rhs);

      void get_data (blob& data, bitmap_info& bmi) const;

      static uint32_t calc_bytes_per_line (uint32_t w, BPP bpp);

    protected:
      basic_map ();
      basic_map (const basic_map&);
      basic_map (basic_map&& rhs);

      void put_data (const blob& data, const bitmap_info& bmi);

      void create (uint32_t w, uint32_t h, BPP bpp);
      void create (const core::size& sz, BPP bpp);

      void copy_from (const basic_map& src_img);
      void copy_from (const basic_map& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void set_id (os::bitmap);

    private:
      os::bitmap id;
    };

    // --------------------------------------------------------------------------
    class bitmap : public basic_map {
    public:
      typedef basic_map super;

      bitmap ();

      bitmap (const bwmap& sz);
      bitmap (uint32_t w, uint32_t h);
      bitmap (const core::size& sz);

      void operator= (const bwmap& rhs);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);

      void copy_from (const bitmap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      bwmap get () const;
      operator bwmap () const;

    };

    // --------------------------------------------------------------------------
    class pixmap : public basic_map {
    public:
      typedef basic_map super;

      pixmap ();

      pixmap (const basic_datamap& rhs);
      pixmap (uint32_t w, uint32_t h);
      pixmap (const core::size& sz);

      void operator= (const basic_datamap& rhs);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);

      void copy_from (const pixmap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      template<BPP T>
      datamap<T> get () const;

      template<BPP T>
      operator datamap<T> () const;

    private:
      void put (const basic_datamap& rhs);
    };

    // --------------------------------------------------------------------------
    class masked_bitmap {
    public:
      typedef pixmap super;

      masked_bitmap ();

      masked_bitmap (const masked_bitmap&);
      masked_bitmap (masked_bitmap&&);

      masked_bitmap (const pixmap& bmp);
      masked_bitmap (pixmap&& bmp);

      masked_bitmap (const pixmap& bmp, const bitmap& mask);
      masked_bitmap (pixmap&& bmp, bitmap&& mask);

      void operator= (const masked_bitmap& rhs);
      void operator= (masked_bitmap&&);

      void operator= (const pixmap& bmp);
      void operator= (pixmap&& bmp);

      core::size size () const;

      bool is_valid () const;

      operator bool () const;

      pixmap image;
      bitmap mask;
    };

    // --------------------------------------------------------------------------
    class graphics;

    // --------------------------------------------------------------------------
    struct frame_image {
      frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t edge);
      frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t horizontal, uint32_t vertical);
      frame_image (const core::rectangle& r, const basic_datamap& img, uint32_t left, uint32_t top, uint32_t right, uint32_t bottom);

      void operator() (const graphics&, const core::point&) const;

    private:
      const core::rectangle rect;
      const basic_datamap& img;

      uint32_t left;
      uint32_t top;
      uint32_t right;
      uint32_t bottom;

      void operator= (frame_image&) = delete;
    };

  } // namespace draw

} // namespace gui

#include <gui/draw/bitmap.inl>
