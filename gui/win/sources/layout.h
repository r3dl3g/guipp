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

namespace gui {

  namespace core {
    struct size;
  }

  namespace win {
    class container;
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

      inline horizontal_lineup (win::container* c)
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
  }
}
