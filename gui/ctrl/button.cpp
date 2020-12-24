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
 * @brief     C++ API: button
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <math.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/ctrl/button.h>
#include <gui/look/button.h>


namespace gui {

  namespace ctrl {

#ifdef GUIPP_QT
    bool get_button_state (const core::event& e) {
      return e.cast<win::QClientEvent>().l1() != 0;
    }
#endif // GUIPP_QT


    // --------------------------------------------------------------------------
    button_base::button_base () {
    }

    button_base::button_base (const button_base& rhs)
      : super(rhs)
    {}

    button_base::button_base (button_base&& rhs)
      : super(std::move(rhs))
    {}

    void button_base::set_hilited (bool on) {
      if (set_state().hilited(on)) {
        notify_event(detail::HILITE_CHANGE_MESSAGE, on);
        invalidate();
      }
    }

    void button_base::set_pushed (bool onp) {
      if (set_state().pushed(onp)) {
        notify_event(onp ? detail::BN_PUSHED_MESSAGE : detail::BN_UNPUSHED_MESSAGE);
        invalidate();
      }
    }

    void button_base::set_checked (bool onf) {
      if (set_state().checked(onf)) {
        notify_event(detail::BN_STATE_MESSAGE, onf ? 1 : 0);
        invalidate();
      }
    }

    void button_base::on_clicked (button_clicked_event::function&& f) {
      on<button_clicked_event>(std::move(f));
    }

    void button_base::on_pushed (button_pushed_event::function&& f) {
      on<button_pushed_event>(std::move(f));
    }

    void button_base::on_released (button_released_event::function&& f) {
      on<button_released_event>(std::move(f));
    }

    void button_base::on_state_changed (button_state_event::function&& f) {
      on<button_state_event>(std::move(f));
    }

    // --------------------------------------------------------------------------
    void push_button_traits::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.client_area().is_inside(pos)) {
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<false>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.client_area().is_inside(pos)) {
            btn.set_checked(!btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.set_checked(!btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked() && btn.client_area().is_inside(pos)) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked()) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

    // --------------------------------------------------------------------------
    basic_animated_button_traits::basic_animated_button_traits ()
      : animation_step(1.0F)
    {}

    basic_animated_button_traits::~basic_animated_button_traits () {
      if (animation_thread.joinable()) {
        animation_thread.join();
      }
    }

    void basic_animated_button_traits::prepare_animation () {
      if (animation_thread.joinable()) {
        animation_thread.join();
      }
      animation_step = 0.0F;
    }

    void basic_animated_button_traits::start_animation (button_base& btn) {
      animation_thread = std::thread([&] () {
        while (animation_step < 1.0F) {
          animation_step += 0.1F;
          win::run_on_main([&] () {
            btn.redraw();
          });
          std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
      });
    }

    void basic_animated_button_traits::set_checked (button_base& btn, bool f) {
      prepare_animation();
      start_animation(btn);
      btn.set_checked(f);
    }

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<false>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.client_area().is_inside(pos)) {
            btn.set_checked(!btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.set_checked(!btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<true>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked() && btn.client_area().is_inside(pos)) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked()) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

  } // ctrl

} // gui
