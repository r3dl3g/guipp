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

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bw_pixel&);

    // --------------------------------------------------------------------------
    template<typename T>
    struct color {};

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct gray_pixel {

      byte value;

      template<typename T>
      static gray_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const gray_pixel&, const gray_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const gray_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<gray_pixel> {
      static constexpr gray_pixel black = { 0 };
      static constexpr gray_pixel white = { 0xff };
    };

    // --------------------------------------------------------------------------
    struct rgb_pixel {

      byte blue;
      byte green;
      byte red;

      template<typename T>
      static rgb_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const rgb_pixel&, const rgb_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const rgb_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<rgb_pixel> {
      static constexpr rgb_pixel black = {0, 0, 0};
      static constexpr rgb_pixel white = {0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    struct rgba_pixel {

      byte blue;
      byte green;
      byte red;
      byte alpha;

      template<typename T>
      static rgba_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const rgba_pixel&, const rgba_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const rgba_pixel&);

    // --------------------------------------------------------------------------
    bool operator== (const rgb_pixel&, const rgba_pixel&);
    bool operator== (const rgba_pixel&, const rgb_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<rgba_pixel> {
      static constexpr rgba_pixel black = {0, 0, 0, 0};
      static constexpr rgba_pixel white = {0xff, 0xff, 0xff, 0};
    };

    // --------------------------------------------------------------------------
    struct bgr_pixel {

      byte red;
      byte green;
      byte blue;

      template<typename T>
      static bgr_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const bgr_pixel&, const bgr_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bgr_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<bgr_pixel> {
      static constexpr bgr_pixel black = {0, 0, 0};
      static constexpr bgr_pixel white = {0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    struct bgra_pixel {

      byte red;
      byte green;
      byte blue;
      byte alpha;

      template<typename T>
      static bgra_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const bgra_pixel&, const bgra_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bgra_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<bgra_pixel> {
      static constexpr bgra_pixel black = {0, 0, 0, 0};
      static constexpr bgra_pixel white = {0xff, 0xff, 0xff, 0};
    };

    // --------------------------------------------------------------------------
    bool operator== (const bgr_pixel&, const bgra_pixel&);
    bool operator== (const bgra_pixel&, const bgr_pixel&);

    // --------------------------------------------------------------------------
    struct argb_pixel {

      byte alpha;
      byte blue;
      byte green;
      byte red;

      template<typename T>
      static argb_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const argb_pixel&, const argb_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const argb_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<argb_pixel> {
      static constexpr argb_pixel black = {0, 0, 0, 0};
      static constexpr argb_pixel white = {0, 0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    struct abgr_pixel {

      byte alpha;
      byte red;
      byte green;
      byte blue;

      template<typename T>
      static abgr_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const abgr_pixel&, const abgr_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const abgr_pixel&);

    // --------------------------------------------------------------------------
    template<>
    struct color<abgr_pixel> {
      static constexpr abgr_pixel black = {0, 0, 0, 0};
      static constexpr abgr_pixel white = {0, 0xff, 0xff, 0xff};
    };

#pragma pack(pop)
    // --------------------------------------------------------------------------

    bw_pixel get_bw (bw_pixel);
    byte get_gray (bw_pixel);
    byte get_red (bw_pixel);
    byte get_green (bw_pixel);
    byte get_blue (bw_pixel);
    GUIPP_DRAW_EXPORT byte get_alpha (bw_pixel);

    bw_pixel get_bw (gray_pixel);
    byte get_gray (gray_pixel);
    byte get_red (gray_pixel);
    byte get_green (gray_pixel);
    byte get_blue (gray_pixel);
    GUIPP_DRAW_EXPORT byte get_alpha (gray_pixel);

    GUIPP_DRAW_EXPORT byte get_alpha (rgb_pixel);
    GUIPP_DRAW_EXPORT byte get_alpha (bgr_pixel);

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

    // --------------------------------------------------------------------------
    template<>
    struct image_data<PixelFormat::BW> {
      using pixel_type = pixel::bw_pixel;
      using raw_type = basepp::array_wrapper<byte>;
      using row_type = basepp::bit_array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info)
        : data(data)
        , info(info)
      {}

      row_type row (uint32_t y) {
        byte* row = data.data(y * info.bytes_per_line, info.bytes_per_line);
        using raw_type = typename row_type::type;
        return std::move(row_type(reinterpret_cast<raw_type*>(row), info.width));
      }

      basepp::bit_wrapper<pixel_type> pixel (uint32_t x, uint32_t y) {
        return row(y)[x];
      }

      const bitmap_info& get_info () const {
        return info;
      }

      raw_type& raw_data () {
        return data;
      }

      image_data& operator= (const image_data&);
      image_data& operator= (const const_image_data<PixelFormat::BW>&);

    private:
      raw_type data;
      bitmap_info info;
    };

  } // namespace draw

} // namespace gui

#include <gui/draw/image_data.inl>
