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


namespace gui {

  namespace core {

    template<>
    struct array_wrapper<gui::pixel::mono> : public bit_array_wrapper<gui::pixel::mono> {
      typedef bit_array_wrapper<gui::pixel::mono> super;
      typedef super::type type;

      inline array_wrapper (type* data, size_t size)
        : super(data, size)
      {}

      inline array_wrapper (std::vector<type>& data)
        : super(data)
      {}

      inline array_wrapper<gui::pixel::mono> sub (size_t offset, size_t n) {
        return array_wrapper<gui::pixel::mono>(data_ + offset / 8, n);
      }
    };

    template<>
    struct array_wrapper<gui::pixel::mono const> : public bit_array_wrapper<gui::pixel::mono const> {
      typedef bit_array_wrapper<gui::pixel::mono const> super;
      typedef super::type type;

      inline array_wrapper (const type* data, size_t size)
        : super(data, size)
      {}

      inline array_wrapper (const std::vector<type>& data)
        : super(data)
      {}

      inline array_wrapper<const gui::pixel::mono> sub (size_t offset, size_t n) const {
        return array_wrapper<gui::pixel::mono const>(data_ + offset / 8, n);
      }
    };

  } // namespace core

  namespace pixel {
    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_gray (T p) {
      return static_cast<byte>((static_cast<uint16_t>(p.red) +
                                static_cast<uint16_t>(p.green) +
                                static_cast<uint16_t>(p.blue)) / 3);
    }

    inline byte get_gray (mono p) {
      return core::system_bw_bits::value[static_cast<byte>(p)];
    }

    inline byte get_gray (gray p) {
      return p.value;
    }

    inline byte get_gray (os::color c) {
      return gui::color::calc_medium_gray(c);
    }

    inline byte get_gray (core::bit_wrapper<const mono> p) {
      return get_gray(static_cast<mono>(p));
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, mono>::type get_bw (T p) {
      return get_gray(p) < 128 ? mono::black : mono::white;
    }

    inline mono get_bw (mono p) {
      return p;
    }

    inline mono get_bw (gray p) {
      return p.value < 128 ? mono::black : mono::white;
    }

    inline mono get_bw (os::color c) {
      return gui::color::calc_medium_gray(c) < 128 ? mono::black : mono::white;
    }

    inline mono get_bw (core::bit_wrapper<const mono> p) {
      return static_cast<mono>(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_red (T p) {
      return p.red;
    }

    inline byte get_red (mono p) {
      return get_gray(p);
    }

    inline byte get_red (gray p) {
      return p.value;
    }

    inline byte get_red (os::color c) {
      return gui::color::get_red(c);
    }

    inline byte get_red (core::bit_wrapper<const mono> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_green (T p) {
      return p.green;
    }

    inline byte get_green (mono p) {
      return get_gray(p);
    }

    inline byte get_green (gray p) {
      return p.value;
    }

    inline byte get_green (os::color c) {
      return gui::color::get_green(c);
    }

    inline byte get_green (core::bit_wrapper<const mono> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_rgb_type<T>::value, byte>::type get_blue (T p) {
      return p.blue;
    }

    inline byte get_blue (mono p) {
      return get_gray(p);
    }

    inline byte get_blue (gray p) {
      return p.value;
    }

    inline byte get_blue (os::color c) {
      return gui::color::get_blue(c);
    }

    inline byte get_blue (core::bit_wrapper<const mono> p) {
      return get_gray(p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline typename std::enable_if<is_alpha_type<T>::value, byte>::type get_alpha (T p) {
      return p.alpha;
    }

    template<typename T>
    inline typename std::enable_if<!is_alpha_type<T>::value, byte>::type get_alpha (T) {
      return IF_WIN32_ELSE(0, 255);
    }

    inline byte get_alpha (os::color c) {
      return gui::color::get_alpha(c);
    }

    inline byte get_alpha (core::bit_wrapper<const mono> p) {
      return IF_WIN32_ELSE(0, 255);
    }

    // --------------------------------------------------------------------------
    inline bool operator== (const gray& l, const gray& r) {
      return l.value == r.value;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    gray gray::build(T t) {
      gray p;
      p = t;
      return p;
    }

    template<typename T>
    inline void gray::operator= (T rhs) {
      value = get_gray(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T, typename S>
    typename std::enable_if<is_rgb_type<S>::value, byte>::type put_rgb (T& lhs, const S rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<!is_rgb_type<S>::value, byte>::type put_rgb (T& lhs, const S rhs) {
      lhs.red = lhs.green = lhs.blue = get_gray(rhs);
    }

    template<typename T>
    void put_rgb (T& lhs, const os::color rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<is_rgb_type<S>::value, byte>::type put_rgba (T& lhs, const S rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
      lhs.alpha = get_alpha(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<!is_rgb_type<S>::value, byte>::type put_rgba (T& lhs, const S rhs) {
      lhs.red = lhs.green = lhs.blue = get_gray(rhs);
      lhs.alpha = get_alpha(rhs);
    }

    template<typename T>
    void put_rgba (T& lhs, const os::color rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
      lhs.alpha = get_alpha(rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    rgb rgb::build(T t) {
      rgb p;
      p = t;
      return p;
    }

    template<typename T>
    inline void rgb::operator= (T rhs) {
      put_rgb(*this, rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    rgba rgba::build(T t) {
      rgba p;
      p = t;
      return p;
    }

    template<typename T>
    inline void rgba::operator= (T rhs) {
      put_rgba(*this, rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bgr bgr::build(T t) {
      bgr p;
      p = t;
      return p;
    }

    template<typename T>
    inline void bgr::operator= (T rhs) {
      put_rgb(*this, rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bgra bgra::build(T t) {
      bgra p;
      p = t;
      return p;
    }

    template<typename T>
    inline void bgra::operator= (T rhs) {
      put_rgba(*this, rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    argb argb::build(T t) {
      argb p;
      p = t;
      return p;
    }

    template<typename T>
    inline void argb::operator= (T rhs) {
      put_rgb(*this, rhs);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    abgr abgr::build(T t) {
      abgr p;
      p = t;
      return p;
    }

    template<typename T>
    inline void abgr::operator= (T rhs) {
      put_rgba(*this, rhs);
    }

    // --------------------------------------------------------------------------
    inline mono operator* (mono p, double f) {
      return f < 0.5F ? mono::black : p;
    }

    inline byte pixel_mul (byte p, double f) {
      return static_cast<byte>(std::min<int>(0xff, static_cast<int>(p * f)));
    }

    inline gray operator* (gray p, double f) {
      return {pixel_mul(p.value, f)};
    }

    inline rgb operator* (rgb p, double f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f)};
    }

    inline rgba operator* (rgba p, double f) {
      return {pixel_mul(p.red, f), pixel_mul(p.green, f), pixel_mul(p.blue, f), p.alpha};
    }

    // --------------------------------------------------------------------------
    inline mono operator* (double f, mono p) {
      return f == 0 ? mono::black : p;
    }

    inline byte pixel_mul (double f, byte p) {
      return static_cast<byte>(std::min<int>(0xff, static_cast<int>(p * f)));
    }

    inline gray operator* (double f, gray p) {
      return {pixel_mul(p.value, f)};
    }

  } // namespace pixel

  namespace draw {

    // --------------------------------------------------------------------------
    template<typename S>
    struct to_pixel<pixel_format_t::BW, S> {
      static pixel::mono to (S s) {
        return pixel::get_bw(s);
      }
    };

    template<typename S>
    struct to_pixel<pixel_format_t::GRAY, S> {
      static pixel::gray to (S s) {
        return pixel::gray{pixel::get_gray(s)};
      }
    };

    template<typename S>
    struct to_pixel<pixel_format_t::RGB, S> {
      static pixel::rgb to (S s) {
        pixel::rgb p;
        p = pixel::get_color(s);
        return p;
      }
    };

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline const_image_data<T>::const_image_data (raw_type data, const bitmap_info& info)
      : super(info)
      , data(data)
    {}

    template<pixel_format_t T>
    inline auto const_image_data<T>::row (uint32_t y) const -> const row_type {
      const byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_type = const typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), width());
    }

    template<pixel_format_t T>
    inline auto const_image_data<T>::pixel (uint32_t x, uint32_t y) const -> pixel_type {
      return row(y)[x];
    }

    template<pixel_format_t T>
    inline auto const_image_data<T>::raw_data () const -> const raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------
    template<pixel_format_t T>
    inline image_data<T>::image_data (raw_type data, const bitmap_info& info)
      : super(info)
      , data(data)
    {}

    template<pixel_format_t T>
    inline auto image_data<T>::row (uint32_t y) -> row_type {
      byte* row = data.data(y * get_info().bytes_per_line, get_info().bytes_per_line);
      using raw_type = typename row_type::type;
      return row_type(reinterpret_cast<raw_type*>(row), width());
    }

    template<pixel_format_t T>
    inline auto image_data<T>::pixel (uint32_t x, uint32_t y) -> pixel_type& {
      return row(y)[x];
    }

    template<pixel_format_t T>
    inline auto image_data<T>::raw_data () -> raw_type& {
      return data;
    }

    // --------------------------------------------------------------------------

    template<pixel_format_t T>
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

    template<pixel_format_t T>
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
