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
#include <gui/core/array_wrapper.h>
#include <gui/draw/bitmap_info.h>


namespace gui {

  namespace draw {

    // --------------------------------------------------------------------------
    enum class bw_pixel : bool {
      black = system_bw_bits::black,
      white = system_bw_bits::white
    };

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct rgb_pixel;
    struct rgba_pixel;

    // --------------------------------------------------------------------------
    struct gray_pixel {
      byte value;

      gray_pixel operator= (bw_pixel bw);
      gray_pixel operator= (rgb_pixel rgb);
      gray_pixel operator= (rgba_pixel rgba);

      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgb_pixel {
      gray_pixel red;
      gray_pixel green;
      gray_pixel blue;

      void operator= (bw_pixel bw);
      void operator= (gray_pixel gray);
      void operator= (rgba_pixel rgba);

      gray_pixel get_gray () const;
      bw_pixel get_bw () const;
    };

    // --------------------------------------------------------------------------
    struct rgba_pixel : public rgb_pixel {
      gray_pixel alpha;

      rgba_pixel ();
      rgba_pixel (gray_pixel r, gray_pixel g, gray_pixel b, gray_pixel a);
      rgba_pixel (gray_pixel r, gray_pixel g, gray_pixel b);

      void operator= (bw_pixel bw);
      void operator= (gray_pixel gray);
      void operator= (rgb_pixel rgb);
    };
#pragma pack(pop)

    // --------------------------------------------------------------------------
    template<BPP T> struct BPP2Pixel {};
    template<> struct BPP2Pixel<BPP::BW>    { using pixel = bw_pixel; };
    template<> struct BPP2Pixel<BPP::GRAY>  { using pixel = gray_pixel ; };
    template<> struct BPP2Pixel<BPP::RGB>   { using pixel = rgb_pixel; };
    template<> struct BPP2Pixel<BPP::RGBA>  { using pixel = rgba_pixel; };

    // --------------------------------------------------------------------------
    template<BPP T>
    using image_row = core::array_wrapper<typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<byte>;
      using row_type = core::array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info);
      row_type row (uint32_t y);
      pixel_type& pixel (uint32_t x, uint32_t y);

    private:
      raw_type data;
      bitmap_info info;
    };

    // --------------------------------------------------------------------------
    template<BPP T>
    using const_image_row = core::array_wrapper<const typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<BPP T>
    struct const_image_data {
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<const byte>;
      using row_type = core::array_wrapper<const pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      const_image_data (raw_type data, const bitmap_info& info);
      const row_type row (uint32_t y) const;
      pixel_type pixel (uint32_t x, uint32_t y) const;

    private:
      raw_type data;
      bitmap_info info;
    };

  } // namespace draw

} // namespace gui

#include <gui/draw/image_data.inl>
