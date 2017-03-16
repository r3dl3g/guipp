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

  namespace win {

    class scroll_view : public window {
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

      void create (const window& parent,
                   const core::rectangle& place = core::rectangle::default_rectangle) {
        super::create(clazz, parent, place);
        vscroll.create(*this, get_vscroll_area());
        hscroll.create(*this, get_hscroll_area());
        vscroll.set_step(1);
        hscroll.set_step(1);
        edge.create(*this, get_edge_area());
      }

      void move_children (const core::point& delta);

    protected:
      core::rectangle get_vscroll_area () const;
      core::rectangle get_hscroll_area () const;
      core::rectangle get_edge_area () const;

      core::rectangle get_visible_area (bool without_scrolls = false) const;

    private:
      core::point   current_pos;
      vscroll_bar   vscroll;
      hscroll_bar   hscroll;
      client_window edge;

      static window_class clazz;
    };

  } // win

} // gui
