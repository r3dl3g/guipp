/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

      namespace detail {

        // --------------------------------------------------------------------------
        template<typename T>
        struct scale_fn<T, scaling::linear> {
          static T inc (T i, T step, T) { return i + step; }
          static T step (T t)           { return t; }
          static T sub (T, T t, T)      { return t; }
          static T min (const core::range<T>&r)      { return r.begin(); }

          static T calc (T t, T)        { return t; }
          static T precalc (const core::range<T>& r) { return r.begin(); }
          static T range (const core::range<T>& r)   { return r.size(); }
        };
        // --------------------------------------------------------------------------
        template<typename T>
        struct scale_fn<T, scaling::log> {
          static T inc (T i, T step, T) { return i * step; }
          static T step (T t)           { return std::pow(10, t); }
          static T sub (T i, T t, T)    { return i * t; }
          static T min (const core::range<T>&r) { return r.begin(); }

          static T calc (T t, T)        { return std::log10(t); }
          static T precalc (const core::range<T>& r) { return calc(r.begin(), 0); }
          static T range (const core::range<T>& r)   { return std::log10(r.end()) - std::log10(r.begin()); }
        };
        // --------------------------------------------------------------------------
        template<typename T>
        struct scale_fn<T, scaling::logn> {
          static T inc (T i, T step, T) { return i * step; }
          static T step (T t)           { return std::pow(M_E, t); }
          static T sub (T i, T t, T)    { return i * t; }
          static T min (const core::range<T>&r) { return r.begin(); }

          static T calc (T t, T)        { return std::log(t); }
          static T precalc (const core::range<T>& r) { return calc(r.begin(), 0); }
          static T range (const core::range<T>& r)   { return std::log(r.end()) - std::log(r.begin()); }
        };
        // --------------------------------------------------------------------------
        template<typename T>
        struct scale_fn<T, scaling::log2> {
          static T inc (T i, T step, T) { return i * step; }
          static T step (T t)           { return std::pow(2, t); }
          static T sub (T i, T t, T)    { return i * t; }
          static T min (const core::range<T>&r) { return r.begin(); }

          static T calc (T t, T)        { return std::log2(t); }
          static T precalc (const core::range<T>& r) { return calc(r.begin(), 0); }
          static T range (const core::range<T>& r)   { return std::log2(r.end()) - std::log2(r.begin()); }
        };
        // --------------------------------------------------------------------------
        template<typename T>
        struct scale_fn<T, scaling::symlog> {
          static T inc (T i, T step, T min) {
            const T abs_min = std::abs(min);
            return (i < -abs_min ? i / step : i >= abs_min ? i * step : i + min);
            // armhf build return always 0 for abs(min). std::abs() solves the problem
//            clog::info() << "scale_fn<scaling::symlog>::inc(i:" << i << ", step:" << step << ", min:" << min << "), abs(min):" << abs_min;
//            if (i < -abs_min) {
//              return i / step;
//            } else if (i >= abs_min) {
//              return i * step;
//            } else {
//              return i + min;
//            }
          }
          static T step (T t)           { return std::pow(10.0, std::abs(t)); }
          static T sub (T i, T t, T min) {
            return i < 0 ? std::abs(i / (10 * t)) : (i > 0 ? std::abs(i * t) : min / 10);
          }
          static T min (const core::range<T>&r) {
            return -r.end();
          }

          static T calc (T t, T min) {
            if (std::abs(t) < std::abs(min)) {
              return t / std::abs(min);
            }
            return std::copysign(std::log10(std::abs(t)) - std::log10(std::abs(min)) + 1, t);
          }
          static T precalc (const core::range<T>& r) {
            return -((std::log10(r.end()) - (std::log10(r.begin()))) + 1);
          }
          static T range (const core::range<T>& r)   {
            return (std::log10(r.end()) - (std::log10(r.begin()))) * 2 + 2;
          }
        };

      } // namespace detail

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      inline range_pair<X, Y> make_range_pair (X x0, X x1, Y y0, Y y1) {
        return std::make_pair(core::range<X>(x0, x1), core::range<Y>(y0, y1));
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      inline range_pair<X, Y> get_min_max (const range_pair<X, Y>& lhs, const range_pair<X, Y>& rhs) {
        return std::make_pair(core::min_max(lhs.first, rhs.first), core::min_max(lhs.second, rhs.second));
      }

      // --------------------------------------------------------------------------
      template<typename T>
      inline bool lower (T lhs, T rhs) {
        return (lhs < rhs);
      }
      // --------------------------------------------------------------------------
      template<typename T>
      inline bool lower_ignore_0 (T lhs, T rhs) {
        return (lhs != T(0)) && (lhs < rhs);
      }
      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename L>
      range_pair<X, Y> find_min_max (const L& v) {
        auto i = std::begin(v);
        auto last = std::end(v);

        auto xmin = i;
        auto xmax = i;
        auto ymin = i;
        auto ymax = i;
        while (++i < last) {
          if (lower(get_x<X, Y>(*i), get_x<X>(*xmin))) {
            xmin = i;
          }
          if (lower(get_x<X, Y>(*xmax), get_x<X>(*i))) {
            xmax = i;
          }
          if (lower(get_y<X, Y>(*i), get_y<Y>(*ymin))) {
            ymin = i;
          }
          if (lower(get_y<X, Y>(*ymax), get_y<Y>(*i))) {
            ymax = i;
          }
        }

        return make_range_pair(get_x<X, Y>(*xmin), get_x<X, Y>(*xmax), get_y<X, Y>(*ymin), get_y<X, Y>(*ymax));
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename L>
      range_pair<X, Y> find_min_max_ignore_0 (const L& v) {
        auto i = std::begin(v);
        auto last = std::end(v);

        auto xmin = last;
        auto ymin = last;
        while (++i < last) {
          const auto x = get_x<X, Y>(*i);
          if (x != X(0)) {
            if ((xmin == last) || lower(x, get_x<X, Y>(*xmin))) {
              xmin = i;
            }
          }
          const auto y = get_y<X, Y>(*i);
          if (y != Y(0)) {
            if ((ymin == last) || lower(y, get_y<X, Y>(*ymin))) {
              ymin = i;
            }
          }
        }
        i = std::begin(v);
        auto xmax = i;
        auto ymax = i;
        while (++i < last) {
          if (lower(get_x<X, Y>(*xmax), get_x<X, Y>(*i))) {
            xmax = i;
          }
          if (lower(get_y<X, Y>(*ymax), get_y<X, Y>(*i))) {
            ymax = i;
          }
        }
        if (xmin == last) {
          xmin = xmax;
        }
        if (ymin == last) {
          ymin = ymax;
        }

        return make_range_pair(get_x<X, Y>(*xmin), get_x<X, Y>(*xmax), get_y<X, Y>(*ymin), get_y<X, Y>(*ymax));
      }

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_dim<T, orientation_t::horizontal, origin_t::start> {
        static constexpr T main_tick_length = 3;
        static constexpr T sub_tick_length = 1;
      };

      template<typename T>
      struct scale_dim<T, orientation_t::horizontal, origin_t::center> {
        static constexpr T main_tick_length = 3;
        static constexpr T sub_tick_length = 1;
      };

      template<typename T>
      struct scale_dim<T, orientation_t::horizontal, origin_t::end> {
        static constexpr T main_tick_length = -3;
        static constexpr T sub_tick_length = -1;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct scale_dim<T, orientation_t::vertical, origin_t::start> {
        static constexpr T main_tick_length = -3;
        static constexpr T sub_tick_length = -1;
      };

      template<typename T>
      struct scale_dim<T, orientation_t::vertical, origin_t::center> {
        static constexpr T main_tick_length = 1;
        static constexpr T sub_tick_length = 1;
      };

      template<typename T>
      struct scale_dim<T, orientation_t::vertical, origin_t::end> {
        static constexpr T main_tick_length = 1;
        static constexpr T sub_tick_length = 1;
      };

      // --------------------------------------------------------------------------
      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::horizontal, origin_t::start> () {
        return text_origin_t::top_hcenter;
      }

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::horizontal, origin_t::center> () {
        return text_origin_t::top_hcenter;
      }

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::horizontal, origin_t::end> () {
        return text_origin_t::bottom_hcenter;
      }

      // --------------------------------------------------------------------------
      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::vertical, origin_t::start> () {
        return text_origin_t::vcenter_right;
      }

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::vertical, origin_t::center> () {
        return text_origin_t::vcenter_left;
      }

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::vertical, origin_t::end> () {
        return text_origin_t::vcenter_left;
      }

      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_pow10 (T t) {
        return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(10.0, std::floor(std::log10(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_bigger_pow10 (T t) {
        return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(10.0, std::ceil(std::log10(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_pow2 (T t) {
        return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(2.0, std::floor(std::log2(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_bigger_pow2 (T t) {
        return (t == 0) ? 0 : static_cast<T>(std::copysign(std::pow(2.0, std::ceil(std::log2(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_bigger_dezimal (T t) {
        const T u = next_smaller_pow10<T>(t);
        const T v = std::ceil(t / u);
        return u * v;
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_dezimal (T t) {
        const T u = next_smaller_pow10<T>(t);
        const T v = std::floor(t / u);
        return u * v;
      }

      // --------------------------------------------------------------------------
      template<typename T>
      struct limits<T, scaling::linear> {
        static core::range<T> calc (T min, T max) {
          T lmin = 0;
          T lmax = 0;
          if ((min == T(0)) && (max == T(0))) {
          } else if (min == T(0)) {
            lmax = next_bigger_dezimal(max);
          } else if (max == T(0)) {
            lmin = next_bigger_dezimal(min);
          } else {
            const auto ma = std::floor(std::log10(std::abs(max)));
            const auto mi = std::floor(std::log10(std::abs(min)));
            const auto m = std::max(mi, ma);
            const auto l = std::copysign(std::pow(10.0, ma < m - 1 ? m - 1 : ma), max);
            const auto lmi = std::copysign(std::pow(10.0, mi < m - 1 ? m - 1 : mi), min);
            lmax = static_cast<T>(l * std::ceil(max / l));
            lmin = static_cast<T>(lmi * (min < 0 ? std::ceil(min / lmi) : std::floor(min / lmi)));
          }
          return { lmin, lmax };
        }
      };
      // --------------------------------------------------------------------------
      template<typename T>
      struct limits<T, scaling::log> {
        static core::range<T> calc (T min, T max) {
          auto lmin = next_smaller_pow10(min);
          auto lmax = next_bigger_pow10(max);
          return { lmin, lmax };
        }
      };
      // --------------------------------------------------------------------------
      template<typename T>
      struct limits<T, scaling::log2> {
        static core::range<T> calc (T min, T max) {
          auto lmin = next_smaller_pow2(min);
          auto lmax = next_bigger_pow2(max);
          return { lmin, lmax };
        }
      };
      // --------------------------------------------------------------------------
      template<typename T>
      struct limits<T, scaling::logn> {
        static core::range<T> calc (T min, T max) {
          auto lmin = next_smaller_pow10(min);
          auto lmax = next_bigger_pow10(max);
          return { lmin, lmax };
        }
      };
      // --------------------------------------------------------------------------
      template<typename T>
      struct limits<T, scaling::symlog> {
        static core::range<T> calc (T min, T max) {
          auto ma = std::max(std::abs(min), std::abs(max));
          auto lmax = next_bigger_pow10(ma);
          return { lmax / 100000, lmax };
        }
      };
      // --------------------------------------------------------------------------
      template<typename T, scaling S>
      scaler<T, S>::scaler (core::range<T> src, core::range<T> target)
        : src(src)
        , target(target)
        , precalced(1)
        , precalced_min(static_cast<double>(src.begin()))
      {
        if ((S != scaling::linear) && (src.begin() == 0)) {
          throw std::runtime_error("Base 0 is not allowed for logarithmic scaler");
        }
        precalc();
      }

      template<typename T, scaling S>
      T scaler<T, S>::operator() (T v) const {
        if (S != scaling::symlog) {
          if (v < src.begin()) {
            return target.begin();
          } else if (v > src.end()) {
            return target.end();
          }
        }
        const auto scaled = detail::scale_fn<T, S>::calc(v, src.begin());
        return static_cast<T>((scaled - precalced_min) * precalced) + target.begin();
      }

      template<typename T, scaling S>
      const core::range<T>& scaler<T, S>::get_source () const {
        return src;
      }

      template<typename T, scaling S>
      const core::range<T>& scaler<T, S>::get_target () const {
        return target;
      }

      template<typename T, scaling S>
      void scaler<T, S>::set_source_range (core::range<T> s) {
        src = s;
        precalc();
      }

      template<typename T, scaling S>
      void scaler<T, S>::set_target_range (core::range<T> t) {
        target = t;
        precalc();
      }

      template<typename T, scaling S>
      void scaler<T, S>::precalc () {
        precalced_min = static_cast<double>(detail::scale_fn<T, S>::precalc(src));
        precalced = static_cast<double>(target.size()) /
                    static_cast<double>(detail::scale_fn<T, S>::range(src));
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<typename T, orientation_t V, scaling S>
        void draw_axis (graphics& g,
                        const core::point& pos,
                        const pen& p,
                        const scaler<T, S>& sc) {
          typedef core::orientation_traits<V> traits;

          const auto d2 = traits::get_2(pos);
          core::point p0, p1;
          traits::set(p0, static_cast<float>(sc.get_target().begin()), d2);
          traits::set(p1, static_cast<float>(sc.get_target().end()), d2);
          g.frame(line(p0, p1), p);
        }

        template<typename T, orientation_t V, scaling S>
        void draw_sub_ticks (graphics& g,
                             os::color color,
                             const scaler<T, S>& sc,
                             T min, T step, T max,
                             T sub_1, T sub_2) {
          typedef core::orientation_traits<V> traits;

          if ((step > 0) && (sub_2 != sub_1) && (min + step < max)) {
            core::point p0, p1;
            traits::set_2(p0, static_cast<float>(sub_1));
            traits::set_2(p1, static_cast<float>(sub_2));
            for (T j = min + step; j < max; j += step) {
              const auto ds = sc(j);

              traits::set_1(p0, static_cast<float>(ds));
              traits::set_1(p1, static_cast<float>(ds));
              g.frame(line(p0, p1), color);
            }
          }
        }

      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling S, origin_t O>
      scale<T, V, S, O>::scale (const core::point& pos,
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

      template<typename T, orientation_t V, scaling S, origin_t O>
      void scale<T, V, S, O>::operator() (graphics& g, const font& font, os::color color) const {

        core::point p0, p1, p2;
        const T d2 = static_cast<T>(traits::get_2(pos));

        traits::set_2(p0, static_cast<float>(d2 + tick_dimension::main_tick_length));
        traits::set_2(p1, static_cast<float>(d2 + main_ticks_length));
        traits::set_2(p2, static_cast<float>(d2 + tick_dimension::main_tick_length * 2));

        const auto main_step = detail::scale_fn<T, S>::step(main);

        const auto min = sc.get_source().begin();
        const auto max = sc.get_source().end();
        T i = detail::scale_fn<T, S>::min(sc.get_source());
        while (i <= max) {
          const auto d1 = sc(i);
          const auto next = detail::scale_fn<T, S>::inc(i, main_step, min);

          traits::set_1(p0, static_cast<float>(d1));
          traits::set_1(p1, static_cast<float>(d1));
          g.frame(line(p0, p1), main_color);

          traits::set_1(p2, static_cast<float>(d1));
          const auto text_origin = scale_text_origin<V, O>();
          g.text(draw::text(fmt(i), p2, text_origin), font, color);

          const T sub_step = detail::scale_fn<T, S>::sub(i, sub, min);
          paint::draw_sub_ticks<T, V, S>(g, sub_color, sc,
                                         i, sub_step, std::min(next, max),
                                         d2 + tick_dimension::sub_tick_length, d2 + sub_ticks_length);
          i = next;
        }
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY>
      wall<X, Y, SX, SY>::wall (const scaler<X, SX>& sx,
                                const scaler<Y, SY>& sy)
        : sx(sx)
        , sy(sy)
      {}

      template<typename X, typename Y, scaling SX, scaling SY>
      void wall<X, Y, SX, SY>::operator() (graphics& g, const brush& b, const pen& p) const {
        g.fill(draw::rectangle(core::point(static_cast<float>(sx.get_target().begin()), static_cast<float>(sy.get_target().begin())),
                               core::point(static_cast<float>(sx.get_target().end()), static_cast<float>(sy.get_target().end()))), b);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY>
      headline<X, Y, SX, SY>::headline (const scaler<X, SX>& sx,
                                        const scaler<Y, SY>& sy,
                                        const std::string& text)
        : sx(sx)
        , sy(sy)
        , text(text)
      {}

      template<typename X, typename Y, scaling SX, scaling SY>
      void headline<X, Y, SX, SY>::operator() (graphics& g, const font& f, os::color color) const {
        core::point p(static_cast<float>(sx.get_target().begin()), static_cast<float>(sy.get_target().end()));
        g.text(draw::text(text, p, text_origin_t::bottom_left), f, color);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY>
      legend<X, Y, SX, SY>::legend (const scaler<X, SX>& sx,
                                    const scaler<Y, SY>& sy,
                                    const std::vector<legend_label>& labels)
        : sx(sx)
        , sy(sy)
        , labels(labels)
      {}

      template<typename X, typename Y, scaling SX, scaling SY>
      void legend<X, Y, SX, SY>::operator() (graphics& g, const font& f, os::color color) const {
        core::point p(static_cast<float>(sx.get_target().begin()),
                      static_cast<float>(sy.get_target().begin() + scale_dim<X, orientation_t::horizontal>::main_tick_length * 2));
        bool first = true;
        for(const auto& l : labels) {
          core::rectangle area(p, core::size{5, 5});
          g.text(draw::bounding_box(l.second, area, text_origin_t::top_left), f, l.first);
          if (first) {
            p.move_y(area.height() + 2);
            first = false;
          }
          g.draw(draw::rectangle(p, core::size(area.height() - 3)), l.first, color);
          p.move_x(area.height());
          g.text(draw::text(l.second, p, text_origin_t::top_left), f, color);
          p.move_x(area.width() + 5);
        }
      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling S>
      axis<T, V, S>::axis (const core::point& pos,
                             const scaler<T, S>& sc)
        : pos(pos)
        , sc(sc)
      {}

      template<typename T, orientation_t V, scaling S>
      void axis<T, V, S>::operator() (graphics& g, const pen& p) const {
        paint::draw_axis<T, V, S>(g, pos, p, sc);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY>
      xy_axis<X, Y, SX, SY>::xy_axis (const scaler<X, SX>& sx,
                                      const scaler<Y, SY>& sy)
        : sx(sx)
        , sy(sy)
      {}

      template<typename X, typename Y, scaling SX, scaling SY>
      void xy_axis<X, Y, SX, SY>::operator() (graphics& g, const pen& p) const {
        core::point pos(static_cast<float>(sx.get_target().begin()),
                        static_cast<float>(sy.get_target().begin()));
        paint::draw_axis<X, orientation_t::horizontal, SX>(g, pos, p, sx);
        paint::draw_axis<Y, orientation_t::vertical, SY>(g, pos, p, sy);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      graph_base<X, Y, C, SX, SY>::graph_base (const scaler<X, SX>& sx,
                                               const scaler<Y, SY>& sy,
                                               point2d_data points)
        : sx(sx)
        , sy(sy)
        , points(points)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      core::rectangle graph_base<X, Y, C, SX, SY>::get_graph_area () const {
        return core::rectangle(core::point(static_cast<float>(sx.get_target().begin()),
                                           static_cast<float>(sy.get_target().begin())),
                               core::point(static_cast<float>(sx.get_target().end()),
                                           static_cast<float>(sy.get_target().end()))).grow({1, 1});
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      line_graph<X, Y, C, SX, SY>::line_graph (const scaler<X, SX>& sx,
                                               const scaler<Y, SY>& sy,
                                               typename super::point2d_data points,
                                               Y zero)
        : super(sx, sy, points)
        , zero(zero)
      {}

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void line_graph<X, Y, C, SX, SY>::operator() (graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void line_graph<X, Y, C, SX, SY>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());

        std::vector<core::point> pts;

        const auto sz = super::points.size();
        pts.reserve(sz+2);

        const core::point::type y0 = static_cast<core::point::type>(super::sy(zero));
        const core::point::type x0 = static_cast<core::point::type>(super::sx(get_x<X, Y>(super::points[0])));
        const core::point::type x1 = static_cast<core::point::type>(super::sx(get_x<X, Y>(super::points[sz - 1])));

        pts.push_back({ x0, y0 });
        calc_points(pts);
        pts.push_back({ x1, y0 });

        g.fill(draw::polygon(pts), b);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void line_graph<X, Y, C, SX, SY>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz);
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          pts.push_back({static_cast<core::point::type>(super::sx(get_x<X, Y>(pt))),
                         static_cast<core::point::type>(super::sy(get_y<X, Y>(pt)))});
        }
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      cascade<X, Y, C, SX, SY>::cascade (const scaler<X, SX>& sx,
                                         const scaler<Y, SY>& sy,
                                         typename super::point2d_data points,
                                         Y zero)
        : super(sx, sy, points)
        , zero(zero)
      {}

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void cascade<X, Y, C, SX, SY>::operator() (graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void cascade<X, Y, C, SX, SY>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.fill(draw::polygon(pts), b);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void cascade<X, Y, C, SX, SY>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz*2 + 1);
        const core::point::type y0 = static_cast<core::point::type>(super::sy(zero));
        core::point::type last_y = y0;
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          core::point::type next_x = static_cast<core::point::type>(super::sx(get_x<X, Y>(pt)));
          core::point::type next_y = static_cast<core::point::type>(super::sy(get_y<X, Y>(pt)));
          pts.push_back({next_x, last_y});
          pts.push_back({next_x, next_y});
          last_y = next_y;
        }
        const core::point::type x1 = static_cast<core::point::type>(super::sx(get_x<X, Y>(super::points[sz - 1])));
        pts.push_back({ x1, y0 });
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      bar_graph<X, Y, C, SX, SY>::bar_graph (const scaler<X, SX>& sx,
                                             const scaler<Y, SY>& sy,
                                             typename super::point2d_data points,
                                             X space)
        : super(sx, sy, points)
        , space(space)
      {}

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void bar_graph<X, Y, C, SX, SY>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        const auto sz = super::points.size();
        const float w = std::max(1.0F, static_cast<float>((super::sx.get_target().end() - super::sx.get_target().begin()) / (sz * 2) - space));
        const auto y0 = static_cast<core::point::type>(super::sy(0));
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          const auto x = static_cast<core::point::type>(super::sx(get_x<X, Y>(pt)));
          const auto y = static_cast<core::point::type>(super::sy(get_y<X, Y>(pt)));
          g.fill(draw::rectangle(core::point(x - w, y0), core::point(x + w, y)), b);
        }
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      points_graph<X, Y, C, SX, SY>::points_graph (const scaler<X, SX>& sx,
                                                   const scaler<Y, SY>& sy,
                                                   typename super::point2d_data points,
                                                   point_drawer drawer)
        : super(sx, sy, points)
        , drawer(drawer)
      {}

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY>
      void points_graph<X, Y, C, SX, SY>::operator() (graphics& g, const brush& b) const {
        if (drawer) {
          clip clp(g, super::get_graph_area());
          const auto sz = super::points.size();
          for (std::size_t i = 0; i < sz; ++i) {
            const auto pt = super::points[i];
            const auto x = static_cast<core::point::type>(super::sx(get_x<X, Y>(pt)));
            const auto y = static_cast<core::point::type>(super::sy(get_y<X, Y>(pt)));
            drawer(g, b, core::point(x, y));
          }
        }
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY>
      constexpr os::color chart<X, Y, SX, SY>::wall_back;

      template<typename X, typename Y, scaling SX, scaling SY>
      chart<X, Y, SX, SY>::chart (const core::rectangle& area, core::range<X> range_x, core::range<Y> range_y)
        : p0(static_cast<float>(area.x() + 50), static_cast<float>(area.y2() - 50))
        , scale_x(range_x, {static_cast<X>(p0.x()), static_cast<X>(area.x2() - 50)})
        , scale_y(range_y, {static_cast<Y>(p0.y()), static_cast<Y>(area.y() + 20)})
      {}

      template<typename X, typename Y, scaling SX, scaling SY>
      auto chart<X, Y, SX, SY>::get_scale_x() const -> const scaler_x_type& {
        return scale_x;
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      auto chart<X, Y, SX, SY>::get_scale_y() const -> const scaler_y_type& {
        return scale_y;
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      const core::point& chart<X, Y, SX, SY>::get_point_00 () const {
        return p0;
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::fill_area (graphics& graph) const {
        graph.draw(wall<X, Y, SX, SY>(scale_x, scale_y), wall_back, wall_back);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_xscale (graphics& graph, X main, X sub, typename scale_x_type::formatter fmt) const {
        graph.text(scale_x_type(p0, scale_x, main, sub,
                                static_cast<X>(scale_y.get_target().size()),
                                static_cast<X>(scale_y.get_target().size()),
                                color::very_light_gray,
                                color::very_very_light_gray,
                                fmt),
                   font::system_small(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_yscale (graphics& graph, Y main, Y sub, typename scale_y_type::formatter fmt) const {
        graph.text(scale_y_type(p0, scale_y, main, sub,
                                static_cast<Y>(scale_x.get_target().size()),
                                static_cast<Y>(scale_x.get_target().size()),
                                color::very_light_gray,
                                color::very_very_light_gray,
                                fmt),
                   font::system_small(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_axis (graphics& graph) const {
        graph.frame(xy_axis<X, Y, SX, SY>(scale_x, scale_y), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_title (graphics& graph, const std::string& title) const {
        graph.text(headline<X, Y, SX, SY>(scale_x, scale_y, title), font::system_bold(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_legend (graphics& graph, const std::vector<legend_label>& labels) const {
        graph.text(legend<X, Y, SX, SY>(scale_x, scale_y, labels), font::system(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      void chart<X, Y, SX, SY>::draw_background (graphics& graph, X xmain, X xsub, Y ymain, Y ysub) const {
        fill_area(graph);
        draw_xscale(graph, xmain, xsub);
        draw_yscale(graph, ymain, ysub);
        draw_axis(graph);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_line_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.frame(line_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_area_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.fill(line_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_cascade_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.frame(cascade<X, Y, C, SX, SY>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_cascade_area_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.fill(cascade<X, Y, C, SX, SY>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_bar_graph (graphics& graph, C data, os::color color, Y space) const {
        graph.fill(bar_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, space), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_point_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, circle(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_diamond_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, diamond(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_cross_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, cross(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY>
      template<typename C>
      void chart<X, Y, SX, SY>::draw_square_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY>(scale_x, scale_y, data, square(radius)), color);
      }

      // --------------------------------------------------------------------------

    } // namespace diagram

  } // namespace draw

} // namespace gui
