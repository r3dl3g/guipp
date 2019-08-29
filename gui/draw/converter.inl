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

      static void row (const typename draw::const_image_data<F>::row_type src0,
                       const typename draw::const_image_data<F>::row_type src1,
                       typename draw::image_data<F>::row_type dst,
                       const bilinear::constants& c,
                       const bilinear::param& py) {

        using type = typename draw::const_image_data<F>::pixel_type;

        for (uint_fast32_t x = 0; x < c.dest_w; ++x) {

          const bilinear::param px = c.calc_x(x);

          const type p00 = src0[c.src_x0 + px.v0];
          const type p01 = src0[c.src_x0 + px.v1];
          const type p10 = src1[c.src_x0 + px.v0];
          const type p11 = src1[c.src_x0 + px.v1];

          const auto r = bilinear::interpolation(p00, p01, p10, p11, px.w, py.w);

          dst[c.dest_x0 + x] = r;
        }
      }

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {

        const bilinear::constants c(src, dest);

        for (uint_fast32_t y = 0; y < c.dest_h; ++y) {

          const bilinear::param py = c.calc_y(y);

          const auto src0 = src_data.row(c.src_y0 + py.v0);
          const auto src1 = src_data.row(c.src_y0 + py.v1);
          const auto dst = dest_data.row(c.dest_y0 + y);

          row(src0, src1, dst, c, py);

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
