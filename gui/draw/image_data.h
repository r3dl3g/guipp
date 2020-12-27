/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <gui/core/color.h>
#include <gui/draw/bitmap_info.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace pixel {

    // --------------------------------------------------------------------------
    enum class mono : bool {
      black = core::system_bw_bits::black,
      white = core::system_bw_bits::white
    };

    // --------------------------------------------------------------------------
    template<bool W>
    struct mono_colors {};

    template<>
    struct mono_colors<false> {
      static constexpr mono value[2] = {mono::white, mono::black};
    };

    template<>
    struct mono_colors<true> {
      static constexpr mono value[2] = {mono::black, mono::white};
    };

    using system_mono_colors = mono_colors<core::os::bitmap_bit_white>;

#pragma pack(push, 1)

    // --------------------------------------------------------------------------
    struct gray {
      typedef byte type;

      type value;

      template<typename T>
      static gray build(T t);

      template<typename T>
      gray& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct rgb {

      byte blue;
      byte green;
      byte red;

      template<typename T>
      static rgb build(T t);

      template<typename T>
      rgb& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct rgba {

      byte blue;
      byte green;
      byte red;
      byte alpha;

      template<typename T>
      static rgba build(T t);

      template<typename T>
      rgba& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct bgr {

      byte red;
      byte green;
      byte blue;

      template<typename T>
      static bgr build(T t);

      template<typename T>
      bgr& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct bgra {

      byte red;
      byte green;
      byte blue;
      byte alpha;

      template<typename T>
      static bgra build(T t);

      template<typename T>
      bgra& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct argb {

      byte alpha;
      byte blue;
      byte green;
      byte red;

      template<typename T>
      static argb build(T t);

      template<typename T>
      argb& operator= (T);
    };

    // --------------------------------------------------------------------------
    struct abgr {

      byte alpha;
      byte red;
      byte green;
      byte blue;

      template<typename T>
      static abgr build(T t);

      template<typename T>
      abgr& operator= (T);
    };

#pragma pack(pop)

    // --------------------------------------------------------------------------
    template<typename T> struct is_rgb_type : public std::false_type  {};
    template<typename T> struct is_alpha_type : public std::false_type {};

    template<> struct is_rgb_type<mono> : public std::false_type {};
    template<> struct is_rgb_type<gray> : public std::false_type {};
    template<> struct is_rgb_type<rgb> : public std::true_type {};
    template<> struct is_rgb_type<rgba> : public std::true_type {};
    template<> struct is_alpha_type<rgba> : public std::true_type {};
    template<> struct is_rgb_type<bgr> : public std::true_type {};
    template<> struct is_rgb_type<bgra> : public std::true_type {};
    template<> struct is_alpha_type<bgra> : public std::true_type {};
    template<> struct is_rgb_type<argb> : public std::true_type {};
    template<> struct is_alpha_type<argb> : public std::true_type {};
    template<> struct is_rgb_type<abgr> : public std::true_type {};
    template<> struct is_alpha_type<abgr> : public std::true_type {};

    // --------------------------------------------------------------------------
    template<typename T>
    struct color {
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<mono> {
      static constexpr mono black = { mono::black };
      static constexpr mono white = { mono::white };
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<gray> {
      static constexpr gray black = { 0 };
      static constexpr gray white = { 0xff };
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<rgb> {
      static constexpr rgb black = {0, 0, 0};
      static constexpr rgb white = {0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<rgba> {
      static constexpr rgba black = {0, 0, 0, 0};
      static constexpr rgba white = {0xff, 0xff, 0xff, 0};
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<bgr> {
      static constexpr bgr black = {0, 0, 0};
      static constexpr bgr white = {0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<bgra> {
      static constexpr bgra black = {0, 0, 0, 0};
      static constexpr bgra white = {0xff, 0xff, 0xff, 0};
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<argb> {
      static constexpr argb black = {0, 0, 0, 0};
      static constexpr argb white = {0, 0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    template<>
    struct color<abgr> {
      static constexpr abgr black = {0, 0, 0, 0};
      static constexpr abgr white = {0, 0xff, 0xff, 0xff};
    };

    // --------------------------------------------------------------------------
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const mono&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const gray&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const rgb&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const rgba&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bgr&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const bgra&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const argb&);
    GUIPP_DRAW_EXPORT std::ostream& operator<< (std::ostream& out, const abgr&);

    // --------------------------------------------------------------------------
    bool operator== (const gray&, const gray&);

    // --------------------------------------------------------------------------
    template<typename T, typename U,
             typename std::enable_if<is_alpha_type<T>::value &&
                                     is_alpha_type<U>::value>::type* = nullptr>
    inline bool operator== (const T& lhs, const U& rhs) {
      return (lhs.red == rhs.red) &&
             (lhs.green == rhs.green) &&
             (lhs.blue == rhs.blue) &&
             (lhs.alpha == rhs.alpha);
    }

    template<typename T, typename U,
             typename std::enable_if<is_rgb_type<T>::value &&  is_rgb_type<U>::value &&
                                     (!is_alpha_type<T>::value || !is_alpha_type<U>::value)>::type* = nullptr>
    inline bool operator== (const T& lhs, const U& rhs) {
      return (lhs.red == rhs.red) && (lhs.green == rhs.green) && (lhs.blue == rhs.blue);
    }

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
    inline rgb make_pixel_from_rgb<rgb> (byte r, byte g, byte b) {
      return rgb{b, g, r};
    }

    template<>
    inline bgr make_pixel_from_rgb<bgr> (byte r, byte g, byte b) {
      return bgr{r, g, b};
    }

    template<>
    inline bgra make_pixel_from_rgba<bgra> (byte r, byte g, byte b, byte a) {
      return bgra{r, g, b, a};
    }

    template<>
    inline rgba make_pixel_from_rgba<rgba> (byte r, byte g, byte b, byte a) {
      return rgba{b, g, r, a};
    }

    template<>
    inline argb make_pixel_from_rgba<argb> (byte r, byte g, byte b, byte a) {
      return argb{a, b, g, r};
    }

    template<>
    inline abgr make_pixel_from_rgba<abgr> (byte r, byte g, byte b, byte a) {
      return abgr{a, r, g, b};
    }

    // --------------------------------------------------------------------------
    GUIPP_DRAW_EXPORT mono operator+ (const mono&, const mono&);
    GUIPP_DRAW_EXPORT gray operator+ (const gray&, const gray&);

    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value && !is_alpha_type<T>::value>::type* = nullptr>
    inline T operator+ (const T& lhs, const T& rhs) {
      return make_pixel_from_rgb<T>(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue);
    }

    template<typename T, typename std::enable_if<is_alpha_type<T>::value>::type* = nullptr>
    inline T operator+ (const T& lhs, const T& rhs) {
      return make_pixel_from_rgba<T>(lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue, lhs.alpha + rhs.alpha);
    }

    // --------------------------------------------------------------------------
    mono operator* (mono p, double f);
    byte pixel_mul (byte p, double f);
    gray operator* (gray p, double f);
    mono operator* (double f, mono p);
    byte pixel_mul (double f, byte p);
    gray operator* (double f, gray p);

    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value && !is_alpha_type<T>::value>::type* = nullptr>
    inline T operator* (const T& p, double f) {
      return make_pixel_from_rgb<T>(pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f));
    }

    template<typename T, typename std::enable_if<is_alpha_type<T>::value>::type* = nullptr>
    inline T operator+ (const T& p, double f) {
      return make_pixel_from_rgba<T>(pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f), p.alpha);
    }

    // --------------------------------------------------------------------------
//    template<typename T> mono get_bw (T);
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
    inline os::color get_color<mono> (mono px) {
      return gui::color::system_bw_colors::value[static_cast<int>(px)];
    }

    template<>
    inline os::color get_color<core::bit_wrapper<const mono>> (core::bit_wrapper<const mono> w) {
      return get_color<mono>(static_cast<mono>(w));
    }

    template<>
    inline os::color get_color<gray> (gray px) {
      return gui::color::calc_rgb_gray(px.value);
    }

    template<>
    inline os::color get_color<os::color> (os::color px) {
      return px;
    }

    // --------------------------------------------------------------------------
    template<typename T = float>
    struct rgb_t {
      T blue;
      T green;
      T red;

      template<typename V>
      inline rgb_t (const V& rhs)
        : blue(rhs.blue)
        , green(rhs.green)
        , red(rhs.red)
      {}

      inline rgb_t (T b, T g, T r)
        : blue(b)
        , green(g)
        , red(r)
      {}

      inline rgb_t operator* (T f) const {
        return {blue * f, green * f, red * f};
      }

      inline rgb_t operator+ (const rgb_t& rhs) const {
        return {blue + rhs.blue, green + rhs.green, red + rhs.red};
      }

      template<typename U,
               typename std::enable_if<pixel::is_rgb_type<U>::value &&
                                       !pixel::is_alpha_type<U>::value>::type* = nullptr>
      operator U () const {
        return pixel::make_pixel_from_rgb<U>(static_cast<byte>(std::min(red, T(255))),
                                             static_cast<byte>(std::min(green, T(255))),
                                             static_cast<byte>(std::min(blue, T(255))));
      }

      template<typename U,
               typename std::enable_if<pixel::is_alpha_type<U>::value>::type* = nullptr>
      operator U () const {
        return pixel::make_pixel_from_rgba<U>(static_cast<byte>(std::min(red, T(255))),
                                              static_cast<byte>(std::min(green, T(255))),
                                              static_cast<byte>(std::min(blue, T(255))),
                                              0);
      }

    };

    // --------------------------------------------------------------------------

  } // namespace pixel

  namespace draw {

    // --------------------------------------------------------------------------
    template<pixel_format_t T> struct BPP2Pixel {};

    template<> struct BPP2Pixel<pixel_format_t::BW>    { using pixel = pixel::mono; };
    template<> struct BPP2Pixel<pixel_format_t::GRAY>  { using pixel = pixel::gray ; };
    template<> struct BPP2Pixel<pixel_format_t::RGB>   { using pixel = pixel::rgb; };
    template<> struct BPP2Pixel<pixel_format_t::RGBA>  { using pixel = pixel::rgba; };
    template<> struct BPP2Pixel<pixel_format_t::BGR>   { using pixel = pixel::bgr; };
    template<> struct BPP2Pixel<pixel_format_t::BGRA>  { using pixel = pixel::bgra; };
    template<> struct BPP2Pixel<pixel_format_t::ARGB>  { using pixel = pixel::argb; };
    template<> struct BPP2Pixel<pixel_format_t::ABGR>  { using pixel = pixel::abgr; };

    // --------------------------------------------------------------------------
    template<pixel_format_t F, typename S, typename T = typename BPP2Pixel<F>::pixel>
    struct to_pixel {
      static T to (S s) {
        T t;
        t = s;
        return t;
      }
    };

    // --------------------------------------------------------------------------
    struct image_data_base {
      inline explicit image_data_base (const bitmap_info& info)
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
    template<pixel_format_t T>
    struct const_image_data : public image_data_base {
      using super = image_data_base;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<const byte>;
      using row_type = core::array_wrapper<const pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      const_image_data (raw_type data, const bitmap_info& info);

      const row_type row (uint32_t y) const;

      pixel_type pixel (uint32_t x, uint32_t y) const;

      const raw_type& raw_data () const;

    private:
      raw_type data;

    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    struct image_data : public image_data_base {
      using super = image_data_base;
      using pixel_type = typename BPP2Pixel<T>::pixel;
      using raw_type = core::array_wrapper<byte>;
      using row_type = core::array_wrapper<pixel_type>;
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
    struct image_data<pixel_format_t::BW> : public image_data_base {
      using super = image_data_base;
      using pixel_type = pixel::mono;
      using raw_type = core::array_wrapper<byte>;
      using row_type = core::bit_array_wrapper<pixel_type>;
      static constexpr size_t pixel_size = sizeof(pixel_type);

      image_data (raw_type data, const bitmap_info& info)
        : super(info)
        , data(data)
      {}

      row_type row (uint32_t y) {
        byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
        using raw_t = typename row_type::type;
        return {reinterpret_cast<raw_t*>(row), width()};
      }

      core::bit_wrapper<pixel_type> pixel (uint32_t x, uint32_t y) {
        return row(y)[x];
      }

      raw_type& raw_data () {
        return data;
      }

      image_data& operator= (const image_data&);
      image_data& operator= (const const_image_data<pixel_format_t::BW>&);

    private:
      raw_type data;

    };

  } // namespace draw

} // namespace gui

#include <gui/draw/image_data.inl>
