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
      template<typename T, scaling S>
      struct scale_fn {
        static T inc (T, T);
        static T step (T);
        static T sub (T, T);
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_fn<T, scaling::linear> {
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
      struct scale_fn<T, scaling::log> {
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
      struct scaler<T, scaling::log> : scaler_base<T> {
        typedef scaler_base<T> super;
        typedef T value_type;
        static const scaling scaling_type = scaling::log;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);

      private:
        void precalc ();

        double precalced;
        double precalced_min;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scaler<T, scaling::linear> : scaler_base<T> {
        typedef scaler_base<T> super;
        typedef T value_type;
        static const scaling scaling_type = scaling::linear;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);

      private:
        void precalc ();

        double precalced;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      scaler<T, scaling::log>::scaler (T mi, T ma, T tmi, T tma)
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
      T scaler<T, scaling::log>::operator() (T v) const {
        return static_cast<T>((std::log(v) - precalced_min) * precalced + super::target_min);
      }

      template<typename T>
      void scaler<T, scaling::log>::set_min_max (T mi, T ma) {
        super::min = mi;
        super::max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling::log>::set_target_min_max (T mi, T ma) {
        super::target_min = mi;
        super::target_max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling::log>::precalc () {
        precalced_min = std::log(super::min);
        precalced = (super::target_max - super::target_min) / (std::log(super::max) - precalced_min);
      }

      // --------------------------------------------------------------------------
      template<typename T>
      scaler<T, scaling::linear>::scaler (T mi, T ma, T tmi, T tma)
        : super(mi, ma, tmi, tma)
        , precalced(1)
      {
        precalc();
      }

      template<typename T>
      T scaler<T, scaling::linear>::operator() (T v) const {
        return static_cast<T>((v - super::min) * precalced + super::target_min);
      }

      template<typename T>
      void scaler<T, scaling::linear>::set_min_max (T mi, T ma) {
        super::min = mi;
        super::max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling::linear>::set_target_min_max (T mi, T ma) {
        super::target_min = mi;
        super::target_max = ma;
        precalc();
      }

      template<typename T>
      void scaler<T, scaling::linear>::precalc () {
        precalced = static_cast<double>(super::target_max - super::target_min)
                    / static_cast<double>(super::max - super::min);
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<typename T, orientation_t V, scaling S>
        void draw_axis (const graphics& g,
                        const core::point& pos,
                        const pen& p,
                        const scaler<T, S>& sc) {
          typedef core::orientation_traits<V> traits;

          const T d2 = traits::get_2(pos);
          core::point p0, p1;
          traits::set(p0, sc.get_target_min(), d2);
          traits::set(p1, sc.get_target_max(), d2);
          g.frame(line(p0, p1), p);
        }

        template<typename T, orientation_t V, scaling S>
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
      template<typename T, orientation_t V, scaling S>
      scale<T, V, S>::scale (const core::point& pos,
                             const scaler_type& sc,
                             T main,
                             T sub,
                             T main_ticks_length,
                             T sub_ticks_length,
                             os::color main_color,
                             os::color sub_color,
                             formatter fmt)
        : pos(pos)
        , sc(sc)
        , main(main)
        , sub(sub)
        , main_ticks_length(main_ticks_length)
        , sub_ticks_length(sub_ticks_length)
        , main_color(main_color)
        , sub_color(sub_color)
        , fmt(fmt)
      {}

      template<typename T, orientation_t V, scaling S>
      void scale<T, V, S>::operator() (const graphics& g, const font& font, os::color color) const {

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
          g.frame(line(p0, p1), main_color);

          traits::set_1(p2, d1);
          g.text(draw::text(fmt(i), p2, scale_text_origin<V>()), font, color);

          const T sub_step = scale_fn<T, S>::sub(i, sub);
          paint::draw_sub_ticks<T, V, S>(g, sub_color, sc,
                                         i, sub_step, std::min(scale_fn<T, S>::inc(i, main_step), max),
                                         d2 + scale_dim<T, V>::sub_tick_length, d2 + sub_ticks_length);

        }
//        paint::draw_axis<T, V, S>(g, pos, color, sc);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U>
      wall<T, U>::wall (const core::point& pos,
                        const scaler_base<T>& sx,
                        const scaler_base<U>& sy)
        : pos(pos)
        , sx(sx)
        , sy(sy)
      {}

      template<typename T, typename U>
      void wall<T, U>::operator() (const graphics& g, const brush& b, const pen& p) const {
        g.fill(draw::rectangle(pos, core::point(sx.get_target_max(), sy.get_target_max())), b);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, scaling SX, scaling SY>
      axis<T, U, SX, SY>::axis (const core::point& pos,
                                const scaler<T, SX>& sx,
                                const scaler<U, SY>& sy)
        : pos(pos)
        , sx(sx)
        , sy(sy)
      {}

      template<typename T, typename U, scaling SX, scaling SY>
      void axis<T, U, SX, SY>::operator() (const graphics& g, const pen& p) const {
        paint::draw_axis<T, orientation_t::horizontal, SX>(g, pos, p, sx);
        paint::draw_axis<U, orientation_t::vertical, SY>(g, pos, p, sy);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      graph_base<T, U, C, SX, SY>::graph_base (const core::point& pos,
                                               const scaler<T, SX>& sx,
                                               const scaler<U, SY>& sy,
                                               point2d_data points)
        : pos(pos)
        , sx(sx)
        , sy(sy)
        , points(points)
      {}

      template<typename T, typename U, typename C, scaling SX, scaling SY>
      core::rectangle graph_base<T, U, C, SX, SY>::get_graph_area () const {
        return core::rectangle(pos, core::point(sx.get_target_max(), sy.get_target_max()));
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      line_graph<T, U, C, SX, SY>::line_graph (const core::point& pos,
                                               const scaler<T, SX>& sx,
                                               const scaler<U, SY>& sy,
                                               typename super::point2d_data points,
                                               U zero)
        : super(pos, sx, sy, points)
        , zero(zero)
      {}

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void line_graph<T, U, C, SX, SY>::operator() (const graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void line_graph<T, U, C, SX, SY>::operator() (const graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());

        std::vector<core::point> pts;

        const auto sz = super::points.size();
        pts.reserve(sz+2);

        const core::point::type y0 = static_cast<core::point::type>(super::sy(zero));
        const core::point::type x0 = static_cast<core::point::type>(super::sx(get_x<T>(super::points[0])));
        const core::point::type x1 = static_cast<core::point::type>(super::sx(get_x<T>(super::points[sz - 1])));

        pts.push_back({ x0, y0 });
        calc_points(pts);
        pts.push_back({ x1, y0 });

        g.fill(draw::polygon(pts), b);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void line_graph<T, U, C, SX, SY>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz);
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          pts.push_back({static_cast<core::point::type>(super::sx(get_x<T>(pt))),
                         static_cast<core::point::type>(super::sy(get_y<U>(pt)))});
        }
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      cascade<T, U, C, SX, SY>::cascade (const core::point& pos,
                                         const scaler<T, SX>& sx,
                                         const scaler<U, SY>& sy,
                                         typename super::point2d_data points,
                                         U zero)
        : super(pos, sx, sy, points)
        , zero(zero)
      {}

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void cascade<T, U, C, SX, SY>::operator() (const graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void cascade<T, U, C, SX, SY>::operator() (const graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.fill(draw::polygon(pts), b);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void cascade<T, U, C, SX, SY>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz*2 + 1);
        const core::point::type y0 = static_cast<core::point::type>(super::sy(zero));
        core::point::type last_y = y0;
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          core::point::type next_x = static_cast<core::point::type>(super::sx(get_x<T>(pt)));
          core::point::type next_y = static_cast<core::point::type>(super::sy(get_y<U>(pt)));
          pts.push_back({next_x, last_y});
          pts.push_back({next_x, next_y});
          last_y = next_y;
        }
        const core::point::type x1 = static_cast<core::point::type>(super::sx(get_x<T>(super::points[sz - 1])));
        pts.push_back({ x1, y0 });
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      bar_graph<T, U, C, SX, SY>::bar_graph (const core::point& pos,
                                             const scaler<T, SX>& sx,
                                             const scaler<U, SY>& sy,
                                             typename super::point2d_data points,
                                             T space)
        : super(pos, sx, sy, points)
        , space(space)
      {}

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void bar_graph<T, U, C, SX, SY>::operator() (const graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        const auto sz = super::points.size();
        const float w = std::max(1.0F, static_cast<float>((super::sx.get_target_max() - super::sx.get_target_min()) / (sz * 2) - space));
        const auto y0 = static_cast<core::point::type>(super::sy(0));
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          const auto x = static_cast<core::point::type>(super::sx(get_x<T>(pt)));
          const auto y = static_cast<core::point::type>(super::sy(get_y<U>(pt)));
          g.fill(draw::rectangle(core::point(x - w, y0), core::point(x + w, y)), b);
        }
      }

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      points_graph<T, U, C, SX, SY>::points_graph (const core::point& pos,
                                                const scaler<T, SX>& sx,
                                                const scaler<U, SY>& sy,
                                                typename super::point2d_data points,
                                                point_drawer drawer)
        : super(pos, sx, sy, points)
        , drawer(drawer)
      {}

      // --------------------------------------------------------------------------
      template<typename T, typename U, typename C, scaling SX, scaling SY>
      void points_graph<T, U, C, SX, SY>::operator() (const graphics& g, const brush& b) const {
        if (drawer) {
          clip clp(g, super::get_graph_area());
          const auto sz = super::points.size();
          for (int i = 0; i < sz; ++i) {
            const auto pt = super::points[i];
            const auto x = static_cast<core::point::type>(super::sx(get_x<T>(pt)));
            const auto y = static_cast<core::point::type>(super::sy(get_y<U>(pt)));
            drawer(g, b, core::point(x, y));
          }
        }
      }

      // --------------------------------------------------------------------------

    } // namespace diagram

  } // namespace draw

} // namespace gui
