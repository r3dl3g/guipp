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
 * @brief     helper for periodic actions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/background_repeater.h>
#include <gui/win/window_event_proc.h>


namespace gui {

  namespace win {

    background_repeater::background_repeater (window& win, std::chrono::milliseconds delay_ms, action_t action)
      : win(win)
      , delay(delay_ms)
      , action(action)
      , active(false)
    {
      if (delay_ms < std::chrono::milliseconds(1)) {
        throw std::runtime_error("Delay can not be smaller than 1 ms");
      }
    }

    background_repeater::background_repeater (window& win, std::chrono::milliseconds delay_ms)
      : win(win)
      , delay(delay_ms)
      , active(false)
    {
      if (delay_ms < std::chrono::milliseconds(1)) {
        throw std::runtime_error("Delay can not be smaller than 1 ms");
      }
    }

    background_repeater::~background_repeater () {
      wait_for_finish();
    }

    void background_repeater::start (action_t a) {
      action = a;
      start();
    }

    void background_repeater::start () {
      wait_for_finish();
      task = std::thread([&] () {
        active = true;
        while (active) {
          const auto end_time = std::chrono::system_clock::now() + delay;
          win::run_on_main(win, action);
          if (active) {
            std::this_thread::sleep_until(end_time);
          }
        }
      });
    }

    void background_repeater::stop () {
      active = false;
    }

    bool background_repeater::is_active () const {
      return active;
    }

    void background_repeater::set_delay (std::chrono::milliseconds d) {
      delay = d;
    }

    std::chrono::milliseconds background_repeater::get_delay () const {
      return delay;
    }

    void background_repeater::wait_for_finish () {
      active = false;
      if (task.joinable()) {
        task.join();
      }
    }

  } // win

} // gui
