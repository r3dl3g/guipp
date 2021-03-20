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
#include <gui/win/background_repeater.h>
#include <gui/win/window_event_proc.h>


namespace gui {

  namespace win {

    background_repeater::background_repeater (window& win, std::chrono::milliseconds delay_ms, action_t action)
      : win(win)
      , delay(delay_ms)
      , action(action)
      , active(false)
    {}

    background_repeater::~background_repeater () {
      wait_for_finish();
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
