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
 * @brief     stack view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/stacked_view_controller.h>
#include <gui/layout/layout_container.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    template<typename T, int H = 50, animation::position P = animation::position::right>
    struct stacked_view : public group_window<layout::border::layouter<H>> {
      typedef group_window<layout::border::layouter<H>> super;
      typedef T title_view_t;

      struct win_data {
        std::string title;
        window* right;
        core::size::type width;
      };

      stacked_view ();

      void push (const window_ptr& view, const std::string& title);
      void push (const window_ptr& view, const std::string& title, window& right, core::size::type width);
      void pop ();

      stacked_view_controller view_stack;
      title_view_t title_view;

      const win_data& get_data (const window_ptr& view) const;

    protected:
      void set_top_data (const window_ptr&);

      std::map<window_ptr, win_data> view_data;
    };

  } // win

} // gui

#include <gui/ctrl/stacked_view.inl>
