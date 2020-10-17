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
 * @brief     C++ API: basic window
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/container.h>
#include <gui/layout/layout.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename B, typename L = layout::standard_layout, typename ... Args>
      class layout_container_base : public B {
      public:
        typedef B super;
        typedef L layout_type;

        layout_container_base (const Args& ... args);
        layout_container_base (const layout_container_base& rhs);
        layout_container_base (layout_container_base&& rhs);

        void layout ();
        void layout (const core::rectangle&);

        void resize (const core::size&, bool repaint = true);
        void place (const core::rectangle&, bool repaint = true);

        layout_type& get_layout ();
        const layout_type& get_layout () const;

      private:
        void init ();
        layout_type layouter;
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_container = detail::layout_container_base<container, L, Args ...>;

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white, typename ... Args>
    class   group_window : public layout_container<L, Args ...> {
    public:
      typedef layout_container<L, Args ...> super;
      typedef window_class<group_window, background> clazz;

      group_window (const Args& ... args);

      void create (container& parent,
                   const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_main_window = detail::layout_container_base<main_window, L, Args ...>;

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_popup_window = detail::layout_container_base<popup_window, L, Args ...>;

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename ... Args>
    using layout_dialog_window = detail::layout_container_base<dialog_window, L, Args ...>;

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#include <gui/layout/layout_container.inl>
