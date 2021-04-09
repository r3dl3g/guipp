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
#include <gui/win/container.h>
#include <gui/win/animation.h>

namespace gui {

  namespace win {

    namespace animation {
      // --------------------------------------------------------------------------
      engine::engine (window& win, std::chrono::milliseconds duration, int fps, float f0, float f1)
        : f0(f0)
        , delta(f1 - f0)
        , duration(duration)
        , repeater(win, std::max(std::chrono::milliseconds(1000) / fps, std::chrono::milliseconds(1)))
        , animation_disabled(false)
      {}

      void engine::start (finish_t fin) {
        if (animation_disabled) {
          for(auto& a : animations) {
            a(f0 + delta);
          }
          clear();
          if (fin) {
            fin();
          }
        } else {
          stop();
          const auto start_time = std::chrono::system_clock::now();
          repeater.start([&, fin, start_time] () {
            const auto now = std::chrono::system_clock::now();
            const float f = f0 + delta * (now - start_time) / duration;
            for(auto& a : animations) {
              a(f);
            }
            if (now >= start_time + duration) {
              repeater.stop();
              clear();
              if (fin) {
                fin();
              }
            }
          });
        }
      }

      void engine::stop () {
        repeater.stop();
        repeater.wait_for_finish();
      }

      void engine::add (const animation_t& a) {
        animations.push_back(a);
      }

      void engine::add (animation_t&& a) {
        animations.push_back(std::move(a));
      }

      void engine::clear () {
        animations.clear();
      }

      void engine::disable_animation (bool disable) {
        animation_disabled = disable;
      }

      // --------------------------------------------------------------------------
      move::move (const window_ptr& win,
                  const core::point& from,
                  const core::point& to)
        : win(win)
        , from(from)
        , diff(to - from)
      {}

      void move::operator () (float f) {
        if (win->is_valid()) {
          win->position(from + diff * f);
          win->set_visible();
          win->get_parent()->invalidate();
        }
      }
      // --------------------------------------------------------------------------
      size::size (const window_ptr& win,
                  const core::size& from,
                  const core::size& to)
        : win(win)
        , from(from)
        , diff(to - from)
      {}

      void size::operator () (float f) {
        if (win->is_valid()) {
          win->resize(from + diff * f);
          win->set_visible();
          win->get_parent()->invalidate();
        }
      }

    } // namespace animation
    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui
