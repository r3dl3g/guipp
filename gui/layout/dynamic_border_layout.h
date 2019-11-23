/**
 * @copyright (c) 2015-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: layout
 *
 * @file
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
#include <gui/layout/layout.h>


namespace gui {

  namespace win {
    class container;
  }

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    enum class dynamic_border_layout_type {
      all_symmetric,
      top_bottom_maximize,
      left_right_maximize,
      top_left_maximize,
      bottom_right_maximize,
      bottom_max_top_min
    };

    // --------------------------------------------------------------------------
    namespace detail {

      class dynamic_border_layout_base {
      public:
        dynamic_border_layout_base (float top_height = 0,
                            float bottom_height = 0,
                            float left_width = 0,
                            float right_width = 0);

        const layout_function& get_center () const;
        const layout_function& get_top () const;
        const layout_function& get_bottom () const;
        const layout_function& get_left () const;
        const layout_function& get_right () const;

        void set_center (layout_function center);
        void set_top (layout_function top);
        void set_bottom (layout_function bottom);
        void set_left (layout_function left);
        void set_right (layout_function right);

        void set_center_top_bottom_left_right (layout_function center,
                                               layout_function top,
                                               layout_function bottom,
                                               layout_function left,
                                               layout_function right);

        float get_top_height () const;
        float get_bottom_height () const;
        float get_left_width () const;
        float get_right_width () const;

        void set_top_height (float v);
        void set_bottom_height (float v);
        void set_left_width (float v);
        void set_right_width (float v);

      protected:
        struct dynamic_border_layout_data {
          dynamic_border_layout_data (float top_height = 0,
                              float bottom_height = 0,
                              float left_width = 0,
                              float right_width = 0);

          layout_function center;
          layout_function top;
          layout_function bottom;
          layout_function left;
          layout_function right;

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
      template<dynamic_border_layout_type T>
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
    template<dynamic_border_layout_type type = dynamic_border_layout_type::top_bottom_maximize>
    class dynamic_border_layout : public detail::dynamic_border_layout_base {
    public:
      typedef detail::dynamic_border_layout_base super;
      typedef detail::dynamic_border_layout_geometrie<type> geometrie;

      dynamic_border_layout (win::container*,
                     float top_height,
                     float bottom_height,
                     float left_width,
                     float right_width);

      dynamic_border_layout (float top_height,
                     float bottom_height,
                     float left_width,
                     float right_width);

      void layout (const core::rectangle& sz);
    };

    template<dynamic_border_layout_type T>
    struct is_layout<dynamic_border_layout<T>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/dynamic_border_layout.inl>
