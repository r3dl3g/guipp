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
 * @brief     controller to manage stacked views
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <stack>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/animation.h>
#include <gui/win/container.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    struct GUIPP_WIN_EXPORT stacked_view_controller {
      typedef std::function<void(const window_ptr&)> callback_fn;

      typedef animation::dual<animation::move> (animation_type) (const window_ptr& out,
                                                                 const window_ptr& in,
                                                                 const gui::core::rectangle& r);

      stacked_view_controller (container& main);

      void push (const window_ptr& view,
                 animation_type at);
      void push (const window_ptr& view);
      void pop (animation_type at);
      void pop ();

      const window_ptr& top () const;
      std::size_t count () const;

      void on_finish_push (callback_fn&&);
      void on_prepare_pop (callback_fn&&);
      void on_finish_pop (callback_fn&&);

      void disable_animation (bool disable);

    private:
      container& main;
      std::stack<window_ptr> view_stack;
      animation::engine animator;
      callback_fn finish_push;
      callback_fn prepare_pop;
      callback_fn finish_pop;
    };

    // --------------------------------------------------------------------------
  } // win

} // gui
