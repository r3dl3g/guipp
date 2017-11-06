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

    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      class lineup_base : public orientation_layout<H> {
      public:
        typedef orientation_layout<H> super;
        typedef core::size::type type;

        lineup_base (win::container* m);
        lineup_base (win::container* m, const lineup_base& rhs);
        lineup_base (win::container* m, lineup_base&& rhs);

        core::rectangle init_area (type border, const core::size&, const core::size&, int gap, std::size_t count);
        void move_area (core::rectangle& area, type offs);
      };

      // --------------------------------------------------------------------------
      template<orientation H>
      class lineup_base<H, origin::start> : public orientation_layout<H> {
      public:
        typedef orientation_layout<H> super;
        typedef core::size::type type;

        lineup_base (win::container* m);
        lineup_base (win::container* m, const lineup_base& rhs);
        lineup_base (win::container* m, lineup_base&& rhs);

        core::rectangle init_area (type border, const core::size& is, const core::size& sz, int, std::size_t);
        void move_area (core::rectangle& area, type offs);
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation H,
             unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    class lineup_layout : public detail::lineup_base<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::lineup_base<H, R> super;

      lineup_layout (win::container* m);
      lineup_layout (win::container* m, const lineup_layout& rhs);
      lineup_layout (win::container* m, lineup_layout&& rhs);

      void layout (const core::size& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    template<unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    using horizontal_lineup = lineup_layout<orientation::horizontal, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             origin R = origin::start>
    using vertical_lineup = lineup_layout<orientation::vertical, dim1, border, gap, sep, R>;

    // --------------------------------------------------------------------------
    template<alignment a,
             unsigned dim1,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2>
    using lineup = lineup_layout<alignment_orientation<a>::value, dim1, border, gap, sep, alignment_origin<a>::value>;

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    // --------------------------------------------------------------------------
    namespace detail {

      // --------------------------------------------------------------------------
      template<orientation H, origin R>
      inline lineup_base<H, R>::lineup_base (win::container* m)
        : super(m)
      {}

      template<orientation H, origin R>
      inline lineup_base<H, R>::lineup_base (win::container* m, const lineup_base& rhs)
        : super(m, rhs)
      {}

      template<orientation H, origin R>
      inline lineup_base<H, R>::lineup_base (win::container* m, lineup_base&& rhs)
        : super(m, std::move(rhs))
      {}

      // --------------------------------------------------------------------------
      template<orientation H>
      inline lineup_base<H, origin::start>::lineup_base (win::container* m)
        : super(m)
      {}

      template<orientation H>
      inline lineup_base<H, origin::start>::lineup_base (win::container* m, const lineup_base& rhs)
        : super(m, rhs)
      {}

      template<orientation H>
      inline lineup_base<H, origin::start>::lineup_base (win::container* m, lineup_base&& rhs)
        : super(m, std::move(rhs))
      {}

      template<orientation H>
      inline core::rectangle lineup_base<H, origin::start>::init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
        return core::rectangle(core::point(border, border), is);
      }

      template<orientation H>
      inline void lineup_base<H, origin::start>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle lineup_base<orientation::horizontal, origin::end>::init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
        return core::rectangle(core::point(sz.width() - is.width() - border, border), is);
      }

      template<>
      inline void lineup_base<orientation::horizontal, origin::end>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, -offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle lineup_base<orientation::vertical, origin::end>::init_area (type border, const core::size& is, const core::size& sz, int, std::size_t) {
        return core::rectangle(core::point(border, sz.height() - is.height() - border), is);
      }

      template<>
      inline void lineup_base<orientation::vertical, origin::end>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, -offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle lineup_base<orientation::horizontal, origin::center>::init_area (type border, const core::size& is, const core::size& sz, int gap, std::size_t count) {
        return core::rectangle(core::point((sz.width() - (is.width() * count + (count - 1) * gap)) / 2, border), is);
      }

      template<>
      inline void lineup_base<orientation::horizontal, origin::center>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

      // --------------------------------------------------------------------------
      template<>
      inline core::rectangle lineup_base<orientation::vertical, origin::center>::init_area (type border, const core::size& is, const core::size& sz, int gap, std::size_t count) {
        return core::rectangle(core::point(border, (sz.height() - (is.height() * count + (count - 1) * gap)) / 2), is);
      }

      template<>
      inline void lineup_base<orientation::vertical, origin::center>::move_area (core::rectangle& area, type offs) {
        super::move_area(area, offs);
      }

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m)
      : super(m) {
      init();
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m, const lineup_layout& rhs)
      : super(m, rhs) {
      init();
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline lineup_layout<H, D, B, G, S, R>::lineup_layout (win::container* m, lineup_layout&& rhs)
      : super(m, std::move(rhs)) {
      init();
    }

    template<orientation height, unsigned dim1, unsigned border, unsigned gap, unsigned sep, origin rows>
    void lineup_layout<height, dim1, border, gap, sep, rows>::layout (const core::size& sz) {
      std::vector<win::window*> children = super::get_children();
      const type border2 = (border * 2);
      const type space = super::get_dimension1(sz) - border2;

      auto count = children.size();
      if (count) {
        if (space > 0) {
          const type dim2 = super::get_dimension2(sz) - border2;
          const type offset = dim1 + gap;
          const type sep_offset = gap + sep;

          core::rectangle area = super::init_area(border, super::make_size(dim1, dim2), sz, gap, count);
          for (win::window* win : children) {
            if (super::is_separator(win)) {
              super::place_child(win, super::get_sep_area(area, sep));
              super::move_area(area, sep_offset);
            } else {
              super::place_child(win, area);
              super::move_area(area, offset);
            }
          }
        } else {
          super::hide_children(children);
        }
      }
      super::update();
    }

    template<orientation H, unsigned D, unsigned B, unsigned G, unsigned S, origin R>
    inline void lineup_layout<H, D, B, G, S, R>::init () {
      super::init(core::bind_method(this, &lineup_layout::layout), [&] () {
        layout(super::get_main_size());
      });
    }

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
