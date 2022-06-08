/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     layout to position windows with changable dimension on top, bottm, left, right or center of a parent
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/layout/border_layout.h>


namespace gui {

  namespace win {
    class container;
  }

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    enum class border_layout_type {
      all_symmetric,
      top_bottom_maximize,
      left_right_maximize,
      bottom_max_top_min,
      top_max_bottom_min,
      left_max_right_min,
      right_max_left_min,
      top_left_maximize,
      bottom_right_maximize,
    };

    // --------------------------------------------------------------------------
    namespace detail {

      class dynamic_border_layout_base : public border::layout_base {
      public:
        typedef layout_base super;

        explicit dynamic_border_layout_base (float top_height = 0,
                                             float bottom_height = 0,
                                             float left_width = 0,
                                             float right_width = 0);

        float get_top_height () const;
        float get_bottom_height () const;
        float get_left_width () const;
        float get_right_width () const;

        void set_top_height (float v);
        void set_bottom_height (float v);
        void set_left_width (float v);
        void set_right_width (float v);

      protected:
        struct dynamic_border_layout_data : public layout_base_data {
          typedef layout_base_data super;

          explicit dynamic_border_layout_data (float top_height = 0,
                                               float bottom_height = 0,
                                               float left_width = 0,
                                               float right_width = 0);

          float top_height;
          float bottom_height;
          float left_width;
          float right_width;

        } data;

      };

      // --------------------------------------------------------------------------
      template<typename T1, typename T2>
      std::pair<core::point::type, core::point::type> make_points (const T1& t1, const T2& t2);

      // --------------------------------------------------------------------------
      template<border_layout_type T>
      struct dynamic_border_layout_geometrie {
        typedef std::pair<core::point::type, core::point::type> points;
        static points get_top_position (const core::rectangle&, const core::rectangle&);
        static points get_bottom_position (const core::rectangle&, const core::rectangle&);
        static points get_left_position (const core::rectangle&, const core::rectangle&);
        static points get_right_position (const core::rectangle&, const core::rectangle&);
      };

      // --------------------------------------------------------------------------
    } // namespace detail

    // --------------------------------------------------------------------------
    template<border_layout_type type = border_layout_type::top_bottom_maximize, int F = 0>
    class dynamic_border_layout : public detail::dynamic_border_layout_base {
    public:
      typedef detail::dynamic_border_layout_base super;
      typedef detail::dynamic_border_layout_geometrie<type> geometrie;

      dynamic_border_layout (win::container*,
                             float top_height = 0,
                             float bottom_height = 0,
                             float left_width = 0,
                             float right_width = 0);

      dynamic_border_layout (float top_height = 0,
                             float bottom_height = 0,
                             float left_width = 0,
                             float right_width = 0);

      void layout (const core::rectangle& r);
    };

    template<border_layout_type T, int F>
    struct is_layout<dynamic_border_layout<T, F>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/dynamic_border_layout.inl>
