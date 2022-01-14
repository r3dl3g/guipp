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
 * @brief     various button controls
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/button.h>


namespace gui {

  namespace ctrl {

#ifdef GUIPP_QT
    bool get_button_state (const core::event& e) {
      return e.cast<win::QClientEvent>().l1() != 0;
    }
#endif // GUIPP_QT


    // --------------------------------------------------------------------------
    button_base::button_base (os::color f)
      : foreground(f)
    {}

    button_base::button_base (button_base&& rhs) noexcept
      : super(std::move(rhs))
      , foreground(rhs.foreground)
    {}

    void button_base::set_hilited (bool on) {
      if (set_state().hilited(on)) {
        notify_hilite_changed(on);
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

    void button_base::set_foreground (os::color c) {
      foreground = c;
    }

    os::color button_base::get_foreground () const {
      return foreground;
    }

    // --------------------------------------------------------------------------
    push_button_traits::push_button_traits (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::native_point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.surface_geometry().is_inside(pos)) {
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state s, os::key_symbol k) {
        if ((s == core::state::none) && ((k == core::keys::enter) || (k == core::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    toggle_button_traits<false>::toggle_button_traits (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::native_point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.surface_geometry().is_inside(pos)) {
            btn.set_checked(!btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state s, os::key_symbol k) {
        if ((s == core::state::none) && ((k == core::keys::enter) || (k == core::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.set_checked(!btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    toggle_button_traits<true>::toggle_button_traits (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::native_point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked() && btn.surface_geometry().is_inside(pos)) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state s, os::key_symbol k) {
        if ((s == core::state::none) && ((k == core::keys::enter) || (k == core::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked()) {
            btn.set_checked(true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

    // --------------------------------------------------------------------------
    basic_animated_button_traits::basic_animated_button_traits (button_base& btn)
      : repeater(btn, std::chrono::milliseconds(20), [&] () {
        animation_step += 0.2F;
        btn.invalidate();
        if (animation_step >= 1.0F) {
          repeater.stop();
        }
      })
      , animation_step(1.0F)
    {}

    basic_animated_button_traits::~basic_animated_button_traits () {
      repeater.wait_for_finish ();
    }

    void basic_animated_button_traits::start_animation () {
      repeater.stop();
      repeater.wait_for_finish();
      animation_step = 0.0F;
      repeater.start();
    }

    // --------------------------------------------------------------------------
    template<>
    animated_button_traits<false>::animated_button_traits (button_base& btn)
      : basic_animated_button_traits(btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::native_point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.surface_geometry().is_inside(pos)) {
            start_animation();
            btn.set_checked(!btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state s, os::key_symbol k) {
        if ((s == core::state::none) && ((k == core::keys::enter) || (k == core::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          start_animation();
          btn.set_checked(!btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    animated_button_traits<true>::animated_button_traits (button_base& btn)
      : basic_animated_button_traits(btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::native_point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked() && btn.surface_geometry().is_inside(pos)) {
            btn.set_checked(true);
            start_animation();
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state s, os::key_symbol k) {
        if ((s == core::state::none) && ((k == core::keys::enter) || (k == core::keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked()) {
            btn.set_checked(true);
            start_animation();
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

  } // ctrl

} // gui
