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
#include <gui/ctrl/scroll_bar.h>
#include <gui/win/layout_container.h>


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    class GUIPP_EXPORT scroll_view_base : protected layout_base {
    public:
      typedef layout_base super;

      scroll_view_base (win::container* main);

      void init (win::vertical_scroll_bar* vscroll,
                 win::horizontal_scroll_bar* hscroll,
                 win::window* edge);

      core::rectangle layout (const core::size& new_size, const core::rectangle& required);

      static core::rectangle get_vscroll_area (const core::size&, bool hscroll_bar_enabled);
      static core::rectangle get_hscroll_area (const core::size&, bool vscroll_bar_enabled);
      static core::rectangle get_edge_area (const core::size&);
      static core::rectangle get_client_area (const core::size&);

    protected:
      void init (std::function<size_callback> f1) {
        super::init(f1);
      }

      win::vertical_scroll_bar*   vscroll;
      win::horizontal_scroll_bar* hscroll;
      win::window*                edge;
    };

    // --------------------------------------------------------------------------
    class GUIPP_EXPORT scroll_view : public scroll_view_base {
    public:
      typedef scroll_view_base super;

      scroll_view (win::container* main);

      void layout (const core::size& new_size);

      void set_in_scroll_event (bool);
      bool is_in_scroll_event () const;

    private:
      const win::move_event me;
      const win::size_event se;

      bool in_scroll_event;

      void handle_child_move (const core::point&);
      void handle_child_size (const core::size&);
    };

    // --------------------------------------------------------------------------
  } // namespace layout

  namespace win {

    // --------------------------------------------------------------------------
    class GUIPP_EXPORT scroll_view : public layout_container<layout::scroll_view> {
    public:
      typedef layout_container<layout::scroll_view> super;
      typedef window_class<scroll_view, IF_WIN32_ELSE((os::color)(COLOR_WINDOW + 1), color::white)> clazz;

      scroll_view ();

      void set_scroll_pos (const core::point& pt);
      core::point get_scroll_pos () const;

      void enable_vscroll_bar (bool enable);
      void enable_hscroll_bar (bool enable);
      bool is_vscroll_bar_enabled () const;
      bool is_hscroll_bar_enabled () const;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void move_children (const core::size& delta);

    protected:
      vertical_scroll_bar& get_vscroll ();
      horizontal_scroll_bar& get_hscroll ();
      window& get_edge ();

    private:
      core::point           current_pos;
      vertical_scroll_bar   vscroll;
      horizontal_scroll_bar hscroll;
      client_window<>       edge;

    };

  } // namespace win

} // gui

#include <gui/ctrl/scroll_view.inl>
