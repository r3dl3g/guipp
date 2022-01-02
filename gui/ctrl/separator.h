/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     pasive separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/control.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      class GUIPP_CTRL_EXPORT separator_base : public control {
      public:
        typedef control super;
        typedef win::no_erase_window_class<separator_base> clazz;

        separator_base () = default;

        void create (win::container& parent,
                     const core::rectangle& place = core::rectangle::def);

      };

      // --------------------------------------------------------------------------
      template<orientation_t Horizontal>
      struct line_traits {
        static draw::line first (const core::rectangle&);
        static draw::line second (const core::rectangle&);
        static core::rectangle area (const core::rectangle&);
      };

      // --------------------------------------------------------------------------
      template<bool Sunken>
      struct color_traits {
        static os::color first (os::color);
        static os::color second (os::color);
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<orientation_t O, bool Sunken = true, os::color background = color::very_very_light_gray>
    class basic_separator : public detail::separator_base {
    public:
      typedef detail::line_traits<O> lt;
      typedef detail::color_traits<Sunken> ct;

      basic_separator ();

    private:
      void paint (draw::graphics& graph);

    };

    // --------------------------------------------------------------------------
    using horizontal_separator = basic_separator<orientation_t::horizontal>;
    using vertical_separator = basic_separator<orientation_t::vertical>;

  } // ctrl

} // gui

#include <gui/ctrl/separator.inl>
