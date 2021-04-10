/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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

      class dynamic_border_layout_base {
      public:
        explicit dynamic_border_layout_base (float top_height = 0,
                                             float bottom_height = 0,
                                             float left_width = 0,
                                             float right_width = 0);

        const layout_function& get_center () const;
        const layout_function& get_top () const;
        const layout_function& get_bottom () const;
        const layout_function& get_left () const;
        const layout_function& get_right () const;

        void set_center (const layout_function& center);
        void set_top (const layout_function& top);
        void set_bottom (const layout_function& bottom);
        void set_left (const layout_function& left);
        void set_right (const layout_function& right);

        void set_center_top_bottom_left_right (const layout_function& center,
                                               const layout_function& top,
                                               const layout_function& bottom,
                                               const layout_function& left,
                                               const layout_function& right);

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
          explicit dynamic_border_layout_data (float top_height = 0,
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
    template<border_layout_type type = border_layout_type::top_bottom_maximize>
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

    template<border_layout_type T>
    struct is_layout<dynamic_border_layout<T>> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/layout/dynamic_border_layout.inl>
