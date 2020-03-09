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
#include <gui/draw/converter.h>


namespace gui {

  namespace draw {

    template<pixel_format_t T>
    class datamap;

    class GUIPP_DRAW_EXPORT basic_datamap {
    public:
      bool is_valid () const;

      basic_datamap () = default;

      operator bool () const;

      const bitmap_info& get_info () const;
      bitmap_info& get_info ();

      template<pixel_format_t T>
      const datamap<T> convert () const;

      core::native_size native_size () const;

      core::size scaled_size () const;

      byte depth () const;
      pixel_format_t pixel_format () const;

      void clear ();

    protected:
      basic_datamap (const blob&, const bitmap_info&);
      basic_datamap (blob&&, bitmap_info&&);

      template<pixel_format_t T>
      const const_image_data<T> get_data () const {
        return const_image_data<T>(core::array_wrapper<const byte>(data), info);
      }

      bitmap_info info;
      blob data;
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    class datamap : public basic_datamap {
    public:
      typedef basic_datamap super;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      static constexpr pixel_format_t px_fmt = T;

      datamap () = default;

      datamap (uint32_t w, uint32_t h);
      datamap (const core::native_size& sz);
      datamap (const const_image_data<T>& data);
      datamap (const core::size& sz);

      datamap (const blob& data, const bitmap_info& bmi);
      datamap (blob&& data, bitmap_info&& bmi);

      template<pixel_format_t S>
      datamap (const datamap<S>& src);

      template<pixel_format_t S>
      datamap (const const_image_data<S>& src);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);
      void create (const core::native_size& sz);
      void create (const const_image_data<T>& data);

      void copy_from (const datamap& src_img,
                      const core::native_rect& src_rect,
                      const core::native_point& dest_pt);

      const const_image_data<T> get_data () const {
        return super::get_data<T>();
      }

      image_data<T> get_data () {
        return image_data<T>(core::array_wrapper<byte>(data), info);
      }

      void crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h);

      template<convert::interpolation I = convert::interpolation::nearest>
      void stretch_from (const datamap& src);

      template<convert::interpolation I = convert::interpolation::nearest>
      void stretch_from (const datamap& src_img,
                         const core::native_rect& src_rect,
                         const core::native_rect& dest_rect);

      void adjust_brightness (float f);
      void invert ();

      void fill (const pixel_type&);

      datamap sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      datamap brightness (double f) const;

      datamap<pixel_format_t::BW> get_mask (pixel::gray limit = {0x7F}) const;
    };

    // --------------------------------------------------------------------------
    typedef datamap<pixel_format_t::BW> bwmap;
    typedef datamap<pixel_format_t::GRAY> graymap;
    typedef datamap<pixel_format_t::RGB> rgbmap;
    typedef datamap<pixel_format_t::RGBA> rgbamap;
    typedef datamap<pixel_format_t::BGR> bgrmap;
    typedef datamap<pixel_format_t::BGRA> bgramap;
    typedef datamap<pixel_format_t::ARGB> argbmap;
    typedef datamap<pixel_format_t::ABGR> abgrmap;

    GUIPP_DRAW_EXPORT core::native_rect checked_area (const bitmap_info& bmi, const core::native_rect& area);
    GUIPP_DRAW_EXPORT core::native_rect checked_area (const bitmap_info& bmi, const core::native_point& pt, const core::native_size& sz);

  } //namespace draw

} // namespace gui

#include <gui/draw/datamap.inl>
