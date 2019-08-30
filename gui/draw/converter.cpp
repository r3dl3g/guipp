/**
 * @copyright (c) 2016-2019 Ing. Buero Rothfuss
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

 // --------------------------------------------------------------------------
 //
 // Common includes
 //
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/converter.h>

namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    namespace bilinear {

      // --------------------------------------------------------------------------
      weights::weights (double v, uint32_t v0)
        : w0(1.0 - (v - v0))
        , w1(v - v0)
      {}

      // --------------------------------------------------------------------------
      param::param (uint32_t v, double scale, uint32_t max)
        : param(create(v, scale, max))
      {}

      param param::create (uint32_t v, double scale, uint32_t max) {
        const double s = static_cast<double>(v) * scale;
        uint32_t v0 = static_cast<uint32_t>(s);
        uint32_t v1 = std::min(v0 + 1, max - 1);
        return {v0, v1, {s, v0}};
      }

      param::param (uint32_t v0, uint32_t v1, weights&& w)
        : v0(v0)
        , v1(v1)
        , w(std::move(w))
      {}

    } // namespace bilinear


    namespace scaling {

      // --------------------------------------------------------------------------
      constants::constants (const core::native_rect& src, const core::native_rect& dest)
        : src_w(src.width())
        , src_h(src.height())
        , dest_w(dest.width())
        , dest_h(dest.height())
        , src_x0(src.x())
        , src_y0(src.y())
        , dest_x0(dest.x())
        , dest_y0(dest.y())
        , scale_y(static_cast<double>(src_h - 1) / static_cast<double>(dest_h - 1))
        , scale_x(static_cast<double>(src_w - 1) / static_cast<double>(dest_w - 1))
      {}

    } // namespace scaling

    // --------------------------------------------------------------------------
    namespace bicubic {

      weights::weights (double w0, double w1, double w2, double w3)
        : w0(w0)
        , w1(w1)
        , w2(w2)
        , w3(w3)
      {}

      weights weights::create (double v0) {
        const double v1 = v0 * v0;
        const double v2 = v1 * v0;

        const double w0 = ((-v0 + 2.0 * v1 - v2) / 2.0);
        const double w1 = ((2.0 - 5.0 * v1 + 3.0 * v2) / 2.0);
        const double w2 = ((v0 + 4.0 * v1 - 3.0 * v2) / 2.0);
        const double w3 = ((-v1 + v2) / 2.0);

        return {w0, w1, w2, w3};
      }

      weights::weights (double v)
        : weights(create(v))
      {}

      param::param (uint32_t v, double scale, uint32_t max)
        : param(create(v, scale, max))
      {}

      param param::create (uint32_t v, double scale, uint32_t max) {
        const double s = static_cast<double>(v) * scale;
        uint32_t v1 = static_cast<uint32_t>(s);
        uint32_t v0 = v1 > 0 ? v1 - 1 : 0;
        uint32_t v2 = std::min(v1 + 1, max - 1);
        uint32_t v3 = std::min(v2 + 1, max - 1);
        return {v0, v1, v2, v3, {s - v1}};
      }

      param::param (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, weights&& w)
        : v0(v0)
        , v1(v1)
        , v2(v2)
        , v3(v3)
        , w(std::move(w))
      {}

      // --------------------------------------------------------------------------
      template<typename T = float>
      T summation (const basepp::array_wrapper<const pixel::gray_pixel> src,
                        const bicubic::param px) {
        return static_cast<T>(src[px.v0].value) * px.w.w0 +
               static_cast<T>(src[px.v1].value) * px.w.w1 +
               static_cast<T>(src[px.v2].value) * px.w.w2 +
               static_cast<T>(src[px.v3].value) * px.w.w3;
      }

      template<>
      const pixel::gray_pixel interpolation<const pixel::gray_pixel> (const basepp::array_wrapper<const pixel::gray_pixel> src0,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src1,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src2,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src3,
                                                                      const bicubic::param px,
                                                                      const bicubic::param py) {
        const auto sum = summation(src0, px) * py.w.w0 +
                         summation(src1, px) * py.w.w1 +
                         summation(src2, px) * py.w.w2 +
                         summation(src3, px) * py.w.w3;
        return {static_cast<byte>(std::min(sum, decltype(sum)(255)))};
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
      template<typename T>
      rgb_t<float> summation (const basepp::array_wrapper<const T> src,
                       const bicubic::param px) {
        rgb_t<float> r = rgb_t<float>(src[px.v0]) * px.w.w0 +
                       rgb_t<float>(src[px.v1]) * px.w.w1 +
                       rgb_t<float>(src[px.v2]) * px.w.w2 +
                       rgb_t<float>(src[px.v3]) * px.w.w3;
        return r;
      }

      // --------------------------------------------------------------------------
      template<>
      const pixel::rgb_pixel interpolation<const pixel::rgb_pixel> (const basepp::array_wrapper<const pixel::rgb_pixel> src0,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src1,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src2,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src3,
                                                                    const bicubic::param px,
                                                                    const bicubic::param py) {
        const auto sum = summation(src0, px) * py.w.w0 +
                         summation(src1, px) * py.w.w1 +
                         summation(src2, px) * py.w.w2 +
                         summation(src3, px) * py.w.w3;
        return sum;
      }

      // --------------------------------------------------------------------------
      template<>
      const pixel::rgba_pixel interpolation<const pixel::rgba_pixel> (const basepp::array_wrapper<const pixel::rgba_pixel> src0,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src1,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src2,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src3,
                                                                      const bicubic::param px,
                                                                      const bicubic::param py) {
        const auto sum = summation(src0, px) * py.w.w0 +
                         summation(src1, px) * py.w.w1 +
                         summation(src2, px) * py.w.w2 +
                         summation(src3, px) * py.w.w3;
        return sum;
      }
      // --------------------------------------------------------------------------
    } // namespace bicubic

  } // namespace convert

} // namespace gui
