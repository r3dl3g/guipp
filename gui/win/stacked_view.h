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
// Library includes
//
#include <gui/win/stacked_view_controller.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    template<typename T, int H = 50, animation::position P = animation::position::right>
    struct stacked_view : public group_window<layout::border::layouter<H>> {
      typedef group_window<layout::border::layouter<H>> super;
      typedef T title_view_t;
      typedef stacked_view_controller::stack_entry entry;

      stacked_view ();

      void push (const window_ptr& view, const std::string& title);
      void pop ();

      stacked_view_controller view_stack;

    protected:
      title_view_t title_view;
    };

    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    stacked_view<T, H, P>::stacked_view ()
      : view_stack(*this)
    {
      super::on_create([&] () {
        title_view.create(*this);
      });
      title_view.on_back([&] () {
        pop();
      });
      view_stack.on_finish_push([&] (const window_ptr& view, const std::string& t) {
        title_view.set_title(t);
        title_view.enable_back(view_stack.count() > 1);
        super::get_layout().set_center(layout::lay(view.get()));
        super::layout();
        super::invalidate();
      });
      view_stack.on_prepare_pop([&] (const window_ptr& view) {
        super::get_layout().set_center(layout::lay(view.get()));
      });
      view_stack.on_finish_pop([&] () {
        title_view.set_title(view_stack.top().title);
        title_view.enable_back(view_stack.count() > 1);
        super::layout();
        super::invalidate();
      });
      super::get_layout().set_top(layout::lay(title_view));
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::push (const window_ptr& view, const std::string& title) {
      view_stack.push(view, title, animation::push_from<P>);
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::pop () {
      view_stack.pop(animation::pop_to<P>);
    }

  } // win

} // gui
