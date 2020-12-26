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
        auto v0 = static_cast<uint32_t>(s);
        uint32_t v1 = std::min(v0 + 1, max - 1);
        return {v0, v1, {s, v0}};
      }

      param::param (uint32_t v0, uint32_t v1, weights&& w)
        : v0(v0)
        , v1(v1)
        , w(std::move(w))
      {}

      template<>
      pixel::mono interpolation (const pixel::mono p00,
                                 const pixel::mono p01,
                                 const pixel::mono p10,
                                 const pixel::mono p11,
                                 const weights& wx,
                                 const weights& wy) {
        return double2mono((mono2double(p00) * wx.w0 + mono2double(p01) * wx.w1) * wy.w0 +
                           (mono2double(p10) * wx.w0 + mono2double(p11) * wx.w1) * wy.w1);
      }

    } // namespace bilinear


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
        auto v1 = static_cast<uint32_t>(s);
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
      double summation (const core::array_wrapper<const pixel::mono>& src,
                        const bicubic::param& px) {
        return mono2double(src[px.v0]) * px.w.w0 + mono2double(src[px.v1]) * px.w.w1 +
               mono2double(src[px.v2]) * px.w.w2 + mono2double(src[px.v3]) * px.w.w3;
      }

      // --------------------------------------------------------------------------
      double summation (const core::array_wrapper<const pixel::gray>& src,
                        const bicubic::param& px) {
        return static_cast<double>(src[px.v0].value) * px.w.w0 +
               static_cast<double>(src[px.v1].value) * px.w.w1 +
               static_cast<double>(src[px.v2].value) * px.w.w2 +
               static_cast<double>(src[px.v3].value) * px.w.w3;
      }

      // --------------------------------------------------------------------------
    } // namespace bicubic

  } // namespace convert

} // namespace gui
