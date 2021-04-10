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
 * @brief     C++ API: window class definition
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/stacked_view_controller.h>


namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    stacked_view_controller::stacked_view_controller (container& main)
      : main(main)
      , animator(main, std::chrono::milliseconds(250))
    {}
    // --------------------------------------------------------------------------
    std::size_t stacked_view_controller::count () const {
      return view_stack.size();
    }
    // --------------------------------------------------------------------------
    const window_ptr& stacked_view_controller::top () const {
      return view_stack.top();
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::push (const window_ptr& in,
                                        animation_type at) {
      if (view_stack.empty()) {
        in->create(in->get_window_class(), main);
        view_stack.push(in);
        if (finish_push) {
          finish_push(in);
        }
        in->set_visible();
      } else {
        window_ptr out = view_stack.top();
        const auto r = out->geometry();
        view_stack.push(in);
        in->create(in->get_window_class(), main, r);
        in->set_visible(false);
        animator.add(at(out, in, r));
        animator.start([&, in, out] () {
          out->set_visible(false);
          if (finish_push) {
            finish_push(in);
          }
          in->set_visible();
        });
      }
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::pop (animation_type at) {
      if (view_stack.size() > 1) {
        window_ptr out(std::move(view_stack.top()));
        view_stack.pop();
        window_ptr in = view_stack.top();
        if (prepare_pop) {
          prepare_pop(in);
        }
        const auto r = out->geometry();
        animator.add(at(out, in, r));
        animator.start([&, out] () {
          out->remove_from_parent();
          if (finish_pop) {
            finish_pop();
          }
        });
      }
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::on_finish_push (finish_push_fn&& fn) {
      finish_push = std::move(fn);
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::on_prepare_pop (prepare_pop_fn&& fn) {
      prepare_pop = std::move(fn);
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::on_finish_pop (finish_pop_fn&& fn) {
      finish_pop = std::move(fn);
    }
    // --------------------------------------------------------------------------
    void stacked_view_controller::disable_animation (bool disable) {
      animator.disable_animation(disable);
    }

  } // win

} // gui
