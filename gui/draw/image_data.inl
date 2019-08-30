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


namespace basepp {

  template<>
  struct array_wrapper<gui::pixel::bw_pixel> : public bit_array_wrapper<gui::pixel::bw_pixel> {
    typedef bit_array_wrapper<gui::pixel::bw_pixel> super;
    typedef super::type type;

    inline array_wrapper (type* data, size_t size)
      : super(data, size)
    {}

    inline array_wrapper (std::vector<type>& data)
      : super(data)
    {}

    inline array_wrapper<gui::pixel::bw_pixel> sub (size_t offset, size_t n) {
      return array_wrapper<gui::pixel::bw_pixel>(data_ + offset / 8, n);
    }
  };

  template<>
  struct array_wrapper<gui::pixel::bw_pixel const> : public bit_array_wrapper<gui::pixel::bw_pixel const> {
    typedef bit_array_wrapper<gui::pixel::bw_pixel const> super;
    typedef super::type type;

    inline array_wrapper (const type* data, size_t size)
      : super(data, size)
    {}

    inline array_wrapper (const std::vector<type>& data)
      : super(data)
    {}

    inline array_wrapper<const gui::pixel::bw_pixel> sub (size_t offset, size_t n) const {
      return array_wrapper<gui::pixel::bw_pixel const>(data_ + offset / 8, n);
    }
  };

} // namespace basepp

namespace gui {

  namespace pixel {
    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_gray (T p) {
      return static_cast<byte>((static_cast<uint16_t>(p.red) +
                                static_cast<uint16_t>(p.green) +
                                static_cast<uint16_t>(p.blue)) / 3);
    }

    inline byte get_gray (bw_pixel p) {
      return basepp::system_bw_bits::value[static_cast<byte>(p)];
    }

    inline byte get_gray (gray_pixel p) {
      return p.value;
    }

    inline byte get_gray (os::color c) {
      return gui::color::calc_medium_gray(c);
    }

    inline byte get_gray (basepp::bit_wrapper<const bw_pixel> p) {
      return get_gray(static_cast<bw_pixel>(p));
    }

        // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, bw_pixel>::type get_bw (T p) {
      return get_gray(p) == 0 ? bw_pixel::black : bw_pixel::white;
    }

    inline bw_pixel get_bw (bw_pixel p) {
      return p;
    }

    inline bw_pixel get_bw (gray_pixel p) {
      return p.value == 0 ? bw_pixel::black : bw_pixel::white;
    }

    inline bw_pixel get_bw (os::color c) {
      return bw_pixel(gui::color::remove_transparency(c) != gui::color::black);
    }

    inline bw_pixel get_bw (basepp::bit_wrapper<const bw_pixel> p) {
      return static_cast<bw_pixel>(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_red (T p) {
      return p.red;
    }

    inline byte get_red (bw_pixel p) {
      return get_gray(p);
    }

    inline byte get_red (gray_pixel p) {
      return p.value;
    }

    inline byte get_red (os::color c) {
      return gui::color::get_red(c);
    }

    inline byte get_red (basepp::bit_wrapper<const bw_pixel> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_green (T p) {
      return p.green;
    }

    inline byte get_green (bw_pixel p) {
      return get_gray(p);
    }

    inline byte get_green (gray_pixel p) {
      return p.value;
    }

    inline byte get_green (os::color c) {
      return gui::color::get_green(c);
    }

    inline byte get_green (basepp::bit_wrapper<const bw_pixel> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_blue (T p) {
      return p.blue;
    }

    inline byte get_blue (bw_pixel p) {
      return get_gray(p);
    }

    inline byte get_blue (gray_pixel p) {
      return p.value;
    }

    inline byte get_blue (os::color c) {
      return gui::color::get_blue(c);
    }

    inline byte get_blue (basepp::bit_wrapper<const bw_pixel> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_alpha_type<T>::value, byte>::type get_alpha (T p) {
      return p.alpha;
    }

    template<typename T>
    inline typename std::enable_if<!is_alpha_type<T>::value, byte>::type get_alpha (T) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

    inline byte get_alpha (os::color c) {
      return gui::color::get_alpha(c);
    }

    inline byte get_alpha (basepp::bit_wrapper<const bw_pixel> p) {
      return 0;//IF_WIN32_ELSE(0, 255);
    }

    // --------------------------------------------------------------------------
    inline bool operator== (const gray_pixel& l, const gray_pixel& r) {
      return l.value == r.value;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    gray_pixel gray_pixel::build(T t) {
      gray_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void gray_pixel::operator= (T rhs) {
      value = get_gray(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    rgb_pixel rgb_pixel::build(T t) {
      rgb_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void rgb_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    rgba_pixel rgba_pixel::build(T t) {
      rgba_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void rgba_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
      alpha = get_alpha(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bgr_pixel bgr_pixel::build(T t) {
      bgr_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void bgr_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bgra_pixel bgra_pixel::build(T t) {
      bgra_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void bgra_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
      alpha = get_alpha(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    argb_pixel argb_pixel::build(T t) {
      argb_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void argb_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
      alpha = get_alpha(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    abgr_pixel abgr_pixel::build(T t) {
      abgr_pixel p;
       p = t;
       return p;
    }

    template<typename T>
    inline void abgr_pixel::operator= (T rhs) {
      red = get_red(rhs);
      green = get_green(rhs);
      blue = get_blue(rhs);
      alpha = get_alpha(rhs);
    }

    // --------------------------------------------------------------------------
    inline bw_pixel operator* (bw_pixel p, float f) {
      return f == 0 ? bw_pixel::black : p;
    }

    inline byte pixel_mul (byte p, float f) {
      return static_cast<byte>(std::min<int>(0xff, static_cast<int>(p * f)));
    }

    inline gray_pixel operator* (gray_pixel p, float f) {
      return {pixel_mul(p.value, f)};
    }

    inline rgb_pixel operator* (rgb_pixel p, float f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f)};
    }

    inline rgba_pixel operator* (rgba_pixel p, float f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f), p.alpha};
    }

    // --------------------------------------------------------------------------
    inline bw_pixel operator* (float f, bw_pixel p) {
      return f == 0 ? bw_pixel::black : p;
    }

    inline byte pixel_mul (float f, byte p) {
      return static_cast<byte>(std::min<int>(0xff, static_cast<int>(p * f)));
    }

    inline gray_pixel operator* (float f, gray_pixel p) {
      return {pixel_mul(p.value, f)};
    }

  } // namespace pixel

  namespace draw {

    // --------------------------------------------------------------------------
    template<typename S>
    struct to_pixel<PixelFormat::BW, S> {
      static pixel::bw_pixel to (S s) {
        return pixel::get_bw(s);
      }
    };

    template<typename S>
    struct to_pixel<PixelFormat::GRAY, S> {
      static pixel::gray_pixel to (S s) {
        return pixel::gray_pixel{pixel::get_gray(s)};
      }
    };

    template<typename S>
    struct to_pixel<PixelFormat::RGB, S> {
      static pixel::rgb_pixel to (S s) {
        pixel::rgb_pixel p;
        p = pixel::get_color(s);
        return p;
      }
    };

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    inline const_image_data<T>::const_image_data (raw_type data, const bitmap_info& info)
      : super(info)
      , data(data)
    {}

    template<PixelFormat T>
    inline auto const_image_data<T>::row (uint32_t y) const -> const row_type {
      const byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_type = const typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), width());
    }

    template<PixelFormat T>
    inline auto const_image_data<T>::pixel (uint32_t x, uint32_t y) const -> pixel_type {
      return row(y)[x];
    }

    template<PixelFormat T>
    inline auto const_image_data<T>::raw_data () const -> const raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------
    template<PixelFormat T>
    inline image_data<T>::image_data (raw_type data, const bitmap_info& info)
      : super(info)
      , data(data)
    {}

    template<PixelFormat T>
    inline auto image_data<T>::row (uint32_t y) -> row_type {
      byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_type = typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), width());
    }

    template<PixelFormat T>
    inline auto image_data<T>::pixel (uint32_t x, uint32_t y) -> pixel_type& {
      return row(y)[x];
    }

    template<PixelFormat T>
    inline auto image_data<T>::raw_data () -> raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------

    template<PixelFormat T>
    image_data<T>& image_data<T>::operator= (const image_data<T>& rhs) {
      if (get_info() == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), get_info().mem_size());
      } else {
        // copy row:row
        auto rows = std::min(height(), rhs.height());
        auto length = std::min(get_info().bytes_per_line, rhs.get_info().bytes_per_line);
        for (int y = 0; y < rows; ++y) {
          raw_data().sub(y, length).copy_from(rhs.raw_data().sub(y, length), length);
        }
      }
      return *this;
    }

    template<PixelFormat T>
    image_data<T>& image_data<T>::operator= (const const_image_data<T>& rhs) {
      if (get_info() == rhs.get_info()) {
        // copy 1:1
        data.copy_from(rhs.raw_data(), get_info().mem_size());
      } else {
        // copy row:row
        auto rows = std::min(height(), rhs.height());
        auto length = std::min(get_info().bytes_per_line, rhs.get_info().bytes_per_line);
        for (decltype(rows) y = 0; y < rows; ++y) {
          raw_data().sub(y, length).copy_from(rhs.raw_data().sub(y, length), length);
        }
      }
      return *this;
    }

  } //namespace draw

} // namespace gui
