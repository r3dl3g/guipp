/**
 * @copyright (c) 2016-2025 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     direct access image data
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace pixel {
    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value, int>::type = 0>
    inline byte get_gray (T p) {
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

    inline byte get_gray (byte p) {
      return p;
    }

    inline byte get_gray (os::color c) {
      return gui::color::calc_medium_gray(c);
    }

    inline byte get_gray(uint32_t c) {
      return gui::color::calc_medium_gray(c);
    }

    inline byte get_gray (core::bit_wrapper<const mono> p) {
      return get_gray(static_cast<mono>(p));
    }

    // --------------------------------------------------------------------------
    template<typename T, typename std::enable_if<is_rgb_type<T>::value, int>::type = 0>
    inline mono get_bw (T p) {
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
    template<typename T, typename std::enable_if<is_rgb_type<T>::value, int>::type = 0>
    inline byte get_red (T p) {
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
    template<typename T, typename std::enable_if<is_rgb_type<T>::value, int>::type = 0>
    inline byte get_green (T p) {
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
    template<typename T, typename std::enable_if<is_rgb_type<T>::value, int>::type = 0>
    inline byte get_blue (T p) {
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
    template<typename T, typename std::enable_if<is_alpha_type<T>::value, int>::type = 0>
    inline byte get_alpha (T p) {
      return p.alpha;
    }

    inline byte get_alpha (mono p) {
      return IF_WIN32_ELSE(0, 255);
    }

    inline byte get_alpha (gray p) {
      return IF_WIN32_ELSE(0, 255);
    }

    inline byte get_alpha (os::color c) {
      return gui::color::get_alpha(c);
    }

    inline byte get_alpha (core::bit_wrapper<const mono>) {
      return IF_WIN32_ELSE(0, 255);
    }

    // --------------------------------------------------------------------------
    inline bool operator== (const gray& l, const gray& r) {
      return l.value == r.value;
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr gray gray::build(T t) {
      gray p{};
      p = t;
      return p;
    }

    template<typename T>
    inline gray& gray::operator= (T rhs) {
      value = get_gray(rhs);
      return *this;
    }

    constexpr gray::operator os::color () const {
      return gui::color::calc_rgb_gray(value);
    }

    // --------------------------------------------------------------------------
    template<typename T, typename S>
    typename std::enable_if<is_rgb_type<S>::value, void>::type put_rgb (T& lhs, const S rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<!is_rgb_type<S>::value, void>::type put_rgb (T& lhs, const S rhs) {
      lhs.red = lhs.green = lhs.blue = get_gray(rhs);
    }

    template<typename T>
    void put_rgb (T& lhs, const os::color rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<is_rgb_type<S>::value, void>::type put_rgba (T& lhs, const S rhs) {
      lhs.red = get_red(rhs);
      lhs.green = get_green(rhs);
      lhs.blue = get_blue(rhs);
      lhs.alpha = get_alpha(rhs);
    }

    template<typename T, typename S>
    typename std::enable_if<!is_rgb_type<S>::value, void>::type put_rgba (T& lhs, const S rhs) {
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
    constexpr rgb rgb::build(T t) {
      rgb p{};
      p = t;
      return p;
    }

    template<typename T>
    inline rgb& rgb::operator= (T rhs) {
      put_rgb(*this, rhs);
      return *this;
    }

    constexpr rgb::operator os::color () const {
      return gui::color::calc_rgb(red, green, blue);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr rgba rgba::build(T t) {
      rgba p{};
      p = t;
      return p;
    }

    template<typename T>
    inline rgba& rgba::operator= (T rhs) {
      put_rgba(*this, rhs);
      return *this;
    }

    constexpr rgba::operator os::color () const {
      return gui::color::calc_rgba(red, green, blue, alpha);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr bgr bgr::build(T t) {
      bgr p{};
      p = t;
      return p;
    }

    template<typename T>
    inline bgr& bgr::operator= (T rhs) {
      put_rgb(*this, rhs);
      return *this;
    }

    constexpr bgr::operator os::color () const {
      return gui::color::calc_rgb(red, green, blue);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr bgra bgra::build(T t) {
      bgra p{};
      p = t;
      return p;
    }

    template<typename T>
    inline bgra& bgra::operator= (T rhs) {
      put_rgba(*this, rhs);
      return *this;
    }

    constexpr bgra::operator os::color () const {
      return gui::color::calc_rgba(red, green, blue, alpha);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr argb argb::build(T t) {
      argb p{};
      p = t;
      return p;
    }

    template<typename T>
    inline argb& argb::operator= (T rhs) {
      put_rgb(*this, rhs);
      return *this;
    }

    constexpr argb::operator os::color () const {
      return gui::color::calc_rgba(red, green, blue, alpha);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    constexpr abgr abgr::build(T t) {
      abgr p{};
      p = t;
      return p;
    }

    template<typename T>
    inline abgr& abgr::operator= (T rhs) {
      put_rgba(*this, rhs);
      return *this;
    }

    constexpr abgr::operator os::color () const {
      return gui::color::calc_rgba(red, green, blue, alpha);
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
        pixel::rgb p{};
        p = pixel::get_color(s);
        return p;
      }
    };

  } // namespace pixel

} // namespace gui
