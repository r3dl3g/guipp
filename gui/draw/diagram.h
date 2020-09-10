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
 * @brief     C++ API: diagram definition
 *
 * @file
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
      template<typename T, typename P>
      struct get {
        inline static T x (const P& p) {
          return p.x;
        }

        inline static T y (const P& p) {
          return p.y;
        }
      };

      // --------------------------------------------------------------------------
      template<typename T, typename P>
      inline T get_x (const P& p) {
        return get<T, P>::x(p);
      }

      // --------------------------------------------------------------------------
      template<typename T, typename P>
      inline T get_y (const P& p) {
        return get<T, P>::y(p);
      }

      // --------------------------------------------------------------------------
      template<typename T>
      T next_smaller_pow10 (T t);

      template<typename T>
      T next_bigger_pow10 (T t);

      template<typename T>
      T next_bigger_dezimal (T t);

      template<typename T>
      T next_smaller_dezimal (T t);

      // --------------------------------------------------------------------------
      enum scaling {
        linear,
        log
      };

      // --------------------------------------------------------------------------
      template<typename T, scaling S>
      struct limits {
        static core::range<T> calc (T min, T max);
      };

      // --------------------------------------------------------------------------
      template<typename T, scaling S = scaling::linear>
      struct scaler {
        typedef T value_type;
        static const scaling scaling_type = S;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        T get_min () const;
        T get_max () const;
        T get_target_min () const;
        T get_target_max () const;
        T get_target_range () const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);
      private:
        void precalc ();

        T min;
        T max;
        T target_min;
        T target_max;

        double precalced;
        double precalced_min;
      };

      // --------------------------------------------------------------------------
      template<orientation_t V>
      text_origin_t scale_text_origin ();

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::horizontal> () {
        return text_origin_t::top_hcenter;
      }

      template<>
      constexpr text_origin_t scale_text_origin<orientation_t::vertical> () {
        return text_origin_t::vcenter_right;
      }

      // --------------------------------------------------------------------------
      template<typename T>
      std::string default_formatter (T i) {
        return ostreamfmt(std::fixed << std::setprecision(1) << i);
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<typename T, orientation_t V, scaling S>
        void draw_axis (const graphics& g,
                        const core::point& pos,
                        os::color color,
                        const scaler<T, S>&);

        template<typename T, orientation_t V, scaling S>
        void draw_sub_ticks (const graphics& g,
                             os::color color,
                             const scaler<T, S>&,
                             T min, T step, T max, T sub_1, T sub_2);

      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling S = scaling::linear>
      struct scale {
        typedef core::orientation_traits<V> traits;
        typedef std::string (formatter_f) (T);
        typedef std::function<formatter_f> formatter;
        typedef scaler<T, S> scaler_type;

        scale (const core::point& pos,
               const scaler_type& sc,
               T main,
               T sub = 0,
               T main_ticks_length = 0,
               T sub_ticks_length = 0,
               os::color main_color = color::very_light_gray,
               os::color sub_color = color::very_very_light_gray,
               formatter fmt = default_formatter<T>);

        void operator() (const graphics&, const font&, os::color) const;

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
        wall (const core::point& pos,
              const scaler<X, SX>& sx,
              const scaler<Y, SY>& sy);

        void operator() (const graphics&, const brush&, const pen&) const;

      private:
        const core::point pos;
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y,
               scaling SX = scaling::linear, scaling SY = scaling::linear>
      struct axis {
        axis (const core::point& pos,
              const scaler<X, SX>& sx,
              const scaler<Y, SY>& sy);

        void operator() (const graphics&, const pen&) const;

      private:
        const core::point pos;
        const scaler<X, SX>& sx;
        const scaler<Y, SY>& sy;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX,
               scaling SY>
      struct graph_base {
        typedef C point2d_data;

        graph_base (const core::point& pos,
                    const scaler<X, SX>& sx,
                    const scaler<Y, SY>& sy,
                    point2d_data);

        core::rectangle get_graph_area () const;

      protected:
        const core::point pos;
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

        line_graph (const core::point& pos,
                    const scaler<X, SX>& sx,
                    const scaler<Y, SY>& sy,
                    typename super::point2d_data,
                    Y zero = Y(0));

        void operator() (const graphics&, const pen&) const;
        void operator() (const graphics&, const brush&) const;

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

        cascade (const core::point& pos,
                 const scaler<X, SX>& sx,
                 const scaler<Y, SY>& sy,
                 typename super::point2d_data,
                 Y zero = Y(0));

        void operator() (const graphics&, const pen&) const;
        void operator() (const graphics&, const brush&) const;

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

        bar_graph (const core::point& pos,
                   const scaler<X, SX>& sx,
                   const scaler<Y, SY>& sy,
                   typename super::point2d_data,
                   X space = X(0));

        void operator() (const graphics&, const brush&) const;

      private:
        const X space;

      };

      // --------------------------------------------------------------------------
      struct circle {
        circle (float radius);
        void operator() (const graphics&, const brush&, const core::point&);
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct diamond {
        diamond (float radius);
        void operator() (const graphics&, const brush&, const core::point&);
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct cross {
        cross (float radius);
        void operator() (const graphics&, const brush&, const core::point&);
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      struct square {
        square (float radius);
        void operator() (const graphics&, const brush&, const core::point&);
      private:
        float radius;
      };

      // --------------------------------------------------------------------------
      template<typename X, typename Y, typename C,
               scaling SX = scaling::linear,
               scaling SY = scaling::linear>
      struct points_graph : public graph_base<X, Y, C, SX, SY> {
        typedef graph_base<X, Y, C, SX, SY> super;

        typedef void (draw_fn) (const graphics&, const brush&, const core::point&);
        typedef std::function<draw_fn> point_drawer;

        points_graph (const core::point& pos,
                      const scaler<X, SX>& sx,
                      const scaler<Y, SY>& sy,
                      typename super::point2d_data,
                      point_drawer drawer);

        void operator() (const graphics&, const brush&) const;

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

        chart (const core::rectangle& area, X xmin, X xmax, Y ymin, Y ymax);

        void fill_area (const graphics& graph) const;
        void draw_xscale (const graphics& graph, X main, X sub, typename scale_x_type::formatter fmt = default_formatter<X>) const;
        void draw_yscale (const graphics& graph, Y main, Y sub, typename scale_y_type::formatter fmt = default_formatter<Y>) const;
        void draw_axis (const graphics& graph) const;

        void draw_background (const graphics& graph, X xmain, X xsub, Y ymain, Y ysub) const;

        template<typename C>
        void draw_line_graph (const graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_area_graph (const graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_cascade_graph (const graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_cascade_area_graph (const graphics& graph, C data, os::color color, Y zero = Y(0)) const;

        template<typename C>
        void draw_bar_graph (const graphics& graph, C data, os::color color, Y space = Y(0)) const;

        template<typename C>
        void draw_point_graph (const graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_diamond_graph (const graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_cross_graph (const graphics& graph, C data, os::color color, float radius) const;

        template<typename C>
        void draw_square_graph (const graphics& graph, C data, os::color color, float radius) const;

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
