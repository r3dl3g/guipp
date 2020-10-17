/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: scrolled views
 *
 * @file
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
                 win::widget* edge,
                 view_type* client);

      void layout (const core::rectangle& new_size) const;

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
      typedef no_erase_window_class<virtual_view> clazz;
      typedef T view_type;

      virtual_view ();

      void create (win::container& parent,
                   const core::rectangle& r = core::rectangle::def);

      view_type             view;
      horizontal_scroll_bar hscroll;
      vertical_scroll_bar   vscroll;
      client_control<>      edge;

    private:
      void handle_create (win::window*, const core::rectangle& r);

    };

  } // ctrl

} // gui

#include <gui/ctrl/virtual_view.inl>
