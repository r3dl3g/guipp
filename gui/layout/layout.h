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
    class widget;
  }

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    class GUIPP_LAYOUT_EXPORT standard_layout {
    public:
      void layout (const core::rectangle&);
    };

    using layout_callback = void(const core::rectangle&);
    using layout_function = std::function<layout_callback>;

    GUIPP_LAYOUT_EXPORT layout_function lay (win::window&);
    GUIPP_LAYOUT_EXPORT layout_function lay (win::window*);
    GUIPP_LAYOUT_EXPORT layout_function lay (win::widget&);
    GUIPP_LAYOUT_EXPORT layout_function lay (win::widget*);

    template <typename T>
    struct is_layout {
      enum {
        value = false
      };
    };

    template<typename T, typename std::enable_if<is_layout<T>::value>::type* = nullptr>
    layout_function lay (T& l) {
      return [&l] (const core::rectangle& r) {
        l.layout(r);
      };
    }

    template<typename T, typename std::enable_if<is_layout<T>::value>::type* = nullptr>
    layout_function lay (T&& l) {
      return [l] (const core::rectangle& r) {
        l.layout(r);
      };
    }

    template<typename T, typename std::enable_if<is_layout<T>::value>::type* = nullptr>
    layout_function lay (T* l) {
      return [l] (const core::rectangle& r) {
        l->layout(r);
      };
    }

    // --------------------------------------------------------------------------
    struct GUIPP_LAYOUT_EXPORT layout_element {
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
    class GUIPP_LAYOUT_EXPORT layout_base {
    public:
      typedef std::vector<layout_element> element_list;

      layout_base () = default;
      layout_base (std::initializer_list<layout_function> list);

      const element_list& get_elements () const;

      void add (const layout_function& e, bool is_separator = false);
      void add (layout_function&& e, bool is_separator = false);

      void add (std::initializer_list<layout_function> list);
      void add (std::initializer_list<win::window*> list);
      void add (std::initializer_list<win::widget*> list);

      void remove_all ();

    protected:
      element_list elements;

    };

    template<>
    struct is_layout<layout_base> {
      enum {
        value = true
      };
    };

    // --------------------------------------------------------------------------
    template<typename L>
    struct attachable_layout {
      using layout_type = L;

      attachable_layout ()
      {}

      attachable_layout (const layout_type& lay)
        : lay(lay)
      {}

      attachable_layout (layout_type&& lay)
        : lay(std::move(lay))
      {}

      operator layout_type& () {
        return lay;
      }

      layout_type* operator-> () {
        return &lay;
      }

      void layout (const core::rectangle& r) const {
        place = r;
        lay.layout(r);
      }

      layout_type lay;
      mutable core::rectangle place;
    };

    template<typename L>
    struct is_layout<attachable_layout<L>> {
      enum {
        value = true
      };
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation_t O>
      class orientation_layout : public layout_base {
      public:
        typedef layout_base super;
        typedef core::size::type type;

        static constexpr orientation_t orientation = O;

        orientation_layout () = default;
        orientation_layout (std::initializer_list<layout_function> list);

        static type get_dimension1 (const core::point&);
        static type get_dimension2 (const core::point&);
        static core::size make_size (type dim1, type dim2);
        static core::rectangle get_sep_area (const core::rectangle& area, type s);
        static void move_area (core::rectangle& area, type offs);

        std::size_t separator_count () const;

      };

      // --------------------------------------------------------------------------
      template<orientation_t H, origin_t R>
      class origin_layout : public orientation_layout<H> {
      public:
        typedef orientation_layout<H> super;
        typedef core::size::type type;

        static constexpr origin_t origin = R;

        origin_layout () = default;
        origin_layout (std::initializer_list<layout_function> list);

        static core::rectangle init_area (type border, type dim1, type dim2,
                                          const core::size&, const core::size&,
                                          int gap, std::size_t count,
                                          int sep, std::size_t sep_count);
        static void move_area (core::rectangle& area, type offs);
        static core::rectangle get_sep_area (const core::rectangle& area, type s);
      };

    } // namespace detail

  } // namespace layout

} // namespace gui

#include <gui/layout/layout.inl>
