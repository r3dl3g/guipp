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


namespace gui {

  namespace draw {

    namespace diagram {

      // --------------------------------------------------------------------------
      template<typename T, scaling_type S>
      struct scale_fn {
        static T inc (T, T);
        static T step (T);
        static T sub (T, T);
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_fn<T, scaling_type::linear> {
        static T inc (T i, T step) {
          return i + step;
        }
        static T step (T t) {
          return t;
        }
        static T sub (T, T t) {
          return t;
        }
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_fn<T, scaling_type::log> {
        static T inc (T i, T step) {
          return i * step;
        }
        static T step (T t) {
          return std::pow(10, t);
        }
        static T sub (T i, T t) {
          return i * t;
        }
      };

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V>
      struct scale_dim {
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_dim<T, orientation_t::horizontal> {
        static constexpr T main_tick_length = 3;
        static constexpr T sub_tick_length = 1;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_dim<T, orientation_t::vertical> {
        static constexpr T main_tick_length = -3;
        static constexpr T sub_tick_length = -1;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      scaler_base<T>::scaler_base (T mi, T ma, T tmi, T tma)
        : min(mi)
        , max(ma)
        , target_min(tmi)
        , target_max(tma)
      {}

      template<typename T>
      T scaler_base<T>::get_min () const {
        return min;
      }

      template<typename T>
      T scaler_base<T>::get_max () const {
        return max;
      }

      template<typename T>
      T scaler_base<T>::get_target_min () const {
        return target_min;
      }

      template<typename T>
      T scaler_base<T>::get_target_max () const {
        return target_max;
      }

      template<typename T>
      T scaler_base<T>::get_target_range () const {
        return target_max - target_min;
      }

      // --------------------------------------------------------------------------
      template<typename T>
      struct scaler<T, scaling_type::log> : scaler_base<T> {
        typedef scaler_base<T> super;
        typedef T value_type;
        static const scaling_type scaling_type = scaling_type::log;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);

      private:
        void precalc ();

        T precalced;
        T precalced_min;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scaler<T, scaling_type::linear> : scaler_base<T> {
        typedef scaler_base<T> super;
        typedef T value_type;
        static const scaling_type scaling_type = scaling_type::linear;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);

      private:
        void precalc ();

        T precalced;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      scaler<T, scaling_type::log>::scaler (T mi, T ma, T tmi, T tma)
        : super(mi, ma, tmi, tma)
        , precalced(1)
        , precalced_min(0)
      {
        if (super::min == 0) {
          throw std::runtime_error("Base 0 is not allowed for logarithmic scaler");
        }
        precalc();
      }

      template<typename T>
      T scaler<T, scaling_type::log>::operator() (T v) const {
        return (std::log(v) - precalced_min) * precalced + super::target_min;
      }

      template<typename T>
      void scaler<T, scaling_type::log>::set_min_max (T mi, T ma) {
        super::min = mi;
        super::max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling_type::log>::set_target_min_max (T mi, T ma) {
        super::target_min = mi;
        super::target_max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling_type::log>::precalc () {
        precalced_min = std::log(super::min);
        precalced = (super::target_max - super::target_min) / (std::log(super::max) - precalced_min);
      }

      // --------------------------------------------------------------------------
      template<typename T>
      scaler<T, scaling_type::linear>::scaler (T mi, T ma, T tmi, T tma)
        : super(mi, ma, tmi, tma)
        , precalced(1)
      {
        precalc();
      }

      template<typename T>
      T scaler<T, scaling_type::linear>::operator() (T v) const {
        return (v - super::min) * precalced + super::target_min;
      }

      template<typename T>
      void scaler<T, scaling_type::linear>::set_min_max (T mi, T ma) {
        super::min = mi;
        super::max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling_type::linear>::set_target_min_max (T mi, T ma) {
        super::target_min = mi;
        super::target_max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling_type::linear>::precalc () {
        precalced = (super::target_max - super::target_min) / (super::max - super::min);
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<typename T, orientation_t V, scaling_type S>
        void draw_axis (const graphics& g,
                        const core::point& pos,
                        os::color color,
                        const scaler<T, S>& sc) {
          typedef core::orientation_traits<V> traits;

          const T d2 = traits::get_2(pos);
          core::point p0, p1;
          traits::set(p0, sc.get_target_min(), d2);
          traits::set(p1, sc.get_target_max(), d2);
          g.frame(line(p0, p1), color);
        }

        template<typename T, orientation_t V, scaling_type S>
        void draw_sub_ticks (const graphics& g,
                             os::color color,
                             const scaler<T, S>& sc,
                             T min, T step, T max,
                             T sub_1, T sub_2) {
          typedef core::orientation_traits<V> traits;

          if ((step > 0) && (sub_2 != sub_1) && (min + step < max)) {
            core::point p0, p1;
            traits::set_2(p0, sub_1);
            traits::set_2(p1, sub_2);
            for (T j = min + step; j < max; j += step) {
              const T ds = sc(j);

              traits::set_1(p0, ds);
              traits::set_1(p1, ds);
              g.frame(line(p0, p1), color);
            }
          }
        }

      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling_type S>
      scale<T, V, S>::scale (const core::point& pos,
                             const scaler_type& sc,
                             T main,
                             T sub,
                             T main_ticks_length,
                             T sub_ticks_length,
                             formatter fmt)
        : pos(pos)
        , sc(sc)
        , main(main)
        , sub(sub)
        , main_ticks_length(main_ticks_length)
        , sub_ticks_length(sub_ticks_length)
        , fmt(fmt)
      {}

      template<typename T, orientation_t V, scaling_type S>
      void scale<T, V, S>::operator() (const graphics& g, const font& font, os::color color) const {
        paint::draw_axis<T, V, S>(g, pos, color, sc);

        core::point p0, p1, p2;
        const T d2 = traits::get_2(pos);

        traits::set_2(p0, d2 + scale_dim<T, V>::main_tick_length);
        traits::set_2(p1, d2 + main_ticks_length);
        traits::set_2(p2, d2 + scale_dim<T, V>::main_tick_length * 2);

        const T main_step = scale_fn<T, S>::step(main);

        const T max = sc.get_max();
        for (T i = sc.get_min(); i <= max; i = scale_fn<T, S>::inc(i, main_step)) {
          const T d1 = sc(i);

          traits::set_1(p0, d1);
          traits::set_1(p1, d1);
          g.frame(line(p0, p1), color);

          traits::set_1(p2, d1);
          g.text(draw::text(fmt(i), p2, scale_text_origin<V>()), font, color::black);

          const T sub_step = scale_fn<T, S>::sub(i, sub);
          paint::draw_sub_ticks<T, V, S>(g, color, sc,
                                         i, sub_step, std::min(scale_fn<T, S>::inc(i, main_step), max),
                                         d2 + scale_dim<T, V>::sub_tick_length, d2 + sub_ticks_length);

        }
      }

      // --------------------------------------------------------------------------
      template<typename T>
      wall<T>::wall (const core::point& pos,
                             const scaler_base<T>& sx,
                             const scaler_base<T>& sy)
        : pos(pos)
        , sx(sx)
        , sy(sy)
      {}

      template<typename T>
      void wall<T>::operator() (const graphics& g, const brush& b, const pen& p) const {
        g.fill(draw::rectangle(pos, core::point(sx.get_target_max(), sy.get_target_max())), b);
      }

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      graph_base<T, SX, SY>::graph_base (const core::point& pos,
                                         const scaler<T, SX>& sx,
                                         const scaler<T, SY>& sy,
                                         point2d_data points)
        : pos(pos)
        , sx(sx)
        , sy(sy)
        , points(points)
      {}

      template<typename T, scaling_type SX, scaling_type SY>
      core::rectangle graph_base<T, SX, SY>::get_graph_area () const {
        return core::rectangle(pos, core::point(sx.get_target_max(), sy.get_target_max()));
      }

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      line_graph<T, SX, SY>::line_graph (const core::point& pos,
                                         const scaler<T, SX>& sx,
                                         const scaler<T, SY>& sy,
                                         typename super::point2d_data points)
        : super(pos, sx, sy, points)
      {}

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      void line_graph<T, SX, SY>::operator() (const graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;

        const auto sz = super::points().size();
        pts.reserve(sz);
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points().at(i);
          pts.push_back({static_cast<core::point::type>(super::sx(pt[0])),
                         static_cast<core::point::type>(super::sy(pt[1]))});
        }
        g.frame(draw::polyline(pts), p);
      }

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      bar_graph<T, SX, SY>::bar_graph (const core::point& pos,
                                       const scaler<T, SX>& sx,
                                       const scaler<T, SY>& sy,
                                       typename super::point2d_data points)
        : super(pos, sx, sy, points)
      {}

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      void bar_graph<T, SX, SY>::operator() (const graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        const auto sz = super::points().size();
        const float w = std::max(1.0F, std::floor(static_cast<float>((super::sx.get_target_max() - super::sx.get_target_min()) / (sz * 2) - 1)));
        const auto y0 = static_cast<core::point::type>(super::sy(0));
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points().at(i);
          const auto x = static_cast<core::point::type>(super::sx(pt[0]));
          const auto y = static_cast<core::point::type>(super::sy(pt[1]));
          g.fill(draw::rectangle(core::point(x - w, y0), core::point(x + w, y)), b);
        }
      }

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      points_graph<T, SX, SY>::points_graph (const core::point& pos,
                                             const scaler<T, SX>& sx,
                                             const scaler<T, SY>& sy,
                                             typename super::point2d_data points,
                                             T radius)
        : super(pos, sx, sy, points)
        , radius(radius)
      {}

      // --------------------------------------------------------------------------
      template<typename T, scaling_type SX, scaling_type SY>
      void points_graph<T, SX, SY>::operator() (const graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        const auto sz = super::points().size();
        const float w = std::min(static_cast<core::point::type>(radius),
                                 std::max(1.0F,
                                          std::floor(static_cast<float>((super::sx.get_target_max() - super::sx.get_target_min()) / (sz * 2) - 1))));
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points().at(i);
          const auto x = static_cast<core::point::type>(super::sx(pt[0]));
          const auto y = static_cast<core::point::type>(super::sy(pt[1]));
          g.fill(draw::arc(core::point(x, y), w, 0, 360), b);
        }
      }

      // --------------------------------------------------------------------------

    } // namespace diagram

  } // namespace draw

} // namespace gui
