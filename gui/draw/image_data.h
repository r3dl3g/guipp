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
    template<typename T>
    struct color {
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct is_rgb_type {};

    template<typename T>
    struct is_alpha_type : public std::false_type {};

    // --------------------------------------------------------------------------
    enum class bw_pixel : bool {
      black = false,
      white = true
    };

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bw_pixel&);
    GUIPP_DRAW_EXPORT bw_pixel operator+ (const bw_pixel&, const bw_pixel&);

    template<> struct is_rgb_type<bw_pixel> : public std::false_type {};

    // --------------------------------------------------------------------------
    template<>
    struct color<bw_pixel> {
      static constexpr bw_pixel black = { bw_pixel::black };
      static constexpr bw_pixel white = { bw_pixel::white };
    };

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct gray_pixel {
      typedef byte type;

      type value;

      template<typename T>
      static gray_pixel build(T t);

      template<typename T>
      void operator= (T);
    };

    bool operator== (const gray_pixel&, const gray_pixel&);

    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const gray_pixel&);
    GUIPP_DRAW_EXPORT gray_pixel operator+ (const gray_pixel&, const gray_pixel&);

    template<> struct is_rgb_type<gray_pixel> : public std::false_type {};

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

    template<> struct is_rgb_type<rgb_pixel> : public std::true_type {};

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

    template<> struct is_rgb_type<rgba_pixel> : public std::true_type {};
    template<> struct is_alpha_type<rgba_pixel> : public std::true_type {};

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

    template<> struct is_rgb_type<bgr_pixel> : public std::true_type {};

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

    template<> struct is_rgb_type<bgra_pixel> : public std::true_type {};
    template<> struct is_alpha_type<bgra_pixel> : public std::true_type {};

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

    template<> struct is_rgb_type<argb_pixel> : public std::true_type {};
    template<> struct is_alpha_type<argb_pixel> : public std::true_type {};

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

    template<> struct is_rgb_type<abgr_pixel> : public std::true_type {};
    template<> struct is_alpha_type<abgr_pixel> : public std::true_type {};

    // --------------------------------------------------------------------------
    template<>
    struct color<abgr_pixel> {
      static constexpr abgr_pixel black = {0, 0, 0, 0};
      static constexpr abgr_pixel white = {0, 0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value && !is_alpha_type<T>::value>::type* = nullptr>
    inline T make_pixel_from_rgb (byte r, byte g, byte b) {
      return T{r, g, b};
    }

    template<typename T, typename std::enable_if<is_alpha_type<T>::value>::type* = nullptr>
    inline T make_pixel_from_rgba (byte r, byte g, byte b, byte a) {
      return T{r, g, b, a};
    }

    // --------------------------------------------------------------------------
    template<>
    inline rgb_pixel make_pixel_from_rgb<rgb_pixel> (byte r, byte g, byte b) {
      return rgb_pixel{b, g, r};
    }

    template<>
    inline bgr_pixel make_pixel_from_rgb<bgr_pixel> (byte r, byte g, byte b) {
      return bgr_pixel{r, g, b};
    }

    template<>
    inline bgra_pixel make_pixel_from_rgba<bgra_pixel> (byte r, byte g, byte b, byte a) {
      return bgra_pixel{r, g, b, a};
    }

    template<>
    inline rgba_pixel make_pixel_from_rgba<rgba_pixel> (byte r, byte g, byte b, byte a) {
      return rgba_pixel{b, g, r, a};
    }

    template<>
    inline argb_pixel make_pixel_from_rgba<argb_pixel> (byte r, byte g, byte b, byte a) {
      return argb_pixel{a, b, g, r};
    }

    template<>
    inline abgr_pixel make_pixel_from_rgba<abgr_pixel> (byte r, byte g, byte b, byte a) {
      return abgr_pixel{a, r, g, b};
    }

    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value && !is_alpha_type<T>::value>::type* = nullptr>
    inline T operator+ (const T& lhs, const T& rhs) {
      return make_pixel_from_rgb<T>(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue);
    }

    template<typename T, typename std::enable_if<is_alpha_type<T>::value>::type* = nullptr>
    inline T operator+ (const T& lhs, const T& rhs) {
      return make_pixel_from_rgba<T>(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue, lhs.alpha + rhs.alpha);
    }

#pragma pack(pop)
    // --------------------------------------------------------------------------
//    template<typename T> bw_pixel get_bw (T);
//    template<typename T> byte get_gray (T);
//    template<typename T> byte get_red (T);
//    template<typename T> byte get_green (T);
//    template<typename T> byte get_blue (T);
//    template<typename T> byte get_alpha (T);

    // --------------------------------------------------------------------------
    template<typename T>
    inline os::color get_color (T px) {
      return gui::color::calc_rgba(get_red(px), get_green(px), get_blue(px), get_alpha(px));
    }

    template<>
    inline os::color get_color<bw_pixel> (bw_pixel px) {
      return static_cast<bool>(px) ? gui::color::white : gui::color::black;
    }

    template<>
    inline os::color get_color<gray_pixel> (gray_pixel px) {
      return gui::color::calc_rgb_gray(px.value);
    }

    template<>
    inline os::color get_color<os::color> (os::color px) {
      return px;
    }

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
    template<PixelFormat F, typename S, typename T = typename BPP2Pixel<F>::pixel>
    struct to_pixel {
      static T to (S s) {
        T t;
        t = s;
        return t;
      }
    };

    // --------------------------------------------------------------------------
    struct image_data_base {
      inline image_data_base (const bitmap_info& info)
        : info(info)
      {}

      inline const bitmap_info& get_info () const {
        return info;
      }

      inline uint32_t width () const {
        return info.width;
      }

      inline uint32_t height () const {
        return info.height;
      }

    private:
      bitmap_info info;
    };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    struct const_image_data : public image_data_base {
      using super = image_data_base;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = basepp::array_wrapper<const byte>;
      using row_type = basepp::array_wrapper<const pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      const_image_data (raw_type data, const bitmap_info& info);

      const row_type row (uint32_t y) const;

      pixel_type pixel (uint32_t x, uint32_t y) const;

      const raw_type& raw_data () const;

    private:
      raw_type data;

    };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    struct image_data : public image_data_base {
      using super = image_data_base;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = basepp::array_wrapper<byte>;
      using row_type = basepp::array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info);

      row_type row (uint32_t y);

      pixel_type& pixel (uint32_t x, uint32_t y);

      raw_type& raw_data ();

      image_data& operator= (const image_data&);
      image_data& operator= (const const_image_data<T>&);

    private:
      raw_type data;

    };

    // --------------------------------------------------------------------------
    template<>
    struct image_data<PixelFormat::BW> : public image_data_base {
      using super = image_data_base;
      using pixel_type = pixel::bw_pixel;
      using raw_type = basepp::array_wrapper<byte>;
      using row_type = basepp::bit_array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info)
        : super(info)
        , data(data)
      {}

      row_type row (uint32_t y) {
        byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
        using raw_type = typename row_type::type;
        return std::move(row_type(reinterpret_cast<raw_type*>(row), width()));
      }

      basepp::bit_wrapper<pixel_type> pixel (uint32_t x, uint32_t y) {
        return row(y)[x];
      }

      raw_type& raw_data () {
        return data;
      }

      image_data& operator= (const image_data&);
      image_data& operator= (const const_image_data<PixelFormat::BW>&);

    private:
      raw_type data;

    };

  } // namespace draw

} // namespace gui

#include <gui/draw/image_data.inl>
