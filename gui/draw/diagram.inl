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

    // --------------------------------------------------------------------------
    template<typename T>
    scaler<T>::scaler (T mi, T ma, T tmi, T tma)
      : min(mi)
      , max(ma)
      , target_min(tmi)
      , target_max(tma)
      , precalced(1)
    {
      precalc();
    }

    template<typename T>
    T scaler<T>::operator() (T v) const {
      return (v - min) * precalced + target_min;
    }

    template<typename T>
    void scaler<T>::set_min_max (T mi, T ma) {
      min = mi;
      max = ma;
      precalc();
    }

    template<typename T>
    void scaler<T>::set_target_min_max (T mi, T ma) {
      target_min = mi;
      target_max = ma;
      precalc();
    }

    template<typename T>
    T scaler<T>::get_min () const {
      return min;
    }

    template<typename T>
    T scaler<T>::get_max () const {
      return max;
    }

    template<typename T>
    T scaler<T>::get_target_min () const {
      return target_min;
    }

    template<typename T>
    T scaler<T>::get_target_max () const {
      return target_max;
    }

    template<typename T>
    void scaler<T>::precalc () {
      precalced = (target_max - target_min) / (max - min);
    }

    // --------------------------------------------------------------------------
    template<typename T, orientation_t V>
    scale<T, V>::scale (const core::point& pos,
                        const scaler<T>& sc,
                        T main, T sub,
                        T main_l, T sub_l,
                        formatter fmt)
      : pos(pos)
      , sc(sc)
      , main(main)
      , sub(sub)
      , main_l(main_l)
      , sub_l(sub_l)
      , fmt(fmt)
    {}

    template<typename T, orientation_t V>
    void scale<T, V>::operator() (const graphics& g, const font& font, os::color color) const {
      core::point p0, p1, p2;
      const T d2 = traits::get_2(pos);
      traits::set(p0, sc.get_target_min(), d2);
      traits::set(p1, sc.get_target_max(), d2);
      g.frame(line(p0, p1), color);
      const T max = sc.get_max();
      traits::set_2(p0, d2 - main_l);
      traits::set_2(p1, d2 + main_l);
      traits::set_2(p2, d2 - main_l * 2);
      for (T i = sc.get_min(); i <= max; i += main) {
        const T d1 = sc(i);
        traits::set_1(p0, d1);
        traits::set_1(p1, d1);
        g.frame(line(p0, p1), color);
        traits::set_1(p2, d1);
        g.text(draw::text(fmt(i), p2, scale_text_origin<V>()), font, color);
        if (i + sub < max) {
          traits::set_2(p0, d2 - sub_l);
          traits::set_2(p1, d2 + sub_l);
          const T smax = std::min(i + main, max);
          for (T j = i + sub; j < smax; j += sub) {
            const T ds = sc(j);
            traits::set_1(p0, ds);
            traits::set_1(p1, ds);
            g.frame(line(p0, p1), color);
          }
        }
      }
    }

    // --------------------------------------------------------------------------
    template<typename T>
    wall<T>::wall (const core::point& pos,
                   const scaler<T>& sx, T main_x, T sub_x,
                   const scaler<T>& sy, T main_y, T sub_y)
      : pos(pos)
      , sx(sx)
      , sy(sy)
      , main_x(main_x)
      , sub_x(sub_x)
      , main_y(main_y)
      , sub_y(sub_y)
    {}

    template<typename T>
    void wall<T>::operator() (const graphics& g, const brush& b, const pen& p) const {
      g.fill(draw::rectangle(pos, core::point(sx.get_target_max(), sy.get_target_max())), b);

      core::point p0(pos.x(), sy.get_target_min());
      core::point p1(pos.x(), sy.get_target_max());
      const T max_x = sx.get_max();
      for (T i = sx.get_min(); i <= max_x; i += main_x) {
        const T xm = sx(i);
        p0.x(xm);
        p1.x(xm);
        g.frame(line(p0, p1), p);
        if (i + sub_x < max_x) {
          const T smax = std::min(i + main_x, max_x);
          for (T j = i + sub_x; j < smax; j += sub_x) {
            const T xs = sx(j);
            p0.x(xs);
            p1.x(xs);
            g.frame(line(p0, p1), p);
          }
        }
      }

      p0.x(sx.get_target_min());
      p1.x(sx.get_target_max());
      const T max_y = sy.get_max();
      for (T i = sy.get_min(); i <= max_y; i += main_y) {
        const T ym = sy(i);
        p0.y(ym);
        p1.y(ym);
        g.frame(line(p0, p1), p);
        if (i + sub_y < max_y) {
          const T smax = std::min(i + main_y, max_y);
          for (T j = i + sub_y; j < smax; j += sub_y) {
            const T ys = sy(j);
            p0.y(ys);
            p1.y(ys);
            g.frame(line(p0, p1), p);
          }
        }
      }

    }

    // --------------------------------------------------------------------------
    template<typename T>
    graph_base<T>::graph_base (const core::point& pos,
                               const scaler<T>& sx,
                               const scaler<T>& sy,
                               point2d_data points)
      : pos(pos)
      , sx(sx)
      , sy(sy)
      , points(points)
    {}

    // --------------------------------------------------------------------------
    template<typename T>
    line_graph<T>::line_graph (const core::point& pos,
                               const scaler<T>& sx,
                               const scaler<T>& sy,
                               typename super::point2d_data points)
      : super(pos, sx, sy, points)
    {}

    // --------------------------------------------------------------------------
    template<typename T>
    void line_graph<T>::operator() (const graphics& g, const pen& p) const {
      std::vector<core::point> pts;

      const auto sz = super::points().size();
      pts.reserve(sz);
      for (int i = 0; i < sz; ++i) {
        const auto pt = super::points().at(i);
        pts.push_back({static_cast<float>(super::sx(pt[0])), static_cast<float>(super::sy(pt[1]))});
      }
      g.frame(draw::polyline(pts), p);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bar_graph<T>::bar_graph (const core::point& pos,
                             const scaler<T>& sx,
                             const scaler<T>& sy,
                             typename super::point2d_data points)
      : super(pos, sx, sy, points)
    {}

    // --------------------------------------------------------------------------
    template<typename T>
    void bar_graph<T>::operator() (const graphics& g, const brush& b) const {
      const auto sz = super::points().size();
      const float w = std::floor(static_cast<float>((super::sx.get_target_max() - super::sx.get_target_min()) / (sz * 2) - 1));
      const auto y0 = static_cast<float>(super::sy(0));
      for (int i = 0; i < sz; ++i) {
        const auto pt = super::points().at(i);
        const auto x = static_cast<float>(super::sx(pt[0]));
        const auto y = static_cast<float>(super::sy(pt[1]));
        g.fill(draw::rectangle(core::point(x - w, y0), core::point(x + w, y)), b);
      }
    }

    // --------------------------------------------------------------------------
    template<typename T>
    points_graph<T>::points_graph (const core::point& pos,
                                   const scaler<T>& sx,
                                   const scaler<T>& sy,
                                   typename super::point2d_data points)
      : super(pos, sx, sy, points)
    {}

    // --------------------------------------------------------------------------
    template<typename T>
    void points_graph<T>::operator() (const graphics& g, const brush& b) const {
      const auto sz = super::points().size();
      const float w = std::floor(static_cast<float>((super::sx.get_target_max() - super::sx.get_target_min()) / (sz * 2) - 1));
      for (int i = 0; i < sz; ++i) {
        const auto pt = super::points().at(i);
        const auto x = static_cast<float>(super::sx(pt[0]));
        const auto y = static_cast<float>(super::sy(pt[1]));
        g.fill(draw::arc(core::point(x, y), w, 0, 360), b);
      }
    }

    // --------------------------------------------------------------------------

  } // namespace draw

} // namespace gui
