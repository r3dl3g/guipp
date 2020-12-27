/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
#include <gui/layout/layout_container.h>
#include <gui/ctrl/scroll_bar.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT scroll_view_base {
    public:
      typedef layout_base super;

      scroll_view_base ();
      scroll_view_base (const scroll_view_base&) = default;
      scroll_view_base (scroll_view_base&&) = default;

      void init (ctrl::vertical_scroll_bar* vscroll,
                 ctrl::horizontal_scroll_bar* hscroll,
                 win::window* edge);

      core::rectangle layout (const core::rectangle& new_size, const core::rectangle& required) const;

      static core::rectangle get_vscroll_area (const core::rectangle&, bool hscroll_bar_enabled);
      static core::rectangle get_hscroll_area (const core::rectangle&, bool vscroll_bar_enabled);
      static core::rectangle get_edge_area (const core::rectangle&);
      static core::rectangle get_client_area (const core::rectangle&);

    protected:
      ctrl::vertical_scroll_bar*   vscroll;
      ctrl::horizontal_scroll_bar* hscroll;
      win::window*                 edge;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT scroll_view : public scroll_view_base {
    public:
      typedef scroll_view_base super;

      scroll_view ();
      scroll_view (const scroll_view&);
      scroll_view (scroll_view&&) noexcept ;

      void layout (const core::rectangle& new_size) const;

      void set_in_scroll_event (bool);
      bool is_in_scroll_event () const;

      void set_main (win::container* main);

    private:
      const win::move_event::function me;
      const win::size_event::function se;

      win::container* main;
      bool in_scroll_event;

      void handle_child_move (const core::point&) const;
      void handle_child_size (const core::size&) const;
    };

    // --------------------------------------------------------------------------
  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT scroll_view_base : public win::layout_container<layout::scroll_view> {
    public:
      typedef win::layout_container<layout::scroll_view> super;

      scroll_view_base ();

      void set_scroll_pos (const core::point& pt);
      core::point get_scroll_pos () const;

      void enable_vscroll_bar (bool enable);
      void enable_hscroll_bar (bool enable);
      bool is_vscroll_bar_enabled () const;
      bool is_hscroll_bar_enabled () const;

      void move_children (const core::size& delta);

    protected:
      void create_children (const core::rectangle& place);

      vertical_scroll_bar& get_vscroll ();
      horizontal_scroll_bar& get_hscroll ();
      window& get_edge ();

    private:
      core::point           current_pos;
      vertical_scroll_bar   vscroll;
      horizontal_scroll_bar hscroll;
      client_control<>      edge;

    };

    // --------------------------------------------------------------------------
    template<os::color background = IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)>
    class scroll_view : public scroll_view_base {
    public:
      typedef scroll_view_base super;
      typedef win::window_class<scroll_view, background> clazz;

      void create (win::container& parent,
                   const core::rectangle& place = core::rectangle::def);

    };

  } // namespace ctrl

} // gui

#include <gui/ctrl/scroll_view.inl>
