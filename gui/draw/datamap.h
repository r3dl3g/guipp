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
#include <gui/draw/image_data.h>


namespace gui {

  namespace draw {

    template<BPP T>
    class datamap;

    class basic_datamap {
    public:
      bool is_valid () const;

      operator bool () const;

      const bitmap_info& get_info () const;
      bitmap_info& get_info ();

      template<BPP T>
      const datamap<T> convert () const;

      core::size size () const;
      byte depth () const;
      BPP bits_per_pixel () const;

      void clear ();

    protected:
      void create (const blob& data, const bitmap_info& bmi);

      template<BPP T>
      const const_image_data<T> get_raw () const {
        return const_image_data<T>(core::array_wrapper<const byte>(data), info);
      }

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
      datamap (const core::uint32_size& sz);
      datamap (const const_image_data<T>& data);
      datamap (const core::size& sz);

      template<BPP S>
      datamap (const datamap<S>& src);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);
      void create (const core::uint32_size& sz);
      void create (const const_image_data<T>& data);

      void copy_from (const datamap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      const const_image_data<T> get_raw () const {
        return super::get_raw<T>();
      }

      image_data<T> get_raw () {
        return image_data<T>(core::array_wrapper<byte>(data), info);
      }

      void crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h);

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
    typedef datamap<BPP::BW> bwmap;
    typedef datamap<BPP::GRAY> graymap;
    typedef datamap<BPP::RGB> rgbmap;
    typedef datamap<BPP::RGBA> rgbamap;

  } //namespace draw

} // namespace gui

#include <gui/draw/datamap.inl>
