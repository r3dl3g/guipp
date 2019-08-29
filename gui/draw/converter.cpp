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

      param constants::calc_y (uint32_t y) const {
        return {y, scale_y, src_h};
      }

      param constants::calc_x (uint32_t x) const {
        return {x, scale_x, src_w};
      }

//      template<>
//      pixel::gray_pixel interpolation(const pixel::gray_pixel p00, const pixel::gray_pixel p01,
//                                      const pixel::gray_pixel p10, const pixel::gray_pixel p11,
//                                      const bilinear::weights& wx, const bilinear::weights& wy) {

//        const auto p00f = static_cast<double>(p00.value) * wx.w0;
//        const auto p01f = static_cast<double>(p01.value) * wx.w1;
//        const auto p10f = static_cast<double>(p10.value) * wx.w0;
//        const auto p11f = static_cast<double>(p11.value) * wx.w1;

//        const auto p0 = p00f + p01f;
//        const auto p1 = p10f + p11f;

//        const auto r = p0 * wy.w0 + p1 * wy.w1;

////        const auto r2 = (p00 * wx.w0 + p01 * wx.w1) * wy.w0 + (p10 * wx.w0 + p11 * wx.w1) * wy.w1;
////        if (static_cast<byte>(r) != r2.value) {
////          LogDebug << "gray_pixel interpolation calculation differs! (" << r << " != " << r2 << ")";
////        }

//        return {static_cast<byte>(r)};
//      }

    } // namespace bilinear

  } // namespace convert

//  namespace draw {

//    typedef int index_t;

//    template<typename T>
//    inline T limit (T min, T x, T max) {
//      return x < min ? min : (x > max ? max : x);
//    }

//    template<typename T>
//    inline T round(double x) {
//      return (T)std::round(x);
//    }

//    template<typename T>
//    inline T floor(double x) {
//      return (T)std::floor(x);
//    }

//    template<typename T>
//    inline T round(float x) {
//      return (T)std::roundf(x);
//    }

//    template<typename T>
//    inline T floor(float x) {
//      return (T)std::floorf(x);
//    }

//    template<typename T, size_t S>
//    class exponents : public std::array<T, S> {
//    public:
//      exponents(T v_) {
//        T v = v_;
//        (*this)[0] = v;
//        for (index_t i = 1; i < S; ++i) {
//          v = v * v_;
//          (*this)[i] = v;
//        }
//      }
//    };

//    template<typename T>
//    class exponents<T, 1> : public std::array<T, 1> {
//    public:
//      exponents(T v)
//        :std::array<T, 1>({ v }) {}
//    };

//    template<typename T>
//    class exponents<T, 2> : public std::array<T, 2> {
//    public:
//      exponents(T v)
//        :std::array<T, 2>({ v, v * v })
//      {}
//    };

//    template<typename T>
//    class exponents<T, 3> : public std::array<T, 3> {
//    public:
//      exponents(T v)
//        :std::array<T, 3>({ v, v * v, v * v * v })
//      {}
//    };

//    template<typename T, size_t S>
//    class weights : public std::array<T, S> {
//    public:
//      weights(const exponents<T, S - 1>&);
////      {
////        throw std::runtime_error(ostreamfmt("weights not implemented for size " << +S));
////      }
//    };

//    template<typename T>
//    class weights<T, 2> : public std::array<T, 2> {
//    public:
//      weights(const exponents<T, 1>& vv)
//        :std::array<T, 2>({ T(1) - vv[0], vv[0] })
//      {}
//    };

//    template<typename T>
//    class weights<T, 4> : public std::array<T, 4> {
//    public:
//      weights(const exponents<T, 3>& vv)
//        :std::array<T, 4>({
//          (-vv[0] + T(2) * vv[1] - vv[2]) / T(2),
//          (T(2) - T(5) * vv[1] + T(3) * vv[2]) / T(2),
//          (vv[0] + T(4) * vv[1] - T(3) * vv[2]) / T(2),
//          (-vv[1] + vv[2]) / T(2)
//        })
//      {}
//    };

//    template <typename T, size_t S>
//    class interpolate {
//    public:
//      using array_wrapper = basepp::array_wrapper;

//      interpolate(const_image_data<T>& in,
//                  image_data<T>& out,
//                  const index_t width,
//                  const index_t height);
//      //      {
////        throw std::runtime_error(ostreamfmt("interpolate not implemented for size " << +S));
////      }
//    };

//    template <typename T>
//    class interpolate<T, 1> {
//    public:
//      interpolate(const_image_data<T>& in_data,
//                  image_data<T>& out_data) {

//        for (index_t y = 0; y < height; ++y) {
//          const auto in = in_data.row(y);
//          auto out = out_data.row(y);
//          for (index_t x = 0; x < width; ++y) {
//            out[x] = in[x];
//          }
//        }
//      }
//    };

//    template <typename T>
//    class interpolate<T, 2> {
//    public:
//      interpolate(const_image_data<T>& in_data,
//                  image_data<T>& out_data,
//                  const index_t width,
//                  const index_t height) {

//        for (index_t y = 0; y < height; ++y) {
//          const auto in0 = in_data.row(y);
//          const auto in1 = in_data.row(y + 1);
//          auto out = out_data.row(y);
//          for (index_t x = 0; x < width; ++y) {

//          const indices<2> f(x, y, width, height);

//          const weights<float, 2> wx(exponents<float, 1>(x - (L)x_floor));
//          const weights<float, 2> wy(exponents<float, 1>(y - (L)y_floor));

//          const L d0[] = {
//            wx[0] * (L)in[f[0][0]],
//            wx[1] * (L)in[f[0][1]]
//          };

//          const L d1[] = {
//            wx[0] * (L)in[f[1][0]],
//            wx[1] * (L)in[f[1][1]]
//          };

//          const L s[] = {
//            d0[0] + d0[1],
//            d1[0] + d1[1]
//          };

//          const L m[] = {
//            wy[0] * s[0],
//            wy[1] * s[1]
//          };

//          const L v = m[0] + m[1];

//          if (v < std::numeric_limits<T>::min()) {
//            out[i] = std::numeric_limits<T>::min();
//          } else if (v > std::numeric_limits<T>::max()) {
//            out[i] = std::numeric_limits<T>::max();
//          } else {
//            out[i] = it3d_round<T>(v);
//          }

//          //indices_linear f((index_t)x_floor, (index_t)y_floor, width, height);
//          //out[i] = it3d_round<T>(wy0 * (wx0 * (L)in[f.m_00] + wx1 * (L)in[f.m_10]) +
//          //                       wy1 * (wx0 * (L)in[f.m_01] + wx1 * (L)in[f.m_11]));
//          }
//        }
//      }
//    };

//    template <typename T, typename L>
//    class interpolate<T, L, 4> {
//    public:
//      using array_wrapper = basepp::array_wrapper;

//      interpolate(array_wrapper<T>& out,
//        const array_wrapper<T>& in,
//        const array_wrapper<L>& lookupX,
//        const array_wrapper<L>& lookupY,
//        const index_t width,
//        const index_t height) {

//        const index_t size = (index_t)in.size();
//        for (index_t i = 0; i < size; ++i) {
//          const L x = lookupX[i] - L(1); // Lookup is calculated for Matlab with start index 1
//          const L y = lookupY[i] - L(1); // Lookup is calculated for Matlab with start index 1

//          const index_t x_floor = it3d_floor<index_t>(x);
//          const index_t y_floor = it3d_floor<index_t>(y);

//          const indices<4> f(x_floor, y_floor, width, height);

//          const weights<L, 4> wx(exponents<L, 3>(x - (L)x_floor));
//          const weights<L, 4> wy(exponents<L, 3>(y - (L)y_floor));

//          const L d0[] = {
//            wx[0] * (L)in[f[0][0]],
//            wx[1] * (L)in[f[0][1]],
//            wx[2] * (L)in[f[0][2]],
//            wx[3] * (L)in[f[0][3]]
//          };

//          const L d1[] = {
//            wx[0] * (L)in[f[1][0]],
//            wx[1] * (L)in[f[1][1]],
//            wx[2] * (L)in[f[1][2]],
//            wx[3] * (L)in[f[1][3]]
//          };

//          const L d2[] = {
//            wx[0] * (L)in[f[2][0]],
//            wx[1] * (L)in[f[2][1]],
//            wx[2] * (L)in[f[2][2]],
//            wx[3] * (L)in[f[2][3]]
//          };

//          const L d3[] = {
//            wx[0] * (L)in[f[3][0]],
//            wx[1] * (L)in[f[3][1]],
//            wx[2] * (L)in[f[3][2]],
//            wx[3] * (L)in[f[3][3]]
//          };

//          const L s[] = {
//            d0[0] + d0[1] + d0[2] + d0[3],
//            d1[0] + d1[1] + d1[2] + d1[3],
//            d2[0] + d2[1] + d2[2] + d2[3],
//            d3[0] + d3[1] + d3[2] + d3[3]
//          };

//          const L m[] = {
//            wy[0] * s[0],
//            wy[1] * s[1],
//            wy[2] * s[2],
//            wy[3] * s[3]
//          };

//          const L v = m[0] + m[1] + m[2] + m[3];

//          /* Slower: */
//          //L v = 0;
//          //for (index_t dy = 0; dy < 4; ++dy) {
//          //  L vx = 0;
//          //  for (index_t dx = 0; dx < 4; ++dx) {
//          //    vx += wx[dx] * (L)in[f[dx][dy]];
//          //  }
//          //  v += vx * wy[dy];
//          //}

//          if (v < std::numeric_limits<T>::min()) {
//            out[i] = std::numeric_limits<T>::min();
//          } else if (v > std::numeric_limits<T>::max()) {
//            out[i] = std::numeric_limits<T>::max();
//          } else {
//            out[i] = it3d_round<T>(v);
//          }
//        }
//      }
//    };


//  } // namespace draw

} // namespace gui
