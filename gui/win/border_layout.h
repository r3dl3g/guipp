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
#include <gui/win/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    enum class border_layout_type {
      all_symmetric,
      top_bottom_maximize,
      left_right_maximize,
      top_left_maximize,
      bottom_right_maximize
    };

    // --------------------------------------------------------------------------
    namespace detail {

      class border_layout_base : protected layout_base {
      public:
        typedef layout_base super;

        border_layout_base (win::container* m,
                            float top_height = 0,
                            float bottom_height = 0,
                            float left_width = 0,
                           float right_width = 0);
        border_layout_base (win::container* m, const border_layout_base& rhs);
        border_layout_base (win::container* m, border_layout_base&& rhs);

        win::window* get_center () const;
        win::window* get_top () const;
        win::window* get_bottom () const;
        win::window* get_left () const;
        win::window* get_right () const;

        void set_center (win::window* center);
        void set_top (win::window* top);
        void set_bottom (win::window* bottom);
        void set_left (win::window* left);
        void set_right (win::window* right);

        void set_center_top_bottom_left_right (win::window* center,
                                               win::window* top,
                                               win::window* bottom,
                                               win::window* left,
                                               win::window* right);

        float get_top_height () const;
        float get_bottom_height () const;
        float get_left_width () const;
        float get_right_width () const;

        void set_top_height (float v);
        void set_bottom_height (float v);
        void set_left_width (float v);
        void set_right_width (float v);

      protected:
        struct border_layout_data {
          border_layout_data (float top_height = 0,
                              float bottom_height = 0,
                              float left_width = 0,
                              float right_width = 0);

          win::window* center;
          win::window* top;
          win::window* bottom;
          win::window* left;
          win::window* right;

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
      struct border_layout_geometrie {
        typedef std::pair<core::point::type, core::point::type> points;
        static points get_top_position (const core::rectangle&, const core::size&);
        static points get_bottom_position (const core::rectangle&, const core::size&);
        static points get_left_position (const core::rectangle&, const core::size&);
        static points get_right_position (const core::rectangle&, const core::size&);
      };

      // --------------------------------------------------------------------------
    } // namespace detail

    // --------------------------------------------------------------------------
    template<border_layout_type type = border_layout_type::top_bottom_maximize>
    class border_layout : public detail::border_layout_base {
    public:
      typedef detail::border_layout_base super;
      typedef detail::border_layout_geometrie<type> geometrie;

      border_layout (win::container* m,
                     float top_height,
                     float bottom_height,
                     float left_width,
                     float right_width);
      border_layout (win::container* m, const border_layout& rhs);
      border_layout (win::container* m, border_layout&& rhs);

      void layout (const core::size& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui

#include <gui/win/border_layout.inl>
