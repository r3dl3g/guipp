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


// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef X11
# include <X11/cursorfont.h>
#endif // X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/slider.h>
#include <gui/core/guidefs.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation_t::vertical>::basic_slider () {
        init();
      }

      template<>
      basic_slider<orientation_t::vertical>::basic_slider (const basic_slider& rhs)
        : super(rhs)
      {
        init();
      }

      template<>
      basic_slider<orientation_t::vertical>::basic_slider (basic_slider&& rhs)
        : super(std::move(rhs))
      {
        init();
      }

      template<>
      void basic_slider<orientation_t::vertical>::init () {
        on_mouse_move_abs([&](os::key_state keys,
                          const core::point & p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && win::left_button_bit_mask::is_set(keys)) {
            set_value(start_window_point.x() + p.x() - start_mouse_point.x());
          }
        });
        on_any_key_down([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case win::keys::left:
               set_value(position().x() - (win::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case win::keys::right:
               set_value(position().x() + (win::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case win::keys::home:
               set_value(min);
               break;
             case win::keys::end:
               set_value(max);
               break;
           }
         });
      }

      template<>
      void basic_slider<orientation_t::vertical>::set_value (core::point::type v) {
        const auto new_x = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_x != pt.x()) {
//          clog::debug() << "slider::set_value old:" << pt.x() << " new:" << new_x;
          move(core::point(new_x, pt.y()), true);
          super::notify_event(detail::SLIDER_MESSAGE, static_cast<long>(new_x - pt.x()));
        }

      }

      template<>
      core::point::type basic_slider<orientation_t::vertical>::get_value () const {
        return position().x();
      }

      // --------------------------------------------------------------------------
      template<>
      basic_slider<orientation_t::horizontal>::basic_slider () {
        init();
      }

      template<>
      basic_slider<orientation_t::horizontal>::basic_slider (const basic_slider& rhs)
        : super(rhs)
      {
        init();
      }

      template<>
      basic_slider<orientation_t::horizontal>::basic_slider (basic_slider&& rhs)
        : super(std::move(rhs))
      {
        init();
      }

      template<>
      void basic_slider<orientation_t::horizontal>::init () {
        on_mouse_move_abs([&](os::key_state keys,
                          const core::point & p) {
          if ((start_mouse_point != core::point::undefined) && is_enabled() && win::left_button_bit_mask::is_set(keys)) {
            set_value(start_window_point.y() + p.y() - start_mouse_point.y());
          }
          return;
        });
        on_any_key_down([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case win::keys::up:
               set_value(position().y() - (win::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case win::keys::down:
               set_value(position().y() + (win::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case win::keys::home:
               set_value(min);
               break;
             case win::keys::end:
               set_value(max);
               break;
           }
        });
      }

      template<>
      void basic_slider<orientation_t::horizontal>::set_value (core::point::type v) {
        const auto new_y = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_y != pt.y()) {
//          clog::debug() << "slider::set_value old:" << pt.y() << " new:" << new_y;
          move(core::point(pt.x(), new_y), true);
          super::notify_event(detail::SLIDER_MESSAGE, static_cast<long>(new_y - pt.y()));
        }

      }

      template<>
      core::point::type basic_slider<orientation_t::horizontal>::get_value () const {
        return position().y();
      }

      // --------------------------------------------------------------------------
      slider_base::slider_base ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
        init();
      }

      slider_base::slider_base (const slider_base& rhs)
        : super(rhs)
        , min(rhs.min)
        , max(rhs.max)
        , start_mouse_point(rhs.start_mouse_point)
        , start_window_point(rhs.start_window_point)
      {
        init();
      }

      slider_base::slider_base (slider_base&& rhs)
        : super(std::move(rhs))
        , min(std::move(rhs.min))
        , max(std::move(rhs.max))
        , start_mouse_point(std::move(rhs.start_mouse_point))
        , start_window_point(std::move(rhs.start_window_point))
      {
        init();
      }

      void slider_base::init () {
#ifdef X11
        static int initialized = detail::init_control_messages();
        (void)initialized;
#endif // X11

        set_accept_focus(true);

        on_left_btn_down([&] (os::key_state, const core::point& pt) {
          capture_pointer();
          start_mouse_point = client_to_screen(pt);
          start_window_point = position();
          take_focus();
        });

        on_left_btn_up([&] (os::key_state, const core::point&) {
          uncapture_pointer();
          start_mouse_point = core::point::undefined;
          start_window_point = core::point::undefined;
        });
        on_set_focus([&] (win::window*) {
          invalidate();
        });
        on_lost_focus([&] (win::window*) {
          invalidate();
        });
      }

      void slider_base::set_min (type i) {
        if (min != i) {
          min = i;
          invalidate();
        }
      }

      void slider_base::set_max (type i) {
        if (max != i) {
          max = i;
          invalidate();
        }
      }

      void slider_base::set_min_max (type mi, type ma) {
        if ((min != mi) || (max != ma)) {
          min = mi;
          max = ma;
          invalidate();
        }
      }

      void slider_base::on_slide (std::function<void(int)>&& f) {
        on<slider_event>(std::move(f));
      }

    } // detail

  } // ctrl

} // gui
