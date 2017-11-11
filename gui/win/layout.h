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
#include <vector>
#include <functional>
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/gui_types.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {
    class container;
    class window;
  }

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      standard_layout (win::container*);
    };

    // --------------------------------------------------------------------------
    class layout_base {
    public:
      typedef void (size_callback) (const core::size& sz);
      typedef void (show_callback) ();

      layout_base (win::container* m);
      layout_base (win::container* m, const layout_base&);
      layout_base (win::container* m, layout_base&&);

      layout_base (const layout_base&) = delete;
      layout_base (layout_base&&) = delete;

      void init (std::function<size_callback> f1);
      void init (std::function<size_callback> f1, std::function<show_callback> f2);

      std::vector<win::window*> get_children ();
      static void place_child (win::window*, const core::rectangle&);
      static bool is_child_visible (win::window*);
      static void hide_children (std::vector<win::window*>&);

      void update ();
      core::size get_main_size () const;

    protected:
      win::container* main;

    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation>
      class orientation_layout : protected layout_base {
      public:
        typedef layout_base super;
        typedef core::size::type type;

        orientation_layout (win::container* m);
        orientation_layout (win::container* m, const orientation_layout& rhs);
        orientation_layout (win::container* m, orientation_layout&& rhs);

        type get_dimension1 (const core::size& sz);
        type get_dimension2 (const core::size& sz);
        core::size make_size (type dim1, type dim2);
        core::rectangle get_sep_area (const core::rectangle& area, type s);
        void move_area (core::rectangle& area, type offs);

        void add_separator (const win::window* w);

        bool is_separator (const win::window* w) const;

        std::size_t separator_count () const;

      protected:
        typedef std::vector<const win::window*> separator_list;
        separator_list separators;

      };

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      class origin_layout : public orientation_layout<H> {
      public:
        typedef orientation_layout<H> super;
        typedef core::size::type type;

        origin_layout (win::container* m);
        origin_layout (win::container* m, const origin_layout& rhs);
        origin_layout (win::container* m, origin_layout&& rhs);

        core::rectangle init_area (type border, const core::size&, const core::size&,
                                   int gap, std::size_t count, int sep, std::size_t sep_count);
        void move_area (core::rectangle& area, type offs);
        core::rectangle get_sep_area (const core::rectangle& area, type s);
      };

    } // namespace detail

  } // namespace layout

} // namespace gui

#include <gui/win/layout.inl>
