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

    template<PixelFormat T>
    class datamap;

    class GUIPP_DRAW_EXPORT basic_datamap {
    public:
      bool is_valid () const;

      basic_datamap ();

      operator bool () const;

      const bitmap_info& get_info () const;
      bitmap_info& get_info ();

      template<PixelFormat T>
      const datamap<T> convert () const;

      core::size size () const;
      byte depth () const;
      PixelFormat pixel_format () const;

      void clear ();

    protected:
      basic_datamap (const blob&, const bitmap_info&);
      basic_datamap (blob&&, bitmap_info&&);

      template<PixelFormat T>
      const const_image_data<T> get_data () const {
        return const_image_data<T>(basepp::array_wrapper<const byte>(data), info);
      }

      bitmap_info info;
      blob data;
    };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    class datamap : public basic_datamap {
    public:
      typedef basic_datamap super;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      static constexpr PixelFormat px_fmt = T;

      datamap ();

      datamap (uint32_t w, uint32_t h);
      datamap (const core::uint32_size& sz);
      datamap (const const_image_data<T>& data);
      datamap (const core::size& sz);
      datamap (const blob& data, const bitmap_info& bmi);
      datamap (blob&& data, bitmap_info&& bmi);

      template<PixelFormat S>
      datamap (const datamap<S>& src);

      template<PixelFormat S>
      datamap (const const_image_data<S>& src);

      void create (uint32_t w, uint32_t h);
      void create (const core::size& sz);
      void create (const core::uint32_size& sz);
      void create (const const_image_data<T>& data);

      void copy_from (const datamap& src_img,
                      const core::rectangle& src_rect,
                      const core::point& dest_pt);

      const const_image_data<T> get_data () const {
        return super::get_data<T>();
      }

      image_data<T> get_data () {
        return image_data<T>(basepp::array_wrapper<byte>(data), info);
      }

      void crop (uint32_t x, uint32_t y, uint32_t w, uint32_t h);

      void stretch_from (const datamap& src);
      void stretch_from (const datamap& src_img,
                         const core::rectangle& src_rect,
                         const core::rectangle& dest_rect);

      void adjust_brightness (float f);
      void invert ();

      void fill (const pixel_type&);

      datamap sub (uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;

      datamap brightness (float f) const;

    };

    // --------------------------------------------------------------------------
    typedef datamap<PixelFormat::BW> bwmap;
    typedef datamap<PixelFormat::GRAY> graymap;
    typedef datamap<PixelFormat::RGB> rgbmap;
    typedef datamap<PixelFormat::RGBA> rgbamap;
    typedef datamap<PixelFormat::BGR> bgrmap;
    typedef datamap<PixelFormat::BGRA> bgramap;
    typedef datamap<PixelFormat::ARGB> argbmap;
    typedef datamap<PixelFormat::ABGR> abgrmap;

    GUIPP_DRAW_EXPORT core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::rectangle& area);
    GUIPP_DRAW_EXPORT core::basic_rectangle<uint32_t> checked_area (const bitmap_info& bmi, const core::point& pt, const core::uint32_size& sz);

  } //namespace draw

} // namespace gui

#include <gui/draw/datamap.inl>
