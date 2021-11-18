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
 * @brief     C++ API: basic controls
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Common includes
//
#ifdef GUIPP_X11
# include <X11/cursorfont.h>
#endif // GUIPP_X11

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/splitter.h>
#include <gui/core/guidefs.h>


namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      template<>
      basic_splitter<orientation_t::vertical>::basic_splitter () {
        init();
      }

      template<>
      basic_splitter<orientation_t::vertical>::basic_splitter (const basic_splitter& rhs)
        : super(rhs)
      {
        init();
      }

      template<>
      basic_splitter<orientation_t::vertical>::basic_splitter (basic_splitter&& rhs) noexcept
        : super(std::move(rhs))
      {
        init();
      }

      template<>
      void basic_splitter<orientation_t::vertical>::init () {
        on_mouse_move([&](os::key_state keys,
                          const core::native_point& p) {
          if ((start_mouse_point != core::native_point::undefined) && is_enabled() && core::left_button_bit_mask::is_set(keys)) {
//            logging::debug() << "splitter::mouse_move to:" << p;
            set_value(start_window_point.x() + core::global::scale_from_native<core::size::type>(p.x() - start_mouse_point.x()));
          }
        });
        on_any_key_down([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case core::keys::left:
               set_value(position().x() - (core::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case core::keys::right:
               set_value(position().x() + (core::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case core::keys::home:
               set_value(min);
               break;
             case core::keys::end:
               set_value(max);
               break;
           }
         });
      }

      template<>
      void basic_splitter<orientation_t::vertical>::set_value (core::point::type v) {
        const auto new_x = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_x != pt.x()) {
//          logging::debug() << "splitter::set_value old:" << pt.x() << " new:" << new_x;
          position(core::point(new_x, pt.y()), true);
          super::notify_event(detail::SLIDER_MESSAGE, static_cast<long>(new_x - pt.x()));
        }

      }

      template<>
      core::point::type basic_splitter<orientation_t::vertical>::get_value () const {
        return position().x();
      }

      // --------------------------------------------------------------------------
      template<>
      basic_splitter<orientation_t::horizontal>::basic_splitter () {
        init();
      }

      template<>
      basic_splitter<orientation_t::horizontal>::basic_splitter (const basic_splitter& rhs)
        : super(rhs)
      {
        init();
      }

      template<>
      basic_splitter<orientation_t::horizontal>::basic_splitter (basic_splitter&& rhs) noexcept
        : super(std::move(rhs))
      {
        init();
      }

      template<>
      void basic_splitter<orientation_t::horizontal>::init () {
        on_mouse_move([&](os::key_state keys,
                              const core::native_point& p) {
          if ((start_mouse_point != core::native_point::undefined) && is_enabled() && core::left_button_bit_mask::is_set(keys)) {
//            logging::debug() << "splitter::mouse_move to:" << p;
            set_value(start_window_point.y() + core::global::scale_from_native<core::size::type>(p.y() - start_mouse_point.y()));
          }
        });
        on_any_key_down([&](os::key_state state, os::key_symbol key, const std::string &) {
           switch (key) {
             case core::keys::up:
               set_value(position().y() - (core::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case core::keys::down:
               set_value(position().y() + (core::control_key_bit_mask::is_set(state) ? 10 : 1));
               break;
             case core::keys::home:
               set_value(min);
               break;
             case core::keys::end:
               set_value(max);
               break;
           }
        });
      }

      template<>
      void basic_splitter<orientation_t::horizontal>::set_value (core::point::type v) {
        const auto new_y = std::min<core::point::type>(max, std::max<core::point::type>(min, v));
        const auto pt = position();
        if (new_y != pt.y()) {
//          logging::debug() << "splitter::set_value old:" << pt.y() << " new:" << new_y;
          position(core::point(pt.x(), new_y), true);
          super::notify_event(detail::SLIDER_MESSAGE, static_cast<long>(new_y - pt.y()));
        }

      }

      template<>
      core::point::type basic_splitter<orientation_t::horizontal>::get_value () const {
        return position().y();
      }

      // --------------------------------------------------------------------------
      splitter_base::splitter_base ()
        : min(0)
        , max(std::numeric_limits<type>::max())
      {
        init();
      }

      splitter_base::splitter_base (const splitter_base& rhs)
        : super(rhs)
        , min(rhs.min)
        , max(rhs.max)
        , start_mouse_point(rhs.start_mouse_point)
        , start_window_point(rhs.start_window_point)
      {
        init();
      }

      splitter_base::splitter_base (splitter_base&& rhs) noexcept
        : super(std::move(rhs))
        , min(std::move(rhs.min))
        , max(std::move(rhs.max))
        , start_mouse_point(std::move(rhs.start_mouse_point))
        , start_window_point(std::move(rhs.start_window_point))
      {
        init();
      }

      void splitter_base::init () {
#ifdef GUIPP_X11
        static int initialized = detail::init_control_messages();
        (void)initialized;
#endif // GUIPP_X11

        on_left_btn_down([&] (os::key_state, const core::native_point& pt) {
          capture_pointer();
          start_mouse_point = pt;//client_to_screen(pt);
          start_window_point = position();
//          logging::debug() << "splitter::left_btn_down at:" << pt << " start_mouse_point:" << start_mouse_point << " start_window_point:" << start_window_point;
          take_focus();
        });

        on_left_btn_up([&] (os::key_state, const core::native_point&) {
          uncapture_pointer();
          start_mouse_point = core::native_point::undefined;
          start_window_point = core::point::undefined;
        });
        on_set_focus([&] () {
          invalidate();
        });
        on_lost_focus([&] () {
          invalidate();
        });
      }

      void splitter_base::set_min (type i) {
        if (min != i) {
          min = i;
          invalidate();
        }
      }

      void splitter_base::set_max (type i) {
        if (max != i) {
          max = i;
          invalidate();
        }
      }

      void splitter_base::set_min_max (type mi, type ma) {
        if ((min != mi) || (max != ma)) {
          min = mi;
          max = ma;
          invalidate();
        }
      }

      void splitter_base::on_slide (std::function<void(int)>&& f) {
        on<slider_event>(std::move(f));
      }

    } // detail

  } // ctrl

} // gui
