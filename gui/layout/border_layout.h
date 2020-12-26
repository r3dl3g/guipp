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

    namespace border {

      // --------------------------------------------------------------------------
      enum class type_t {
        all_symmetric,
        top_bottom_maximize,
        left_right_maximize,
        bottom_max_top_min,
        top_max_bottom_min,
        left_max_right_min,
        right_max_left_min
      };

      // --------------------------------------------------------------------------
      struct left_width {
        core::point::type left;
        core::size::type width;
      };

      struct top_height {
        core::point::type top;
        core::size::type height;
      };

      // --------------------------------------------------------------------------
      template<int TO, int BO, int LE, int RI, type_t T>
      struct border_geometrie {
        static left_width get_top (const core::rectangle&);
        static left_width get_bottom (const core::rectangle&);
        static top_height get_left (const core::rectangle&);
        static top_height get_right (const core::rectangle&);
      };

      // --------------------------------------------------------------------------
      template<int TO = 0, int BO = 0, int LE = 0, int RI = 0, type_t T = type_t::top_bottom_maximize>
      class layouter {
      public:
        typedef border_geometrie<TO, BO, LE, RI, T> geometrie;

        static constexpr int top = TO;
        static constexpr int bottom = BO;
        static constexpr int left = LE;
        static constexpr int right = RI;
        static constexpr type_t type = T;

        explicit layouter (win::container* = nullptr);

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

        int get_top_height () const;
        int get_bottom_height () const;
        int get_left_width () const;
        int get_right_width () const;

        void layout (const core::rectangle& r);

      protected:
        struct {
          layout_function center;
          layout_function top;
          layout_function bottom;
          layout_function left;
          layout_function right;
        } data;

      };

      template<int TO = 0, int BO = 0, int LE = 0, int RI = 0>
      class sym_layouter : public layouter<TO, BO, LE, RI, type_t::all_symmetric> {
      public:
        typedef layouter<TO, BO, LE, RI, type_t::all_symmetric> super;

        explicit sym_layouter (win::container* p = nullptr);

        const layout_function& get_top_left () const;
        const layout_function& get_top_right () const;
        const layout_function& get_bottom_left () const;
        const layout_function& get_bottom_right () const;

        void set_top_left (const layout_function& top_left);
        void set_top_right (const layout_function& top_right);
        void set_bottom_left (const layout_function& bottom_left);
        void set_bottom_right (const layout_function& bottom_right);

        void layout (const core::rectangle& r);

      protected:
        struct {
          layout_function top_left;
          layout_function top_right;
          layout_function bottom_left;
          layout_function bottom_right;
        } data;
      };

    } // namespace border

    template<int TO, int BO, int LE, int RI, border::type_t T>
    struct is_layout<border::layouter<TO, BO, LE, RI, T>> {
      enum {
        value = true
      };
    };

  } // namespace layout

} // namespace gui

#include <gui/layout/border_layout.inl>
