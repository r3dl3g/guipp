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
#include <gui/draw/bitmap_info.h>


namespace gui {

  namespace draw {

    class basic_datamap {
    public:
      bool is_valid () const;

      operator bool () const;

      const bitmap_info& get_info () const;
      const blob& get_data () const;

      bitmap_info& get_info ();
      blob& get_data ();

      core::size size () const;
      byte depth () const;
      BPP bits_per_pixel () const;

      void create (const blob& data, const bitmap_info& bmi);
      void clear ();

    protected:
      bitmap_info info;
      blob data;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    class datamap : public basic_datamap {
    public:
      typedef basic_datamap super;
      static constexpr BPP bpp = T;

      datamap ();

      datamap (uint32_t w, uint32_t h);
      datamap (uint32_t w, uint32_t h, const blob& data);
      datamap (const core::size& sz);

      template<BPP S>
      datamap (const datamap<S>& src);

      datamap (const basic_datamap& src);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);
      void create (uint32_t w, uint32_t h, const blob& data);

      void copy_from (const datamap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      void crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      void stretch_from (const datamap& src);
      void stretch_from (const datamap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

      void adjust_brightness (float f);
      void invert ();

      datamap sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      datamap brightness (float f) const;

    };

    // --------------------------------------------------------------------------
    typedef datamap<BPP::BW> maskmap;
    typedef datamap<BPP::GRAY> graymap;
    typedef datamap<BPP::RGB> rgbmap;
    typedef datamap<BPP::RGBA> rgbamap;

  } //namespace draw

} // namespace gui

#include <gui/draw/datamap.inl>
