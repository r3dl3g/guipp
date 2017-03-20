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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "gui_types.h"

namespace gui {

  namespace win {
    class container;
    class window;
  }

  namespace layout {

    // --------------------------------------------------------------------------
    class standard_layout {
    public:
      standard_layout (win::container*) {}

      void layout (const core::size& new_size) {}
    };

    namespace detail {
      class horizontal_lineup_base {
      public:
        horizontal_lineup_base (win::container* main,
                                unsigned short border,
                                unsigned short gap)
          : main(main)
          , border(border)
          , gap(gap)
        {}

        void layout (const core::size& new_size);

      private:
        const win::container* main;
        const unsigned short border;
        const unsigned short gap;
      };

      class vertical_lineup_base {
      public:
        vertical_lineup_base (win::container* main,
                              unsigned short border,
                              unsigned short gap)
          : main(main)
          , border(border)
          , gap(gap)
        {}

        void layout (const core::size& new_size);

      private:
        const win::container* main;
        const unsigned short border;
        const unsigned short gap;
      };
    }

    template<unsigned short B = 0, unsigned short G = 0>
    class horizontal_lineup : public detail::horizontal_lineup_base {
    public:
      typedef detail::horizontal_lineup_base super;

      horizontal_lineup (win::container* c)
        : super(c, B, G)
      {}

      inline void layout (const core::size& sz) {
        super::layout(sz);
      }
    };

    template<unsigned short B = 0, unsigned short G = 0>
    class vertical_lineup : public detail::vertical_lineup_base {
    public:
      typedef detail::vertical_lineup_base super;

      inline vertical_lineup (win::container* c)
        : super(c, B, G)
      {}

      inline void layout (const core::size& sz) {
        super::layout(sz);
      }
    };

    enum class What : char {
      left,
      right,
      top,
      bottom
    };

    enum class Where : char {
      x,
      x2,
      y,
      y2,
      width,
      height
    };

    class attach {
    public:
      attach (win::container* main)
      {}

      void abs (win::window* target, win::window* source, What what, Where where, short offset) {
        attachments.push_back({target, source, what, where, offset, double_to_short(1.0)});
      }

      void rel (win::window* target, win::window* source, What what, double relative, short offset = 0) {
        attachments.push_back({target, source, what,
                               (what == What::left) || (what == What::right) ? Where::width : Where::height,
                               offset, double_to_short(relative)});
      }

      void layout (const core::size& sz);

    private:
      struct attachment {
        win::window* target;
        win::window* source;
        What what;
        Where where;
        short offset;
        short relative;

        void adjust (core::rectangle& rect,
                     const core::size& sz,
                     const core::rectangle& outer) const;

        core::point::type adjust (const core::size& sz,
                                  const core::rectangle& outer) const;

        inline core::point::type calc (core::point::type a) const {
          return core::point::type ((double)a * short_to_double(relative)) + offset;
        }
      };

      static inline double short_to_double (short offset) {
        return (double)offset / 10000.0;
      }

      static inline short double_to_short (double rel) {
        return int16_t(rel * 10000.0);
      }

      std::vector<attachment> attachments;
    };
  }
}
