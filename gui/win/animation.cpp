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

    // --------------------------------------------------------------------------
    animation::animation (window& win, std::chrono::milliseconds duration, int fps, float f0, float f1)
      : f0(f0)
      , delta(f1 - f0)
      , duration(duration)
      , repeater(win, std::max(std::chrono::milliseconds(1000) / fps, std::chrono::milliseconds(1))) {
    }

    void animation::start (finish_t fin) {
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
          if (fin) {
            fin();
          }
        }
      });
    }

    void animation::stop () {
      repeater.stop();
      repeater.wait_for_finish();
    }

    void animation::add (const animation_t& a) {
      animations.push_back(a);
    }

    void animation::add (animation_t&& a) {
      animations.push_back(std::move(a));
    }

    void animation::clear () {
      animations.clear();
    }

    // --------------------------------------------------------------------------
    move_animation::move_animation (const window_ptr& win,
                                    const core::point& from,
                                    const core::point& to)
      : win(win)
      , from(from)
      , diff(to - from)
    {}

    void move_animation::operator () (float f) {
      if (win->is_valid()) {
        win->position(from + diff * f);
        win->set_visible(true);
        win->get_parent()->invalidate();
      }
    }

    // --------------------------------------------------------------------------
    size_animation::size_animation (const window_ptr& win,
                                    const core::size& from,
                                    const core::size& to)
      : win(win)
      , from(from)
      , diff(to - from)
    {}

    void size_animation::operator () (float f) {
      if (win->is_valid()) {
        win->resize(from + diff * f);
        win->set_visible(true);
        win->get_parent()->invalidate();
      }
    }

    // --------------------------------------------------------------------------
  } // win

} // gui
