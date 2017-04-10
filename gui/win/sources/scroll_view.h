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
* @brief     C++ API: basic controls
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
#include "scroll_bar.h"


namespace gui {

  namespace layout {

    // --------------------------------------------------------------------------
    class scroll_view {
    public:
      scroll_view ();

      void init (win::container* main,
                 win::vscroll_bar* vscroll,
                 win::hscroll_bar* hscroll,
                 win::client_window* edge);

      void layout (const core::size& new_size, win::container* main);

      void set_current_pos (const core::point& pt);
      core::point get_current_pos () const;

      static core::rectangle get_vscroll_area (const core::size&, bool hscroll_bar_enabled);
      static core::rectangle get_hscroll_area (const core::size&, bool vscroll_bar_enabled);
      static core::rectangle get_edge_area (const core::size&);
      static core::rectangle get_visible_area (const core::size&);

    private:

      win::move_event me;
      win::size_event se;

      void handle_child_move (const core::point&);
      void handle_child_size (const core::size&);

      core::point         current_pos;
      win::container*     main;
      win::vscroll_bar*   vscroll;
      win::hscroll_bar*   hscroll;
      win::client_window* edge;
    };
    // --------------------------------------------------------------------------

  }

  namespace win {

    // --------------------------------------------------------------------------
    class scroll_view : public layout_container<layout::scroll_view> {
    public:
      typedef window super;

      scroll_view ();

      void calc_area ();

      void set_scroll_pos (const core::point& pt);
      core::point get_scroll_pos () const;

      void enable_vscroll_bar (bool enable);
      void enable_hscroll_bar (bool enable);
      bool is_vscroll_bar_enabled () const;
      bool is_hscroll_bar_enabled () const;

      void create (const container& parent,
                   const core::rectangle& place = core::rectangle::def);

      void move_children (const core::point& delta);

    protected:
      vscroll_bar& get_vscroll ();
      hscroll_bar& get_hscroll ();
      client_window& get_edge ();

    private:
      vscroll_bar   vscroll;
      hscroll_bar   hscroll;
      client_window edge;

      static window_class clazz;
    };
    // --------------------------------------------------------------------------

  } // win

} // gui
