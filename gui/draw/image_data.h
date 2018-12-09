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
 // Common includes
 //
#include <algorithm>
 
 // --------------------------------------------------------------------------
//
// Library includes
//
#include <base/array_wrapper.h>
#include <gui/draw/bitmap_info.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    enum class bw_pixel : bool {
      black = basepp::system_bw_bits::black,
      white = basepp::system_bw_bits::white
    };

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct rgb_pixel;
    struct rgba_pixel;
    struct bgr_pixel;
    struct bgra_pixel;
    struct argb_pixel;
    struct abgr_pixel;

    // --------------------------------------------------------------------------
    struct gray_pixel {
      byte value;

      gray_pixel operator= (bw_pixel);
      gray_pixel operator= (rgb_pixel);
      gray_pixel operator= (rgba_pixel);
      gray_pixel operator= (bgr_pixel);
      gray_pixel operator= (bgra_pixel);
      gray_pixel operator= (argb_pixel);
      gray_pixel operator= (abgr_pixel);

      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgb_pixel {
      byte red;
      byte green;
      byte blue;

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgba_pixel);
      void operator= (bgr_pixel);
      void operator= (bgra_pixel);
      void operator= (argb_pixel);
      void operator= (abgr_pixel);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgba_pixel : public rgb_pixel {
      byte alpha;

      rgba_pixel ();
      rgba_pixel (byte r, byte g, byte b, byte a);
      rgba_pixel (byte r, byte g, byte b);

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgb_pixel);
      void operator= (bgr_pixel);
      void operator= (bgra_pixel);
      void operator= (argb_pixel);
      void operator= (abgr_pixel);
    };

    // --------------------------------------------------------------------------
    struct bgr_pixel {
      byte blue;
      byte green;
      byte red;

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgb_pixel);
      void operator= (rgba_pixel);
      void operator= (bgra_pixel);
      void operator= (argb_pixel);
      void operator= (abgr_pixel);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct bgra_pixel : public bgr_pixel {
      byte alpha;

      bgra_pixel ();
      bgra_pixel (byte b, byte g, byte r, byte a);
      bgra_pixel (byte b, byte g, byte r);

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgb_pixel);
      void operator= (rgba_pixel);
      void operator= (bgr_pixel);
      void operator= (argb_pixel);
      void operator= (abgr_pixel);
    };

    // --------------------------------------------------------------------------
    struct argb_pixel {
      byte alpha;
      byte red;
      byte green;
      byte blue;

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgb_pixel);
      void operator= (rgba_pixel);
      void operator= (bgr_pixel);
      void operator= (bgra_pixel);
      void operator= (abgr_pixel);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct abgr_pixel {
      byte alpha;
      byte blue;
      byte green;
      byte red;

      void operator= (bw_pixel);
      void operator= (gray_pixel);
      void operator= (rgb_pixel);
      void operator= (rgba_pixel);
      void operator= (bgr_pixel);
      void operator= (bgra_pixel);
      void operator= (argb_pixel);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

#pragma pack(pop)

    // --------------------------------------------------------------------------
    template<BPP T> struct BPP2Pixel {};
    template<> struct BPP2Pixel<BPP::BW>    { using pixel = bw_pixel; };
    template<> struct BPP2Pixel<BPP::GRAY>  { using pixel = gray_pixel ; };
    template<> struct BPP2Pixel<BPP::RGB>   { using pixel = rgb_pixel; };
    template<> struct BPP2Pixel<BPP::RGBA>  { using pixel = rgba_pixel; };
    template<> struct BPP2Pixel<BPP::BGR>   { using pixel = bgr_pixel; };
    template<> struct BPP2Pixel<BPP::BGRA>  { using pixel = bgra_pixel; };
    template<> struct BPP2Pixel<BPP::ARGB>  { using pixel = argb_pixel; };
    template<> struct BPP2Pixel<BPP::ABGR>  { using pixel = abgr_pixel; };

    // --------------------------------------------------------------------------
    template<BPP T>
    using const_image_row = basepp::array_wrapper<const typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct const_image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = basepp::array_wrapper<const byte>;
      using row_type = basepp::array_wrapper<const pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      const_image_data (raw_type data, const bitmap_info& info);
      const row_type row (uint32_t y) const;
      pixel_type pixel (uint32_t x, uint32_t y) const;
      const bitmap_info& get_info () const;
      const raw_type& raw_data () const;

    private:
      raw_type data;
      bitmap_info info;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    using image_row = basepp::array_wrapper<typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = basepp::array_wrapper<byte>;
      using row_type = basepp::array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info);
      row_type row (uint32_t y);
      pixel_type& pixel (uint32_t x, uint32_t y);
      const bitmap_info& get_info () const;
      raw_type& raw_data ();

      image_data& operator= (const image_data&);
      image_data& operator= (const const_image_data<T>&);

    private:
      raw_type data;
      bitmap_info info;
    };

  } // namespace draw

} // namespace gui

#include <gui/draw/image_data.inl>
