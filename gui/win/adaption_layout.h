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
#include <gui/core/bind_method.h>

namespace gui {

  // --------------------------------------------------------------------------
  namespace layout {

    // --------------------------------------------------------------------------
    template<orientation H,
             unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = -1,
             origin R = origin::start>
    class adaption_layout : public detail::origin_layout<H, R> {
    public:
      typedef core::size::type type;
      typedef detail::origin_layout<H, R> super;

      adaption_layout (win::container* m);
      adaption_layout (win::container* m, const adaption_layout& rhs);
      adaption_layout (win::container* m, adaption_layout&& rhs);

      void layout (const core::size& sz);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = -1,
             origin o = origin::start>
    using horizontal_adaption = adaption_layout<orientation::horizontal, border, gap, sep, min, max, o>;

    // --------------------------------------------------------------------------
    template<unsigned border = 0,
             unsigned gap = 0,
             unsigned sep = 2,
             unsigned min = 0,
             unsigned max = -1,
             origin o = origin::start>
    using vertical_adaption = adaption_layout<orientation::vertical, border, gap, sep, min, max, o>;

    // --------------------------------------------------------------------------
    //
    // inlines
    //
    template<orientation H, unsigned B, unsigned G, unsigned S, unsigned I, unsigned A, origin O>
    inline adaption_layout<H, B, G, S, I, A, O>::adaption_layout (win::container* m)
      : super(m) {
      init();
    }

    template<orientation H, unsigned B, unsigned G, unsigned S, unsigned I, unsigned A, origin O>
    inline adaption_layout<H, B, G, S, I, A, O>::adaption_layout (win::container* m, const adaption_layout& rhs)
      : super(m, rhs) {
      init();
    }

    template<orientation H, unsigned B, unsigned G, unsigned S, unsigned I, unsigned A, origin O>
    inline adaption_layout<H, B, G, S, I, A, O>::adaption_layout (win::container* m, adaption_layout&& rhs)
      : super(m, std::move(rhs)) {
      init();
    }

    template<orientation H, unsigned B, unsigned G, unsigned S, unsigned I, unsigned A, origin O>
    inline void adaption_layout<H, B, G, S, I, A, O>::init () {
      super::init(core::bind_method(this, &adaption_layout::layout), [&] () {
        layout(super::get_main_size());
      });
    }

    template<orientation height, unsigned border, unsigned gap, unsigned sep, unsigned min, unsigned max, origin o>
    void adaption_layout<height, border, gap, sep, min, max, o>::layout (const core::size& sz) {
      std::vector<win::window*> children = super::get_children();
      const std::size_t count = children.size();
      const std::size_t sep_count = super::separator_count();
      const type border2 = (border * 2);
      const type space = super::get_dimension1(sz) - border2 - sep_count * sep;

      if (count) {
        const type dim1 = std::max<type>(type(min), std::min<type>(type(max), (space - (gap * (count - 1))) / (count - sep_count)));
        if (dim1 >= 1) {
          const type dim2 = super::get_dimension2(sz) - border2;
          const type offset = dim1 + gap;
          const type sep_offset = gap + sep;

          core::rectangle area = super::init_area(border, super::make_size(dim1, dim2), sz,
                                                  gap, count, sep, sep_count);
          for (win::window* win : children) {
            if (super::is_separator(win)) {
              super::place_child(win, super::get_sep_area(area, type(sep)));
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

    // --------------------------------------------------------------------------
  } // namespace layout

} // namespace gui
