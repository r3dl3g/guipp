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
 * @brief     C++ API: separator view
 *
 * @file
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
    struct stacked_view_controller {
      typedef std::function<void(const window_ptr&, const std::string&)> finish_push_fn;
      typedef std::function<void(const window_ptr&)> prepare_pop_fn;
      typedef std::function<void()> finish_pop_fn;

      typedef animation::dual<animation::move> (animation_type) (const window_ptr& out,
                                                                 const window_ptr& in,
                                                                 const gui::core::rectangle& r);

      struct stack_entry {
        window_ptr view;
        std::string title;
      };

      stacked_view_controller (container& main);

      void push (const window_ptr& view,
                 const std::string& title,
                 animation_type at = animation::push_from<animation::position::right>);
      void pop (animation_type at = animation::pop_to<animation::position::right>);

      const stack_entry& top () const;
      std::size_t count () const;

      void on_finish_push (finish_push_fn&&);
      void on_prepare_pop (prepare_pop_fn&&);
      void on_finish_pop (finish_pop_fn&&);

      void disable_animation (bool disable);

    private:
      container& main;
      std::stack<stack_entry> view_stack;
      animation::engine animator;
      finish_push_fn finish_push;
      prepare_pop_fn prepare_pop;
      finish_pop_fn finish_pop;
    };

    // --------------------------------------------------------------------------
  } // win

} // gui
