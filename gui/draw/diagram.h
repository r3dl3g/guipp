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

      template<typename X, typename Y>
      struct point2d {
        X x;
        Y y;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename P>
      struct get {
        inline static X x (const P& p) {
          return static_cast<X>(p.x);
        }

        inline static Y y (const P& p) {
          return static_cast<Y>(p.y);
        }
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename P>
      inline X get_x (const P& p) {
        return get<X, Y, P>::x(p);
      }

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename P>
      inline Y get_y (const P& p) {
        return get<X, Y, P>::y(p);
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
        template<typename T, scaling S>
        struct scale_fn {
          static T inc (T, T, T);
          static T step (T);
          static T min (const core::range<T>&);

          static T calc (T, T);
          static T precalc (const core::range<T>&);
          static T range (const core::range<T>&);
        };

      } // namespace detail

      // --------------------------------------------------------------------------
      template<typename T, scaling S>
      struct limits {
        static core::range<T> calc (T min, T max);
      };

      // --------------------------------------------------------------------------
      typedef std::pair<os::color, std::string> legend_label;

      // --------------------------------------------------------------------------
      template<typename T, scaling S = scaling::linear>
      struct scaler {
        typedef T value_type;
        static const scaling scaling_type = S;

        explicit scaler (core::range<T> src = {T(0), T(1)}, core::range<T> target = {T(0), T(1)});

        T operator() (T v) const;

        const core::range<T>& get_source () const;
        const core::range<T>& get_target () const;

        void set_source_range (core::range<T> src);
        void set_target_range (core::range<T> target);
      private:
        void precalc ();

        core::range<T> src;
        core::range<T> target;

        double precalced;
        double precalced_min;
      };

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

        template<typename T, orientation_t V, scaling S>
        void draw_axis (graphics& g,
                        const core::point& pos,
                        os::color color,
                        const scaler<T, S>&);

        template<typename T, orientation_t V, scaling S>
        void draw_sub_ticks (graphics& g,
                             os::color color,
                             const scaler<T, S>&,
                             T min, T step, T max, T sub_1, T sub_2);

      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling S = scaling::linear, origin_t O = origin_t::start>
      struct scale {
        typedef core::orientation_traits<V> traits;
        typedef std::string (formatter_f) (T);
        typedef std::function<formatter_f> formatter;
        typedef scaler<T, S> scaler_type;
        typedef scale_dim<T, V, O> tick_dimension;

        scale (const core::point& pos,
               const scaler_type& sc,
               T main,
               T sub = 0,
               T main_ticks_length = 0,
               T sub_ticks_length = 0,
               os::color main_color = color::very_light_gray,
               os::color sub_color = color::very_very_light_gray,
               formatter fmt = default_formatter<T>);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const core::point pos;
        const scaler_type& sc;
        T main;
        T sub;
        T main_ticks_length;
        T sub_ticks_length;
        os::color main_color;
        os::color sub_color;
        formatter fmt;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX = scaling::linear, scaling SY = scaling::linear>
      struct wall {
        wall (const scaler<X, SX>& sx,
              const scaler<Y, SY>& sy);

        void operator() (graphics&, const brush&, const pen&) const;

      private:
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX = scaling::linear, scaling SY = scaling::linear>
      struct headline {
        headline (const scaler<X, SX>& sx,
                  const scaler<Y, SY>& sy,
                  const std::string&);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
        const std::string& text;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, scaling SX = scaling::linear, scaling SY = scaling::linear>
      struct legend {
        legend (const scaler<X, SX>& sx,
                const scaler<Y, SY>& sy,
                const std::vector<legend_label>& labels);

        void operator() (graphics&, const font&, os::color) const;

      private:
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
        const std::vector<legend_label> labels;
      };

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling S = scaling::linear>
      struct axis {
        axis (const core::point& pos,
              const scaler<T, S>& sc);

        void operator() (graphics&, const pen&) const;

      private:
        const core::point pos;
        const scaler<T, S>& sc;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y,
               scaling SX = scaling::linear, scaling SY = scaling::linear>
      struct xy_axis {
        xy_axis (const scaler<X, SX>& sx,
                 const scaler<Y, SY>& sy);

        void operator() (graphics&, const pen&) const;

      private:
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX,
               scaling SY>
      struct graph_base {
        typedef C point2d_data;

        graph_base (const scaler<X, SX>& sx,
                    const scaler<Y, SY>& sy,
                    point2d_data);

        core::rectangle get_graph_area () const;

      protected:
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
        point2d_data points;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct line_graph : public graph_base<X, Y, C, SX, SY> {
        typedef graph_base<X, Y, C, SX, SY> super;

        line_graph (const scaler<X, SX>& sx,
                    const scaler<Y, SY>& sy,
                    typename super::point2d_data,
                    Y zero = Y(0));

        void operator() (graphics&, const pen&) const;
        void operator() (graphics&, const brush&) const;

      private:
        void calc_points (std::vector<core::point>&) const;

        const Y zero;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct cascade : public graph_base<X, Y, C, SX, SY> {
        typedef graph_base<X, Y, C, SX, SY> super;

        cascade (const scaler<X, SX>& sx,
                 const scaler<Y, SY>& sy,
                 typename super::point2d_data,
                 Y zero = Y(0));

        void operator() (graphics&, const pen&) const;
        void operator() (graphics&, const brush&) const;

      private:
        void calc_points (std::vector<core::point>&) const;

        const Y zero;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct bar_graph : public graph_base<X, Y, C, SX, SY> {
        typedef graph_base<X, Y, C, SX, SY> super;

        bar_graph (const scaler<X, SX>& sx,
                   const scaler<Y, SY>& sy,
                   typename super::point2d_data,
                   X space = X(0));

        void operator() (graphics&, const brush&) const;

      private:
        const X space;

      };

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
      template<typename X, typename Y, typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct points_graph : public graph_base<X, Y, C, SX, SY> {
        typedef graph_base<X, Y, C, SX, SY> super;

        typedef void (draw_fn) (graphics&, const brush&, const core::point&);
        typedef std::function<draw_fn> point_drawer;

        points_graph (const scaler<X, SX>& sx,
                      const scaler<Y, SY>& sy,
                      typename super::point2d_data,
                      point_drawer drawer);

        void operator() (graphics&, const brush&) const;

      private:
        point_drawer drawer;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct chart {
        typedef scaler<X, SX> scaler_x_type;
        typedef scaler<Y, SY> scaler_y_type;

        typedef scale<X, orientation_t::horizontal, SX> scale_x_type;
        typedef scale<Y, orientation_t::vertical, SY> scale_y_type;

        static constexpr os::color wall_back = color::rgb_gray<0xF8>::value;

        chart (const core::rectangle& area, core::range<X> range_x, core::range<Y> range_y);

        void fill_area (graphics& graph) const;
        void draw_xscale (graphics& graph, X main, X sub, typename scale_x_type::formatter fmt = default_formatter<X>) const;
        void draw_yscale (graphics& graph, Y main, Y sub, typename scale_y_type::formatter fmt = default_formatter<Y>) const;
        void draw_axis (graphics& graph) const;
        void draw_title (graphics& graph, const std::string& title) const;
        void draw_legend (graphics& graph, const std::vector<legend_label>& labels) const;

        void draw_background (graphics& graph, X xmain, X xsub, Y ymain, Y ysub) const;

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

      // --------------------------------------------------------------------------

    } // namespace diagram
  } //namespace draw

} // namespace gui

#include <gui/draw/diagram.inl>
