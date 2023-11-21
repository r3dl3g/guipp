/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     diagram drawing
 *
 * @license   MIT license. See accompanying file LICENSE.
 */


// --------------------------------------------------------------------------
namespace std {
  template<typename T, typename std::enable_if<std::is_unsigned<T>::value>::type* = nullptr>
  inline T abs (T t) {
    return t;
  }
}

// --------------------------------------------------------------------------
namespace gui {

  namespace draw {

    namespace diagram {

      namespace detail {

        // --------------------------------------------------------------------------
        template<typename S, typename T, typename I>
        struct scale_fn<S, T, scaling::linear, I> {
          static S inc (S i, I step, S)              { return i + step; }
          static I step (I t)                        { return t; }
          static I sub (S, I t, S)                   { return t; }
          static S min (const core::range<S>&r)      { return r.begin(); }

          static T calc (S s, S)                     { return convert<T>(s); }
          static T precalc (const core::range<S>& r) { return convert<T>(r.begin()); }
          static T range (const core::range<S>& r)   { return convert<T>(r.end() - r.begin()); }
        };
        // --------------------------------------------------------------------------
        template<typename S, typename T, typename I>
        struct scale_fn<S, T, scaling::log, I> {
          static S inc (S i, I step, S)         { return i * step; }
          static I step (I t) {
            return std::pow(10, static_cast<double>(t));
          }
          static I sub (S i, I t, S)            { return i * t; }
          static S min (const core::range<S>&r) { return r.begin(); }

          static T calc (S t, S) { 
            return static_cast<T>(std::log10(static_cast<double>(t)));
          }
          static T precalc (const core::range<S>& r) {
            return calc(r.begin(), {});
          }
          static T range (const core::range<S>& r) {
            return convert<T>(std::log10(static_cast<double>(r.end())) -
                              std::log10(static_cast<double>(r.begin())));
          }
        };
        // --------------------------------------------------------------------------
        template<typename S, typename T, typename I>
        struct scale_fn<S, T, scaling::logn, I> {
          static S inc (S i, I step, S) { return i * step; }
          static I step (I t) {
            return std::pow(M_E, static_cast<double>(t));
          }
          static I sub (S i, I t, S)    { return i * t; }
          static S min (const core::range<S>&r) { return r.begin(); }

          static T calc (S t, S) {
            return convert<T>(std::log(static_cast<double>(t)));
          }
          static T precalc (const core::range<S>& r) { return calc(r.begin(), {}); }
          static T range (const core::range<S>& r) {
            return convert<T>(std::log(static_cast<double>(r.end())) -
                              std::log(static_cast<double>(r.begin())));
          }
        };
        // --------------------------------------------------------------------------
        template<typename S, typename T, typename I>
        struct scale_fn<S, T, scaling::log2, I> {
          static S inc (S i, I step, S) { return i * step; }
          static I step (I t)           { return std::pow(2, static_cast<double>(t)); }
          static I sub (S i, I t, S)    { return i * t; }
          static S min (const core::range<S>&r) { return r.begin(); }

          static T calc (S t, S) {
            return convert<T>(std::log2(static_cast<double>(t)));
          }
          static T precalc (const core::range<S>& r) { return calc(r.begin(), {}); }
          static T range (const core::range<S>& r)   { 
            return std::log2(r.end()) - std::log2(r.begin());
          }
        };
        // --------------------------------------------------------------------------
        template<typename S, typename T, typename I>
        struct scale_fn<S, T, scaling::symlog, I> {
          static S inc (S i, I step, S min) {
            const S abs_min = std::abs(min);
            return (i < -abs_min ? i / step : i >= abs_min ? i * step : i + min);
          }

          static I step (I t)           { return std::pow(10.0, std::abs(t)); }
          static I sub (S i, I t, S min) {
            return i < 0 ? std::abs(i / (10 * t)) : (i > 0 ? std::abs(i * t) : min / 10);
          }
          static S min (const core::range<S>&r) {
            return -r.end();
          }

          static T calc (S t, S min) {
            if (std::abs(t) < std::abs(min)) {
              return convert<T>(t / std::abs(min));
            }
            return convert<T>(std::copysign(std::log10(std::abs(t)) -
                                                std::log10(std::abs(min)) + 1, t));
          }
          static T precalc (const core::range<S>& r) {
            return convert<T>(-((std::log10(r.end()) -
                                   (std::log10(r.begin()))) + 1));
          }
          static T range (const core::range<S>& r)   {
            return convert<T>((std::log10(r.end()) -
                              (std::log10(r.begin()))) * 2 + 2);
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
          if (lower(get_x<X>(*i), get_x<X>(*xmin))) {
            xmin = i;
          }
          if (lower(get_x<X>(*xmax), get_x<X>(*i))) {
            xmax = i;
          }
          if (lower(get_y<Y>(*i), get_y<Y>(*ymin))) {
            ymin = i;
          }
          if (lower(get_y<Y>(*ymax), get_y<Y>(*i))) {
            ymax = i;
          }
        }

        return make_range_pair(get_x<X>(*xmin), get_x<X>(*xmax), get_y<Y>(*ymin), get_y<Y>(*ymax));
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename L>
      range_pair<X, Y> find_min_max_ignore_0 (const L& v) {
        auto i = std::begin(v);
        auto last = std::end(v);

        auto xmin = last;
        auto ymin = last;
        while (++i < last) {
          const auto x = get_x<X>(*i);
          if (x != X(0)) {
            if ((xmin == last) || lower(x, get_x<X>(*xmin))) {
              xmin = i;
            }
          }
          const auto y = get_y<Y>(*i);
          if (y != Y(0)) {
            if ((ymin == last) || lower(y, get_y<Y>(*ymin))) {
              ymin = i;
            }
          }
        }
        i = std::begin(v);
        auto xmax = i;
        auto ymax = i;
        while (++i < last) {
          if (lower(get_x<X>(*xmax), get_x<X>(*i))) {
            xmax = i;
          }
          if (lower(get_y<Y>(*ymax), get_y<Y>(*i))) {
            ymax = i;
          }
        }
        if (xmin == last) {
          xmin = xmax;
        }
        if (ymin == last) {
          ymin = ymax;
        }

        return make_range_pair(get_x<X>(*xmin), get_x<X>(*xmax), get_y<Y>(*ymin), get_y<Y>(*ymax));
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
        return (t == 0) ? 0 : convert<T>(std::copysign(std::pow(10.0, std::floor(std::log10(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_bigger_pow10 (T t) {
        return (t == 0) ? 0 : convert<T>(std::copysign(std::pow(10.0, std::ceil(std::log10(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_pow2 (T t) {
        return (t == 0) ? 0 : convert<T>(std::copysign(std::pow(2.0, std::floor(std::log2(std::abs(t)))), t));
      }
      // --------------------------------------------------------------------------
      template<typename T>
      T next_bigger_pow2 (T t) {
        return (t == 0) ? 0 : convert<T>(std::copysign(std::pow(2.0, std::ceil(std::log2(std::abs(t)))), t));
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
            lmax = convert<T>(l * std::ceil(max / l));
            lmin = convert<T>(lmi * (min < 0 ? std::ceil(min / lmi) : std::floor(min / lmi)));
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
      template<typename S, scaling F, typename I, typename T>
      scaler<S, F, I, T>::scaler (core::range<S> src, core::range<T> target)
        : src(src)
        , target(target)
        , precalced(1)
        , precalced_min(target.begin())
      {
        if ((F != scaling::linear) && (src.begin() == S{})) {
          throw std::runtime_error("Base 0 is not allowed for logarithmic scaler");
        }
        precalc();
      }

      template<typename S, scaling F, typename I, typename T>
      T scaler<S, F, I, T>::operator() (S v) const {
        if (F != scaling::symlog) {
          if (v < src.begin()) {
            return target.begin();
          } else if (v > src.end()) {
            return target.end();
          }
        }
        const auto scaled = detail::scale_fn<S, T, F, I>::calc(v, src.begin());
        return convert<T>(((scaled - precalced_min) * precalced) + target.begin());
      }

      template<typename S, scaling F, typename I, typename T>
      const core::range<S>& scaler<S, F, I, T>::get_source () const {
        return src;
      }

      template<typename S, scaling F, typename I, typename T>
      const core::range<T>& scaler<S, F, I, T>::get_target () const {
        return target;
      }

      template<typename S, scaling F, typename I, typename T>
      void scaler<S, F, I, T>::set_source_range (core::range<S> s) {
        src = s;
        precalc();
      }

      template<typename S, scaling F, typename I, typename T>
      void scaler<S, F, I, T>::set_target_range (core::range<T> t) {
        target = t;
        precalc();
      }

      template<typename S, scaling F, typename I, typename T>
      const I scaler<S, F, I, T>::get_source_size () const {
        return src.end() - src.begin();
      }

      template<typename S, scaling F, typename I, typename T>
      const T scaler<S, F, I, T>::get_target_size () const {
        return target.end() - target.begin();
      }

      template<typename S, scaling F, typename I, typename T>
      void scaler<S, F, I, T>::precalc () {
        precalced_min = detail::scale_fn<S, T, F, I>::precalc(src);
        precalced = static_cast<double>(get_target_size()) /
                    static_cast<double>(detail::scale_fn<S, T, F, I>::range(src));
      }

      template<typename S, scaling F, typename I, typename T>
      scaler<S, F, I, T> mk_scaler (core::range<S> src,
                                    core::range<T> target) {
        return scaler<S, F, I, T>(src, target);
      }
      // --------------------------------------------------------------------------
      namespace paint {

        template<orientation_t V, scaling F, typename S, typename I, typename T>
        void draw_axis (graphics& g,
                        const core::point& pos,
                        const pen& p,
                        const scaler<S, F, I, T>& sc) {
          typedef core::orientation_traits<V> traits;

          const auto d2 = traits::get_2(pos);
          core::point p0, p1;
          traits::set(p0, sc.get_target().begin(), d2);
          traits::set(p1, sc.get_target().end(), d2);
          g.frame(line(p0, p1), p);
        }

        template<orientation_t V, scaling F, typename S, typename I, typename T>
        void draw_sub_ticks (graphics& g,
                             os::color color,
                             const scaler<S, F, I, T>& sc,
                             S min, I step, S max,
                             T sub_1, T sub_2) {
          typedef core::orientation_traits<V> traits;

          if ((step > I{}) && (sub_2 != sub_1) && (min + step < max)) {
            core::point p0, p1;
            traits::set_2(p0, sub_1);
            traits::set_2(p1, sub_2);
            for (S j = min + step; j < max; j += step) {
              const auto ds = sc(j);

              traits::set_1(p0, ds);
              traits::set_1(p1, ds);
              g.frame(line(p0, p1), color);
            }
          }
        }

      }

      // --------------------------------------------------------------------------
      template<typename S, orientation_t V, scaling F, origin_t O, typename I, typename T>
      scale<S, V, F, O, I, T>::scale (const core::point& pos,
                                      const scaler_type& sc,
                                      I main,
                                      I sub,
                                      T main_ticks_length,
                                      T sub_ticks_length,
                                      os::color main_color,
                                      os::color sub_color,
                                      scale_formatter<S> fmt)
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

      template<typename S, orientation_t V, scaling F, origin_t O, typename I, typename T>
      void scale<S, V, F, O, I, T>::operator() (graphics& g, const font& font, os::color color) const {

        core::point p0, p1, p2;
        const T d2 = convert<T>(traits::get_2(pos));

        traits::set_2(p0, (d2 + tick_dimension::main_tick_length));
        traits::set_2(p1, (d2 + main_ticks_length));
        traits::set_2(p2, (d2 + tick_dimension::main_tick_length * 2));

        const auto main_step = detail::scale_fn<S, T, F, I>::step(main);

        const auto min = sc.get_source().begin();
        const auto max = sc.get_source().end();
        auto i = detail::scale_fn<S, T, F, I>::min(sc.get_source());
        while (i <= max) {
          const auto d1 = sc(i);
          const auto next = detail::scale_fn<S, T, F, I>::inc(i, main_step, min);

          traits::set_1(p0, d1);
          traits::set_1(p1, d1);
          g.frame(line(p0, p1), main_color);

          traits::set_1(p2, d1);
          const auto text_origin = scale_text_origin<V, O>();
          g.text(draw::text(fmt(i), p2, text_origin), font, color);

          const auto sub_step = detail::scale_fn<S, T, F, I>::sub(i, sub, min);
          paint::draw_sub_ticks<V, F, S, I, T>(g, sub_color, sc,
                                            i, sub_step, std::min(next, max),
                                            d2 + tick_dimension::sub_tick_length, d2 + sub_ticks_length);
          i = next;
        }
      }

      template<orientation_t V, origin_t O, scaling F, typename S, typename I, typename T>
      scale<S, V, F, O, I, T> mk_scale (const core::point& pos,
                                        const scaler<S, F, I, T>& sc,
                                        I main, I sub,
                                        T main_ticks_length,
                                        T sub_ticks_length,
                                        os::color main_color,
                                        os::color sub_color,
                                        scale_formatter<S> fmt) {
        return scale<S, V, F, O, I, T>(pos, sc, main, sub, 
                                       main_ticks_length, sub_ticks_length,
                                       main_color, sub_color, fmt);
      }
      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      wall<X, Y, SX, SY, IX, IY, T>::wall (const scaler<X, SX, IX, T>& sx,
                                           const scaler<Y, SY, IY, T>& sy)
        : sx(sx)
        , sy(sy)
      {}

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void wall<X, Y, SX, SY, IX, IY, T>::operator() (graphics& g, const brush& b, const pen& p) const {
        g.fill(draw::rectangle(core::point(sx.get_target().begin(), sy.get_target().begin()),
                               core::point(sx.get_target().end(),   sy.get_target().end())), b);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      wall<X, Y, SX, SY, IX, IY, T> mk_wall (const scaler<X, SX, IX, T>& sx,
                                             const scaler<Y, SY, IY, T>& sy) {
        return wall<X, Y, SX, SY, IX, IY, T>(sx, sy);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      headline<X, Y, SX, SY, IX, IY, T>::headline (const scaler<X, SX, IX, T>& sx,
                                           const scaler<Y, SY, IY, T>& sy,
                                           const std::string& text)
        : sx(sx)
        , sy(sy)
        , text(text)
      {}

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void headline<X, Y, SX, SY, IX, IY, T>::operator() (graphics& g, const font& f, os::color color) const {
        core::point p(sx.get_target().begin(), sy.get_target().end());
        g.text(draw::text(text, p, text_origin_t::bottom_left), f, color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      headline<X, Y, SX, SY, IX, IY, T> mk_headline (const scaler<X, SX, IX, T>& sx,
                                                     const scaler<Y, SY, IY, T>& sy,
                                                     const std::string& s) {
        return headline<X, Y, SX, SY, IX, IY, T>(sx, sy, s);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      legend<X, Y, SX, SY, IX, IY, T>::legend (const scaler<X, SX, IX, T>& sx,
                                               const scaler<Y, SY, IY, T>& sy,
                                               const std::vector<legend_label>& labels)
        : sx(sx)
        , sy(sy)
        , labels(labels)
      {}

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void legend<X, Y, SX, SY, IX, IY, T>::operator() (graphics& g, const font& f, os::color color) const {
        core::point p(sx.get_target().begin(),
                      sy.get_target().begin() + scale_dim<X, orientation_t::horizontal>::main_tick_length * 2);
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

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      legend<X, Y, SX, SY, IX, IY, T> mk_legend (const scaler<X, SX, IX, T>& sx,
                                                 const scaler<Y, SY, IY, T>& sy,
                                                 const std::vector<legend_label>& l) {
        return legend<X, Y, SX, SY, IX, IY, T>(sx, sy, l);
      }

      // --------------------------------------------------------------------------
      template<orientation_t V, typename S, scaling F, typename I, typename T>
      axis<V, S, F, I, T>::axis (const core::point& pos,
                                 const scaler<S, F, I, T>& sc)
        : pos(pos)
        , sc(sc)
      {}

      template<orientation_t V, typename S, scaling F, typename I, typename T>
      void axis<V, S, F, I, T>::operator() (graphics& g, const pen& p) const {
        paint::draw_axis<V, F, S, I, T>(g, pos, p, sc);
      }

      template<orientation_t V, scaling F, typename S, typename I, typename T>
      axis<V, S, F, I, T> mk_axis (const core::point& pos,
                                   const scaler<S, F, I, T>& sc) {
        return axis<V, S, F, I, T>(pos, sc);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      xy_axis<X, Y, SX, SY, IX, IY, T>::xy_axis (const scaler<X, SX, IX, T>& sx,
                                                 const scaler<Y, SY, IY, T>& sy)
        : sx(sx)
        , sy(sy)
      {}

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void xy_axis<X, Y, SX, SY, IX, IY, T>::operator() (graphics& g, const pen& p) const {
        core::point pos(sx.get_target().begin(), sy.get_target().begin());
        paint::draw_axis<orientation_t::horizontal, SX, X, IX, T>(g, pos, p, sx);
        paint::draw_axis<orientation_t::vertical, SY, Y, IY, T>(g, pos, p, sy);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      xy_axis<X, Y, SX, SY, IX, IY, T> mk_xy_axis (const scaler<X, SX, IX, T>& sx,
                                           const scaler<Y, SY, IY, T>& sy) {
        return xy_axis<X, Y, SX, SY, IX, IY, T>(sx, sy);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      graph_base<X, Y, C, SX, SY, IX, IY, T>::graph_base (const scaler<X, SX, IX, T>& sx,
                                                          const scaler<Y, SY, IY, T>& sy,
                                                          point2d_data points)
        : sx(sx)
        , sy(sy)
        , points(points)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      core::rectangle graph_base<X, Y, C, SX, SY, IX, IY, T>::get_graph_area () const {
        const auto& sxt = sx.get_target();
        const auto& syt = sy.get_target();
        const auto x0 = std::min(sxt.begin(), sxt.end());
        const auto y0 = std::min(syt.begin(), syt.end());
        const auto x1 = std::max(sxt.begin(), sxt.end());
        const auto y1 = std::max(syt.begin(), syt.end());
        return core::rectangle(core::point(x0, y0), core::point(x1, y1)).grow({1, 1});
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      line_graph<X, Y, C, SX, SY, IX, IY, T>::line_graph (const scaler<X, SX, IX, T>& sx,
                                                          const scaler<Y, SY, IY, T>& sy,
                                                          typename super::point2d_data points,
                                                          Y zero)
        : super(sx, sy, points)
        , zero(zero)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void line_graph<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void line_graph<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());

        std::vector<core::point> pts;

        const auto sz = super::points.size();
        pts.reserve(sz+2);

        const core::point::type y0 = super::sy(zero);
        const core::point::type x0 = super::sx(get_x<X>(super::points[0]));
        const core::point::type x1 = super::sx(get_x<X>(super::points[sz - 1]));

        pts.push_back({ x0, y0 });
        calc_points(pts);
        pts.push_back({ x1, y0 });

        g.fill(draw::polygon(pts), b);
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void line_graph<X, Y, C, SX, SY, IX, IY, T>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz);
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          pts.push_back({super::sx(get_x<X>(pt)),
                         super::sy(get_y<Y>(pt))});
        }
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      line_graph<X, Y, C, SX, SY, IX, IY, T> mk_line_graph (const scaler<X, SX, IX, T>& sx,
                                                            const scaler<Y, SY, IY, T>& sy,
                                                            C data, Y zero) {
        return line_graph<X, Y, C, SX, SY, IX, IY, T>(sx, sy, data, zero);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      cascade<X, Y, C, SX, SY, IX, IY, T>::cascade (const scaler<X, SX, IX, T>& sx,
                                                    const scaler<Y, SY, IY, T>& sy,
                                                    typename super::point2d_data points,
                                                    Y zero)
        : super(sx, sy, points)
        , zero(zero)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void cascade<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const pen& p) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.frame(draw::polyline(pts), p);
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void cascade<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        std::vector<core::point> pts;
        calc_points(pts);
        g.fill(draw::polygon(pts), b);
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void cascade<X, Y, C, SX, SY, IX, IY, T>::calc_points (std::vector<core::point>& pts) const {
        const auto sz = super::points.size();
        pts.reserve(sz*2 + 1);
        const core::point::type y0 = super::sy(zero);
        core::point::type last_y = y0;
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          core::point::type next_x = super::sx(get_x<X>(pt));
          core::point::type next_y = super::sy(get_y<Y>(pt));
          pts.push_back({next_x, last_y});
          pts.push_back({next_x, next_y});
          last_y = next_y;
        }
        const core::point::type x1 = super::sx(get_x<X>(super::points[sz - 1]));
        pts.push_back({ x1, y0 });
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      cascade<X, Y, C, SX, SY, IX, IY, T> mk_cascade (const scaler<X, SX, IX, T>& sx,
                                                      const scaler<Y, SY, IY, T>& sy,
                                                      C data, Y zero) {
        return cascade<X, Y, C, SX, SY, IX, IY, T>(sx, sy, data, zero);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      bar_graph<X, Y, C, SX, SY, IX, IY, T>::bar_graph (const scaler<X, SX, IX, T>& sx,
                                                        const scaler<Y, SY, IY, T>& sy,
                                                        typename super::point2d_data points,
                                                        T space)
        : super(sx, sy, points)
        , space(space)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void bar_graph<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const brush& b) const {
        clip clp(g, super::get_graph_area());
        const auto sz = super::points.size();
        const auto w = std::max<T>(T(1), T((super::sx.get_target().end() - super::sx.get_target().begin()) / (sz * 2) - space));
        const auto y0 = super::sy(0);
        for (int i = 0; i < sz; ++i) {
          const auto pt = super::points[i];
          const auto x = super::sx(get_x<X>(pt));
          const auto y = super::sy(get_y<Y>(pt));
          g.fill(draw::rectangle(core::point(x - w, y0), core::point(x + w, y)), b);
        }
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      bar_graph<X, Y, C, SX, SY, IX, IY, T> mk_bar_graph (const scaler<X, SX, IX, T>& sx,
                                                          const scaler<Y, SY, IY, T>& sy,
                                                          C data, T space) {
        return bar_graph<X, Y, C, SX, SY, IX, IY, T>(sx, sy, data, space);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      points_graph<X, Y, C, SX, SY, IX, IY, T>::points_graph (const scaler<X, SX, IX, T>& sx,
                                                              const scaler<Y, SY, IY, T>& sy,
                                                              typename super::point2d_data points,
                                                              point_drawer drawer)
        : super(sx, sy, points)
        , drawer(drawer)
      {}

      template<typename X, typename Y, typename C, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void points_graph<X, Y, C, SX, SY, IX, IY, T>::operator() (graphics& g, const brush& b) const {
        if (drawer) {
          clip clp(g, super::get_graph_area());
          const auto sz = super::points.size();
          for (std::size_t i = 0; i < sz; ++i) {
            const auto pt = super::points[i];
            const auto x = super::sx(get_x<X>(pt));
            const auto y = super::sy(get_y<Y>(pt));
            drawer(g, b, core::point(x, y));
          }
        }
      }

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      points_graph<X, Y, C, SX, SY, IX, IY, T> mk_points_graph (const scaler<X, SX, IX, T>& sx,
                                                                const scaler<Y, SY, IY, T>& sy,
                                                                C data, point_drawer drawer) {
        return points_graph<X, Y, C, SX, SY, IX, IY, T>(sx, sy, data, drawer);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      constexpr os::color chart<X, Y, SX, SY, IX, IY, T>::wall_back;

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      chart<X, Y, SX, SY, IX, IY, T>::chart (const core::rectangle& area, core::range<X> range_x, core::range<Y> range_y)
        : p0(area.x() + 50, area.y2() - 50)
        , scale_x(range_x, {convert<T>(p0.x()), convert<T>(area.x2() - 50)})
        , scale_y(range_y, {convert<T>(p0.y()), convert<T>(area.y() + 20)})
      {}

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      auto chart<X, Y, SX, SY, IX, IY, T>::get_scale_x() const -> const scaler_x_type& {
        return scale_x;
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      auto chart<X, Y, SX, SY, IX, IY, T>::get_scale_y() const -> const scaler_y_type& {
        return scale_y;
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      const core::point& chart<X, Y, SX, SY, IX, IY, T>::get_point_00 () const {
        return p0;
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::fill_area (graphics& graph) const {
        graph.draw(wall<X, Y, SX, SY>(scale_x, scale_y), wall_back, wall_back);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_xscale (graphics& graph, IX main, IX sub, scale_formatter<X> fmt) const {
        graph.text(scale_x_type(p0, scale_x, main, sub,
                                convert<T>(scale_y.get_target_size()),
                                convert<T>(scale_y.get_target_size()),
                                color::very_light_gray,
                                color::very_very_light_gray,
                                fmt),
                   font::system_small(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_yscale (graphics& graph, IY main, IY sub, scale_formatter<Y> fmt) const {
        graph.text(scale_y_type(p0, scale_y, main, sub,
                                convert<T>(scale_x.get_target_size()),
                                convert<T>(scale_x.get_target_size()),
                                color::very_light_gray,
                                color::very_very_light_gray,
                                fmt),
                   font::system_small(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_axis (graphics& graph) const {
        graph.frame(xy_axis<X, Y, SX, SY>(scale_x, scale_y), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_title (graphics& graph, const std::string& title) const {
        graph.text(headline<X, Y, SX, SY>(scale_x, scale_y, title), font::system_bold(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_legend (graphics& graph, const std::vector<legend_label>& labels) const {
        graph.text(legend<X, Y, SX, SY>(scale_x, scale_y, labels), font::system(), color::black);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_background (graphics& graph, IX xmain, IX xsub, IY ymain, IY ysub) const {
        fill_area(graph);
        draw_xscale(graph, xmain, xsub);
        draw_yscale(graph, ymain, ysub);
        draw_axis(graph);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_line_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.frame(line_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_area_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.fill(line_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_cascade_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.frame(cascade<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_cascade_area_graph (graphics& graph, C data, os::color color, Y zero) const {
        graph.fill(cascade<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, zero), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_bar_graph (graphics& graph, C data, os::color color, Y space) const {
        graph.fill(bar_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, space), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_point_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, circle(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_diamond_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, diamond(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_cross_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, cross(radius)), color);
      }

      template<typename X, typename Y, scaling SX, scaling SY, typename IX, typename IY, typename T>
      template<typename C>
      void chart<X, Y, SX, SY, IX, IY, T>::draw_square_graph (graphics& graph, C data, os::color color, float radius) const {
        graph.fill(points_graph<X, Y, C, SX, SY, IX, IY, T>(scale_x, scale_y, data, square(radius)), color);
      }

      template<scaling SX, scaling SY, typename T, typename X, typename Y, typename IX, typename IY>
      chart<X, Y, SX, SY, IX, IY, T> mk_chart (const core::rectangle& area,
                                       core::range<X> range_x,
                                       core::range<Y> range_y) {
        return chart<X, Y, SX, SY, IX, IY, T>(area, range_x, range_y);
      }

      // --------------------------------------------------------------------------

    } // namespace diagram

  } // namespace draw

} // namespace gui
