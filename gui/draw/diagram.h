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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <array>
#include <iomanip>
#include <functional>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/orientation_traits.h>
#include <gui/draw/graphics.h>
#include <gui/draw/drawers.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <util/ostreamfmt.h>


namespace gui {

  namespace draw {

    namespace diagram {

      // --------------------------------------------------------------------------
      template<typename T, typename S>
      inline T convert (const S& s) {
        return static_cast<T>(s);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      struct point2d {
        X x;
        Y y;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename P>
      inline X get_x (const P& p) {
        return convert<X>(p.x);
      }

      // --------------------------------------------------------------------------
      template<typename Y, typename P>
      inline Y get_y (const P& p) {
        return convert<Y>(p.y);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      using range_pair = std::pair<core::range<X>, core::range<Y>>;

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      range_pair<X, Y> make_range_pair (X x0, X x1, Y y0, Y y1);

      // --------------------------------------------------------------------------
      template<typename X, typename Y>
      range_pair<X, Y> get_min_max (const range_pair<X, Y>& lhs, const range_pair<X, Y>& rhs);

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C>
      range_pair<X, Y> find_min_max (const C& v);

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C>
      range_pair<X, Y> find_min_max_ignore_0 (const C& v);

      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_pow10 (T t);

      template<typename T>
      T next_bigger_pow10 (T t);

      template<typename T>
      T next_smaller_pow2 (T t);

      template<typename T>
      T next_bigger_pow2 (T t);

      template<typename T>
      T next_bigger_dezimal (T t);

      template<typename T>
      T next_smaller_dezimal (T t);

      template<typename T>
      struct default_sub_type {
        typedef T type;
      };

      // --------------------------------------------------------------------------
      enum scaling {
        linear,
        log,
        log2,
        logn,
        symlog
      };

      namespace detail {

        // --------------------------------------------------------------------------
        template<typename S, typename T, scaling F, typename I = typename default_sub_type<S>::type>
        struct scale_fn {
          static S inc (S i, I step, S min);
          static I step (I step);
          static I sub (S i, I t, S min);
          static S min (const core::range<S>&);

          static T calc (S i, S min);
          static T precalc (const core::range<S>&);
          static T range (const core::range<S>&);
        };

      } // namespace detail

      // --------------------------------------------------------------------------
      template<typename T, scaling F>
      struct limits {
        static core::range<T> calc (T min, T max);
      };

      // --------------------------------------------------------------------------
      typedef std::pair<os::color, std::string> legend_label;

      // --------------------------------------------------------------------------
      template<typename S,
               scaling F = scaling::linear,
               typename I = typename default_sub_type<S>::type,
               typename T = core::point::type>
      struct scaler {
        typedef S source_type;
        typedef I increment_type;
        typedef T target_type;

        static const scaling scaling_type = F;

        explicit scaler (core::range<S> src = {S(0), S(1)},
                         core::range<T> target = {T(0), T(1)});

        T operator() (S v) const;

        const core::range<S>& get_source () const;
        const core::range<T>& get_target () const;

        void set_source_range (core::range<S> src);
        void set_target_range (core::range<T> target);

        const I get_source_size () const;
        const T get_target_size () const;

      private:
        void precalc ();

        core::range<S> src;
        core::range<T> target;

        double precalced;
        T precalced_min;
      };

      template<scaling F, typename S, typename I = typename default_sub_type<S>::type, typename T = core::point::type>
      scaler<S, F, I, T> mk_scaler (core::range<S> src = {S(0), S(1)},
                                    core::range<T> target = {T(0), T(1)});

      // --------------------------------------------------------------------------
      template<orientation_t V, origin_t O>
      text_origin_t scale_text_origin ();

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, origin_t O = origin_t::start>
      struct scale_dim {};

      // --------------------------------------------------------------------------
      template<typename T>
      std::string default_formatter (T i) {
        return ostreamfmt(std::fixed << std::setprecision(1) << i);
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<orientation_t V, scaling F, typename S, typename I, typename T>
        void draw_axis (graphics& g,
                        const core::point& pos,
                        os::color color,
                        const scaler<S, F, I, T>&);

        template<orientation_t V, scaling F, typename S, typename I, typename T>
        void draw_sub_ticks (graphics& g,
                             os::color color,
                             const scaler<S, F, I, T>&,
                             S min, I step, S max, T sub_1, T sub_2);

      }

      // --------------------------------------------------------------------------
      template<typename T>
      using scale_formatter = std::function<std::string(T)>;

      // --------------------------------------------------------------------------
      template<typename S,
               orientation_t V,
               scaling F = scaling::linear,
               origin_t O = origin_t::start,
               typename I = typename default_sub_type<S>::type, 
               typename T = core::point::type>
      struct scale {
        typedef core::orientation_traits<V> traits;
        typedef scaler<S, F, I, T> scaler_type;
        typedef scale_dim<T, V, O> tick_dimension;

        scale (const core::point& pos,
               const scaler_type& sc,
               I main,
               I sub = 0,
               T main_ticks_length = 0,
               T sub_ticks_length = 0,
               const std::string& u = {},
               os::color main_color = color::very_light_gray,
               os::color sub_color = color::very_very_light_gray,
               scale_formatter<S> fmt = default_formatter<S>);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const core::point pos;
        const scaler_type& sc;
        I main;
        I sub;
        T main_ticks_length;
        T sub_ticks_length;
        std::string unit;
        os::color main_color;
        os::color sub_color;
        scale_formatter<S> fmt;
      };

      template<orientation_t V, origin_t O = origin_t::start,
               scaling F, typename S, typename I = typename default_sub_type<S>::type,
               typename T = core::point::type>
      scale<S, V, F, O, I, T> mk_scale (const core::point& pos,
                                        const scaler<S, F, I, T>& sc,
                                        I main,
                                        I sub = I{0},
                                        T main_ticks_length = T{0},
                                        T sub_ticks_length = T{0},
                                        const std::string& unit = {},
                                        os::color main_color = color::very_light_gray,
                                        os::color sub_color = color::very_very_light_gray,
                                        scale_formatter<S> fmt = default_formatter<S>);

      // --------------------------------------------------------------------------
      template<typename SX,
               typename SY,
               typename T = core::point::type>
      struct wall {
        typedef SX scaler_x;
        typedef SY scaler_y;
        wall (const scaler_x& sx,
              const scaler_y& sy);

        void operator() (graphics&, const brush&, const pen&) const;

      private:
        const scaler_x& sx;
        const scaler_y& sy;
      };

      template<typename SX, typename SY, typename T = core::point::type>
      wall<SX, SY, T> mk_wall (const SX& sx, const SY& sy);

      // --------------------------------------------------------------------------
      template<typename SX,
               typename SY,
               typename T = core::point::type>
      struct headline {
        typedef SX scaler_x;
        typedef SY scaler_y;
        headline (const scaler_x& sx,
                  const scaler_y& sy,
                  const std::string&);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const scaler_x& sx;
        const scaler_y& sy;
        const std::string& text;
      };

      template<typename SX, typename SY, typename T = core::point::type>
      headline<SX, SY, T> mk_headline (const SX& sx, const SY& sy,
                                       const std::string& s);

      // --------------------------------------------------------------------------
      template<typename SX,
               typename SY,
               typename T = core::point::type>
      struct legend {
        typedef SX scaler_x;
        typedef SY scaler_y;
        legend (const scaler_x& sx,
                const scaler_y& sy,
                const std::vector<legend_label>& labels);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const scaler_x& sx;
        const scaler_y& sy;
        const std::vector<legend_label> labels;
      };

      template<typename SX, typename SY, typename T = core::point::type>
      legend<SX, SY, T> mk_legend (const SX& sx, const SY& sy,
                                   const std::vector<legend_label>& l);

      // --------------------------------------------------------------------------
      template<orientation_t V, typename S>
      struct axis {
        typedef S scaler_t;
        axis (const core::point& pos, const scaler_t& sc);

        void operator() (graphics&, const pen&) const;

      private:
        const core::point pos;
        const scaler_t& sc;
      };

      template<orientation_t V, typename S>
      axis<V, S> mk_axis (const core::point& pos, const S& sc);

      // --------------------------------------------------------------------------
      template<typename SX, typename SY>
      struct xy_axis {
        typedef SX scaler_x;
        typedef SY scaler_y;
        xy_axis (const scaler_x& sx,
                 const scaler_y& sy);

        void operator() (graphics&, const pen&) const;

      private:
        const scaler_x& sx;
        const scaler_y& sy;
      };

      template<typename SX, typename SY>
      xy_axis<SX, SY> mk_xy_axis (const SX& sx, const SY& sy);

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               typename C,
               scaling SX,
               scaling SY,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct graph_base {
        typedef C point2d_data;

        graph_base (const scaler<X, SX, IX, T>& sx,
                    const scaler<Y, SY, IY, T>& sy,
                    point2d_data);

        core::rectangle get_graph_area () const;

      protected:
        const scaler<X, SX, IX, T>& sx;
        const scaler<Y, SY, IY, T>& sy;
        point2d_data points;
      };

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct line_graph : public graph_base<X, Y, C, SX, SY, IX, IY, T> {
        typedef graph_base<X, Y, C, SX, SY, IX, IY, T> super;

        line_graph (const scaler<X, SX, IX, T>& sx,
                    const scaler<Y, SY, IY, T>& sy,
                    typename super::point2d_data,
                    Y zero = Y(0));

        void operator() (graphics&, const pen&) const;
        void operator() (graphics&, const brush&) const;

      private:
        void calc_points (std::vector<core::point>&) const;

        const Y zero;
      };

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      line_graph<X, Y, C, SX, SY, IX, IY, T> mk_line_graph (const scaler<X, SX, IX, T>& sx,
                                                            const scaler<Y, SY, IY, T>& sy,
                                                            C data, Y zero = Y(0));

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct cascade : public graph_base<X, Y, C, SX, SY, IX, IY, T> {
        typedef graph_base<X, Y, C, SX, SY, IX, IY, T> super;

        cascade (const scaler<X, SX, IX, T>& sx,
                 const scaler<Y, SY, IY, T>& sy,
                 typename super::point2d_data,
                 Y zero = Y(0));

        void operator() (graphics&, const pen&) const;
        void operator() (graphics&, const brush&) const;

      private:
        void calc_points (std::vector<core::point>&) const;

        const Y zero;
      };

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      cascade<X, Y, C, SX, SY, IX, IY, T> mk_cascade (const scaler<X, SX, IX, T>& sx,
                                                      const scaler<Y, SY, IY, T>& sy,
                                                      C data, Y zero = Y(0));

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct bar_graph : public graph_base<X, Y, C, SX, SY, IX, IY, T> {
        typedef graph_base<X, Y, C, SX, SY, IX, IY, T> super;

        bar_graph (const scaler<X, SX, IX, T>& sx,
                   const scaler<Y, SY, IY, T>& sy,
                   typename super::point2d_data,
                   T space = X(0));

        void operator() (graphics&, const brush&) const;

      private:
        const T space;

      };

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      bar_graph<X, Y, C, SX, SY, IX, IY, T> mk_bar_graph (const scaler<X, SX, IX, T>& sx,
                                                          const scaler<Y, SY, IY, T>& sy,
                                                          C data, T space = X(0));

      // --------------------------------------------------------------------------
      typedef void (point_draw_fn) (graphics&, const brush&, const core::point&);
      typedef std::function<point_draw_fn> point_drawer;

      // --------------------------------------------------------------------------
      struct GUIPP_DRAW_EXPORT circle {
        explicit circle (float radius);
        void operator() (graphics&, const brush&, const core::point&) const;
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct GUIPP_DRAW_EXPORT diamond {
        explicit diamond (float radius);
        void operator() (graphics&, const brush&, const core::point&) const;
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct GUIPP_DRAW_EXPORT cross {
        explicit cross (float radius);
        void operator() (graphics&, const brush&, const core::point&) const;
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct GUIPP_DRAW_EXPORT square {
        explicit square (float radius);
        void operator() (graphics&, const brush&, const core::point&) const;
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct points_graph : public graph_base<X, Y, C, SX, SY, IX, IY, T> {
        typedef graph_base<X, Y, C, SX, SY, IX, IY, T> super;

        points_graph (const scaler<X, SX, IX, T>& sx,
                      const scaler<Y, SY, IY, T>& sy,
                      typename super::point2d_data,
                      point_drawer drawer);

        void operator() (graphics&, const brush&) const;

      private:
        point_drawer drawer;
      };

      template<typename X, typename Y, typename C, scaling SX, scaling SY,
               typename IX, typename IY, typename T>
      points_graph<X, Y, C, SX, SY, IX, IY, T> mk_points_graph (const scaler<X, SX, IX, T>& sx,
                                                                const scaler<Y, SY, IY, T>& sy,
                                                                C data, point_drawer drawer);

      // --------------------------------------------------------------------------
      template<typename X,
               typename Y,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type,
               typename T = core::point::type>
      struct chart {
        typedef scaler<X, SX, IX, T> scaler_x_type;
        typedef scaler<Y, SY, IY, T> scaler_y_type;

        typedef scale<X, orientation_t::horizontal, SX, origin_t::start, IX, T> scale_x_type;
        typedef scale<Y, orientation_t::vertical, SY, origin_t::start, IY, T> scale_y_type;

        static constexpr os::color wall_back = color::rgb_gray<0xF8>::value;

        chart (const core::rectangle& area, core::range<X> range_x, core::range<Y> range_y);

        void fill_area (graphics& graph) const;
        void draw_xscale (graphics& graph, IX main, IX sub, const std::string& u = {}, scale_formatter<X> fmt = default_formatter<X>) const;
        void draw_yscale (graphics& graph, IY main, IY sub, const std::string& u = {}, scale_formatter<Y> fmt = default_formatter<Y>) const;
        void draw_axis (graphics& graph) const;
        void draw_title (graphics& graph, const std::string& title) const;
        void draw_legend (graphics& graph, const std::vector<legend_label>& labels) const;

        void draw_background (graphics& graph, IX xmain, IX xsub, IY ymain, IY ysub) const;

        template<typename C>
        void draw_line_graph (graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_area_graph (graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_cascade_graph (graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_cascade_area_graph (graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_bar_graph (graphics& graph, C data, os::color color, Y space = Y(0)) const;

        template<typename C>
        void draw_point_graph (graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_diamond_graph (graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_cross_graph (graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_square_graph (graphics& graph, C data, os::color color, float radius) const;

        const scaler_x_type& get_scale_x() const;
        const scaler_y_type& get_scale_y() const;
        const core::point& get_point_00 () const;

      private:
        core::point p0;
        scaler_x_type scale_x;
        scaler_y_type scale_y;
      };

      template<scaling SX = scaling::linear,
               scaling SY = scaling::linear,
               typename T = core::point::type,
               typename X,
               typename Y,
               typename IX = typename default_sub_type<X>::type,
               typename IY = typename default_sub_type<Y>::type>
      chart<X, Y, SX, SY, IX, IY, T> mk_chart (const core::rectangle& area,
                                               core::range<X> range_x,
                                               core::range<Y> range_y);

      // --------------------------------------------------------------------------

    } // namespace diagram
  } //namespace draw

} // namespace gui

#include <gui/draw/diagram.inl>
