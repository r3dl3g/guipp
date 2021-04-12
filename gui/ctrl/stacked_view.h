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
      view_stack.on_finish_push([&] (const window_ptr& in) {
        set_top_data(in);
        title_view.enable_back(view_stack.count() > 1);
        super::get_layout().set_center(layout::lay(in.get()));
        super::layout();
        super::invalidate();
      });
      view_stack.on_prepare_pop([&] (const window_ptr& out) {
        super::get_layout().set_center(nullptr);
        const win_data& data = view_data[out];
        if (data.right) {
          data.right->remove_from_parent();
        }
        view_data.erase(out);
      });
      view_stack.on_finish_pop([&] (const window_ptr& in) {
        super::get_layout().set_center(layout::lay(in.get()));
        set_top_data(in);
        title_view.enable_back(view_stack.count() > 1);
        super::layout();
        super::invalidate();
      });
      super::get_layout().set_top(layout::lay(title_view));
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::set_top_data (const window_ptr& in) {
      const win_data& data = view_data[in];
      title_view.set_title(data.title);
      if (data.right) {
        title_view.set_right_view(*data.right, data.width);
      } else {
        title_view.clear_right_view();
      }
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::push (const window_ptr& view, const std::string& title) {
      view_data[view] = win_data{title, nullptr, 0};
      view_stack.push(view, animation::push_from<P>);
      title_view.clear_right_view();
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::push (const window_ptr& view, const std::string& title, window& right, core::size::type width) {
      view_data[view] = win_data{title, &right, width};
      view_stack.push(view, animation::push_from<P>);
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    void stacked_view<T, H, P>::pop () {
      view_stack.pop(animation::pop_to<P>);
    }
    // --------------------------------------------------------------------------
    template<typename T, int H, animation::position P>
    auto stacked_view<T, H, P>::get_data (const window_ptr& view) const -> const win_data& {
      static win_data empty{"", 0, 0};
      const auto i = view_data.find(view);
      return (i != view_data.end()) ? i->second : empty;
    }

  } // win

} // gui
