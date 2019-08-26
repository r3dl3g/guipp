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
        for (uint_fast32_t x = 0; x < dest_w; ++x) {
          const uint32_t src_x = (x * src_w / dest_w);
          dst[dest_x0 + x] = src[src_x0 + src_x];
        }
      }

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {
        for (uint_fast32_t y = 0; y < dest.height(); ++y) {
          const uint32_t src_y = y * src.height() / dest.height();
          row(src_data.row(src.y() + src_y),
              dest_data.row(dest.y() + y),
              src.x(), dest.x(), src.width(), dest.width());
        }
      }

    }; // struct stretch

    // --------------------------------------------------------------------------
    template<PixelFormat F, interpolation I>
    inline void stretch<F, I>::sub (const typename draw::const_image_data<F> src,
                                    draw::image_data<F> dest) {
      sub(src, dest, {0, 0, src.width(), src.hright()}, {0, 0, dest.width(), dest.hright()});
    }

    // --------------------------------------------------------------------------
    template<PixelFormat F>
    struct stretch<F, interpolation::bilinear> {

      static void row (const typename draw::const_image_data<F>::row_type src0,
                       const typename draw::const_image_data<F>::row_type src1,
                       typename draw::image_data<F>::row_type dst,
                       const uint32_t src_x0, const uint32_t src_w,
                       const uint32_t dest_x0, const uint32_t dest_w,
                       const float xscale,
                       const float weight_y_0, const float weight_y_1) {
        for (uint_fast32_t dest_x = 0; dest_x < dest_w; ++dest_x) {
          const float x = static_cast<float>(dest_x) * xscale;
          const uint32_t x0 = static_cast<uint32_t>(std::floor(x));
          const uint32_t x1 = std::min(static_cast<uint32_t>(std::ceil(x)), src_w - 1);
          const float weight_x_0 = fabs(x0 - x);
          const float weight_x_1 = fabs(x1 - x);
          const auto p00 = src0[x0 + src_x0];
          const auto p01 = src0[x1 + src_x0];
          const auto p10 = src1[x0 + src_x0];
          const auto p11 = src1[x1 + src_x0];

          const auto p0 = p00 * weight_x_0 + p01 * weight_x_1;
          const auto p1 = p10 * weight_x_0 + p11 * weight_x_1;

          const auto p = p0 * weight_y_0 + p1 * weight_y_1;

          dst[dest_x0 + dest_x] = p;
        }
      }

      static void sub (const typename draw::const_image_data<F> src_data,
                       draw::image_data<F> dest_data,
                       const core::native_rect& src,
                       const core::native_rect& dest) {
        const uint32_t dest_w = dest.width();
        const uint32_t dest_h = dest.height();
        const float scale_x = static_cast<float>(src.width()) / static_cast<float>(dest_w);
        const float scale_y = static_cast<float>(src.height()) / static_cast<float>(dest_h);
        const uint32_t src_x0 = src.x();
        const uint32_t src_w = src.width();
        const uint32_t dest_x0 = dest.x();
        for (uint_fast32_t dest_y = 0; dest_y < dest_h; ++dest_y) {
          const float y = static_cast<float>(dest_y) * scale_y;
          const uint32_t y0 = static_cast<uint32_t>(std::floor(y));
          const uint32_t y1 = std::min(static_cast<uint32_t>(std::ceil(y)), src.height() - 1);
          const float weight_y_0 = fabs(y0 - y);
          const float weight_y_1 = fabs(y1 - y);

          const auto src0 = src_data.row(src.y() + y0);
          const auto src1 = src_data.row(src.y() + y1);
          const auto dst = dest_data.row(dest_y);

          row (src0, src1, dst, src_x0, src_w, dest_x0, dest_w, scale_x, weight_y_0, weight_y_1);

        }
      }

    }; // struct stretch

    namespace brightness {

      template<PixelFormat px_fmt>
      void row (typename draw::image_data<px_fmt>::row_type data, uint32_t w, float f) {
        for (uint_fast32_t x = 0; x < w; ++x) {
          data[x] = data[x] * f;
        }
      }

      template<PixelFormat px_fmt>
      void adjust (draw::image_data<px_fmt> data, uint32_t w, uint32_t h, float f) {
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
