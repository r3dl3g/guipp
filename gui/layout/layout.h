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
#include <gui++-layout-export.h>


namespace gui {

  namespace win {
    class window;
  }

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      void layout (const core::rectangle&);
    };

    using layout_callback = void(const core::rectangle&);
    using layout_function = std::function<layout_callback>;

    layout_function win (win::window& w);
    layout_function win (win::window* w);

    template<typename T>
    layout_function lay (T& l) {
      return [&l] (const core::rectangle& r) {
        l.layout(r);
      };
    }

    template<typename T>
    layout_function lay (T&& l) {
      return [l] (const core::rectangle& r) {
        l.layout(r);
      };
    }

    // --------------------------------------------------------------------------
    struct layout_element {
      layout_element (const layout_function& fkt, bool is_separator = false)
        : fkt(fkt)
        , separator(is_separator)
      {}

      layout_element (layout_function&& fkt, bool is_separator = false)
        : fkt(std::move(fkt))
        , separator(is_separator)
      {}

      void operator() (const core::rectangle& r) const {
        fkt(r);
      }

      operator bool () const {
        return (bool)fkt;
      }

      bool is_separator () const {
        return separator;
      }

    private:
      layout_function fkt;
      bool separator;
    };

    // --------------------------------------------------------------------------
    class layout_base {
    public:
      typedef std::vector<layout_element> element_list;

      layout_base () = default;
      layout_base (std::initializer_list<layout_function> list);

      const element_list& get_elements () const;

      void add (const layout_function& e, bool is_separator = false);
      void add (layout_function&& e, bool is_separator = false);
      void add (std::initializer_list<layout_function> list);
      void add (std::initializer_list<win::window*> list);

    protected:
      element_list elements;

    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation>
      class orientation_layout : public layout_base {
      public:
        typedef layout_base super;
        typedef core::size::type type;

        orientation_layout () = default;
        orientation_layout (std::initializer_list<layout_function> list);

        type get_dimension1 (const core::point&) const;
        type get_dimension2 (const core::point&) const;
        core::size make_size (type dim1, type dim2) const;
        core::rectangle get_sep_area (const core::rectangle& area, type s) const;
        void move_area (core::rectangle& area, type offs) const;

        std::size_t separator_count () const;

      };

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      class origin_layout : public orientation_layout<H> {
      public:
        typedef orientation_layout<H> super;
        typedef core::size::type type;

        origin_layout () = default;
        origin_layout (std::initializer_list<layout_function> list);

        core::rectangle init_area (type border, type dim1, type dim2,
                                   const core::size&, const core::size&,
                                   int gap, std::size_t count,
                                   int sep, std::size_t sep_count) const;
        void move_area (core::rectangle& area, type offs) const;
        core::rectangle get_sep_area (const core::rectangle& area, type s) const;
      };

    } // namespace detail

  } // namespace layout

} // namespace gui

#include <gui/layout/layout.inl>
