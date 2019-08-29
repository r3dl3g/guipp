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
 * @brief     C++ API: bitmap converter
 *
 * @file
 */

#pragma once


namespace gui {

  namespace convert {

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    namespace format {

      template<PixelFormat From>
      struct line<From, PixelFormat::BW> {
        static inline void convert (const typename draw::const_image_data<From>::row_type in,
                                    typename draw::image_data<PixelFormat::BW>::row_type out,
                                    uint32_t w) {
          for (uint_fast32_t x = 0; x < w; ++x) {
            out[x] = get_bw(in[x]);
          }
        }
      };

      template<PixelFormat From, PixelFormat To>
      void line<From, To>::convert (const typename draw::const_image_data<From>::row_type in,
                                    typename draw::image_data<To>::row_type out,
                                    uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          out[x] = in[x];
        }
      }

      template<PixelFormat From, PixelFormat To>
      void convert (const typename draw::const_image_data<From> in,
                    draw::image_data<To> out,
                    uint32_t w, uint32_t h) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          line<From, To>::convert(in.row(y), out.row(y), w);
        }
      }


    } // namespace format

    namespace copy {

      template<PixelFormat px_fmt>
      void row (const typename draw::const_image_data<px_fmt>::row_type src,
                typename draw::image_data<px_fmt>::row_type dst,
                uint32_t src_x0, uint32_t dest_x0, uint32_t w) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          dst[dest_x0 + x] = src[src_x0 + x];
        }
      }

      template<PixelFormat px_fmt>
      void sub (const typename draw::const_image_data<px_fmt> src_data,
                draw::image_data<px_fmt> dest_data,
                const core::native_point& src,
                const core::native_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          row<px_fmt>(src_data.row(src.y() + y),
                   dest_data.row(dest.y() + y),
                   src.x(), dest.x(), dest.width());
        }
      }

    } // namespace copy

    // --------------------------------------------------------------------------
    template<PixelFormat F>
    struct stretch<F, interpolation::nearest> {

      static void row (const typename draw::const_image_data<F>::row_type src,
                       typename draw::image_data<F>::row_type dst,
                       uint32_t src_x0, uint32_t dest_x0,
                       uint32_t src_w, uint32_t dest_w) {
        const double scale_x = static_cast<double>(src_w) / static_cast<double>(dest_w);
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const double fx = (x + 0.5) * scale_x;
          const uint32_t src_x = std::min(src_w - 1, static_cast<uint32_t>(fx));
          dst[dest_x0 + x] = src[src_x0 + src_x];
        }
      }

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {
        const uint32_t src_h = src.height();
        const uint32_t dest_h = dest.height();
        const double scale_y = static_cast<double>(src_h) / static_cast<double>(dest_h);
        for (uint_fast32_t y = 0; y < dest_h; ++y) {
          const double fy = (y + 0.5) * scale_y;
          const uint32_t src_y = std::min(src_h - 1, static_cast<uint32_t>(fy));
          row(src_data.row(src.y() + src_y),
              dest_data.row(dest.y() + y),
              src.x(), dest.x(), src.width(), dest.width());
        }
      }

    }; // struct stretch

    // --------------------------------------------------------------------------
    namespace bilinear {

      struct weights {
        const double w0;
        const double w1;

        weights (double v, uint32_t v0);

      };

      struct param {
        const uint32_t v0;
        const uint32_t v1;
        const weights w;

        param (uint32_t v, double scale, uint32_t max);

      private:
        static param create (uint32_t v, double scale, uint32_t max);

        param (uint32_t v0, uint32_t v1, weights&& w);

      };

      struct constants {
        const uint32_t src_w;
        const uint32_t src_h;
        const uint32_t dest_w;
        const uint32_t dest_h;
        const uint32_t src_x0;
        const uint32_t src_y0;
        const uint32_t dest_x0;
        const uint32_t dest_y0;
        const double scale_y;
        const double scale_x;

        constants (const core::native_rect& src, const core::native_rect& dest);

        param calc_y (uint32_t y) const;

        param calc_x (uint32_t x) const;

      };

      // --------------------------------------------------------------------------
      template<typename T>
      T interpolation(const T p00, const T p01, const T p10, const T p11,
                      const bilinear::weights& wx, const bilinear::weights& wy) {
        return {(p00 * wx.w0 + p01 * wx.w1) * wy.w0 + (p10 * wx.w0 + p11 * wx.w1) * wy.w1};
      }

    } // namespace bilinear

    // --------------------------------------------------------------------------
    template<PixelFormat F>
    struct stretch<F, interpolation::bilinear> {

      using type = typename draw::const_image_data<F>::pixel_type;

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {

        const bilinear::constants c(src, dest);

        for (uint_fast32_t y = 0; y < c.dest_h; ++y) {

          const bilinear::param py = c.calc_y(y);

          const auto src0 = src_data.row(c.src_y0 + py.v0);
          const auto src1 = src_data.row(c.src_y0 + py.v1);

          auto dst = dest_data.row(c.dest_y0 + y);

          for (uint_fast32_t x = 0; x < c.dest_w; ++x) {
            const bilinear::param px = c.calc_x(x);
            const auto r = bilinear::interpolation<type>(src0[c.src_x0 + px.v0],
                                                         src0[c.src_x0 + px.v1],
                                                         src1[c.src_x0 + px.v0],
                                                         src1[c.src_x0 + px.v1],
                                                         px.w, py.w);
            dst[c.dest_x0 + x] = r;
          }

        }
      }

    }; // struct stretch

    // --------------------------------------------------------------------------
    namespace bicubic {

      struct weights {
        const double w0;
        const double w1;
        const double w2;
        const double w3;

        weights (double v);

      private:
        static weights create (double v);

        weights (double, double, double, double);

      };

      struct param {
        const uint32_t v0;
        const uint32_t v1;
        const uint32_t v2;
        const uint32_t v3;
        const weights w;

        param (uint32_t v, double scale, uint32_t max);

      private:
        static param create (uint32_t v, double scale, uint32_t max);

        param (uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, weights&& w);

      };

      struct constants {
        const uint32_t src_w;
        const uint32_t src_h;
        const uint32_t dest_w;
        const uint32_t dest_h;
        const uint32_t src_x0;
        const uint32_t src_y0;
        const uint32_t dest_x0;
        const uint32_t dest_y0;
        const double scale_y;
        const double scale_x;

        constants (const core::native_rect& src, const core::native_rect& dest);

        param calc_y (uint32_t y) const;

        param calc_x (uint32_t x) const;

      };

      // --------------------------------------------------------------------------
      template<typename T>
      const T summation (const basepp::array_wrapper<const T> src,
                         const bicubic::param px,
                         const double f) {
        return src[px.v0] * px.w.w0 * f +
            src[px.v1] * px.w.w1 * f +
            src[px.v2] * px.w.w2 * f +
            src[px.v3] * px.w.w3 * f;
      }
      // --------------------------------------------------------------------------
      template<typename T>
      const T interpolation (const basepp::array_wrapper<const T> src0,
                             const basepp::array_wrapper<const T> src1,
                             const basepp::array_wrapper<const T> src2,
                             const basepp::array_wrapper<const T> src3,
                             const bicubic::param px,
                             const bicubic::param py) {
        return {summation(src0, px, py.w.w0)
              + summation(src1, px, py.w.w1)
              + summation(src2, px, py.w.w2)
              + summation(src3, px, py.w.w3)};
      }

      // --------------------------------------------------------------------------
      template<>
      const pixel::gray_pixel interpolation<const pixel::gray_pixel> (const basepp::array_wrapper<const pixel::gray_pixel> src0,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src1,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src2,
                                                                      const basepp::array_wrapper<const pixel::gray_pixel> src3,
                                                                      const bicubic::param px,
                                                                      const bicubic::param py);

      // --------------------------------------------------------------------------
      template<>
      const pixel::rgb_pixel interpolation<const pixel::rgb_pixel> (const basepp::array_wrapper<const pixel::rgb_pixel> src0,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src1,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src2,
                                                                    const basepp::array_wrapper<const pixel::rgb_pixel> src3,
                                                                    const bicubic::param px,
                                                                    const bicubic::param py);

      // --------------------------------------------------------------------------
      template<>
      const pixel::rgba_pixel interpolation<const pixel::rgba_pixel> (const basepp::array_wrapper<const pixel::rgba_pixel> src0,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src1,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src2,
                                                                      const basepp::array_wrapper<const pixel::rgba_pixel> src3,
                                                                      const bicubic::param px,
                                                                      const bicubic::param py);

    } // namespace bicubic

    // --------------------------------------------------------------------------
    template<PixelFormat F>
    struct stretch<F, interpolation::bicubic> {

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {

        using type = typename draw::const_image_data<F>::pixel_type;
        const bicubic::constants c(src, dest);

        for (uint_fast32_t y = 0; y < c.dest_h; ++y) {

          const bicubic::param py = c.calc_y(y);

          const auto src0 = src_data.row(c.src_y0 + py.v0).sub(c.src_x0, c.src_w);
          const auto src1 = src_data.row(c.src_y0 + py.v1).sub(c.src_x0, c.src_w);
          const auto src2 = src_data.row(c.src_y0 + py.v2).sub(c.src_x0, c.src_w);
          const auto src3 = src_data.row(c.src_y0 + py.v3).sub(c.src_x0, c.src_w);

          auto dst = dest_data.row(c.dest_y0 + y);

          for (uint_fast32_t x = 0; x < c.dest_w; ++x) {
            const bicubic::param px = c.calc_x(x);
            const auto r = bicubic::interpolation<const type>(src0, src1, src2, src3, px, py);
            dst[c.dest_x0 + x] = r;
          }

        }
      }

    }; // struct stretch

    // --------------------------------------------------------------------------
    template<PixelFormat F, interpolation I>
    inline void stretch<F, I>::sub (const typename draw::const_image_data<F> src,
                                    draw::image_data<F> dest) {
      sub(src, dest, {0, 0, src.width(), src.hright()}, {0, 0, dest.width(), dest.hright()});
    }

    namespace brightness {

      template<PixelFormat px_fmt>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, double f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = data[x] * f;
        }
      }

      template<PixelFormat px_fmt>
      void adjust (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, double f) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<px_fmt>(data.row(y), w, f);
        }
      }

    }

    namespace fill {

      template<PixelFormat px_fmt, typename pixel_type>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, const pixel_type& px) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = px;
        }
      }

      template<PixelFormat px_fmt, typename pixel_type>
      void fill (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, const pixel_type& px) {
        for (uint_fast32_t y = 0; y < h; ++y) {
          row<px_fmt>(data.row(y), w, px);
        }
      }

    }

  } // namespace convert

} // namespace gui
