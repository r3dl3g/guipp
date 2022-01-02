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
 * @brief     capsule control for scrollable view controls
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/scroll_view.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    template<typename T>
    class virtual_layout : public scroll_view_base {
    public:
      typedef scroll_view_base super;
      typedef T view_type;

      virtual_layout ();

      void init (ctrl::vertical_scroll_bar* vscroll,
                 ctrl::horizontal_scroll_bar* hscroll,
                 win::window* edge,
                 view_type* client);

      void layout (const core::rectangle& r) const;

    private:
      view_type* client;

    };

    // --------------------------------------------------------------------------
  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<typename T, os::color background = color::transparent>
    class virtual_view : public win::layout_container<layout::virtual_layout<T> > {
    public:
      typedef layout::virtual_layout<T> layout_type;
      typedef win::layout_container<layout_type> super;
      typedef win::no_erase_window_class<virtual_view> clazz;
      typedef T view_type;

      virtual_view ();

      template<typename ... Args>
      virtual_view (const Args& ... args);

      void create (win::container& parent,
                   const core::rectangle& r = core::rectangle::def);

      view_type* operator-> ();
      const view_type* operator-> () const;

      view_type             view;
      horizontal_scroll_bar hscroll;
      vertical_scroll_bar   vscroll;
      client_control        edge;

    private:
      void init ();
      void handle_create ();

    };

  } // ctrl

} // gui

#include <gui/ctrl/virtual_view.inl>
