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
#include <gui/core/color.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace pixel {

    // --------------------------------------------------------------------------
    enum class bw_pixel : bool {
      black = false,
      white = true
    };

    std::ostream& operator<< (std::ostream& out, const bw_pixel&);

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct gray_pixel {
      static const gray_pixel black;
      static const gray_pixel white;

      byte value;

      gray_pixel (byte value = 0);

      template<typename T>
      gray_pixel (T);

      bool operator== (const gray_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const gray_pixel&);

    // --------------------------------------------------------------------------
    struct rgb_pixel {
      static const rgb_pixel black;
      static const rgb_pixel white;

      byte red;
      byte green;
      byte blue;

      rgb_pixel (byte r, byte g, byte b);

      template<typename T>
      rgb_pixel (T);

      bool operator== (const rgb_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const rgb_pixel&);

    // --------------------------------------------------------------------------
    struct rgba_pixel {
      static const rgba_pixel black;
      static const rgba_pixel white;

      byte red;
      byte green;
      byte blue;
      byte alpha;

      rgba_pixel (byte r, byte g, byte b, byte a);

      template<typename T>
      rgba_pixel (T);

      bool operator== (const rgba_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const rgba_pixel&);

    // --------------------------------------------------------------------------
    struct bgra_pixel;

    struct bgr_pixel {
      static const bgr_pixel black;
      static const bgr_pixel white;

      byte blue;
      byte green;
      byte red;

      bgr_pixel (byte, byte, byte);

      template<typename T>
      bgr_pixel (T);

      bool operator== (const bgr_pixel&) const;
      bool operator== (const bgra_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const bgr_pixel&);

    // --------------------------------------------------------------------------
    struct bgra_pixel {
      static const bgra_pixel black;
      static const bgra_pixel white;

      byte blue;
      byte green;
      byte red;
      byte alpha;

      bgra_pixel (byte, byte, byte, byte);

      template<typename T>
      bgra_pixel (T);

      bool operator== (const bgra_pixel&) const;
      bool operator== (const bgr_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const bgra_pixel&);

    // --------------------------------------------------------------------------
    struct argb_pixel {
      static const argb_pixel black;
      static const argb_pixel white;

      byte alpha;
      byte red;
      byte green;
      byte blue;

      argb_pixel (byte, byte, byte, byte);

      template<typename T>
      argb_pixel (T);

      bool operator== (const argb_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const argb_pixel&);

    // --------------------------------------------------------------------------
    struct abgr_pixel {
      static const abgr_pixel black;
      static const abgr_pixel white;

      byte alpha;
      byte blue;
      byte green;
      byte red;

      abgr_pixel (byte, byte, byte, byte);

      template<typename T>
      abgr_pixel (T);

      bool operator== (const abgr_pixel&) const;

      template<typename T>
      void operator= (T);
    };

    std::ostream& operator<< (std::ostream& out, const abgr_pixel&);

#pragma pack(pop)

    bw_pixel get_bw (bw_pixel);
    byte get_gray (bw_pixel);
    byte get_red (bw_pixel);
    byte get_green (bw_pixel);
    byte get_blue (bw_pixel);
    byte get_alpha (bw_pixel);

    bw_pixel get_bw (gray_pixel);
    byte get_gray (gray_pixel);
    byte get_red (gray_pixel);
    byte get_green (gray_pixel);
    byte get_blue (gray_pixel);
    byte get_alpha (gray_pixel);

    template<typename T> bw_pixel get_bw (T);
    template<typename T> byte get_gray (T);
    template<typename T> byte get_red (T);
    template<typename T> byte get_green (T);
    template<typename T> byte get_blue (T);
    template<typename T> byte get_alpha (T);

  } // namespace pixel

  namespace draw {

    // --------------------------------------------------------------------------
    template<PixelFormat T> struct BPP2Pixel {};
    template<> struct BPP2Pixel<PixelFormat::BW>    { using pixel = pixel::bw_pixel; };
    template<> struct BPP2Pixel<PixelFormat::GRAY>  { using pixel = pixel::gray_pixel ; };
    template<> struct BPP2Pixel<PixelFormat::RGB>   { using pixel = pixel::rgb_pixel; };
    template<> struct BPP2Pixel<PixelFormat::RGBA>  { using pixel = pixel::rgba_pixel; };
    template<> struct BPP2Pixel<PixelFormat::BGR>   { using pixel = pixel::bgr_pixel; };
    template<> struct BPP2Pixel<PixelFormat::BGRA>  { using pixel = pixel::bgra_pixel; };
    template<> struct BPP2Pixel<PixelFormat::ARGB>  { using pixel = pixel::argb_pixel; };
    template<> struct BPP2Pixel<PixelFormat::ABGR>  { using pixel = pixel::abgr_pixel; };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    using const_image_row = basepp::array_wrapper<const typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<PixelFormat T>
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
    template<PixelFormat T>
    using image_row = basepp::array_wrapper<typename BPP2Pixel<T>::pixel>;

    // --------------------------------------------------------------------------
    template<PixelFormat T>
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
