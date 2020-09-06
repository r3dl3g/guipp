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

      template<typename T, std::size_t N>
      using point = std::array<T, N>;

      template<typename T>
      using point1d = point<T, 1>;

      template<typename T>
      using point2d = point<T, 2>;

      template<typename T>
      using point3d = point<T, 3>;

      template<typename T>
      struct scaler_base {
        typedef T value_type;

        scaler_base (T mi, T ma, T tmi, T tma);

        T get_min () const;
        T get_max () const;
        T get_target_min () const;
        T get_target_max () const;
        T get_target_range () const;

      protected:
        T min;
        T max;
        T target_min;
        T target_max;
      };

      // --------------------------------------------------------------------------
      enum scaling_type {
        linear,
        log
      };

      // --------------------------------------------------------------------------
      template<typename T, scaling_type S = scaling_type::linear>
      struct scaler : scaler_base<T> {
        typedef scaler_base<T> super;
        typedef T value_type;
        static const scaling_type scaling_type = S;

        scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

        T operator() (T v) const;

        void set_min_max (T mi, T ma);
        void set_target_min_max (T mi, T ma);
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
        return ostreamfmt(i);
      }

      // --------------------------------------------------------------------------
      namespace paint {

        template<typename T, orientation_t V, scaling_type S>
        void draw_axis (const graphics& g,
                        const core::point& pos,
                        os::color color,
                        const scaler<T, S>&);

        template<typename T, orientation_t V, scaling_type S>
        void draw_sub_ticks (const graphics& g,
                             os::color color,
                             const scaler<T, S>&,
                             T min, T step, T max, T sub_1, T sub_2);

      }

      // --------------------------------------------------------------------------
      template<typename T, orientation_t V, scaling_type S = scaling_type::linear>
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
               formatter fmt = default_formatter<T>);

        void operator() (const graphics&, const font&, os::color) const;

      private:
        const core::point pos;
        const scaler_type& sc;
        T main;
        T sub;
        T main_ticks_length;
        T sub_ticks_length;
        formatter fmt;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct wall {
        wall (const core::point& pos,
              const scaler_base<T>& sx,
              const scaler_base<T>& sy);

        void operator() (const graphics&, const brush&, const pen&) const;

      private:
        const core::point pos;
        const scaler_base<T>& sx;
        const scaler_base<T>& sy;
      };

      // --------------------------------------------------------------------------
      template<typename T>
      struct graph_data {
        typedef const graph_data& (provider) ();
        virtual std::size_t size () const = 0;
        virtual point2d<T> at (std::size_t) const = 0;

        const graph_data& operator ()() const {
          return *this;
        }

      };

      // --------------------------------------------------------------------------
      template<typename T,
               scaling_type SX,
               scaling_type SY>
      struct graph_base {
        using point2d_data = std::function<typename graph_data<T>::provider>;

        graph_base (const core::point& pos,
                    const scaler<T, SX>& sx,
                    const scaler<T, SY>& sy,
                    point2d_data);

        core::rectangle get_graph_area () const;

      protected:
        const core::point pos;
        const scaler<T, SX>& sx;
        const scaler<T, SY>& sy;
        point2d_data points;
      };

      // --------------------------------------------------------------------------
      template<typename T,
               scaling_type SX = scaling_type::linear,
               scaling_type SY = scaling_type::linear>
      struct line_graph : public graph_base<T, SX, SY> {
        typedef graph_base<T, SX, SY> super;

        line_graph (const core::point& pos,
                    const scaler<T, SX>& sx,
                    const scaler<T, SY>& sy,
                    typename super::point2d_data);

        void operator() (const graphics&, const pen&) const;

      };

      // --------------------------------------------------------------------------
      template<typename T,
               scaling_type SX = scaling_type::linear,
               scaling_type SY = scaling_type::linear>
      struct bar_graph : public graph_base<T, SX, SY> {
        typedef graph_base<T, SX, SY> super;

        bar_graph (const core::point& pos,
                   const scaler<T, SX>& sx,
                   const scaler<T, SY>& sy,
                   typename super::point2d_data);

        void operator() (const graphics&, const brush&) const;

      };

      // --------------------------------------------------------------------------
      template<typename T,
               scaling_type SX = scaling_type::linear,
               scaling_type SY = scaling_type::linear>
      struct points_graph : public graph_base<T, SX, SY> {
        typedef graph_base<T, SX, SY> super;

        points_graph (const core::point& pos,
                      const scaler<T, SX>& sx,
                      const scaler<T, SY>& sy,
                      typename super::point2d_data,
                      T radius = 3);

        void operator() (const graphics&, const brush&) const;

      protected:
        T radius;
      };

      // --------------------------------------------------------------------------

    } // namespace diagram
  } //namespace draw

} // namespace gui

#include <gui/draw/diagram.inl>
