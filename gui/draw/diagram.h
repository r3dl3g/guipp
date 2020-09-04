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

    template<typename T, std::size_t N>
    using point = std::array<T, N>;

    template<typename T>
    using point1d = point<T, 1>;

    template<typename T>
    using point2d = point<T, 2>;

    template<typename T>
    using point3d = point<T, 3>;

    // --------------------------------------------------------------------------
    template<typename T>
    struct scaler {
      scaler (T mi = 0, T ma = 1, T tmi = 0, T tma = 1);

      T operator() (T v) const;

      void set_min_max (T mi, T ma);
      void set_target_min_max (T mi, T ma);

      T get_min () const;
      T get_max () const;
      T get_target_min () const;
      T get_target_max () const;

    private:
      void precalc ();

      T min;
      T max;
      T target_min;
      T target_max;
      T precalced;
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
    template<typename T, orientation_t V>
    struct scale {
      typedef core::orientation_traits<V> traits;
      typedef std::string (formatter_f) (T);
      typedef std::function<formatter_f> formatter;

      scale (const core::point& pos,
             const scaler<T>& sc,
             T main,
             T main_l,
             T sub = 0,
             T sub_l = 0,
             formatter fmt = default_formatter<T>);

      void operator() (const graphics&, const font&, os::color) const;

    private:
      const core::point pos;
      const scaler<T>& sc;
      T main;
      T main_l;
      T sub;
      T sub_l;
      formatter fmt;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct wall {
      wall (const core::point& pos,
            const scaler<T>& sx,
            const scaler<T>& sy,
            T main_x,
            T main_y,
            T sub_x = 0,
            T sub_y = 0);

      void operator() (const graphics&, const brush&, const pen&) const;

    private:
      const core::point pos;
      const scaler<T>& sx;
      const scaler<T>& sy;
      T main_x;
      T main_y;
      T sub_x;
      T sub_y;
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
    template<typename T>
    struct graph_base {
      using point2d_data = std::function<typename graph_data<T>::provider>;

      graph_base (const core::point& pos,
                  const scaler<T>& sx, const scaler<T>& sy,
                  point2d_data);

    protected:
      const core::point pos;
      const scaler<T>& sx;
      const scaler<T>& sy;
      point2d_data points;
    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct line_graph : public graph_base<T> {
      typedef graph_base<T> super;

      line_graph (const core::point& pos,
                  const scaler<T>& sx,
                  const scaler<T>& sy,
                  typename super::point2d_data);

      void operator() (const graphics&, const pen&) const;

    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct bar_graph : public graph_base<T> {
      typedef graph_base<T> super;

      bar_graph (const core::point& pos,
                 const scaler<T>& sx,
                 const scaler<T>& sy,
                 typename super::point2d_data);

      void operator() (const graphics&, const brush&) const;

    };

    // --------------------------------------------------------------------------
    template<typename T>
    struct points_graph : public graph_base<T> {
      typedef graph_base<T> super;

      points_graph (const core::point& pos,
                    const scaler<T>& sx,
                    const scaler<T>& sy,
                    typename super::point2d_data);

      void operator() (const graphics&, const brush&) const;

    };

    // --------------------------------------------------------------------------

  } //namespace draw

} // namespace gui

#include <gui/draw/diagram.inl>
